# luax
## What is it?
Luax is an simplified version of C and C++ based on Lua. It is written in C and C++ and is made to bw portable. It is also embeddable, interpreted, bytecoded, and assembly compiled. It comes with a load of libraries for USB 
control, FFI, audio, etc. Luax's focus is to be a portable language that works everywhere while being simple
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
## Building
### Requirements
- Python 3
- CMake
- A C/C++ compiler (Preferably GCC or Clang)
- Git
### Building
- `git clone https://github.com/AsynchronousAI/luax`
- `cd luax`
- `python3 build.py` for an interactive setup. 
### Other options
- `python3 build.py help` for help
- `python3 build.py clean` to clean the build
You must use build.py to build as it modifys the souce code to work with your requirements and avoids conflicts since this has multiple MakeFiles. 
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
## TODO:
- FFI
- Add linter
- fix require
- add loop over tables insted of needing to use ipairs
- Publish extension