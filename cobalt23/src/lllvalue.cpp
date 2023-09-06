/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "lllvalue.h"
#include "lllcompilerstate.h"

extern "C" {
#include "lprefix.h"
#include "lfunc.h"
#include "lgc.h"
#include "lobject.h"
#include "lopcodes.h"
}

namespace lll {

Value::Value(CompilerState& cs) :
    cs_(cs) {
}

llvm::Value* Value::HasTag(int tag) {
    return cs_.B_.CreateICmpEQ(GetTag(), cs_.MakeInt(tag));
}

Constant::Constant(CompilerState& cs, int arg) :
    Value(cs),
    tvalue_(cs.proto_->k + arg) {
}

llvm::Value* Constant::GetTag() {
    return cs_.MakeInt(rttype(tvalue_));
}

llvm::Value* Constant::GetTValue() {
    auto ttvalue = cs_.rt_.GetType("TValue");
    return cs_.InjectPointer(ttvalue, tvalue_);
}

llvm::Value* Constant::GetBoolean() {
    return cs_.MakeInt(bvalue(tvalue_));
}

llvm::Value* Constant::GetInteger() {
    auto tluainteger = cs_.rt_.GetType("lua_Integer");
    return cs_.MakeInt(ivalue(tvalue_), tluainteger);
}

llvm::Value* Constant::GetFloat() {
    auto tluanumber = cs_.rt_.GetType("lua_Number");
    return llvm::ConstantFP::get(tluanumber, nvalue(tvalue_));
}

llvm::Value* Constant::GetTString() {
    auto ttstring = cs_.rt_.GetType("TString");
    return cs_.InjectPointer(ttstring, tsvalue(tvalue_));
}

llvm::Value* Constant::GetTable() {
    auto ttable = cs_.rt_.GetType("Table");
    return cs_.InjectPointer(ttable, hvalue(tvalue_));
}

llvm::Value* Constant::GetGCValue() {
    auto tgcobject = cs_.rt_.GetType("GCObject");
    return cs_.InjectPointer(tgcobject, gcvalue(tvalue_));
}

MutableValue::MutableValue(CompilerState& cs) :
    Value(cs) {
}

llvm::Value* MutableValue::GetTag() {
    return cs_.B_.CreateLoad(GetField(TAG), "tag");
}

llvm::Value* MutableValue::GetBoolean() {
    return GetValue(cs_.rt_.MakeIntT(sizeof(int)), "bvalue");
}

llvm::Value* MutableValue::GetInteger() {
    return cs_.B_.CreateLoad(GetField(VALUE), "ivalue");
}

llvm::Value* MutableValue::GetFloat() {
    return GetValue(cs_.rt_.GetType("lua_Number"), "nvalue");
}

llvm::Value* MutableValue::GetTString() {
    return GetValue(cs_.rt_.GetType("TString"), "strvalue");
}

llvm::Value* MutableValue::GetTable() {
    return GetValue(cs_.rt_.GetType("Table"), "hvalue");
}

llvm::Value* MutableValue::GetGCValue() {
    return GetValue(cs_.rt_.GetType("GCObject"), "gcvalue");
}

void MutableValue::SetTagK(int tag) {
    SetTag(cs_.MakeInt(tag));
}

void MutableValue::SetTag(llvm::Value* tag) {
    cs_.B_.CreateStore(tag, GetField(TAG));
}

void MutableValue::SetValue(llvm::Value* value) {
    auto field = GetField(VALUE);
    auto ptrtype = llvm::PointerType::get(value->getType(), 0);
    auto tfield = cs_.B_.CreateBitCast(field, ptrtype);
    cs_.B_.CreateStore(value, tfield);
}

void MutableValue::Assign(Value& value) {
    SetTag(value.GetTag());
    SetValue(value.GetInteger());
}

void MutableValue::SetBoolean(llvm::Value* bvalue) {
    SetTagK(LUA_TBOOLEAN);
    auto type = cs_.rt_.MakeIntT(sizeof(int));
    cs_.B_.CreateStore(bvalue, GetValuePtr(type, "bvalue"));
}

void MutableValue::SetInteger(llvm::Value* ivalue) {
    SetTagK(LUA_TNUMINT);
    cs_.B_.CreateStore(ivalue, GetField(VALUE));
}

void MutableValue::SetFloat(llvm::Value* fvalue) {
    SetTagK(LUA_TNUMFLT);
    auto type = cs_.rt_.GetType("lua_Number");
    cs_.B_.CreateStore(fvalue, GetValuePtr(type, "nvalue"));
}

llvm::Value* MutableValue::GetField(Field field) {
    std::vector<llvm::Value*> indices =
        {cs_.MakeInt(0), cs_.MakeInt((int)field)};
    auto name = field == VALUE ? "value.ptr" : "tag.ptr";
    return cs_.B_.CreateGEP(GetTValue(), indices, name);
}

llvm::Value* MutableValue::GetValue(llvm::Type* type, const std::string& name) {
    return cs_.B_.CreateLoad(GetValuePtr(type, name), name);
}

llvm::Value* MutableValue::GetValuePtr(llvm::Type* type,
        const std::string& name) {
    auto ptrtype = llvm::PointerType::get(type, 0);
    return cs_.B_.CreateBitCast(GetField(VALUE), ptrtype, name + ".ptr");
}

Register::Register(CompilerState& cs, int arg) :
    MutableValue(cs),
    arg_(arg),
    tvalue_(nullptr) {
}

void Register::Init() {
    auto ttvalue = cs_.rt_.GetType("TValue");
    auto name = "r" + std::to_string(arg_) + "_";
    tvalue_ = cs_.B_.CreateAlloca(ttvalue, nullptr, name);
    ReloadTValue();
}

void Register::ReloadTValue() {
    auto tvalue = cs_.B_.CreateGEP(cs_.GetBase(), cs_.MakeInt(arg_));
    cs_.B_.CreateStore(tvalue, tvalue_);
}

llvm::Value* Register::GetTValue() {
    return cs_.B_.CreateLoad(tvalue_, tvalue_->getName() + "load");
}

RTRegister::RTRegister(CompilerState& cs, llvm::Value* tvalue) :
    MutableValue(cs),
    tvalue_(tvalue) {
}

llvm::Value* RTRegister::GetTValue() {
    return tvalue_;
}

Upvalue::Upvalue(CompilerState& cs, int arg) :
    MutableValue(cs),
    arg_(arg),
    upval_(nullptr) {
}

void Upvalue::ReloadTValue() {
    auto upvalptr = cs_.B_.CreateGEP(cs_.values_.upvals, cs_.MakeInt(arg_),
            "upval.ptr");
    upval_ = cs_.B_.CreateLoad(upvalptr, "upval");
    tvalue_ = cs_.LoadField(upval_, cs_.rt_.GetType("TValue"),
            offsetof(UpVal, v), "upval.v");
}

llvm::Value* Upvalue::GetUpVal() {
    return upval_;
}

llvm::Value* Upvalue::GetTValue() {
    return tvalue_;
}

Stack::Stack(CompilerState& cs) :
    cs_(cs) {
    for (int i = 0; i < cs.proto_->sizek; ++i)
        k_.emplace_back(cs, i);
    for (int i = 0; i < cs.proto_->maxstacksize + 1; ++i)
        r_.emplace_back(cs, i);
    for (int i = 0; i < cs.proto_->sizeupvalues; ++i)
        u_.emplace_back(cs, i);
}

Value& Stack::GetRK(int arg) {
    if (ISK(arg))
        return GetK(INDEXK(arg));
    else
        return GetR(arg);
}

Constant& Stack::GetK(int arg) {
    assert(arg >= 0);
    assert(arg < (int)k_.size());
    return k_[arg];
}

Register& Stack::GetR(int arg) {
    assert(arg >= 0);
    assert(arg < (int)r_.size());
    return r_[arg];
}

Upvalue& Stack::GetUp(int arg) {
    assert(arg >= 0);
    assert(arg < (int)u_.size());
    auto& u = u_[arg];
    u.ReloadTValue();
    return u;
}

void Stack::InitValues() {
    for (auto& r : r_)
        r.Init();
}

void Stack::Update() {
    cs_.UpdateBase();
    for (auto& r : r_)
        r.ReloadTValue();
}

}

