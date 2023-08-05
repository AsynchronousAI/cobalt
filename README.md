# lxx
## What is it?
lxx is an simplified version of C and C++ based on Lua. It is written in C, C++ and is made to be portable. It is also embeddable, interpreted, bytecoded, and assembly compiled. It comes with a load of libraries for USB 
control, FFI, audio, etc. lxx's focus is to be a portable language that works everywhere while being simple
and made to make applications and games.

## Features
- Ultrafast and presice 
- FFI
- Embeddable
- Audio control
- Easy to use
- C-like syntax
- Interpreted, Bytecoded, and Standalone executables
- Read [examples](/Examples.md) for more
- Supports using lua alongside lxx
## Building
### Dependencies
- Make (if you want to build)
- GCC or Clang (if you want to build)
- GCC (if you want to use core.macros())
### Building
- `git clone https://github.com/AsynchronousAI/lxx`
- `cd lxx`
- `make src`
### Other options
- `make clean` - Cleans the build
### End result
- `lxx` executable in the `../dist` folder for interpreting
- `lxxc` executable in the `../dist` folder for compiling to bytecode
- `lxxasm` executable in the `../dist` folder for making .o files from bytecode
- If you chose to move them to the PATH/$PATH they will be there instead of the dist folder
## Credits
- Lua (Ljs Starting point)
- Ljs (Starting point)
- selene (Used in Linter)
- libffifb (Used in FFI)
- LuaDist (Alot of their code is used in lxx)
## TODO:
- FFI
- Publish extension