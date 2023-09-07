/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


/*
Read only tables, saves memory by not having to copy the table
*/

#ifndef lrotable_h
#define lrotable_h

#include "cobalt.h"
#include "lauxlib.h"
#include "llimits.h"

typedef lua_Number luaR_result;

// A number entry in the read only table
typedef struct {
  const char *name;
  lua_Number value;
} luaR_value_entry;

// A mapping between table name and its entries
typedef struct {
  const char *name;
  const luaL_reg *pfuncs;
  const luaR_value_entry *pvalues;
} luaR_table;

luaR_result luaR_findglobal(const char *key, lu_byte *ptype);
int luaR_findfunction(lua_State *L, const luaL_reg *ptable);
luaR_result luaR_findentry(void *data, const char *key, lu_byte *ptype);

#endif