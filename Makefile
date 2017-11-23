
GLLIBS         = -lGLEW -lGL -lglfw

TARGETS        = linux windows wasm linux_release
.PHONY:        $(TARGETS) dirs
.DEFAULT_GOAL := linux

MAKEFLAGS+="-j $(shell nproc)"

dirs:
	@mkdir -p build/linux build/windows build/wasm

build/mapper.bc:
	clang-6.0 $(CFLAGS) -Ilib/ -c -emit-llvm -o $@ lib/wdc65816/mapper_helper.c

build/%/libbase.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -xc -DBASE_IMPLEMENTATION -c lib/base/base.h

build/%/libwdc65816.o: build/mapper.bc
	$(CC) $(CFLAGS) -Ilib/ -I/usr/lib/llvm-6.0/include -o $@ -c lib/wdc65816/build.c
build/%/libwdc65816_run.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/wdc65816/build_run.c

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
	$(CC) $(LFLAGS) -o $@$(EXT) $^ -lLLVM-6.0

build/%/imockup: build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/imockup.o build/%/libnuklear.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ -lLLVM-6.0 $(GLLIBS)

build/%/nuts:    build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libnuts.o build/%/nuts.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ -lLLVM-6.0

-include build/*.d
-include build/linux/*.d
-include build/windows/*.d

windows: CC      =../mxe/usr/bin/i686-w64-mingw32.static-gcc
windows: GLLIBS  =-lglew32 -lglfw3 -lopengl32 -lmingw32 -mwindows     \
                  -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8  \
                  -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
                  -lshell32 -lversion -luuid -static-libgcc
windows: EXT     =.exe

wasm:    CC      =../emscripten/emcc
wasm:    CFLAGS  =-s WASM=1 -O2 -DNO_COMPUTED_GOTO
wasm:    LFLAGS  =-s "BINARYEN_METHOD='native-wasm'" -v
wasm:    EXT     =.js
windows: CFLAGS  = -g -Wall -Werror -MMD -MP
windows: dirs build/windows/lmockup build/windows/imockup build/windows/nuts
linux:   CFLAGS  = -g -Wall -Werror -MMD -MP
linux:   dirs build/linux/lmockup   build/linux/imockup   build/linux/nuts
wasm:    dirs build/wasm/lmockup    build/wasm/imockup    build/wasm/nuts

linux_release: CFLAGS  = -O3 -Wall -Werror -MMD -MP
linux_release: dirs build/linux/lmockup   build/linux/imockup   build/linux/nuts


# all:
# 	gcc -g3 -O0 -mno-avx -Werror -I../../lib nuts.c -o nuts

# macro_debug:
# 	gcc -g -E -P -Werror -I../../lib nuts.c | clang-format > nuts.pp.c
# 	gcc -g -O0 -mno-avx -Werror -fpreprocessed -I../../lib nuts.pp.c -o nuts

# release:
# 	gcc -g -O3 -Werror -march=native -I../../lib nuts.c -o nuts

# profile:
# 	gcc -g -fprofile-generate -O3 -Werror  -I../../lib nuts.c -o nuts

# profile_use:
# 	gcc -g -fprofile-use -O3 -Werror -I../../lib nuts.c -o nuts
