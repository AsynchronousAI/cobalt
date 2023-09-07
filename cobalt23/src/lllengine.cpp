/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "lllengine.h"

#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

#include <fstream>
#include <iostream>

namespace lll {

Engine::Engine(llvm::ExecutionEngine* ee, llvm::Module* module,
               llvm::Function* function)
    : ee_(ee), module_(module), function_(ee->getPointerToFunction(function)) {}

void* Engine::GetFunction() { return function_; }

void Engine::Dump() { module_->dump(); }

void Engine::Write(const std::string& path) {
  std::string module_str;
  llvm::raw_string_ostream module_os(module_str);
  module_->print(module_os, nullptr);
  auto filename = path + ".ll";
  std::ofstream fout(filename);
  if (fout.is_open()) {
    fout << module_str;
    fout.close();
    system(("llc -O2 " + filename).c_str());
  } else {
    std::cerr << "Engine::Write: Couldn't save the file " << filename << "\n";
  }
}

}  // namespace lll
