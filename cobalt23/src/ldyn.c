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

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#define LIB_HANDLE HMODULE
#define LOAD_LIBRARY(filename) LoadLibrary(filename)
#define GET_FUNCTION(handle, funcname) GetProcAddress(handle, funcname)
#define CLOSE_LIBRARY(handle) FreeLibrary(handle)
#define access(path, mode) _access(path, mode)
#define F_OK 0
#define EXISTS 0
#else
#include <dlfcn.h>
#include <unistd.h>
#define LIB_HANDLE void *
#define LOAD_LIBRARY(filename) dlopen(filename, RTLD_LAZY)
#define GET_FUNCTION(handle, funcname) dlsym(handle, funcname)
#define CLOSE_LIBRARY(handle) dlclose(handle)
#define EXISTS 1
#endif

#define DYNAMIC "dynamic library"
#define DYNAMIC_F "dynamic function"

typedef void (*CFunction)();

static void *run_c(char *code, char *fn) {
  // if temp.c or temp.dll or temp.so already exists, error
  if (access("temp.c", F_OK) == EXISTS || access("temp.dll", F_OK) == EXISTS || access("temp.so", F_OK) == EXISTS) {
    return "Error: temp.c, temp.dll, or temp.so already exists.";
  }
  // check gcc is installed
  if (system("gcc --version") != 0) {
    return "Error: gcc is not installed.\nCannot compile-run C code.";
  }
  #if defined(_WIN32)
    // Create a temporary C file
    FILE* file = fopen("temp.c", "w");
    if (file == NULL) {
        return ("Failed to create temporary file\n");
    }
    fprintf(file, "%s", code);
    fclose(file);

    // Compile the C file into a DLL
    system("gcc -shared -o temp.dll temp.c");

    // Load the DLL
    HMODULE handle = LoadLibrary("temp.dll");
    if (handle == NULL) {
        remove("temp.c");
        remove("temp.dll");
        return ("Failed to load DLL\n");
    }

    // Get the function pointer
    CFunction dynamicFunction = (CFunction)GetProcAddress(handle, fn);
    if (dynamicFunction == NULL) {
      remove("temp.c");
      remove("temp.dll");
        return ("Failed to get function pointer\n");
    }

    // Call the dynamic function
    dynamicFunction();

    // Unload the DLL
    FreeLibrary(handle);

    // Remove the temporary files
    remove("temp.c");
    remove("temp.dll");
  #else
    FILE* file = fopen("temp.c", "w");
    if (file == NULL) {
        return ("Failed to create temporary file\n");
    }
    fprintf(file, "%s", code);
    fclose(file);

    // Compile the C file into a shared library
    system("gcc -shared -fPIC -o temp.so temp.c");

    // Load the shared library
    void* handle = dlopen("./temp.so", RTLD_LAZY);
    if (handle == NULL) {
    remove("temp.c");
    remove("temp.so");
        return ("Failed to load shared library: %s\n", dlerror());
    }

    // Get the function pointer
    CFunction dynamicFunction = (CFunction)dlsym(handle, fn);
    if (dynamicFunction == NULL) {
      remove("temp.c");
      remove("temp.so");
        return ("Failed to get function pointer: %s\n", dlerror());
    }

    // Call the dynamic function
    dynamicFunction();

    // Unload the shared library
    dlclose(handle);

    // Remove the temporary files
    remove("temp.c");
    remove("temp.so");

    return 0;
  #endif
  
  return 0;
}

static void *ldyn_run_c(lua_State *L) {
  const char *code = luaL_checkstring(L, 1);
  const char *fn = luaL_checkstring(L, 2);
  void *runned = run_c(code, fn);
  if (runned != 0) {
    lua_pushnil(L);
    luaL_error(L, "unable to run C code\n\t%s", runned);
    return 2;
  }
  lua_pushnil(L);
  return 1;
}

/* calling */
static int ldyn_call_function(lua_State *L) {
  void (*func)(int,int,int,int,int) = lua_touserdata(L, 1);
  int args[5] = {luaL_optinteger(L, 2, 0), luaL_optinteger(L, 3, 0), luaL_optinteger(L, 4, 0), luaL_optinteger(L, 5, 0), luaL_optinteger(L, 6, 0)};

  func(args[0], args[1], args[2], args[3], args[4]);
  lua_pushnil(L);
  return 0;
}

static void *ldyn_get_function(lua_State *L) {
  LIB_HANDLE handle = lua_touserdata(L, 1);
  const char *funcname = luaL_checkstring(L, 2);

  void *func = GET_FUNCTION(handle, funcname);
  if (func == NULL) {
    lua_pushnil(L);
    luaL_error(L, "unable to find function '%s' (DYNERR: not found)", funcname);
    return 2;
  }

  lua_pushlightuserdata(L, func);
  luaL_newmetatable(L, DYNAMIC_F);

  /* add method call */
  lua_pushstring(L, "__call");
  lua_pushcfunction(L, ldyn_call_function);
  lua_settable(L, -3);

  lua_setmetatable(L, -2);

  return 1;
}

static void *ldyn_close_library(lua_State *L) {
  LIB_HANDLE handle = lua_touserdata(L, 1);
  CLOSE_LIBRARY(handle);
  return 0;
}

static void *ldyn_load_library(lua_State *L) {
  const char *filename = luaL_checkstring(L, 1);
  LIB_HANDLE handle = LOAD_LIBRARY(filename);
  if (handle == NULL) {
    lua_pushnil(L);
    luaL_error(L, "unable to load library '%s' (DYNERR: file not found)", filename);
    return 2;
  }
  lua_pushlightuserdata(L, handle);
  luaL_newmetatable(L, DYNAMIC);

  /* add an index method */
  lua_pushstring(L, "__index");
  lua_pushcfunction(L, ldyn_get_function);
  lua_settable(L, -3);

  /* add a gc method */
  lua_pushstring(L, "__gc");
  lua_pushcfunction(L, ldyn_close_library);
  lua_settable(L, -3);

  lua_setmetatable(L, -2);

  return 1;
}


static const luaL_Reg ldyn_lib[] = {
    {"load", ldyn_load_library},
    {"get", ldyn_get_function},
    {"close", ldyn_close_library},
    {"run", ldyn_run_c},
    {NULL, NULL},
};

LUALIB_API int luaopen_dyn(lua_State *L) {
  luaL_newlib(L, ldyn_lib);
  return 1;
}
