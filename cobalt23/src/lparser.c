/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#define lparser_c
#define LUA_CORE

#include "lparser.h"

#include <limits.h>
#include <string.h>

#include "cobalt.h"
#include "lcode.h"
#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "llex.h"
#include "lmem.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lprefix.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"


/* maximum number of local variables per function (must be smaller
   than 250, due to the bytecode format) */
#define MAXVARS 250

#define hasmultret(k) ((k) == VCALL || (k) == VVARARG)

/* because all strings are unified by the scanner, the parser
   can use pointer equality for string equality */
#define eqstr(a, b) ((a) == (b))

/*
** nodes for block list (list of active blocks)
*/
typedef struct BlockCnt {
  struct BlockCnt *previous; /* chain */
  int firstlabel;            /* index of first label in this block */
  int firstgoto;             /* index of first pending goto in this block */
  lu_byte nactvar;           /* # active locals outside the block */
  lu_byte upval;     /* true if some variable in the block is an upvalue */
  lu_byte isloop;    /* true if 'block' is a loop */
  lu_byte insidetbc; /* true if inside the scope of a to-be-closed var. */
} BlockCnt;

#define FSCOPE_LOOP 0x01        /* Scope is a (breakable) loop. */
#define FSCOPE_FORINLOOP 0x02   /* Scope is a (breakable) for in loop. */
#define FSCOPE_DOWHILELOOP 0x04 /* Scope is a (breakable) do while in loop. */
#define LOOP_HAS_BREAK 0x08
#define LOOP_HAS_CONTINUE 0x10

#define LABEL_BREAK "break"
#define LABEL_CONTINUE "continue"

/*
** prototypes for recursive non-terminal functions
*/
static void statement(LexState *ls);
static void expr(LexState *ls, expdesc *v);

static l_noret error_expected(LexState *ls, int token) {
  luaX_syntaxerror(
      ls, luaO_pushfstring(ls->L, "%s expected", luaX_token2str(ls, token)));
}

static l_noret errorlimit(FuncState *fs, int limit, const char *what) {
  lua_State *L = fs->ls->L;
  const char *msg;
  int line = fs->f->linedefined;
  const char *where = (line == 0)
                          ? "main function"
                          : luaO_pushfstring(L, "function at line %d", line);
  msg = luaO_pushfstring(L, "too many %s (limit is %d) in %s", what, limit,
                         where);
  luaX_syntaxerror(fs->ls, msg);
}

static void checklimit(FuncState *fs, int v, int l, const char *what) {
  if (v > l) errorlimit(fs, l, what);
}

/*
** Test whether next token is 'c'; if so, skip it.
*/
static int testnext(LexState *ls, int c) {
  if (ls->t.token == c) {
    luaX_next(ls);
    return 1;
  } else
    return 0;
}

/*
** Check that next token is 'c'.
*/
static void check(LexState *ls, int c) {
  if (ls->t.token != c) error_expected(ls, c);
}

/*
** Check that next token is 'c' and skip it.
*/
static void checknext(LexState *ls, int c) {
  check(ls, c);
  luaX_next(ls);
}

#define check_condition(ls, c, msg)      \
  {                                      \
    if (!(c)) luaX_syntaxerror(ls, msg); \
  }

/*
** Check that next token is 'what' and skip it. In case of error,
** raise an error that the expected 'what' should match a 'who'
** in line 'where' (if that is not the current line).
*/
static void check_match(LexState *ls, int what, int who, int where) {
  if (l_unlikely(!testnext(ls, what))) {
    if (where == ls->linenumber) /* all in the same line? */
      error_expected(ls, what);  /* do not need a complex message */
    else {
      luaX_syntaxerror(
          ls, luaO_pushfstring(ls->L, "%s expected (to close %s at line %d)",
                               luaX_token2str(ls, what),
                               luaX_token2str(ls, who), where));
    }
  }
}

static TString *str_checkname(LexState *ls) {
  TString *ts;
  check(ls, TK_NAME);
  ts = ls->t.seminfo.ts;
  luaX_next(ls);
  return ts;
}

static void init_exp(expdesc *e, expkind k, int i) {
  e->f = e->t = NO_JUMP;
  e->k = k;
  e->u.info = i;
}

static void codestring(expdesc *e, TString *s) {
  e->f = e->t = NO_JUMP;
  e->k = VKSTR;
  e->u.strval = s;
}

static void codename(LexState *ls, expdesc *e) {
  codestring(e, str_checkname(ls));
}

/*
** Register a new local variable in the active 'Proto' (for debug
** information).
*/
static int registerlocalvar(LexState *ls, FuncState *fs, TString *varname) {
  Proto *f = fs->f;
  int oldsize = f->sizelocvars;
  luaM_growvector(ls->L, f->locvars, fs->ndebugvars, f->sizelocvars, LocVar,
                  SHRT_MAX, "local variables");
  while (oldsize < f->sizelocvars) f->locvars[oldsize++].varname = NULL;
  f->locvars[fs->ndebugvars].varname = varname;
  f->locvars[fs->ndebugvars].startpc = fs->pc;
  luaC_objbarrier(ls->L, f, varname);
  return fs->ndebugvars++;
}
#if 0
static void var_check_unique_or_shadow0 (LexState *ls, FuncState *fs, TString *name, int shadowOnly) {
  Dyndata *dyd = ls->dyd;

  /* allow '_' and '(for...' duplicates */
  const char *str_name = getstr(name);
  if(!(str_name[0] == '(' || (tsslen(name) == 1 && str_name[0] == '_'))) {
    int vidx, nactvar_n, first_block_local;
    vidx = fs->firstlocal;
    nactvar_n = dyd->actvar.n;
    //nactvar_n = fs->nactvar;
    first_block_local = fs->bl ? fs->bl->nactvar+fs->firstlocal : 0;
    for (; vidx < nactvar_n; ++vidx) {
      Vardesc *vd = &dyd->actvar.arr[fs->firstlocal + vidx];
      if (vd && name == vd->vd.name) {
      //LocVar *lv = &fs->f->locvars[dyd->actvar.arr[vidx].vd.ridx];
      //if (lv && name == lv->varname) {
        if(vidx <= first_block_local) {
          int saved_top = lua_gettop(ls->L);
          //lua_warning(ls->L, luaO_pushfstring(ls->L,
          //       "Name [%s] already declared will be shadowed", str_name), 0);
          luaX_syntaxwarning(ls, luaO_pushfstring(ls->L,
                 "Name [%s] already declared will be shadowed", str_name));
          lua_settop(ls->L, saved_top);
        }
        else if(!shadowOnly) {
          luaX_syntaxerror(ls, luaO_pushfstring(ls->L,
                 "Name [%s] already declared", str_name));
        }
      }
    }
  }
  //if(fs->prev) var_check_unique_or_shadow(ls, fs->prev, name, 1);
}
#endif
static void var_check_unique_or_shadow(LexState *ls, FuncState *fs,
                                       TString *name, int shadowOnly) {
  Dyndata *dyd = ls->dyd;

  /* allow '_' and '(for...' duplicates */
  const char *str_name = getstr(name);
  if (!(str_name[0] == '(' || (tsslen(name) == 1 && str_name[0] == '_'))) {
    int vidx, nactvar_n, first_block_local;
    vidx = 0;
    nactvar_n = dyd->actvar.n;
    first_block_local =
        fs->bl ? fs->bl->nactvar + fs->firstlocal : fs->firstlocal;
    for (; vidx < nactvar_n; ++vidx) {
      Vardesc *vd = dyd->actvar.arr + vidx;
      if (vd && name == vd->vd.name) {
        if (vidx <= first_block_local) {
          /*
          int saved_top = lua_gettop(ls->L);
          luaX_syntaxwarning(
              ls, luaO_pushfstring(
                      ls->L, "Name [%s] already declared will be shadowed",
                      str_name));
          lua_settop(ls->L, saved_top);
          */ 
          /* disabled warnings (they are annoying and uneeded)*/
        } else if (!shadowOnly) {
          luaX_syntaxerror(
              ls,
              luaO_pushfstring(ls->L, "Name [%s] already declared", str_name));
        }
      }
    }
  }
}

/*
** Create a new local variable with the given 'name'. Return its index
** in the function.
*/
static int new_localvar(LexState *ls, TString *name) {
  lua_State *L = ls->L;
  FuncState *fs = ls->fs;
  Dyndata *dyd = ls->dyd;

  // var_check_unique_or_shadow0(fs, name, 1);
  var_check_unique_or_shadow(ls, fs, name, 0);
#if 0
  int vidx, nactvar_n;
  vidx = fs->bl ? fs->bl->nactvar + fs->firstlocal : fs->firstlocal;
  //nactvar_n = dyd->actvar.n;
  nactvar_n = fs->nactvar;
  if(nactvar_n > fs->ndebugvars) nactvar_n = fs->ndebugvars;
  for (; vidx < nactvar_n; ++vidx) {
      Vardesc *vd = dyd->actvar.arr+vidx;
    LocVar *lv = &fs->f->locvars[vd->vd.ridx];
    if (lv && name == lv->varname) {
      /* allow '_' duplicates */
      const char *sname = getstr(name);
      //if(sname[0] == '(') break; //for loop variables
      if(tsslen(name) == 1 && sname[0] == '_') break;
      luaX_syntaxerror(ls, luaO_pushfstring(ls->L,
             "Name [%s] already declared", sname));
    }
  }
#endif
  Vardesc *var;
  checklimit(fs, dyd->actvar.n + 1 - fs->firstlocal, MAXVARS,
             "local variables");
  luaM_growvector(L, dyd->actvar.arr, dyd->actvar.n + 1, dyd->actvar.size,
                  Vardesc, USHRT_MAX, "local variables");
  var = &dyd->actvar.arr[dyd->actvar.n++];
  var->vd.kind = VDKREG; /* default */
  var->vd.name = name;
  return dyd->actvar.n - 1 - fs->firstlocal;
}

#define new_localvarliteral(ls, v) \
  new_localvar(ls, luaX_newstring(ls, "" v, (sizeof(v) / sizeof(char)) - 1));

/*
** Return the "variable description" (Vardesc) of a given variable.
** (Unless noted otherwise, all variables are referred to by their
** compiler indices.)
*/
static Vardesc *getlocalvardesc(FuncState *fs, int vidx) {
  return &fs->ls->dyd->actvar.arr[fs->firstlocal + vidx];
}

/*
** Convert 'nvar', a compiler index level, to its corresponding
** register. For that, search for the highest variable below that level
** that is in a register and uses its register index ('ridx') plus one.
*/
static int reglevel(FuncState *fs, int nvar) {
  while (nvar-- > 0) {
    Vardesc *vd = getlocalvardesc(fs, nvar); /* get previous variable */
    if (vd->vd.kind != RDKCTC)               /* is in a register? */
      return vd->vd.ridx + 1;
  }
  return 0; /* no variables in registers */
}

/*
** Return the number of variables in the register stack for the given
** function.
*/
int luaY_nvarstack(FuncState *fs) { return reglevel(fs, fs->nactvar); }

