/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "ltrace.h"

#include <stdio.h>

lua_State* Lua_newstate(lua_Alloc f, void* ud, const char* _FILE, int _LINE,
                        const char* _FUNC) {
  lua_State* rc = (lua_newstate)(f, ud);
  printf("%s:%d [%s] lua_newstate f=%p ud=%p -> %p\n", _FILE, _LINE, _FUNC, f,
         ud, rc);
  return rc;
}

void Lua_close(lua_State* L, const char* _FILE, int _LINE, const char* _FUNC) {
  (lua_close)(L);
  printf("%s:%d [%s] lua_close L=%p\n", _FILE, _LINE, _FUNC, L);
}

lua_State* Lua_newthread(lua_State* L, const char* _FILE, int _LINE,
                         const char* _FUNC) {
  lua_State* rc = (lua_newthread)(L);
  printf("%s:%d [%s] lua_newthread L=%p -> %p\n", _FILE, _LINE, _FUNC, L, rc);
  return rc;
}

int Lua_resetthread(lua_State* L, const char* _FILE, int _LINE,
                    const char* _FUNC) {
  int rc = (lua_resetthread)(L);
  printf("%s:%d [%s] lua_resetthread L=%p -> %d\n", _FILE, _LINE, _FUNC, L, rc);
  return rc;
}

lua_CFunction Lua_atpanic(lua_State* L, lua_CFunction panicf, const char* _FILE,
                          int _LINE, const char* _FUNC) {
  lua_CFunction rc = (lua_atpanic)(L, panicf);
  printf("%s:%d [%s] lua_atpanic L=%p panicf=%p -> %p\n", _FILE, _LINE, _FUNC,
         L, panicf, rc);
  return rc;
}

lua_Number Lua_version(lua_State* L, const char* _FILE, int _LINE,
                       const char* _FUNC) {
  lua_Number rc = (lua_version)(L);
  printf("%s:%d [%s] lua_version L=%p -> %g\n", _FILE, _LINE, _FUNC, L, rc);
  return rc;
}

int Lua_absindex(lua_State* L, int idx, const char* _FILE, int _LINE,
                 const char* _FUNC) {
  int rc = (lua_absindex)(L, idx);
  printf("%s:%d [%s] lua_absindex L=%p idx=%d -> %d\n", _FILE, _LINE, _FUNC, L,
         idx, rc);
  return rc;
}

int Lua_gettop(lua_State* L, const char* _FILE, int _LINE, const char* _FUNC) {
  int rc = (lua_gettop)(L);
  printf("%s:%d [%s] lua_gettop L=%p -> %d\n", _FILE, _LINE, _FUNC, L, rc);
  return rc;
}

void Lua_settop(lua_State* L, int idx, const char* _FILE, int _LINE,
                const char* _FUNC) {
  (lua_settop)(L, idx);
  printf("%s:%d [%s] lua_settop L=%p idx=%d\n", _FILE, _LINE, _FUNC, L, idx);
}

void Lua_pushvalue(lua_State* L, int idx, const char* _FILE, int _LINE,
                   const char* _FUNC) {
  (lua_pushvalue)(L, idx);
  printf("%s:%d [%s] lua_pushvalue L=%p idx=%d\n", _FILE, _LINE, _FUNC, L, idx);
}

void Lua_rotate(lua_State* L, int idx, int n, const char* _FILE, int _LINE,
                const char* _FUNC) {
  (lua_rotate)(L, idx, n);
  printf("%s:%d [%s] lua_rotate L=%p idx=%d n=%d\n", _FILE, _LINE, _FUNC, L,
         idx, n);
}

void Lua_copy(lua_State* L, int fromidx, int toidx, const char* _FILE,
              int _LINE, const char* _FUNC) {
  (lua_copy)(L, fromidx, toidx);
  printf("%s:%d [%s] lua_copy L=%p fromidx=%d toidx=%d\n", _FILE, _LINE, _FUNC,
         L, fromidx, toidx);
}

int Lua_checkstack(lua_State* L, int n, const char* _FILE, int _LINE,
                   const char* _FUNC) {
  int rc = (lua_checkstack)(L, n);
  printf("%s:%d [%s] lua_checkstack L=%p n=%d -> %d\n", _FILE, _LINE, _FUNC, L,
         n, rc);
  return rc;
}

