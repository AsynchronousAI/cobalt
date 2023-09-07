/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#define lmathlib_c
#define LUA_LIB

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "cobalt.h"
#include "lauxlib.h"
#include "lprefix.h"
#include "lualib.h"

#undef PI
#define PI (l_mathop(3.141592653589793238462643383279502884))

static int math_abs(lua_State *L) {
  if (lua_isinteger(L, 1)) {
    lua_Integer n = lua_tointeger(L, 1);
    if (n < 0) n = (lua_Integer)(0u - (lua_Unsigned)n);
    lua_pushinteger(L, n);
  } else
    lua_pushnumber(L, l_mathop(fabs)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_sin(lua_State *L) {
  lua_pushnumber(L, l_mathop(sin)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_cos(lua_State *L) {
  lua_pushnumber(L, l_mathop(cos)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_tan(lua_State *L) {
  lua_pushnumber(L, l_mathop(tan)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_asin(lua_State *L) {
  lua_pushnumber(L, l_mathop(asin)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_acos(lua_State *L) {
  lua_pushnumber(L, l_mathop(acos)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_atan(lua_State *L) {
  lua_Number y = luaL_checknumber(L, 1);
  lua_Number x = luaL_optnumber(L, 2, 1);
  lua_pushnumber(L, l_mathop(atan2)(y, x));
  return 1;
}

static int math_toint(lua_State *L) {
  int valid;
  lua_Integer n = lua_tointegerx(L, 1, &valid);
  if (l_likely(valid))
    lua_pushinteger(L, n);
  else {
    luaL_checkany(L, 1);
    luaL_pushfail(L); /* value is not convertible to integer */
  }
  return 1;
}

static void pushnumint(lua_State *L, lua_Number d) {
  lua_Integer n;
  if (lua_numbertointeger(d, &n)) /* does 'd' fit in an integer? */
    lua_pushinteger(L, n);        /* result is integer */
  else
    lua_pushnumber(L, d); /* result is float */
}

static int math_floor(lua_State *L) {
  if (lua_isinteger(L, 1))
    lua_settop(L, 1); /* integer is its own floor */
  else {
    lua_Number d = l_mathop(floor)(luaL_checknumber(L, 1));
    pushnumint(L, d);
  }
  return 1;
}

static int math_ceil(lua_State *L) {
  if (lua_isinteger(L, 1))
    lua_settop(L, 1); /* integer is its own ceil */
  else {
    lua_Number d = l_mathop(ceil)(luaL_checknumber(L, 1));
    pushnumint(L, d);
  }
  return 1;
}

static int math_fmod(lua_State *L) {
  if (lua_isinteger(L, 1) && lua_isinteger(L, 2)) {
    lua_Integer d = lua_tointeger(L, 2);
    if ((lua_Unsigned)d + 1u <= 1u) { /* special cases: -1 or 0 */
      luaL_argcheck(L, d != 0, 2, "zero");
      lua_pushinteger(L, 0); /* avoid overflow with 0x80000... / -1 */
    } else
      lua_pushinteger(L, lua_tointeger(L, 1) % d);
  } else
    lua_pushnumber(
        L, l_mathop(fmod)(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

/*
** next function does not use 'modf', avoiding problems with 'double*'
** (which is not compatible with 'float*') when lua_Number is not
** 'double'.
*/
static int math_modf(lua_State *L) {
  if (lua_isinteger(L, 1)) {
    lua_settop(L, 1);     /* number is its own integer part */
    lua_pushnumber(L, 0); /* no fractional part */
  } else {
    lua_Number n = luaL_checknumber(L, 1);
    /* integer part (rounds toward zero) */
    lua_Number ip = (n < 0) ? l_mathop(ceil)(n) : l_mathop(floor)(n);
    pushnumint(L, ip);
    /* fractional part (test needed for inf/-inf) */
    lua_pushnumber(L, (n == ip) ? l_mathop(0.0) : (n - ip));
  }
  return 2;
}

static int math_sqrt(lua_State *L) {
  lua_pushnumber(L, l_mathop(sqrt)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_ult(lua_State *L) {
  lua_Integer a = luaL_checkinteger(L, 1);
  lua_Integer b = luaL_checkinteger(L, 2);
  lua_pushboolean(L, (lua_Unsigned)a < (lua_Unsigned)b);
  return 1;
}

static int math_log(lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  lua_Number res;
  if (lua_isnoneornil(L, 2))
    res = l_mathop(log)(x);
  else {
    lua_Number base = luaL_checknumber(L, 2);
#if !defined(LUA_USE_C89)
    if (base == l_mathop(2.0))
      res = l_mathop(log2)(x);
    else
#endif
        if (base == l_mathop(10.0))
      res = l_mathop(log10)(x);
    else
      res = l_mathop(log)(x) / l_mathop(log)(base);
  }
  lua_pushnumber(L, res);
  return 1;
}

static int math_exp(lua_State *L) {
  lua_pushnumber(L, l_mathop(exp)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_deg(lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1) * (l_mathop(180.0) / PI));
  return 1;
}

static int math_rad(lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1) * (PI / l_mathop(180.0)));
  return 1;
}

static int math_min(lua_State *L) {
  int n = lua_gettop(L); /* number of arguments */
  int imin = 1;          /* index of current minimum value */
  int i;
  luaL_argcheck(L, n >= 1, 1, "value expected");
  for (i = 2; i <= n; i++) {
    if (lua_compare(L, i, imin, LUA_OPLT)) imin = i;
  }
  lua_pushvalue(L, imin);
  return 1;
}

static int math_max(lua_State *L) {
  int n = lua_gettop(L); /* number of arguments */
  int imax = 1;          /* index of current maximum value */
  int i;
  luaL_argcheck(L, n >= 1, 1, "value expected");
  for (i = 2; i <= n; i++) {
    if (lua_compare(L, imax, i, LUA_OPLT)) imax = i;
  }
  lua_pushvalue(L, imax);
  return 1;
}

static int math_type(lua_State *L) {
  if (lua_type(L, 1) == LUA_TNUMBER)
    lua_pushstring(L, (lua_isinteger(L, 1)) ? "integer" : "float");
  else {
    luaL_checkany(L, 1);
    luaL_pushfail(L);
  }
  return 1;
}

/*
** {==================================================================
** Pseudo-Random Number Generator based on 'xoshiro256**'.
** ===================================================================
*/

/* number of binary digits in the mantissa of a float */
#define FIGS l_floatatt(MANT_DIG)

#if FIGS > 64
/* there are only 64 random bits; use them all */
#undef FIGS
#define FIGS 64
#endif

/*
** LUA_RAND32 forces the use of 32-bit integers in the implementation
** of the PRN generator (mainly for testing).
*/
#if !defined(LUA_RAND32) && !defined(Rand64)

/* try to find an integer type with at least 64 bits */

#if (ULONG_MAX >> 31 >> 31) >= 3

/* 'long' has at least 64 bits */
#define Rand64 unsigned long

#elif !defined(LUA_USE_C89) && defined(LLONG_MAX)

/* there is a 'long long' type (which must have at least 64 bits) */
#define Rand64 unsigned long long

#elif (LUA_MAXUNSIGNED >> 31 >> 31) >= 3

/* 'lua_Integer' has at least 64 bits */
#define Rand64 lua_Unsigned

#endif

#endif

#if defined(Rand64) /* { */

/*
** Standard implementation, using 64-bit integers.
** If 'Rand64' has more than 64 bits, the extra bits do not interfere
** with the 64 initial bits, except in a right shift. Moreover, the
** final result has to discard the extra bits.
*/

/* avoid using extra bits when needed */
#define trim64(x) ((x)&0xffffffffffffffffu)

/* rotate left 'x' by 'n' bits */
static Rand64 rotl(Rand64 x, int n) {
  return (x << n) | (trim64(x) >> (64 - n));
}

static Rand64 nextrand(Rand64 *state) {
  Rand64 state0 = state[0];
  Rand64 state1 = state[1];
  Rand64 state2 = state[2] ^ state0;
  Rand64 state3 = state[3] ^ state1;
  Rand64 res = rotl(state1 * 5, 7) * 9;
  state[0] = state0 ^ state3;
  state[1] = state1 ^ state2;
  state[2] = state2 ^ (state1 << 17);
  state[3] = rotl(state3, 45);
  return res;
}

/* must take care to not shift stuff by more than 63 slots */

/*
** Convert bits from a random integer into a float in the
** interval [0,1), getting the higher FIG bits from the
** random unsigned integer and converting that to a float.
*/

/* must throw out the extra (64 - FIGS) bits */
#define shift64_FIG (64 - FIGS)

/* to scale to [0, 1), multiply by scaleFIG = 2^(-FIGS) */
#define scaleFIG (l_mathop(0.5) / ((Rand64)1 << (FIGS - 1)))

static lua_Number I2d(Rand64 x) {
  return (lua_Number)(trim64(x) >> shift64_FIG) * scaleFIG;
}

/* convert a 'Rand64' to a 'lua_Unsigned' */
#define I2UInt(x) ((lua_Unsigned)trim64(x))

/* convert a 'lua_Unsigned' to a 'Rand64' */
#define Int2I(x) ((Rand64)(x))

#else /* no 'Rand64'   }{ */

/* get an integer with at least 32 bits */
#if LUAI_IS32INT
typedef unsigned int lu_int32;
#else
typedef unsigned long lu_int32;
#endif

/*
** Use two 32-bit integers to represent a 64-bit quantity.
*/
typedef struct Rand64 {
  lu_int32 h; /* higher half */
  lu_int32 l; /* lower half */
} Rand64;

/*
** If 'lu_int32' has more than 32 bits, the extra bits do not interfere
** with the 32 initial bits, except in a right shift and comparisons.
** Moreover, the final result has to discard the extra bits.
*/

/* avoid using extra bits when needed */
#define trim32(x) ((x)&0xffffffffu)

/*
** basic operations on 'Rand64' values
*/

/* build a new Rand64 value */
static Rand64 packI(lu_int32 h, lu_int32 l) {
  Rand64 result;
  result.h = h;
  result.l = l;
  return result;
}

/* return i << n */
static Rand64 Ishl(Rand64 i, int n) {
  lua_assert(n > 0 && n < 32);
  return packI((i.h << n) | (trim32(i.l) >> (32 - n)), i.l << n);
}

/* i1 ^= i2 */
static void Ixor(Rand64 *i1, Rand64 i2) {
  i1->h ^= i2.h;
  i1->l ^= i2.l;
}

/* return i1 + i2 */
static Rand64 Iadd(Rand64 i1, Rand64 i2) {
  Rand64 result = packI(i1.h + i2.h, i1.l + i2.l);
  if (trim32(result.l) < trim32(i1.l)) /* carry? */
    result.h++;
  return result;
}

/* return i * 5 */
static Rand64 times5(Rand64 i) {
  return Iadd(Ishl(i, 2), i); /* i * 5 == (i << 2) + i */
}

/* return i * 9 */
static Rand64 times9(Rand64 i) {
  return Iadd(Ishl(i, 3), i); /* i * 9 == (i << 3) + i */
}

/* return 'i' rotated left 'n' bits */
static Rand64 rotl(Rand64 i, int n) {
  lua_assert(n > 0 && n < 32);
  return packI((i.h << n) | (trim32(i.l) >> (32 - n)),
               (trim32(i.h) >> (32 - n)) | (i.l << n));
}

/* for offsets larger than 32, rotate right by 64 - offset */
static Rand64 rotl1(Rand64 i, int n) {
  lua_assert(n > 32 && n < 64);
  n = 64 - n;
  return packI((trim32(i.h) >> n) | (i.l << (32 - n)),
               (i.h << (32 - n)) | (trim32(i.l) >> n));
}

/*
** implementation of 'xoshiro256**' algorithm on 'Rand64' values
*/
static Rand64 nextrand(Rand64 *state) {
  Rand64 res = times9(rotl(times5(state[1]), 7));
  Rand64 t = Ishl(state[1], 17);
  Ixor(&state[2], state[0]);
  Ixor(&state[3], state[1]);
  Ixor(&state[1], state[2]);
  Ixor(&state[0], state[3]);
  Ixor(&state[2], t);
  state[3] = rotl1(state[3], 45);
  return res;
}

/*
** Converts a 'Rand64' into a float.
*/

/* an unsigned 1 with proper type */
#define UONE ((lu_int32)1)

#if FIGS <= 32

/* 2^(-FIGS) */
#define scaleFIG (l_mathop(0.5) / (UONE << (FIGS - 1)))

/*
** get up to 32 bits from higher half, shifting right to
** throw out the extra bits.
*/
static lua_Number I2d(Rand64 x) {
  lua_Number h = (lua_Number)(trim32(x.h) >> (32 - FIGS));
  return h * scaleFIG;
}

#else /* 32 < FIGS <= 64 */

/* must take care to not shift stuff by more than 31 slots */

/* 2^(-FIGS) = 1.0 / 2^30 / 2^3 / 2^(FIGS-33) */
#define scaleFIG \
  (l_mathop(1.0) / (UONE << 30) / l_mathop(8.0) / (UONE << (FIGS - 33)))

/*
** use FIGS - 32 bits from lower half, throwing out the other
** (32 - (FIGS - 32)) = (64 - FIGS) bits
*/
#define shiftLOW (64 - FIGS)

/*
** higher 32 bits go after those (FIGS - 32) bits: shiftHI = 2^(FIGS - 32)
*/
#define shiftHI ((lua_Number)(UONE << (FIGS - 33)) * l_mathop(2.0))

static lua_Number I2d(Rand64 x) {
  lua_Number h = (lua_Number)trim32(x.h) * shiftHI;
  lua_Number l = (lua_Number)(trim32(x.l) >> shiftLOW);
  return (h + l) * scaleFIG;
}

#endif

/* convert a 'Rand64' to a 'lua_Unsigned' */
static lua_Unsigned I2UInt(Rand64 x) {
  return ((lua_Unsigned)trim32(x.h) << 31 << 1) | (lua_Unsigned)trim32(x.l);
}

/* convert a 'lua_Unsigned' to a 'Rand64' */
static Rand64 Int2I(lua_Unsigned n) {
  return packI((lu_int32)(n >> 31 >> 1), (lu_int32)n);
}

#endif /* } */

/*
** A state uses four 'Rand64' values.
*/
typedef struct {
  Rand64 s[4];
} RanState;

/*
** Project the random integer 'ran' into the interval [0, n].
** Because 'ran' has 2^B possible values, the projection can only be
** uniform when the size of the interval is a power of 2 (exact
** division). Otherwise, to get a uniform projection into [0, n], we
** first compute 'lim', the smallest Mersenne number not smaller than
** 'n'. We then project 'ran' into the interval [0, lim].  If the result
** is inside [0, n], we are done. Otherwise, we try with another 'ran',
** until we have a result inside the interval.
*/
static lua_Unsigned project(lua_Unsigned ran, lua_Unsigned n, RanState *state) {
  if ((n & (n + 1)) == 0) /* is 'n + 1' a power of 2? */
    return ran & n;       /* no bias */
  else {
    lua_Unsigned lim = n;
    /* compute the smallest (2^b - 1) not smaller than 'n' */
    lim |= (lim >> 1);
    lim |= (lim >> 2);
    lim |= (lim >> 4);
    lim |= (lim >> 8);
    lim |= (lim >> 16);
#if (LUA_MAXUNSIGNED >> 31) >= 3
    lim |= (lim >> 32); /* integer type has more than 32 bits */
#endif
    lua_assert((lim & (lim + 1)) == 0   /* 'lim + 1' is a power of 2, */
               && lim >= n              /* not smaller than 'n', */
               && (lim >> 1) < n);      /* and it is the smallest one */
    while ((ran &= lim) > n)            /* project 'ran' into [0..lim] */
      ran = I2UInt(nextrand(state->s)); /* not inside [0..n]? try again */
    return ran;
  }
}

static int math_random(lua_State *L) {
  lua_Integer low, up;
  lua_Unsigned p;
  RanState *state = (RanState *)lua_touserdata(L, lua_upvalueindex(1));
  Rand64 rv = nextrand(state->s); /* next pseudo-random value */
  switch (lua_gettop(L)) {        /* check number of arguments */
    case 0: {                     /* no arguments */
      lua_pushnumber(L, I2d(rv)); /* float between 0 and 1 */
      return 1;
    }
    case 1: { /* only upper limit */
      low = 1;
      up = luaL_checkinteger(L, 1);
      if (up == 0) {                    /* single 0 as argument? */
        lua_pushinteger(L, I2UInt(rv)); /* full random integer */
        return 1;
      }
      break;
    }
    case 2: { /* lower and upper limits */
      low = luaL_checkinteger(L, 1);
      up = luaL_checkinteger(L, 2);
      break;
    }
    default:
      return luaL_error(L, "wrong number of arguments");
  }
  /* random integer in the interval [low, up] */
  luaL_argcheck(L, low <= up, 1, "interval is empty");
  /* project random integer into the interval [0, up - low] */
  p = project(I2UInt(rv), (lua_Unsigned)up - (lua_Unsigned)low, state);
  lua_pushinteger(L, p + (lua_Unsigned)low);
  return 1;
}

static void setseed(lua_State *L, Rand64 *state, lua_Unsigned n1,
                    lua_Unsigned n2) {
  int i;
  state[0] = Int2I(n1);
  state[1] = Int2I(0xff); /* avoid a zero state */
  state[2] = Int2I(n2);
  state[3] = Int2I(0);
  for (i = 0; i < 16; i++)
    nextrand(state); /* discard initial values to "spread" seed */
  lua_pushinteger(L, n1);
  lua_pushinteger(L, n2);
}

/*
** Set a "random" seed. To get some randomness, use the current time
** and the address of 'L' (in case the machine does address space layout
** randomization).
*/
static void randseed(lua_State *L, RanState *state) {
  lua_Unsigned seed1 = (lua_Unsigned)time(NULL);
  lua_Unsigned seed2 = (lua_Unsigned)(size_t)L;
  setseed(L, state->s, seed1, seed2);
}

static int math_randomseed(lua_State *L) {
  RanState *state = (RanState *)lua_touserdata(L, lua_upvalueindex(1));
  if (lua_isnone(L, 1)) {
    randseed(L, state);
  } else {
    lua_Integer n1 = luaL_checkinteger(L, 1);
    lua_Integer n2 = luaL_optinteger(L, 2, 0);
    setseed(L, state->s, n1, n2);
  }
  return 2; /* return seeds */
}

static const luaL_Reg randfuncs[] = {
    {"random", math_random}, {"randomseed", math_randomseed}, {NULL, NULL}};

/*
** Register the random functions and initialize their state.
*/
static void setrandfunc(lua_State *L) {
  RanState *state = (RanState *)lua_newuserdatauv(L, sizeof(RanState), 0);
  randseed(L, state); /* initialize with a "random" seed */
  lua_pop(L, 2);      /* remove pushed seeds */
  luaL_setfuncs(L, randfuncs, 1);
}

/* }================================================================== */

/*
** {==================================================================
** Deprecated functions (for compatibility only)
** ===================================================================
*/
#if defined(LUA_COMPAT_MATHLIB)

static int math_cosh(lua_State *L) {
  lua_pushnumber(L, l_mathop(cosh)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_sinh(lua_State *L) {
  lua_pushnumber(L, l_mathop(sinh)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_tanh(lua_State *L) {
  lua_pushnumber(L, l_mathop(tanh)(luaL_checknumber(L, 1)));
  return 1;
}

static int math_pow(lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  lua_Number y = luaL_checknumber(L, 2);
  lua_pushnumber(L, l_mathop(pow)(x, y));
  return 1;
}

static int math_frexp(lua_State *L) {
  int e;
  lua_pushnumber(L, l_mathop(frexp)(luaL_checknumber(L, 1), &e));
  lua_pushinteger(L, e);
  return 2;
}

static int math_ldexp(lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  int ep = (int)luaL_checkinteger(L, 2);
  lua_pushnumber(L, l_mathop(ldexp)(x, ep));
  return 1;
}

static int math_log10(lua_State *L) {
  lua_pushnumber(L, l_mathop(log10)(luaL_checknumber(L, 1)));
  return 1;
}

#endif
/* }================================================================== */

// MATHx (lmathx)
#define MYNAME "mathx"

#define A(i) luaL_checknumber(L, i)
#define I(i) ((int)luaL_checkinteger(L, i))

#undef PI
#define PI (l_mathop(3.141592653589793238462643383279502884))
#define rad(x) ((x) * (PI / l_mathop(180.0)))
#define deg(x) ((x) * (l_mathop(180.0) / PI))

static int Lfmax(lua_State *L) /** fmax */
{
  int i, n = lua_gettop(L);
  lua_Number m = A(1);
  for (i = 2; i <= n; i++) m = l_mathop(fmax)(m, A(i));
  lua_pushnumber(L, m);
  return 1;
}

static int Lfmin(lua_State *L) /** fmin */
{
  int i, n = lua_gettop(L);
  lua_Number m = A(1);
  for (i = 2; i <= n; i++) m = l_mathop(fmin)(m, A(i));
  lua_pushnumber(L, m);
  return 1;
}

static int Lfrexp(lua_State *L) /** frexp */
{
  int e;
  lua_pushnumber(L, l_mathop(frexp)(A(1), &e));
  lua_pushinteger(L, e);
  return 2;
}

static int Lldexp(lua_State *L) /** ldexp */
{
  lua_pushnumber(L, l_mathop(ldexp)(A(1), I(2)));
  return 1;
}

static int Lmodf(lua_State *L) /** modf */
{
  lua_Number ip;
  lua_Number fp = l_mathop(modf)(A(1), &ip);
  lua_pushnumber(L, ip);
  lua_pushnumber(L, fp);
  return 2;
}

static int Lfabs(lua_State *L) /** fabs */
{
  lua_pushnumber(L, l_mathop(fabs)(A(1)));
  return 1;
}

static int Lacos(lua_State *L) /** acos */
{
  lua_pushnumber(L, l_mathop(acos)(A(1)));
  return 1;
}

static int Lacosh(lua_State *L) /** acosh */
{
  lua_pushnumber(L, l_mathop(acosh)(A(1)));
  return 1;
}

static int Lasin(lua_State *L) /** asin */
{
  lua_pushnumber(L, l_mathop(asin)(A(1)));
  return 1;
}

static int Lasinh(lua_State *L) /** asinh */
{
  lua_pushnumber(L, l_mathop(asinh)(A(1)));
  return 1;
}

static int Latan(lua_State *L) /** atan */
{
  int n = lua_gettop(L);
  if (n == 1)
    lua_pushnumber(L, l_mathop(atan)(A(1)));
  else
    lua_pushnumber(L, l_mathop(atan2)(A(1), A(2)));
  return 1;
}

static int Latan2(lua_State *L) /** atan2 */
{
  lua_pushnumber(L, l_mathop(atan2)(A(1), A(2)));
  return 1;
}

static int Latanh(lua_State *L) /** atanh */
{
  lua_pushnumber(L, l_mathop(atanh)(A(1)));
  return 1;
}

static int Lcbrt(lua_State *L) /** cbrt */
{
  lua_pushnumber(L, l_mathop(cbrt)(A(1)));
  return 1;
}

static int Lceil(lua_State *L) /** ceil */
{
  lua_pushnumber(L, l_mathop(ceil)(A(1)));
  return 1;
}

static int Lcopysign(lua_State *L) /** copysign */
{
  lua_pushnumber(L, l_mathop(copysign)(A(1), A(2)));
  return 1;
}

static int Lcos(lua_State *L) /** cos */
{
  lua_pushnumber(L, l_mathop(cos)(A(1)));
  return 1;
}

static int Lcosh(lua_State *L) /** cosh */
{
  lua_pushnumber(L, l_mathop(cosh)(A(1)));
  return 1;
}

static int Ldeg(lua_State *L) /** deg */
{
  lua_pushnumber(L, deg(A(1)));
  return 1;
}

static int Lerf(lua_State *L) /** erf */
{
  lua_pushnumber(L, l_mathop(erf)(A(1)));
  return 1;
}

static int Lerfc(lua_State *L) /** erfc */
{
  lua_pushnumber(L, l_mathop(erfc)(A(1)));
  return 1;
}

static int Lexp(lua_State *L) /** exp */
{
  lua_pushnumber(L, l_mathop(exp)(A(1)));
  return 1;
}

static int Lexp2(lua_State *L) /** exp2 */
{
  lua_pushnumber(L, l_mathop(exp2)(A(1)));
  return 1;
}

static int Lexpm1(lua_State *L) /** expm1 */
{
  lua_pushnumber(L, l_mathop(expm1)(A(1)));
  return 1;
}

static int Lfdim(lua_State *L) /** fdim */
{
  lua_pushnumber(L, l_mathop(fdim)(A(1), A(2)));
  return 1;
}

static int Lfloor(lua_State *L) /** floor */
{
  lua_pushnumber(L, l_mathop(floor)(A(1)));
  return 1;
}

static int Lfma(lua_State *L) /** fma */
{
  lua_pushnumber(L, l_mathop(fma)(A(1), A(2), A(3)));
  return 1;
}

static int Lfmod(lua_State *L) /** fmod */
{
  lua_pushnumber(L, l_mathop(fmod)(A(1), A(2)));
  return 1;
}

static int Lgamma(lua_State *L) /** gamma */
{
  lua_pushnumber(L, l_mathop(tgamma)(A(1)));
  return 1;
}

static int Lhypot(lua_State *L) /** hypot */
{
  lua_pushnumber(L, l_mathop(hypot)(A(1), A(2)));
  return 1;
}

static int Lisfinite(lua_State *L) /** isfinite */
{
  lua_pushboolean(L, isfinite(A(1)));
  return 1;
}

static int Lisinf(lua_State *L) /** isinf */
{
  lua_pushboolean(L, isinf(A(1)));
  return 1;
}

static int Lisnan(lua_State *L) /** isnan */
{
  lua_pushboolean(L, isnan(A(1)));
  return 1;
}

static int Lisnormal(lua_State *L) /** isnormal */
{
  lua_pushboolean(L, isnormal(A(1)));
  return 1;
}

static int Llgamma(lua_State *L) /** lgamma */
{
  lua_pushnumber(L, l_mathop(lgamma)(A(1)));
  return 1;
}

static int Llog(lua_State *L) /** log */
{
  int n = lua_gettop(L);
  if (n == 1)
    lua_pushnumber(L, l_mathop(log)(A(1)));
  else {
    lua_Number b = A(2);
    if (b == 10.0)
      lua_pushnumber(L, l_mathop(log10)(A(1)));
    else if (b == 2.0)
      lua_pushnumber(L, l_mathop(log2)(A(1)));
    else
      lua_pushnumber(L, l_mathop(log)(A(1)) / l_mathop(log)(b));
  }
  return 1;
}

static int Llog10(lua_State *L) /** log10 */
{
  lua_pushnumber(L, l_mathop(log10)(A(1)));
  return 1;
}

static int Llog1p(lua_State *L) /** log1p */
{
  lua_pushnumber(L, l_mathop(log1p)(A(1)));
  return 1;
}

static int Llog2(lua_State *L) /** log2 */
{
  lua_pushnumber(L, l_mathop(log2)(A(1)));
  return 1;
}

static int Llogb(lua_State *L) /** logb */
{
  lua_pushnumber(L, l_mathop(logb)(A(1)));
  return 1;
}

static int Lnearbyint(lua_State *L) /** nearbyint */
{
  lua_pushnumber(L, l_mathop(nearbyint)(A(1)));
  return 1;
}

static int Lnextafter(lua_State *L) /** nextafter */
{
  lua_pushnumber(L, l_mathop(nextafter)(A(1), A(2)));
  return 1;
}

static int Lpow(lua_State *L) /** pow */
{
  lua_pushnumber(L, l_mathop(pow)(A(1), A(2)));
  return 1;
}

static int Lrad(lua_State *L) /** rad */
{
  lua_pushnumber(L, rad(A(1)));
  return 1;
}

static int Lremainder(lua_State *L) /** remainder */
{
  lua_pushnumber(L, l_mathop(remainder)(A(1), A(2)));
  return 1;
}

static int Lround(lua_State *L) /** round */
{
  lua_pushnumber(L, l_mathop(round)(A(1)));
  return 1;
}

static int Lscalbn(lua_State *L) /** scalbn */
{
  lua_pushnumber(L, l_mathop(scalbn)(A(1), A(2)));
  return 1;
}

static int Lsin(lua_State *L) /** sin */
{
  lua_pushnumber(L, l_mathop(sin)(A(1)));
  return 1;
}

static int Lsinh(lua_State *L) /** sinh */
{
  lua_pushnumber(L, l_mathop(sinh)(A(1)));
  return 1;
}

static int Lsqrt(lua_State *L) /** sqrt */
{
  lua_pushnumber(L, l_mathop(sqrt)(A(1)));
  return 1;
}

static int Ltan(lua_State *L) /** tan */
{
  lua_pushnumber(L, l_mathop(tan)(A(1)));
  return 1;
}

static int Ltanh(lua_State *L) /** tanh */
{
  lua_pushnumber(L, l_mathop(tanh)(A(1)));
  return 1;
}

static int Ltrunc(lua_State *L) /** trunc */
{
  lua_pushnumber(L, l_mathop(trunc)(A(1)));
  return 1;
}

static const luaL_Reg mathlib[] = {{"abs", math_abs},
                                   {"acos", math_acos},
                                   {"asin", math_asin},
                                   {"atan", math_atan},
                                   {"ceil", math_ceil},
                                   {"cos", math_cos},
                                   {"deg", math_deg},
                                   {"exp", math_exp},
                                   {"tointeger", math_toint},
                                   {"floor", math_floor},
                                   {"fmod", math_fmod},
                                   {"ult", math_ult},
                                   {"log", math_log},
                                   {"max", math_max},
                                   {"min", math_min},
                                   {"modf", math_modf},
                                   {"rad", math_rad},
                                   {"sin", math_sin},
                                   {"sqrt", math_sqrt},
                                   {"tan", math_tan},
                                   {"type", math_type},
#if defined(LUA_COMPAT_MATHLIB)
                                   {"atan2", math_atan},
                                   {"cosh", math_cosh},
                                   {"sinh", math_sinh},
                                   {"tanh", math_tanh},
                                   {"pow", math_pow},
                                   {"frexp", math_frexp},
                                   {"ldexp", math_ldexp},
                                   {"log10", math_log10},
#endif
                                   /* placeholders */
                                   {"random", NULL},
                                   {"randomseed", NULL},
                                   {"pi", NULL},
                                   {"huge", NULL},
                                   {"maxinteger", NULL},
                                   {"mininteger", NULL},
                                   /* mathx */
                                   {"fabs", Lfabs},
                                   {"acos", Lacos},
                                   {"acosh", Lacosh},
                                   {"asin", Lasin},
                                   {"asinh", Lasinh},
                                   {"atan", Latan},
                                   {"atan2", Latan2},
                                   {"atanh", Latanh},
                                   {"cbrt", Lcbrt},
                                   {"ceil", Lceil},
                                   {"copysign", Lcopysign},
                                   {"cos", Lcos},
                                   {"cosh", Lcosh},
                                   {"deg", Ldeg},
                                   {"erf", Lerf},
                                   {"erfc", Lerfc},
                                   {"exp", Lexp},
                                   {"exp2", Lexp2},
                                   {"expm1", Lexpm1},
                                   {"fdim", Lfdim},
                                   {"floor", Lfloor},
                                   {"fma", Lfma},
                                   {"fmax", Lfmax},
                                   {"fmin", Lfmin},
                                   {"fmod", Lfmod},
                                   {"frexp", Lfrexp},
                                   {"gamma", Lgamma},
                                   {"hypot", Lhypot},
                                   {"isfinite", Lisfinite},
                                   {"isinf", Lisinf},
                                   {"isnan", Lisnan},
                                   {"isnormal", Lisnormal},
                                   {"ldexp", Lldexp},
                                   {"lgamma", Llgamma},
                                   {"log", Llog},
                                   {"log10", Llog10},
                                   {"log1p", Llog1p},
                                   {"log2", Llog2},
                                   {"logb", Llogb},
                                   {"modf", Lmodf},
                                   {"nearbyint", Lnearbyint},
                                   {"nextafter", Lnextafter},
                                   {"pow", Lpow},
                                   {"rad", Lrad},
                                   {"remainder", Lremainder},
                                   {"round", Lround},
                                   {"scalbn", Lscalbn},
                                   {"sin", Lsin},
                                   {"sinh", Lsinh},
                                   {"sqrt", Lsqrt},
                                   {"tan", Ltan},
                                   {"tanh", Ltanh},
                                   {"trunc", Ltrunc},
                                   {NULL, NULL}};

/* RANDOM (lrandom) */
#include "cobalt.h"
#include "lauxlib.h"

/* THIS SNIPPET IS FROM Mersenne Twister's random.c */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

typedef struct {
  unsigned long v[N]; /* the array for the state vector  */
  int i;
} MT;

#define mt (o->v)
#define mti (o->i)

/* initializes mt[N] with a seed */
static void init_genrand(MT *o, unsigned long s) {
  mt[0] = s & 0xffffffffUL;
  for (mti = 1; mti < N; mti++) {
    mt[mti] = (1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    /* In the previous versions, MSBs of the seed affect   */
    /* only MSBs of the array mt[].                        */
    /* 2002/01/09 modified by Makoto Matsumoto             */
    mt[mti] &= 0xffffffffUL;
    /* for >32 bit machines */
  }
}

/* generates a random number on [0,0xffffffff]-interval */
static unsigned long genrand_int32(MT *o) {
  unsigned long y;
  static unsigned long mag01[2] = {0x0UL, MATRIX_A};
  /* mag01[x] = x * MATRIX_A  for x=0,1 */

  if (mti >= N) { /* generate N words at one time */
    int kk;

    if (mti == N + 1)          /* if init_genrand() has not been called, */
      init_genrand(o, 5489UL); /* a default initial seed is used */

    for (kk = 0; kk < N - M; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
      mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    for (; kk < N - 1; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
      mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
    mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    mti = 0;
  }

  y = mt[mti++];

  /* Tempering */
  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680UL;
  y ^= (y << 15) & 0xefc60000UL;
  y ^= (y >> 18);

  return y;
}

/* generates a random number on [0,1) with 53-bit resolution*/
static double genrand(MT *o) {
  unsigned long a = genrand_int32(o) >> 5, b = genrand_int32(o) >> 6;
  return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

#define AUTHOR "Mersenne Twister MT19937"
/*************************************/
#define RTYPE "random"

#define SEED 2018UL

static MT *Pget(lua_State *L, int i) { return luaL_checkudata(L, i, RTYPE); }

static MT *Pnew(lua_State *L) {
  MT *c = lua_newuserdata(L, sizeof(MT));
  luaL_newmetatable(L, RTYPE);
  lua_setmetatable(L, -2);

  return c;
}

static int Lnew(lua_State *L) /** new([seed]) */
{
  lua_Number seed = luaL_optnumber(L, 1, SEED);
  MT *c = Pnew(L);
  init_genrand(c, seed);
  return 1;
}

static int Lclone(lua_State *L) /** clone(c) */
{
  MT *c = Pget(L, 1);
  MT *d = Pnew(L);
  *d = *c;
  return 1;
}

static int Lseed(lua_State *L) /** seed(c,[seed]) */
{
  MT *c = Pget(L, 1);
  init_genrand(c, luaL_optnumber(L, 2, SEED));
  lua_settop(L, 1);
  return 1;
}

static int Lvalue(lua_State *L) /** value(c,[a,b]) */
{
  MT *c = Pget(L, 1);
  double a, b, r = genrand(c);
  switch (lua_gettop(L)) {
    case 1:
      lua_pushnumber(L, r);
      return 1;
    case 2:
      a = 1;
      b = luaL_checknumber(L, 2);
      break;
    default:
      a = luaL_checknumber(L, 2);
      b = luaL_checknumber(L, 3);
      break;
  }
  if (a > b) {
    double t = a;
    a = b;
    b = t;
  }
  a = ceil(a);
  b = floor(b);
  if (a > b) return 0;
  r = a + floor(r * (b - a + 1));
  lua_pushnumber(L, r);
  return 1;
}

#define MYTOSTRING

static const luaL_Reg Random[] = {MYTOSTRING{"clone", Lclone},
                                  {"new", Lnew},
                                  {"seed", Lseed},
                                  {"value", Lvalue},
                                  {NULL, NULL}};

/*
** Open math library
*/
LUAMOD_API int luaopen_math(lua_State *L) {
  luaL_newlib(L, mathlib);
  lua_pushnumber(L, PI);
  lua_setfield(L, -2, "pi");
  lua_pushnumber(L, (lua_Number)HUGE_VAL);
  lua_setfield(L, -2, "huge");
  lua_pushinteger(L, LUA_MAXINTEGER);
  lua_setfield(L, -2, "maxinteger");
  lua_pushinteger(L, LUA_MININTEGER);
  lua_setfield(L, -2, "mininteger");
  setrandfunc(L);
  lua_pushnumber(L, INFINITY);
  lua_setfield(L, -2, "inf");
  lua_pushnumber(L, NAN);
  lua_setfield(L, -2, "nan");

  luaL_newlib(L, Random);
  lua_setfield(L, -2, "rand");

  return 1;
}
