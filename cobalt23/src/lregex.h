/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef _TINY_REGEX_C
#define _TINY_REGEX_C

#ifndef RE_DOT_MATCHES_NEWLINE
/* Define to 0 if you DON'T want '.' to match '\r' + '\n' */
#define RE_DOT_MATCHES_NEWLINE 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Typedef'd pointer to get abstract datatype. */
typedef struct regex_t* re_t;

/* Compile regex string pattern to a regex_t-array. */
re_t re_compile(const char* pattern);

/* Find matches of the compiled pattern inside text. */
int re_matchp(re_t pattern, const char* text, int* matchlength);

/* Find matches of the txt pattern inside text (will compile automatically
 * first). */
int re_match(const char* pattern, const char* text, int* matchlength);

#ifdef __cplusplus
}
#endif

#endif /* ifndef _TINY_REGEX_C */