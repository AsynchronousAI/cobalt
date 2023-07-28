#include <stdlib.h>
#include <string.h>
#include "lua.h"
#include "lauxlib.h"

typedef struct {
    void *ptr;
} pointer_t;

static int l_alloc(lua_State *L) {
    size_t size = luaL_checkinteger(L, 1);
    pointer_t *ptr = (pointer_t *)lua_newuserdata(L, sizeof(pointer_t));
    ptr->ptr = malloc(size);
    luaL_getmetatable(L, "pointer");
    lua_setmetatable(L, -2);
    return 1;
}

static int l_free(lua_State *L) {
    pointer_t *ptr = (pointer_t *)luaL_checkudata(L, 1, "pointer");
    free(ptr->ptr);
    return 0;
}

static int l_set(lua_State *L) {
    pointer_t *ptr = (pointer_t *)luaL_checkudata(L, 1, "pointer");
    size_t offset = luaL_checkinteger(L, 2);
    size_t size = luaL_checkinteger(L, 3);
    const void *data = luaL_checkstring(L, 4);
    memcpy((char *)ptr->ptr + offset, data, size);
    return 0;
}

static int l_get(lua_State *L) {
    pointer_t *ptr = (pointer_t *)luaL_checkudata(L, 1, "pointer");
    size_t offset = luaL_checkinteger(L, 2);
    size_t size = luaL_checkinteger(L, 3);
    char *data = (char *)malloc(size);
    memcpy(data, (char *)ptr->ptr + offset, size);
    lua_pushlstring(L, data, size);
    free(data);
    return 1;
}

static const struct luaL_Reg lib[] = {
    {"alloc", l_alloc},
    {"free", l_free},
    {"set", l_set},
    {"get", l_get},
    {NULL, NULL}
};

int luaopen_memory(lua_State *L) {
    luaL_newmetatable(L, "pointer");
    lua_pushcfunction(L, l_free);
    lua_setfield(L, -2, "__gc");
    luaL_newlib(L, lib);
    return 1;
}