void Lua_xmove(lua_State* from, lua_State* to, int n, const char* _FILE,
               int _LINE, const char* _FUNC) {
  (lua_xmove)(from, to, n);
  printf("%s:%d [%s] lua_xmove from=%p to=%p n=%d\n", _FILE, _LINE, _FUNC, from,
         to, n);
}

int Lua_isnumber(lua_State* L, int idx, const char* _FILE, int _LINE,
                 const char* _FUNC) {
  int rc = (lua_isnumber)(L, idx);
  printf("%s:%d [%s] lua_isnumber L=%p idx=%d -> %d\n", _FILE, _LINE, _FUNC, L,
         idx, rc);
  return rc;
}

int Lua_isstring(lua_State* L, int idx, const char* _FILE, int _LINE,
                 const char* _FUNC) {
  int rc = (lua_isstring)(L, idx);
  printf("%s:%d [%s] lua_isstring L=%p idx=%d -> %d\n", _FILE, _LINE, _FUNC, L,
         idx, rc);
  return rc;
}

int Lua_iscfunction(lua_State* L, int idx, const char* _FILE, int _LINE,
                    const char* _FUNC) {
  int rc = (lua_iscfunction)(L, idx);
  printf("%s:%d [%s] lua_iscfunction L=%p idx=%d -> %d\n", _FILE, _LINE, _FUNC,
         L, idx, rc);
  return rc;
}

int Lua_isinteger(lua_State* L, int idx, const char* _FILE, int _LINE,
                  const char* _FUNC) {
  int rc = (lua_isinteger)(L, idx);
  printf("%s:%d [%s] lua_isinteger L=%p idx=%d -> %d\n", _FILE, _LINE, _FUNC, L,
         idx, rc);
  return rc;
}

int Lua_isuserdata(lua_State* L, int idx, const char* _FILE, int _LINE,
                   const char* _FUNC) {
  int rc = (lua_isuserdata)(L, idx);
  printf("%s:%d [%s] lua_isuserdata L=%p idx=%d -> %d\n", _FILE, _LINE, _FUNC,
         L, idx, rc);
  return rc;
}

int Lua_type(lua_State* L, int idx, const char* _FILE, int _LINE,
             const char* _FUNC) {
  int rc = (lua_type)(L, idx);
  printf("%s:%d [%s] lua_type L=%p idx=%d -> %d\n", _FILE, _LINE, _FUNC, L, idx,
         rc);
  return rc;
}

const char* Lua_typename(lua_State* L, int tp, const char* _FILE, int _LINE,
                         const char* _FUNC) {
  const char* rc = (lua_typename)(L, tp);
  printf("%s:%d [%s] lua_typename L=%p tp=%d -> \"%s\"\n", _FILE, _LINE, _FUNC,
         L, tp, rc);
  return rc;
}

lua_Number Lua_tonumberx(lua_State* L, int idx, int* isnum, const char* _FILE,
                         int _LINE, const char* _FUNC) {
  lua_Number rc = (lua_tonumberx)(L, idx, isnum);
  printf("%s:%d [%s] lua_tonumberx L=%p idx=%d isnum=%p -> %g\n", _FILE, _LINE,
         _FUNC, L, idx, isnum, rc);
  return rc;
}

lua_Integer Lua_tointegerx(lua_State* L, int idx, int* isnum, const char* _FILE,
                           int _LINE, const char* _FUNC) {
  lua_Integer rc = (lua_tointegerx)(L, idx, isnum);
  printf("%s:%d [%s] lua_tointegerx L=%p idx=%d isnum=%p -> %d\n", _FILE, _LINE,
         _FUNC, L, idx, isnum, rc);
  return rc;
}

int Lua_toboolean(lua_State* L, int idx, const char* _FILE, int _LINE,
                  const char* _FUNC) {
  int rc = (lua_toboolean)(L, idx);
  printf("%s:%d [%s] lua_toboolean L=%p idx=%d -> %d\n", _FILE, _LINE, _FUNC, L,
         idx, rc);
  return rc;
}

const char* Lua_tolstring(lua_State* L, int idx, size_t* len, const char* _FILE,
                          int _LINE, const char* _FUNC) {
  const char* rc = (lua_tolstring)(L, idx, len);
  printf("%s:%d [%s] lua_tolstring L=%p idx=%d len=%p -> \"%s\"\n", _FILE,
         _LINE, _FUNC, L, idx, len, rc);
  return rc;
}

