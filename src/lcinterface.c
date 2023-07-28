/*
Like ffi for LuaJIT, but can be interpreted and bytecoded.
*/

/*
    [DISABLED]
*/
#define lcinterface_c
#define LUA_LIB

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dlfcn.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

typedef union {
  double d;
  int b;
  void* p;
} lcinterface_Value;

typedef struct {
  const char* name;
  void* addr;
} lcinterface_Symbol;

static int lcinterface_load(lua_State* L) {
  const char* libname = luaL_checkstring(L, 1);
  void* lib = dlopen(libname, RTLD_LAZY);
  if (lib == NULL) {
    lua_pushnil(L);
    lua_pushstring(L, dlerror());
    return 2;
  }
  lua_pushlightuserdata(L, lib);
  return 1;
}

static int lcinterface_unload(lua_State* L) {
  void* lib = lua_touserdata(L, 1);
  dlclose(lib);
  return 0;
}

static int lcinterface_get_symbol(lua_State* L) {
  void* lib = lua_touserdata(L, 1);
  const char* symbol = luaL_checkstring(L, 2);
  void* addr = dlsym(lib, symbol);
  if (addr == NULL) {
    lua_pushnil(L);
    lua_pushstring(L, dlerror());
    return 2;
  }
  lcinterface_Symbol* sym = lua_newuserdata(L, sizeof(lcinterface_Symbol));
  sym->name = symbol;
  sym->addr = addr;
  return 1;
}

static int lcinterface_call(lua_State* L) {
  void* lib = lua_touserdata(L, 1);
  lcinterface_Symbol* sym = lua_touserdata(L, 2);
  int nargs = lua_gettop(L) - 2;
  lcinterface_Value* args = malloc(nargs * sizeof(lcinterface_Value));
  int i;
  for (i = 0; i < nargs; i++) {
    switch (lua_type(L, i + 3)) {
      case LUA_TNUMBER:
        args[i].d = lua_tonumber(L, i + 3);
        break;
      case LUA_TBOOLEAN:
        args[i].b = lua_toboolean(L, i + 3);
        break;
      case LUA_TSTRING:
        args[i].p = (void*)lua_tostring(L, i + 3);
        break;
      case LUA_TUSERDATA:
        args[i].p = lua_touserdata(L, i + 3);
        break;
      default:
        luaL_argerror(L, i + 3, "unsupported argument type");
    }
  }
  lcinterface_Value result;
  void (*func)(void) = (void (*)(void))sym->addr;
  switch (nargs) {
    case 0:
      func();
      break;
    default:
      luaL_error(L, "too many arguments");
  }
  free(args);
  return 0;
}

static const struct luaL_Reg lcinterface_lib[] = {
  {"load", lcinterface_load},
  {"unload", lcinterface_unload},
  {"get_symbol", lcinterface_get_symbol},
  {"call", lcinterface_call},
  {NULL, NULL}
};

LUALIB_API int luaopen_lcinterface(lua_State* L) {
  luaL_newlib(L, lcinterface_lib);
  return 1;
}