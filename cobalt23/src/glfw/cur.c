/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#include "internal.h"

static int cmp(cur_t *cur1, cur_t *cur2) /* the compare function */
    { return (cur1->id < cur2->id ? -1 : cur1->id > cur2->id); } 

static RB_HEAD(curtree_s, cur_s) Head = RB_INITIALIZER(&Head);

RB_PROTOTYPE_STATIC(curtree_s, cur_s, entry, cmp) 
RB_GENERATE_STATIC(curtree_s, cur_s, entry, cmp) 
 
static cur_t *cur_remove(cur_t *cur) 
    { return RB_REMOVE(curtree_s, &Head, cur); }
static cur_t *cur_insert(cur_t *cur) 
    { return RB_INSERT(curtree_s, &Head, cur); }
cur_t *cur_search(int id) 
    { cur_t tmp; tmp.id = id; return RB_FIND(curtree_s, &Head, &tmp); }
cur_t *cur_first(int id) 
    { cur_t tmp; tmp.id = id; return RB_NFIND(curtree_s, &Head, &tmp); }
cur_t *cur_next(cur_t *cur)
    { return RB_NEXT(curtree_s, &Head, cur); }
#if 0
cur_t *cur_prev(cur_t *cur)
    { return RB_PREV(curtree_s, &Head, cur); }
cur_t *cur_min(void)
    { return RB_MIN(curtree_s, &Head); }
cur_t *cur_max(void)
    { return RB_MAX(curtree_s, &Head); }
cur_t *cur_root(void)
    { return RB_ROOT(&Head); }
#endif
cur_t *cur_new(void)
    {
    cur_t *cur;
    int id = id_new();
    if(id == -1)
        return NULL;
    if((cur = (cur_t*)malloc(sizeof(cur_t))) == NULL) 
        { id_free(id); return NULL; }
    memset(cur, 0, sizeof(cur_t));
    cur->id = id;
    if(cur_search(cur->id))
        { free(cur); id_free(id); return NULL; }
    //cur->Zzz = LUA_NOREF;
    cur_insert(cur);
    return cur;
    }

void cur_free(cur_t* cur)
    {
    if(cur_search(cur->id) == cur)
        {
        /* release callback references */
        //CALLBACK_UNREF(cur->Zzz);
        cur_remove(cur);
        }
    id_free(cur->id);
    free(cur);
    }

void cur_free_all(void)
    {
    cur_t *cur;
    while((cur = cur_first(0)))
        cur_free(cur);
    }

cur_t* cur_check(lua_State *L, int arg)
    {
    int id = luaL_checkinteger(L, arg);
    cur_t *cur = cur_search(id);
    if(!cur)
        luaL_error(L, "invalid cursor id");
    return cur;
    }

