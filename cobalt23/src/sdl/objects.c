/* The MIT License (MIT)
 *
 * Copyright (c) 2023 Stefano Trettel
 *
 * Software repository: MoonSDL2, https://github.com/stetre/moonsdl2
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "internal.h"

ud_t *newuserdata(lua_State *L, void *handle, const char *mt, const char *tracename)
    {
    ud_t *ud;
    /* we use handle as search key */
    ud = (ud_t*)udata_new(L, sizeof(ud_t), (uint64_t)(uintptr_t)handle, mt);
    memset(ud, 0, sizeof(ud_t));
    ud->handle = handle;
    ud->ref1 = ud->ref2 = ud->ref3 = ud->ref4 = LUA_NOREF;
    MarkValid(ud);
    if(trace_objects)
        printf("create %s %p (%p)\n", tracename, (void*)ud, handle);
    return ud;
    }

int freeuserdata(lua_State *L, ud_t *ud, const char *tracename)
    {
    /* The 'Valid' mark prevents double calls when an object is explicitly destroyed, 
     * and subsequently deleted also by the GC (the ud sticks around until the GC
     * collects it, so we mark it as invalid when the object is explicitly destroyed
     * by the script, or implicitly destroyed because child of a destroyed object). */
    if(!IsValid(ud)) return 0;
    CancelValid(ud);
    if(ud->info) 
        Free(L, ud->info);
    Unreference(L, ud->ref1);
    Unreference(L, ud->ref2);
    Unreference(L, ud->ref3);
    Unreference(L, ud->ref4);
    if(trace_objects)
        printf("delete %s %p (%p)\n", tracename, (void*)ud, ud->handle);
    udata_free(L, (uint64_t)(uintptr_t)ud->handle);
    return 1;
    }

static int freeifchild(lua_State *L, const void *mem, const char *mt, const void *parent_ud)
/* callback for udata_scan */
    {
    ud_t *ud = (ud_t*)mem;
    (void)mt;
    if(IsValid(ud) && (ud->parent_ud == parent_ud))
        ud->destructor(L, ud);
    return 0;
    }

int freechildren(lua_State *L,  const char *mt, ud_t *parent_ud)
/* calls the self destructor for all 'mt' objects that are children of the given parent_ud */
    {
    return udata_scan(L, mt, parent_ud, freeifchild);
    }

int pushuserdata(lua_State *L, ud_t *ud)
    {
    if(!IsValid(ud)) return unexpected(L);
    return udata_push(L, (uint64_t)(uintptr_t)ud->handle);
    }

ud_t *userdata(void *handle)
    {
    ud_t *ud = (ud_t*)udata_mem((uint64_t)(uintptr_t)handle);
    if(ud && IsValid(ud)) return ud;
    return NULL;
    }

void *testxxx(lua_State *L, int arg, ud_t **udp, const char *mt)
    {
    ud_t *ud = (ud_t*)udata_test(L, arg, mt);
    if(ud && IsValid(ud)) { if(udp) *udp=ud; return ud->handle; }
    if(udp) *udp = NULL;
    return 0;
    }

#if 0
void *testoneofxxx(lua_State *L, int arg, ud_t **udp, char **mtp)
    {
    void *handle = NULL;
    int i = 0;
    char *mt = NULL;
    while((mt = mtp[i++]) != NULL)
        {
        handle = testxxx(L, arg, udp, mt);
        if(handle) return handle;
        }
    if(udp) *udp = NULL;
    return 0;
    }
#endif


void *checkxxx(lua_State *L, int arg, ud_t **udp, const char *mt)
    {
    ud_t *ud = (ud_t*)udata_test(L, arg, mt);
    if(ud && IsValid(ud)) 
        { if(udp) *udp = ud; return ud->handle; }
    lua_pushfstring(L, "not a %s", mt);
    luaL_argerror(L, arg, lua_tostring(L, -1));
    return 0;
    }

void *optxxx(lua_State *L, int arg, ud_t **udp, const char *mt)
/* This differs from testxxx in that it fails in case the argument is
 * something different than either none/nil or the expected userdata type
 */
    {
    if(lua_isnoneornil(L, arg))
        { if(udp) *udp = NULL; return 0; }
    return checkxxx(L, arg, udp, mt);
    }

int pushxxx(lua_State *L, void *handle)
    { return udata_push(L, (uint64_t)(uintptr_t)handle); }

int setmetatable(lua_State *L, const char *mt)
/* Sets the metatable of the table on top of the stack */
    {
    luaL_getmetatable(L, mt);
    lua_setmetatable(L, -2);
    return 0;
    }

