/*
** Base and coroutine library.
** Copyright (C) 2005-2020 Mike Pall. See Copyright Notice in luajit.h
**
** Major portions taken verbatim or adapted from the Lua interpreter.
** Copyright (C) 1994-2011 Lua.org, PUC-Rio. See Copyright Notice in lua.h
*/

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#define lib_base_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "lj_obj.h"
#include "lj_gc.h"
#include "lj_err.h"
#include "lj_debug.h"
#include "lj_str.h"
#include "lj_tab.h"
#include "lj_meta.h"
#include "lj_state.h"
#include "lj_frame.h"
#if LJ_HASFFI
#include "lj_ctype.h"
#include "lj_cconv.h"
#endif
#include "lj_bc.h"
#include "lj_ff.h"
#include "lj_dispatch.h"
#include "lj_char.h"
#include "lj_strscan.h"
#include "lj_strfmt.h"
#include "lj_lib.h"

/* -- Base library: checks ------------------------------------------------ */

#define LJLIB_MODULE_base

LJLIB_ASM(assert)		LJLIB_REC(.)
{
  lj_lib_checkany(L, 1);
  if (L->top == L->base+1)
    lj_err_caller(L, LJ_ERR_ASSERT);
  else if (tvisstr(L->base+1) || tvisnumber(L->base+1))
    lj_err_callermsg(L, strdata(lj_lib_checkstr(L, 2)));
  else
    lj_err_run(L);
  return FFH_UNREACHABLE;
}

/* ORDER LJ_T */
LJLIB_PUSH("null")
LJLIB_PUSH("boolean")
LJLIB_PUSH(top-1)  /* boolean */
LJLIB_PUSH("userdata")
LJLIB_PUSH("string")
LJLIB_PUSH("upval")
LJLIB_PUSH("thread")
LJLIB_PUSH("proto")
LJLIB_PUSH("function")
LJLIB_PUSH("trace")
LJLIB_PUSH("cdata")
LJLIB_PUSH("table")
LJLIB_PUSH(top-9)  /* userdata */
LJLIB_PUSH("number")
LJLIB_ASM_(type)		LJLIB_REC(.)
/* Recycle the lj_lib_checkany(L, 1) from assert. */

/* -- Base library: iterators --------------------------------------------- */

/* This solves a circular dependency problem -- change FF_next_N as needed. */
LJ_STATIC_ASSERT((int)FF_next == FF_next_N);

LJLIB_ASM(next)
{
  lj_lib_checktab(L, 1);
  return FFH_UNREACHABLE;
}

#if LJ_52 || LJ_HASFFI
static int ffh_pairs(lua_State *L, MMS mm)
{
  TValue *o = lj_lib_checkany(L, 1);
  cTValue *mo = lj_meta_lookup(L, o, mm);
  if ((LJ_52 || tviscdata(o)) && !tvisnil(mo)) {
    L->top = o+1;  /* Only keep one argument. */
    copyTV(L, L->base-1-LJ_FR2, mo);  /* Replace callable. */
    return FFH_TAILCALL;
  } else {
    if (!tvistab(o)) lj_err_argt(L, 1, LUA_TTABLE);
    if (LJ_FR2) { copyTV(L, o-1, o); o--; }
    setfuncV(L, o-1, funcV(lj_lib_upvalue(L, 1)));
    if (mm == MM_pairs) setnilV(o+1); else setintV(o+1, 0);
    return FFH_RES(3);
  }
}
#else
#define ffh_pairs(L, mm)	(lj_lib_checktab(L, 1), FFH_UNREACHABLE)
#endif

LJLIB_PUSH(lastcl)
LJLIB_ASM(pairs)		LJLIB_REC(xpairs 0)
{
  return ffh_pairs(L, MM_pairs);
}

LJLIB_NOREGUV LJLIB_ASM(ipairs_aux)	LJLIB_REC(.)
{
  lj_lib_checktab(L, 1);
  lj_lib_checkint(L, 2);
  return FFH_UNREACHABLE;
}

LJLIB_PUSH(lastcl)
LJLIB_ASM(ipairs)		LJLIB_REC(xpairs 1)
{
  return ffh_pairs(L, MM_ipairs);
}

