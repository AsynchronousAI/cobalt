
#define lcinterface_c
#define LUA_LIB

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dlfcn.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"
#include <sys/mman.h>
void* read_memory2(void* address, size_t size) {
  FILE* fp = tmpfile();
  fwrite(address, size, 1, fp);
  rewind(fp);
  void* buffer = malloc(size);
  fread(buffer, size, 1, fp);
  fclose(fp);
  return buffer;
}

void write_memory2(void* address, void* data, size_t size) {
  FILE* fp = tmpfile();
  fwrite(data, size, 1, fp);
  rewind(fp);
  fread(address, size, 1, fp);
  fclose(fp);
}

void delete_memory2(void* address) {
  free(address);
}

typedef struct {
  char address[8];
  int size;
} Pointer;

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
      address = (void*)lua_tostring(L, 1);
      break;
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
  void* data = read_memory2(address, size);
  lua_pushlstring(L, (const char*)data, size);
  delete_memory2(data);
  return 1;
}

static int free_hex_memory_address(lua_State* L) {
  const char* address_str = luaL_checkstring(L, 1);
  void* address = (void*)strtol(address_str, NULL, 16);
  delete_memory2(address);
  return 0;
}

static int set_hex_memory_address(lua_State* L) {
  const char* address_str = luaL_checkstring(L, 1);
  void* address = (void*)strtol(address_str, NULL, 16);
  size_t size;
  const char* data = luaL_checklstring(L, 2, &size);
  void* buffer = malloc(size);
  memcpy(buffer, data, size);
  write_memory2(address, buffer, size);
  delete_memory2(buffer);
  return 0;
}
static int permission_check(lua_State* L) {
  const char* address_str = luaL_checkstring(L, 1);
  const char* perm_str = luaL_checkstring(L, 2);
  
  void* address = (void*)strtol(address_str, NULL, 16);
  size_t size = luaL_checkinteger(L, 2);
  // if perm_str is r, it is checking read permission. if perm_str is w, it is checking write permission. if perm_str is rw, it is checking read and write permission. 
  // if perm_str is f, it is checking if it can be freed. if perm_str is a, it is checking if it can be allocated. if perm_str is af, it is checking if it can be allocated and freed.
  int prot = PROT_NONE;
  if (strcmp(perm_str, "r") == 0) {
    prot = PROT_READ;
  } else if (strcmp(perm_str, "w") == 0) {
    prot = PROT_WRITE;
  } else if (strcmp(perm_str, "rw") == 0) {
    prot = PROT_READ | PROT_WRITE;
  } else if (strcmp(perm_str, "f") == 0) {
    prot = PROT_READ | PROT_WRITE;
  } else if (strcmp(perm_str, "a") == 0) {
    prot = PROT_READ | PROT_WRITE;
  } else if (strcmp(perm_str, "af") == 0) {
    prot = PROT_READ | PROT_WRITE;
  } else {
    luaL_error(L, "Invalid permission string");
  }
  if (mprotect(address, size, prot) == 0) {
    lua_pushboolean(L, 1);
  } else {
    lua_pushboolean(L, 0);
  }
  prot = PROT_NONE;
  mprotect(address, size, prot);
  return 1;
}
static int allocate_hex_memory_address(lua_State* L) {
  size_t size = luaL_checkinteger(L, 1);
  void* address = malloc(size);
  char buffer[32];
  sprintf(buffer, "%p", address);
  lua_pushstring(L, buffer);
  return 1;
}
static int isfree_hex_memory_address(lua_State* L) {
  const char* address_str = luaL_checkstring(L, 1);
  void* address = (void*)strtol(address_str, NULL, 16);
  size_t size = luaL_checkinteger(L, 2);
  int prot = PROT_READ | PROT_WRITE;
  if (mprotect(address, size, prot) == 0) {
    lua_pushboolean(L, 1);
  } else {
    lua_pushboolean(L, 0);
  }
  prot = PROT_NONE;
  mprotect(address, size, prot);
  return 1;
}

// this doesnt use 

static const struct luaL_Reg lcinterface_lib[] = {
  //{"pointer", {
    {"get", get_hex_memory_address},
    {"fetch", fetch_hex_memory_address},
    {"free", free_hex_memory_address},
    {"set", set_hex_memory_address},
    {"perm", permission_check},
    {"alloc", allocate_hex_memory_address},
    {"isfree", isfree_hex_memory_address},
  //}},
  //{NULL, NULL}
};

LUALIB_API int luaopen_cffi(lua_State* L) {
  luaL_newlib(L, lcinterface_lib);
  return 1;
}