#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitReader.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/OrcBindings.h>
#include <llvm-c/Transforms/PassManagerBuilder.h>
#include "mapper.h"

void wdc65816_mapper_add(wdc65816_mapper_t* mapper, wdc65816_mapper_entry_t* entry) {
    if(mapper->num_entries >= 255) return;
    mapper->entries[mapper->num_entries] = *entry;
    mapper->num_entries++;
}


u8 wdc65816_mapper_read(wdc65816_mapper_t* mapper, u32 addr) {
    u8* data_ptr = wdc65816_mapper_ptr(mapper, addr);
    if(data_ptr) return *data_ptr;
    return 0;
}

void wdc65816_mapper_write(wdc65816_mapper_t* mapper, u32 addr, u8 data) {
    u8* data_ptr = wdc65816_mapper_ptr(mapper, addr);
    if(data_ptr) *data_ptr = data;
}

u8* wdc65816_mapper_ptr(wdc65816_mapper_t* mapper, u32 full_addr) {
    u8 bank  = full_addr >> 16;
    u16 addr = full_addr & 0xFFFF;
    
    for(int i = 0; i < mapper->num_entries; i++) {
        wdc65816_mapper_entry_t* entry = &mapper->entries[i];
        if(entry->bank_low <= bank && bank <= entry->bank_high
           && entry->addr_low <= addr && addr <= entry->addr_high) {
            u32 offset = wdc65816_mapper_reduce(full_addr, entry->mask);
            if(entry->size) offset = entry->base + wdc65816_mapper_mirror(offset, entry->size - entry->base);
            return entry->data + offset;
        }
    }
    return 0;
}

static
u64 symbol_resolver(const char* name, void* ctx) {
    wdc65816_mapper_t* mapper = ctx;
    uint i;
    sscanf(name, "data%u", &i);
    if(i < mapper->num_entries) {
        return (uint64_t)mapper->entries[i].data ? (uint64_t)mapper->entries[i].data : 1;
    }
    return 0;
}