/* -- Base library: getters and setters ----------------------------------- */

LJLIB_ASM_(getmetatable)	LJLIB_REC(.)
/* Recycle the lj_lib_checkany(L, 1) from assert. */

LJLIB_ASM(setmetatable)		LJLIB_REC(.)
{
  GCtab *t = lj_lib_checktab(L, 1);
  GCtab *mt = lj_lib_checktabornil(L, 2);
  if (!tvisnil(lj_meta_lookup(L, L->base, MM_metatable)))
    lj_err_caller(L, LJ_ERR_PROTMT);
  setgcref(t->metatable, obj2gco(mt));
  if (mt) { lj_gc_objbarriert(L, t, mt); }
  settabV(L, L->base-1-LJ_FR2, t);
  return FFH_RES(1);
}

LJLIB_CF(getfenv)		LJLIB_REC(.)
{
  GCfunc *fn;
  cTValue *o = L->base;
  if (!(o < L->top && tvisfunc(o))) {
    int level = lj_lib_optint(L, 1, 1);
    o = lj_debug_frame(L, level, &level);
    if (o == NULL)
      lj_err_arg(L, 1, LJ_ERR_INVLVL);
    if (LJ_FR2) o--;
  }
  fn = &gcval(o)->fn;
  settabV(L, L->top++, isluafunc(fn) ? tabref(fn->l.env) : tabref(L->env));
  return 1;
}

LJLIB_CF(setfenv)
{
  GCfunc *fn;
  GCtab *t = lj_lib_checktab(L, 2);
  cTValue *o = L->base;
  if (!(o < L->top && tvisfunc(o))) {
    int level = lj_lib_checkint(L, 1);
    if (level == 0) {
      /* NOBARRIER: A thread (i.e. L) is never black. */
      setgcref(L->env, obj2gco(t));
      return 0;
    }
    o = lj_debug_frame(L, level, &level);
    if (o == NULL)
      lj_err_arg(L, 1, LJ_ERR_INVLVL);
    if (LJ_FR2) o--;
  }
  fn = &gcval(o)->fn;
  if (!isluafunc(fn))
    lj_err_caller(L, LJ_ERR_SETFENV);
  setgcref(fn->l.env, obj2gco(t));
  lj_gc_objbarrier(L, obj2gco(fn), t);
  setfuncV(L, L->top++, fn);
  return 1;
}

LJLIB_ASM(rawget)		LJLIB_REC(.)
{
  lj_lib_checktab(L, 1);
  lj_lib_checkany(L, 2);
  return FFH_UNREACHABLE;
}

LJLIB_CF(rawset)		LJLIB_REC(.)
{
  lj_lib_checktab(L, 1);
  lj_lib_checkany(L, 2);
  L->top = 1+lj_lib_checkany(L, 3);
  lua_rawset(L, 1);
  return 1;
}

LJLIB_CF(rawequal)		LJLIB_REC(.)
{
  cTValue *o1 = lj_lib_checkany(L, 1);
  cTValue *o2 = lj_lib_checkany(L, 2);
  setboolV(L->top-1, lj_obj_equal(o1, o2));
  return 1;
}

#if LJ_52
LJLIB_CF(rawlen)		LJLIB_REC(.)
{
  cTValue *o = L->base;
  int32_t len;
  if (L->top > o && tvisstr(o))
    len = (int32_t)strV(o)->len;
  else
    len = (int32_t)lj_tab_len(lj_lib_checktab(L, 1));
  setintV(L->top-1, len);
  return 1;
}
#endif

LJLIB_CF(unpack)
{
  GCtab *t = lj_lib_checktab(L, 1);
  int32_t n, i = lj_lib_optint(L, 2, 1);
  int32_t e = (L->base+3-1 < L->top && !tvisnil(L->base+3-1)) ?
	      lj_lib_checkint(L, 3) : (int32_t)lj_tab_len(t);
  uint32_t nu;
  if (i > e) return 0;
  nu = (uint32_t)e - (uint32_t)i;
  n = (int32_t)(nu+1);
  if (nu >= LUAI_MAXCSTACK || !lua_checkstack(L, n))
    lj_err_caller(L, LJ_ERR_UNPACK);
  do {
    cTValue *tv = lj_tab_getint(t, i);
    if (tv) {
      copyTV(L, L->top++, tv);
    } else {
      setnilV(L->top++);
    }
  } while (i++ < e);
  return n;
}

