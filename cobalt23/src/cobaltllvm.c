/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#define AOT_USE_GOTOS

#undef LUAI_ASSERT

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cobalt.h"
#include "lauxlib.h"
#include "ldebug.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lopnames.h"
#include "lstate.h"
#include "lundump.h"

//
// Command-line arguments and main function
// ----------------------------------------
//

static const char *program_name = "cobaltllvm";
static char *input_filename = NULL;
static char *output_filename = NULL;
static char *provided_input_filename = NULL;

int preprocessor = 1;

static FILE *output_file = NULL;
static int nfunctions = 0;
static TString **tmname;

static void usage() {
  fprintf(stderr,
          "usage: %s [options] [filename]\nAvailable options are:\n  -o name  "
          "output to file 'name'\n  -p       do not run the preprocessor on "
          "the input\n  -i name  preprocess to file 'name'\n  -D name  provide "
          "'name' to the preprocessor\n  -e       add a main symbol for executables\n",
          program_name);
}

static void fatal_error(const char *msg) {
  fprintf(stderr, "%s: %s\n", program_name, msg);
  exit(1);
}

static __attribute__((format(printf, 1, 2))) void print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(output_file, fmt, args);
  va_end(args);
}

static __attribute__((format(printf, 1, 2))) void println(const char *fmt,
                                                          ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(output_file, fmt, args);
  va_end(args);
  fprintf(output_file, "\n");
}

static void printnl() {
  // This separate function avoids Wformat-zero-length warnings with println
  fprintf(output_file, "\n");
}

static void doargs(int argc, char **argv) {
  program_name = argv[0];

  int do_opts = 1;
  int npos = 0;
  for (int i = 1; i < argc; i++) {
    char *arg = argv[i];
    if (do_opts && arg[0] == '-') {
      if (0 == strcmp(arg, "--")) {
        do_opts = 0;
      } else if (0 == strcmp(arg, "-h")) {
        usage();
        exit(0);
      } else if (0 == strcmp(arg, "-o")) {
        i++;
        if (i >= argc) {
          fatal_error("missing argument for -o");
        }
        output_filename = argv[i];
      } else if (0 == strcmp(arg, "-p")) {
        preprocessor = 0;
      } else if (0 == strcmp(arg, "-i")) {
        i++;
        if (i >= argc) {
          fatal_error("missing argument for -i");
        }
        provided_input_filename = argv[i];
      } else if (0 == strcmp(arg, "-D")) {
        fatal_error(
            "inputting preprocessor definitions not supported yet. use "
            "cobaltpre to input preprocessor definitions.");
      } else {
        fprintf(stderr, "unknown option %s\n", arg);
        exit(1);
      }
    } else {
      switch (npos) {
        case 0:
          input_filename = arg;
          break;
        default:
          fatal_error("too many positional arguments");
          break;
      }
      npos++;
    }
  }

  if (output_filename == NULL) {
    usage();
    exit(1);
  }
}

static void replace_dots(char *);
static void print_functions();
static void print_source_code();

int main(int argc, char **argv) {
  // Process input arguments

  doargs(argc, argv);

  // Run preprocessor
  char process[256];
  if (preprocessor == 1){
    char command[256];
    // set process to input_filename+.ii
    if (provided_input_filename == NULL) {
      strcpy(process, input_filename);
      strcat(process, ".cii");
    } else {
      strcpy(process, provided_input_filename);
    }

    snprintf(command, sizeof(command),
            "cobalt -e 'import(\"preprocess\")->Interface(\"%s\", \"-o\", \"%s\")'",
            input_filename, process);
    // printf(command);
    FILE *fp;
    char path[1035];
    fp = popen(command, "r");
    if (fp == NULL) {
      printf("Failed to run preprocessor\n");
      exit(1);
    }
    while (fgets(path, sizeof(path) - 1, fp) != NULL) {
      printf("%s", path);
    }
    pclose(fp);
  }
  // Read the input

  lua_State *L = luaL_newstate();
  if (luaL_loadfile(L, preprocessor == 1 ? process : input_filename) != LUA_OK) {
    fatal_error(lua_tostring(L, -1));
  }
  Proto *proto = getproto(s2v(L->top - 1));
  tmname = G(L)->tmname;

  // Generate the file

  output_file = fopen(output_filename, "w");
  if (output_file == NULL) {
    fatal_error(strerror(errno));
  }

  // WRITE CODE
}