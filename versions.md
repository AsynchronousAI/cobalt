# Versions:

***
# Compilers and Interpreters:
## 23
### cobalt 
> **Type:** (Interpreter and Bytecoder) 

> **Binaries:** `cobalt` `cobaltc`

Original lightweight Cobalt interpreter and bytecoder. It can be compiled using
`make` and is the default option, it has the most and newest features and is
the most stable and portable. On sync with cobaltjit.
### cobaltjit 
> **Type:** (Optional JIT Compiler) 

> **Binary:** `cobaltjit`

Optional tool used for JIT compilation. It can be compiled using
`make jit` command. 

## 24
## cobalt-one
> **Type:** (Optional Standalone executable compiler)
> **Binary:** `cobalt-one`

cobalt24 standalone executable compiler, only given to testers and developers. [beta page](https://cobaltlang.vercel.app/beta)
## cobalt24
> **Type:** (Beta Interpreter and Bytecoder)
> **Binary:** `cobalt` `cobaltc`

cobalt24, only given to testers and developers. [beta page](https://cobaltlang.vercel.app/beta)

## cobaltjit
> **Type:** (Optional JIT Compiler)
> **Binary:** `cobaltjit`

cobalt24 JIT, only given to testers and developers. [beta page](https://cobaltlang.vercel.app/beta)

***
# Other:
## cobaltasm
> **Type:** (Optional Assembler)

> **Binary:** `cobaltasm`

asm combined to a binary, install using `make asm`.

It converts assembly code to machine code is used by JIT, and based on DynASM.
## lua-cobalt 
> **Type:** (Optional Lua to Cobalt transpiler) 

> **Binary:** `lua-cobalt`

Optional tool used to convert Lua code to Cobalt code. It can be compiled using
`make lua-cobalt` command.
## minicobalt 
> **Type:** (Minimized one script interpreter ) 

> **Binary:** `minicobalt`

> **File:** `jit/minicobalt`

Lightweight ~4k lines version of cobalt, Included with `cobaltjit`, installed with
`make setup` after `make jit` is ran. Documentation is not provided and 
it is not recommended to use it because of its lack of features and stability.

***

<sub>Read credits [here](https://github.com/cobalt-lang/cobalt/blob/master/COPYRIGHTS.md)</sub>