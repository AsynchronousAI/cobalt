/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

ERR_ENTRY ( OK                       )
ERR_ENTRY ( UNSUPPORTED_PROTOCOL     )
ERR_ENTRY ( FAILED_INIT              )
ERR_ENTRY ( URL_MALFORMAT            )
#if LCURL_CURL_VER_GE(7,21,5)
ERR_ENTRY ( NOT_BUILT_IN             )
#endif
ERR_ENTRY ( COULDNT_RESOLVE_PROXY    )
ERR_ENTRY ( COULDNT_RESOLVE_HOST     )
ERR_ENTRY ( COULDNT_CONNECT          )
#if LCURL_CURL_VER_GE(7,51,0)
ERR_ENTRY ( WEIRD_SERVER_REPLY       )
#else
ERR_ENTRY ( FTP_WEIRD_SERVER_REPLY   )
#endif
ERR_ENTRY ( REMOTE_ACCESS_DENIED     )
#if LCURL_CURL_VER_GE(7,31,0)
ERR_ENTRY ( FTP_ACCEPT_FAILED        )
#endif
ERR_ENTRY ( FTP_WEIRD_PASS_REPLY     )
#if LCURL_CURL_VER_GE(7,24,0)
ERR_ENTRY ( FTP_ACCEPT_TIMEOUT       )
#endif
ERR_ENTRY ( FTP_WEIRD_PASV_REPLY     )
ERR_ENTRY ( FTP_WEIRD_227_FORMAT     )
ERR_ENTRY ( FTP_CANT_GET_HOST        )
ERR_ENTRY ( FTP_COULDNT_SET_TYPE     )
ERR_ENTRY ( PARTIAL_FILE             )
ERR_ENTRY ( FTP_COULDNT_RETR_FILE    )
ERR_ENTRY ( OBSOLETE20               )
ERR_ENTRY ( QUOTE_ERROR              )
ERR_ENTRY ( HTTP_RETURNED_ERROR      )
ERR_ENTRY ( WRITE_ERROR              )
ERR_ENTRY ( OBSOLETE24               )
ERR_ENTRY ( UPLOAD_FAILED            )
ERR_ENTRY ( READ_ERROR               )
ERR_ENTRY ( OUT_OF_MEMORY            )
ERR_ENTRY ( OPERATION_TIMEDOUT       )
ERR_ENTRY ( OBSOLETE29               )
ERR_ENTRY ( FTP_PORT_FAILED          )
ERR_ENTRY ( FTP_COULDNT_USE_REST     )
ERR_ENTRY ( OBSOLETE32               )
ERR_ENTRY ( RANGE_ERROR              )
ERR_ENTRY ( HTTP_POST_ERROR          )
ERR_ENTRY ( SSL_CONNECT_ERROR        )
ERR_ENTRY ( BAD_DOWNLOAD_RESUME      )
ERR_ENTRY ( FILE_COULDNT_READ_FILE   )
ERR_ENTRY ( LDAP_CANNOT_BIND         )
ERR_ENTRY ( LDAP_SEARCH_FAILED       )
ERR_ENTRY ( OBSOLETE40               )
ERR_ENTRY ( FUNCTION_NOT_FOUND       )
ERR_ENTRY ( ABORTED_BY_CALLBACK      )
ERR_ENTRY ( BAD_FUNCTION_ARGUMENT    )
ERR_ENTRY ( OBSOLETE44               )
ERR_ENTRY ( INTERFACE_FAILED         )
ERR_ENTRY ( OBSOLETE46               )
ERR_ENTRY ( TOO_MANY_REDIRECTS       )
#if LCURL_CURL_VER_GE(7,21,5)
ERR_ENTRY ( UNKNOWN_OPTION           )
#else
ERR_ENTRY ( UNKNOWN_TELNET_OPTION    ) /* User specified an unknown option */
#endif
ERR_ENTRY ( TELNET_OPTION_SYNTAX     )
ERR_ENTRY ( OBSOLETE50               )
ERR_ENTRY ( PEER_FAILED_VERIFICATION )
ERR_ENTRY ( GOT_NOTHING              )
ERR_ENTRY ( SSL_ENGINE_NOTFOUND      )
ERR_ENTRY ( SSL_ENGINE_SETFAILED     )
ERR_ENTRY ( SEND_ERROR               )
ERR_ENTRY ( RECV_ERROR               )
ERR_ENTRY ( OBSOLETE57               )
ERR_ENTRY ( SSL_CERTPROBLEM          )
ERR_ENTRY ( SSL_CIPHER               )
#if LCURL_CURL_VER_GE(7,62,0)
ERR_ENTRY ( OBSOLETE51               )
#else
ERR_ENTRY ( SSL_CACERT               )
#endif
ERR_ENTRY ( BAD_CONTENT_ENCODING     )
ERR_ENTRY ( LDAP_INVALID_URL         )
ERR_ENTRY ( FILESIZE_EXCEEDED        )
ERR_ENTRY ( USE_SSL_FAILED           )
ERR_ENTRY ( SEND_FAIL_REWIND         )
ERR_ENTRY ( SSL_ENGINE_INITFAILED    )
ERR_ENTRY ( LOGIN_DENIED             )
ERR_ENTRY ( TFTP_NOTFOUND            )
ERR_ENTRY ( TFTP_PERM                )
ERR_ENTRY ( REMOTE_DISK_FULL         )
ERR_ENTRY ( TFTP_ILLEGAL             )
ERR_ENTRY ( TFTP_UNKNOWNID           )
ERR_ENTRY ( REMOTE_FILE_EXISTS       )
ERR_ENTRY ( TFTP_NOSUCHUSER          )
ERR_ENTRY ( CONV_FAILED              )
ERR_ENTRY ( CONV_REQD                )
ERR_ENTRY ( SSL_CACERT_BADFILE       )
ERR_ENTRY ( REMOTE_FILE_NOT_FOUND    )
ERR_ENTRY ( SSH                      )
ERR_ENTRY ( SSL_SHUTDOWN_FAILED      )
ERR_ENTRY ( AGAIN                    )
ERR_ENTRY ( SSL_CRL_BADFILE          )
ERR_ENTRY ( SSL_ISSUER_ERROR         )
#if LCURL_CURL_VER_GE(7,20,0)
ERR_ENTRY ( FTP_PRET_FAILED          )
#endif
#if LCURL_CURL_VER_GE(7,21,0)
ERR_ENTRY ( FTP_BAD_FILE_LIST        )
#endif
#if LCURL_CURL_VER_GE(7,20,0)
ERR_ENTRY ( RTSP_CSEQ_ERROR          )
ERR_ENTRY ( RTSP_SESSION_ERROR       )
#endif
#if LCURL_CURL_VER_GE(7,21,0)
ERR_ENTRY ( CHUNK_FAILED             )
#endif
#if LCURL_CURL_VER_GE(7,30,0)
ERR_ENTRY ( NO_CONNECTION_AVAILABLE  )
#endif
#if LCURL_CURL_VER_GE(7,38,0)
ERR_ENTRY ( HTTP2                    )
#else
ERR_ENTRY ( OBSOLETE16               )
#endif
#if LCURL_CURL_VER_GE(7,39,0)
ERR_ENTRY ( SSL_PINNEDPUBKEYNOTMATCH )
#endif
#if LCURL_CURL_VER_GE(7,41,0)
ERR_ENTRY ( SSL_INVALIDCERTSTATUS    )
#endif
#if LCURL_CURL_VER_GE(7,49,0)
ERR_ENTRY ( HTTP2_STREAM             )
#endif
#if LCURL_CURL_VER_GE(7,59,0)
ERR_ENTRY ( RECURSIVE_API_CALL       )
#endif
#if LCURL_CURL_VER_GE(7,66,0)
ERR_ENTRY ( AUTH_ERROR               )
#endif
#if LCURL_CURL_VER_GE(7,68,0)
ERR_ENTRY ( HTTP3                    )
#endif
#if LCURL_CURL_VER_GE(7,69,0)
ERR_ENTRY ( QUIC_CONNECT_ERROR       )
#endif
#if LCURL_CURL_VER_GE(7,73,0)
ERR_ENTRY ( PROXY                    )
#endif
