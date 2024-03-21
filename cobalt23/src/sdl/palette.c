/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "internal.h"

static int freepalette(lua_State *L, ud_t *ud) {
  palette_t *palette = (palette_t *)ud->handle;
  //  freechildren(L, _MT, ud);
  if (!freeuserdata(L, ud, "palette")) return 0;
  SDL_FreePalette(palette);
  return 0;
}

static int newpalette(lua_State *L, palette_t *palette) {
  ud_t *ud;
  ud = newuserdata(L, palette, PALETTE_MT, "palette");
  ud->parent_ud = NULL;
  ud->destructor = freepalette;
  return 1;
}

static int Create(lua_State *L) {
  int ncolors = luaL_checkinteger(L, 1);
  palette_t *palette = SDL_AllocPalette(ncolors);
  if (palette == NULL) GetError(L);
  return newpalette(L, palette);
}

static int SetColors(lua_State *L) {
  int count;
  palette_t *palette = checkpalette(L, 1, NULL);
  color_t *colors = checkcolorlist(L, 2, &count, NULL);
  int firstcolor = optindex(L, 3, 0);
  int ec = SDL_SetPaletteColors(palette, colors, firstcolor, count);
  Free(L, colors);
  if (ec != 0) CheckError(L, ec);
  return 0;
}

static int GetColors(lua_State *L) {
  palette_t *palette = checkpalette(L, 1, NULL);
  pushcolorlist(L, palette->colors, palette->ncolors);
  return 1;
}

static int NColors(lua_State *L) {
  palette_t *palette = checkpalette(L, 1, NULL);
  lua_pushinteger(L, palette->ncolors);
  return 1;
}

DESTROY_FUNC(palette)

static const struct luaL_Reg Methods[] = {
    {"free", Destroy},
    {"set_colors", SetColors},
    {"get_colors", GetColors},
    {"ncolors", NColors},
    {NULL, NULL} /* sentinel */
};

static const struct luaL_Reg MetaMethods[] = {
    {"__gc", Destroy}, {NULL, NULL} /* sentinel */
};

static const struct luaL_Reg Functions[] = {
    {"alloc_palette", Create}, {NULL, NULL} /* sentinel */
};

void moonsdl2_open_palette(lua_State *L) {
  udata_define(L, PALETTE_MT, Methods, MetaMethods);
  luaL_setfuncs(L, Functions, 0);
}
