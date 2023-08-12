/*
** Library initialization.
** Copyright (C) 2005-2020 Mike Pall. See Copyright Notice in luajit.h
**
** Major parts taken verbatim from the Lua interpreter.
** Copyright (C) 1994-2008 Lua.org, PUC-Rio. See Copyright Notice in lua.h
*/

#define lib_init_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "lj_arch.h"

static const luaL_Reg lj_lib_load[] = {
  { "",			luaopen_base },
  { LUA_LOADLIBNAME,	luaopen_package },
  { LUA_TABLIBNAME,	luaopen_table },
  { LUA_IOLIBNAME,	luaopen_io },
  { LUA_OSLIBNAME,	luaopen_os },
  { LUA_STRLIBNAME,	luaopen_string },
  { LUA_MATHLIBNAME,	luaopen_math },
  { LUA_DBLIBNAME,	luaopen_debug },
  { LUA_BITLIBNAME,	luaopen_bit },
  { "bit32",	luaopen_bit },
  { LUA_JITLIBNAME,	luaopen_jit },
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
  {LUA_ASYNCNAME, luaopen_coroutine},
  { NULL,		NULL }
};

static const luaL_Reg lj_lib_preload[] = {
#if LJ_HASFFI
  { LUA_FFILIBNAME,	luaopen_ffi }, 
#endif
  { "bit32",	luaopen_bit },
  { LUA_JITLIBNAME,	luaopen_jit },
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
  {LUA_ASYNCNAME, luaopen_coroutine},
  { NULL,		NULL }
};

LUALIB_API void luaL_openlibs(lua_State *L)
{
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
}

