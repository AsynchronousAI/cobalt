/*
** Standard library header.
** Copyright (C) 2005-2020 Mike Pall. See Copyright Notice in luajit.h
*/

#ifndef _LUALIB_H
#define _LUALIB_H

#include "lua.h"

#define LUA_FILEHANDLE	"FILE*"

#define LUA_COLIBNAME	"thread"
#define LUA_ASYNCNAME	"async"
#define LUA_MATHLIBNAME	"math"
#define LUA_STRLIBNAME	"string"
#define LUA_TABLIBNAME	"table"
#define LUA_IOLIBNAME	"io"
#define LUA_OSLIBNAME	"os"
#define LUA_LOADLIBNAME	"package"
#define LUA_DBLIBNAME	"debug"
#define LUA_BITLIBNAME	"bit"
#define LUA_JITLIBNAME	"jit"
#define LUA_FFILIBNAME	"ffi"
#define LUA_COLORLIBNAME "Color"
#define LUA_2DLIBNAME "Vector2"
#define LUA_3DLIBNAME "Vector3"
#define LUA_TRANSFORMNAME "Transform"
#define LUA_CORENAME "core"
#define LUA_DEVICENAME "device"
#define LUA_FILESYSTEMNAME "file"
#define LUA_COMPLEXNAME "complex"
#define LUA_STRUCTNAME "struct"
#define LUA_SIGNALNAME "signal"
#define LUA_SOCKETNAME "msg"


LUALIB_API int luaopen_base(lua_State *L);
LUALIB_API int luaopen_math(lua_State *L);
LUALIB_API int luaopen_string(lua_State *L);
LUALIB_API int luaopen_table(lua_State *L);
LUALIB_API int luaopen_io(lua_State *L);
LUALIB_API int luaopen_os(lua_State *L);
LUALIB_API int luaopen_package(lua_State *L);
LUALIB_API int luaopen_debug(lua_State *L);
LUALIB_API int luaopen_bit(lua_State *L);
LUALIB_API int luaopen_coroutine(lua_State *L);
LUALIB_API int luaopen_jit(lua_State *L);
LUALIB_API int luaopen_ffi(lua_State *L);
LUALIB_API int luaopen_color(lua_State *L);
LUALIB_API int luaopen_2D(lua_State *L);
LUALIB_API int luaopen_3D(lua_State *L);
LUALIB_API int luaopen_transform(lua_State *L);
LUALIB_API int luaopen_lcinterface(lua_State *L);
LUALIB_API int luaopen_device(lua_State *L);
LUALIB_API int luaopen_lfs(lua_State *L);
LUALIB_API int luaopen_complex(lua_State *L);
LUALIB_API int luaopen_struct(lua_State *L);
LUALIB_API int luaopen_signal(lua_State *L);
LUALIB_API int luaopen_chan(lua_State *L);

LUALIB_API void luaL_openlibs(lua_State *L);

#ifndef lua_assert
#define lua_assert(x)	((void)0)
#endif

#endif