/*
** Get the debug-information entry for current variable 'vidx'.
*/
static LocVar *localdebuginfo(FuncState *fs, int vidx) {
  Vardesc *vd = getlocalvardesc(fs, vidx);
  if (vd->vd.kind == RDKCTC)
    return NULL; /* no debug info. for constants */
  else {
    int idx = vd->vd.pidx;
    lua_assert(idx < fs->ndebugvars);
    return &fs->f->locvars[idx];
  }
}

/*
** Create an expression representing variable 'vidx'
*/
static void init_var(FuncState *fs, expdesc *e, int vidx) {
  e->f = e->t = NO_JUMP;
  e->k = VLOCAL;
  e->u.var.vidx = vidx;
  e->u.var.ridx = getlocalvardesc(fs, vidx)->vd.ridx;
}

/*
** Raises an error if variable described by 'e' is read only
*/
static void check_readonly(LexState *ls, expdesc *e) {
  FuncState *fs = ls->fs;
  TString *varname = NULL; /* to be set if variable is const */
  switch (e->k) {
    case VCONST: {
      varname = ls->dyd->actvar.arr[e->u.info].vd.name;
      break;
    }
    case VLOCAL: {
      Vardesc *vardesc = getlocalvardesc(fs, e->u.var.vidx);
      if (vardesc->vd.kind != VDKREG) /* not a regular variable? */
        varname = vardesc->vd.name;
      break;
    }
    case VUPVAL: {
      Upvaldesc *up = &fs->f->upvalues[e->u.info];
      if (up->kind != VDKREG) varname = up->name;
      break;
    }
    default:
      return; /* other cases cannot be read-only */
  }
  if (varname) {
    const char *msg = luaO_pushfstring(
        ls->L, "attempt to assign to const variable '%s'", getstr(varname));
    luaK_semerror(ls, msg); /* error */
  }
}

/*
** Start the scope for the last 'nvars' created variables.
*/
static void adjustlocalvars(LexState *ls, int nvars) {
  FuncState *fs = ls->fs;
  int reglevel = luaY_nvarstack(fs);
  int i;
  for (i = 0; i < nvars; i++) {
    int vidx = fs->nactvar++;
    Vardesc *var = getlocalvardesc(fs, vidx);
    var->vd.ridx = reglevel++;
    var->vd.pidx = registerlocalvar(ls, fs, var->vd.name);
  }
}

/*
** Close the scope for all variables up to level 'tolevel'.
** (debug info.)
*/
static void removevars(FuncState *fs, int tolevel) {
  fs->ls->dyd->actvar.n -= (fs->nactvar - tolevel);
  while (fs->nactvar > tolevel) {
    LocVar *var = localdebuginfo(fs, --fs->nactvar);
    if (var) /* does it have debug information? */
      var->endpc = fs->pc;
  }
}

/*
** Search the upvalues of the function 'fs' for one
** with the given 'name'.
*/
static int searchupvalue(FuncState *fs, TString *name) {
  int i;
  Upvaldesc *up = fs->f->upvalues;
  for (i = 0; i < fs->nups; i++) {
    if (eqstr(up[i].name, name)) return i;
  }
  return -1; /* not found */
}

static Upvaldesc *allocupvalue(FuncState *fs) {
  Proto *f = fs->f;
  int oldsize = f->sizeupvalues;
  checklimit(fs, fs->nups + 1, MAXUPVAL, "upvalues");
  luaM_growvector(fs->ls->L, f->upvalues, fs->nups, f->sizeupvalues, Upvaldesc,
                  MAXUPVAL, "upvalues");
  while (oldsize < f->sizeupvalues) f->upvalues[oldsize++].name = NULL;
  return &f->upvalues[fs->nups++];
}

static int newupvalue(FuncState *fs, TString *name, expdesc *v) {
  Upvaldesc *up = allocupvalue(fs);
  FuncState *prev = fs->prev;
  if (v->k == VLOCAL) {
    up->instack = 1;
    up->idx = v->u.var.ridx;
    up->kind = getlocalvardesc(prev, v->u.var.vidx)->vd.kind;
    lua_assert(eqstr(name, getlocalvardesc(prev, v->u.var.vidx)->vd.name));
  } else {
    up->instack = 0;
    up->idx = cast_byte(v->u.info);
    up->kind = prev->f->upvalues[v->u.info].kind;
    lua_assert(eqstr(name, prev->f->upvalues[v->u.info].name));
  }
  up->name = name;
  luaC_objbarrier(fs->ls->L, fs->f, name);
  return fs->nups - 1;
}

/*
** Look for an active local variable with the name 'n' in the
** function 'fs'. If found, initialize 'var' with it and return
** its expression kind; otherwise return -1.
*/
static int searchvar(FuncState *fs, TString *n, expdesc *var) {
  int i;
  for (i = cast_int(fs->nactvar) - 1; i >= 0; i--) {
    Vardesc *vd = getlocalvardesc(fs, i);
    if (eqstr(n, vd->vd.name)) { /* found? */
      if (vd->vd.kind == RDKCTC) /* compile-time constant? */
        init_exp(var, VCONST, fs->firstlocal + i);
      else /* real variable */
        init_var(fs, var, i);
      return var->k;
    }
  }
  return -1; /* not found */
}

/*
** Mark block where variable at given level was defined
** (to emit close instructions later).
*/
static void markupval(FuncState *fs, int level) {
  BlockCnt *bl = fs->bl;
  while (bl->nactvar > level) bl = bl->previous;
  bl->upval = 1;
  fs->needclose = 1;
}

/*
** Mark that current block has a to-be-closed variable.
*/
static void marktobeclosed(FuncState *fs) {
  BlockCnt *bl = fs->bl;
  bl->upval = 1;
  bl->insidetbc = 1;
  fs->needclose = 1;
}

/*
** Find a variable with the given name 'n'. If it is an upvalue, add
** this upvalue into all intermediate functions. If it is a global, set
** 'var' as 'void' as a flag.
*/
static void singlevaraux(FuncState *fs, TString *n, expdesc *var, int base) {
  if (fs == NULL)            /* no more levels? */
    init_exp(var, VVOID, 0); /* default is global */
  else {
    int v = searchvar(fs, n, var); /* look up locals at current level */
    if (v >= 0) {                  /* found? */
      if (v == VLOCAL && !base)
        markupval(fs, var->u.var.vidx); /* local will be used as an upval */
    } else { /* not found as local at current level; try upvalues */
      int idx = searchupvalue(fs, n);             /* try existing upvalues */
      if (idx < 0) {                              /* not found? */
        singlevaraux(fs->prev, n, var, 0);        /* try upper levels */
        if (var->k == VLOCAL || var->k == VUPVAL) /* local or upvalue? */
          idx = newupvalue(fs, n, var);           /* will be a new upvalue */
        else      /* it is a global or a constant */
          return; /* don't need to do anything at this level */
      }
      init_exp(var, VUPVAL, idx); /* new or old upvalue */
    }
  }
}
/*
** Adjust the number of results from an expression list 'e' with 'nexps'
** expressions to 'nvars' values.
*/
static void adjust_assign (LexState *ls, int nvars, int nexps, expdesc *e) {
  FuncState *fs = ls->fs;
  int needed = nvars - nexps;  /* extra values needed */
  if (hasmultret(e->k)) {  /* last expression has multiple returns? */
    int extra = needed + 1;  /* discount last expression itself */
    if (extra < 0)
      extra = 0;
    luaK_setreturns(fs, e, extra);  /* last exp. provides the difference */
  }
  else {
    if (e->k != VVOID)  /* at least one expression? */
      luaK_exp2nextreg(fs, e);  /* close last expression */
    if (needed > 0)  /* missing values? */
      luaK_nil(fs, fs->freereg, needed);  /* complete with nils */
  }
  if (needed > 0)
    luaK_reserveregs(fs, needed);  /* registers for extra values */
  else  /* adding 'needed' is actually a subtraction */
    fs->freereg += needed;  /* remove extra values */
}

/*
** Find a variable with the given name 'n', handling global variables
** too.
*/
static void singlevarinner(LexState *ls, expdesc *var, TString *varname) {
  FuncState *fs = ls->fs;
  singlevaraux(fs, varname, var, 1);
  if (var->k == VVOID) { /* global name? */
    expdesc key;
    singlevaraux(fs, ls->envn, var, 1); /* get environment variable */
    lua_assert(var->k != VVOID);        /* this one must exist */
    codestring(&key, varname);          /* key is variable name */
    luaK_indexed(fs, var, &key);        /* env[varname] */
  }
}

static void singlevar (LexState *ls, expdesc *var) {
  TString *varname = str_checkname(ls);
  if (ls->t.token == TK_WALRUS) {
    luaX_next(ls);
    new_localvar(ls, varname);
    expr(ls, var);
    adjust_assign(ls, 1, 1, var);
    adjustlocalvars(ls, 1);
    return;
  }
  singlevarinner(ls, var, varname);
}


#define enterlevel(ls) luaE_incCstack(ls->L)

#define leavelevel(ls) ((ls)->L->nCcalls--)

/*
** Generates an error that a goto jumps into the scope of some
** local variable.
*/
static l_noret jumpscopeerror(LexState *ls, Labeldesc *gt) {
  const char *varname = getstr(getlocalvardesc(ls->fs, gt->nactvar)->vd.name);
  const char *msg = "<goto %s> at line %d jumps into the scope of local '%s'";
  msg = luaO_pushfstring(ls->L, msg, getstr(gt->name), gt->line, varname);
  luaK_semerror(ls, msg); /* raise the error */
}

/*
** Solves the goto at index 'g' to given 'label' and removes it
** from the list of pending goto's.
** If it jumps into the scope of some variable, raises an error.
*/
static void solvegoto(LexState *ls, int g, Labeldesc *label) {
  int i;
  Labellist *gl = &ls->dyd->gt; /* list of goto's */
  Labeldesc *gt = &gl->arr[g];  /* goto to be resolved */
  lua_assert(eqstr(gt->name, label->name));
  if (l_unlikely(gt->nactvar < label->nactvar)) /* enter some scope? */
    jumpscopeerror(ls, gt);
  luaK_patchlist(ls->fs, gt->pc, label->pc);
  for (i = g; i < gl->n - 1; i++) /* remove goto from pending list */
    gl->arr[i] = gl->arr[i + 1];
  gl->n--;
}

/*
** Search for an active label with the given name.
*/
static Labeldesc *findlabel(LexState *ls, TString *name) {
  int i;
  Dyndata *dyd = ls->dyd;
  /* check labels in current function for a match */
  for (i = ls->fs->firstlabel; i < dyd->label.n; i++) {
    Labeldesc *lb = &dyd->label.arr[i];
    if (eqstr(lb->name, name)) /* correct label? */
      return lb;
  }
  return NULL; /* label not found */
}

/*
** Adds a new label/goto in the corresponding list.
*/
static int newlabelentry(LexState *ls, Labellist *l, TString *name, int line,
                         int pc) {
  int n = l->n;
  luaM_growvector(ls->L, l->arr, n, l->size, Labeldesc, SHRT_MAX,
                  "labels/gotos");
  l->arr[n].name = name;
  l->arr[n].line = line;
  l->arr[n].nactvar = ls->fs->nactvar;
  l->arr[n].close = 0;
  l->arr[n].pc = pc;
  l->n = n + 1;
  return n;
}

