/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include <assert.h>
#include <limits.h>
#include <math.h>
#include <string.h>

#include "cobalt.h"
#include "lauxlib.h"
#include "lualib.h"

static int json_decode(lua_State *L) {
}
static int json_encode(lua_State *L) {
}

LUALIB_API int luaopen_json(lua_State *L) {
  luaL_Reg json_lib[] = {
    {"decode", json_decode},
    {"encode", json_encode},
    {NULL, NULL}
  };
  luaL_newlib(L, json_lib);
  (int *)lua_newuserdata(L, sizeof(int));
  luaL_newmetatable(L, "json_null");
  lua_setmetatable(L, -2);
  lua_setfield(L, -2, "nullval");

  return 1;
}