LJLIB_CF(select)		LJLIB_REC(.)
{
  int32_t n = (int32_t)(L->top - L->base);
  if (n >= 1 && tvisstr(L->base) && *strVdata(L->base) == '#') {
    setintV(L->top-1, n-1);
    return 1;
  } else {
    int32_t i = lj_lib_checkint(L, 1);
    if (i < 0) i = n + i; else if (i > n) i = n;
    if (i < 1)
      lj_err_arg(L, 1, LJ_ERR_IDXRNG);
    return n - i;
  }
}

/* -- Base library: conversions ------------------------------------------- */

LJLIB_ASM(tonumber)		LJLIB_REC(.)
{
  int32_t base = lj_lib_optint(L, 2, 10);
  if (base == 10) {
    TValue *o = lj_lib_checkany(L, 1);
    if (lj_strscan_numberobj(o)) {
      copyTV(L, L->base-1-LJ_FR2, o);
      return FFH_RES(1);
    }
#if LJ_HASFFI
    if (tviscdata(o)) {
      CTState *cts = ctype_cts(L);
      CType *ct = lj_ctype_rawref(cts, cdataV(o)->ctypeid);
      if (ctype_isenum(ct->info)) ct = ctype_child(cts, ct);
      if (ctype_isnum(ct->info) || ctype_iscomplex(ct->info)) {
	if (LJ_DUALNUM && ctype_isinteger_or_bool(ct->info) &&
	    ct->size <= 4 && !(ct->size == 4 && (ct->info & CTF_UNSIGNED))) {
	  int32_t i;
	  lj_cconv_ct_tv(cts, ctype_get(cts, CTID_INT32), (uint8_t *)&i, o, 0);
	  setintV(L->base-1-LJ_FR2, i);
	  return FFH_RES(1);
	}
	lj_cconv_ct_tv(cts, ctype_get(cts, CTID_DOUBLE),
		       (uint8_t *)&(L->base-1-LJ_FR2)->n, o, 0);
	return FFH_RES(1);
      }
    }
#endif
  } else {
    const char *p = strdata(lj_lib_checkstr(L, 1));
    char *ep;
    unsigned int neg = 0;
    unsigned long ul;
    if (base < 2 || base > 36)
      lj_err_arg(L, 2, LJ_ERR_BASERNG);
    while (lj_char_isspace((unsigned char)(*p))) p++;
    if (*p == '-') { p++; neg = 1; } else if (*p == '+') { p++; }
    if (lj_char_isalnum((unsigned char)(*p))) {
      ul = strtoul(p, &ep, base);
      if (p != ep) {
	while (lj_char_isspace((unsigned char)(*ep))) ep++;
	if (*ep == '\0') {
	  if (LJ_DUALNUM && LJ_LIKELY(ul < 0x80000000u+neg)) {
	    if (neg) ul = -ul;
	    setintV(L->base-1-LJ_FR2, (int32_t)ul);
	  } else {
	    lua_Number n = (lua_Number)ul;
	    if (neg) n = -n;
	    setnumV(L->base-1-LJ_FR2, n);
	  }
	  return FFH_RES(1);
	}
      }
    }
  }
  setnilV(L->base-1-LJ_FR2);
  return FFH_RES(1);
}

LJLIB_ASM(tostring)		LJLIB_REC(.)
{
  TValue *o = lj_lib_checkany(L, 1);
  cTValue *mo;
  L->top = o+1;  /* Only keep one argument. */
  if (!tvisnil(mo = lj_meta_lookup(L, o, MM_tostring))) {
    copyTV(L, L->base-1-LJ_FR2, mo);  /* Replace callable. */
    return FFH_TAILCALL;
  }
  lj_gc_check(L);
  setstrV(L, L->base-1-LJ_FR2, lj_strfmt_obj(L, L->base));
  return FFH_RES(1);
}