lua_Unsigned Lua_rawlen(lua_State* L, int idx, const char* _FILE, int _LINE,
                        const char* _FUNC) {
  lua_Unsigned rc = (lua_rawlen)(L, idx);
  printf("%s:%d [%s] lua_rawlen L=%p idx=%d -> %u\n", _FILE, _LINE, _FUNC, L,
         idx, rc);
  return rc;
}

lua_CFunction Lua_tocfunction(lua_State* L, int idx, const char* _FILE,
                              int _LINE, const char* _FUNC) {
  lua_CFunction rc = (lua_tocfunction)(L, idx);
  printf("%s:%d [%s] lua_tocfunction L=%p idx=%d -> %p\n", _FILE, _LINE, _FUNC,
         L, idx, rc);
  return rc;
}

void* Lua_touserdata(lua_State* L, int idx, const char* _FILE, int _LINE,
                     const char* _FUNC) {
  void* rc = (lua_touserdata)(L, idx);
  printf("%s:%d [%s] lua_touserdata L=%p idx=%d -> %p\n", _FILE, _LINE, _FUNC,
         L, idx, rc);
  return rc;
}

lua_State* Lua_tothread(lua_State* L, int idx, const char* _FILE, int _LINE,
                        const char* _FUNC) {
  lua_State* rc = (lua_tothread)(L, idx);
  printf("%s:%d [%s] lua_tothread L=%p idx=%d -> %p\n", _FILE, _LINE, _FUNC, L,
         idx, rc);
  return rc;
}

const void* Lua_topointer(lua_State* L, int idx, const char* _FILE, int _LINE,
                          const char* _FUNC) {
  const void* rc = (lua_topointer)(L, idx);
  printf("%s:%d [%s] lua_topointer L=%p idx=%d -> %p\n", _FILE, _LINE, _FUNC, L,
         idx, rc);
  return rc;
}

void Lua_arith(lua_State* L, int op, const char* _FILE, int _LINE,
               const char* _FUNC) {
  (lua_arith)(L, op);
  printf("%s:%d [%s] lua_arith L=%p op=%d\n", _FILE, _LINE, _FUNC, L, op);
}

int Lua_rawequal(lua_State* L, int idx1, int idx2, const char* _FILE, int _LINE,
                 const char* _FUNC) {
  int rc = (lua_rawequal)(L, idx1, idx2);
  printf("%s:%d [%s] lua_rawequal L=%p idx1=%d idx2=%d -> %d\n", _FILE, _LINE,
         _FUNC, L, idx1, idx2, rc);
  return rc;
}

int Lua_compare(lua_State* L, int idx1, int idx2, int op, const char* _FILE,
                int _LINE, const char* _FUNC) {
  int rc = (lua_compare)(L, idx1, idx2, op);
  printf("%s:%d [%s] lua_compare L=%p idx1=%d idx2=%d op=%d -> %d\n", _FILE,
         _LINE, _FUNC, L, idx1, idx2, op, rc);
  return rc;
}

void Lua_pushnil(lua_State* L, const char* _FILE, int _LINE,
                 const char* _FUNC) {
  (lua_pushnil)(L);
  printf("%s:%d [%s] lua_pushnil L=%p\n", _FILE, _LINE, _FUNC, L);
}

void Lua_pushnumber(lua_State* L, lua_Number n, const char* _FILE, int _LINE,
                    const char* _FUNC) {
  (lua_pushnumber)(L, n);
  printf("%s:%d [%s] lua_pushnumber L=%p n=%g\n", _FILE, _LINE, _FUNC, L, n);
}

void Lua_pushinteger(lua_State* L, lua_Integer n, const char* _FILE, int _LINE,
                     const char* _FUNC) {
  (lua_pushinteger)(L, n);
  printf("%s:%d [%s] lua_pushinteger L=%p n=%d\n", _FILE, _LINE, _FUNC, L, n);
}

const char* Lua_pushlstring(lua_State* L, const char* s, size_t len,
                            const char* _FILE, int _LINE, const char* _FUNC) {
  const char* rc = (lua_pushlstring)(L, s, len);
  printf("%s:%d [%s] lua_pushlstring L=%p s=\"%s\" len=%u -> \"%s\"\n", _FILE,
         _LINE, _FUNC, L, s, len, rc);
  return rc;
}

