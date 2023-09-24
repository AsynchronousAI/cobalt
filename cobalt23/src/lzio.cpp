/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#define lzio_c
#define LUA_CORE

#include "lzio.h"

#include <string.h>

#include "cobalt.h"
#include "llimits.h"
#include "lmem.h"
#include "lprefix.h"
#include "lstate.h"

int luaZ_fill(ZIO *z) {
  size_t size;
  lua_State *L = z->L;
  const char *buff;
  #ifdef QUICK_FILL
  if (z->eoz) return EOZ;
  #endif
  lua_unlock(L);
  buff = z->reader(L, z->data, &size);
  lua_lock(L);
  #ifndef QUICK_FILL
  if (buff == NULL || size == 0) return EOZ;
  #else
  if (buff == NULL || size == 0) {
     z->eoz = 1;  /* avoid calling reader function next time */
     return EOZ;
  }
  #endif
  z->n = size - 1; /* discount char being returned */
  z->p = buff;
  #ifdef QUICK_FILL
  z->eoz = 0;
  #endif
  return cast_uchar(*(z->p++));
}

void luaZ_init(lua_State *L, ZIO *z, lua_Reader reader, void *data) {
  z->L = L;
  z->reader = reader;
  z->data = data;
  z->n = 0;
  z->p = NULL;
}

/* --------------------------------------------------------------- read --- */
size_t luaZ_read(ZIO *z, void *b, size_t n) {
  while (n) {
    size_t m;
    if (z->n == 0) {           /* no bytes in buffer? */
      if (luaZ_fill(z) == EOZ) /* try to read more */
        return n; /* no more input; return number of missing bytes */
      else {
        z->n++; /* luaZ_fill consumed first byte; put it back */
        z->p--;
      }
    }
    m = (n <= z->n) ? n : z->n; /* min. between n and z->n */
    memcpy(b, z->p, m);
    z->n -= m;
    z->p += m;
    b = (char *)b + m;
    n -= m;
  }
  return 0;
}