/* -- Base library: throw and catch errors -------------------------------- */

LJLIB_CF(error)
{
  int32_t level = lj_lib_optint(L, 2, 1);
  lua_settop(L, 1);
  if (lua_isstring(L, 1) && level > 0) {
    luaL_where(L, level);
    lua_pushvalue(L, 1);
    lua_concat(L, 2);
  }
  return lua_error(L);
}

LJLIB_ASM(pcall)		LJLIB_REC(.)
{
  lj_lib_checkany(L, 1);
  lj_lib_checkfunc(L, 2);  /* For xpcall only. */
  return FFH_UNREACHABLE;
}
LJLIB_ASM_(xpcall)		LJLIB_REC(.)

/* -- Base library: load Lua code ----------------------------------------- */

static int load_aux(lua_State *L, int status, int envarg)
{
  if (status == LUA_OK) {
    if (tvistab(L->base+envarg-1)) {
      GCfunc *fn = funcV(L->top-1);
      GCtab *t = tabV(L->base+envarg-1);
      setgcref(fn->c.env, obj2gco(t));
      lj_gc_objbarrier(L, fn, t);
    }
    return 1;
  } else {
    setnilV(L->top-2);
    return 2;
  }
}

LJLIB_CF(loadfile)
{
  GCstr *fname = lj_lib_optstr(L, 1);
  GCstr *mode = lj_lib_optstr(L, 2);
  int status;
  lua_settop(L, 3);  /* Ensure env arg exists. */
  status = luaL_loadfilex(L, fname ? strdata(fname) : NULL,
			  mode ? strdata(mode) : NULL);
  return load_aux(L, status, 3);
}

static const char *reader_func(lua_State *L, void *ud, size_t *size)
{
  UNUSED(ud);
  luaL_checkstack(L, 2, "too many nested functions");
  copyTV(L, L->top++, L->base);
  lua_call(L, 0, 1);  /* Call user-supplied function. */
  L->top--;
  if (tvisnil(L->top)) {
    *size = 0;
    return NULL;
  } else if (tvisstr(L->top) || tvisnumber(L->top)) {
    copyTV(L, L->base+4, L->top);  /* Anchor string in reserved stack slot. */
    return lua_tolstring(L, 5, size);
  } else {
    lj_err_caller(L, LJ_ERR_RDRSTR);
    return NULL;
  }
}

LJLIB_CF(load)
{
  GCstr *name = lj_lib_optstr(L, 2);
  GCstr *mode = lj_lib_optstr(L, 3);
  int status;
  if (L->base < L->top && (tvisstr(L->base) || tvisnumber(L->base))) {
    GCstr *s = lj_lib_checkstr(L, 1);
    lua_settop(L, 4);  /* Ensure env arg exists. */
    status = luaL_loadbufferx(L, strdata(s), s->len, strdata(name ? name : s),
			      mode ? strdata(mode) : NULL);
  } else {
    lj_lib_checkfunc(L, 1);
    lua_settop(L, 5);  /* Reserve a slot for the string from the reader. */
    status = lua_loadx(L, reader_func, NULL, name ? strdata(name) : "=(load)",
		       mode ? strdata(mode) : NULL);
  }
  return load_aux(L, status, 4);
}

LJLIB_CF(loadstring)
{
  return lj_cf_load(L);
}

LJLIB_CF(dofile)
{
  GCstr *fname = lj_lib_optstr(L, 1);
  setnilV(L->top);
  L->top = L->base+1;
  if (luaL_loadfile(L, fname ? strdata(fname) : NULL) != LUA_OK)
    lua_error(L);
  lua_call(L, 0, LUA_MULTRET);
  return (int)(L->top - L->base) - 1;
}

/* -- Base library: GC control -------------------------------------------- */

LJLIB_CF(gcinfo)
{
  setintV(L->top++, (int32_t)(G(L)->gc.total >> 10));
  return 1;
}

