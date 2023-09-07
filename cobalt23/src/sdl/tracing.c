/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "internal.h"

int trace_objects = 0;

static int TraceObjects(lua_State *L) {
  trace_objects = checkboolean(L, 1);
  return 0;
}

static int Now(lua_State *L) {
  lua_pushnumber(L, now());
  return 1;
}

static int Since(lua_State *L) {
  double t = luaL_checknumber(L, 1);
  lua_pushnumber(L, since(t));
  return 1;
}

static int Sleep(lua_State *L) {
  double seconds = luaL_checknumber(L, 1);
  sleeep(seconds);
  return 0;
}

static int GetTicks(lua_State *L)  // use now() instead
{
  pushmilliseconds(L, SDL_GetTicks());
  return 1;
}

static int TicksPassed(lua_State *L)  // use since() instead
{
  uint32_t a = checkmilliseconds(L, 1);
  uint32_t b = checkmilliseconds(L, 2);
  pushmilliseconds(L, SDL_TICKS_PASSED(a, b));
  return 1;
}

static int Delay(lua_State *L)  // use sleep() instead
{
  uint32_t ms = checkmilliseconds(L, 1);
  SDL_Delay(ms);
  return 0;
}

static int GetPerformanceCounter(lua_State *L) {
  lua_pushinteger(L, SDL_GetPerformanceCounter());
  return 1;
}

static int GetPerformanceFrequency(lua_State *L) {
  lua_pushinteger(L, SDL_GetPerformanceFrequency());
  return 1;
}

/* ----------------------------------------------------------------------- */

static const struct luaL_Reg Functions[] = {
    {"trace_objects", TraceObjects},
    {"now", Now},
    {"since", Since},
    {"sleep", Sleep},
    {"get_ticks", GetTicks},
    {"ticks_passed", TicksPassed},
    {"delay", Delay},
    {"get_performance_counter", GetPerformanceCounter},
    {"get_performance_frequency", GetPerformanceFrequency},
    {NULL, NULL} /* sentinel */
};

void moonsdl2_open_tracing(lua_State *L) { luaL_setfuncs(L, Functions, 0); }
