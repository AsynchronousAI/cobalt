/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#ifndef _LCHTTPPOST_H_
#define _LCHTTPPOST_H_

#include <stdlib.h>

#include "lcurl.h"
#include "lcutils.h"

#define LCURL_HPOST_STREAM_MAGIC 0xAA

typedef struct lcurl_hpost_stream_tag {
  unsigned char magic;

  lua_State **L;
  lcurl_callback_t rd;
  lcurl_read_buffer_t rbuffer;
  struct lcurl_hpost_stream_tag *next;
} lcurl_hpost_stream_t;

typedef struct lcurl_hpost_tag {
  lua_State *L;
  struct curl_httppost *post;
  struct curl_httppost *last;
  int storage;
  int err_mode;
  lcurl_hpost_stream_t *stream;
} lcurl_hpost_t;

int lcurl_hpost_create(lua_State *L, int error_mode);

void lcurl_hpost_initlib(lua_State *L, int nup);

lcurl_hpost_t *lcurl_gethpost_at(lua_State *L, int i);

#define lcurl_gethpost(L) lcurl_gethpost_at((L), 1)

#endif