static int newgotoentry(LexState *ls, TString *name, int line, int pc) {
  return newlabelentry(ls, &ls->dyd->gt, name, line, pc);
}

/*
** Solves forward jumps. Check whether new label 'lb' matches any
** pending gotos in current block and solves them. Return true
** if any of the goto's need to close upvalues.
*/
static int solvegotos(LexState *ls, Labeldesc *lb) {
  Labellist *gl = &ls->dyd->gt;
  int i = ls->fs->bl->firstgoto;
  int needsclose = 0;
  while (i < gl->n) {
    if (eqstr(gl->arr[i].name, lb->name)) {
      needsclose |= gl->arr[i].close;
      solvegoto(ls, i, lb); /* will remove 'i' from the list */
    } else
      i++;
  }
  return needsclose;
}

/*
** Create a new label with the given 'name' at the given 'line'.
** 'last' tells whether label is the last non-op statement in its
** block. Solves all pending goto's to this new label and adds
** a close instruction if necessary.
** Returns true iff it added a close instruction.
*/
static int createlabel(LexState *ls, TString *name, int line, int last) {
  FuncState *fs = ls->fs;
  Labellist *ll = &ls->dyd->label;
  int l = newlabelentry(ls, ll, name, line, luaK_getlabel(fs));
  if (last) { /* label is last no-op statement in the block? */
    /* assume that locals are already out of scope */
    ll->arr[l].nactvar = fs->bl->nactvar;
  }
  if (solvegotos(ls, &ll->arr[l])) { /* need close? */
    luaK_codeABC(fs, OP_CLOSE, luaY_nvarstack(fs), 0, 0);
    return 1;
  }
  return 0;
}

/*
** Adjust pending gotos to outer level of a block.
*/
static void movegotosout(FuncState *fs, BlockCnt *bl) {
  int i;
  Labellist *gl = &fs->ls->dyd->gt;
  /* correct pending gotos to current block */
  for (i = bl->firstgoto; i < gl->n; i++) { /* for each pending goto */
    Labeldesc *gt = &gl->arr[i];
    /* leaving a variable scope? */
    if (reglevel(fs, gt->nactvar) > reglevel(fs, bl->nactvar))
      gt->close |= bl->upval;  /* jump may need a close */
    gt->nactvar = bl->nactvar; /* update goto level */
  }
}

static void enterblock(FuncState *fs, BlockCnt *bl, lu_byte isloop) {
  bl->isloop = isloop;
  bl->nactvar = fs->nactvar;
  bl->firstlabel = fs->ls->dyd->label.n;
  bl->firstgoto = fs->ls->dyd->gt.n;
  bl->upval = 0;
  bl->insidetbc = (fs->bl != NULL && fs->bl->insidetbc);
  bl->previous = fs->bl;
  fs->bl = bl;
  lua_assert(fs->freereg == luaY_nvarstack(fs));
}

/*
** create a label named 'continue' to resolve continue statements
*/
static int fixcontinues(LexState *ls, BlockCnt *bl) {
  TString *n = luaS_new(ls->L, LABEL_CONTINUE);
  /*ls->fs->pc-1 to jump to inside the loop botton */
  int target = ls->fs->pc - 1;
  if (bl->isloop & (FSCOPE_FORINLOOP | FSCOPE_DOWHILELOOP)) --target;
  int l = newlabelentry(ls, &ls->dyd->label, n, 0, target);
  return solvegotos(ls, &ls->dyd->label.arr[l]);
}

/*
** generates an error for an undefined 'goto'.
*/
static l_noret undefgoto(LexState *ls, Labeldesc *gt) {
  const char *msg;
  if (eqstr(gt->name, luaS_newliteral(ls->L, LABEL_BREAK))) {
    msg = "break outside loop at line %d";
    msg = luaO_pushfstring(ls->L, msg, gt->line);
  } else {
    msg = "no visible label '%s' for <goto> at line %d";
    msg = luaO_pushfstring(ls->L, msg, getstr(gt->name), gt->line);
  }
  luaK_semerror(ls, msg);
}

static void leaveblock(FuncState *fs) {
  BlockCnt *bl = fs->bl;
  LexState *ls = fs->ls;
  int hasclose = 0;
  int stklevel = reglevel(fs, bl->nactvar); /* level outside the block */
  if (bl->isloop) {                         /* fix pending breaks? */
    if (bl->isloop & LOOP_HAS_CONTINUE && !(bl->isloop & FSCOPE_DOWHILELOOP))
      hasclose += fixcontinues(ls, bl); /* close pending continues */
    if (bl->isloop & LOOP_HAS_BREAK)
      hasclose += createlabel(ls, luaS_newliteral(ls->L, LABEL_BREAK), 0, 0);
  }
  if (!hasclose && bl->previous && bl->upval)
    luaK_codeABC(fs, OP_CLOSE, stklevel, 0, 0);
  fs->bl = bl->previous;
  removevars(fs, bl->nactvar);
  lua_assert(bl->nactvar == fs->nactvar);
  fs->freereg = stklevel;            /* free registers */
  ls->dyd->label.n = bl->firstlabel; /* remove local labels */
  if (bl->previous)                  /* inner block? */
    movegotosout(fs, bl);            /* update pending gotos to outer block */
  else {
    if (bl->firstgoto < ls->dyd->gt.n) /* pending gotos in outer block? */
      undefgoto(ls, &ls->dyd->gt.arr[bl->firstgoto]); /* error */
  }
}

/*
** adds a new prototype into list of prototypes
*/
static Proto *addprototype(LexState *ls) {
  Proto *clp;
  lua_State *L = ls->L;
  FuncState *fs = ls->fs;
  Proto *f = fs->f; /* prototype of current function */
  if (fs->np >= f->sizep) {
    int oldsize = f->sizep;
    luaM_growvector(L, f->p, fs->np, f->sizep, Proto *, MAXARG_Bx, "functions");
    while (oldsize < f->sizep) f->p[oldsize++] = NULL;
  }
  f->p[fs->np++] = clp = luaF_newproto(L);
  luaC_objbarrier(L, f, clp);
  return clp;
}

/*
** codes instruction to create new closure in parent function.
** The OP_CLOSURE instruction uses the last available register,
** so that, if it invokes the GC, the GC knows which registers
** are in use at that time.

*/
static void codeclosure(LexState *ls, expdesc *v) {
  FuncState *fs = ls->fs->prev;
  init_exp(v, VRELOC, luaK_codeABx(fs, OP_CLOSURE, 0, fs->np - 1));
  luaK_exp2nextreg(fs, v); /* fix it at the last register */
}

static void open_func(LexState *ls, FuncState *fs, BlockCnt *bl) {
  Proto *f = fs->f;
  fs->prev = ls->fs; /* linked list of funcstates */
  fs->ls = ls;
  ls->fs = fs;
  fs->pc = 0;
  fs->previousline = f->linedefined;
  fs->iwthabs = 0;
  fs->lasttarget = 0;
  fs->freereg = 0;
  fs->nk = 0;
  fs->nabslineinfo = 0;
  fs->np = 0;
  fs->nups = 0;
  fs->ndebugvars = 0;
  fs->nactvar = 0;
  fs->needclose = 0;
  fs->firstlocal = ls->dyd->actvar.n;
  fs->firstlabel = ls->dyd->label.n;
  fs->bl = NULL;
  f->source = ls->source;
  luaC_objbarrier(ls->L, f, f->source);
  f->maxstacksize = 2; /* registers 0/1 are always valid */
  enterblock(fs, bl, 0);
}

static void close_func(LexState *ls) {
  lua_State *L = ls->L;
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  luaK_ret(fs, luaY_nvarstack(fs), 0); /* final return */
  leaveblock(fs);
  lua_assert(fs->bl == NULL);
  luaK_finish(fs);
  luaM_shrinkvector(L, f->code, f->sizecode, fs->pc, Instruction);
  luaM_shrinkvector(L, f->lineinfo, f->sizelineinfo, fs->pc, ls_byte);
  luaM_shrinkvector(L, f->abslineinfo, f->sizeabslineinfo, fs->nabslineinfo,
                    AbsLineInfo);
  luaM_shrinkvector(L, f->k, f->sizek, fs->nk, TValue);
  luaM_shrinkvector(L, f->p, f->sizep, fs->np, Proto *);
  luaM_shrinkvector(L, f->locvars, f->sizelocvars, fs->ndebugvars, LocVar);
  luaM_shrinkvector(L, f->upvalues, f->sizeupvalues, fs->nups, Upvaldesc);
  ls->fs = fs->prev;
  luaC_checkGC(L);
}

/*============================================================*/
/* GRAMMAR RULES */
/*============================================================*/

/*
** check whether current token is in the follow set of a block.
** 'until' closes syntactical blocks, but do not close scope,
** so it is handled in separate.
*/
static int block_follow(LexState *ls /*, int dowhile*/) {
  switch (ls->t.token) {
    case TK_ELSE:
    case '}':
    case TK_EOS:
      return 1;
    default:
      return 0;
  }
}

static void statlist(LexState *ls) {
  /* statlist -> { stat [';'] } */
  while (!block_follow(ls /*, 1*/)) {
    if (ls->t.token == TK_RETURN) {
      statement(ls);
      return; /* 'return' must be last statement */
    }
    statement(ls);
  }
}

static void fieldsel(LexState *ls, expdesc *v) {
  /* fieldsel -> ['.' | ':'] NAME */
  FuncState *fs = ls->fs;
  expdesc key;
  luaK_exp2anyregup(fs, v);
  luaX_next(ls); /* skip the dot or colon */
  codename(ls, &key);
  luaK_indexed(fs, v, &key);
}

static void yindex(LexState *ls, expdesc *v) {
  /* index -> '[' expr ']' */
  luaX_next(ls); /* skip the '[' */
  expr(ls, v);
  luaK_exp2val(ls->fs, v);
  checknext(ls, ']');
}

static void sindex(LexState *ls, expdesc *v) {
  /* index -> string */
  expr(ls, v);
  luaK_exp2val(ls->fs, v);
}

/*
** {======================================================================
** Rules for Constructors
** =======================================================================
*/

typedef struct ConsControl {
  expdesc v;   /* last list item read */
  expdesc *t;  /* table descriptor */
  int nh;      /* total number of 'record' elements */
  int na;      /* number of array elements already stored */
  int tostore; /* number of array elements pending to be stored */
} ConsControl;

static void recfield(LexState *ls, ConsControl *cc) {
  /* recfield -> (NAME | '['exp']') = exp */
  FuncState *fs = ls->fs;
  int reg = ls->fs->freereg;
  expdesc tab, key, val;
  if (ls->t.token == TK_NAME) {
    checklimit(fs, cc->nh, MAX_INT, "items in a constructor");
    codename(ls, &key);
  } else if (ls->t.token == TK_STRING) {
    sindex(ls, &key);
  } else if (ls->t.token == TK_FSTRING) {
    luaX_syntaxerror(ls, "formatted strings cannot be used as a key");
  } else /* ls->t.token == '[' */
    yindex(ls, &key);
  cc->nh++;
  if (!(testnext(ls, '=') || testnext(ls, ':')))
    luaX_syntaxerror(ls, "expect '=' or ':' as key value delimiter");
  tab = *cc->t;
  luaK_indexed(fs, &tab, &key);
  expr(ls, &val);
  luaK_storevar(fs, &tab, &val);
  fs->freereg = reg; /* free registers */
}

