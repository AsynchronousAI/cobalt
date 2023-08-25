/* ltrace.h -- automatically extracted from cobalt.h (5.4) */

#ifdef __LCC__
#define __FUNCTION__ __func__
#else
#ifndef __GNUC__
#define __FUNCTION__ "-"
#endif
#endif

#include "cobalt.h"
#include "lauxlib.h"

lua_State* Lua_newstate(lua_Alloc f, void* ud,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_newstate(f,ud) \
	Lua_newstate(f,ud,__FILE__,__LINE__,__FUNCTION__)

void Lua_close(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_close(L) \
	Lua_close(L,__FILE__,__LINE__,__FUNCTION__)

lua_State* Lua_newthread(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_newthread(L) \
	Lua_newthread(L,__FILE__,__LINE__,__FUNCTION__)

int Lua_resetthread(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_resetthread(L) \
	Lua_resetthread(L,__FILE__,__LINE__,__FUNCTION__)

lua_CFunction Lua_atpanic(lua_State* L, lua_CFunction panicf,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_atpanic(L,panicf) \
	Lua_atpanic(L,panicf,__FILE__,__LINE__,__FUNCTION__)

lua_Number Lua_version(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_version(L) \
	Lua_version(L,__FILE__,__LINE__,__FUNCTION__)

int Lua_absindex(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_absindex(L,idx) \
	Lua_absindex(L,idx,__FILE__,__LINE__,__FUNCTION__)

int Lua_gettop(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_gettop(L) \
	Lua_gettop(L,__FILE__,__LINE__,__FUNCTION__)

void Lua_settop(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_settop(L,idx) \
	Lua_settop(L,idx,__FILE__,__LINE__,__FUNCTION__)

void Lua_pushvalue(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_pushvalue(L,idx) \
	Lua_pushvalue(L,idx,__FILE__,__LINE__,__FUNCTION__)

void Lua_rotate(lua_State* L, int idx, int n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_rotate(L,idx,n) \
	Lua_rotate(L,idx,n,__FILE__,__LINE__,__FUNCTION__)

void Lua_copy(lua_State* L, int fromidx, int toidx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_copy(L,fromidx,toidx) \
	Lua_copy(L,fromidx,toidx,__FILE__,__LINE__,__FUNCTION__)

int Lua_checkstack(lua_State* L, int n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_checkstack(L,n) \
	Lua_checkstack(L,n,__FILE__,__LINE__,__FUNCTION__)

void Lua_xmove(lua_State* from, lua_State* to, int n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_xmove(from,to,n) \
	Lua_xmove(from,to,n,__FILE__,__LINE__,__FUNCTION__)

int Lua_isnumber(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_isnumber(L,idx) \
	Lua_isnumber(L,idx,__FILE__,__LINE__,__FUNCTION__)

int Lua_isstring(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_isstring(L,idx) \
	Lua_isstring(L,idx,__FILE__,__LINE__,__FUNCTION__)

int Lua_iscfunction(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_iscfunction(L,idx) \
	Lua_iscfunction(L,idx,__FILE__,__LINE__,__FUNCTION__)

int Lua_isinteger(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_isinteger(L,idx) \
	Lua_isinteger(L,idx,__FILE__,__LINE__,__FUNCTION__)

int Lua_isuserdata(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_isuserdata(L,idx) \
	Lua_isuserdata(L,idx,__FILE__,__LINE__,__FUNCTION__)

int Lua_type(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_type(L,idx) \
	Lua_type(L,idx,__FILE__,__LINE__,__FUNCTION__)

const char* Lua_typename(lua_State* L, int tp,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_typename(L,tp) \
	Lua_typename(L,tp,__FILE__,__LINE__,__FUNCTION__)

lua_Number Lua_tonumberx(lua_State* L, int idx, int* isnum,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_tonumberx(L,idx,isnum) \
	Lua_tonumberx(L,idx,isnum,__FILE__,__LINE__,__FUNCTION__)

lua_Integer Lua_tointegerx(lua_State* L, int idx, int* isnum,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_tointegerx(L,idx,isnum) \
	Lua_tointegerx(L,idx,isnum,__FILE__,__LINE__,__FUNCTION__)

int Lua_toboolean(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_toboolean(L,idx) \
	Lua_toboolean(L,idx,__FILE__,__LINE__,__FUNCTION__)

const char* Lua_tolstring(lua_State* L, int idx, size_t* len,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_tolstring(L,idx,len) \
	Lua_tolstring(L,idx,len,__FILE__,__LINE__,__FUNCTION__)

lua_Unsigned Lua_rawlen(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_rawlen(L,idx) \
	Lua_rawlen(L,idx,__FILE__,__LINE__,__FUNCTION__)

lua_CFunction Lua_tocfunction(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_tocfunction(L,idx) \
	Lua_tocfunction(L,idx,__FILE__,__LINE__,__FUNCTION__)

void* Lua_touserdata(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_touserdata(L,idx) \
	Lua_touserdata(L,idx,__FILE__,__LINE__,__FUNCTION__)

lua_State* Lua_tothread(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_tothread(L,idx) \
	Lua_tothread(L,idx,__FILE__,__LINE__,__FUNCTION__)

const void* Lua_topointer(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_topointer(L,idx) \
	Lua_topointer(L,idx,__FILE__,__LINE__,__FUNCTION__)

void Lua_arith(lua_State* L, int op,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_arith(L,op) \
	Lua_arith(L,op,__FILE__,__LINE__,__FUNCTION__)

int Lua_rawequal(lua_State* L, int idx1, int idx2,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_rawequal(L,idx1,idx2) \
	Lua_rawequal(L,idx1,idx2,__FILE__,__LINE__,__FUNCTION__)

int Lua_compare(lua_State* L, int idx1, int idx2, int op,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_compare(L,idx1,idx2,op) \
	Lua_compare(L,idx1,idx2,op,__FILE__,__LINE__,__FUNCTION__)

void Lua_pushnil(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_pushnil(L) \
	Lua_pushnil(L,__FILE__,__LINE__,__FUNCTION__)

void Lua_pushnumber(lua_State* L, lua_Number n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_pushnumber(L,n) \
	Lua_pushnumber(L,n,__FILE__,__LINE__,__FUNCTION__)

void Lua_pushinteger(lua_State* L, lua_Integer n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_pushinteger(L,n) \
	Lua_pushinteger(L,n,__FILE__,__LINE__,__FUNCTION__)

const char* Lua_pushlstring(lua_State* L, const char* s, size_t len,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_pushlstring(L,s,len) \
	Lua_pushlstring(L,s,len,__FILE__,__LINE__,__FUNCTION__)

const char* Lua_pushstring(lua_State* L, const char* s,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_pushstring(L,s) \
	Lua_pushstring(L,s,__FILE__,__LINE__,__FUNCTION__)

void Lua_pushcclosure(lua_State* L, lua_CFunction fn, int n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_pushcclosure(L,fn,n) \
	Lua_pushcclosure(L,fn,n,__FILE__,__LINE__,__FUNCTION__)

void Lua_pushboolean(lua_State* L, int b,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_pushboolean(L,b) \
	Lua_pushboolean(L,b,__FILE__,__LINE__,__FUNCTION__)

void Lua_pushlightuserdata(lua_State* L, void* p,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_pushlightuserdata(L,p) \
	Lua_pushlightuserdata(L,p,__FILE__,__LINE__,__FUNCTION__)

int Lua_pushthread(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_pushthread(L) \
	Lua_pushthread(L,__FILE__,__LINE__,__FUNCTION__)

int Lua_getglobal(lua_State* L, const char* name,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_getglobal(L,name) \
	Lua_getglobal(L,name,__FILE__,__LINE__,__FUNCTION__)

int Lua_gettable(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_gettable(L,idx) \
	Lua_gettable(L,idx,__FILE__,__LINE__,__FUNCTION__)

int Lua_getfield(lua_State* L, int idx, const char* k,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_getfield(L,idx,k) \
	Lua_getfield(L,idx,k,__FILE__,__LINE__,__FUNCTION__)

int Lua_geti(lua_State* L, int idx, lua_Integer n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_geti(L,idx,n) \
	Lua_geti(L,idx,n,__FILE__,__LINE__,__FUNCTION__)

int Lua_rawget(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_rawget(L,idx) \
	Lua_rawget(L,idx,__FILE__,__LINE__,__FUNCTION__)

int Lua_rawgeti(lua_State* L, int idx, lua_Integer n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_rawgeti(L,idx,n) \
	Lua_rawgeti(L,idx,n,__FILE__,__LINE__,__FUNCTION__)

int Lua_rawgetp(lua_State* L, int idx, const void* p,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_rawgetp(L,idx,p) \
	Lua_rawgetp(L,idx,p,__FILE__,__LINE__,__FUNCTION__)

void Lua_createtable(lua_State* L, int narr, int nrec,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_createtable(L,narr,nrec) \
	Lua_createtable(L,narr,nrec,__FILE__,__LINE__,__FUNCTION__)

void* Lua_newuserdatauv(lua_State* L, size_t sz, int nuvalue,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_newuserdatauv(L,sz,nuvalue) \
	Lua_newuserdatauv(L,sz,nuvalue,__FILE__,__LINE__,__FUNCTION__)

int Lua_getmetatable(lua_State* L, int objindex,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_getmetatable(L,objindex) \
	Lua_getmetatable(L,objindex,__FILE__,__LINE__,__FUNCTION__)

int Lua_getiuservalue(lua_State* L, int idx, int n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_getiuservalue(L,idx,n) \
	Lua_getiuservalue(L,idx,n,__FILE__,__LINE__,__FUNCTION__)

void Lua_setglobal(lua_State* L, const char* name,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_setglobal(L,name) \
	Lua_setglobal(L,name,__FILE__,__LINE__,__FUNCTION__)

void Lua_settable(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_settable(L,idx) \
	Lua_settable(L,idx,__FILE__,__LINE__,__FUNCTION__)

void Lua_setfield(lua_State* L, int idx, const char* k,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_setfield(L,idx,k) \
	Lua_setfield(L,idx,k,__FILE__,__LINE__,__FUNCTION__)

void Lua_seti(lua_State* L, int idx, lua_Integer n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_seti(L,idx,n) \
	Lua_seti(L,idx,n,__FILE__,__LINE__,__FUNCTION__)

void Lua_rawset(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_rawset(L,idx) \
	Lua_rawset(L,idx,__FILE__,__LINE__,__FUNCTION__)

void Lua_rawseti(lua_State* L, int idx, lua_Integer n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_rawseti(L,idx,n) \
	Lua_rawseti(L,idx,n,__FILE__,__LINE__,__FUNCTION__)

void Lua_rawsetp(lua_State* L, int idx, const void* p,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_rawsetp(L,idx,p) \
	Lua_rawsetp(L,idx,p,__FILE__,__LINE__,__FUNCTION__)

int Lua_setmetatable(lua_State* L, int objindex,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_setmetatable(L,objindex) \
	Lua_setmetatable(L,objindex,__FILE__,__LINE__,__FUNCTION__)

int Lua_setiuservalue(lua_State* L, int idx, int n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_setiuservalue(L,idx,n) \
	Lua_setiuservalue(L,idx,n,__FILE__,__LINE__,__FUNCTION__)

void Lua_callk(lua_State* L, int nargs, int nresults,
 lua_KContext ctx, lua_KFunction k,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_callk(L,nargs,nresults,ctx,k) \
	Lua_callk(L,nargs,nresults,ctx,k,__FILE__,__LINE__,__FUNCTION__)

int Lua_pcallk(lua_State* L, int nargs, int nresults, int errfunc,
 lua_KContext ctx, lua_KFunction k,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_pcallk(L,nargs,nresults,errfunc,ctx,k) \
	Lua_pcallk(L,nargs,nresults,errfunc,ctx,k,__FILE__,__LINE__,__FUNCTION__)

int Lua_load(lua_State* L, lua_Reader reader, void* dt,
 const char* chunkname, const char* mode,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_load(L,reader,dt,chunkname,mode) \
	Lua_load(L,reader,dt,chunkname,mode,__FILE__,__LINE__,__FUNCTION__)

int Lua_dump(lua_State* L, lua_Writer writer, void* data, int strip,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_dump(L,writer,data,strip) \
	Lua_dump(L,writer,data,strip,__FILE__,__LINE__,__FUNCTION__)

int Lua_yieldk(lua_State* L, int nresults, lua_KContext ctx,
 lua_KFunction k,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_yieldk(L,nresults,ctx,k) \
	Lua_yieldk(L,nresults,ctx,k,__FILE__,__LINE__,__FUNCTION__)

int Lua_resume(lua_State* L, lua_State* from, int narg,
 int* nres,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_resume(L,from,narg,nres) \
	Lua_resume(L,from,narg,nres,__FILE__,__LINE__,__FUNCTION__)

int Lua_status(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_status(L) \
	Lua_status(L,__FILE__,__LINE__,__FUNCTION__)

int Lua_isyieldable(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_isyieldable(L) \
	Lua_isyieldable(L,__FILE__,__LINE__,__FUNCTION__)

void Lua_setwarnf(lua_State* L, lua_WarnFunction f, void* ud,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_setwarnf(L,f,ud) \
	Lua_setwarnf(L,f,ud,__FILE__,__LINE__,__FUNCTION__)

void Lua_warning(lua_State* L, const char* msg, int tocont,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_warning(L,msg,tocont) \
	Lua_warning(L,msg,tocont,__FILE__,__LINE__,__FUNCTION__)

int Lua_error(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_error(L) \
	Lua_error(L,__FILE__,__LINE__,__FUNCTION__)

int Lua_next(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_next(L,idx) \
	Lua_next(L,idx,__FILE__,__LINE__,__FUNCTION__)

void Lua_concat(lua_State* L, int n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_concat(L,n) \
	Lua_concat(L,n,__FILE__,__LINE__,__FUNCTION__)

void Lua_len(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_len(L,idx) \
	Lua_len(L,idx,__FILE__,__LINE__,__FUNCTION__)

size_t Lua_stringtonumber(lua_State* L, const char* s,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_stringtonumber(L,s) \
	Lua_stringtonumber(L,s,__FILE__,__LINE__,__FUNCTION__)

lua_Alloc Lua_getallocf(lua_State* L, void* *ud,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_getallocf(L,ud) \
	Lua_getallocf(L,ud,__FILE__,__LINE__,__FUNCTION__)

void Lua_setallocf(lua_State* L, lua_Alloc f, void* ud,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_setallocf(L,f,ud) \
	Lua_setallocf(L,f,ud,__FILE__,__LINE__,__FUNCTION__)

void Lua_toclose(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_toclose(L,idx) \
	Lua_toclose(L,idx,__FILE__,__LINE__,__FUNCTION__)

void Lua_closeslot(lua_State* L, int idx,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_closeslot(L,idx) \
	Lua_closeslot(L,idx,__FILE__,__LINE__,__FUNCTION__)

int Lua_getstack(lua_State* L, int level, lua_Debug* ar,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_getstack(L,level,ar) \
	Lua_getstack(L,level,ar,__FILE__,__LINE__,__FUNCTION__)

int Lua_getinfo(lua_State* L, const char* what, lua_Debug* ar,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_getinfo(L,what,ar) \
	Lua_getinfo(L,what,ar,__FILE__,__LINE__,__FUNCTION__)

const char* Lua_getlocal(lua_State* L, const lua_Debug* ar, int n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_getlocal(L,ar,n) \
	Lua_getlocal(L,ar,n,__FILE__,__LINE__,__FUNCTION__)

const char* Lua_setlocal(lua_State* L, const lua_Debug* ar, int n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_setlocal(L,ar,n) \
	Lua_setlocal(L,ar,n,__FILE__,__LINE__,__FUNCTION__)

const char* Lua_getupvalue(lua_State* L, int funcindex, int n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_getupvalue(L,funcindex,n) \
	Lua_getupvalue(L,funcindex,n,__FILE__,__LINE__,__FUNCTION__)

const char* Lua_setupvalue(lua_State* L, int funcindex, int n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_setupvalue(L,funcindex,n) \
	Lua_setupvalue(L,funcindex,n,__FILE__,__LINE__,__FUNCTION__)

void* Lua_upvalueid(lua_State* L, int fidx, int n,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_upvalueid(L,fidx,n) \
	Lua_upvalueid(L,fidx,n,__FILE__,__LINE__,__FUNCTION__)

void Lua_upvaluejoin(lua_State* L, int fidx1, int n1,
 int fidx2, int n2,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_upvaluejoin(L,fidx1,n1,fidx2,n2) \
	Lua_upvaluejoin(L,fidx1,n1,fidx2,n2,__FILE__,__LINE__,__FUNCTION__)

void Lua_sethook(lua_State* L, lua_Hook func, int mask, int count,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_sethook(L,func,mask,count) \
	Lua_sethook(L,func,mask,count,__FILE__,__LINE__,__FUNCTION__)

lua_Hook Lua_gethook(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_gethook(L) \
	Lua_gethook(L,__FILE__,__LINE__,__FUNCTION__)

int Lua_gethookmask(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_gethookmask(L) \
	Lua_gethookmask(L,__FILE__,__LINE__,__FUNCTION__)

int Lua_gethookcount(lua_State* L,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_gethookcount(L) \
	Lua_gethookcount(L,__FILE__,__LINE__,__FUNCTION__)

int Lua_setcstacklimit(lua_State* L, unsigned int limit,
	const char* _FILE, int _LINE, const char* _FUNC);

#define	lua_setcstacklimit(L,limit) \
	Lua_setcstacklimit(L,limit,__FILE__,__LINE__,__FUNCTION__)
