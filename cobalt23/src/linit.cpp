/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


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

#include <stddef.h>

#include "cobalt.h"
#include "lauxlib.h"
#include "lprefix.h"
#include "lualib.h"

/*
** these libs are loaded by cobalt.c and are readily available to any Lua
** program
*/
static const luaL_Reg loadedlibs[] = {
    /* Base */
    {"_G", luaopen_base},
    {"_", luaopen_under},

    /* C API */
    {LUA_LOADLIBNAME, luaopen_package},
    {LUA_TABLIBNAME, luaopen_table},
    {LUA_IOLIBNAME, luaopen_io},
    {LUA_OSLIBNAME, luaopen_os},
    {LUA_STRLIBNAME, luaopen_string},
    {LUA_MATHLIBNAME, luaopen_math},
    {LUA_DBLIBNAME, luaopen_debug},
    {LUA_CORENAME, luaopen_core},
    {LUA_DEVICENAME, luaopen_device},
    {LUA_FILESYSTEMNAME, luaopen_lfs},
    {LUA_SIGNALNAME, luaopen_signal},

    {NULL, NULL}};

/*
** these libs are in the source code but must be `require`/`import`-d
** into the global table
*/
static const luaL_Reg preloadedlibs[] = {
    /* Platform specifics */
#if defined __unix__ || defined LUA_USE_POSIX || defined __APPLE__
    {LUA_UNIXNAME, luaopen_unix},
#elif defined _WIN32 || defined _WIN64 || defined __CYGWIN__ || \
    defined __MINGW32__ || defined LUA_USE_WINDOWS || defined LUA_USE_MINGW
    {LUA_WINNAME, luaopen_win},
#endif

    /* Cobalt AOT */
    {LUA_PREPROCESSORNAME, luaopen_preprocess},

    /* C API */
    {LUA_STRUCTNAME, luaopen_struct},
    {LUA_COLORLIBNAME, luaopen_color},
    {LUA_ALLOCNAME, luaopen_alloc},
    {LUA_LPEGNAME, luaopen_lpeg},
    {LUA_JSONAME, luaopen_json},
    {LUA_COLIBNAME, luaopen_coroutine},
    {LUA_ASYNCLIBNAME, luaopen_async},
    {LUA_UTF8LIBNAME, luaopen_utf8},
    {LUA_BITLIBNAME, luaopen_bit32},
    {LUA_GLMATHNAME, luaopen_moonglmath},
    {LUA_BITOPNAME, luaopen_bit},
    {LUA_SOCKETNAME, luaopen_chan},
    {LUA_CRYPTNAME, luaopen_crypt},
// dynamic loading
#if defined(LUA_USE_DLOPEN) || defined(LUA_DL_DLL)
    {LUA_DYNNAME, luaopen_dyn},
#endif

// FFI
#ifdef COBALT_FFI
    {LUA_FFINAME, luaopen_ffi},
#endif

// Bindings
#ifdef COBALT_SDL
    {LUA_SDLNAME, luaopen_moonsdl2},
#endif
#ifdef COBALT_PYTHON
    {LUA_PYTHONNAME, luaopen_python},
#endif
#ifdef COBALT_USB
    {LUA_USBNAME, luaopen_moonusb},
#endif

    {NULL, NULL}
};

LUALIB_API void luaL_openlibs(lua_State *L) {
  const luaL_Reg *lib;

  /* "require" functions from 'loadedlibs' and set results to global table */
  for (lib = loadedlibs; lib->func; lib++) {
    luaL_requiref(L, lib->name, lib->func, 1);
    lua_pop(L, 1); /* remove lib */
  }

  /* add open functions from 'preloadedlibs' into 'package.preload' table */
  luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
  for (lib = preloadedlibs; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_setfield(L, -2, lib->name);
  }
}