static void closelistfield(FuncState *fs, ConsControl *cc) {
  if (cc->v.k == VVOID) return; /* there is no list item */
  luaK_exp2nextreg(fs, &cc->v);
  cc->v.k = VVOID;
  if (cc->tostore == LFIELDS_PER_FLUSH) {
    luaK_setlist(fs, cc->t->u.info, cc->na, cc->tostore); /* flush */
    cc->na += cc->tostore;
    cc->tostore = 0; /* no more items pending */
  }
}

static void lastlistfield(FuncState *fs, ConsControl *cc) {
  if (cc->tostore == 0) return;
  if (hasmultret(cc->v.k)) {
    luaK_setmultret(fs, &cc->v);
    luaK_setlist(fs, cc->t->u.info, cc->na, LUA_MULTRET);
    cc->na--; /* do not count last expression (unknown number of elements) */
  } else {
    if (cc->v.k != VVOID) luaK_exp2nextreg(fs, &cc->v);
    luaK_setlist(fs, cc->t->u.info, cc->na, cc->tostore);
  }
  cc->na += cc->tostore;
}

static void listfield(LexState *ls, ConsControl *cc) {
  /* listfield -> exp */
  expr(ls, &cc->v);
  cc->tostore++;
}

#include <stdio.h>

static void field(LexState *ls, ConsControl *cc) {
  /* field -> listfield | recfield */
  switch (ls->t.token) {
    case TK_STRING:
    case TK_FSTRING:
    case TK_NAME: { /* may be 'listfield' or 'recfield' */
      int ntk = luaX_lookahead(ls);
      if (!((ntk == '=') || (ntk == ':'))) /* expression? */
        listfield(ls, cc);
      else
        recfield(ls, cc);
      break;
    }
    case '[': {
      recfield(ls, cc);
      break;
    }
    default: {
      listfield(ls, cc);
      break;
    }
  }
}

static void constructor_base(LexState *ls, expdesc *t, int openTk,
                             int closeTk) {
  /* constructor -> '{' [ field { sep field } [sep] ] '}'
     sep -> ',' | ';' */
  FuncState *fs = ls->fs;
  int line = ls->linenumber;
  int pc = luaK_codeABC(fs, OP_NEWTABLE, 0, 0, 0);
  ConsControl cc;
  luaK_code(fs, 0); /* space for extra arg. */
  cc.na = cc.nh = cc.tostore = 0;
  cc.t = t;
  init_exp(t, VNONRELOC, fs->freereg); /* table will be at stack top */
  luaK_reserveregs(fs, 1);
  init_exp(&cc.v, VVOID, 0); /* no value (yet) */
  checknext(ls, openTk);
  do {
    lua_assert(cc.v.k == VVOID || cc.tostore > 0);
    if (ls->t.token == closeTk) break;
    closelistfield(fs, &cc);
    if (openTk == '{')
      field(ls, &cc);
    else
      listfield(ls, &cc);
  } while (testnext(ls, ',') || testnext(ls, ';'));
  check_match(ls, closeTk, openTk, line);
  lastlistfield(fs, &cc);
  luaK_settablesize(fs, pc, t->u.info, cc.na, cc.nh);
}

static void constructor(LexState *ls, expdesc *t) {
  /* constructor -> '{' [ field { sep field } [sep] ] '}'
     sep -> ',' | ';' */
  constructor_base(ls, t, '{', '}');
}

static void constructor_array(LexState *ls, expdesc *t) {
  /* constructor -> '[' [ field { sep field } [sep] ] ']'
     sep -> ',' | ';' */
  constructor_base(ls, t, '[', ']');
}

/* }====================================================================== */

static void optParamType(LexState *ls) {
  if (testnext(ls, ':')) {   /*optional parameter type*/
    checknext(ls, TK_NAME);  /*for now do nothing, discard*/
    if (testnext(ls, '[')) { /*optional parameter type*/
      checknext(ls, ']');    /*for now do nothing, discard*/
    }
  }
}

static void setvararg(FuncState *fs, int nparams) {
  fs->f->is_vararg = 1;
  luaK_codeABC(fs, OP_VARARGPREP, nparams, 0, 0);
}

static void parlist(LexState *ls) {
  /* parlist -> [ {NAME ','} (NAME | '...') ] */
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  int nparams = 0;
  int isvararg = 0;
  if (ls->t.token != ')') { /* is 'parlist' not empty? */
    do {
      switch (ls->t.token) {
        case TK_NAME: {
          new_localvar(ls, str_checkname(ls));
          nparams++;
          break;
        }
        case TK_DOTS: {
          luaX_next(ls);
          isvararg = 1;
          break;
        }
        default:
          luaX_syntaxerror(ls, "<name> or '...' expected");
      }
      if (testnext(ls, ':')) {  /*optional parameter type*/
        checknext(ls, TK_NAME); /*for now do nothing, discard*/
      }
      optParamType(ls);
    } while (!isvararg && testnext(ls, ','));
  }
  adjustlocalvars(ls, nparams);
  f->numparams = cast_byte(fs->nactvar);
  if (isvararg) setvararg(fs, f->numparams); /* declared vararg */
  luaK_reserveregs(fs, fs->nactvar); /* reserve registers for parameters */
}

/*
** Lambda implementation.
** Shorthands lambda expressions into `function (...) return ... end`.
** The '|' token was chosen because it's not commonly used as an unary operator in programming.
** The ':>' arrow syntax looked more visually appealing than a colon. It also plays along with common lambda tokens.
*/
static void lambdabody (LexState *ls, expdesc *e, int line) {
  FuncState new_fs;
  BlockCnt bl;
  new_fs.f = addprototype(ls);
  new_fs.f->linedefined = line;
  open_func(ls, &new_fs, &bl);
  checknext(ls, '|');
  parlist(ls);
  checknext(ls, '|');
  checknext(ls, ':');
  checknext(ls, '>');
  expr(ls, e);
  luaK_ret(&new_fs, luaK_exp2anyreg(&new_fs, e), 1);
  new_fs.f->lastlinedefined = ls->linenumber;
  codeclosure(ls, e);
  close_func(ls);
}

static void body(LexState *ls, expdesc *e, int ismethod, int line) {
  /* body ->  '(' parlist ')' block END */
  FuncState new_fs;
  BlockCnt bl;
  new_fs.f = addprototype(ls);
  new_fs.f->linedefined = line;
  open_func(ls, &new_fs, &bl);
  checknext(ls, '(');
  if (ismethod) {
    new_localvarliteral(ls, "this"); /* create 'this' parameter */
    adjustlocalvars(ls, 1);
  }
  parlist(ls);
  checknext(ls, ')');
  optParamType(ls);
  checknext(ls, '{');
  statlist(ls);
  new_fs.f->lastlinedefined = ls->linenumber;
  check_match(ls, '}', TK_FUNCTION, line);
  codeclosure(ls, e);
  close_func(ls);
}

static int explist(LexState *ls, expdesc *v) {
  /* explist -> expr { ',' expr } */
  int n = 1; /* at least one expression */
  expr(ls, v);
  while (testnext(ls, ',')) {
    luaK_exp2nextreg(ls->fs, v);
    expr(ls, v);
    n++;
  }
  return n;
}

static void funcargs(LexState *ls, expdesc *f, int line) {
  FuncState *fs = ls->fs;
  expdesc args;
  int base, nparams;
  switch (ls->t.token) {
    case '(': { /* funcargs -> '(' [ explist ] ')' */
      luaX_next(ls);
      if (ls->t.token == ')') /* arg list is empty? */
        args.k = VVOID;
      else {
        explist(ls, &args);
        if (hasmultret(args.k)) luaK_setmultret(fs, &args);
      }
      check_match(ls, ')', '(', line);
      break;
    }
    default: {
      luaX_syntaxerror(ls, "function arguments expected");
    }
  }
  lua_assert(f->k == VNONRELOC);
  base = f->u.info; /* base register for call */
  if (hasmultret(args.k))
    nparams = LUA_MULTRET; /* open call */
  else {
    if (args.k != VVOID) luaK_exp2nextreg(fs, &args); /* close last argument */
    nparams = fs->freereg - (base + 1);
  }
  init_exp(f, VCALL, luaK_codeABC(fs, OP_CALL, base, nparams + 1, 2));
  luaK_fixline(fs, line);
  fs->freereg = base + 1; /* call remove function and arguments and leaves
                             (unless changed) one result */
}

/*
** {======================================================================
** Expression parsing
** =======================================================================
*/

static void inc_dec_op(LexState *ls, BinOpr op, expdesc *v, int isPost);


/*
** Safe navigation is entirely accredited to SvenOlsen.
** http://lua-users.org/wiki/SvenOlsen
*/
static void safe_navigation(LexState *ls, expdesc *v) {
  FuncState *fs = ls->fs;
  luaK_exp2nextreg(fs, v);
  luaK_codeABC(fs, OP_TEST, v->u.info, NO_REG, 0 );
  {
    int old_free = fs->freereg;             
    int vreg = v->u.info;
    int j = luaK_jump(fs);
    expdesc key;
    switch(ls->t.token) {
      case '[': {
        luaX_next(ls);  /* skip the '[' */
        if (ls->t.token == '-') {
          expr(ls, &key);
          switch (key.k) {
            case VKINT: {
              key.u.ival *= -1;
              break;
            }
            case VKFLT: {
              key.u.nval *= -1;
              break;
            }
            default: {
              luaX_notedsyntaxerror(ls, "unexpected symbol during navigation.", "unary '-' on non-numeral type.");
            }
          }
        }
        else expr(ls, &key);
        checknext(ls, ']');
        luaK_indexed(fs, v, &key);
        break; 
      }       
      case '.': {
        luaX_next(ls);
        codename(ls, &key);
        luaK_indexed(fs, v, &key);
        break;
      }
      default: {
        luaX_syntaxerror(ls, "unexpected symbol");
      }
    }
    luaK_exp2nextreg(fs, v);
    fs->freereg = old_free;
    if (v->u.info != vreg) {
      luaK_codeABC(fs, OP_MOVE, vreg, v->u.info, 0);
      v->u.info = vreg;
    }
    luaK_patchtohere(fs, j);
  }
}


