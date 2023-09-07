/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#define l3dtransform_c
#define LUA_LIB

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "cobalt.h"
#include "lauxlib.h"
#include "lclasses.h"
#include "lprefix.h"
#include "lualib.h"

int default_transform[12] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0};

static int transform_index(lua_State* L) {
  Transform* transform = (Transform*)lua_touserdata(L, 1);
  const char* key = luaL_checkstring(L, 2);
  if (strcmp(key, "X") == 0) {
    lua_pushnumber(L, transform->x);
    return 1;
  } else if (strcmp(key, "Y") == 0) {
    lua_pushnumber(L, transform->y);
    return 1;
  } else if (strcmp(key, "Z") == 0) {
    lua_pushnumber(L, transform->z);
    return 1;
  } else if (strcmp(key, "OX") == 0) {
    lua_pushnumber(L, transform->ox);
    return 1;
  } else if (strcmp(key, "OY") == 0) {
    lua_pushnumber(L, transform->oy);
    return 1;
  } else if (strcmp(key, "OZ") == 0) {
    lua_pushnumber(L, transform->oz);
    return 1;
  } else if (strcmp(key, "SX") == 0) {
    lua_pushnumber(L, transform->sx);
    return 1;
  } else if (strcmp(key, "SY") == 0) {
    lua_pushnumber(L, transform->sy);
    return 1;
  } else if (strcmp(key, "SZ") == 0) {
    lua_pushnumber(L, transform->sz);
    return 1;
  } else if (strcmp(key, "PX") == 0) {
    lua_pushnumber(L, transform->px);
    return 1;
  } else if (strcmp(key, "PY") == 0) {
    lua_pushnumber(L, transform->py);
    return 1;
  } else if (strcmp(key, "PZ") == 0) {
    lua_pushnumber(L, transform->pz);
    return 1;
  }
  return 0;
}

static int push_transform(lua_State* L, int x, int y, int z, int ox, int oy,
                          int oz, int sx, int sy, int sz, int px, int py,
                          int pz) {
  Transform* transform = (Transform*)lua_newuserdata(L, sizeof(Transform));
  transform->x = x ?: default_transform[0];
  transform->y = y ?: default_transform[1];
  transform->z = z ?: default_transform[2];
  transform->ox = ox ?: default_transform[3];
  transform->oy = oy ?: default_transform[4];
  transform->oz = oz ?: default_transform[5];
  transform->sx = sx ?: default_transform[6];
  transform->sy = sy ?: default_transform[7];
  transform->sz = sz ?: default_transform[8];
  transform->px = px ?: default_transform[9];
  transform->py = py ?: default_transform[10];
  transform->pz = pz ?: default_transform[11];

  if (luaL_newmetatable(L, "transform")) {
    lua_pushcfunction(L, transform_index);
    lua_setfield(L, -2, "__index");
  }
  lua_setmetatable(L, -2);
  return 1;
}

