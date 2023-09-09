/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

/*
Convert bytecode to LLVM IR
*/

#include "cobalt.h"
#include "lauxlib.h"
#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "lopcodes.h"

void luaLLVM_convert(const char* bytecode) {
    const Instruction* opcodes = get_opcodes(version);

    const unsigned char* p = (const unsigned char*)bytecode + 18;
    const unsigned char* end = (const unsigned char*)bytecode + strlen(bytecode);
    char returns[255][255] = {0};
    
    while (p < end) {
        Instruction i = *(const Instruction*)p;
        OpCode op = GET_OPCODE(i);
        printf("%s\n", luaP_opnames[op]);

        returns += luaP_opnames[op];

        p += sizeof(Instruction);
    }
}

LLVMModuleRef luaLLVM_convert(const char* bytecode) {
    /* converts bytecode to LLVM IR */

    // Create LLVM module
    LLVMModuleRef module = LLVMModuleCreateWithName("lua_module");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef void_type = LLVMVoidType();
    LLVMTypeRef func_type = LLVMFunctionType(void_type, NULL, 0, false);
    LLVMValueRef func = LLVMAddFunction(module, "main", func_type);
    LLVMBasicBlockRef block = LLVMAppendBasicBlock(func, "entry");

    LLVMPositionBuilderAtEnd(builder, block);

    // Parse bytecode to OP codes
    int i = 0;
    for 

    
    // End and return
    LLVMDisposeBuilder(builder);

    return module;
}