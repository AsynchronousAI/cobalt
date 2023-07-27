/* 
Instances quickly summed up

## How to make a CoreInstance
Using CoreInstance.new is how to make one, this is an internal term and cannot be accessed by the user 
It requires security level 7 to use.

Arguments:
CoreInstance.new(table InheritsFrom, string Name, table Properties, table Methods): returns NewCoreInstance
NewCoreInstance.mod(): returns ModifyProfile

ModifyProfile.Properties = table Properties (Variables)
ModifyProfile.Methods = table Methods (Functions)
ModifyProfile.Name = string Name (CoreInstance Name, not file name)
ModifyProfile.Extension = string Extension (Required extension for the instance)

## How can the user get one?
All files when imported, loaded, or created are automatically converted to a CoreInstance.

## How can the user create one?
- If luax is being hosted locally then the user can use the CoreInstance.new function by forcing the security level to 7
other ways are only possible if the service that uses luax allows it.
*/

/*
TURNED OFF RN
*/
#define linstance_c
#define LUA_LIB

#include "lprefix.h"


#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


typedef struct { // Default Instance
    Instance Instance,
    

} Instance;

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