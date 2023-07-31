# luax
## What is it?
Luax is an simplified version of C and C++ based on Lua. It is written in C and C++ and is made to bw portable. It is also embeddable, interpreted, bytecoded, and assembly compiled. It comes with a load of libraries for USB 
control, FFI, audio, etc. Luax's focus is to be a portable language that works everywhere while being simple
and made to make applications and games.

## Features
- Built in libraries for graphics, audio, and more
- Ultrafast
- Way better syntax 
- Easy to learn
- Portable (Written in C)
- Embeddable
- Built in Lexer
- Interpreter
- Bytecode compiler
- Bytecode interpreter
- Vector3 and Vector2 class
- Transform class
- Assembly compiler
- Faster and more random number generator
- wait/swait (seconds), mwait (milliseconds, 1/1000 of a second), and uwait (microseconds, 1/1000000 of a second) functions with amazing accuracy
- Slicing like in Python (<string>.slice(<start>, <end>, <step>))
- Syntax like C and C++
- Class support (<name>: <class> = <value>)
- Easy way to get system variables (sys)
- FFI (Foreign Function Interface)
- USB support
## Building
### Requirements
- Python 3
- CMake
- A C/C++ compiler (Preferably GCC or Clang)
- Git
### Building
- `git clone https://github.com/AsynchronousAI/luax`
- `cd luax`
- `python3 build.py` for an interactive setup. We do not reccomend using Makefile because there are over 5 of them and are implemented in the build.py script.
### End result
- `luax` executable in the `dist` folder
- `luaxc` executable in the `dist` folder
- `luaxasm` executable in the `dist` folder
- If you chose to move them to the PATH/$PATH they will be there instead of the dist folder
## Credits
- Lua
- Ljs
- selene
- libffifb
- moonusb
## TODO:
- Redo FFI so it can be included
- Implement USB
- Fix slice
- Fix memory