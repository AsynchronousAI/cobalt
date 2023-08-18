#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // Construct the command to run the Cobalt script
    char *command = "cobalt -e 'import('preprocess')('test.cobalt', 'file', 'out.cobalt')'";

    // Execute the command in the terminal
    int status = system(command);

    // Check if the command executed successfully
    if (status != 0) {
        printf("Error: command failed with status %d\n", status);
        return 1;
    }

    return 0;
}