#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef LLLVARARG_H
#define LLLVARARG_H

#include "lllopcode.h"

namespace lll {

class Register;

class Vararg : public Opcode {
 public:
  // Constructor
  Vararg(CompilerState& cs, Stack& stack);

  // Compiles the opcode
  void Compile();

 private:
  // Compilation steps
  void ComputeAvailableArgs();
  void ComputeRequiredArgs();
  void ComputeNMoves();
  void MoveAvailable();
  void FillRequired();

  // Retuns the register at ra + offset
  llvm::Value* GetRegisterFromA(llvm::Value* offset);

  llvm::Value* available_;
  llvm::Value* required_;
  llvm::Value* nmoves_;
  llvm::BasicBlock* movecheck_;
  llvm::BasicBlock* move_;
  llvm::BasicBlock* fillcheck_;
  llvm::BasicBlock* fill_;
};

}  // namespace lll

#endif

#ifdef __cplusplus
}
#endif