const char* Lua_pushstring(lua_State* L, const char* s, const char* _FILE,
                           int _LINE, const char* _FUNC) {
  const char* rc = (lua_pushstring)(L, s);
  printf("%s:%d [%s] lua_pushstring L=%p s=\"%s\" -> \"%s\"\n", _FILE, _LINE,
         _FUNC, L, s, rc);
  return rc;
}

void Lua_pushcclosure(lua_State* L, lua_CFunction fn, int n, const char* _FILE,
                      int _LINE, const char* _FUNC) {
  (lua_pushcclosure)(L, fn, n);
  printf("%s:%d [%s] lua_pushcclosure L=%p fn=%p n=%d\n", _FILE, _LINE, _FUNC,
         L, fn, n);
}

void Lua_pushboolean(lua_State* L, int b, const char* _FILE, int _LINE,
                     const char* _FUNC) {
  (lua_pushboolean)(L, b);
  printf("%s:%d [%s] lua_pushboolean L=%p b=%d\n", _FILE, _LINE, _FUNC, L, b);
}

void Lua_pushlightuserdata(lua_State* L, void* p, const char* _FILE, int _LINE,
                           const char* _FUNC) {
  (lua_pushlightuserdata)(L, p);
  printf("%s:%d [%s] lua_pushlightuserdata L=%p p=%p\n", _FILE, _LINE, _FUNC, L,
         p);
}

int Lua_pushthread(lua_State* L, const char* _FILE, int _LINE,
                   const char* _FUNC) {
  int rc = (lua_pushthread)(L);
  printf("%s:%d [%s] lua_pushthread L=%p -> %d\n", _FILE, _LINE, _FUNC, L, rc);
  return rc;
}

int Lua_getglobal(lua_State* L, const char* name, const char* _FILE, int _LINE,
                  const char* _FUNC) {
  int rc = (lua_getglobal)(L, name);
  printf("%s:%d [%s] lua_getglobal L=%p name=\"%s\" -> %d\n", _FILE, _LINE,
         _FUNC, L, name, rc);
  return rc;
}

int Lua_gettable(lua_State* L, int idx, const char* _FILE, int _LINE,
                 const char* _FUNC) {
  int rc = (lua_gettable)(L, idx);
  printf("%s:%d [%s] lua_gettable L=%p idx=%d -> %d\n", _FILE, _LINE, _FUNC, L,
         idx, rc);
  return rc;
}

int Lua_getfield(lua_State* L, int idx, const char* k, const char* _FILE,
                 int _LINE, const char* _FUNC) {
  int rc = (lua_getfield)(L, idx, k);
  printf("%s:%d [%s] lua_getfield L=%p idx=%d k=\"%s\" -> %d\n", _FILE, _LINE,
         _FUNC, L, idx, k, rc);
  return rc;
}

int Lua_geti(lua_State* L, int idx, lua_Integer n, const char* _FILE, int _LINE,
             const char* _FUNC) {
  int rc = (lua_geti)(L, idx, n);
  printf("%s:%d [%s] lua_geti L=%p idx=%d n=%d -> %d\n", _FILE, _LINE, _FUNC, L,
         idx, n, rc);
  return rc;
}

int Lua_rawget(lua_State* L, int idx, const char* _FILE, int _LINE,
               const char* _FUNC) {
  int rc = (lua_rawget)(L, idx);
  printf("%s:%d [%s] lua_rawget L=%p idx=%d -> %d\n", _FILE, _LINE, _FUNC, L,
         idx, rc);
  return rc;
}

int Lua_rawgeti(lua_State* L, int idx, lua_Integer n, const char* _FILE,
                int _LINE, const char* _FUNC) {
  int rc = (lua_rawgeti)(L, idx, n);
  printf("%s:%d [%s] lua_rawgeti L=%p idx=%d n=%d -> %d\n", _FILE, _LINE, _FUNC,
         L, idx, n, rc);
  return rc;
}

