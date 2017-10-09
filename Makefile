CFLAGS       := -g -Wall -Werror -O3 -MMD -MP

GLLIBS       = -lGLEW -lGL -lglfw

TARGETS = linux windows wasm
.PHONY:  $(TARGETS) dirs
.DEFAULT_GOAL := linux

dirs:
	@mkdir -p build/linux build/windows build/wasm

build/mapper.bc:
	clang-5.0 $(CFLAGS) -Ilib/ -c -emit-llvm -o $@ lib/r65816/mapper_helper.c

build/%/libbase.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -xc -DBASE_IMPLEMENTATION -c lib/base/base.h

build/%/libr65816.o: build/mapper.bc
	$(CC) $(CFLAGS) -Ilib/ -I/usr/lib/llvm-6.0/include -o $@ -c lib/r65816/build.c
build/%/libr65816_run.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/r65816/build_run.c

build/%/libmockup.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/mockup/build.c

build/%/libnuklear.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c lib/nuklear/nuklear.c

build/%/lmockup.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c app/mockup_light/main.c

build/%/imockup.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -c app/mockup/main.c

build/%/lmockup: build/%/libbase.o build/%/libr65816.o build/%/libr65816_run.o build/%/libmockup.o build/%/lmockup.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ -lLLVM-6.0

build/%/imockup: build/%/libbase.o build/%/libr65816.o build/%/libr65816_run.o build/%/libmockup.o build/%/imockup.o build/%/libnuklear.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ -lLLVM-6.0 $(GLLIBS)

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
windows: dirs build/windows/lmockup build/windows/imockup
linux:   dirs build/linux/lmockup build/linux/imockup
wasm:    dirs build/wasm/lmockup    build/wasm/imockup