static void primaryexp(LexState *ls, expdesc *v) {
  /* primaryexp -> NAME | '(' expr ')' */
  switch (ls->t.token) {
    case '(': {
      int line = ls->linenumber;
      luaX_next(ls);
      expr(ls, v);
      check_match(ls, ')', '(', line);
      luaK_dischargevars(ls->fs, v);
      return;
    }
    case TK_NAME: {
      singlevar(ls, v);
      return;
    }
    case TK_PLUSPLUS: {
      luaX_next(ls);
      inc_dec_op(ls, OPR_ADD, v, 0);
      return;
    }
    case TK_MINUSMINUS: {
      luaX_next(ls);
      inc_dec_op(ls, OPR_SUB, v, 0);
      return;
    }
    case TK_FORMAT: {
      printf("format\n");
      /* print the contents of format also */
      printf("%s\n", getstr(ls->t.seminfo.ts));
      luaX_next(ls);
      return;
    }
    default: {
      luaX_syntaxerror(ls, "unexpected symbol");
    }
  }
}

static void suffixedexp(LexState *ls, expdesc *v) {
  /* suffixedexp ->
       primaryexp { '.' NAME | '[' exp ']' | ':' NAME funcargs | funcargs } */
  FuncState *fs = ls->fs;
  int line = ls->linenumber;
  primaryexp(ls, v);
  for (;;) {
    switch (ls->t.token) {
      case '?': {  /* safe navigation */
        luaX_next(ls); /* skip '?' */
        
        safe_navigation(ls, v);
        break;
      }
      case '.': { /* fieldsel */
        fieldsel(ls, v);
        break;
      }
      case '[': { /* '[' exp ']' */
        expdesc key;
        luaK_exp2anyregup(fs, v);
        yindex(ls, &key);
        luaK_indexed(fs, v, &key);
        break;
      }
      case TK_ARROW: { /* ':' NAME funcargs */
        expdesc key;
        luaX_next(ls);
        codename(ls, &key);
        luaK_self(fs, v, &key);
        funcargs(ls, v, line);
        break;
      }
      case '(': { /* funcargs */
        luaK_exp2nextreg(fs, v);
        funcargs(ls, v, line);
        break;
      }
      case TK_PLUSPLUS: {
        luaX_next(ls);
        inc_dec_op(ls, OPR_ADD, v, 1);
        return;
      }
      case TK_MINUSMINUS: {
        luaX_next(ls);
        inc_dec_op(ls, OPR_SUB, v, 1);
        return;
      }
      default:
        return;
    }
  }
}

static void simpleexp(LexState *ls, expdesc *v) {
  /* simpleexp -> FLT | INT | STRING | NIL | TRUE | FALSE | ... |
                  constructor | FUNCTION body | suffixedexp */
  switch (ls->t.token) {
    case TK_FLT: {
      init_exp(v, VKFLT, 0);
      v->u.nval = ls->t.seminfo.r;
      break;
    }
    case TK_INT: {
      init_exp(v, VKINT, 0);
      v->u.ival = ls->t.seminfo.i;
      break;
    }
    case TK_FSTRING:
    case TK_STRING: {
      codestring(v, ls->t.seminfo.ts);
      break;
    }
    case TK_NIL: {
      init_exp(v, VNIL, 0);
      break;
    }
    case TK_TRUE: {
      init_exp(v, VTRUE, 0);
      break;
    }
    case TK_FALSE: {
      init_exp(v, VFALSE, 0);
      break;
    }
    case TK_DOTS: { /* vararg */
      FuncState *fs = ls->fs;
      check_condition(ls, fs->f->is_vararg,
                      "cannot use '...' outside a vararg function");
      init_exp(v, VVARARG, luaK_codeABC(fs, OP_VARARG, 0, 0, 1));
      break;
    }
    case '{': { /* constructor */
      constructor(ls, v);
      return;
    }
    case '[': { /* array */
      constructor_array(ls, v);
      return;
    }
    case TK_FUNCTION: {
      luaX_next(ls);
      body(ls, v, 0, ls->linenumber);
      return;
    }
    case '|': {
      lambdabody(ls, v, ls->linenumber);
      return;
    }
    case TK___LINE__: {
      init_exp(v, VKINT, 0);
      v->u.ival = ls->linenumber;
      break;
    }
    case TK___FILE__: {
      codestring(v, ls->source);
      break;
    }
    default: {
      suffixedexp(ls, v);
      return;
    }
  }
  luaX_next(ls);
}

static UnOpr getunopr(int op) {
  switch (op) {
    case TK_NOT:
      return OPR_NOT;
    case '-':
      return OPR_MINUS;
    case '!':
      return OPR_BNOT;
    case '#':
      return OPR_LEN;
    // case TK_PLUSPLUS: return OPR_PLUSPLUS;
    // case TK_MINUSMINUS: return OPR_MINUSMINUS;
    default:
      return OPR_NOUNOPR;
  }
}

static BinOpr getbinopr(int op) {
  switch (op) {
    /*
     * if we change the number/order of elments here
     * we should also change in the priority struct bellow
     * and on lcode.h enum BinOpr
     */
    case '+':
      return OPR_ADD;
    case '-':
      return OPR_SUB;
    case '*':
      return OPR_MUL;
    case '%':
      return OPR_MOD;
    case TK_POW:
      return OPR_POW;
    case '/':
      return OPR_DIV;
    case TK_IDIV:
      return OPR_IDIV;
    case '&':
      return OPR_BAND;
    case '|':
      return OPR_BOR;
    case '^':
      return OPR_BXOR;
    case TK_SHL:
      return OPR_SHL;
    case TK_SHR:
      return OPR_SHR;
    case TK_FSTRING:
      return OPR_CONCAT;
    case TK_CONCAT:
      return OPR_CONCAT;
    case TK_NE:
      return OPR_NE;
    case TK_COAL: 
      return OPR_COAL;
    case TK_EQ:
      return OPR_EQ;
    case '<':
      return OPR_LT;
    case TK_LE:
      return OPR_LE;
    case '>':
      return OPR_GT;
    case TK_GE:
      return OPR_GE;
    case TK_AND:
      return OPR_AND;
    case TK_OR:
      return OPR_OR;
    default:
      return OPR_NOBINOPR;
  }
}

/*
** Priority table for binary operators.
*/
static const struct {
  lu_byte left;  /* left priority for each binary operator */
  lu_byte right; /* right priority */
} priority[] = {
    /* ORDER OPR */
    {10, 10}, {10, 10},         /* '+' '-' */
    {11, 11}, {11, 11},         /* '*' '%' */
    {14, 13},                   /* '**' (right associative) */
    {11, 11}, {11, 11},         /* '/' '//' */
    {6, 6},   {4, 4},   {5, 5}, /* '&' '|' '~' */
    {7, 7},   {7, 7},           /* '<<' '>>' */
    {9, 8},                     /* '..' (right associative) */
    {3, 3},   {3, 3},   {3, 3}, /* ==, <, <= */
    {3, 3},   {3, 3},   {3, 3}, /* !=, >, >= */
    {2, 2},   {1, 1}, {1, 1}            /* and, or, ?? */
};

#define UNARY_PRIORITY 12 /* priority for unary operators */

/*
** subexpr -> (simpleexp | unop subexpr) { binop subexpr }
** where 'binop' is any binary operator with a priority higher than 'limit'
*/
static BinOpr subexpr(LexState *ls, expdesc *v, int limit) {
  BinOpr op;
  UnOpr uop;
  enterlevel(ls);
  uop = getunopr(ls->t.token);
  if (uop != OPR_NOUNOPR) { /* prefix (unary) operator? */
    int line = ls->linenumber;
    luaX_next(ls); /* skip operator */
    subexpr(ls, v, UNARY_PRIORITY);
    luaK_prefix(ls->fs, uop, v, line);
  } else
    simpleexp(ls, v);
  /* expand while operators have priorities higher than 'limit' */
  op = getbinopr(ls->t.token);
  while (op != OPR_NOBINOPR && priority[op].left > limit) {
    expdesc v2;
    BinOpr nextop;
    int line = ls->linenumber;
    luaX_next(ls); /* skip operator */
    luaK_infix(ls->fs, op, v);
    /* read sub-expression with higher priority */
    nextop = subexpr(ls, &v2, priority[op].right);
    luaK_posfix(ls->fs, op, v, &v2, line);
    op = nextop;
  }
  leavelevel(ls);
  return op; /* return first untreated operator */
}

static void expr(LexState *ls, expdesc *v) {
  subexpr(ls, v, 0);
  if (testnext(ls, '?')) {
    int condexit;
    int escapelist = NO_JUMP;
    int reg;
    FuncState *fs = ls->fs;
    if (v->k == VNIL) v->k = VFALSE; /* 'falses' are all equal here */
    luaK_goiftrue(ls->fs, v);        /* skip over block if condition is false */
    condexit = v->f;
    expr(ls, v);                  /* eval part for true conditional */
    luaK_exp2nextreg(fs, v);      /* set result to reg. */
    reg = luaK_exp2anyreg(fs, v); /* set result to reg. */
    luaK_concat(fs, &escapelist, luaK_jump(fs)); /* must jump over it */
    luaK_patchtohere(fs, condexit);
    checknext(ls, ':');
    expr(ls, v);                      /* eval part for false conditional */
    luaK_exp2reg(fs, v, reg);         /* set result to reg. */
    luaK_patchtohere(fs, escapelist); /* patch escape list to conditional end */
  }
}

/* }==================================================================== */

/*
** {======================================================================
** Rules for Statements
** =======================================================================
*/

static void block(LexState *ls) {
  /* block -> [stat | statlist] */
  int line = ls->linenumber;
  FuncState *fs = ls->fs;
  BlockCnt bl;
  enterblock(fs, &bl, 0);
  if (testnext(ls, '{')) {
    statlist(ls);
    check_match(ls, '}', '{', line);
  } else {
    statement(ls);
    while (testnext(ls, ';'))
      ;  // skip all ; after statement it breaks else/elseif
  }
  leaveblock(fs);
}

/*
** structure to chain all variables in the left-hand side of an
** assignment
*/
struct LHS_assign {
  struct LHS_assign *prev;
  expdesc v; /* variable (global, local, upvalue, or indexed) */
};

/*
** check whether, in an assignment to an upvalue/local variable, the
** upvalue/local variable is begin used in a previous assignment to a
** table. If so, save original upvalue/local value in a safe place and
** use this safe copy in the previous assignment.
*/
static void check_conflict(LexState *ls, struct LHS_assign *lh, expdesc *v) {
  FuncState *fs = ls->fs;
  int extra = fs->freereg; /* eventual position to save local variable */
  int conflict = 0;
  for (; lh; lh = lh->prev) {    /* check all previous assignments */
    if (vkisindexed(lh->v.k)) {  /* assignment to table field? */
      if (lh->v.k == VINDEXUP) { /* is table an upvalue? */
        if (v->k == VUPVAL && lh->v.u.ind.t == v->u.info) {
          conflict = 1; /* table is the upvalue being assigned now */
          lh->v.k = VINDEXSTR;
          lh->v.u.ind.t = extra; /* assignment will use safe copy */
        }
      } else { /* table is a register */
        if (v->k == VLOCAL && lh->v.u.ind.t == v->u.var.ridx) {
          conflict = 1;          /* table is the local being assigned now */
          lh->v.u.ind.t = extra; /* assignment will use safe copy */
        }
        /* is index the local being assigned? */
        if (lh->v.k == VINDEXED && v->k == VLOCAL &&
            lh->v.u.ind.idx == v->u.var.ridx) {
          conflict = 1;
          lh->v.u.ind.idx = extra; /* previous assignment will use safe copy */
        }
      }
    }
  }
  if (conflict) {
    /* copy upvalue/local value to a temporary (in position 'extra') */
    if (v->k == VLOCAL)
      luaK_codeABC(fs, OP_MOVE, extra, v->u.var.ridx, 0);
    else
      luaK_codeABC(fs, OP_GETUPVAL, extra, v->u.info, 0);
    luaK_reserveregs(fs, 1);
  }
}

