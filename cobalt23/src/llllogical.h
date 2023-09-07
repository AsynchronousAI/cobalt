/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef LLLLOGICAL_H
#define LLLLOGICAL_H

#include "lllopcode.h"

namespace lll {

class Register;
class Value;

class Logical : public Opcode {
 public:
  // Constructor
  Logical(CompilerState& cs, Stack& stack);

  // Compiles the opcode
  void Compile();

 private:
  // Compilation steps
  void ComputeInteger();
  void ComputeTaggedMethod();

  // Performs the integer binary operation
  llvm::Value* PerformIntOp(llvm::Value* a, llvm::Value* b);

  // Obtains the corresponding tag for the opcode
  int GetMethodTag();

  Register& ra_;
  Value& rkb_;
  Value& rkc_;
  llvm::BasicBlock* trytm_;
};

}  // namespace lll

#endif
