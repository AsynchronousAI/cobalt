/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#include "cobalt.h"
#include "lauxlib.h"
#include "lualib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>
#define LIB_HANDLE HMODULE
#define LOAD_LIBRARY(filename) LoadLibrary(filename)
#define GET_FUNCTION(handle, funcname) GetProcAddress(handle, funcname)
#define CLOSE_LIBRARY(handle) FreeLibrary(handle)
#else
#include <dlfcn.h>
#define LIB_HANDLE void *
#define LOAD_LIBRARY(filename) dlopen(filename, RTLD_LAZY)
#define GET_FUNCTION(handle, funcname) dlsym(handle, funcname)
#define CLOSE_LIBRARY(handle) dlclose(handle)
#endif

#define DYNAMIC "dynamic function"

static void *ldyn_load_library(lua_State *L) {
  const char *filename = luaL_checkstring(L, 1);
  LIB_HANDLE handle = LOAD_LIBRARY(filename);
  if (handle == NULL) {
    lua_pushnil(L);
    luaL_error(L, "unable to load library '%s'", filename);
    return 2;
  }
  lua_pushlightuserdata(L, handle);
  return 1;
}

static void *ldyn_call_function(lua_State *L) {
  LIB_HANDLE handle = lua_touserdata(L, 1);
  const char *funcname = luaL_checkstring(L, 2);

  void *func = GET_FUNCTION(handle, funcname);
  if (func == NULL) {
    lua_pushnil(L);
    luaL_error(L, "unable to find function '%s'", funcname);
    return 2;
  }
  
  /* 
  The functions are unlike Lua C API functions and do not use
  lua_states. Instead, they use the C std library functions.
  
  This calls func with the arguments passed to this function after arg 2.
  */
  int argc = lua_gettop(L)-2;
  void *args[argc];
  for (int i = 0; i < argc; i++) {
    args[i] = lua_touserdata(L, i+3);
  }
  func(); // temp
  
  lua_pushinteger(L, 0);


  return 1;
}

static void *ldyn_close_library(lua_State *L) {
  LIB_HANDLE handle = lua_touserdata(L, 1);
  CLOSE_LIBRARY(handle);
  return 0;
}

static const luaL_Reg ldyn_lib[] = {
    {"load", ldyn_load_library},
    {"call", ldyn_call_function},
    {"close", ldyn_close_library},
    {NULL, NULL},
};

LUALIB_API int luaopen_dyn(lua_State *L) {
  luaL_newlib(L, ldyn_lib);
  return 1;
}