/*
** Parse and compile a multiple assignment. The first "variable"
** (a 'suffixedexp') was already read by the caller.
**
** assignment -> suffixedexp restassign
** restassign -> ',' suffixedexp restassign | '=' explist
*/
static void restassign(LexState *ls, struct LHS_assign *lh, int nvars) {
  expdesc e;
  check_condition(ls, vkisvar(lh->v.k), "syntax error");
  check_readonly(ls, &lh->v);
  if (testnext(ls, ',')) { /* restassign -> ',' suffixedexp restassign */
    struct LHS_assign nv;
    nv.prev = lh;
    suffixedexp(ls, &nv.v);
    if (!vkisindexed(nv.v.k)) check_conflict(ls, lh, &nv.v);
    enterlevel(ls); /* control recursion depth */
    restassign(ls, &nv, nvars + 1);
    leavelevel(ls);
  } else { /* restassign -> '=' explist */
    int nexps;
    checknext(ls, '=');
    nexps = explist(ls, &e);
    if (nexps != nvars)
      adjust_assign(ls, nvars, nexps, &e);
    else {
      luaK_setoneret(ls->fs, &e); /* close last expression */
      luaK_storevar(ls->fs, &lh->v, &e);
      return; /* avoid default */
    }
  }
  init_exp(&e, VNONRELOC, ls->fs->freereg - 1); /* default assignment */
  luaK_storevar(ls->fs, &lh->v, &e);
}

static void assign_compound(LexState *ls, struct LHS_assign *lh, int opType) {
  FuncState *fs = ls->fs;
  expdesc lhv, infix, rh;
  int nexps;
  int line = ls->linenumber;
  BinOpr op;
  /*store expression before grounding */
  lhv = lh->v;

  check_condition(
      ls, vkisvar(lh->v.k),
      "syntax error in left hand expression in compound assignment");

  /* parse Compound operation. */
  switch (opType) {
    case TK_CADD:
      op = OPR_ADD;
      break;
    case TK_CSUB:
      op = OPR_SUB;
      break;
    case TK_CMUL:
      op = OPR_MUL;
      break;
    case TK_CDIV:
      op = OPR_DIV;
      break;
    case TK_CMOD:
      op = OPR_MOD;
      break;
  };
  luaX_next(ls);

  /* store compound results in a new register (needed for nested tables) */
  if (vkisindexed(lh->v.k)) luaK_reserveregs(fs, 1);

  /* ground the lhs expresion */
  luaK_exp2nextreg(fs, &lh->v);

  /* parse right-hand expression */
  nexps = explist(ls, &rh);
  check_condition(
      ls, nexps == 1,
      "syntax error in right hand expression in compound assignment");

  infix = lh->v;
  luaK_infix(fs, op, &infix);

  luaK_posfix(fs, op, &infix, &rh, line);
  /* use the lhs before grounding to store */
  luaK_storevar(fs, &lhv, &infix);
}

static int cond(LexState *ls) {
  /* cond -> exp */
  expdesc v;
  expr(ls, &v);                  /* read condition */
  if (v.k == VNIL) v.k = VFALSE; /* 'falses' are all equal here */
  luaK_goiftrue(ls->fs, &v);
  return v.f;
}

static int nocond(LexState *ls) {
  /* cond -> exp */
  expdesc v;
  expr(ls, &v);                  /* read condition */
  if (v.k == VNIL) v.k = VFALSE; /* 'falses' are all equal here */
  luaK_goiffalse(ls->fs, &v);
  return v.t;
}

static void gotostat(LexState *ls) {
  FuncState *fs = ls->fs;
  int line = ls->linenumber;
  TString *name = str_checkname(ls); /* label's name */
  Labeldesc *lb = findlabel(ls, name);
  if (lb == NULL) /* no label? */
    /* forward jump; will be resolved when the label is declared */
    newgotoentry(ls, name, line, luaK_jump(fs));
  else { /* found a label */
    /* backward jump; will be resolved here */
    int lblevel = reglevel(fs, lb->nactvar); /* label level */
    if (luaY_nvarstack(fs) > lblevel) /* leaving the scope of a variable? */
      luaK_codeABC(fs, OP_CLOSE, lblevel, 0, 0);
    /* create jump and link it to the label */
    luaK_patchlist(fs, luaK_jump(fs), lb->pc);
  }
}

/*
** Break statement. Semantically equivalent to "goto break".
*/
static void breakstat(LexState *ls) {
  BlockCnt *bl = ls->fs->bl;
  int line = ls->linenumber;
  luaX_next(ls); /* skip break */
  while (bl && !bl->isloop) {
    bl = bl->previous;
  }
  if (!bl) luaX_syntaxerror(ls, "no loop to break");
  newgotoentry(ls, luaS_newliteral(ls->L, LABEL_BREAK), line,
               luaK_jump(ls->fs));
  bl->isloop |= LOOP_HAS_BREAK;
}

static void continuestat(LexState *ls) {
  BlockCnt *bl = ls->fs->bl;
  int line = ls->linenumber;
  luaX_next(ls); /* skip continue */
  while (bl && !bl->isloop) {
    bl = bl->previous;
  }
  if (!bl) luaX_syntaxerror(ls, "no loop to continue");
  newgotoentry(ls, luaS_new(ls->L, LABEL_CONTINUE), line, luaK_jump(ls->fs));
  bl->isloop |= LOOP_HAS_CONTINUE;
}

/*
** Check whether there is already a label with the given 'name'.
*/
static void checkrepeated(LexState *ls, TString *name) {
  Labeldesc *lb = findlabel(ls, name);
  if (l_unlikely(lb != NULL)) { /* already defined? */
    const char *msg = "label '%s' already defined on line %d";
    msg = luaO_pushfstring(ls->L, msg, getstr(name), lb->line);
    luaK_semerror(ls, msg); /* error */
  }
}

static void labelstat(LexState *ls, TString *name, int line) {
  /* label -> NAME ':' */
  checknext(ls, ':');                       /* skip colon */
  while (ls->t.token == ';') statement(ls); /* skip other no-op statements */
  checkrepeated(ls, name);                  /* check for repeated labels */
  createlabel(ls, name, line, block_follow(ls /*, 0*/));
}

static void whilestat(LexState *ls /*, int line*/) {
  /* whilestat -> WHILE ( cond ) block */
  FuncState *fs = ls->fs;
  int whileinit;
  int condexit;
  BlockCnt bl;
  luaX_next(ls); /* skip WHILE */
  checknext(ls, '(');
  whileinit = luaK_getlabel(fs);
  condexit = cond(ls);
  checknext(ls, ')');
  enterblock(fs, &bl, FSCOPE_LOOP);
  block(ls);
  luaK_jumpto(fs, whileinit);
  leaveblock(fs);
  luaK_patchtohere(fs, condexit); /* false conditions finish the loop */
}

static void dowhilestat(LexState *ls, int line) {
  /* dowhilestat -> DO block WHILE ( cond ) */
  int condexit;
  FuncState *fs = ls->fs;
  int dowhile_init = luaK_getlabel(fs);
  BlockCnt bl1, bl2;
  enterblock(fs, &bl1, FSCOPE_LOOP | FSCOPE_DOWHILELOOP); /* loop block */
  enterblock(fs, &bl2, 0);                                /* scope block */
  luaX_next(ls);                                          /* skip DO */
  checknext(ls, '{');
  statlist(ls);
  check_match(ls, '}', TK_DO, line);
  if (bl1.isloop & LOOP_HAS_CONTINUE) {
    TString *n = luaS_new(ls->L, LABEL_CONTINUE);
    int l = newlabelentry(ls, &ls->dyd->label, n, 0, ls->fs->pc - 2);
    solvegotos(ls, &ls->dyd->label.arr[l]);
  }
  check_match(ls, TK_WHILE, TK_DO, line);
  line = ls->linenumber;
  checknext(ls, '(');
  condexit = nocond(ls); /* read condition (inside scope block) */
  check_match(ls, ')', TK_WHILE, line);
  leaveblock(fs);                   /* finish scope */
  if (bl2.upval) {                  /* upvalues? */
    int exit = luaK_jump(fs);       /* normal exit must jump over fix */
    luaK_patchtohere(fs, condexit); /* repetition must close upvalues */
    luaK_codeABC(fs, OP_CLOSE, reglevel(fs, bl2.nactvar), 0, 0);
    condexit = luaK_jump(fs);   /* repeat after closing upvalues */
    luaK_patchtohere(fs, exit); /* normal exit comes to here */
  }
  luaK_patchlist(fs, condexit, dowhile_init); /* close the loop */
  leaveblock(fs);                             /* finish loop */
}

/*
** Read an expression and generate code to put its results in next
** stack slot.
**
*/
static void exp1(LexState *ls) {
  expdesc e;
  expr(ls, &e);
  luaK_exp2nextreg(ls->fs, &e);
  lua_assert(e.k == VNONRELOC);
}

/*
** Fix for instruction at position 'pc' to jump to 'dest'.
** (Jump addresses are relative in Lua). 'back' true means
** a back jump.
*/
static void fixforjump(FuncState *fs, int pc, int dest, int back) {
  Instruction *jmp = &fs->f->code[pc];
  int offset = dest - (pc + 1);
  if (back) offset = -offset;
  if (l_unlikely(offset > MAXARG_Bx))
    luaX_syntaxerror(fs->ls, "control structure too long");
  SETARG_Bx(*jmp, offset);
}

/*
** Generate code for a 'for' loop.
*/
static void forbody(LexState *ls, int base, int line, int nvars, int isgen) {
  /* forbody -> DO block */
  static const OpCode forprep[2] = {OP_FORPREP, OP_TFORPREP};
  static const OpCode forloop[2] = {OP_FORLOOP, OP_TFORLOOP};
  BlockCnt bl;
  FuncState *fs = ls->fs;
  int prep, endfor;
  prep = luaK_codeABx(fs, forprep[isgen], base, 0);
  enterblock(fs, &bl, 0); /* scope for declared variables */
  adjustlocalvars(ls, nvars);
  luaK_reserveregs(fs, nvars);
  block(ls);
  leaveblock(fs); /* end of scope for declared variables */
  fixforjump(fs, prep, luaK_getlabel(fs), 0);
  if (isgen) { /* generic for? */
    luaK_codeABC(fs, OP_TFORCALL, base, 0, nvars);
    luaK_fixline(fs, line);
  }
  endfor = luaK_codeABx(fs, forloop[isgen], base, 0);
  fixforjump(fs, endfor, prep + 1, 1);
  luaK_fixline(fs, line);
}

