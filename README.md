# lxx
## What is it?
lxx is an simplified version of C and C++ based on Lua. It is written in C, C++ and is made to be portable. It can be used as a standalone language, or embedded into C or C++, or 
bridged to even more languages. It is made to be simple, and low level. It is also made to be cross platform, and can be used on Windows, Linux, and MacOS. It has alot of 
built in tools for all uses, like a set of VScode tools, a linter, a compiler, a syntax highlighter, bytecoder, interpreter, FFI, complex numbers, and more. Its purpose is to
be a simplified, high lever but at the same time low level language that can be used for anything and is made to be served as an alternative to Lua, C, and C++.

## Lua vs Lxx
- Lua is a scripting language                           
 Lxx is a programming language
 ***
- Lua is interpreted and bytecoded                      
 Lxx is interpreted, bytecoded, and assembly compiled
 ***
- Lua is made to be embedded                            
 Lxx is made to be embedded, bridged, and used as a standalone language
 ***
- Lua is made to be simple                              
 Lxx is made to be simple and low level
 ***
- Lua is made to be portable                            
 Lxx is made to be portable
 ***
- Lua cannot access memory directly                     
 Lxx can access memory directly
 ***
- Lua cannot use Macros                                 
 Lxx can use Macros, even compiler defined C macros (ex: _\_APPLE\_\_, _\_WIN32\_\_, etc)
 ***
- Lua uses Makefiles                                    
 Lxx uses CMake
 ***
- Lua uses a garbage collector                          
 Lxx uses a garbage collector, and manual memory management
 ***
- Pure Lua doesnt support complex numbers               
 Lxx supports complex numbers
 ***
- Lua doesnt have Vector3, Color, Vector2, etc          
 Lxx has Vector3, Color, Vector2, etc
 ***
- Lua has a very "quirky" syntax                        
 Lxx has a more C like syntax
 ***
- Lua doesnt have a built in FFI (not including LuaJIT) 
 Lxx has a built in FFI
 ***
- Lua is good at cross platform                         
 Lxx is great at cross platform with alot of tools to help
 ***
- Lua uses coroutines (which arent really threads)      
 Lxx uses real threads using pthreads (or windows threads on windows)
 ***
- Lua lacks a communication system between threads      
 Lxx has a communication system between threads
 ***
- Lua is more light weight and for scripting            
 Lxx is also light weight but is for longer programs and games
## Example code
[This](/Examples.md) markdown file has short examples of LXX code. This shows real short working code that can do really cool stuff.
[This](/lxx-1.0.0-tests) test directory has ~100 example LXX files. This shows more specific longer tests and examples of LXX code. Most of these were ported from LuaJIT and 
rewritten in LXX using lua-lxx.
## Building
LXX at the moment requires you to build it and does
not offer prebuilt binaries. This will change in the future.
### Dependencies
- CMake (if you want to build)
- NPM (if you want to build the VSCode extension)
- VSCE (if you want to build the VSCode extension)
- GCC or Clang (if you want to build)
- GCC (if you want to use core.macros())
Using it doesnt require any dependencies apart from core.macros() which requires GCC in the user's device
### Building
- `git clone https://github.com/AsynchronousAI/lxx`
- `cd lxx`
- `cmake .`
- `make`
This will result in 2 files in the lxx folder, lxx and lxxc. You can test the shell using `./lxx` and try
some code then install it to your PATH/$PATH.
### lua-lxx (OPTIONAL)
This is an optional tool for converting Lua source code to LXX source code. This is not required to use LXX but is a nice tool to have.
- `make lua-lxx` - Make sure you didnt clean the build folder or this will fail
You will find newly compiled binaries at lxx-1.0.0/lua-lxx. If you install in the next step it should be in your PATH/$PATH.
You can use this by making a `compile.lua` in your cwd and running `lua-lxx` and the new code should be in the terminal.

### Installing
#### For python3
- `sudo make setup` - Make sure you didnt clean the build folder or this will fail, this also requires python3 but installs with `python3`
#### For py
- `sudo make setuppy` - Make sure you didnt clean the build folder or this will fail, this also requires python3 but installs with `py` instead of `python3`
#### For python
- `sudo make setup-py` - Make sure you didnt clean the build folder or this will fail, this also requires python but installs with `python` instead of `python3` or `py`

### End result
- `lxx` executable in the cwd for the shell, interpreter, and bytecode interpreter
- `lxxc` executable in the cwd for compiling to bytecode
- `lxxjit` executable in the cwd for JIT compiling
- `lua-lxx` executable in the cwd for converting Lua source code to LXX source code if you did that step

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
- Rebrand alot of the code
- Make a better linter
- Integrate JIT
- Add tests
- Install.py for CMake
- library installer