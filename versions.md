# Versions:

***
# Compilers and Interpreters:
## cobalt 
> **Type:** (Interpreter and Bytecoder) 
> **Binaries:** `cobalt` `cobaltc`

## cobaltjit 
> **Type:** (Optional JIT Compiler) 
> **Binary:** `cobaltjit`
Optional tool used for JIT compilation. It can be compiled using
`make cobaltjit` command. It is slower than Raptor. It is 
a inbetween of the interpreter and Raptor.

## cobalt-raptor
> **Type:** (Optional Ultrafast JIT Compiler) 
> **Binary:** `cobaltraptor`
Optional tool used for ultrafast JIT compilation. It can be compiled using
`make cobaltraptor` command. It is faster than CobaltJIT, and 
compatible with it. It also includes more tools to 
optimize the code. It is the fastest Cobalt compiler.

***
# Other:
## lua-cobalt 
> **Type:** (Optional Lua to Cobalt transpiler) 
> **Binary:** `lua-cobalt`
Optional tool used to convert Lua code to Cobalt code. It can be compiled using
`make lua-cobalt` command.
## minicobalt 
> **Type:** (Minimized one script version) 
> **File:** `jit/minicobalt`
Used for internal use by JIT and Raptor compilers. Generated
using `jit/genminicobalt` script.
***

<sub>Read credits [here](https://github.com/cobalt-lang/cobalt/blob/master/COPYRIGHTS.md)</sub>