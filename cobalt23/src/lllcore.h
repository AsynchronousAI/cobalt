/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef LLLCORE_H
#define LLLCORE_H

#include "lstate.h"
#include "lobject.h"

/* Compiles a function and attachs it to the proto
** In success returns 0, else returns 1
** If errmsg != NULL also returns the error message (must be freed) */
int LLLCompile (lua_State *L, Proto *p, char **errmsg);

/* Also compiles all children functions */
int LLLCompileAll (lua_State *L, Proto *p, char **errmsg);

/* Enables or disables the auto compilation */
void LLLSetAutoCompileEnable (int enable);

/* Returns whether the auto compilation is enable */
int LLLIsAutoCompileEnable();

/* Sets the number of calls required to auto-compile a function */
void LLLSetCallsToCompile (int calls);

/* Obtains the number of calls required to auto-compile a function */
int LLLGetCallsToCompile();

/* Returns whether the function is compiled */
int LLLIsCompiled (Proto *p);

/* Destroys the engine */
void LLLFreeEngine (lua_State *L, Proto *p);

/* Dumps the LLMV function (debug) */
void LLLDump (Proto *p);

/* Writes the bytecode and the asm of $p to $path (debug) */
void LLLWrite (Proto *p, const char *path);

#endif

