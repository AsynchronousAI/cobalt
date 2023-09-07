/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef LLLTABLEGET_H
#define LLLTABLEGET_H

#include "lllopcode.h"

namespace lll {

class Value;
class Register;

class TableGet : public Opcode {
 public:
  // Constructor
  TableGet(CompilerState& cs, Stack& stack, Value& table, Value& key,
           Register& dest);

  // Compiles the opcode
  void Compile();

 private:
  // Compilation steps
  void CheckTable();
  void SwithTag();
  void PerformGet();
  void SearchForTM();
  void SaveResult();
  void FinishGet();

  // Call of a specific luaH_get*
  typedef llvm::Value* (Value::*GetMethod)();
  void PerformGetCase(llvm::BasicBlock* block, GetMethod getmethod,
                      const char* suffix);

  Value& table_;
  Value& key_;
  Register& dest_;
  llvm::Value* tablevalue_;
  IncomingList results_;
  IncomingList tms_;
  llvm::BasicBlock* switchtag_;
  llvm::BasicBlock* getint_;
  llvm::BasicBlock* getshrstr_;
  llvm::BasicBlock* getlngstr_;
  llvm::BasicBlock* getany_;
  llvm::BasicBlock* saveresult_;
  llvm::BasicBlock* searchtm_;
  llvm::BasicBlock* finishget_;
};

}  // namespace lll

#endif
