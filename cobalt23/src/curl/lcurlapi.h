/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef _LURL_H_
#define _LURL_H_

#include <stdlib.h>

#include "lcurl.h"
#include "lcutils.h"

void lcurl_url_initlib(lua_State *L, int nup);

#if LCURL_CURL_VER_GE(7, 62, 0)

typedef struct lcurl_url_tag {
  CURLU *url;

  int err_mode;
} lcurl_url_t;

int lcurl_url_create(lua_State *L, int error_mode);

lcurl_url_t *lcurl_geturl_at(lua_State *L, int i);

#endif

#endif