/*
** $Id: lbaselib.c,v 1.314.1.1 2017/04/19 17:39:34 roberto Exp $
** Basic library
** See Copyright Notice in lua.h
*/

#define lbaselib_c
#define LUA_LIB

#include "lprefix.h"


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"
#include <unistd.h>


#include <stdio.h>
#include <stdarg.h>

#include <time.h>

enum {
  COLOR_BLACK = 30,
  COLOR_RED,
  COLOR_GREEN,
  COLOR_YELLOW,
  COLOR_BLUE,
  COLOR_MAGENTA,
  COLOR_CYAN,
  COLOR_WHITE
};

enum {
  STYLE_BOLD = 1,
  STYLE_UNDERLINE = 4,
  STYLE_REVERSE = 7,
  STYLE_CONCEALED = 8
};

const char *color(const char *text, int color, const char **styles, int num_styles) {
  static char buf[1024];
  char *p = buf;
  const char *style;
  int i;

  p += sprintf(p, "\033[");
  for (i = 0; i < num_styles; i++) {
    style = styles[i];
    if (style == "bold") {
      p += sprintf(p, "1;");
    } else if (style == "underline") {
      p += sprintf(p, "4;");
    } else if (style == "reverse") {
      p += sprintf(p, "7;");
    } else if (style == "concealed") {
      p += sprintf(p, "8;");
    }
  }
  p += sprintf(p, "%dm%s\033[0m", color, text);
  return buf;
}

const char *red(const char *text, const char **styles, int num_styles) {
  return color(text, COLOR_RED, styles, num_styles);
}

const char *green(const char *text, const char **styles, int num_styles) {
  return color(text, COLOR_GREEN, styles, num_styles);
}

const char *yellow(const char *text, const char **styles, int num_styles) {
  return color(text, COLOR_YELLOW, styles, num_styles);
}

const char *blue(const char *text, const char **styles, int num_styles) {
  return color(text, COLOR_BLUE, styles, num_styles);
}

const char *magenta(const char *text, const char **styles, int num_styles) {
  return color(text, COLOR_MAGENTA, styles, num_styles);
}

const char *cyan(const char *text, const char **styles, int num_styles) {
  return color(text, COLOR_CYAN, styles, num_styles);
}

const char *white(const char *text, const char **styles, int num_styles) {
  return color(text, COLOR_WHITE, styles, num_styles);
}