static int transform_new(lua_State* L) {
  // Input can be in many forms:
  // Values can be ints or Vec3 userdatas (x,y,z).
  // Will be organized like this:
  // pos, orient, scale, pivot
  // if any of them are -1 that means default value
  // pos, orient, scale, pivot can be a a Vec3 each or 3 ints each

  int x = default_transform[0];
  int y = default_transform[1];
  int z = default_transform[2];
  int ox = default_transform[3];
  int oy = default_transform[4];
  int oz = default_transform[5];
  int sx = default_transform[6];
  int sy = default_transform[7];
  int sz = default_transform[8];
  int px = default_transform[9];
  int py = default_transform[10];
  int pz = default_transform[11];

  int nargs = lua_gettop(L);

  if (nargs >= 1) {
    if (lua_isnumber(L, 1)) {
      x = lua_tointeger(L, 1);
      if (nargs >= 2 && lua_isnumber(L, 2)) {
        y = lua_tointeger(L, 2);
        if (nargs >= 3 && lua_isnumber(L, 3)) {
          z = lua_tointeger(L, 3);
        }
      }
    } else if (lua_isuserdata(L, 1)) {
      Vec3* vec = (Vec3*)lua_touserdata(L, 1);
      x = vec->x;
      y = vec->y;
      z = vec->z;
    }
  }

  if (nargs >= 4) {
    if (lua_isnumber(L, 4)) {
      ox = lua_tointeger(L, 4);
      if (nargs >= 5 && lua_isnumber(L, 5)) {
        oy = lua_tointeger(L, 5);
        if (nargs >= 6 && lua_isnumber(L, 6)) {
          oz = lua_tointeger(L, 6);
        }
      }
    } else if (lua_isuserdata(L, 4)) {
      Vec3* vec = (Vec3*)lua_touserdata(L, 4);
      ox = vec->x;
      oy = vec->y;
      oz = vec->z;
    }
  }

  if (nargs >= 7) {
    if (lua_isnumber(L, 7)) {
      sx = lua_tointeger(L, 7);
      if (nargs >= 8 && lua_isnumber(L, 8)) {
        sy = lua_tointeger(L, 8);
        if (nargs >= 9 && lua_isnumber(L, 9)) {
          sz = lua_tointeger(L, 9);
        }
      }
    } else if (lua_isuserdata(L, 7)) {
      Vec3* vec = (Vec3*)lua_touserdata(L, 7);
      sx = vec->x;
      sy = vec->y;
      sz = vec->z;
    }
  }

  if (nargs >= 10) {
    if (lua_isnumber(L, 10)) {
      px = lua_tointeger(L, 10);
      if (nargs >= 11 && lua_isnumber(L, 11)) {
        py = lua_tointeger(L, 11);
        if (nargs >= 12 && lua_isnumber(L, 12)) {
          pz = lua_tointeger(L, 12);
        }
      }
    } else if (lua_isuserdata(L, 10)) {
      Vec3* vec = (Vec3*)lua_touserdata(L, 10);
      px = vec->x;
      py = vec->y;
      pz = vec->z;
    }
  }
  push_transform(L, x, y, z, ox, oy, oz, sx, sy, sz, px, py, pz);
  return 1;
}

static int transform_add(lua_State* L) {
  Transform* a = (Transform*)lua_touserdata(L, 1);
  Transform* b = (Transform*)lua_touserdata(L, 2);

  push_transform(L, a->x + b->x, a->y + b->y, a->z + b->z, a->ox + b->ox,
                 a->oy + b->oy, a->oz + b->oz, a->sx + b->sx, a->sy + b->sy,
                 a->sz + b->sz, a->px + b->px, a->py + b->py, a->pz + b->pz);
  return 1;
}
static int transform_sub(lua_State* L) {
  Transform* a = (Transform*)lua_touserdata(L, 1);
  Transform* b = (Transform*)lua_touserdata(L, 2);

  push_transform(L, a->x - b->x, a->y - b->y, a->z - b->z, a->ox - b->ox,
                 a->oy - b->oy, a->oz - b->oz, a->sx - b->sx, a->sy - b->sy,
                 a->sz - b->sz, a->px - b->px, a->py - b->py, a->pz - b->pz);
  return 1;
}

static const luaL_Reg lib[] = {{"new", transform_new},
                               {"add", transform_add},
                               {"sub", transform_sub},
                               {NULL, NULL}};

LUAMOD_API int luaopen_transform(lua_State* L) {
  luaL_newlib(L, lib);

  push_transform(L, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0);
  lua_setfield(L, -2, "zero");
  push_transform(L, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0);
  lua_setfield(L, -2, "one");
  push_transform(
      L, default_transform[0], default_transform[1], default_transform[2],
      default_transform[3], default_transform[4], default_transform[5],
      default_transform[6], default_transform[7], default_transform[8],
      default_transform[9], default_transform[10], default_transform[11]);
  lua_setfield(L, -2, "default");
  return 1;
}