-include config.mk

PLATFORMS      = linux windows wasm linux_release
EXECUTABLE     = linux linux_release windows windows_release #wasm wasm_release
TARGETS        = $(PLATFORMS) static_analysis
.PHONY:          $(TARGETS) all clean release test
.DEFAULT_GOAL := linux

#MAKEFLAGS+="-j $(shell nproc)"

build/%/:
	mkdir -p $@

build/%/libbase.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/base/base.c

build/%/libbase_gui.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c -DBASE_GUI lib/base/base.c

build/%/libwdc65816.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/wdc65816/build.c

build/%/libwdc65816_run.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/wdc65816/build_run.c

build/%/libnuts.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/nuts/nuts.c

build/%/libmockup.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/mockup/build.c

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

build/%/lmockup$(EXT):   build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/lmockup.o
	$(LD) $(LFLAGS) -o $@$(EXT) $^ $(LIBS)

build/%/imockup$(EXT):   build/%/libbase_gui.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/imockup.o build/%/libnuts.o
	$(LD) $(LFLAGS) -o $@$(EXT) $^ $(GLLIBS) $(LIBS) 

#build/%/imockup_js.wasm: build/%/libbase_gui.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/imockup.o build/%/libnuts.o
#	$(LD) $(LFLAGS) -o $@ $^

build/%/nuts$(EXT):      build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libnuts.o build/%/nuts.o
	$(LD) $(LFLAGS) -o $@$(EXT) $^  $(LIBS)

build/%/rat$(EXT):       build/%/rat.o build/%/libbase.o
	$(LD) $(LFLAGS) -o $@$(EXT) $^ $(LIBS)

build/%/gui_test.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c app/gui_test/main.c

build/%/gui_test$(EXT):  build/%/gui_test.o build/%/libbase_gui.o
	$(LD) $(LFLAGS) -o $@$(EXT) $^ $(GLLIBS) $(LIBS)

build/%/test/bml:
	$(CC) $(CFLAGS) $(LFLAGS) -Ilib/ -o $@$(EXT) $(LIBS) test/bml/main.c
	-$@$(EXT)
build/%/test/freelist:
	$(CC) $(CFLAGS) $(LFLAGS) -Ilib/ -o $@$(EXT) $(LIBS) test/freelist/main.c
	-$@$(EXT)

build/font_baker: app/font_baker/main.c
	gcc app/font_baker/main.c -o build/font_baker -lm
lib/base/font.c: build/font_baker
	build/font_baker data/liberation_sans_regular.ttf liberation_sans_regular > $@

