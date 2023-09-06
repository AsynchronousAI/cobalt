/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#ifndef LLLRUNTIME_H
#define LLLRUNTIME_H

#include <cstdio>
#include <map>

#include <llvm/IR/LLVMContext.h>

#define STRINGFY(a) #a
#define STRINGFY2(a) STRINGFY(a)

namespace lll {

class Runtime {
public:
    // Gets the unique instance
    // Creates it when called for the first time
    static Runtime* Instance();

    // Obtains the type declaration
    llvm::Type* GetType(const std::string& name);

    // Obtains the function declaration
    llvm::Function* GetFunction(llvm::Module* module, const std::string& name);

    // Makes a llvm int type
    llvm::Type* MakeIntT(int nbytes = sizeof(int));

private:
    Runtime();
    void InitTypes();
    void InitFunctions();

    // Adds a named struct type with $size bytes
    void AddStructType(const std::string& name, size_t size);

    // Adds a function that can be compiled
    void AddFunction(const std::string& name, llvm::FunctionType* type,
                     void* address);

    // Verifies if the key exists in the map. If not, abort! (debug only)
    template<typename MapType>
    inline void AssertKeyExists(const MapType& map, const std::string& key) {
    #ifdef DEBUG
    if (map.find(key) == map.end()) {
        fprinf(stderr, "Key not found %s!\n", key.c_str();
        exit(1);
    }
    #else
    (void)map;
    (void)key;
    #endif
    }

    static Runtime* instance_;
    llvm::LLVMContext& context_;
    std::map<std::string, llvm::Type*> types_;
    std::map<std::string, llvm::FunctionType*> functions_;
};

}

#endif