int Lua_rawgetp(lua_State* L, int idx, const void* p, const char* _FILE,
                int _LINE, const char* _FUNC) {
  int rc = (lua_rawgetp)(L, idx, p);
  printf("%s:%d [%s] lua_rawgetp L=%p idx=%d p=%p -> %d\n", _FILE, _LINE, _FUNC,
         L, idx, p, rc);
  return rc;
}

void Lua_createtable(lua_State* L, int narr, int nrec, const char* _FILE,
                     int _LINE, const char* _FUNC) {
  (lua_createtable)(L, narr, nrec);
  printf("%s:%d [%s] lua_createtable L=%p narr=%d nrec=%d\n", _FILE, _LINE,
         _FUNC, L, narr, nrec);
}

void* Lua_newuserdatauv(lua_State* L, size_t sz, int nuvalue, const char* _FILE,
                        int _LINE, const char* _FUNC) {
  void* rc = (lua_newuserdatauv)(L, sz, nuvalue);
  printf("%s:%d [%s] lua_newuserdatauv L=%p sz=%u nuvalue=%d -> %p\n", _FILE,
         _LINE, _FUNC, L, sz, nuvalue, rc);
  return rc;
}

int Lua_getmetatable(lua_State* L, int objindex, const char* _FILE, int _LINE,
                     const char* _FUNC) {
  int rc = (lua_getmetatable)(L, objindex);
  printf("%s:%d [%s] lua_getmetatable L=%p objindex=%d -> %d\n", _FILE, _LINE,
         _FUNC, L, objindex, rc);
  return rc;
}

int Lua_getiuservalue(lua_State* L, int idx, int n, const char* _FILE,
                      int _LINE, const char* _FUNC) {
  int rc = (lua_getiuservalue)(L, idx, n);
  printf("%s:%d [%s] lua_getiuservalue L=%p idx=%d n=%d -> %d\n", _FILE, _LINE,
         _FUNC, L, idx, n, rc);
  return rc;
}

void Lua_setglobal(lua_State* L, const char* name, const char* _FILE, int _LINE,
                   const char* _FUNC) {
  (lua_setglobal)(L, name);
  printf("%s:%d [%s] lua_setglobal L=%p name=\"%s\"\n", _FILE, _LINE, _FUNC, L,
         name);
}

void Lua_settable(lua_State* L, int idx, const char* _FILE, int _LINE,
                  const char* _FUNC) {
  (lua_settable)(L, idx);
  printf("%s:%d [%s] lua_settable L=%p idx=%d\n", _FILE, _LINE, _FUNC, L, idx);
}

void Lua_setfield(lua_State* L, int idx, const char* k, const char* _FILE,
                  int _LINE, const char* _FUNC) {
  (lua_setfield)(L, idx, k);
  printf("%s:%d [%s] lua_setfield L=%p idx=%d k=\"%s\"\n", _FILE, _LINE, _FUNC,
         L, idx, k);
}

void Lua_seti(lua_State* L, int idx, lua_Integer n, const char* _FILE,
              int _LINE, const char* _FUNC) {
  (lua_seti)(L, idx, n);
  printf("%s:%d [%s] lua_seti L=%p idx=%d n=%d\n", _FILE, _LINE, _FUNC, L, idx,
         n);
}

void Lua_rawset(lua_State* L, int idx, const char* _FILE, int _LINE,
                const char* _FUNC) {
  (lua_rawset)(L, idx);
  printf("%s:%d [%s] lua_rawset L=%p idx=%d\n", _FILE, _LINE, _FUNC, L, idx);
}

void Lua_rawseti(lua_State* L, int idx, lua_Integer n, const char* _FILE,
                 int _LINE, const char* _FUNC) {
  (lua_rawseti)(L, idx, n);
  printf("%s:%d [%s] lua_rawseti L=%p idx=%d n=%d\n", _FILE, _LINE, _FUNC, L,
         idx, n);
}

void Lua_rawsetp(lua_State* L, int idx, const void* p, const char* _FILE,
                 int _LINE, const char* _FUNC) {
  (lua_rawsetp)(L, idx, p);
  printf("%s:%d [%s] lua_rawsetp L=%p idx=%d p=%p\n", _FILE, _LINE, _FUNC, L,
         idx, p);
}