LJLIB_CF(collectgarbage)
{
  int opt = lj_lib_checkopt(L, 1, LUA_GCCOLLECT,  /* ORDER LUA_GC* */
    "\4stop\7restart\7collect\5count\1\377\4step\10setpause\12setstepmul\1\377\11isrunning");
  int32_t data = lj_lib_optint(L, 2, 0);
  if (opt == LUA_GCCOUNT) {
    setnumV(L->top, (lua_Number)G(L)->gc.total/1024.0);
  } else {
    int res = lua_gc(L, opt, data);
    if (opt == LUA_GCSTEP || opt == LUA_GCISRUNNING)
      setboolV(L->top, res);
    else
      setintV(L->top, res);
  }
  L->top++;
  return 1;
}

/* -- Base library: miscellaneous functions ------------------------------- */

LJLIB_PUSH(top-2)  /* Upvalue holds weak table. */
LJLIB_CF(newproxy)
{
  lua_settop(L, 1);
  lua_newuserdata(L, 0);
  if (lua_toboolean(L, 1) == 0) {  /* newproxy(): without metatable. */
    return 1;
  } else if (lua_isboolean(L, 1)) {  /* newproxy(true): with metatable. */
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_pushboolean(L, 1);
    lua_rawset(L, lua_upvalueindex(1));  /* Remember mt in weak table. */
  } else {  /* newproxy(proxy): inherit metatable. */
    int validproxy = 0;
    if (lua_getmetatable(L, 1)) {
      lua_rawget(L, lua_upvalueindex(1));
      validproxy = lua_toboolean(L, -1);
      lua_pop(L, 1);
    }
    if (!validproxy)
      lj_err_arg(L, 1, LJ_ERR_NOPROXY);
    lua_getmetatable(L, 1);
  }
  lua_setmetatable(L, 2);
  return 1;
}

/* -- Output functions ---------------------------------------------------- */
LJLIB_PUSH("tostring")
LJLIB_CF(print)
{
  ptrdiff_t i, nargs = L->top - L->base;
  cTValue *tv = lj_tab_getstr(tabref(L->env), strV(lj_lib_upvalue(L, 1)));
  int shortcut;
  if (tv && !tvisnil(tv)) {
    copyTV(L, L->top++, tv);
  } else {
    setstrV(L, L->top++, strV(lj_lib_upvalue(L, 1)));
    lua_gettable(L, LUA_GLOBALSINDEX);
    tv = L->top-1;
  }
  shortcut = (tvisfunc(tv) && funcV(tv)->c.ffid == FF_tostring)
	      && !gcrefu(basemt_it(G(L), LJ_TNUMX));
  for (i = 0; i < nargs; i++) {
    cTValue *o = &L->base[i];
    const char *str;
    size_t size;
    MSize len;
    if (shortcut && (str = lj_strfmt_wstrnum(L, o, &len)) != NULL) {
      size = len;
    } else {
      copyTV(L, L->top+1, o);
      copyTV(L, L->top, L->top-1);
      L->top += 2;
      lua_call(L, 1, 1);
      str = lua_tolstring(L, -1, &size);
      if (!str)
	lj_err_caller(L, LJ_ERR_PRTOSTR);
      L->top--;
    }
    if (i)
      putchar('\t');
    fwrite(str, 1, size, stdout);
  }
  putchar('\n');
  return 0;
}

LJLIB_CF(inputf) {
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
LJLIB_CF(warn) {
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
    lua_pushstring(L, "\033[1;33mwarning: \033[0m");
    lua_insert(L, -2);
    lua_concat(L, 2);
    s = lua_tolstring(L, -1, &l);  /* get result */
    if (s == NULL)
      return luaL_error(L, "'tostring' must return a string to 'warn'");
    lua_call(L, 1, 0);
  }
  return 0;
}

LJLIB_CF(info) {
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
    lua_pushstring(L, "\033[1;34minfo: \033[0m");
    lua_insert(L, -2);
    lua_concat(L, 2);
    s = lua_tolstring(L, -1, &l);  /* get result */
    if (s == NULL)
      return luaL_error(L, "'tostring' must return a string to 'info'");
    lua_call(L, 1, 0);
  }
  return 0;
}

