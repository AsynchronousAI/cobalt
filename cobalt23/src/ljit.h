/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

/*
JIT functions for Cobalt
*/

#include "jit.h"

int context_init(){
    jit_context_t context = jit_context_create();
    jit_context_build_start(context);
    jit_type_t signature = jit_type_create_signature(jit_abi_cdecl, jit_type_int, NULL, 0, 1);
    jit_function_t function = jit_function_create(context, signature);
    jit_value_t param = jit_value_get_param(function, 0);
    jit_value_t constant = jit_value_create_nint_constant(function, jit_type_int, 42);
    jit_value_t result = jit_insn_add(function, param, constant);
    jit_insn_return(function, result);
    jit_function_compile(function);
    jit_context_build_end(context);
    int (*func)(int) = (int (*)(int))jit_function_to_closure(function);
    int result = func(1);
    jit_context_destroy(context);
    return result;
}