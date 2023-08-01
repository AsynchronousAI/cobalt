#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dlfcn.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <sys/mman.h>

#define MAX_MEMORY 0x10000000

static void* memory_map = NULL;

static int allocate_hex_memory_address(lua_State* L) {
  size_t size = luaL_checkinteger(L, 1);
  if (memory_map == NULL) {
    memory_map = mmap(NULL, MAX_MEMORY, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory_map == MAP_FAILED) {
      luaL_error(L, "Failed to allocate memory");
    }
  }
  void* address = mmap(memory_map, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
  if (address == MAP_FAILED) {
    luaL_error(L, "Failed to allocate memory");
  }
  lua_pushinteger(L, (lua_Integer)address);
  return 1;
}

static int free_hex_memory_address(lua_State* L) {
  void* address = (void*)luaL_checkinteger(L, 1);
  int result = munmap(address, MAX_MEMORY);
  if (result == -1) {
    luaL_error(L, "Failed to free memory");
  }
  return 0;
}

static int get_hex_memory_address(lua_State* L) {
  void* address = (void*)luaL_checkinteger(L, 1);
  size_t size = luaL_checkinteger(L, 2);
  char* buffer = malloc(size);
  if (buffer == NULL) {
    luaL_error(L, "Failed to allocate memory");
  }
  memcpy(buffer, address, size);
  lua_pushlstring(L, buffer, size);
  free(buffer);
  return 1;
}

static int set_hex_memory_address(lua_State* L) {
  void* address = (void*)luaL_checkinteger(L, 1);
  size_t size = luaL_checkinteger(L, 2);
  const char* data = luaL_checkstring(L, 3);
  memcpy(address, data, size);
  return 0;
}

static int isfree_hex_memory_address(lua_State* L) {
  void* address = (void*)luaL_checkinteger(L, 1);
  size_t size = luaL_checkinteger(L, 2);
  int result = mprotect(address, size, PROT_NONE);
  if (result == -1) {
    lua_pushboolean(L, 0);
  } else {
    lua_pushboolean(L, 1);
  }
  return 1;
}

static int fetch_hex_memory_address(lua_State* L) {
  void* address = (void*)luaL_checkinteger(L, 1);
  size_t size = luaL_checkinteger(L, 2);
  int result = mprotect(address, size, PROT_READ);
  if (result == -1) {
    luaL_error(L, "Failed to set memory permissions");
  }
  return 0;
}

static int permission_check(lua_State* L) {
  void* address = (void*)luaL_checkinteger(L, 1);
  size_t size = luaL_checkinteger(L, 2);
  int prot = luaL_checkinteger(L, 3);
  int result = mprotect(address, size, prot);
  if (result == -1) {
    luaL_error(L, "Failed to set memory permissions");
  }
  return 0;
}
static int resize_memory(lua_State* L) {
  void* address = lua_touserdata(L, 1);
  size_t size = luaL_checkinteger(L, 2);
  void* new_address = realloc(address, size);
  if (new_address == NULL) {
    luaL_error(L, "Failed to resize memory");
  }
  lua_pushlightuserdata(L, new_address);
  return 1;
}


static const struct luaL_Reg lcinterface_lib[] = {
  {NULL, NULL}
};

static const struct luaL_Reg lcinterface_memory_lib[] = {
  {"get", get_hex_memory_address},
  {"fetch", fetch_hex_memory_address},
  {"free", free_hex_memory_address},
  {"set", set_hex_memory_address},
  {"perm", permission_check},
  {"alloc", allocate_hex_memory_address},
  {"isfree", isfree_hex_memory_address},
  {"realloc", resize_memory},
  {NULL, NULL}
};

LUALIB_API int luaopen_lcinterface(lua_State* L) {
  luaL_newlib(L, lcinterface_lib);
  luaL_newlib(L, lcinterface_memory_lib);
  lua_setfield(L, -2, "memory");
  return 1;
}