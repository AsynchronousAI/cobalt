/*
** Standard Color library for luax
*/

#define lcolorlib_c
#define LUA_LIB

#include "lprefix.h"


#include <stdlib.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


typedef struct {
  int r;
  int g;
  int b;
} Color;



static int color_hsv (lua_State *L) {
  int h = luaL_checknumber(L, 1);
  int s = luaL_checknumber(L, 2);
  int v = luaL_checknumber(L, 3);
  lua_pushnumber(L, -1);
  return 1;
}

static void push_color(lua_State *L, int r, int g, int b) {
  Color* color = (Color*)lua_newuserdata(L, sizeof(Color));
  color->r = r;
  color->g = g;
  color->b = b;
  luaL_getmetatable(L, "color");
  lua_setmetatable(L, -2);
}

static int hue2rgb(int p, int q, int t) {
  if (t < 0) t += 1;
  if (t > 1) t -= 1;
  if (t < 1/6) return p + (q - p) * 6 * t;
  if (t < 1/2) return q;
  if (t < 2/3) return p + (q - p) * (2/3 - t) * 6;
  return p;
}

static int color_rgb (lua_State *L) {
  int r = luaL_checknumber(L, 1);
  int g = luaL_checknumber(L, 2);
  int b = luaL_checknumber(L, 3);
  push_color(L, r, g, b);
  return 1;
}

static int color_hsl (lua_State *L) {
  int h = luaL_checknumber(L, 1);
  int s = luaL_checknumber(L, 2);
  int l = luaL_checknumber(L, 3);
  
  int r, g, b;
  if (s == 0) {
    r = g = b = l; // achromatic
  } else {
    int q = l < 0.5 ? l * (1 + s) : l + s - l * s;
    int p = 2 * l - q;
    r = hue2rgb(p, q, h + 1/3);
    g = hue2rgb(p, q, h);
    b = hue2rgb(p, q, h - 1/3);
  }

  push_color(L, r, g, b);
  return 1;
}

static int color_cmyk (lua_State *L) {
  int c = luaL_checknumber(L, 1);
  int m = luaL_checknumber(L, 2);
  int y = luaL_checknumber(L, 3);
  int k = luaL_checknumber(L, 4);

  int r = 255 * (1 - c) * (1 - k);
  int g = 255 * (1 - m) * (1 - k);
  int b = 255 * (1 - y) * (1 - k);
  
  push_color(L, r, g, b);
  return 1;
}

static int color_fromhex (lua_State *L) {
  const char *hex = luaL_checkstring(L, 1);
  int r, g, b;
  sscanf(hex, "%02x%02x%02x", &r, &g, &b);
  push_color(L, r, g, b);
  return 1;
}







static const luaL_Reg colorlib[] = {
  {"hsv", color_hsv},
  {"rgb", color_rgb},
  {"hsl", color_hsl},
  {"cmyk", color_cmyk},
  {"fromHex", color_fromhex},

  {NULL, NULL}
};


/*
** Open color library
*/
LUAMOD_API int luaopen_color (lua_State *L) {
  luaL_newlib(L, colorlib);
  push_color(L, 0,0,0);
  lua_setfield(L, -2, "black");
  push_color(L, 255,255,255);
  lua_setfield(L, -2, "white");
  return 1;
}

