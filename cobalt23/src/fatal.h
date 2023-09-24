/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

/*
Handles fatal errors
*/

#define FATAL_ERR "\033[1;31mfatal error:\033[0m a fatal error has occured, on line %d in file %s\nthis is likely caused by an unhandled error in the\ncobalt interpreter please report this to the Cobalt GitHub repository (ERR: INT%d)\n"

#ifdef LUA_USE_POSIX || defined(_unix_) || defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__MACH__) || defined(__linux)
#include <signal.h>
int line = 0;
const char *file = NULL;

void fault_handler(int signal, siginfo_t *info, void *context) {
  printf(FATAL_ERR, line, file, signal);
  exit(1);
}

int handle(int line, const char *file) {
  struct sigaction sa;
  sa.sa_sigaction = segfault_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGSEGV, &sa, NULL);

  return 0;
}
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <stdio.h>
int line = 0;
const char *file = NULL;

LONG WINAPI fault_handler(EXCEPTION_POINTERS *ExceptionInfo) {
  printf(FATAL_ERR, line, file, signal);
  exit(1);
}

int handle(int line, const char *file) {
  SetUnhandledExceptionFilter(segfault_handler);

  return 0;
}
#endif