static void fornum(LexState *ls, TString *varname, int line) {
  /* fornum -> NAME = exp,exp[,exp] forbody */
  FuncState *fs = ls->fs;
  int base = fs->freereg;
  new_localvarliteral(ls, "(for index)");
  new_localvarliteral(ls, "(for limit)");
  new_localvarliteral(ls, "(for step)");
  new_localvar(ls, varname);
  checknext(ls, '=');
  exp1(ls); /* initial value */
  checknext(ls, ',');
  exp1(ls); /* limit */
  if (testnext(ls, ','))
    exp1(ls); /* optional step */
  else {      /* default step = 1 */
    luaK_int(fs, fs->freereg, 1);
    luaK_reserveregs(fs, 1);
  }
  checknext(ls, ')');
  adjustlocalvars(ls, 3); /* control variables */
  forbody(ls, base, line, 1, 0);
}

static void forlist(LexState *ls, TString *indexname) {
  /* forlist -> NAME {,NAME} IN explist forbody */
  FuncState *fs = ls->fs;
  expdesc e;
  int nvars = 5; /* gen, state, control, toclose, 'indexname' */
  int line;
  int base = fs->freereg;
  /* create control variables */
  new_localvarliteral(ls, "(for generator)");
  new_localvarliteral(ls, "(for state)");
  new_localvarliteral(ls, "(for control)");
  new_localvarliteral(ls, "(for toclose)");
  /* create declared variables */
  new_localvar(ls, indexname);
  while (testnext(ls, ',')) {
    new_localvar(ls, str_checkname(ls));
    nvars++;
  }
  checknext(ls, TK_IN);
  line = ls->linenumber;
  adjust_assign(ls, 4, explist(ls, &e), &e);
  adjustlocalvars(ls, 4); /* control variables */
  marktobeclosed(fs);     /* last control var. must be closed */
  luaK_checkstack(fs, 3); /* extra space to call generator */
  checknext(ls, ')');
  forbody(ls, base, line, nvars - 4, 1);
}

static void forstat(LexState *ls, int line) {
  /* forstat -> FOR (fornum | forlist) END */
  FuncState *fs = ls->fs;
  TString *varname;
  BlockCnt bl;
  enterblock(fs, &bl, FSCOPE_LOOP); /* scope for loop and control variables */
  luaX_next(ls);                    /* skip 'for' */
  checknext(ls, '(');
  varname = str_checkname(ls); /* first variable name */
  switch (ls->t.token) {
    case '=':
      fornum(ls, varname, line);
      break;
    case ',':
    case TK_IN: {
      bl.isloop |= FSCOPE_FORINLOOP;
      forlist(ls, varname);
      break;
    }
    default:
      luaX_syntaxerror(ls, "'=' or 'in' expected");
  }
  leaveblock(fs); /* loop scope ('break' jumps to this point) */
}

/*
** Check whether next instruction is a single jump (a 'break', a 'goto'
** to a forward label, or a 'goto' to a backward label with no variable
** to close). If so, set the name of the 'label' it is jumping to
** ("break" for a 'break') or to where it is jumping to ('target') and
** return true. If not a single jump, leave input unchanged, to be
** handled as a regular statement.
*/
#if 0
static int issinglejump (LexState *ls, TString **label, int *target) {
  if (testnext(ls, TK_BREAK)) {  /* a break? */
    *label = luaS_newliteral(ls->L, "break");
    return 1;
  }
  else if (ls->t.token != TK_GOTO || luaX_lookahead(ls) != TK_NAME)
    return 0;  /* not a valid goto */
  else {
    TString *lname = ls->lookahead.seminfo.ts;  /* label's id */
    Labeldesc *lb = findlabel(ls, lname);
    if (lb) {  /* a backward jump? */
      /* does it need to close variables? */
      if (luaY_nvarstack(ls->fs) > stacklevel(ls->fs, lb->nactvar))
        return 0;  /* not a single jump; cannot optimize */
      *target = lb->pc;
    }
    else  /* jump forward */
      *label = lname;
    luaX_next(ls);  /* skip goto */
    luaX_next(ls);  /* skip name */
    return 1;
  }
}
#endif


static void test_then_block(LexState *ls, int *escapelist) {
  /* test_then_block -> [IF | ELSEIF] cond THEN block */
  FuncState *fs = ls->fs;
  expdesc v;
  int jf;        /* instruction to skip 'then' code (if condition is false) */
  luaX_next(ls); /* skip IF or ELSEIF */
  checknext(ls, '(');
  expr(ls, &v); /* read condition */
  checknext(ls, ')');
  luaK_goiftrue(ls->fs, &v); /* skip over block if condition is false */
  jf = v.f;
  block(ls);
  if (ls->t.token == TK_ELSE) /* followed by 'else'/'else if'? */
    luaK_concat(fs, escapelist, luaK_jump(fs)); /* must jump over it */
  luaK_patchtohere(fs, jf);
}

static void ifstat(LexState *ls /*, int line*/) {
  /* ifstat -> IF ( cond ) block {ELSE IF ( cond ) block} [ELSE block] */
  FuncState *fs = ls->fs;
  int escapelist = NO_JUMP;         /* exit list for finished parts */
  test_then_block(ls, &escapelist); /* IF ( cond ) block */
  while (testnext(ls, TK_ELSE)) {
    if (ls->t.token == TK_IF) {
      test_then_block(ls, &escapelist); /* ELSE IF cond THEN block */
      continue;                         /* try again nested ELSE IF */
    }
    block(ls); /* 'else' part */
    break;
  }
  luaK_patchtohere(fs, escapelist); /* patch escape list to 'if' end */
}

static void localfunc(LexState *ls) {
  expdesc b;
  FuncState *fs = ls->fs;
  int fvar = fs->nactvar;              /* function's variable index */
  new_localvar(ls, str_checkname(ls)); /* new local variable */
  adjustlocalvars(ls, 1);              /* enter its scope */
  body(ls, &b, 0, ls->linenumber);     /* function created in next register */
  /* debug information will only see the variable after this point! */
  localdebuginfo(fs, fvar)->startpc = fs->pc;
}

static int getlocalattribute(LexState *ls) {
  /* ATTRIB -> ['<' Name '>'] */
  if (testnext(ls, '<')) {
    const char *attr = getstr(str_checkname(ls));
    checknext(ls, '>');
    if (strcmp(attr, "const") == 0)
      return RDKCONST; /* read-only variable */
    else if (strcmp(attr, "close") == 0)
      return RDKTOCLOSE; /* to-be-closed variable */
    else if (strcmp(attr, "ref") == 0)
      luaK_semerror(
          ls, luaO_pushfstring(ls->L, "refrence values are not supported"));
    else if (strcmp(attr, "pre") == 0){
      luaG_addinfo(ls->L, "\n\033[1;33mwarning: \033[0m preprocessed variables will not work directly in the interpreter, use the preprocessor to generate preprocessed code", ls->linenumber, 0);
      return VDKREG;
    } else
      luaK_semerror(ls,
                    luaO_pushfstring(ls->L, "unknown attribute '%s'", attr));
  }
  return VDKREG; /* regular variable */
}

static void checktoclose(FuncState *fs, int level) {
  if (level != -1) { /* is there a to-be-closed variable? */
    marktobeclosed(fs);
    luaK_codeABC(fs, OP_TBC, reglevel(fs, level), 0, 0);
  }
}

static void localstat(LexState *ls) {
  /* stat -> LOCAL NAME ATTRIB { ',' NAME ATTRIB } ['=' explist] */
  FuncState *fs = ls->fs;
  int toclose = -1; /* index of to-be-closed variable (if any) */
  Vardesc *var;     /* last variable */
  int vidx, kind;   /* index and kind of last variable */
  int nvars = 0;
  int nexps;
  expdesc e;
  do {
    vidx = new_localvar(ls, str_checkname(ls));
    kind = getlocalattribute(ls);
    getlocalvardesc(fs, vidx)->vd.kind = kind;
    if (kind == RDKTOCLOSE) { /* to-be-closed? */
      if (toclose != -1)      /* one already present? */
        luaK_semerror(ls, "multiple to-be-closed variables in local list");
      toclose = fs->nactvar + nvars;
    }
    nvars++;
    optParamType(ls);
  } while (testnext(ls, ','));
  if (testnext(ls, '='))
    nexps = explist(ls, &e);
  else {
    e.k = VVOID;
    nexps = 0;
  }
  var = getlocalvardesc(fs, vidx);       /* get last variable */
  if (nvars == nexps &&                  /* no adjustments? */
      var->vd.kind == RDKCONST &&        /* last variable is const? */
      luaK_exp2const(fs, &e, &var->k)) { /* compile-time constant? */
    var->vd.kind = RDKCTC;          /* variable is a compile-time constant */
    adjustlocalvars(ls, nvars - 1); /* exclude last variable */
    fs->nactvar++;                  /* but count it */
  } else {
    adjust_assign(ls, nvars, nexps, &e);
    adjustlocalvars(ls, nvars);
  }
  checktoclose(fs, toclose);
}

static void constlocalstat(LexState *ls) {
  /* stat -> LOCAL NAME ATTRIB { ',' NAME ATTRIB } ['=' explist] */
  FuncState *fs = ls->fs;
  int toclose = -1; /* index of to-be-closed variable (if any) */
  Vardesc *var;     /* last variable */
  int vidx, kind;   /* index and kind of last variable */
  int nvars = 0;
  int nexps;
  expdesc e;
  do {
    vidx = new_localvar(ls, str_checkname(ls));
    kind = RDKCONST;
    getlocalvardesc(fs, vidx)->vd.kind = kind;
    if (kind == RDKTOCLOSE) { /* to-be-closed? */
      if (toclose != -1)      /* one already present? */
        luaK_semerror(ls, "multiple to-be-closed variables in local list");
      toclose = fs->nactvar + nvars;
    }
    nvars++;
    optParamType(ls);
  } while (testnext(ls, ','));
  if (testnext(ls, '='))
    nexps = explist(ls, &e);
  else {
    e.k = VVOID;
    nexps = 0;
  }
  var = getlocalvardesc(fs, vidx);       /* get last variable */
  if (nvars == nexps &&                  /* no adjustments? */
      var->vd.kind == RDKCONST &&        /* last variable is const? */
      luaK_exp2const(fs, &e, &var->k)) { /* compile-time constant? */
    var->vd.kind = RDKCTC;          /* variable is a compile-time constant */
    adjustlocalvars(ls, nvars - 1); /* exclude last variable */
    fs->nactvar++;                  /* but count it */
  } else {
    adjust_assign(ls, nvars, nexps, &e);
    adjustlocalvars(ls, nvars);
  }
  checktoclose(fs, toclose);
}

