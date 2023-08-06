# lxx
## What is it?
lxx is meant to be a *modernized* version of C/C++ based on Lua source code.
## Why?
Lua is a great language but it has some flaws. Lxx is meant to fix these flaws and make it more low level with features such
as accessing C macros, manual memory management, classes for real OOP, real threads (not coroutines 🤮), and more.
## Example code
[This](/Examples.md) markdown file has short examples of LXX code. This shows real short working code that can do really cool stuff.
[This](/lxx-1.0.0-tests) test directory has ~100 example LXX files. This shows more specific longer tests and examples of LXX code. Most of these were ported from LuaJIT and 
rewritten in LXX using lua-lxx.
## Building
LXX at the moment requires you to build it and does not offer prebuilt binaries. This will change in the future.

You must have Make installed and CMake must export to Make
### Dependencies
#### Windows only
- [Windows equivelent](https://www.cygwin.com) of `make`, `cmake`, and `gcc` (or `clang`) <br>
You need these because this was originally designed for POSIX systems and some of the dependencies are POSIX only. Luckily cygwin has all of these.
#### Building source
- CMake
- GCC or Clang
- Make
#### Building VSCode extension
- NPM (if you want to build the VSCode extension)
- VSCE (if you want to build the VSCode extension)
#### Optional tools (for usage)
- GCC (For accessing C macros, optional feature)
- Nothing else
### Building
- `git clone https://github.com/AsynchronousAI/lxx`
- `cd lxx`
- `cmake .`
- `make` <br>
This will result in 2 files in the lxx folder, lxx and lxxc. You can test the shell using `./lxx` and try
some code then install it to your PATH/$PATH.
### lua-lxx (OPTIONAL)
This is an optional tool for converting Lua source code to LXX source code. This is not required to use LXX but is a nice tool to have.
- `make lua-lxx` - Make sure you didnt clean the build folder or this will fail
You will find newly compiled binaries at lxx-1.0.0/lua-lxx. If you install in the next step it should be in your PATH/$PATH.
You can use this by making a `compile.lua` in your cwd and running `lua-lxx` and the new code should be in the terminal.

### Installing binaries and libraries
#### For python3
- `sudo make setup` - Make sure you didnt clean the build folder or this will fail, this also requires python3 but installs with `python3`
#### For py
- `sudo make setuppy` - Make sure you didnt clean the build folder or this will fail, this also requires python3 but installs with `py` instead of `python3` usually for windows
#### For python
- `sudo make setup-py` - Make sure you didnt clean the build folder or this will fail, this also requires python but installs with `python` instead of `python3` or `py`

### End result
- `lxx` executable in the cwd for the shell, interpreter, and bytecode interpreter
- `lxxc` executable in the cwd for compiling to bytecode
- `lxxjit` executable in the cwd for JIT compiling
- `lua-lxx` executable in the cwd for converting Lua source code to LXX source code if you did that step

### Extra options
- `make clean` - Cleans the build folder and object files
- `vsce package` - Packages the VSCode extension (when your CWD is the VSCode extension folder). You shouldnt need to do this because the plugin is in the VSCode marketplace.
## Credits
- Lua (Ljs Starting point)
- LuaJIT (LjsJIT Starting point)
- Ljs (Starting point)
- LjsJIT (Starting point for JIT)
- selene (Used in Linter)
- libffifb (Used in FFI)
- LuaDist (Alot of their code is used in lxx)

## TODO:
- FFI
- Copyright 
- Make a better linter
- Integrate JIT
- library installer