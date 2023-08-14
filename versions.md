# Versions:

***
# Compilers and Interpreters:
## cobalt 
> **Type:** (Interpreter and Bytecoder) 

> **Binaries:** `cobalt` `cobaltc`

Original lightweight Cobalt interpreter and bytecoder. It can be compiled using
`make` and is the default option, it has the most and newest features and is
the most stable and portable.  
## cobaltjit 
> **Type:** (Optional JIT Compiler) 

> **Binary:** `cobaltjit`

Optional tool used for JIT compilation. It can be compiled using
`make jit` command. It is slower than Raptor. It is 
a inbetween of the interpreter and Raptor.

## cobalt-raptor
> **Type:** (Optional Ultrafast JIT Compiler) 

> **Binary:** `cobaltraptor`

Optional tool like JIT but includes more tools for optimization. It can be compiled using
`make raptor` command. It is faster than JIT but heavier in executable size.

## cobalt-one
> **Type:** (Optional Standalone executable compiler)
> **Binary:** `cobalt-one`

cobalt24 (Not current cobalt23) tool for compiling standalone executables. Not available to the public yet.
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

Ultralightweight version of cobalt minimized to ~4k lines. It will be built along side 
cobaltJIT and is not documentated and will need to be installed manually. It is not
recommended to use this, it is just for internal JIT use and for our own testing.
***

<sub>Read credits [here](https://github.com/cobalt-lang/cobalt/blob/master/COPYRIGHTS.md)</sub>