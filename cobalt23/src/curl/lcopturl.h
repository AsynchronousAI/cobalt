/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

ENTRY_PART(fragment,      UPART_FRAGMENT , CURLUE_NO_FRAGMENT     )
ENTRY_PART(host,          UPART_HOST     , CURLUE_NO_HOST         )
ENTRY_PART(options,       UPART_OPTIONS  , CURLUE_NO_OPTIONS      )
ENTRY_PART(password,      UPART_PASSWORD , CURLUE_NO_PASSWORD     )
ENTRY_PART(path,          UPART_PATH     , CURLUE_OK              )
ENTRY_PART(port,          UPART_PORT     , CURLUE_NO_PORT         )
ENTRY_PART(query,         UPART_QUERY    , CURLUE_NO_QUERY        )
ENTRY_PART(scheme,        UPART_SCHEME   , CURLUE_NO_SCHEME       )
ENTRY_PART(url,           UPART_URL      , CURLUE_OK              )
ENTRY_PART(user,          UPART_USER     , CURLUE_NO_USER         )

#if LCURL_CURL_VER_GE(7,65,0)
ENTRY_PART(zoneid,        UPART_ZONEID   , CURLUE_UNKNOWN_PART    )
#endif

ENTRY_FLAG(DEFAULT_PORT        )
ENTRY_FLAG(NO_DEFAULT_PORT     )
ENTRY_FLAG(DEFAULT_SCHEME      )
ENTRY_FLAG(NON_SUPPORT_SCHEME  )
ENTRY_FLAG(PATH_AS_IS          )
ENTRY_FLAG(DISALLOW_USER       )
ENTRY_FLAG(URLDECODE           )
ENTRY_FLAG(URLENCODE           )
ENTRY_FLAG(APPENDQUERY         )
ENTRY_FLAG(GUESS_SCHEME        )

#if LCURL_CURL_VER_GE(7,67,0)
ENTRY_FLAG(NO_AUTHORITY        )
#endif
