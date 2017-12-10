-include config.mk

PLATFORMS      = linux windows wasm linux_release
.PHONY:          $(PLATFORMS) dirs
.DEFAULT_GOAL := linux

MAKEFLAGS+="-j $(shell nproc)"


build/mapper.bc: lib/wdc65816/mapper_helper.c
	clang-6.0 -Ilib/ -c -emit-llvm -o $@ lib/wdc65816/mapper_helper.c

build/%/:
	@mkdir -p $@

build/gen/mapper_bc.h: build/gen/ build/mapper.bc
	xxd -i build/mapper.bc > $@

build/%/libbase.o:
	$(CC) $(CFLAGS) -Ilib/ -o $@ -xc -DBASE_IMPLEMENTATION -c lib/base/base.h

build/%/libwdc65816.o: build/gen/mapper_bc.h
	$(CC) $(CFLAGS) -Ilib/ -Ibuild/gen/ $(LLVMINC) -o $@ -c lib/wdc65816/build.c
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
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(LLVMLIBS) $(LLVMLIBS) $(LLVMLIBS)

build/%/imockup: build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libmockup.o build/%/imockup.o build/%/libnuklear.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(LLVMLIBS) $(LLVMLIBS) $(LLVMLIBS) $(GLLIBS)

build/%/nuts:    build/%/libbase.o build/%/libwdc65816.o build/%/libwdc65816_run.o build/%/libnuts.o build/%/nuts.o
	$(CC) $(LFLAGS) -o $@$(EXT) $^ $(LLVMLIBS) $(LLVMLIBS) $(LLVMLIBS)

-include build/*.d
-include build/linux/*.d
-include build/windows/*.d


TARGETS = build/$$@/ build/$$@/lmockup build/$$@/imockup build/$$@/nuts
windows:       CC       = $(CCWINDOWS)
windows:       LD       = $(LDWINDOWS)
windows:       CFLAGS   = -g -Wall -Werror -MMD -MP
windows:       GLLIBS   = -lglew32 -lglfw3 -lopengl32 -lmingw32 -mwindows     \
                          -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8  \
                          -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
                          -lshell32 -lversion -luuid -static-libgcc
windows:       LLVMLIBS = -lLLVMAArch64AsmParser -lLLVMAArch64AsmPrinter \
                          -lLLVMAArch64CodeGen -lLLVMAArch64Desc -lLLVMAArch64Disassembler \
                          -lLLVMAArch64Info -lLLVMAArch64Utils -lLLVMAnalysis -lLLVMARMAsmParser \
                          -lLLVMARMAsmPrinter -lLLVMARMCodeGen -lLLVMARMDesc -lLLVMARMDisassembler \
                          -lLLVMARMInfo -lLLVMAsmParser -lLLVMAsmPrinter -lLLVMBitReader \
                          -lLLVMBitWriter -lLLVMCodeGen -lLLVMCore -lLLVMCppBackendCodeGen \
                          -lLLVMCppBackendInfo -lLLVMDebugInfo -lLLVMExecutionEngine \
                          -lLLVMHexagonAsmPrinter -lLLVMHexagonCodeGen -lLLVMHexagonDesc \
                          -lLLVMHexagonInfo -lLLVMInstCombine -lLLVMInstrumentation \
                          -lLLVMInterpreter -lLLVMipa -lLLVMipo -lLLVMIRReader \
                          -lLLVMJIT -lLLVMLinker -lLLVMLTO \
                          -lLLVMMC -lLLVMMCDisassembler -lLLVMMCJIT \
                          -lLLVMMCParser -lLLVMMipsAsmParser -lLLVMMipsAsmPrinter \
                          -lLLVMMipsCodeGen -lLLVMMipsDesc -lLLVMMipsDisassembler \
                          -lLLVMMipsInfo -lLLVMMSP430AsmPrinter -lLLVMMSP430CodeGen \
                          -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMNVPTXAsmPrinter \
                          -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo \
                          -lLLVMObjCARCOpts -lLLVMObject -lLLVMOption \
                          -lLLVMPowerPCAsmParser -lLLVMPowerPCAsmPrinter -lLLVMPowerPCCodeGen \
                          -lLLVMPowerPCDesc -lLLVMPowerPCInfo -lLLVMR600AsmPrinter \
                          -lLLVMR600CodeGen -lLLVMR600Desc -lLLVMR600Info \
                          -lLLVMRuntimeDyld -lLLVMScalarOpts -lLLVMSelectionDAG \
                          -lLLVMSparcCodeGen -lLLVMSparcDesc -lLLVMSparcInfo \
                          -lLLVMSupport -lLLVMSystemZAsmParser -lLLVMSystemZAsmPrinter \
                          -lLLVMSystemZCodeGen -lLLVMSystemZDesc -lLLVMSystemZDisassembler \
                          -lLLVMSystemZInfo -lLLVMTableGen -lLLVMTarget \
                          -lLLVMTransformUtils -lLLVMVectorize -lLLVMX86AsmParser \
                          -lLLVMX86AsmPrinter -lLLVMX86CodeGen -lLLVMX86Desc \
                          -lLLVMX86Disassembler -lLLVMX86Info -lLLVMX86Utils \
                          -lLLVMXCoreAsmPrinter -lLLVMXCoreCodeGen -lLLVMXCoreDesc \
                          -lLLVMXCoreDisassembler -lLLVMXCoreInfo
windows:       EXT      = .exe
windows:       build/windows/ build/windows/lmockup build/windows/imockup build/windows/nuts

wasm:          CC       = $(CCWASM)
wasm:          CFLAGS   = -s WASM=1 -O2 -DNO_COMPUTED_GOTO
wasm:          LFLAGS   = -s "BINARYEN_METHOD='native-wasm'" -v
wasm:          EXT      = .js
wasm:          build/wasm/ build/wasm/lmockup build/wasm/imockup build/wasm/nuts

linux:         CC       = $(CCLINUX)
linux:         LD       = $(LDLINUX)
linux:         CFLAGS   = -g -Wall -Werror -MMD -MP
linux:         GLLIBS   = -lGLEW -lGL -lglfw
linux:         LLVMLIBS = -lLLVM-5.0
linux:         LLVMINC  = -I/usr/lib/llvm-5.0/include/
linux:         build/linux/ build/linux/lmockup build/linux/imockup build/linux/nuts

linux_release: CFLAGS   = -Ofast -march=native -Wall -Werror -MMD -MP
linux_release: GLLIBS   = -lGLEW -lGL -lglfw
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
