#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: no argument provided\n");
        return 1;
    }

    // Allocate a buffer for the command
    char command[256];
    snprintf(command, sizeof(command), "cobalt -e 'import(\"preprocess\")(\"%s\", \"file\", true)'", argv[1]);

    // Execute the command in the terminal
    int status = system(command);

    // Check if the command executed successfully
    if (status != 0) {
        printf("Error: command failed with status %d\n", status);
        return 1;
    }

    return 0;
}