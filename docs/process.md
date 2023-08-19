# JIT and Interpreter Process

## Introduction
This will cover how the compiler and interpreter work, and how they are
implemented. This is not a tutorial, but a reference for developers and
contributors.
</br>
This is from my experience with modifying the compiler and interpreter, and
should be accurate. If you find any errors, please open an issue.

## JIT
### Build the JIT
##### `make jit`
Begins the build process.
##### Check system requirements
Verifies system requirements. May require enviormnent variables to be set.
##### Build libraries
The libraries will get built to a `.so` file for the library refrence and
initilizer.
###### Build the JIT
Builds the main core with the library.
</br>
Some parts are written in assembly, and will not require a compiler since 
it will use the built in assembler.
</br>
###### Link the JIT
Makes an executable with the core and library.
### JIT Compilitation
##### Source Code
Source code is written by the user.
</br>
Usually `.cobalt` files.

##### Preprocessor
The preprocessor will replace macros and include files. The preprocessor is
written in 100% cobalt, and is wrapped in a C program that makes the `cobaltpre`
binary.
</br>
It will generate a new file with the preprocessed code. Usually `.cobaltii` files.
</br>
</br>
This step is optional. And is not included with the JIT.

##### AST
Parses the source code into a tree and gets it compiled into Assembly.

Usually `.S` files. (Assembly)
##### Assembler
The assembler is built in and converts the assembly into machine code.  Use the 
`-b` flag to get a .o object file out.
##### Executer
The code will get executed.
## Interpreter
### Build the Interpreter
##### `make`
Begins the build process for the interpreter and bytecoder.
##### Build libraries
The libraries will get built to a `.so` file for the library refrence and
initilizer.
##### Build the Interpreter
Builds the main core with the library.

##### Build the Bytecoder
The bytecoder doesnt require to be linked with the library, so it will be built
seperatly.

##### Linker
Links the interpreter with the library as `cobalt` and the bytecoder as `cobaltc`.
### Interpreter Execution
#### From Source Code
##### Source Code
Source code is written by the user.
</br>
Usually `.cobalt` files.

##### Preprocessor
The preprocessor will replace macros and include files. The preprocessor is
written in 100% cobalt, and is wrapped in a C program that makes the `cobaltpre`.
</br>
It will generate a new file with the preprocessed code. Usually `.cobaltii` files.
</br>
This step is optional.
</br>
The Interpreter, JIT, Bytecoder all use the same preprocessor. The preprocessor is not included with any of the binaries.

##### AST
Parses the source code into a tree.
</br>
Usually held internally as a tree.

##### Executer
The tree will get executed in C.
#### To Bytecode
##### Source Code
Source code is written by the user.
</br>
Usually `.cobalt` files.

##### Preprocessor
The preprocessor will replace macros and include files. The preprocessor is
written in 100% cobalt, and is wrapped in a C program that makes the `cobaltpre`.
</br>
It will generate a new file with the preprocessed code. Usually `.cobaltii` files.
</br>
This step is optional.
</br>
The Interpreter, JIT, Bytecoder all use the same preprocessor. The preprocessor is not included with any of the binaries.

##### AST
Parses the source code into a tree.
</br>
Usually held internally as a tree.

##### Bytecoder
The tree will get converted into bytecode.
#### From Bytecode
##### Bytecode
The bytecode will be parsed into a tree.
##### Executer
The tree will get executed in C.