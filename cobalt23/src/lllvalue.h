/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef LLLVALUE_H
#define LLLVALUE_H

#include <string>
#include <vector>

namespace llvm {
class Value;
class Type;
}  // namespace llvm

extern "C" {
struct lua_TValue;
}

namespace lll {

class CompilerState;

// The top abstraction for constant, registers and upvalues
class Value {
 public:
  // Constructor
  Value(CompilerState& cs);

  // Returns whether the values has the following tag
  llvm::Value* HasTag(int tag);

  // Obtains the tag of the value
  virtual llvm::Value* GetTag() = 0;

  // Obtains the TValue (this should only be used when calling functions)
  virtual llvm::Value* GetTValue() = 0;

  // Access methods
  virtual llvm::Value* GetBoolean() = 0;
  virtual llvm::Value* GetInteger() = 0;
  virtual llvm::Value* GetFloat() = 0;
  virtual llvm::Value* GetTString() = 0;
  virtual llvm::Value* GetTable() = 0;
  virtual llvm::Value* GetGCValue() = 0;

 protected:
  CompilerState& cs_;
};

// Represents a constant
class Constant : public Value {
 public:
  // Constructor
  Constant(CompilerState& cs, int arg);

  // Value Implementation
  llvm::Value* GetTag();
  llvm::Value* GetTValue();
  llvm::Value* GetBoolean();
  llvm::Value* GetInteger();
  llvm::Value* GetFloat();
  llvm::Value* GetTString();
  llvm::Value* GetTable();
  llvm::Value* GetGCValue();

 private:
  struct lua_TValue* tvalue_;
};

// Base class for mutable values: upvalues and registers
class MutableValue : public Value {
 public:
  // Constructor
  MutableValue(CompilerState& cs);

  // Value Implementation
  virtual llvm::Value* GetTag();
  virtual llvm::Value* GetTValue() = 0;
  virtual llvm::Value* GetBoolean();
  virtual llvm::Value* GetInteger();
  virtual llvm::Value* GetFloat();
  virtual llvm::Value* GetTString();
  virtual llvm::Value* GetTable();
  virtual llvm::Value* GetGCValue();

  // Manipulates the tag of the value
  virtual void SetTagK(int tag);
  virtual void SetTag(llvm::Value* tag);

  // Sets the value field
  virtual void SetValue(llvm::Value* value);

  // Copy the contents of $value to this
  virtual void Assign(Value& value);

  // Sets the value and the tag
  virtual void SetBoolean(llvm::Value* bvalue);
  virtual void SetInteger(llvm::Value* ivalue);
  virtual void SetFloat(llvm::Value* fvalue);

 protected:
  // TValue fields
  enum Field { VALUE = 0, TAG = 1 };

  // Obtains the pointer to a field
  llvm::Value* GetField(Field field);

  // Obtains the pointer to a value and cast it to $type
  llvm::Value* GetValuePtr(llvm::Type* type, const std::string& fieldname);

  // Obtains the value and load it
  llvm::Value* GetValue(llvm::Type* type, const std::string& fieldname);
};

// Represents a register of lua stack
class Register : public MutableValue {
 public:
  // Constructor
  Register(CompilerState& cs, int arg);

  // Initializes the values (create allocas)
  void Init();

  // Reloads the tvalue, should be called after stack update
  void ReloadTValue();

  // Obtains the TValue
  llvm::Value* GetTValue();

 private:
  int arg_;
  llvm::Value* tvalue_;
};

// Represents a register that is only known at runtime
class RTRegister : public MutableValue {
 public:
  // Constructor
  RTRegister(CompilerState& cs, llvm::Value* tvalue);

  // Obtains the TValue
  llvm::Value* GetTValue();

 private:
  llvm::Value* tvalue_;
};

// Represents an upvalue
class Upvalue : public MutableValue {
 public:
  // Constructor
  Upvalue(CompilerState& cs, int arg);

  // Reloads the tvalue
  void ReloadTValue();

  // Obtains the pointer to UpVal structure
  llvm::Value* GetUpVal();

  // Obtains the TValue
  llvm::Value* GetTValue();

 private:
  int arg_;
  llvm::Value* upval_;
  llvm::Value* tvalue_;
};

// Manages all values
class Stack {
 public:
  // Constructor
  Stack(CompilerState& cs);

  // Obtains a RK(arg) value
  Value& GetRK(int arg);

  // Obtains a constant
  Constant& GetK(int arg);

  // Obtains a register
  Register& GetR(int arg);

  // Obtains an upvalue
  Upvalue& GetUp(int arg);

  // Initializes the values; should be called at the entry block
  void InitValues();

  // Updates the registers after a stack reallocation
  void Update();

 private:
  CompilerState& cs_;
  std::vector<Constant> k_;
  std::vector<Register> r_;
  std::vector<Upvalue> u_;
};

}  // namespace lll

#endif
