#include <llvm-c/Core.h>

int test() {
    // Initialize LLVM
    LLVMInitializeCore(LLVMGetGlobalPassRegistry());
    LLVMContextRef context = LLVMContextCreate();
    LLVMModuleRef module = LLVMModuleCreateWithName("lua_module");
    LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);

    // Create a function
    LLVMTypeRef funcType = LLVMFunctionType(LLVMInt32TypeInContext(context), NULL, 0, 0);
    LLVMValueRef func = LLVMAddFunction(module, "main", funcType);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(func, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    // Create LLVM IR instructions
    LLVMValueRef x = LLVMConstInt(LLVMInt32TypeInContext(context), 10, 0);
    LLVMValueRef y = LLVMConstInt(LLVMInt32TypeInContext(context), 20, 0);
    LLVMValueRef sum = LLVMBuildAdd(builder, x, y, "sum");
    LLVMBuildRet(builder, sum);

    // Print LLVM IR code
    char* irCode = LLVMPrintModuleToString(module);
    printf("LLVM IR:\n%s\n", irCode);

    // Clean up
    LLVMDisposeModule(module);
    LLVMContextDispose(context);

    return 0;
}