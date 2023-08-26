/*
* llink.c
* Link Cobalt libraries to the C API
* Read copyright notice at cobalt.h
*/

#include "cobalt.h"
#include "lauxlib.h"
#include "lualib.h"

#define link(file, name) \
static int luaopen_##name(lua_State* L) { \
    luaL_dofile(L, file); \
    return 1; \
}