#define linstance_c
#define LUA_LIB

#include "lprefix.h"


#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


typedef struct {
    int x; // position x
    int y; // position y
    int z; // position z
    int ox; // orientation x
    int oy; // orientation y
    int oz; // orientation z
    int sx; // scale x
    int sy; // scale y
    int sz; // scale z
    int px; // pivot x
    int py; // pivot y
    int pz; // pivot z
    
} Transform;

static const luaL_Reg lib[] = {
    {"new", vec2_new},
    {"__add", vec2_add},
    {"__sub", vec2_sub},
    {"__mul", vec2_mul},
    {"__div", vec2_div},
  {NULL, NULL}
};

static const luaL_Reg meta[] = {
  {"__index", vec2_index},
  {NULL, NULL}
};


LUAMOD_API int linstance_c (lua_State *L) {
  luaL_newlib(L, lib);

  push_vec2(L, 0, 0);
  lua_setfield(L, -2, "zero");
  push_vec2(L, 1, 1);
  lua_setfield(L, -2, "one");
  return 1;
}