/*
* llink.c
* Link Cobalt libraries to the C API
* Read copyright notice at cobalt.h

* Removed and replaced with the AOT compiler.
*/ 

#include "cobalt.h"
#include "lauxlib.h"
#include "lualib.h"

#include <stdio.h>
#include <stdlib.h>

#define link(file, name) \
static char* source_##name = NULL; \
static int luaopen_##name(lua_State* L) { \
    if (source_##name == NULL) { \
        FILE* file_##name = fopen(file, "r"); \
        fseek(file_##name, 0, SEEK_END); \
        long size_##name = ftell(file_##name); \
        fseek(file_##name, 0, SEEK_SET); \
        source_##name = malloc(size_##name + 1); \
        fread(source_##name, 1, size_##name, file_##name); \
        fclose(file_##name); \
        source_##name[size_##name] = '\0'; \
    } \
    luaL_dostring(L, source_##name); \
    return 1; \
}
