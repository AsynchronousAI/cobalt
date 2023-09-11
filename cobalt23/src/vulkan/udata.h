/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "../cobalt.h"
#include "../lauxlib.h"

#ifndef Malloc
#define Malloc moonvulkan_Malloc
void *Malloc(lua_State *L, size_t size);
#define Free moonvulkan_Free
void Free(lua_State *L, void *ptr);
#endif

#define udata_t  moonvulkan_udata_t
#define udata_s  moonvulkan_udata_s
#define moonvulkan_udata_t struct moonvulkan_udata_s

#define udata_new moonvulkan_udata_new
void *udata_new(lua_State*, size_t, uint64_t, const char*);
#define udata_unref moonvulkan_udata_unref
int udata_unref(lua_State *L, uint64_t);
#define udata_free moonvulkan_udata_free
int udata_free(lua_State*, uint64_t);
#define udata_mem moonvulkan_udata_mem
void *udata_mem(uint64_t);
#define udata_push moonvulkan_udata_push
int udata_push(lua_State*, uint64_t);
#define udata_free_all moonvulkan_udata_free_all
void udata_free_all(lua_State *L);
#define udata_scan moonvulkan_udata_scan
int udata_scan(lua_State *L, const char *mt,  
            void *info, int (*func)(lua_State *L, const void *mem, const char* mt, const void *info));
#define udata_define moonvulkan_udata_define
int udata_define(lua_State*, const char*, const luaL_Reg*, const luaL_Reg*);
#define udata_inherit moonvulkan_udata_inherit
int udata_inherit(lua_State*, const char*, const char*);
#define udata_test moonvulkan_udata_test
void *udata_test(lua_State*, int, const char*);
#define udata_addmethods moonvulkan_udata_addmethods
int udata_addmethods(lua_State*, const char*, const luaL_Reg*);

#ifdef __cplusplus
}
#endif

