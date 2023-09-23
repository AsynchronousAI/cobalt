#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef lualib_h
#define lualib_h

#include "cobalt.h"

/* version suffix for environment variable names */
#define LUA_VERSUFFIX "cobalt23"

LUAMOD_API int(luaopen_base)(lua_State *L);
LUAMOD_API int(luaopen_under)(lua_State *L);

#define LUA_PREPROCESSORNAME "preprocess"
LUAMOD_API int(luaopen_preprocess)(lua_State *L);

#define LUA_ASYNCLIBNAME "async"
LUAMOD_API int(luaopen_async)(lua_State *L);

#define LUA_COLIBNAME "coroutine"
LUAMOD_API int(luaopen_coroutine)(lua_State *L);

#define LUA_TABLIBNAME "table"
LUAMOD_API int(luaopen_table)(lua_State *L);

#define LUA_IOLIBNAME "io"
LUAMOD_API int(luaopen_io)(lua_State *L);

#define LUA_GLMATHNAME "glmath"
LUAMOD_API int(luaopen_moonglmath)(lua_State *L);

#define LUA_OSLIBNAME "os"
LUAMOD_API int(luaopen_os)(lua_State *L);

#define LUA_STRLIBNAME "string"
LUAMOD_API int(luaopen_string)(lua_State *L);

#define LUA_UTF8LIBNAME "utf8"
LUAMOD_API int(luaopen_utf8)(lua_State *L);

#define LUA_BITLIBNAME "bit32"
LUAMOD_API int(luaopen_bit32)(lua_State *L);

#define LUA_MATHLIBNAME "math"
LUAMOD_API int(luaopen_math)(lua_State *L);

#define LUA_COLORLIBNAME "Color"
LUAMOD_API int(luaopen_color)(lua_State *L);

#define LUA_CORENAME "core"
LUAMOD_API int(luaopen_core)(lua_State *L);

#define LUA_DBLIBNAME "debug"
LUAMOD_API int(luaopen_debug)(lua_State *L);

#define LUA_LOADLIBNAME "package"
LUAMOD_API int(luaopen_package)(lua_State *L);

#define LUA_DEVICENAME "device"
LUAMOD_API int(luaopen_device)(lua_State *L);

#define LUA_STRUCTNAME "struct"
LUAMOD_API int(luaopen_struct)(lua_State *L);

#define LUA_BITOPNAME "bit"
LUAMOD_API int(luaopen_bit)(lua_State *L);

#define LUA_SIGNALNAME "signal"
LUAMOD_API int(luaopen_signal)(lua_State *L);

#define LUA_FILESYSTEMNAME "file"
LUAMOD_API int(luaopen_lfs)(lua_State *L);

#define LUA_SOCKETNAME "msg"
LUAMOD_API int(luaopen_chan)(lua_State *L);

#define LUA_ALLOCNAME "alloc"
LUAMOD_API int(luaopen_alloc)(lua_State *L);

#define LUA_JSONAME "json"
LUAMOD_API int(luaopen_json)(lua_State *L);

#define LUA_CRYPTNAME "crypt"
LUAMOD_API int(luaopen_crypt)(lua_State *L);

#define LUA_LPEGNAME "lpeg"
LUAMOD_API int(luaopen_lpeg)(lua_State *L);

#if defined(COBALT_FFI)
#define LUA_FFINAME "ffi"
LUAMOD_API int(luaopen_ffi)(lua_State *L);
#endif

// Bindings
#if defined(COBALT_SDL)
#define LUA_SDLNAME "sdl"
LUAMOD_API int(luaopen_moonsdl2)(lua_State *L);
#endif
#ifdef COBALT_PYTHON
#define LUA_PYTHONNAME "python"
LUAMOD_API int(luaopen_python)(lua_State *L);
#endif
#if defined(COBALT_USB)
#define LUA_USBNAME "usb"
LUAMOD_API int(luaopen_moonusb)(lua_State *L);
#endif

// Platform specific libraries
#if defined __unix__ || defined LUA_USE_POSIX || defined __APPLE__
#define LUA_UNIXNAME "unix"
LUAMOD_API int(luaopen_unix)(lua_State *L);
#elif defined _WIN32 || defined _WIN64 || defined __CYGWIN__ || \
    defined __MINGW32__ || defined LUA_USE_WINDOWS || defined LUA_USE_MINGW
#define LUA_WINNAME "win"
LUAMOD_API int(luaopen_win)(lua_State *L);
#endif
#if defined(LUA_USE_DLOPEN) || defined(LUA_DL_DLL)
#define LUA_DYNNAME "dynamic"
LUAMOD_API int(luaopen_dyn)(lua_State *L);
#endif

/* open all previous libraries */
LUALIB_API void(luaL_openlibs)(lua_State *L);

#if !defined(lua_assert)
#define lua_assert(x) ((void)0)
#endif

#endif

#ifdef __cplusplus
}
#endif
