/*
** $Id: linit.c,v 1.39.1.1 2017/04/19 17:20:42 roberto Exp $
** Initialization of libraries for cobalt.c and other clients
** See Copyright Notice in cobalt.h
*/


#define linit_c
#define LUA_LIB

/*
** If you embed Lua in your program and need to open the standard
** libraries, call luaL_openlibs in your program. If you need a
** different set of libraries, copy this file to your project and edit
** it to suit your needs.
**
** You can also *preload* libraries, so that a later 'require' can
** open the library, which is already linked to the application.
** For that, do the following code:
**
**  luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
**  lua_pushcfunction(L, luaopen_modname);
**  lua_setfield(L, -2, modname);
**  lua_pop(L, 1);  // remove PRELOAD table
*/

#include "lprefix.h"


#include <stddef.h>

#include "cobalt.h"

#include "lualib.h"
#include "lauxlib.h"


/*
** these libs are loaded by cobalt.c and are readily available to any Lua
** program
*/
static const luaL_Reg loadedlibs[] = {
  {"_G", luaopen_base},
  {LUA_LOADLIBNAME, luaopen_package},
  {LUA_COLIBNAME, luaopen_coroutine},
  {LUA_TABLIBNAME, luaopen_table},
  {LUA_IOLIBNAME, luaopen_io},
  {LUA_ROUTINELIBNAME, luaopen_coroutine2},
  {LUA_OSLIBNAME, luaopen_os},
  {LUA_STRLIBNAME, luaopen_string},
  {LUA_MATHLIBNAME, luaopen_math},
  {LUA_UTF8LIBNAME, luaopen_utf8},
  {LUA_DBLIBNAME, luaopen_debug},
  {LUA_COLORLIBNAME, luaopen_color},
  {LUA_2DLIBNAME, luaopen_2D},
  {LUA_3DLIBNAME, luaopen_3D},
  {LUA_TRANSFORMNAME, luaopen_transform},
  {LUA_CORENAME, luaopen_lcinterface},
  {LUA_DEVICENAME, luaopen_device},
  {LUA_FILESYSTEMNAME, luaopen_lfs},
  {LUA_COMPLEXNAME, luaopen_complex},
  {LUA_STRUCTNAME, luaopen_struct},
  {LUA_SIGNALNAME, luaopen_signal},
  {LUA_SOCKETNAME, luaopen_chan},
  {LUA_BITLIBNAME, luaopen_bit32},
  {LUA_BITOPNAME, luaopen_bit},
  {NULL, NULL}
};


static const luaL_Reg lib_preload[] = {

  { NULL,		NULL }
};

LUALIB_API void luaL_openlibs(lua_State *L)
{
  const luaL_Reg *lib;
  /* "require" functions from 'loadedlibs' and set results to global table */
  for (lib = loadedlibs; lib->func; lib++) {
    luaL_requiref(L, lib->name, lib->func, 1);
    lua_pop(L, 1);  /* remove lib */
  }
  /* // JIT snippet, JIT supports PRELOAD require. Might add to interpreted if needed.
  const luaL_Reg *lib;
  for (lib = lj_lib_load; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_pushstring(L, lib->name);
    lua_call(L, 1, 0);
  }
  luaL_findtable(L, LUA_REGISTRYINDEX, "_PRELOAD",
		 sizeof(lj_lib_preload)/sizeof(lj_lib_preload[0])-1);
  for (lib = lj_lib_preload; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_setfield(L, -2, lib->name);
  }
  lua_pop(L, 1);
  */
}