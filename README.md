# lxx
## What is it?
lxx is an simplified version of C and C++ based on Lua. It is written in C and C++ and is made to bw portable. It is also embeddable, interpreted, bytecoded, and assembly compiled. It comes with a load of libraries for USB 
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
### Requirements
- Python 3
- CMake
- A C/C++ compiler (Preferably GCC or Clang)
- Git
### Building
- `git clone https://github.com/AsynchronousAI/lxx`
- `cd lxx`
- `python3 build.py` for an interactive setup. 
### Other options
- `python3 build.py help` for help
- `python3 build.py clean` to clean the build
You must use build.py to build as it modifys the souce code to work with your requirements and avoids conflicts since this has multiple MakeFiles. 
### End result
- `lxx` executable in the `dist` folder for interpreting
- `lxxc` executable in the `dist` folder for compiling to bytecode
- `lxxasm` executable in the `dist` folder for making .o files from bytecode
- If you chose to move them to the PATH/$PATH they will be there instead of the dist folder
## Credits
- Lua
- Ljs
- selene
- libffifb
## Dependencies
- Lua 5.4
- Python (if you want to build)
- CMake (if you want to build)
- A C/C++ compiler (if you want to build or use the FFI)

## TODO:
- FFI
- Add linter
- fix require
- Publish extension