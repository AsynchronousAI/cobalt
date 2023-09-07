/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef _LCURL_H_
#define _LCURL_H_

#include <assert.h>
#include <string.h>

#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/multi.h"
#include "l52util.h"

#define LCURL_PREFIX "cURL"

#define LCURL_LUA_REGISTRY lua_upvalueindex(1)

#define LCURL_USERVALUES lua_upvalueindex(2)

/* only for `mime` API */
#define LCURL_MIME_EASY lua_upvalueindex(3)

#endif
