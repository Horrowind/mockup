-include config.mk

PLATFORMS      = linux windows wasm linux_release clean
.PHONY:          $(PLATFORMS)
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
build/%/libwdc65816_llvm.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/wdc65816/build_llvm.c

build/%/libnuts.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/nuts/nuts.c

build/%/libmockup.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/mockup/build.c

build/%/libnuklear.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/nuklear/nuklear.c

build/%/lmockup.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c app/mockup_light/main.c

build/%/imockup.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c app/mockup/main.c

build/%/nuts.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c app/nuts/main.c

build/%/lmockup: build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/lmockup.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(LIBS)

build/%/imockup: build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/imockup.o build/%/libnuklear.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(GLLIBS) $(LIBS) 

build/%/nuts:    build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libnuts.o build/%/nuts.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(LIBS)

-include build/*.d
-include build/linux/*.d
-include build/windows/*.d
-include build/wasm/*.d

windows:       CC       = $(CCWINDOWS)
windows:       CFLAGS   = -g -Wall -Werror -MMD -MP 
windows:       GLLIBS   =  -lglew32 -lglfw3 -lopengl32 
windows:       LIBS     = -lmingw32 -mwindows     \
                          -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8  \
                          -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
                          -lshell32 -lversion -luuid -static-libgcc -lshlwapi
windows:       EXT      = .exe
windows:       build/windows/ build/windows/lmockup build/windows/imockup build/windows/nuts

wasm:          CC       = $(CCWASM)
wasm:          CFLAGS   = -O2 -DNO_COMPUTED_GOTO
wasm:          LFLAGS   = -s WASM=1 -s "BINARYEN_METHOD='native-wasm'" -s USE_GLFW=3 -O2 --preload_file=SuperMarioWorld.sfc

wasm:          EXT      = .html
wasm:          build/wasm/ build/wasm/lmockup build/wasm/imockup build/wasm/nuts

linux:         CC       = $(CCLINUX)
linux:         CFLAGS   = -g -Wall -Werror -MMD -MP
linux:         GLLIBS   = -lGLEW -lGL -lglfw
linux:         LFLAGS   =
linux:         build/linux/ build/linux/lmockup build/linux/imockup build/linux/nuts

linux_release: CC       = $(CCLINUX)
linux_release: CFLAGS   = -O3 -Wall -Werror -MMD -MP
linux_release: GLLIBS   = -lGLEW -lGL -lglfw
linux_release: EXT      =
linux_release: build/linux_release/ build/linux_release/lmockup build/linux_release/imockup build/linux_release/nuts

clean:
	rm -r build

# macro_debug:
# 	gcc -g -E -P -Werror -I../../lib nuts.c | clang-format > nuts.pp.c
# 	gcc -g -O0 -mno-avx -Werror -fpreprocessed -I../../lib nuts.pp.c -o nuts

# profile:
# 	gcc -g -fprofile-generate -O3 -Werror  -I../../lib nuts.c -o nuts

# profile_use:
# 	gcc -g -fprofile-use -O3 -Werror -I../../lib nuts.c -o nuts