int Lua_setmetatable(lua_State* L, int objindex, const char* _FILE, int _LINE,
                     const char* _FUNC) {
  int rc = (lua_setmetatable)(L, objindex);
  printf("%s:%d [%s] lua_setmetatable L=%p objindex=%d -> %d\n", _FILE, _LINE,
         _FUNC, L, objindex, rc);
  return rc;
}

int Lua_setiuservalue(lua_State* L, int idx, int n, const char* _FILE,
                      int _LINE, const char* _FUNC) {
  int rc = (lua_setiuservalue)(L, idx, n);
  printf("%s:%d [%s] lua_setiuservalue L=%p idx=%d n=%d -> %d\n", _FILE, _LINE,
         _FUNC, L, idx, n, rc);
  return rc;
}

void Lua_callk(lua_State* L, int nargs, int nresults, lua_KContext ctx,
               lua_KFunction k, const char* _FILE, int _LINE,
               const char* _FUNC) {
  (lua_callk)(L, nargs, nresults, ctx, k);
  printf("%s:%d [%s] lua_callk L=%p nargs=%d nresults=%d ctx=%p k=%p\n", _FILE,
         _LINE, _FUNC, L, nargs, nresults, ctx, k);
}

int Lua_pcallk(lua_State* L, int nargs, int nresults, int errfunc,
               lua_KContext ctx, lua_KFunction k, const char* _FILE, int _LINE,
               const char* _FUNC) {
  int rc = (lua_pcallk)(L, nargs, nresults, errfunc, ctx, k);
  printf(
      "%s:%d [%s] lua_pcallk L=%p nargs=%d nresults=%d errfunc=%d ctx=%p k=%p "
      "-> %d\n",
      _FILE, _LINE, _FUNC, L, nargs, nresults, errfunc, ctx, k, rc);
  return rc;
}

int Lua_load(lua_State* L, lua_Reader reader, void* dt, const char* chunkname,
             const char* mode, const char* _FILE, int _LINE,
             const char* _FUNC) {
  int rc = (lua_load)(L, reader, dt, chunkname, mode);
  printf(
      "%s:%d [%s] lua_load L=%p reader=%p dt=%p chunkname=\"%s\" mode=\"%s\" "
      "-> %d\n",
      _FILE, _LINE, _FUNC, L, reader, dt, chunkname, mode, rc);
  return rc;
}

int Lua_dump(lua_State* L, lua_Writer writer, void* data, int strip,
             const char* _FILE, int _LINE, const char* _FUNC) {
  int rc = (lua_dump)(L, writer, data, strip);
  printf("%s:%d [%s] lua_dump L=%p writer=%p data=%p strip=%d -> %d\n", _FILE,
         _LINE, _FUNC, L, writer, data, strip, rc);
  return rc;
}

int Lua_yieldk(lua_State* L, int nresults, lua_KContext ctx, lua_KFunction k,
               const char* _FILE, int _LINE, const char* _FUNC) {
  int rc = (lua_yieldk)(L, nresults, ctx, k);
  printf("%s:%d [%s] lua_yieldk L=%p nresults=%d ctx=%p k=%p -> %d\n", _FILE,
         _LINE, _FUNC, L, nresults, ctx, k, rc);
  return rc;
}

int Lua_resume(lua_State* L, lua_State* from, int narg, int* nres,
               const char* _FILE, int _LINE, const char* _FUNC) {
  int rc = (lua_resume)(L, from, narg, nres);
  printf("%s:%d [%s] lua_resume L=%p from=%p narg=%d nres=%p -> %d\n", _FILE,
         _LINE, _FUNC, L, from, narg, nres, rc);
  return rc;
}

int Lua_status(lua_State* L, const char* _FILE, int _LINE, const char* _FUNC) {
  int rc = (lua_status)(L);
  printf("%s:%d [%s] lua_status L=%p -> %d\n", _FILE, _LINE, _FUNC, L, rc);
  return rc;
}

int Lua_isyieldable(lua_State* L, const char* _FILE, int _LINE,
                    const char* _FUNC) {
  int rc = (lua_isyieldable)(L);
  printf("%s:%d [%s] lua_isyieldable L=%p -> %d\n", _FILE, _LINE, _FUNC, L, rc);
  return rc;
}

void Lua_setwarnf(lua_State* L, lua_WarnFunction f, void* ud, const char* _FILE,
                  int _LINE, const char* _FUNC) {
  (lua_setwarnf)(L, f, ud);
  printf("%s:%d [%s] lua_setwarnf L=%p f=%p ud=%p\n", _FILE, _LINE, _FUNC, L, f,
         ud);
}

