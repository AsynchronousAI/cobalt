/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

ERR_ENTRY ( OK                 )
ERR_ENTRY ( CALL_MULTI_PERFORM )
ERR_ENTRY ( BAD_HANDLE         )
ERR_ENTRY ( BAD_EASY_HANDLE    )
ERR_ENTRY ( OUT_OF_MEMORY      )
ERR_ENTRY ( INTERNAL_ERROR     )
ERR_ENTRY ( BAD_SOCKET         )
ERR_ENTRY ( UNKNOWN_OPTION     )
#if LCURL_CURL_VER_GE(7,32,1)
ERR_ENTRY ( ADDED_ALREADY      )
#endif
#if LCURL_CURL_VER_GE(7,59,0)
ERR_ENTRY ( RECURSIVE_API_CALL       )
#endif
