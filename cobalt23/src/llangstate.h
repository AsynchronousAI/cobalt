#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

/* 
Provides a simple interface to switch the interpreter state.
*/


#ifndef LLANGSTATE_H
#ifndef INTERNAL_AOT
#define LLANGSTATE_H

extern int State = 1;

/*
1: normal interpreter
2: mini interpreter
3: jit interpreter
4: byteaot
5: caot
6: iraot
*/

int luaB_state(lua_State *L){
    /* switches the interpreter state */
    #ifdef AOT_IS_MODULE
        lua_error("AOT compiler is used and the state is locked.");
        return 0;
    #else
        int n = lua_gettop(L); /* number of arguments */
        if (n != 1) {
            lua_pushliteral(L, "wrong number of arguments");
            lua_error(L);
        }
        if (!lua_isnumber(L, 1)) {
            lua_pushliteral(L, "wrong argument type");
            lua_error(L);
        }
        if (lua_tonumber(L, 1) < 1 || lua_tonumber(L, 1) > 3) {
            lua_pushliteral(L, "invalid state");
            lua_error(L);
        }
        State = lua_tonumber(L, 1);
        return 0;
    #endif
}

int luaB_getstate(lua_State *L){
    /* returns the current interpreter state */
    #ifdef AOT_IS_MODULE
        lua_pushnumber(L, 3);
    #else
        lua_pushnumber(L, State);
    #endif
    return 1;
}
#endif // INTERNAL_AOT
#endif // LLANGSTATE_H
#ifdef __cplusplus
}
#endif
