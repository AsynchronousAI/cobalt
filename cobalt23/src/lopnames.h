#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#if !defined(lopnames_h)
#define lopnames_h

#include <stddef.h>

/* ORDER OP */

static const char *const opnames[] = {
    "MOVE",       "LOADI",    "LOADF",    "LOADK",      "LOADKX",   "LOADFALSE",
    "LFALSESKIP", "LOADTRUE", "LOADNIL",  "GETUPVAL",   "SETUPVAL", "GETTABUP",
    "GETTABLE",   "GETI",     "GETFIELD", "SETTABUP",   "SETTABLE", "SETI",
    "SETFIELD",   "NEWTABLE", "SELF",     "ADDI",       "ADDK",     "SUBK",
    "MULK",       "MODK",     "POWK",     "DIVK",       "IDIVK",    "BANDK",
    "BORK",       "BXORK",    "SHRI",     "SHLI",       "ADD",      "SUB",
    "MUL",        "MOD",      "POW",      "DIV",        "IDIV",     "BAND",
    "BOR",        "BXOR",     "SHL",      "SHR",        "MMBIN",    "MMBINI",
    "MMBINK",     "UNM",      "BNOT",     "NOT",        "LEN",      "CONCAT",
    "CLOSE",      "TBC",      "JMP",      "EQ",         "LT",       "LE",
    "EQK",        "EQI",      "LTI",      "LEI",        "GTI",      "GEI",
    "TEST",       "TESTSET",  "CALL",     "TAILCALL",   "RETURN",   "RETURN0",
    "RETURN1",    "FORLOOP",  "FORPREP",  "TFORPREP",   "TFORCALL", "TFORLOOP",
    "SETLIST",    "CLOSURE",  "DEFER",    "VARARG",     "VARARGPREP","EXTRAARG", 
    NULL
};

#endif

#ifdef __cplusplus
}
#endif