static int luaB_print (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  int i;
  lua_getglobal(L, "tostring");
  
  // outpus (<time>) in gray 
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char time_str[80];
  sprintf(time_str, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, 
    tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  printf(L, white(time_str, (const char *[]){"bold"}, 1));

  for (i=1; i<=n; i++) {
    const char *s;
    size_t l;
    lua_pushvalue(L, -1);  /* function to be called */
    lua_pushvalue(L, i);   /* value to print */
    lua_call(L, 1, 1);
    s = lua_tolstring(L, -1, &l);  /* get result */
    if (s == NULL)
      return luaL_error(L, "'tostring' must return a string to 'print'");
    if (i>1) lua_writestring("\t", 1);
    lua_writestring(s, l);
    lua_pop(L, 1);  /* pop result */
  }
  lua_writeline();
  return 0;
}

static int luaB_warn (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  int i;
  lua_getglobal(L, "tostring");
  lua_getglobal(L, "print");
  for (i=1; i<=n; i++) {
    const char *s;
    size_t l;
    lua_pushvalue(L, -2);  /* function to be called */
    lua_pushvalue(L, i);   /* value to print */
    lua_call(L, 1, 1);
    lua_pushstring(L, yellow("warning: ", (const char *[]){"bold"}, 1));
    lua_insert(L, -2);
    lua_concat(L, 2);
    s = lua_tolstring(L, -1, &l);  /* get result */
    if (s == NULL)
      return luaL_error(L, "'tostring' must return a string to 'warn'");
    lua_call(L, 1, 0);
  }
  return 0;
}

static int luaB_info (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  int i;
  lua_getglobal(L, "tostring");
  lua_getglobal(L, "print");
  for (i=1; i<=n; i++) {
    const char *s;
    size_t l;
    lua_pushvalue(L, -2);  /* function to be called */
    lua_pushvalue(L, i);   /* value to print */
    lua_call(L, 1, 1);
    lua_pushstring(L, blue("info: ", (const char *[]){"bold"}, 1));
    lua_insert(L, -2);
    lua_concat(L, 2);
    s = lua_tolstring(L, -1, &l);  /* get result */
    if (s == NULL)
      return luaL_error(L, "'tostring' must return a string to 'info'");
    lua_call(L, 1, 0);
  }
  return 0;
}


#define SPACECHARS	" \f\n\r\t\v"

static const char *b_str2int (const char *s, int base, lua_Integer *pn) {
  lua_Unsigned n = 0;
  int neg = 0;
  s += strspn(s, SPACECHARS);  /* skip initial spaces */
  if (*s == '-') { s++; neg = 1; }  /* handle signal */
  else if (*s == '+') s++;
  if (!isalnum((unsigned char)*s))  /* no digit? */
    return NULL;
  do {
    int digit = (isdigit((unsigned char)*s)) ? *s - '0'
                   : (toupper((unsigned char)*s) - 'A') + 10;
    if (digit >= base) return NULL;  /* invalid numeral */
    n = n * base + digit;
    s++;
  } while (isalnum((unsigned char)*s));
  s += strspn(s, SPACECHARS);  /* skip trailing spaces */
  *pn = (lua_Integer)((neg) ? (0u - n) : n);
  return s;
}


static int luaB_tonumber (lua_State *L) {
  if (lua_isnoneornil(L, 2)) {  /* standard conversion? */
    luaL_checkany(L, 1);
    if (lua_type(L, 1) == LUA_TNUMBER) {  /* already a number? */
      lua_settop(L, 1);  /* yes; return it */
      return 1;
    }
    else {
      size_t l;
      const char *s = lua_tolstring(L, 1, &l);
      if (s != NULL && lua_stringtonumber(L, s) == l + 1)
        return 1;  /* successful conversion to number */
      /* else not a number */
    }
  }
  else {
    size_t l;
    const char *s;
    lua_Integer n = 0;  /* to avoid warnings */
    lua_Integer base = luaL_checkinteger(L, 2);
    luaL_checktype(L, 1, LUA_TSTRING);  /* no numbers as strings */
    s = lua_tolstring(L, 1, &l);
    luaL_argcheck(L, 2 <= base && base <= 36, 2, "base out of range");
    if (b_str2int(s, (int)base, &n) == s + l) {
      lua_pushinteger(L, n);
      return 1;
    }  /* else not a number */
  }  /* else not a number */
  lua_pushnil(L);  /* not a number */
  return 1;
}


static int luaB_error (lua_State *L) {
  int level = (int)luaL_optinteger(L, 2, 1);
  lua_settop(L, 1);
  if (lua_type(L, 1) == LUA_TSTRING && level > 0) {
    luaL_where(L, level);   /* add extra information */
    lua_pushvalue(L, 1);
    lua_concat(L, 2);
  }
  return lua_error(L);
}


static int luaB_getmetatable (lua_State *L) {
  luaL_checkany(L, 1);
  if (!lua_getmetatable(L, 1)) {
    lua_pushnil(L);
    return 1;  /* no metatable */
  }
  luaL_getmetafield(L, 1, "__metatable");
  return 1;  /* returns either __metatable field (if present) or metatable */
}


static int luaB_setmetatable (lua_State *L) {
  int t = lua_type(L, 2);
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_argcheck(L, t == LUA_TNIL || t == LUA_TTABLE, 2,
                    "nil or table expected");
  if (luaL_getmetafield(L, 1, "__metatable") != LUA_TNIL)
    return luaL_error(L, "cannot change a protected metatable");
  lua_settop(L, 2);
  lua_setmetatable(L, 1);
  return 1;
}


static int luaB_rawequal (lua_State *L) {
  luaL_checkany(L, 1);
  luaL_checkany(L, 2);
  lua_pushboolean(L, lua_rawequal(L, 1, 2));
  return 1;
}


static int luaB_rawlen (lua_State *L) {
  int t = lua_type(L, 1);
  luaL_argcheck(L, t == LUA_TTABLE || t == LUA_TSTRING, 1,
                   "table or string expected");
  lua_pushinteger(L, lua_rawlen(L, 1));
  return 1;
}


static int luaB_rawget (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checkany(L, 2);
  lua_settop(L, 2);
  lua_rawget(L, 1);
  return 1;
}

static int luaB_rawset (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checkany(L, 2);
  luaL_checkany(L, 3);
  lua_settop(L, 3);
  lua_rawset(L, 1);
  return 1;
}


static int luaB_collectgarbage (lua_State *L) {
  static const char *const opts[] = {"stop", "restart", "collect",
    "count", "step", "setpause", "setstepmul",
    "isrunning", NULL};
  static const int optsnum[] = {LUA_GCSTOP, LUA_GCRESTART, LUA_GCCOLLECT,
    LUA_GCCOUNT, LUA_GCSTEP, LUA_GCSETPAUSE, LUA_GCSETSTEPMUL,
    LUA_GCISRUNNING};
  int o = optsnum[luaL_checkoption(L, 1, "collect", opts)];
  int ex = (int)luaL_optinteger(L, 2, 0);
  int res = lua_gc(L, o, ex);
  switch (o) {
    case LUA_GCCOUNT: {
      int b = lua_gc(L, LUA_GCCOUNTB, 0);
      lua_pushnumber(L, (lua_Number)res + ((lua_Number)b/1024));
      return 1;
    }
    case LUA_GCSTEP: case LUA_GCISRUNNING: {
      lua_pushboolean(L, res);
      return 1;
    }
    default: {
      lua_pushinteger(L, res);
      return 1;
    }
  }
}


static int luaB_type (lua_State *L) {
  int t = lua_type(L, 1);
  luaL_argcheck(L, t != LUA_TNONE, 1, "value expected");
  lua_pushstring(L, lua_typename(L, t));
  return 1;
}


static int pairsmeta (lua_State *L, const char *method, int iszero,
                      lua_CFunction iter) {
  luaL_checkany(L, 1);
  if (luaL_getmetafield(L, 1, method) == LUA_TNIL) {  /* no metamethod? */
    lua_pushcfunction(L, iter);  /* will return generator, */
    lua_pushvalue(L, 1);  /* state, */
    if (iszero) lua_pushinteger(L, 0);  /* and initial value */
    else lua_pushnil(L);
  }
  else {
    lua_pushvalue(L, 1);  /* argument 'self' to metamethod */
    lua_call(L, 1, 3);  /* get 3 values from metamethod */
  }
  return 3;
}


static int luaB_next (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  lua_settop(L, 2);  /* create a 2nd argument if there isn't one */
  if (lua_next(L, 1))
    return 2;
  else {
    lua_pushnil(L);
    return 1;
  }
}


static int luaB_pairs (lua_State *L) {
  return pairsmeta(L, "__pairs", 0, luaB_next);
}


/*
** Traversal function for 'ipairs'
*/
static int ipairsaux (lua_State *L) {
  lua_Integer i = luaL_checkinteger(L, 2) + 1;
  lua_pushinteger(L, i);
  return (lua_geti(L, 1, i) == LUA_TNIL) ? 1 : 2;
}


/*
** 'ipairs' function. Returns 'ipairsaux', given "table", 0.
** (The given "table" may not be a table.)
*/
static int luaB_ipairs (lua_State *L) {
#if defined(LUA_COMPAT_IPAIRS)
  return pairsmeta(L, "__ipairs", 1, ipairsaux);
#else
  luaL_checkany(L, 1);
  lua_pushcfunction(L, ipairsaux);  /* iteration function */
  lua_pushvalue(L, 1);  /* state */
  lua_pushinteger(L, 0);  /* initial value */
  return 3;
#endif
}


static int load_aux (lua_State *L, int status, int envidx) {
  if (status == LUA_OK) {
    if (envidx != 0) {  /* 'env' parameter? */
      lua_pushvalue(L, envidx);  /* environment for loaded function */
      if (!lua_setupvalue(L, -2, 1))  /* set it as 1st upvalue */
        lua_pop(L, 1);  /* remove 'env' if not used by previous call */
    }
    return 1;
  }
  else {  /* error (message is on top of the stack) */
    lua_pushnil(L);
    lua_insert(L, -2);  /* put before error message */
    return 2;  /* return nil plus error message */
  }
}


static int luaB_loadfile (lua_State *L) {
  const char *fname = luaL_optstring(L, 1, NULL);
  const char *mode = luaL_optstring(L, 2, NULL);
  int env = (!lua_isnone(L, 3) ? 3 : 0);  /* 'env' index or 0 if no 'env' */
  int status = luaL_loadfilex(L, fname, mode);
  return load_aux(L, status, env);
}


/*
** {======================================================
** Generic Read function
** =======================================================
*/


/*
** reserved slot, above all arguments, to hold a copy of the returned
** string to avoid it being collected while parsed. 'load' has four
** optional arguments (chunk, source name, mode, and environment).
*/
#define RESERVEDSLOT	5


/*
** Reader for generic 'load' function: 'lua_load' uses the
** stack for internal stuff, so the reader cannot change the
** stack top. Instead, it keeps its resulting string in a
** reserved slot inside the stack.
*/
static const char *generic_reader (lua_State *L, void *ud, size_t *size) {
  (void)(ud);  /* not used */
  luaL_checkstack(L, 2, "too many nested functions");
  lua_pushvalue(L, 1);  /* get function */
  lua_call(L, 0, 1);  /* call it */
  if (lua_isnil(L, -1)) {
    lua_pop(L, 1);  /* pop result */
    *size = 0;
    return NULL;
  }
  else if (!lua_isstring(L, -1))
    luaL_error(L, "reader function must return a string");
  lua_replace(L, RESERVEDSLOT);  /* save string in reserved slot */
  return lua_tolstring(L, RESERVEDSLOT, size);
}


static int luaB_load (lua_State *L) {
  int status;
  size_t l;
  const char *s = lua_tolstring(L, 1, &l);
  const char *mode = luaL_optstring(L, 3, "bt");
  int env = (!lua_isnone(L, 4) ? 4 : 0);  /* 'env' index or 0 if no 'env' */
  if (s != NULL) {  /* loading a string? */
    const char *chunkname = luaL_optstring(L, 2, s);
    status = luaL_loadbufferx(L, s, l, chunkname, mode);
  }
  else {  /* loading from a reader function */
    const char *chunkname = luaL_optstring(L, 2, "=(load)");
    luaL_checktype(L, 1, LUA_TFUNCTION);
    lua_settop(L, RESERVEDSLOT);  /* create reserved slot */
    status = lua_load(L, generic_reader, NULL, chunkname, mode);
  }
  return load_aux(L, status, env);
}

/* }====================================================== */


static int dofilecont (lua_State *L, int d1, lua_KContext d2) {
  (void)d1;  (void)d2;  /* only to match 'lua_Kfunction' prototype */
  return lua_gettop(L) - 1;
}


static int luaB_dofile (lua_State *L) {
  const char *fname = luaL_optstring(L, 1, NULL);
  lua_settop(L, 1);
  if (luaL_loadfile(L, fname) != LUA_OK)
    return lua_error(L);
  lua_callk(L, 0, LUA_MULTRET, 0, dofilecont);
  return dofilecont(L, 0, 0);
}


static int luaB_assert (lua_State *L) {
  if (lua_toboolean(L, 1))  /* condition is true? */
    return lua_gettop(L);  /* return all arguments */
  else {  /* error */
    luaL_checkany(L, 1);  /* there must be a condition */
    lua_remove(L, 1);  /* remove it */
    lua_pushliteral(L, "assertion failed!");  /* default message */
    lua_settop(L, 1);  /* leave only message (default if no other one) */
    return luaB_error(L);  /* call 'error' */
  }
}


static int luaB_select (lua_State *L) {
  int n = lua_gettop(L);
  if (lua_type(L, 1) == LUA_TSTRING && *lua_tostring(L, 1) == '#') {
    lua_pushinteger(L, n-1);
    return 1;
  }
  else {
    lua_Integer i = luaL_checkinteger(L, 1);
    if (i < 0) i = n + i;
    else if (i > n) i = n;
    luaL_argcheck(L, 1 <= i, 1, "index out of range");
    return n - (int)i;
  }
}


/*
** Continuation function for 'pcall' and 'xpcall'. Both functions
** already pushed a 'true' before doing the call, so in case of success
** 'finishpcall' only has to return everything in the stack minus
** 'extra' values (where 'extra' is exactly the number of items to be
** ignored).
*/
static int finishpcall (lua_State *L, int status, lua_KContext extra) {
  if (status != LUA_OK && status != LUA_YIELD) {  /* error? */
    lua_pushboolean(L, 0);  /* first result (false) */
    lua_pushvalue(L, -2);  /* error message */
    return 2;  /* return false, msg */
  }
  else
    return lua_gettop(L) - (int)extra;  /* return all results */
}


static int luaB_pcall (lua_State *L) {
  int status;
  luaL_checkany(L, 1);
  lua_pushboolean(L, 1);  /* first result if no errors */
  lua_insert(L, 1);  /* put it in place */
  status = lua_pcallk(L, lua_gettop(L) - 2, LUA_MULTRET, 0, 0, finishpcall);
  return finishpcall(L, status, 0);
}


/*
** Do a protected call with error handling. After 'lua_rotate', the
** stack will have <f, err, true, f, [args...]>; so, the function passes
** 2 to 'finishpcall' to skip the 2 first values when returning results.
*/
static int luaB_xpcall (lua_State *L) {
  int status;
  int n = lua_gettop(L);
  luaL_checktype(L, 2, LUA_TFUNCTION);  /* check error function */
  lua_pushboolean(L, 1);  /* first result */
  lua_pushvalue(L, 1);  /* function */
  lua_rotate(L, 3, 2);  /* move them below function's arguments */
  status = lua_pcallk(L, n - 2, LUA_MULTRET, 2, 2, finishpcall);
  return finishpcall(L, status, 2);
}


static int luaB_tostring (lua_State *L) {
  luaL_checkany(L, 1);
  luaL_tolstring(L, 1, NULL);
  return 1;
}
static int luaB_uwait(lua_State *L) {
  float m = luaL_optnumber(L, 1, 1);
  usleep(m);
  // returns a function that waits m without needing any arguments
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushnumber(L, m);
  lua_pushcclosure(L, luaB_uwait, 1);
  return 1;
}

static int luaB_wait(lua_State *L) {
  float seconds = luaL_optnumber(L, 1, 1);
  usleep(seconds * 1000000);
  // returns a function that waits m without needing any arguments
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushnumber(L, seconds * 1000000);
  lua_pushcclosure(L, luaB_uwait, 1);
  return 1;
}

static int luaB_mwait(lua_State *L) {
  float ms = luaL_optnumber(L, 1, 1);
  usleep(ms * 1000);
  // returns a function that waits m without needing any arguments
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushnumber(L, ms * 1000);
  lua_pushcclosure(L, luaB_uwait, 1);
  return 1;
}


static int luaB_slice(lua_State *L) {
  int nargs = lua_gettop(L);
  luaL_argcheck(L, nargs >= 2, 2, "expected at least 2 arguments");

  int start = luaL_checkinteger(L, 1);
  int stop = luaL_checkinteger(L, 2);
  int step = 1;

  if (nargs >= 3) {
    step = luaL_checkinteger(L, 3);
    luaL_argcheck(L, step != 0, 3, "step cannot be zero");
  }

  luaL_checktype(L, -1, LUA_TTABLE);
  int len = lua_rawlen(L, -1);

  if (start < 0) {
    start = len + start + 1;
  }

  if (stop < 0) {
    stop = len + stop + 1;
  }

  luaL_argcheck(L, start >= 1 && start <= len, 1, "start index out of range");
  luaL_argcheck(L, stop >= 1 && stop <= len, 2, "stop index out of range");

  if (step > 0) {
    luaL_argcheck(L, start <= stop, 2, "stop index must be greater than start index");
  } else {
    luaL_argcheck(L, start >= stop, 2, "stop index must be less than start index");
  }

  lua_newtable(L);
  int j = 1;

  for (int i = start; step > 0 ? i <= stop : i >= stop; i += step) {
    lua_rawgeti(L, -2, i);
    lua_rawseti(L, -2, j);
    j++;
  }

  return 1;
}


static int luaB_inputf(lua_State *L) {
  const char *prompt = luaL_optstring(L, 1, " ");
  const char *fmt = luaL_optstring(L, 2, "*l");
  int nargs = lua_gettop(L);
  luaL_argcheck(L, nargs <= 2, 2, "expected at most 2 arguments");
  luaL_argcheck(L, nargs >= 1, 1, "expected at least 1 argument");

  printf("%s", prompt);
  char buf[1024];
  fgets(buf, sizeof(buf), stdin);
  buf[strlen(buf) - 1] = '\0';

  if (strcmp(fmt, "*n") == 0) {
    lua_pushnumber(L, atof(buf));
  } else if (strcmp(fmt, "*a") == 0) {
    lua_pushstring(L, buf);
  } else if (strcmp(fmt, "*l") == 0) {
    lua_pushstring(L, buf);
  } else {
    return luaL_error(L, "invalid format");
  }

  return 1;
}


static int luaB_new(lua_State *L) {
  // Runs arg1.new(arg2, arg3, ...)
  // So new(arg1, arg2, arg3, ...) is equivalent to arg1.new(arg2, arg3, ...)
  int nargs = lua_gettop(L);
  luaL_argcheck(L, nargs >= 1, 1, "expected at least 1 argument");
  luaL_argcheck(L, lua_type(L, 1) == LUA_TTABLE, 1, "expected a table");

  lua_getfield(L, 1, "new");
  lua_insert(L, 1);
  lua_remove(L, 1);
  lua_call(L, nargs, 1);
  return 1;
}

static int luaB_range(lua_State *L) {
  // range(start, stop, step)
  // returns a table of numbers from start to stop (inclusive) with step
  // if step is not provided, it defaults to 1
  int nargs = lua_gettop(L);
  luaL_argcheck(L, nargs >= 2, 2, "expected at least 2 arguments");
  luaL_argcheck(L, nargs <= 3, 3, "expected at most 3 arguments");

  int start = luaL_checkinteger(L, 1);
  int stop = luaL_checkinteger(L, 2);
  int step = 1;

  if (nargs >= 3) {
    step = luaL_checkinteger(L, 3);
    luaL_argcheck(L, step != 0, 3, "step cannot be zero");
  }

  if (start < 0) {
    start = stop + start + 1;
  }

  if (stop < 0) {
    stop = stop + stop + 1;
  }

  luaL_argcheck(L, start >= 1 && start <= stop, 1, "start index out of range");
  luaL_argcheck(L, stop >= 1 && stop <= stop, 2, "stop index out of range");

  lua_newtable(L);
  int j = 1;

  for (int i = start; i <= stop; i += step) {
    lua_pushinteger(L, i);
    lua_rawseti(L, -2, j);
    j++;
  }

  return 1;
}
static int newenum(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    luaL_checktype(L, 2, LUA_TTABLE);

    // Check that the name isnt already taken
    lua_getglobal(L, name);
    if (!lua_isnil(L, -1)) {
        return luaL_error(L, "enum name '%s' is already taken", name);
    }
    lua_pop(L, 1);
    // Create the enum table
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setglobal(L, name);
    // Add the enum values
    lua_pushnil(L);
    while (lua_next(L, 2) != 0) {
        lua_pushvalue(L, -2);
        lua_insert(L, -2);
        lua_settable(L, -4);
    }
    return 1;
}
static const luaL_Reg base_funcs[] = {
  {"assert", luaB_assert},
  {"range", luaB_range},
  {"enum", newenum},
  //{"new", luaB_new},
  //{"slice", luaB_slice},
  {"collectgarbage", luaB_collectgarbage},
  {"dofile", luaB_dofile},
  {"inputf", luaB_inputf},
  {"error", luaB_error},
  {"getmetatable", luaB_getmetatable},
  {"ipairs", luaB_ipairs},
  {"loadfile", luaB_loadfile},
  {"load", luaB_load},
  {"loadstring", luaB_load},
  {"next", luaB_next},
  {"pairs", luaB_pairs},
  {"try", luaB_pcall},
  {"print", luaB_print},
  {"warn", luaB_warn},
  {"info", luaB_info},
  {"rawequal", luaB_rawequal},
  {"rawlen", luaB_rawlen},
  {"rawget", luaB_rawget},
  {"rawset", luaB_rawset},
  {"select", luaB_select},
  {"setmetatable", luaB_setmetatable},
  {"tonumber", luaB_tonumber},
  {"tostring", luaB_tostring},
  {"type", luaB_type},
  {"xtry", luaB_xpcall},
  
  {"wait", luaB_wait},
  /*subwait*/
  {"swait", luaB_wait},
  {"mwait", luaB_mwait},
  {"uwait", luaB_uwait},
  /* placeholders */
  {"_G", NULL},
  {"_VERSION", NULL},
  {NULL, NULL}
};


LUAMOD_API int luaopen_base (lua_State *L) {
  /* open lib into global table */
  lua_pushglobaltable(L);
  luaL_setfuncs(L, base_funcs, 0);
  /* set global _G */
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "_G");
  /* set global _VERSION */
  lua_pushliteral(L, "lxx 1.0.0");
  lua_setfield(L, -2, "_VERSION");
  return 1;
}

