#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef llex_h
#define llex_h

#include <limits.h>
#include "lobject.h"
#include "lzio.h"

/*
** Single-char tokens (terminal symbols) are represented by their own
** numeric code. Other tokens start at the following value.
*/
#define FIRST_RESERVED (UCHAR_MAX + 1)

#if !defined(LUA_ENV)
#define LUA_ENV "_ENV"
#endif

/*
 * WARNING: if you change the order of this enumeration,
 * grep "ORDER RESERVED"
 */
enum RESERVED {
  /* terminal symbols denoted by reserved words */
  TK_AUTO = FIRST_RESERVED,
  TK_BREAK,
  TK_CONTINUE,
  TK_DO,
  TK_ELSE,
  TK_FALSE,
  TK___FILE__,
  TK_FOR,
  TK_FUNCTION,
  TK_GOTO,
  TK_IF,
  TK_IN,
  TK_LET,
  TK___LINE__,
  TK_LOCAL,
  TK_NIL,
  TK_RETURN,
  TK_TRUE,
  TK_VAR,
  TK_CASE, 
  TK_DEFAULT, 
  TK_AS,  
  TK_EXTENDS, 
  TK_INSTANCEOF,
  TK_SWITCH, 
  TK_ENUM, 
  TK_NEW, 
  TK_CLASS,
  TK_PARENT,
  TK_DEFER, 
  TK_EXPORT,
  TK_CONFIG,
  TK_WHILE,

  /* other terminal symbols */
  TK_WALRUS, /* highest priority */
  TK_AND,
  TK_NOT,
  TK_OR,
  TK_POW,
  TK_IDIV,
  TK_CONCAT,
  TK_DOTS,
  TK_EQ,
  TK_GE,
  TK_LE,
  TK_NE,
  TK_CADD,
  TK_CSUB,
  TK_CMUL,
  TK_CDIV,
  TK_CMOD,
  TK_CCONCAT,
  TK_PLUSPLUS,
  TK_MINUSMINUS,
  TK_SHL,
  TK_SHR,
  TK_ARROW,
  TK_DBCOLON,
  TK_EOS,
  TK_FLT,
  TK_INT,
  TK_NAME,
  TK_STRING,
  TK_COMPONENT,
  TK_COAL,
};

/* number of reserved words */
#define NUM_RESERVED (cast_int(TK_WHILE - FIRST_RESERVED))

typedef union {
  lua_Number r;
  lua_Integer i;
  TString *ts;
} SemInfo; /* semantics information */

typedef struct Token {
  int token;
  SemInfo seminfo;
} Token;

typedef struct EnumDesc {
  struct Enumerator {
    TString* name;
    lua_Integer value;
  };
  std::vector<Enumerator> enumerators;
};


/* state of the lexer plus state of the parser when shared by all
   functions */
typedef struct LexState {
  int current;          /* current character (charint) */
  int linenumber;       /* input line counter */
  int lastline;         /* line of last token 'consumed' */
  Token t;              /* current token */
  Token lookahead;      /* look ahead token */
  struct FuncState *fs; /* current function (parser) */
  struct lua_State *L;
  ZIO *z;              /* input stream */
  Mbuffer *buff;       /* buffer for tokens */
  Table *h;            /* to avoid collection/reuse strings */
  struct Dyndata *dyd; /* dynamic structures used by the parser */
  TString *source;     /* current source name */
  TString *envn;       /* environment variable name */

  /* added in cobalt */
  std::vector<EnumDesc> enums{};
  std::stack<TString*> parent_classes{};
  std::vector<TString*> export_symbols;
  
  /** configurations */
  bool strict_type_config=false;
  bool check_type=false;

  /** methods */
  [[nodiscard]] TString* getParentClass() const noexcept {
    if (parent_classes.empty())
      return nullptr;
    return parent_classes.top();
  }
} LexState;

LUAI_FUNC void luaX_init(lua_State *L);
LUAI_FUNC void luaX_setinput(lua_State *L, LexState *ls, ZIO *z,
                             TString *source, int firstchar);
LUAI_FUNC TString *luaX_newstring(LexState *ls, const char *str, size_t l);
LUAI_FUNC void luaX_next(LexState *ls);
LUAI_FUNC void luaX_prev(LexState *ls);
LUAI_FUNC int luaX_lookahead(LexState *ls);
LUAI_FUNC l_noret luaX_syntaxerror(LexState *ls, const char *s, const char *errid = nullptr);
LUAI_FUNC l_noret luaX_notedsyntaxerror(LexState *ls, const char *s, const char *msg, const char *errid = nullptr);
LUAI_FUNC void luaX_syntaxwarning(LexState *ls, const char *s);
LUAI_FUNC const char *luaX_token2str(LexState *ls, int token);

#endif

#ifdef __cplusplus
}
#endif
