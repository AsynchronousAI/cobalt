/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

/* 
Provides a simple interface to switch the interpreter state.
*/

#ifndef COBALT_MINI_H
#define COBALT_MINI_H
#include "host/minicobalt.c"

int langstate = 0; /*
0: normal interpreter
1: mini interpreter
2: JIT interpreter/compiler
3: AOT compiler (locked and activated if AOT compiler is being used)
*/
#ifdef AOT_IS_MODULE
    langstate = 3;
#endif

static int luaB_state(lua_State *L){
    /* switches the interpreter state */
    if (langstate == 3)
        luaL_error(L, "AOT compiler is used and the state is locked.");
    int n = lua_gettop(L); /* number of arguments */
    if (n != 1) {
        lua_pushliteral(L, "wrong number of arguments");
        lua_error(L);
    }
    if (!lua_isnumber(L, 1)) {
        lua_pushliteral(L, "wrong argument type");
        lua_error(L);
    }
    if (lua_tonumber(L, 1) < 0 || lua_tonumber(L, 1) > 2) {
        lua_pushliteral(L, "invalid state");
        lua_error(L);
    }
    langstate = lua_tonumber(L, 1);
    return 0;
}

static int luaB_getstate(lua_State *L){
    /* returns the current interpreter state */
    lua_pushnumber(L, langstate);
    return 1;
}


#endif