void wdc65816_mapper_init_functions(wdc65816_mapper_t* mapper) {
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMLinkInMCJIT();


    
#if 0
    // Sort mapper entries by size, so the entries which map large parts of the
    // bus are evaluated first.
    wdc65816_mapper_t mapper_cpy = *mapper;
    mapper = &mapper_cpy;
    for(int i = 0; i < mapper->num_entries - 1; i++) {
        int bank_i = mapper->entries[i].bank_high - mapper->entries[i].bank_low + 1;
        int addr_i = mapper->entries[i].addr_high - mapper->entries[i].addr_low + 1;
        int size_i = bank_i * addr_i;
        for(int j = 1; j < mapper->num_entries; j++) {
            int bank_j = mapper->entries[j].bank_high - mapper->entries[j].bank_low + 1;
            int addr_j = mapper->entries[j].addr_high - mapper->entries[j].addr_low + 1;
            int size_j = bank_j * addr_j;
            if(size_j > size_i) {
                wdc65816_mapper_entry_t tmp = mapper->entries[j];
                mapper->entries[j] = mapper->entries[i];
                mapper->entries[i] = tmp;
            }
        }
    }
#endif
    
    LLVMMemoryBufferRef mapper_objectmem_buf;
    LLVMCreateMemoryBufferWithContentsOfFile("build/mapper.bc", &mapper_objectmem_buf, NULL);

    LLVMContextRef context = LLVMContextCreate();
    
    LLVMModuleRef mapper_module;
    LLVMParseBitcodeInContext2(context, mapper_objectmem_buf, &mapper_module);

    LLVMTypeRef lint8     = LLVMInt8TypeInContext(context);
    LLVMTypeRef lint32    = LLVMInt32TypeInContext(context);
    LLVMTypeRef lint8_ptr = LLVMPointerType(lint8, 0);
    LLVMTypeRef lvoid     = LLVMVoidTypeInContext(context);
    
    LLVMTypeRef read_func_type  = LLVMFunctionType(lint8,     (LLVMTypeRef[]){ lint32        }, 1, 0);
    LLVMTypeRef write_func_type = LLVMFunctionType(lvoid,     (LLVMTypeRef[]){ lint32, lint8 }, 2, 0);
    LLVMTypeRef ptr_func_type   = LLVMFunctionType(lint8_ptr, (LLVMTypeRef[]){ lint32        }, 1, 0);

    LLVMValueRef data_ptr[256];
    for(int i = 0; i < mapper->num_entries; i++) {
        char data_name[8];
        sprintf(data_name, "data%i", i);
        data_ptr[i] = LLVMAddGlobal(mapper_module, lint8, data_name);
    }
    
    LLVMValueRef mapper_functions[] = {
        LLVMAddFunction(mapper_module, "mapper_read",  read_func_type),
        LLVMAddFunction(mapper_module, "mapper_write", write_func_type),
        LLVMAddFunction(mapper_module, "mapper_ptr",   ptr_func_type)
    };

    LLVMValueRef reduce_function = LLVMGetNamedFunction(mapper_module, "wdc65816_mapper_reduce");
    LLVMValueRef mirror_function = LLVMGetNamedFunction(mapper_module, "wdc65816_mapper_mirror");

    LLVMBuilderRef builder  = LLVMCreateBuilderInContext(context);
    
    for(int i = 0; i < 3; i++) {
        LLVMBasicBlockRef entry  = LLVMAppendBasicBlockInContext(context, mapper_functions[i],  "entry");
        LLVMPositionBuilderAtEnd(builder, entry);

        LLVMValueRef bank = LLVMBuildLShr(builder,
                                          LLVMGetParam(mapper_functions[i], 0),
                                          LLVMConstInt(lint32, 16, 1) ,
                                          "bank");
        LLVMValueRef addr = LLVMBuildAnd(builder,
                                         LLVMGetParam(mapper_functions[i], 0),
                                         LLVMConstInt(lint32, 0xFFFF, 1) ,
                                         "addr");
    
        LLVMBasicBlockRef if_statements[mapper->num_entries + 1];
        LLVMBasicBlockRef branches[mapper->num_entries];
        for(int j = 0; j < mapper->num_entries; j++) {
            if_statements[j] = LLVMAppendBasicBlockInContext(context, mapper_functions[i], "if1");
            branches[j] = LLVMAppendBasicBlockInContext(context, mapper_functions[i], "branch");
        }
        if_statements[mapper->num_entries] = LLVMAppendBasicBlockInContext(context, mapper_functions[i], "branch");

        LLVMBuildBr(builder, if_statements[0]);


        for(int j = 0; j < mapper->num_entries; j++) {
            wdc65816_mapper_entry_t* entry = &mapper->entries[j];
            LLVMPositionBuilderAtEnd(builder, if_statements[j]);

            LLVMBasicBlockRef if2 = LLVMAppendBasicBlockInContext(context, mapper_functions[i], "if2");
            LLVMBasicBlockRef if3 = LLVMAppendBasicBlockInContext(context, mapper_functions[i], "if3");
            LLVMBasicBlockRef if4 = LLVMAppendBasicBlockInContext(context, mapper_functions[i], "if4");
        
            LLVMValueRef cond1 = LLVMBuildICmp(builder, LLVMIntULE,
                                               LLVMConstInt(lint32, entry->bank_low, 1),
                                               bank, "cond1");

            LLVMBuildCondBr(builder, cond1, if2, if_statements[j + 1]);
            LLVMPositionBuilderAtEnd(builder, if2);
            LLVMValueRef cond2 = LLVMBuildICmp(builder, LLVMIntUGE,
                                               LLVMConstInt(lint32, entry->bank_high, 1),
                                               bank, "cond2");
            LLVMBuildCondBr(builder, cond2, if3, if_statements[j + 1]);
            LLVMPositionBuilderAtEnd(builder, if3);
            LLVMValueRef cond3 = LLVMBuildICmp(builder, LLVMIntULE,
                                               LLVMConstInt(lint32, entry->addr_low, 1),
                                               addr, "cond3");
            LLVMBuildCondBr(builder, cond3, if4, if_statements[j + 1]);
            LLVMPositionBuilderAtEnd(builder, if4);
            LLVMValueRef cond4 = LLVMBuildICmp(builder, LLVMIntUGE,
                                               LLVMConstInt(lint32, entry->addr_high, 1),
                                               addr, "cond4");
            LLVMBuildCondBr(builder, cond4, branches[j], if_statements[j + 1]);

            LLVMPositionBuilderAtEnd(builder, branches[j]);
            LLVMValueRef offset =
                LLVMBuildCall(builder, reduce_function,
                              (LLVMValueRef[2]) {
                                  LLVMGetParam(mapper_functions[i], 0),
                                  LLVMConstInt(lint32, entry->mask, 1)
                              }, 2, "offset");

            LLVMValueRef ptr;
            if(entry->size) {
                LLVMValueRef size = LLVMBuildSub(builder,
                                                 LLVMConstInt(lint32, entry->size, 1),
                                                 LLVMConstInt(lint32, entry->base, 1), "size");
                LLVMValueRef mirror_value =
                    LLVMBuildCall(builder, mirror_function,
                                  (LLVMValueRef[2]) { offset, size }, 2, "mirror");

                LLVMValueRef index = LLVMBuildAdd(builder, LLVMConstInt(lint32, entry->base, 1),
                                                  mirror_value, "");
                ptr = LLVMBuildGEP(builder,
                                   data_ptr[j],
                                   (LLVMValueRef[]){ index },
                                   1, "ptr");
            } else {
                ptr = LLVMBuildGEP(builder,
                                   data_ptr[j],
                                   (LLVMValueRef[]){ offset },
                                   1, "ptr");
            }
            if(i == 0) { // read
                LLVMValueRef data = LLVMBuildLoad(builder, ptr, "data");
                LLVMBuildRet(builder, data);
            } else if(i == 1) { // write
                LLVMBuildStore(builder, LLVMGetParam(mapper_functions[i], 1), ptr);
                LLVMBuildRetVoid(builder);
            } else if(i == 2) { // ptr
                LLVMBuildRet(builder, ptr);
            } else {
                assert(0 && "This should not happen");
            }

        }

        LLVMPositionBuilderAtEnd(builder, if_statements[mapper->num_entries]);
        if(i == 0) { // read
            LLVMBuildRet(builder, LLVMConstNull(lint8));
        } else if(i == 1) { // write
            LLVMBuildRetVoid(builder);
        } else if(i == 2) { // ptr
            LLVMBuildRet(builder, LLVMConstNull(lint8_ptr));
        } else {
            assert(0 && "This should not happen");
        }
    }

    //LLVMDumpModule(mapper_module);
        
    char *error = NULL;
    if(LLVMVerifyModule(mapper_module, LLVMPrintMessageAction, &error)) {
        LLVMDumpModule(mapper_module);
        printf("%s", error);
    }
    LLVMDisposeMessage(error);

    //LLVMWriteBitcodeToFile(mapper_module, "mapper_module.bc");
    LLVMPassManagerRef pass_manager = LLVMCreatePassManager();
    LLVMPassManagerRef function_pass_manager = LLVMCreateFunctionPassManagerForModule(mapper_module);
    LLVMPassManagerBuilderRef pass_builder = LLVMPassManagerBuilderCreate();
    LLVMPassManagerBuilderSetOptLevel(pass_builder, 3);
    LLVMPassManagerBuilderUseInlinerWithThreshold(pass_builder, 512);
    LLVMPassManagerBuilderPopulateModulePassManager(pass_builder, pass_manager);
    LLVMPassManagerBuilderPopulateFunctionPassManager(pass_builder, function_pass_manager);
    LLVMRunPassManager(pass_manager, mapper_module);

    LLVMSharedModuleRef mapper_shared_module = LLVMOrcMakeSharedModule(mapper_module);
    char *def_triple = LLVMGetDefaultTargetTriple();   // E.g. "x86_64-linux-gnu"
    LLVMTargetRef target_ref;
    if (LLVMGetTargetFromTriple(def_triple, &target_ref, &error)) {
        fprintf(stderr, "Error: Can not get target.\n");
        exit(1);
    }
 
    if (!LLVMTargetHasJIT(target_ref)) {
        fprintf(stderr, "Error: JIT not supported.\n");
        exit(1);
    }
    
    LLVMTargetMachineRef tm_ref =
        LLVMCreateTargetMachine(target_ref, def_triple, "", "",
                                LLVMCodeGenLevelDefault,
                                LLVMRelocDefault,
                                LLVMCodeModelJITDefault);
    if(!tm_ref) {
        fprintf(stderr, "Error: Can not create target machine.\n");
        exit(1);
    }
    LLVMDisposeMessage(def_triple);

    LLVMOrcJITStackRef orc_ref = LLVMOrcCreateInstance(tm_ref);
    LLVMOrcModuleHandle mapper_handle;
    
    LLVMOrcAddEagerlyCompiledIR(orc_ref,
                                &mapper_handle,
                                mapper_shared_module,
                                symbol_resolver, mapper);

    LLVMOrcGetSymbolAddress(orc_ref, (LLVMOrcTargetAddress*)&mapper->read,  "mapper_read");
    LLVMOrcGetSymbolAddress(orc_ref, (LLVMOrcTargetAddress*)&mapper->write, "mapper_write");
    LLVMOrcGetSymbolAddress(orc_ref, (LLVMOrcTargetAddress*)&mapper->ptr,   "mapper_ptr");
}

