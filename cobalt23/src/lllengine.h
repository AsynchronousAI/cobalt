/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef LLLENGINE_H
#define LLLENGINE_H

#include <memory>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

namespace lll {

class Engine {
public:
    Engine(llvm::ExecutionEngine* ee, llvm::Module* module,
            llvm::Function* function);

    // Gets the compiled function
    void* GetFunction();

    // Dumps the compiled modules
    void Dump();

    // Writes the bytecode and asm files
    void Write(const std::string& path);

private:
    std::unique_ptr<llvm::ExecutionEngine> ee_;
    llvm::Module* module_;
    void* function_;
};

}

#endif

