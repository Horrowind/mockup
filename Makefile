-include config.mk

PLATFORMS      = linux windows wasm linux_release
EXECUTABLE     = linux windows linux_release
TARGETS        = $(PLATFORMS) static_analysis
.PHONY:          $(TARGETS) all clean release test
.DEFAULT_GOAL := linux

#MAKEFLAGS+="-j $(shell nproc)"

build/%/:
	mkdir -p $@

build/%/libbase.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/base/base.c

build/%/libwdc65816.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/wdc65816/build.c

build/%/libwdc65816_run.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/wdc65816/build_run.c

build/%/libnuts.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/nuts/nuts.c

build/%/libmockup.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/mockup/build.c

build/%/libnuklear.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/nuklear/nuklear.c

build/%/libnuklear_js.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/nuklear/nuklear_js.c

build/%/lmockup.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c app/mockup_light/main.c

build/%/imockup.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c app/mockup/main.c

build/%/nuts.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c app/nuts/main.c

build/%/nuts-fmt.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c app/nuts-fmt/main.c

build/%/rat.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c app/rat/main.c

build/%/lmockup:    build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/lmockup.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(LIBS)

build/%/imockup:    build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/imockup.o build/%/libnuklear.o build/%/libnuts.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(GLLIBS) $(LIBS) 

build/%/imockup_js.wasm: build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/imockup.o build/%/libnuklear.o build/%/libnuts.o
	$(LD) $(LFLAGS) -o $@$(EXT) $^ $(GLLIBS) $(LIBS)

build/%/nuts:       build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libnuts.o build/%/nuts.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(LIBS)

build/%/rat:        build/%/rat.o build/%/libbase.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(LIBS)

build/%/test/bml:
	$(CC) $(CFLAGS) $(LFLAGS) -Ilib/ -o $@$(EXT) $(LIBS) test/bml/main.c
	-$@$(EXT)

#.SECONDARY: build/*/*.o
-include build/linux/*.d
-include build/linux_release/*.d
-include build/windows/*.d
-include build/wasm/*.d
#-include build/static_analysis/*.d


windows:         CC       = $(CC_WINDOWS)
windows:         CFLAGS   = -g -Wall -Werror -MMD -MP -DWINDOWS -DARCH32
windows:         GLLIBS   = -lglew32 -lglfw3 -lopengl32 
windows:         LIBS     = -lmingw32 -mwindows     \
                          -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8  \
                          -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
                          -lshell32 -lversion -luuid -static-libgcc -lshlwapi
windows:         EXT      = .exe

wasm:            CC       = $(CC_WASM)
wasm:            LD       = $(LD_WASM)
wasm:            CFLAGS   = -O2 -Wall -Werror --target=wasm32-unknown-unknown-wasm -MMD -MP -DNO_COMPUTED_GOTO -DWASM -DARCH32 -DCLANG
#wasm:            LFLAGS   = --target=wasm32-unknown-unknown-wasm -nostdlib --no-entry
wasm:            LFLAGS   = --no-entry --strip-all -allow-undefined-file wasm.syms --import-memory -z stack-size=1572864
wasm:            EXT      = .wasm
wasm:            build/wasm/ build/wasm/imockup_js.wasm

linux:           CC       = $(CC_LINUX)
linux:           CFLAGS   = -g3 -Wall -Wextra -Werror -Wunused-result -Wno-sign-compare -MMD -MP -DLINUX -DARCH64 -DGCC
linux:           GLLIBS   = -lGLEW -lGL -lglfw

linux_release:   CC       = $(CC_LINUX)
linux_release:   CFLAGS   = -O3 -Wall -Wextra -Werror -MMD -MP -DNDEBUG -DLINUX -DARCH64 -DGCC
linux_release:   GLLIBS   = -lGLEW -lGL -lglfw

#STATIC_ANALYSIS           = scan-build-6.0 --status-bugs gcc
#static_analysis: CC       = $(STATIC_ANALYSIS)
#static_analysis: CFLAGS   = -g -Wall -Werror -Wunused-result -MMD -MP -DLINUX
#static_analysis: GLLIBS   = -lGLEW -lGL -lglfw

$(EXECUTABLE): %: build/%/ build/%/lmockup build/%/imockup build/%/nuts build/%/rat

all: windows wasm linux

# TODO: add release modes for windows and wasm
release: linux_release #windows_release wasm_release 


clean:
	rm -r build

upload:
	curl -H "Authorization: Bearer $(KEY)" -F "mockup.html=@html/mockup.html" https://neocities.org/api/upload


.PRECIOUS: build/linux/test/bml
test:            CC       = $(CC_LINUX)
test:            CFLAGS   = $(CFLAGS_LINUX)
test:            GLLIBS   = -lGLEW -lGL -lglfw
test: build/linux/test/ build/linux/test/bml
# macro_debug:
# 	gcc -g -E -P -Werror -I../../lib nuts.c | clang-format > nuts.pp.c
# 	gcc -g -O0 -mno-avx -Werror -fpreprocessed -I../../lib nuts.pp.c -o nuts

# profile:
# 	gcc -g -fprofile-generate -O3 -Werror  -I../../lib nuts.c -o nuts

# profile_use:
# 	gcc -g -fprofile-use -O3 -Werror -I../../lib nuts.c -o nuts

.SUFFIXES:
MAKEFLAGS += --no-builtin-rules