void Lua_warning(lua_State* L, const char* msg, int tocont, const char* _FILE,
                 int _LINE, const char* _FUNC) {
  (lua_warning)(L, msg, tocont);
  printf("%s:%d [%s] lua_warning L=%p msg=\"%s\" tocont=%d\n", _FILE, _LINE,
         _FUNC, L, msg, tocont);
}

int Lua_error(lua_State* L, const char* _FILE, int _LINE, const char* _FUNC) {
  int rc = (lua_error)(L);
  printf("%s:%d [%s] lua_error L=%p -> %d\n", _FILE, _LINE, _FUNC, L, rc);
  return rc;
}

int Lua_next(lua_State* L, int idx, const char* _FILE, int _LINE,
             const char* _FUNC) {
  int rc = (lua_next)(L, idx);
  printf("%s:%d [%s] lua_next L=%p idx=%d -> %d\n", _FILE, _LINE, _FUNC, L, idx,
         rc);
  return rc;
}

void Lua_concat(lua_State* L, int n, const char* _FILE, int _LINE,
                const char* _FUNC) {
  (lua_concat)(L, n);
  printf("%s:%d [%s] lua_concat L=%p n=%d\n", _FILE, _LINE, _FUNC, L, n);
}

void Lua_len(lua_State* L, int idx, const char* _FILE, int _LINE,
             const char* _FUNC) {
  (lua_len)(L, idx);
  printf("%s:%d [%s] lua_len L=%p idx=%d\n", _FILE, _LINE, _FUNC, L, idx);
}

size_t Lua_stringtonumber(lua_State* L, const char* s, const char* _FILE,
                          int _LINE, const char* _FUNC) {
  size_t rc = (lua_stringtonumber)(L, s);
  printf("%s:%d [%s] lua_stringtonumber L=%p s=\"%s\" -> %u\n", _FILE, _LINE,
         _FUNC, L, s, rc);
  return rc;
}

lua_Alloc Lua_getallocf(lua_State* L, void** ud, const char* _FILE, int _LINE,
                        const char* _FUNC) {
  lua_Alloc rc = (lua_getallocf)(L, ud);
  printf("%s:%d [%s] lua_getallocf L=%p ud=%p -> %p\n", _FILE, _LINE, _FUNC, L,
         ud, rc);
  return rc;
}

void Lua_setallocf(lua_State* L, lua_Alloc f, void* ud, const char* _FILE,
                   int _LINE, const char* _FUNC) {
  (lua_setallocf)(L, f, ud);
  printf("%s:%d [%s] lua_setallocf L=%p f=%p ud=%p\n", _FILE, _LINE, _FUNC, L,
         f, ud);
}

void Lua_toclose(lua_State* L, int idx, const char* _FILE, int _LINE,
                 const char* _FUNC) {
  (lua_toclose)(L, idx);
  printf("%s:%d [%s] lua_toclose L=%p idx=%d\n", _FILE, _LINE, _FUNC, L, idx);
}

void Lua_closeslot(lua_State* L, int idx, const char* _FILE, int _LINE,
                   const char* _FUNC) {
  (lua_closeslot)(L, idx);
  printf("%s:%d [%s] lua_closeslot L=%p idx=%d\n", _FILE, _LINE, _FUNC, L, idx);
}

int Lua_getstack(lua_State* L, int level, lua_Debug* ar, const char* _FILE,
                 int _LINE, const char* _FUNC) {
  int rc = (lua_getstack)(L, level, ar);
  printf("%s:%d [%s] lua_getstack L=%p level=%d ar=%p -> %d\n", _FILE, _LINE,
         _FUNC, L, level, ar, rc);
  return rc;
}

int Lua_getinfo(lua_State* L, const char* what, lua_Debug* ar,
                const char* _FILE, int _LINE, const char* _FUNC) {
  int rc = (lua_getinfo)(L, what, ar);
  printf("%s:%d [%s] lua_getinfo L=%p what=\"%s\" ar=%p -> %d\n", _FILE, _LINE,
         _FUNC, L, what, ar, rc);
  return rc;
}