/* -- Time functions ------------------------------------------------------ */
LJLIB_CF(uwait) {
  float m = luaL_optnumber(L, 1, 1);
  #ifdef _WIN32
  if (m >= 1000) {
    Sleep(m);
  } else {
    lua_error(L, "uwait not supported for less than 1000 microseconds on Windows.");
  }
  #else
  usleep(m);
  #endif
  // returns a function that waits m without needing any arguments
  /*
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushnumber(L, seconds * 1000000);
  lua_pushcclosure(L, luaB_uwait, 1);*/
  return 1;
}

LJLIB_CF(wait) {
  float seconds = luaL_optnumber(L, 1, 1);
  #ifdef _WIN32
  Sleep(seconds * 1000);
  #else
  usleep(seconds * 1000000);
  #endif
  
  // returns a function that waits m without needing any arguments
  /*
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushnumber(L, seconds * 1000000);
  lua_pushcclosure(L, luaB_uwait, 1);*/
  return 1;
}

LJLIB_CF(swait) {
  float seconds = luaL_optnumber(L, 1, 1);
  #ifdef _WIN32
  Sleep(seconds * 1000);
  #else
  usleep(seconds * 1000000);
  #endif
  
  // returns a function that waits m without needing any arguments
  /*
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushnumber(L, seconds * 1000000);
  lua_pushcclosure(L, luaB_uwait, 1);*/
  return 1;
}