static void autolocalstat(LexState *ls) {
  /* stat -> LOCAL NAME ATTRIB { ',' NAME ATTRIB } ['=' explist] */
  FuncState *fs = ls->fs;
  int toclose = -1; /* index of to-be-closed variable (if any) */
  Vardesc *var;     /* last variable */
  int vidx, kind;   /* index and kind of last variable */
  int nvars = 0;
  int nexps;
  expdesc e;
  do {
    vidx = new_localvar(ls, str_checkname(ls));
    kind = RDKTOCLOSE;
    getlocalvardesc(fs, vidx)->vd.kind = kind;
    if (kind == RDKTOCLOSE) { /* to-be-closed? */
      if (toclose != -1)      /* one already present? */
        luaK_semerror(ls, "multiple to-be-closed variables in local list");
      toclose = fs->nactvar + nvars;
    }
    nvars++;
    optParamType(ls);
  } while (testnext(ls, ','));
  if (testnext(ls, '='))
    nexps = explist(ls, &e);
  else {
    e.k = VVOID;
    nexps = 0;
  }
  var = getlocalvardesc(fs, vidx);       /* get last variable */
  if (nvars == nexps &&                  /* no adjustments? */
      var->vd.kind == RDKCONST &&        /* last variable is const? */
      luaK_exp2const(fs, &e, &var->k)) { /* compile-time constant? */
    var->vd.kind = RDKCTC;          /* variable is a compile-time constant */
    adjustlocalvars(ls, nvars - 1); /* exclude last variable */
    fs->nactvar++;                  /* but count it */
  } else {
    adjust_assign(ls, nvars, nexps, &e);
    adjustlocalvars(ls, nvars);
  }
  checktoclose(fs, toclose);
}

static int funcname(LexState *ls, expdesc *v) {
  /* funcname -> NAME {fieldsel} [':' NAME] */
  int ismethod = 0;
  singlevar(ls, v);
  while (ls->t.token == '.') fieldsel(ls, v);
  if (ls->t.token == TK_DBCOLON) {
    ismethod = 1;
    fieldsel(ls, v);
  }
  return ismethod;
}

static void funcstat(LexState *ls, int line) {
  /* funcstat -> FUNCTION funcname body */
  int ismethod;
  expdesc v, b;
  luaX_next(ls); /* skip FUNCTION */
  ismethod = funcname(ls, &v);
  body(ls, &b, ismethod, line);
  check_readonly(ls, &v);
  luaK_storevar(ls->fs, &v, &b);
  luaK_fixline(ls->fs, line); /* definition "happens" in the first line */
}
static void exprstat(LexState *ls) {
  /* stat -> func | compound | assignment */
  FuncState *fs = ls->fs;
  struct LHS_assign v;
  suffixedexp(ls, &v.v);
  if (v.v.k == VCALL) { /* stat -> func */
    Instruction *inst = &getinstruction(fs, &v.v);
    SETARG_C(*inst, 1); /* call statement uses no results */
  } else {              /* stat -> compound | assignment */
    v.prev = NULL;
    switch (ls->t.token) {
      case TK_CADD:
      case TK_CSUB:
      case TK_CMUL:
      case TK_CDIV:
      case TK_CMOD:
        assign_compound(ls, &v, ls->t.token);
        break;
      case ',':
      case '=':
        restassign(ls, &v, 1);
        break;
      case ';':  // done:
        /* TK_PLUSPLUS, TK_MINUMINUS should be already managed */
        break;
      default:
        luaX_syntaxerror(
            ls,
            lua_pushfstring(
                ls->L,
                "'++', '--', +=', '-=', '*=', '/=', '%%=', ':=' or '=' expected"));
        break;
    }
  }
}

static void retstat(LexState *ls) {
  /* stat -> RETURN [explist] [';'] */
  FuncState *fs = ls->fs;
  expdesc e;
  int nret;                       /* number of values being returned */
  int first = luaY_nvarstack(fs); /* first slot to be returned */
  if (block_follow(ls /*, 1*/) || ls->t.token == ';')
    nret = 0; /* return no values */
  else {
    nret = explist(ls, &e); /* optional return values */
    if (hasmultret(e.k)) {
      luaK_setmultret(fs, &e);
      if (e.k == VCALL && nret == 1 && !fs->bl->insidetbc) { /* tail call? */
        SET_OPCODE(getinstruction(fs, &e), OP_TAILCALL);
        lua_assert(GETARG_A(getinstruction(fs, &e)) == luaY_nvarstack(fs));
      }
      nret = LUA_MULTRET; /* return all values */
    } else {
      if (nret == 1)                     /* only one single value? */
        first = luaK_exp2anyreg(fs, &e); /* can use original slot */
      else { /* values must go to the top of the stack */
        luaK_exp2nextreg(fs, &e);
        lua_assert(nret == fs->freereg - first);
      }
    }
  }
  luaK_ret(fs, first, nret);
  testnext(ls, ';'); /* skip optional semicolon */
}

static void inc_dec_op(LexState *ls, BinOpr op, expdesc *v, int isPost) {
  FuncState *fs = ls->fs;
  expdesc lv, e1, e2;
  int indices;
  if (!v) v = &lv;
  indices = fs->freereg;
  init_exp(&e2, VKINT, 0);
  e2.u.ival = (lua_Integer)1;
  if (isPost) {
    check_condition(ls, vkisvar(v->k),
                    "syntax error expression not assignable");
    lv = e1 = *v;
    if (vkisindexed(v->k)) luaK_reserveregs(fs, 1);
    luaK_exp2nextreg(fs, v);
    luaK_reserveregs(fs, 1);  // copy again to operate on it
    luaK_posfix(fs, op, &e1, &e2, ls->linenumber);
    luaK_storevar(fs, &lv, &e1);
    --fs->freereg;  // remove extra copy register
    return;
  }
  suffixedexp(ls, v);
  check_condition(ls, vkisvar(v->k), "syntax error expression not assignable");
  e1 = *v;
  if (vkisindexed(v->k)) luaK_reserveregs(fs, fs->freereg - indices);
  luaK_posfix(fs, op, &e1, &e2, ls->linenumber);
  luaK_storevar(fs, v, &e1);
  if (v != &lv) luaK_exp2nextreg(fs, v);
}

static void statement(LexState *ls) {
  int line = ls->linenumber; /* may be needed for error messages */
  enterlevel(ls);
  switch (ls->t.token) {
    case ';': {      /* stat -> ';' (empty statement) */
      luaX_next(ls); /* skip ';' */
      break;
    }
    case TK_IF: { /* stat -> ifstat */
      ifstat(ls /*, line*/);
      break;
    }
    case TK_WHILE: { /* stat -> whilestat */
      whilestat(ls /*, line*/);
      break;
    }
    case '{': { /* stat -> { block } */
      block(ls);
      break;
    }
    case TK_FOR: { /* stat -> forstat */
      forstat(ls, line);
      break;
    }
    case TK_DO: { /* stat -> dowhilestat */
      dowhilestat(ls, line);
      break;
    }
    case TK_FUNCTION: { /* stat -> funcstat */
      funcstat(ls, line);
      break;
    }
    case TK_LET: 
      /* add const */
      luaX_next(ls);                 /* skip LOCAL */
      if (testnext(ls, TK_FUNCTION)) /* local function? */
        luaX_notedsyntaxerror(ls, "functions cannot be constant", "use 'var' instead of 'let'");
      else
        constlocalstat(ls);
      break;
    case TK_AUTO:
      /* add const */
      luaX_next(ls);                 /* skip LOCAL */
      if (testnext(ls, TK_FUNCTION)) /* local function? */
        luaX_notedsyntaxerror(ls, "functions cannot be auto/to-be-closed", "use 'var' instead of 'auto'");
      else
        autolocalstat(ls);
      break;
    case TK_LOCAL: 
      printf("\033[1;33mwarning: \033[0m use 'var' instead of 'local'\n");
    case TK_VAR: {                 /* stat -> localstat */
      luaX_next(ls);                 /* skip LOCAL */
      if (testnext(ls, TK_FUNCTION)) /* local function? */
        localfunc(ls);
      else
        localstat(ls);
      break;
    }
    case TK_RETURN: { /* stat -> retstat */
      luaX_next(ls);  /* skip RETURN */
      retstat(ls);
      break;
    }
    case TK_CONTINUE: { /* stat -> continuestat */
      continuestat(ls);
      break;
    }
    case TK_BREAK: { /* stat -> breakstat */
      breakstat(ls);
      break;
    }
    case TK_GOTO: {  /* stat -> 'goto' NAME */
      luaX_next(ls); /* skip 'goto' */
      gotostat(ls);
      break;
    }
    case TK_PLUSPLUS: {
      luaX_next(ls);
      inc_dec_op(ls, OPR_ADD, NULL, 0);
      break;
    }
    case TK_MINUSMINUS: {
      luaX_next(ls);
      inc_dec_op(ls, OPR_SUB, NULL, 0);
      break;
    }
    case TK_NAME: {
      if (ls->current == ':') { /* stat -> label */
        labelstat(ls, str_checkname(ls), line);
        break;
      }
    }
    /*fallthrough*/
    default: { /* stat -> func | assignment */
      exprstat(ls);
      break;
    }
  }
  lua_assert(ls->fs->f->maxstacksize >= ls->fs->freereg &&
             ls->fs->freereg >= luaY_nvarstack(ls->fs));
  ls->fs->freereg = luaY_nvarstack(ls->fs); /* free registers */
  leavelevel(ls);
}

/* }====================================================================== */

/*
** compiles the main function, which is a regular vararg function with an
** upvalue named LUA_ENV
*/
static void mainfunc(LexState *ls, FuncState *fs) {
  BlockCnt bl;
  Upvaldesc *env;
  open_func(ls, fs, &bl);
  setvararg(fs, 0);       /* main function is always declared vararg */
  env = allocupvalue(fs); /* ...set environment upvalue */
  env->instack = 1;
  env->idx = 0;
  env->kind = VDKREG;
  env->name = ls->envn;
  luaC_objbarrier(ls->L, fs->f, env->name);
  luaX_next(ls); /* read first token */
  statlist(ls);  /* parse main body */
  check(ls, TK_EOS);
  close_func(ls);
}

LClosure *luaY_parser(lua_State *L, ZIO *z, Mbuffer *buff, Dyndata *dyd,
                      const char *name, int firstchar) {
  LexState lexstate;
  FuncState funcstate;
  LClosure *cl = luaF_newLclosure(L, 1); /* create main closure */
  setclLvalue2s(L, L->top, cl); /* anchor it (to avoid being collected) */
  luaD_inctop(L);
  lexstate.h = luaH_new(L);           /* create table for scanner */
  sethvalue2s(L, L->top, lexstate.h); /* anchor it */
  luaD_inctop(L);
  funcstate.f = cl->p = luaF_newproto(L);
  luaC_objbarrier(L, cl, cl->p);
  funcstate.f->source = luaS_new(L, name); /* create and anchor TString */
  luaC_objbarrier(L, funcstate.f, funcstate.f->source);
  lexstate.buff = buff;
  lexstate.dyd = dyd;
  dyd->actvar.n = dyd->gt.n = dyd->label.n = 0;
  luaX_setinput(L, &lexstate, z, funcstate.f->source, firstchar);
  mainfunc(&lexstate, &funcstate);
  lua_assert(!funcstate.prev && funcstate.nups == 1 && !lexstate.fs);
  /* all scopes should be correctly finished */
  lua_assert(dyd->actvar.n == 0 && dyd->gt.n == 0 && dyd->label.n == 0);
  L->top--;  /* remove scanner's table */
  return cl; /* closure is on the stack, too */
}
