/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef LLLOPCODE_H
#define LLLOPCODE_H

#include <string>
#include <vector>

namespace llvm {
class BasicBlock;
class Value;
class Type;
}  // namespace llvm

namespace lll {

class CompilerState;
class Stack;

class Opcode {
 public:
  // Default contructor
  Opcode(CompilerState& cs, Stack& stack);

  // Compiles the opcode
  virtual void Compile() = 0;

 protected:
  // List of incomming values and blocks
  typedef std::vector<std::pair<llvm::Value*, llvm::BasicBlock*>> IncomingList;

  // Creates a Phi value and adds it's incoming values
  llvm::Value* CreatePHI(llvm::Type* type, const IncomingList& incoming,
                         const std::string& name);

  CompilerState& cs_;
  Stack& stack_;
  llvm::BasicBlock* entry_;
  llvm::BasicBlock* exit_;
};

}  // namespace lll

#endif
