/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

/*
* Autogenerates builtin operators.
* in Cobalt that are not written in Cobalt.
* this is run on Compile-time not runtime
* and only imports needed operators.
*/
#define TK_END '}'
#define TK_DO '{'
#define TK_BEGIN '{'
#define TK_THEN '{'

#ifdef __cplusplus
extern "C" {
#endif

static void builtinoperators (LexState *ls) {
  bool uses_new = false;
  bool uses_extends = false;
  bool uses_instanceof = false;

  /* discover what operators are used */
  for (const auto& t : ls->tokens) {
    switch (t.token) {
      case TK_NEW:
        uses_new = true;
        break;
      case TK_EXTENDS:
        uses_extends = true;
        break;
      case TK_INSTANCEOF:
        uses_instanceof = true;
        break;
    }
  }

  /* inject implementers */
  if (uses_new || uses_extends || uses_instanceof) {
    /* capture state */
    std::vector<Token> tokens = std::move(ls->tokens);

    ls->tokens = {}; /* avoid use of moved warning */

    if (uses_new) {
      // local function BUILTINOP_new(mt, ...)
      ls->tokens.emplace_back(Token(TK_VAR));
      ls->tokens.emplace_back(Token(TK_FUNCTION));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "BUILTINOP_new")));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mt")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_DOTS));
      ls->tokens.emplace_back(Token(')'));
      ls->tokens.emplace_back(Token('{'));

      //   if type(mt) ~= "table" then
      ls->tokens.emplace_back(Token(TK_IF));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "type")));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mt")));
      ls->tokens.emplace_back(Token(')'));
      ls->tokens.emplace_back(Token(TK_NE));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "table")));
      ls->tokens.emplace_back(Token(')'));
      ls->tokens.emplace_back(Token(TK_THEN));

      //     error "'new' used on non-table value"
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "error")));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "'new' used on non-table value")));

      //   end
      ls->tokens.emplace_back(Token(TK_END));

      //   local t = {}
      ls->tokens.emplace_back(Token(TK_VAR));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "t")));
      ls->tokens.emplace_back(Token('='));
      ls->tokens.emplace_back(Token('{'));
      ls->tokens.emplace_back(Token('}'));

      //   setmetatable(t, mt)
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "setmetatable")));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "t")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mt")));
      ls->tokens.emplace_back(Token(')'));

      //   if not mt.__index or mt.__parent then
      ls->tokens.emplace_back(Token(TK_IF));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NOT));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mt")));
      ls->tokens.emplace_back(Token('.'));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "__index")));
      ls->tokens.emplace_back(Token(TK_OR));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mt")));
      ls->tokens.emplace_back(Token('.'));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "__parent")));
      ls->tokens.emplace_back(Token(')'));
      ls->tokens.emplace_back(Token(TK_THEN));

      //     mt.__index = mt
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mt")));
      ls->tokens.emplace_back(Token('.'));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "__index")));
      ls->tokens.emplace_back(Token('='));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mt")));

      //   end
      ls->tokens.emplace_back(Token(TK_END));

      //   if t.__construct then
      ls->tokens.emplace_back(Token(TK_IF));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "t")));
      ls->tokens.emplace_back(Token('.'));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "__construct")));
      ls->tokens.emplace_back(Token(')'));
      ls->tokens.emplace_back(Token(TK_THEN));

      //     t:__construct(...)
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "t")));
      ls->tokens.emplace_back(Token(':'));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "__construct")));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_DOTS));
      ls->tokens.emplace_back(Token(')'));

      //   end
      ls->tokens.emplace_back(Token(TK_END));

      //   return t
      ls->tokens.emplace_back(Token(TK_RETURN));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "t")));

      // end
      ls->tokens.emplace_back(Token(TK_END));
    }
    if (uses_extends) {
      // local function BUILTINOP_extends(c, p)
      ls->tokens.emplace_back(Token(TK_VAR));
      ls->tokens.emplace_back(Token(TK_FUNCTION));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "BUILTINOP_extends")));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "c")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "p")));
      ls->tokens.emplace_back(Token(')'));
      ls->tokens.emplace_back(Token('{'));

      //   for { ... } as mm do
      ls->tokens.emplace_back(Token(TK_FOR));
      ls->tokens.emplace_back(Token('{'));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__gc")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__mode")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__len")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__eq")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__add")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__sub")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__mul")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__mod")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__pow")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__div")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__idiv")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__band")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__bor")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__bxor")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__shl")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__shr")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__unm")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__bnot")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__lt")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__le")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__concat")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__call")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__close")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_STRING, luaX_newliteral(ls, "__tostring")));
      ls->tokens.emplace_back(Token('}'));
      ls->tokens.emplace_back(Token(TK_AS));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mm")));
      ls->tokens.emplace_back(Token(TK_DO));

      //     if p[mm] and not c[mm] then
      ls->tokens.emplace_back(Token(TK_IF));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "p")));
      ls->tokens.emplace_back(Token('['));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mm")));
      ls->tokens.emplace_back(Token(']'));
      ls->tokens.emplace_back(Token(TK_AND));
      ls->tokens.emplace_back(Token(TK_NOT));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "c")));
      ls->tokens.emplace_back(Token('['));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mm")));
      ls->tokens.emplace_back(Token(']'));
      ls->tokens.emplace_back(Token(')'));
      ls->tokens.emplace_back(Token(TK_THEN));

      //      c[mm] = p[mm]
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "c")));
      ls->tokens.emplace_back(Token('['));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mm")));
      ls->tokens.emplace_back(Token(']'));
      ls->tokens.emplace_back(Token('='));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "p")));
      ls->tokens.emplace_back(Token('['));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mm")));
      ls->tokens.emplace_back(Token(']'));

      //     end
      ls->tokens.emplace_back(Token(TK_END));
      
      //   end
      ls->tokens.emplace_back(Token(TK_END));

      //   setmetatable(c, { __index = p })
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "setmetatable")));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "c")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token('{'));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "__index")));
      ls->tokens.emplace_back(Token('='));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "p")));
      ls->tokens.emplace_back(Token('}'));
      ls->tokens.emplace_back(Token(')'));

      //   c.__parent = p
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "c")));
      ls->tokens.emplace_back(Token('.'));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "__parent")));
      ls->tokens.emplace_back(Token('='));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "p")));

      // end
      ls->tokens.emplace_back(Token(TK_END));
    }
    if (uses_instanceof) {
      // local function BUILTINOP_instanceof(t, mt)
      ls->tokens.emplace_back(Token(TK_VAR));
      ls->tokens.emplace_back(Token(TK_FUNCTION));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "BUILTINOP_instanceof")));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "t")));
      ls->tokens.emplace_back(Token(','));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mt")));
      ls->tokens.emplace_back(Token(')'));
      ls->tokens.emplace_back(Token('{'));

      //   t = getmetatable(t)
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "t")));
      ls->tokens.emplace_back(Token('='));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "getmetatable")));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "t")));
      ls->tokens.emplace_back(Token(')'));

      //   while t do
      ls->tokens.emplace_back(Token(TK_WHILE));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "t")));
      ls->tokens.emplace_back(Token(')'));
      ls->tokens.emplace_back(Token(TK_DO));

      //     if t == mt then
      ls->tokens.emplace_back(Token(TK_IF));
      ls->tokens.emplace_back(Token('('));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "t")));
      ls->tokens.emplace_back(Token(TK_EQ));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "mt")));
      ls->tokens.emplace_back(Token(')'));
      ls->tokens.emplace_back(Token(TK_THEN));

      //       return true
      ls->tokens.emplace_back(Token(TK_RETURN));
      ls->tokens.emplace_back(Token(TK_TRUE));

      //     end
      ls->tokens.emplace_back(Token(TK_END));

      //     t = t.__parent
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "t")));
      ls->tokens.emplace_back(Token('='));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "t")));
      ls->tokens.emplace_back(Token('.'));
      ls->tokens.emplace_back(Token(TK_NAME, luaX_newliteral(ls, "__parent")));

      //   end
      ls->tokens.emplace_back(Token(TK_END));

      //   return false
      ls->tokens.emplace_back(Token(TK_RETURN));
      ls->tokens.emplace_back(Token(TK_FALSE));

      // end
      ls->tokens.emplace_back(Token(TK_END));
    }

    /* parse */
    ls->tokens.emplace_back(Token(TK_EOS));
    luaX_next(ls);
    statlist(ls);

    /* reset state */
    ls->tidx = -1;
    ls->tokens = std::move(tokens);
  }
}

#ifdef __cplusplus
}
#endif