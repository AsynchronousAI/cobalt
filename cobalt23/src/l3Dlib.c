/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#define l3Dlib_c
#define LUA_LIB

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "cobalt.h"
#include "lauxlib.h"
#include "lclasses.h"
#include "lprefix.h"
#include "lualib.h"

static int vec3_index(lua_State* L) {
  Vec3* vec3 = (Vec3*)lua_touserdata(L, 1);
  const char* key = luaL_checkstring(L, 2);
  if (strcmp(key, "X") == 0) {
    lua_pushnumber(L, vec3->x);
    return 1;
  } else if (strcmp(key, "Y") == 0) {
    lua_pushnumber(L, vec3->y);
    return 1;
  } else if (strcmp(key, "Z") == 0) {
    lua_pushnumber(L, vec3->z);
    return 1;
  }
  return 0;
}

static int push_vec3(lua_State* L, float x, float y, float z) {
  Vec3* vec3 = (Vec3*)lua_newuserdata(L, sizeof(Vec3));
  vec3->x = x;
  vec3->y = y;
  vec3->z = z;
  if (luaL_newmetatable(L, "vec3")) {
    lua_pushcfunction(L, vec3_index);
    lua_setfield(L, -2, "__index");
  }
  lua_setmetatable(L, -2);
  return 1;
}

static int vec3_add(lua_State* L) {
  Vec3* a = (Vec3*)lua_touserdata(L, 1);
  Vec3* b = (Vec3*)lua_touserdata(L, 2);
  push_vec3(L, a->x + b->x, a->y + b->y, a->z + b->z);
  return 1;
}

static int vec3_sub(lua_State* L) {
  Vec3* a = (Vec3*)lua_touserdata(L, 1);
  Vec3* b = (Vec3*)lua_touserdata(L, 2);
  push_vec3(L, a->x - b->x, a->y - b->y, a->z - b->z);
  return 1;
}

static int vec3_mul(lua_State* L) {
  Vec3* a = (Vec3*)lua_touserdata(L, 1);
  float b = luaL_checknumber(L, 2);
  push_vec3(L, a->x * b, a->y * b, a->z * b);
  return 1;
}

static int vec3_div(lua_State* L) {
  Vec3* a = (Vec3*)lua_touserdata(L, 1);
  float b = luaL_checknumber(L, 2);
  push_vec3(L, a->x / b, a->y / b, a->z / b);
  return 1;
}

static int vec3_dot(lua_State* L) {
  Vec3* a = (Vec3*)lua_touserdata(L, 1);
  Vec3* b = (Vec3*)lua_touserdata(L, 2);
  lua_pushnumber(L, a->x * b->x + a->y * b->y + a->z * b->z);
  return 1;
}

static int vec3_cross(lua_State* L) {
  Vec3* a = (Vec3*)lua_touserdata(L, 1);
  Vec3* b = (Vec3*)lua_touserdata(L, 2);
  push_vec3(L, a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z,
            a->x * b->y - a->y * b->x);
  return 1;
}

static int vec3_length(lua_State* L) {
  Vec3* a = (Vec3*)lua_touserdata(L, 1);
  lua_pushnumber(L, sqrt(a->x * a->x + a->y * a->y + a->z * a->z));
  return 1;
}

static int vec3_normalize(lua_State* L) {
  Vec3* a = (Vec3*)lua_touserdata(L, 1);
  float length = sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
  push_vec3(L, a->x / length, a->y / length, a->z / length);
  return 1;
}

static int vec3_angle(lua_State* L) {
  Vec3* a = (Vec3*)lua_touserdata(L, 1);
  Vec3* b = (Vec3*)lua_touserdata(L, 2);
  float dot = a->x * b->x + a->y * b->y + a->z * b->z;
  float length = sqrt(a->x * a->x + a->y * a->y + a->z * a->z) *
                 sqrt(b->x * b->x + b->y * b->y + b->z * b->z);
  lua_pushnumber(L, acos(dot / length));
  return 1;
}

static int vec3_rotate(lua_State* L) {
  Vec3* a = (Vec3*)lua_touserdata(L, 1);
  Vec3* b = (Vec3*)lua_touserdata(L, 2);
  float angle = luaL_checknumber(L, 3);
  float s = sin(angle);
  float c = cos(angle);
  float x = a->x - b->x;
  float y = a->y - b->y;
  float z = a->z - b->z;
  push_vec3(L, x * c - y * s + b->x, x * s + y * c + b->y, z);
  return 1;
}

static int vec3_new(lua_State* L) {
  float x = luaL_checknumber(L, 1);
  float y = luaL_checknumber(L, 2);
  float z = luaL_checknumber(L, 3);
  push_vec3(L, x, y, z);
  return 1;
}

static const luaL_Reg vec3_lib[] = {
    {"add", vec3_add},       {"sub", vec3_sub},
    {"mul", vec3_mul},       {"div", vec3_div},
    {"dot", vec3_dot},       {"cross", vec3_cross},
    {"length", vec3_length}, {"normalize", vec3_normalize},
    {"angle", vec3_angle},   {"rotate", vec3_rotate},
    {"new", vec3_new},       {NULL, NULL}};

LUAMOD_API int luaopen_3D(lua_State* L) {
  luaL_newlib(L, vec3_lib);
  return 1;
}