const char* Lua_getlocal(lua_State* L, const lua_Debug* ar, int n,
                         const char* _FILE, int _LINE, const char* _FUNC) {
  const char* rc = (lua_getlocal)(L, ar, n);
  printf("%s:%d [%s] lua_getlocal L=%p ar=%p n=%d -> \"%s\"\n", _FILE, _LINE,
         _FUNC, L, ar, n, rc);
  return rc;
}

const char* Lua_setlocal(lua_State* L, const lua_Debug* ar, int n,
                         const char* _FILE, int _LINE, const char* _FUNC) {
  const char* rc = (lua_setlocal)(L, ar, n);
  printf("%s:%d [%s] lua_setlocal L=%p ar=%p n=%d -> \"%s\"\n", _FILE, _LINE,
         _FUNC, L, ar, n, rc);
  return rc;
}

const char* Lua_getupvalue(lua_State* L, int funcindex, int n,
                           const char* _FILE, int _LINE, const char* _FUNC) {
  const char* rc = (lua_getupvalue)(L, funcindex, n);
  printf("%s:%d [%s] lua_getupvalue L=%p funcindex=%d n=%d -> \"%s\"\n", _FILE,
         _LINE, _FUNC, L, funcindex, n, rc);
  return rc;
}

const char* Lua_setupvalue(lua_State* L, int funcindex, int n,
                           const char* _FILE, int _LINE, const char* _FUNC) {
  const char* rc = (lua_setupvalue)(L, funcindex, n);
  printf("%s:%d [%s] lua_setupvalue L=%p funcindex=%d n=%d -> \"%s\"\n", _FILE,
         _LINE, _FUNC, L, funcindex, n, rc);
  return rc;
}

void* Lua_upvalueid(lua_State* L, int fidx, int n, const char* _FILE, int _LINE,
                    const char* _FUNC) {
  void* rc = (lua_upvalueid)(L, fidx, n);
  printf("%s:%d [%s] lua_upvalueid L=%p fidx=%d n=%d -> %p\n", _FILE, _LINE,
         _FUNC, L, fidx, n, rc);
  return rc;
}

void Lua_upvaluejoin(lua_State* L, int fidx1, int n1, int fidx2, int n2,
                     const char* _FILE, int _LINE, const char* _FUNC) {
  (lua_upvaluejoin)(L, fidx1, n1, fidx2, n2);
  printf("%s:%d [%s] lua_upvaluejoin L=%p fidx1=%d n1=%d fidx2=%d n2=%d\n",
         _FILE, _LINE, _FUNC, L, fidx1, n1, fidx2, n2);
}

void Lua_sethook(lua_State* L, lua_Hook func, int mask, int count,
                 const char* _FILE, int _LINE, const char* _FUNC) {
  (lua_sethook)(L, func, mask, count);
  printf("%s:%d [%s] lua_sethook L=%p func=%p mask=%d count=%d\n", _FILE, _LINE,
         _FUNC, L, func, mask, count);
}

lua_Hook Lua_gethook(lua_State* L, const char* _FILE, int _LINE,
                     const char* _FUNC) {
  lua_Hook rc = (lua_gethook)(L);
  printf("%s:%d [%s] lua_gethook L=%p -> %p\n", _FILE, _LINE, _FUNC, L, rc);
  return rc;
}

int Lua_gethookmask(lua_State* L, const char* _FILE, int _LINE,
                    const char* _FUNC) {
  int rc = (lua_gethookmask)(L);
  printf("%s:%d [%s] lua_gethookmask L=%p -> %d\n", _FILE, _LINE, _FUNC, L, rc);
  return rc;
}

int Lua_gethookcount(lua_State* L, const char* _FILE, int _LINE,
                     const char* _FUNC) {
  int rc = (lua_gethookcount)(L);
  printf("%s:%d [%s] lua_gethookcount L=%p -> %d\n", _FILE, _LINE, _FUNC, L,
         rc);
  return rc;
}

int Lua_setcstacklimit(lua_State* L, unsigned int limit, const char* _FILE,
                       int _LINE, const char* _FUNC) {
  int rc = (lua_setcstacklimit)(L, limit);
  printf("%s:%d [%s] lua_setcstacklimit L=%p limit=%u -> %d\n", _FILE, _LINE,
         _FUNC, L, limit, rc);
  return rc;
}
