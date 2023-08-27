/*
* llink.c
* Link Cobalt libraries to the C API
* Read copyright notice at cobalt.h
*/

#include "cobalt.h"
#include "lauxlib.h"
#include "lualib.h"

#define link(file, name) \
FILE* file = fopen(name, "r"); \
fseek(file, 0, SEEK_END); \
long size = ftell(file); \
fseek(file, 0, SEEK_SET); \
char* source = malloc(size + 1); \
fread(source, 1, size, file); \
fclose(file); \
static int luaopen_##name(lua_State* L) { \
    luaL_dostring(L, source); \
    return 1; \
}
