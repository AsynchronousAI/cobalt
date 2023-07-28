// Converts lua bytecode to luasm.
// Lua bytecode vs luasm

// Lua bytecode stores function and variable names.
// Luasm does not, it stores the index of the function or variable.

// Lua bytecode does not store any memory management information, it requires on the runtime gc.
// Luasm uses a borrow checker to ensure that all memory is freed.

// @AsynchronousAI - 2023 

#include <string.h>

//////////////////////////
static int parseBytecode(char[] bytecode) {
    
}