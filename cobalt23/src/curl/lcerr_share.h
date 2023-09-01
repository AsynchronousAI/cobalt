/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

ERR_ENTRY ( OK            )
ERR_ENTRY ( BAD_OPTION    )
ERR_ENTRY ( IN_USE        )
ERR_ENTRY ( INVALID       )
ERR_ENTRY ( NOMEM         )
#if LCURL_CURL_VER_GE(7,23,0)
ERR_ENTRY ( NOT_BUILT_IN  )
#endif