#.SECONDARY: build/*/*.o
-include build/linux/*.d
-include build/linux_release/*.d
-include build/windows/*.d
-include build/wasm/*.d
#-include build/static_analysis/*.d


windows:         CC       = $(CC_WINDOWS)
windows:         LD       = $(LD_WINDOWS)
windows:         CFLAGS   = -g -Wall -Werror  -MMD -MP -DWINDOWS -DARCH32 -DGCC  #-fno-builtin -fno-tree-loop-distribute-patterns -nostdlib -nodefaultlibs -mrtd #-fno-leading-underscore
windows:         LFLAGS   = #-mrtd -lmsvcrt80
windows:         GLLIBS   = -lopengl32 
windows:         LIBS     = -lmingw32 -mwindows -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8   \
                            -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 		     \
                            -lshell32 -lversion -luuid -static-libgcc -lshlwapi -lkernel32 -lmsvcr90
windows:         EXT      = .exe

wasm:            CC       = $(CC_WASM)
wasm:            LD       = $(LD_WASM)
wasm:            CFLAGS   = -O0 -g3 -Wall -Werror --target=wasm32-unknown-unknown-wasm -MMD -MP -DNO_COMPUTED_GOTO -DWASM -DARCH32 -DCLANG
#wasm:            LFLAGS   = --target=wasm32-unknown-unknown-wasm -nostdlib --no-entry
wasm:            LFLAGS   = --no-threads --no-entry --export init --export update --export deinit -allow-undefined-file data/wasm.syms --import-memory --no-gc-sections #-z stack-size=1572864 
wasm:            EXT      = .wasm
wasm:            GLLIBS   =
wasm:            LIBS     =
wasm:            build/wasm/ build/wasm/gui_test
	tools/wasm-sourcemap.py --dwarfdump "/usr/bin/llvm-dwarfdump-8" --prefix $(pwd)=wasm:/// -s -o html/binary.wasm.map -x -u http://localhost:8080/binary.wasm.map -w html/binary.wasm build/wasm/gui_test.wasm

wasm_release:    CC       = $(CC_WASM)
wasm_release:    LD       = $(LD_WASM)
wasm_release:    CFLAGS   = -O2 -Wall -Werror --target=wasm32-unknown-unknown-wasm -MMD -MP -DNO_COMPUTED_GOTO -DWASM -DARCH32 -DCLANG
wasm_release:    LFLAGS   = --no-entry --strip-all -allow-undefined-file data/wasm.syms --import-memory -error-limit=0 #-z stack-size=1572864  
wasm_release:    EXT      = .wasm
wasm_release:
	cp build/wasm_release/gui_test.wasm html/binary.wasm
	zopfli html/binary.wasm


linux:           CC       = $(CC_LINUX)
linux:           LD       = $(LD_LINUX)
ifeq ($(COMPILER_LINUX),GCC)
linux:           CFLAGS   = -g3 -Wall -Wextra -Werror -Wunused-result -MMD -MP -DLINUX -DARCH64 -DGCC
linux:           LFLAGS   = 
endif
ifeq ($(COMPILER_LINUX),TCC)
linux:           CFLAGS   = -g3 -Wall -Wextra -Werror -Wunused-result -DLINUX -DARCH64 -DTCC
linux:           LFLAGS   = lib/base/platform_linux_syscall.s
endif
ifeq ($(COMPILER_LINUX),CLANG)
linux:           CFLAGS   = -g3 -Wall -Wextra -Werror -Wunused-result -DLINUX -DARCH64 -DCLANG
linux:           LFLAGS   = 
endif
linux:           GLLIBS   = -lGL -lX11 -ldl -lm -lc
linux:           LIBS     =

linux_release:   CC       = $(CC_LINUX)
linux_release:   LD       = $(LD_LINUX)
linux_release:   CFLAGS   = -O3 -Wall -Wextra -Werror -Wunused-result -MMD -MP -DNDEBUG -DLINUX -DARCH64 -D$(COMPILER_LINUX)
linux_release:   GLLIBS   = -lGLEW -lGL -lX11 -ldl -lm -lc

#STATIC_ANALYSIS           = scan-build-6.0 --status-bugs gcc
#static_analysis: CC       = $(STATIC_ANALYSIS)
#static_analysis: CFLAGS   = -g -Wall -Werror -Wunused-result -MMD -MP -DLINUX
#static_analysis: GLLIBS   = -lGLEW -lGL -lglfw

$(EXECUTABLE): %: build/%/ build/%/gui_test$(EXT) build/%/nuts$(EXT) build/%/rat$(EXT) build/%/lmockup build/%/imockup 

all: windows wasm linux

# TODO: add release modes for windows and wasm
release: linux_release #windows_release wasm_release 


clean:
	rm -r build

upload:
	curl -H "Authorization: Bearer $(KEY)" -F "mockup.html=@html/mockup.html" https://neocities.org/api/upload


.PRECIOUS: build/linux/test/bml
test:            CC       = $(CC_LINUX)
test:            CFLAGS   = -g3 -Wall -Wextra -Werror -Wunused-result -Wno-sign-compare -MMD -MP -DLINUX -DARCH64 -DGCC
test: build/linux/test/ build/linux/test/bml build/linux/test/freelist
# macro_debug:
# 	gcc -g -E -P -Werror -I../../lib nuts.c | clang-format > nuts.pp.c
# 	gcc -g -O0 -mno-avx -Werror -fpreprocessed -I../../lib nuts.pp.c -o nuts

# profile:
# 	gcc -g -fprofile-generate -O3 -Werror  -I../../lib nuts.c -o nuts

# profile_use:
# 	gcc -g -fprofile-use -O3 -Werror -I../../lib nuts.c -o nuts

.SUFFIXES:
MAKEFLAGS += --no-builtin-rules