LJLIB_CF(mwait) {
  float ms = luaL_optnumber(L, 1, 1);
  #ifdef _WIN32
  Sleep(ms);
  #else
  usleep(ms * 1000);
  #endif
  // returns a function that waits m without needing any arguments
  /*
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushnumber(L, seconds * 1000000);
  lua_pushcclosure(L, luaB_uwait, 1);*/
  return 1;
}
/* -- Other functions ----------------------------------------------------- */
LJLIB_CF(typedef){
  // typedef(name, table)
  // adds a new method to the table if it doesnt exist
  // saves the table in the global namespace
  // the new method returns an unlocked version of the table
  // otherwise the table will be read-only
  const char *name = luaL_checkstring(L, 1);
  luaL_checktype(L, 2, LUA_TTABLE);

  // Check that the name isnt already taken
  lua_getglobal(L, name);
  if (!lua_isnil(L, -1)) {
    return luaL_error(L, "type name '%s' is already taken", name);
  }
  lua_pop(L, 1);
  // Duplicate the inputted table and save it globally
  lua_pushvalue(L, 2);
  lua_setglobal(L, name);

  return 1;
}
LJLIB_CF(range) {
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
LJLIB_CF(enum) {
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
LJLIB_PUSH(top-3)
LJLIB_SET(_VERSION)

#include "lj_libdef.h"

/* -- Coroutine library --------------------------------------------------- */

#define LJLIB_MODULE_coroutine

LJLIB_CF(coroutine_status)
{
  const char *s;
  lua_State *co;
  if (!(L->top > L->base && tvisthread(L->base)))
    lj_err_arg(L, 1, LJ_ERR_NOCORO);
  co = threadV(L->base);
  if (co == L) s = "running";
  else if (co->status == LUA_YIELD) s = "suspended";
  else if (co->status != LUA_OK) s = "dead";
  else if (co->base > tvref(co->stack)+1+LJ_FR2) s = "normal";
  else if (co->top == co->base) s = "dead";
  else s = "suspended";
  lua_pushstring(L, s);
  return 1;
}

LJLIB_CF(coroutine_running)
{
#if LJ_52
  int ismain = lua_pushthread(L);
  setboolV(L->top++, ismain);
  return 2;
#else
  if (lua_pushthread(L))
    setnilV(L->top++);
  return 1;
#endif
}

LJLIB_CF(coroutine_isyieldable)
{
  setboolV(L->top++, cframe_canyield(L->cframe));
  return 1;
}

LJLIB_CF(coroutine_create)
{
  lua_State *L1;
  if (!(L->base < L->top && tvisfunc(L->base)))
    lj_err_argt(L, 1, LUA_TFUNCTION);
  L1 = lua_newthread(L);
  setfuncV(L, L1->top++, funcV(L->base));
  return 1;
}

LJLIB_ASM(coroutine_yield)
{
  lj_err_caller(L, LJ_ERR_CYIELD);
  return FFH_UNREACHABLE;
}

static int ffh_resume(lua_State *L, lua_State *co, int wrap)
{
  if (co->cframe != NULL || co->status > LUA_YIELD ||
      (co->status == LUA_OK && co->top == co->base)) {
    ErrMsg em = co->cframe ? LJ_ERR_CORUN : LJ_ERR_CODEAD;
    if (wrap) lj_err_caller(L, em);
    setboolV(L->base-1-LJ_FR2, 0);
    setstrV(L, L->base-LJ_FR2, lj_err_str(L, em));
    return FFH_RES(2);
  }
  lj_state_growstack(co, (MSize)(L->top - L->base));
  return FFH_RETRY;
}

LJLIB_ASM(coroutine_resume)
{
  if (!(L->top > L->base && tvisthread(L->base)))
    lj_err_arg(L, 1, LJ_ERR_NOCORO);
  return ffh_resume(L, threadV(L->base), 0);
}

LJLIB_NOREG LJLIB_ASM(coroutine_wrap_aux)
{
  return ffh_resume(L, threadV(lj_lib_upvalue(L, 1)), 1);
}

//
/* Inline declarations. */

LJ_ASMF void lj_ff_coroutine_wrap_aux(void);
#if !(LJ_TARGET_MIPS && defined(ljamalg_c))
LJ_FUNCA_NORET void LJ_FASTCALL lj_ffh_coroutine_wrap_err(lua_State *L,
							  lua_State *co);
#endif

//
/* Error handler, called from assembler VM. */

void LJ_FASTCALL lj_ffh_coroutine_wrap_err(lua_State *L, lua_State *co)
{
  co->top--; copyTV(L, L->top, co->top); L->top++;
  if (tvisstr(L->top-1))
    lj_err_callermsg(L, strVdata(L->top-1));
  else
    lj_err_run(L);
}

///* Forward declaration. */

static void setpc_wrap_aux(lua_State *L, GCfunc *fn);

LJLIB_CF(coroutine_wrap)
{
  GCfunc *fn;
  lj_cf_coroutine_create(L);
  fn = lj_lib_pushcc(L, lj_ffh_coroutine_wrap_aux, FF_coroutine_wrap_aux, 1);
  setpc_wrap_aux(L, fn);
  return 1;
}

#include "lj_libdef.h"

/* Fix the PC of wrap_aux. Really ugly workaround. */
static void setpc_wrap_aux(lua_State *L, GCfunc *fn)
{
  setmref(fn->c.pc, &L2GG(L)->bcff[lj_lib_init_coroutine[1]+2]);
}

/* ------------------------------------------------------------------------ */

static void newproxy_weaktable(lua_State *L)
{
  /* NOBARRIER: The table is new (marked white). */
  GCtab *t = lj_tab_new(L, 0, 1);
  settabV(L, L->top++, t);
  setgcref(t->metatable, obj2gco(t));
  setstrV(L, lj_tab_setstr(L, t, lj_str_newlit(L, "__mode")),
	    lj_str_newlit(L, "kv"));
  t->nomm = (uint8_t)(~(1u<<MM_mode));
}

// The following section has been modified from the original LuaJIT source to include Cobalt libraries
/*
int lj_lib_init_color(lua_State *L){
  luaopen_color(L);
}
int lj_lib_cf_luaget_color(lua_State *L){
  luaopen_color(L);
}*/
LUALIB_API int luaopen_base(lua_State *L)
{
  /* NOBARRIER: Table and value are the same. */
  GCtab *env = tabref(L->env);
  settabV(L, lj_tab_setstr(L, env, lj_str_newlit(L, "_G")), env);
  lua_pushliteral(L, LUA_VERSION);  /* top-3. */
  newproxy_weaktable(L);  /* top-2. */
  LJ_LIB_REG(L, "_G", base);
  //LJ_LIB_REG(L, LUA_COLIBNAME, coroutine); Disable coroutine library, use Async instead for real threading
  return 2;
}
