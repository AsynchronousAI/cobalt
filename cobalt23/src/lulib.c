/*
* lulib.c
* @AsynchronousAI
* Hidden standard Cobalt
* accessed using `_item` in Cobalt
*/

#define lulib_c
#define LUA_LIB

#include "lprefix.h"


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cobalt.h"

#include "lauxlib.h"
#include "lualib.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdarg.h>

static const luaL_Reg base_funcs[] = {
  
  /* placeholders */
  {"VERSION", NULL},
  {NULL, NULL}
};

LUALIB_API int luaopen_under (lua_State *L) {
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

