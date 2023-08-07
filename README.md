# cobalt
## What is it?
cobalt is meant to be a *modernized* version of C/C++ based on Lua source code. This is written in C and cobalt so it is ultraportable.
## Why?
Lua is a great language but it has some flaws. cobalt is meant to fix these flaws and make it more low level with features such
as accessing C macros, manual memory management, classes for real OOP, real threads (not coroutines 🤮), and more.
## Example code
[This](/Examples.md) markdown file has short examples of cobalt code. This shows real short working code that can do really cool stuff.
[This](/cobalt-1.0.0-tests) test directory has ~100 example cobalt files. This shows more specific longer tests and examples of cobalt code. Most of these were ported from LuaJIT and 
rewritten in cobalt using lua-cobalt.
## Building
cobalt at the moment requires you to build it and does not offer prebuilt binaries. This will change in the future.

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
- `git clone https://github.com/cobalt-lang/cobalt`
- `cd cobalt`
- `cmake .`
- `make` <br>
This will result in 2 files in the cobalt folder, cobalt and cobaltc. You can test the shell using `./cobalt` and try
some code then install it to your PATH/$PATH.
### lua-cobalt (OPTIONAL)
This is an optional tool for converting Lua source code to cobalt source code. This is not required to use cobalt but is a nice tool to have.
- `make lua-cobalt` - Make sure you didnt clean the build folder or this will fail

Using `make setup(-/py)` will automatically detect and install the compiled binaries of lua-cobalt alongside the main cobalt binaries.
#### Usage:
- `lua-cobalt myfile.lua > myfile.cobalt` - Converts myfile.lua to myfile.cobalt

### Installing binaries and libraries
#### For python3
- `sudo make setup` - Make sure you didnt clean the build folder or this will fail, this also requires python3 but installs with `python3`
#### For py
- `sudo make setuppy` - Make sure you didnt clean the build folder or this will fail, this also requires python3 but installs with `py` instead of `python3` usually for windows
#### For python
- `sudo make setup-py` - Make sure you didnt clean the build folder or this will fail, this also requires python but installs with `python` instead of `python3` or `py`

### End result
- `cobalt` executable in the cwd for the shell, interpreter, and bytecode interpreter
- `cobaltc` executable in the cwd for compiling to bytecode
- `cobaltjit` executable in the cwd for JIT compiling
- `lua-cobalt` executable in the cwd for converting Lua source code to cobalt source code if you did that step

### Extra options
- `make clean` - Cleans the build folder and object files
- `vsce package` - Packages the VSCode extension (when your CWD is the VSCode extension folder). You shouldnt need to do this because the plugin is in the VSCode marketplace.
## Credits
Read full credits [here](/COPYRIGHTS.md)

## TODO:
- FFI (far future) (alongside library installer)
- Batchfiles (far future)
- Make a better linter (far future)
- library installer

### LuaJIT compatibility
- Port libraries to JIT
- Overall sync JIT
- Cmake JIT

#### Changes to add to JIT
- New libraries
- Range function
- inputf
- typedef
- C macros integration
- wait functions
- improved output
- Enum function
- Integrate to Cmake
- JIT Goto