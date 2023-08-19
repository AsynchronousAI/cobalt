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

// MACROS:
static int dumpmacros(lua_State* L){
  // Step 1: Check if GCC is installed
  // otherwise error

  // Only do the following if core.premacros is nil
  lua_getglobal(L, "core");
  lua_getfield(L, -1, "premacros");

  if (lua_isnil(L, -1)) {
    if (system("gcc -v") != 0){
      luaL_error(L, "GCC is not installed");
    }
    // Run gcc -dM -E - < /dev/null and return the output
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    char* output = NULL;
    size_t output_len = 0;

    fp = popen("gcc -dM -E - < /dev/null", "r");
    if (fp == NULL) {
      return NULL;
    }

    while ((read = getline(&line, &len, fp)) != -1) {
      output = realloc(output, output_len + read);
      memcpy(output + output_len, line, read);
      output_len += read;
    }

    pclose(fp);
    free(line);

    if (output == NULL) {
      return NULL;
    }

    output[output_len] = '\0';

    // Format the output
    // 1- Split by \n
    // 2- Remove #define from each line
    // 3- Make a new table and for every item in the old table add another table in the newtable with {item:split(" ")[1] = item:split(" ")[2]}
    char* token = strtok(output, "\n");
    lua_newtable(L);
    int i = 1;
    while (token != NULL) {
      char* define = strstr(token, "#define");
      if (define != NULL) {
        lua_pushstring(L, define + 8);
        lua_seti(L, -2, i);
        i++;
      }
      token = strtok(NULL, "\n");
    }
    
    free(output);
  }

  // Write the table to core.premacros
  lua_getglobal(L, "core");
  lua_pushvalue(L, -2);
  lua_setfield(L, -2, "premacros");


  if (lua_isstring(L, 1)) {
    // core.premacros is a table of strings
    // ["A 1", "B 2", etc.]
    // So if A is requested, return 1
    // If B is requested, return 2

    // Go to core.premacros
    lua_getglobal(L, "core");
    lua_getfield(L, -1, "premacros");
    
    // Get the string
    const char* macro = lua_tostring(L, 1);
    // Iterate over the table
    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
      // Split the item by space
      char* item = lua_tostring(L, -1);
      char* token = strtok(item, " ");
      // if the token is the macro return the item 
      if (strcmp(token, macro) == 0) {
        lua_pushstring(L, item + strlen(token) + 1);
        return 1;
      }

      // Remove value, keep key for next iteration
      lua_pop(L, 1);
    }
    // If we get here, the macro was not found
    // Return nil
    lua_pushnil(L);

    return 1;
  }
  // Return core.premacros
  lua_getglobal(L, "core");
  lua_getfield(L, -1, "premacros");

  return 1;
}

// ERRORS:
#include <errno.h>
static int error(lua_State* L){
  // It has 2 optional arguments, error message, and exit code. It uses perror and exits with the exit code
  const char* message = luaL_optstring(L, 1, NULL);
  int exit_code = luaL_optinteger(L, 2, 1);
  printf("%s\n", message);
  exit(exit_code);

  return 0;
}

// It is in the Cobalt C errors
char cobalt_errors[][256] = {
  // TYPECHECKER
  "Type error",
  //// EXPECTIONS
  "Expected %s, got %s for argument %d",
  "Expected %s, got %s",
  "Expected %s, got %s for argument %d of %s",
  "Expected %s, for variable %s",

  // MEMORY
  "Failed to allocate memory",
  "Failed to set memory permissions",
  "Failed to free memory",
  "Failed to resize memory",

  // OTHER
  "Undisclosed error",
  "error1",
  "error2",
  "error3",
  "error4",
  "error5",
  ""
};

static int cobaltstrerror(lua_State* L){
  // Uses errno.h and returns the error message
  int errnum = luaL_checkinteger(L, 1);
  if (errnum <= sys_nerr) {
    lua_pushstring(L, strerror(errnum));
  }else{
    if (errnum <= sys_nerr+sizeof(cobalt_errors)/256) {
      lua_pushstring(L, cobalt_errors[errnum-sys_nerr]);
    }else{
      lua_pushstring(L, "Unknown error");
    }
  }
  return 1;
}
static const errorlen(lua_State* L){
  if (luaL_optstring(L, 1, NULL) == "C") {
    lua_pushinteger(L, sys_nerr);
  }else{
    lua_pushinteger(L, sys_nerr+sizeof(cobalt_errors)/256);
  }
  return 1;
}
static const struct luaL_Reg lcinterface_lib[] = {
  {"macros", dumpmacros},
  {NULL, NULL}
};

static const struct luaL_Reg lcinterface_error_lib[] = {
  {"cerror", error},
  {"strerror", cobaltstrerror},
  {"errorlen", errorlen},
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
  luaL_newlib(L, lcinterface_error_lib);
  lua_setfield(L, -2, "error");

  return 1;
}