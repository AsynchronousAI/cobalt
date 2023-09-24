/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

/*
Handles fatal errors
*/
#define FATAL_ERR "\033[1m%s: \033[31mfatal error:\033[0m a fatal error has occured, on line %d \n (ERR: M%d)\n"
#define FATAL_ERR_MANUAL "\033[1m%s: \033[1;31mfatal error:\033[0m an abort has been raised by the program\n"
#ifdef __cplusplus
extern "C" {
#endif
int handlefatal(int line, const char file[30]);
#ifdef __cplusplus
}
#endif