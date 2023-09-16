# Cobalt
> **NOTICE** Cobalt is heavily indevelopment, read the bottom of this file for a TODO.


Cobalt is a Lowlevel fork of Lua 5.4 which includes:
- SDL bindings (optional) for 2D graphics
- interpreter, compiler, and JIT FFI
- compile to LLVM IR if you have LLVM capable version of cobalt
- more libraries/bindings
- improved syntax
- Memory optimizations
- type system (typechecker in preprocessor)
- preprocessor
- `_INCLUDES` global variable is a table of all libraries built with cobalt (ex: `if (!_INCLUDES["sdl"]) error("SDL is required")`)
- allocation tracker (track bytes and pool allocator stats)
- `uwait`, `swait`, `mwait` for system sleep
- new std functions
- Manipulate dynamic libraries like `dyn.load`, `.get`, and `.close`. 
- USB (libusb) bindings
- `unix`, `win`, and `core` (core is cross plat) for lowlevel system calls
- AOT compiler byte->C or byte->LLVMIR if you have LLVM capable version of cobalt
- libclang bindings (compile C code from Cobalt)
- cURL capabilites
- light functions
- regex support (not full regex only a small subset), use `unix.regex` for POSIX regex
- 2x speed improved gc (optional) using libgc
- improved vm
- Vulkan bindings (optional) for high-performance 3D graphics
- Python bridge `import("python")`
- readonly tables or `rotables`
- Easy interface to get device information `device`, `device.specs().CPU`, etc
- LLVM JIT compiler
- `debug.snap` to track current stack
- bytecode optimizer
- C API extended to allow C datastructures to be posted to Cobalt without needing to be wrapped
- 30% increase with pool allocator
- lpeg-labels built in
- more operators
- tenary operator
- `` ` `` for python like format ``print(`Hello {name}`)`` directly implemented to the parser and bytecode compatable
- CMake build system
## When to, and not use
### Graphics Programming
Cobalt is a great choice for graphics programming as it has SDL, GLFW, Vulkan, and a Graphical Math library built in.
### AI
Cobalt doesn't include AI libraries/tools but is possible using the Python bridge or C FFI. Cobalt is a great choice for AI as it has a fast VM and JIT and access to Python and C's utilities.
### Web Development
Cobalt can compile to WASM through Cobalt->C->WASM or Cobalt->LLVM-IR->WASM and imported by JS. Cobalt 
natively doesnt support web development but is planned for [`cobalt 2024`](https://cobaltlang.vercel.app/beta). As a backend Cobalt is a great choice with it's networking capabilities and speed.
## Comparision
In comparision to Lua, cobalt has the following to improve speed/memory:
- 2x faster gc
- 30% faster pool allocator
- 3-4x less memory usage by the cobalt core
- light functions and rotables
- 1.5x to 2.5x faster vm with JIT
- Mini interpreter that kicks in when some of cobalts resources are not needed for less memory usage
- Easy memory leak tracking
***
which is a ~8.5x speed improvement over Lua 5.4.3 (with Cobalt JIT). and if you are not looking for on-runtime, the new AOT is 2x faster with Bytecode and 10x faster with LLVM IR AOT.
## Syntax
### Lua:
```lua  
local a = 1
local b <const> = 2
local c = a + b
local w = math.sqrt(1043)

for i = 1, 10 do
    print(i)
    if i == 5 then
        continue
    end
end

local tbl = {1, 2, 3}
for i, v in ipairs(tbl) do
    print(i, v)
end

while true do 
    print("In a loop, and b is "..b) 
    break 
end

local x = if a == 1 then "A is 1" else "A is not 1"
```
### Cobalt:
```ts
var a = 1;
var b <const> = 2;
var w <pre> = math.sqrt(1043); /* will calculate at preprocess time and replace w with the result instead of calculating on runtime */
var c: number = a + b; /* optional typechecker/annotations */

for (i = 1, 10){
    print(i);
    if (i == 5){
        continue;
    }
}

