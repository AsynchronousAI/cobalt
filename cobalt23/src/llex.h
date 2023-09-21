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
  TK_WHILE,
  /* other terminal symbols */
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
  /* added in cobalt: */
  TK_CASE, 
  TK_DEFAULT, 
  TK_AS, 
  TK_BEGIN, 
  TK_EXTENDS, 
  TK_INSTANCEOF,
  TK_SWITCH, 
  TK_ENUM, 
  TK_NEW, 
  TK_CLASS,
  TK_PARENT, 
  TK_EXPORT,
  TK_COAL,
  TK_WALRUS,
  TK_FSTRING,
  TK_FORMAT,
};

/* number of reserved words */
#define NUM_RESERVED (cast_int(TK_WHILE - FIRST_RESERVED + 1))

typedef union {
  lua_Number r;
  lua_Integer i;
  TString *ts;
} SemInfo; /* semantics information */

typedef struct Token {
  int token;
  SemInfo seminfo;
} Token;

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
} LexState;

LUAI_FUNC void luaX_init(lua_State *L);
LUAI_FUNC void luaX_setinput(lua_State *L, LexState *ls, ZIO *z,
                             TString *source, int firstchar);
LUAI_FUNC TString *luaX_newstring(LexState *ls, const char *str, size_t l);
LUAI_FUNC void luaX_next(LexState *ls);
LUAI_FUNC int luaX_lookahead(LexState *ls);
LUAI_FUNC l_noret luaX_syntaxerror(LexState *ls, const char *s);
LUAI_FUNC l_noret luaX_notedsyntaxerror(LexState *ls, const char *s, const char *msg);
LUAI_FUNC void luaX_syntaxwarning(LexState *ls, const char *s);
LUAI_FUNC const char *luaX_token2str(LexState *ls, int token);

#endif
