/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Error: No arguments\n");
    return 1;
  }
  // Allocate a buffer for the command
  char command[256];

  if (argc == 2) {
    // Format the command
    snprintf(command, sizeof(command),
             "cobalt -e 'import(\"preprocess\")(\"%s\", \"file\", true)'",
             argv[1]);
    // printf("%s\n", command);
  } else {
    // Make a new string, formatted_args, to hold the arguments beyond the first
    // combined into a single string
    char *formatted_args = malloc(1);
    formatted_args[0] = '\0';
    // Loop through the arguments beyond the first
    for (int i = 2; i < argc; i++) {
      // Make a new string, formatted_arg, to hold the current argument
      char *formatted_arg = malloc(strlen(argv[i]) + 3);
      // Copy the current argument into formatted_arg
      strcpy(formatted_arg, "\"");
      strcat(formatted_arg, argv[i]);
      strcat(formatted_arg, "\"");
      // Concatenate formatted_arg onto formatted_args
      formatted_args = realloc(
          formatted_args, strlen(formatted_args) + strlen(formatted_arg) + 2);
      strcat(formatted_args, formatted_arg);
      strcat(formatted_args, ", ");
      // Free formatted_arg
      free(formatted_arg);
    }
    // Remove the trailing ", " from formatted_args
    formatted_args[strlen(formatted_args) - 2] = '\0';

    // Format the command
    snprintf(command, sizeof(command),
             "cobalt -e 'import(\"preprocess\")(\"%s\", \"file\", true, %s)'",
             argv[1], formatted_args);
    // printf("%s\n", command);
  }
  // Execute the command in the terminal
  int status = system(command);

  // Check if the command executed successfully
  if (status != 0) {
    printf("Error: command failed with status %d\n", status);
    return 1;
  }

  return 0;
}