/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#define lulib_c
#define LUA_LIB

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cobalt.h"
#include "lauxlib.h"
#include "lprefix.h"
#include "lualib.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdarg.h>
#include <stdio.h>

static int luaUB_global(lua_State *L) {
  // takes in a string (key) and a value. sets the global variable with the key
  // to the value and returns the value
  int nargs = lua_gettop(L);
  luaL_argcheck(L, nargs >= 1, 1, "expected at least 1 argument");
  luaL_argcheck(L, nargs <= 2, 2, "expected at most 2 arguments");
  const char *key = luaL_checkstring(L, 1);
  if (nargs == 2) {
    lua_setglobal(L, key);
  }
  lua_getglobal(L, key);
  return 1;
}
static int luaUB_ipairsaux (lua_State *L) {
  lua_Integer i = luaL_checkinteger(L, 2);
  i = luaL_intop(+, i, 1);
  lua_pushinteger(L, i);
  return (lua_geti(L, 1, i) == LUA_TNIL) ? 1 : 2;
}

static const luaL_Reg base_funcs[] = {{"global", luaUB_global},
                                      {"ipairsaux", luaUB_ipairsaux},

                                      /* placeholders */
                                      {"VERSION", NULL},
                                      {NULL, NULL}};

LUALIB_API int luaopen_under(lua_State *L) {
  /* open lib into global table */
  luaL_newlib(L, base_funcs);

  /* set global _VERSION */
  lua_pushnumber(L, COBALT_VERSION_NUM);
  lua_setfield(L, -2, "VERSION");

  /* set global _LUA */
  lua_pushnumber(L, COBALTLUA_VERSION_NUM);
  lua_setfield(L, -2, "LUA");

  return 1;
}
