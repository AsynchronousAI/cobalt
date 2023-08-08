/*
** Standard Color library for cobalt
*/

#define lcolorlib_c
#define LUA_LIB


#include <stdlib.h>
#include <math.h>
#include <string.h>



#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "lj_obj.h"
#include "lj_err.h"
#include "lj_buf.h"
#include "lj_strscan.h"
#include "lj_strfmt.h"
#if LJ_HASFFI
#include "lj_ctype.h"
#include "lj_cdata.h"
#include "lj_cconv.h"
#include "lj_carith.h"
#endif
#include "lj_ff.h"
#include "lj_lib.h"
#include "classes.h"
/*
TO BE ADDED FUNCTIONS:
Color->to_rgb
Color->hue_offset
Color->complementary
Color->neighbors
Color->triadic
Color->split_complementary
Color->desaturate_to
Color->desaturate_by
Color->lighten_to
Color->lighten_by
Color->variations
Color->tints
Color->shades
Color->tint
Color->shade
*/


static int color_index(lua_State *L) {
  Color* color = (Color*)lua_touserdata(L, 1);
  const char* key = luaL_checkstring(L, 2);
  if (strcmp(key, "R") == 0) {
    lua_pushinteger(L, color->r);
    return 1;
  } else if (strcmp(key, "G") == 0) {
    lua_pushinteger(L, color->g);
    return 1;
  } else if (strcmp(key, "B") == 0) {
    lua_pushinteger(L, color->b);
    return 1;
  }
  return 0;
}

static void push_color(lua_State *L, int r, int g, int b) {
  Color* color = (Color*)lua_newuserdata(L, sizeof(Color));
  color->r = r;
  color->g = g;
  color->b = b;
  if (luaL_newmetatable(L, "color")) {
    lua_pushcfunction(L, color_index);
    lua_setfield(L, -2, "__index");
  }
  lua_setmetatable(L, -2);
}

static void hsl_to_rgb(double h, double s, double l, int* r, int* g, int* b) {
    double c = (1 - fabs(2 * l - 1)) * s;
    double x = c * (1 - fabs(fmod(h / 60, 2) - 1));
    double m = l - c / 2;
    double r1, g1, b1;
    if (h < 60) {
        r1 = c;
        g1 = x;
        b1 = 0;
    } else if (h < 120) {
        r1 = x;
        g1 = c;
        b1 = 0;
    } else if (h < 180) {
        r1 = 0;
        g1 = c;
        b1 = x;
    } else if (h < 240) {
        r1 = 0;
        g1 = x;
        b1 = c;
    } else if (h < 300) {
        r1 = x;
        g1 = 0;
        b1 = c;
    } else {
        r1 = c;
        g1 = 0;
        b1 = x;
    }
    *r = (int)((r1 + m) * 255);
    *g = (int)((g1 + m) * 255);
    *b = (int)((b1 + m) * 255);
}

static void rgb_to_hsl(int r, int g, int b, double* h, double* s, double* l) {
    double r1 = (double)r / 255;
    double g1 = (double)g / 255;
    double b1 = (double)b / 255;
    double max = r1;
    if (g1 > max) {
        max = g1;
    }
    if (b1 > max) {
        max = b1;
    }
    double min = r1;
    if (g1 < min) {
        min = g1;
    }
    if (b1 < min) {
        min = b1;
    }
    double c = max - min;
    double h1;
    if (c == 0) {
        h1 = 0;
    } else if (max == r1) {
        h1 = fmod((g1 - b1) / c, 6);
    } else if (max == g1) {
        h1 = (b1 - r1) / c + 2;
    } else {
        h1 = (r1 - g1) / c + 4;
    }
    *h = h1 * 60;
    *l = (max + min) / 2;
    if (c == 0) {
        *s = 0;
    } else {
        *s = c / (1 - fabs(2 * *l - 1));
    }
}

static int color_hsl_to_rgb (lua_State *L) {
  double h = luaL_checknumber(L, 1);
  double s = luaL_checknumber(L, 2);
  double l = luaL_checknumber(L, 3);
  int r, g, b;
  hsl_to_rgb(h, s, l, &r, &g, &b);
  push_color(L, r, g, b);
  return 1;
}

static int string(lua_State *L) {
    const char *rgb_string = luaL_checkstring(L, 1);

    int r, g, b;
    if (sscanf(rgb_string, "#%02x%02x%02x", &r, &g, &b) != 3) {
        luaL_error(L, "invalid RGB string format");
    }
    
    push_color(L, r, g, b);

    return 1;
}

static int color_rgb_to_hsl (lua_State *L) {
  int r = luaL_checknumber(L, 1);
  int g = luaL_checknumber(L, 2);
  int b = luaL_checknumber(L, 3);
  double h, s, l;
  rgb_to_hsl(r, g, b, &h, &s, &l);
  lua_pushnumber(L, h);
  lua_pushnumber(L, s);
  lua_pushnumber(L, l);
  return 3;
}


static int color_rgb (lua_State *L) {
  int r = luaL_checknumber(L, 1);
  int g = luaL_checknumber(L, 2);
  int b = luaL_checknumber(L, 3);
  push_color(L, r, g, b);
  return 1;
}

static void hex_to_rgb(const char *hex, int *r, int *g, int *b) {
    if (strlen(hex) != 6) {
        fprintf(stderr, "Invalid hex color format: %s\n", hex);
        exit(1);
    }

    if (sscanf(hex, "%2x%2x%2x", r, g, b) != 3) {
        fprintf(stderr, "Invalid hex color format: %s\n", hex);
        exit(1);
    }
}

static int color_fromhex (lua_State *L) {
  const char *hex = luaL_checkstring(L, 1);
  // if the hex starts with a #, remove it
  if (hex[0] == '#') {
    hex++;
  }
  
  int r, g, b;
  hex_to_rgb(hex, &r, &g, &b);
  push_color(L, r, g, b);
}




static const luaL_Reg colorlib[] = {
  {"new", color_rgb},
  {"hex", color_fromhex},
  {"hsl_to_rgb", color_hsl_to_rgb},
  {"rgb_to_hsl", color_rgb_to_hsl},
  {"string", string},

  {NULL, NULL}
};


/*
** Open color library
*/
LUALIB_API int luaopen_color (lua_State *L) {
  luaL_newlib(L, colorlib);
  push_color(L, 0,0,0);
  lua_setfield(L, -2, "black");
  push_color(L, 255,255,255);
  lua_setfield(L, -2, "white");
  return 1;
}