var tbl = [1, 2, 3]; // [] is array (no keys), {} is table (keys)
for (i, v in $tbl){ // $, @, & are preprocessor only
    print(i, v);
}
var tbl2 <ref> = tbl[2]; /* tbl2 will not copy tbl[2] to its own instance but rather be a pointer to it, supported in cobalt24+ */
print(tbl2); // 2 (indexing starts at 1 like Lua)

while (true) {
    print(`In a loop, and b is {b}`) // python like format
    break; 
}

var x: string = a == 1 ? "A is 1" : "A is not 1";
```
## Portability
Cobalt is made to be built and run on PC's running Windows, macOS, or Linux then
using LLVM or bindings further ported to other platforms. Cobalt is not made to be
built on other platforms or built for other platforms than the host computer as
upon buildtime many CPU specific optimizations are made.

### What about for microcontrollers?
#### Executable
For microcontrollers you need to have atleast 20kb of ram and 200kb of flash. Libraries like `core`, `device` might be
missing features and the `unix`, `win` libraries should not be available.

To build the following flags are reccomended:
```bash
-DCURL=off -DSDL=off -DM=2 -DCLANG=off -DLLVM=off -DCROSS=on -DPYTHON=off
```
* -DCURL, -DSDL, -DPYTHON, -DCLANG disable the bindings for those libraries (they are not supported and needed for microcontrollers)
* -DLLVM=off disables JIT and LLVM AOT which are just junk for microcontrollers.
* -DM=2 maxes out memory optimizations in sacrifice of speed. `-DM=0` is the default and `-DM=1` is light memory optimizations.
* -DCROSS=on disables CPU specific optimizations.

> **NOTICE:** You must build cobalt on the host OS (and device is -DCROSS and -DFFI is off) so this is not really good for distrubuting to other platforms/devices.
#### C API
You can also use `#include` to include the cobalt stdlibs and run it there.
```c
/* equivelent to flags: */
#define COBALT_CURL 0
#define COBALT_SDL 0
#define COBALT_CLANG 0
#define COBALT_PYTHON 0
#define COBALT_LLVM 0
#define MOPT 2
/*************/

#include <cobalt.h>
#include <lauxlib.h>
#include <lualib.h>
#include <lautoc.h>

/* This is a barebones start without the Clang, LLVM, cURL, and SDL libraries or standard libraries */
```
Then you can use the C API to run cobalt code.
```c
int main(){
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaL_loadstring(L, "print('Hello World!')");
    lua_pcall(L, 0, 0, 0);
    lua_close(L);
    return 0;
}
```
#### C
You could compile sourcecode to C or LLVM IR on your local computer and use that in the microcontroller.
```bash
cobaltaot main.cobalt -o main.c -m mainsymbol
```
will generate a C file representing `main.cobalt` and the symbol that will start operating for the linker is `mainsymbol`.
#### LLVM
```bash
cobaltaot main.cobalt -o main.ll -m mainsymbol
```
# Python
Run cobalt code in python
```py
import cobalt
cobalt.eval("print('Hello World!')") # prints Hello World!
```
Run python code in cobalt
```js
python = import("python")
python.eval("print('Hello World!')") // prints Hello World!
sys = python.import("sys")
sys.version // prints python version
```
***
# TODO
- Redo [documentation site](cobaltlang.vercel.app)
- Mini interpreter that can kick in
- Bind JIT source files and fixes
- LLVM IR AOT
- Fix formatstrings
- Preprocess values 
- Implement the `rlua` memory optimizations
- typechecker & type creation
- OpenGL bindings (check moonGL)
- Test on windows.
# Known Issues (and fixes)
## Library not loaded: _
This happens with the SDL library, to fix this you need to install (or reinstall) SDL2.

## `cobalt: error: cannot open file 'lua.h'`
Cobalt uses `cobalt.h` not `lua.h` so you need to change your include to `cobalt.h`.
