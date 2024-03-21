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
#define Malloc moonsdl2_Malloc
void *Malloc(lua_State *L, size_t size);
#define Free moonsdl2_Free
void Free(lua_State *L, void *ptr);
#endif

#define udata_t moonsdl2_udata_t
#define udata_s moonsdl2_udata_s
#define moonsdl2_udata_t struct moonsdl2_udata_s

#define udata_new moonsdl2_udata_new
void *udata_new(lua_State *, size_t, uint64_t, const char *);
#define udata_unref moonsdl2_udata_unref
int udata_unref(lua_State *L, uint64_t);
#define udata_free moonsdl2_udata_free
int udata_free(lua_State *, uint64_t);
#define udata_mem moonsdl2_udata_mem
void *udata_mem(uint64_t);
#define udata_push moonsdl2_udata_push
int udata_push(lua_State *, uint64_t);
#define udata_free_all moonsdl2_udata_free_all
void udata_free_all(lua_State *L);
#define udata_scan moonsdl2_udata_scan
int udata_scan(lua_State *L, const char *mt, void *info,
               int (*func)(lua_State *L, const void *mem, const char *mt,
                           const void *info));
#define udata_define moonsdl2_udata_define
int udata_define(lua_State *, const char *, const luaL_Reg *, const luaL_Reg *);
#define udata_inherit moonsdl2_udata_inherit
int udata_inherit(lua_State *, const char *, const char *);
#define udata_test moonsdl2_udata_test
void *udata_test(lua_State *, int, const char *);
#define udata_addmethods moonsdl2_udata_addmethods
int udata_addmethods(lua_State *, const char *, const luaL_Reg *);

#ifdef __cplusplus
}
#endif
