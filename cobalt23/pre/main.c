/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char command[256] = "cobalt -e\"require('preprocess')->interface(";

  // Iterate through the arguments and append them to the command string
  for (int i = 1; i < argc; i++) {
      strcat(command, "'");
      strcat(command, argv[i]);
      strcat(command, "', ");
  }

  // Close the command string with a double quote
  strcat(command, ")\"");

  // Execute the command using the system function
  if (system(command) == 0){
    return 0;
  }else{
    exit(1);
  }
  return 0;
}