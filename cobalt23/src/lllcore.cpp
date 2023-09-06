/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include <cstring>
#include <iostream>

#include "lllcompiler.h"
#include "lllengine.h"

extern "C" {
#include "lprefix.h"
#include "lapi.h"
#include "lauxlib.h"
#include "lmem.h"
#include "lllcore.h"
}

#define GETENGINE(p) static_cast<lll::Engine *>(p->llldata)
#define SETENGINE(p, e) { \
    auto engine = e; \
    p->llldata = engine; \
    p->lllfunction = reinterpret_cast<LLLFunction>(engine->GetFunction()); }

static int autocompile_ = 1;
static int callstocompile_ = 50;

void writeerror (lua_State *L, char **outerr, const char *err) {
    if (outerr) {
        *outerr = luaM_newvector(L, strlen(err) + 1, char);
        strcpy(*outerr, err);
    }
}

int LLLCompile (lua_State *L, Proto *p, char **errmsg) {
    if (GETENGINE(p) != NULL) {
        writeerror(L, errmsg, "Function already compiled");
        return 1;
    }

    lll::Compiler compiler(L, p);
    if (!compiler.Compile()) {
        writeerror(L, errmsg, compiler.GetErrorMessage().c_str());
        return 1;
    }

    SETENGINE(p, compiler.GetEngine());
    return 0;
}

int LLLCompileAll (lua_State *L, Proto *p, char **errmsg) {
    if (LLLCompile(L, p, errmsg))
        return 1;
    for (int i = 0; i < p->sizep; ++i)
        if (LLLCompileAll(L, p->p[i], errmsg))
            return 1;
    return 0;
}

void LLLSetAutoCompileEnable (int enable) {
    autocompile_ = enable;
}

int LLLIsAutoCompileEnable() {
    return autocompile_;
}

void LLLSetCallsToCompile (int calls) {
    callstocompile_ = calls;
}

int LLLGetCallsToCompile() {
    return callstocompile_;
}

int LLLIsCompiled (Proto *p) {
    return GETENGINE(p) != NULL;
}

void LLLFreeEngine (lua_State *L, Proto *p) {
    (void)L;
    delete GETENGINE(p);
}

void LLLDump (Proto *p) {
    auto e = GETENGINE(p);
    if (e)
        e->Dump();
    else
        std::cerr << "LLDump: Engine not found!\n";
}

void LLLWrite (Proto *p, const char *path) {
    auto e = GETENGINE(p);
    if (e)
        e->Write(path);
    else
        std::cerr << "LLLWrite: Engine not found!\n";
}

