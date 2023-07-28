#define lmemlib_c
#define LUA_LIB

#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"

void* read_memory(void* address, size_t size) {
  void* buffer = malloc(size);
  memcpy(buffer, address, size);
  return buffer;
}

void write_memory(void* address, void* data, size_t size) {
  memcpy(address, data, size);
}

void delete_memory(void* address) {
  free(address);
}

static int get_hex_memory_address(lua_State* L) {
  void* address = NULL;
  int type = lua_type(L, 1);
  switch (type) {
    case LUA_TNIL:
      break;
    case LUA_TBOOLEAN:
      address = (void*)(lua_toboolean(L, 1) ? 1 : 0);
      break;
    case LUA_TLIGHTUSERDATA:
      address = lua_touserdata(L, 1);
      break;
    case LUA_TNUMBER:
      address = (void*)(intptr_t)lua_tonumber(L, 1);
      break;
    case LUA_TSTRING:
    case LUA_TTABLE:
    case LUA_TFUNCTION:
    case LUA_TUSERDATA:
    case LUA_TTHREAD:
      address = lua_topointer(L, 1);
      break;
    default:
      luaL_error(L, "Invalid Lua value type");
      break;
  }
  char buffer[32];
  sprintf(buffer, "%p", address);
  lua_pushstring(L, buffer);
  return 1;
}

static int fetch_hex_memory_address(lua_State* L) {
  const char* address_str = luaL_checkstring(L, 1);
  void* address = (void*)strtol(address_str, NULL, 16);
  size_t size = luaL_checkinteger(L, 2);
  void* data = read_memory(address, size);
  lua_pushlstring(L, (const char*)data, size);
  delete_memory(data);
  return 1;
}

static int free_hex_memory_address(lua_State* L) {
  const char* address_str = luaL_checkstring(L, 1);
  void* address = (void*)strtol(address_str, NULL, 16);
  delete_memory(address);
  return 0;
}

static int set_hex_memory_address(lua_State* L) {
  const char* address_str = luaL_checkstring(L, 1);
  void* address = (void*)strtol(address_str, NULL, 16);
  size_t size;
  const char* data = luaL_checklstring(L, 2, &size);
  write_memory(address, (void*)data, size);
  return 0;
}

static const luaL_Reg memlib[] = {
  {"get", get_hex_memory_address},
  {"fetch", fetch_hex_memory_address},
  {"free", free_hex_memory_address},
  {"set", set_hex_memory_address},
  {NULL, NULL}
};

/*
** Open memory library
*/
LUAMOD_API int luaopen_memory (lua_State *L) {
  luaL_newlib(L, memlib);
  return 1;
}


