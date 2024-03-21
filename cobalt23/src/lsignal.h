#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include <signal.h>

#include "cobalt.h"
#include "lauxlib.h"

#ifndef lsig

#define lsig

struct lua_signal {
  char *name; /* name of the signal */
  int sig;    /* the signal */
};

#endif
#ifdef __cplusplus
}
#endif
