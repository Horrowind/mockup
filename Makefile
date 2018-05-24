-include config.mk

PLATFORMS      = linux windows wasm linux_release
TARGETS        = $(PLATFORMS) static_analysis
.PHONY:          $(TARGETS) all clean release
.DEFAULT_GOAL := linux

MAKEFLAGS+="-j $(shell nproc)"

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

build/%/nuts-fmt.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c app/nuts-fmt/main.c

build/%/lmockup:  build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/lmockup.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(LIBS)

build/%/imockup:  build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/imockup.o build/%/libnuklear.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(GLLIBS) $(LIBS) 

build/%/imockup_js:  build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/imockup.o build/%/libnuklear_js.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(GLLIBS) $(LIBS) 

build/%/nuts:     build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libnuts.o build/%/nuts.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(LIBS)

-include build/linux/*.d
-include build/linux_release/*.d
-include build/windows/*.d
-include build/wasm/*.d
-include build/static_analysis/*.d


windows:         CC       = $(CCWINDOWS)
windows:         CFLAGS   = -g -Wall -Werror -MMD -MP -DWINDOWSsy
windows:         GLLIBS   = -lglew32 -lglfw3 -lopengl32 
windows:         LIBS     = -lmingw32 -mwindows     \
                          -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8  \
                          -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
                          -lshell32 -lversion -luuid -static-libgcc -lshlwapi
windows:         EXT      = .exe

wasm:            CC       = $(CCWASM)
wasm:            CFLAGS   = -O2 -Wall -Werror --target=wasm32-unknown-unknown-wasm -MMD -MP -DNO_COMPUTED_GOTO -DWASM
wasm:            LFLAGS   = --target=wasm32-unknown-unknown-wasm -nostdlib
wasm:            EXT      = .wasm

linux:           CC       = $(CCLINUX)
linux:           CFLAGS   = -g -Wall -Werror -Wunused-result -MMD -MP -DLINUX
linux:           GLLIBS   = -lGLEW -lGL -lglfw

linux_release:   CC       = $(CCLINUX)
linux_release:   CFLAGS   = -O3 -g -Wall -Werror -MMD -MP -DNDEBUG -DLINUX
linux_release:   GLLIBS   = -lGLEW -lGL -lglfw

STATIC_ANALYSIS           = scan-build-6.0 --status-bugs gcc
static_analysis: CC       = $(STATIC_ANALYSIS)
static_analysis: CFLAGS   = -g -Wall -Werror -Wunused-result -MMD -MP
static_analysis: GLLIBS   = -lGLEW -lGL -lglfw

$(TARGETS): %: build/%/ build/%/lmockup build/%/imockup build/%/nuts
all: windows wasm linux

# TODO: add release modes for windows and wasm
all_release: windows_release wasm_release linux_release


clean:
	rm -r build

upload:
	curl -H "Authorization: Bearer $(KEY)" -F "mockup.html=@html/mockup.html" https://neocities.org/api/upload

# macro_debug:
# 	gcc -g -E -P -Werror -I../../lib nuts.c | clang-format > nuts.pp.c
# 	gcc -g -O0 -mno-avx -Werror -fpreprocessed -I../../lib nuts.pp.c -o nuts

# profile:
# 	gcc -g -fprofile-generate -O3 -Werror  -I../../lib nuts.c -o nuts

# profile_use:
# 	gcc -g -fprofile-use -O3 -Werror -I../../lib nuts.c -o nuts
