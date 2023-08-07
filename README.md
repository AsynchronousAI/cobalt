# cobalt
A programming language made in C and C++ made to be the *portable, simple, and modern* successor of C.

## Example code:
The following code is a snippet that shows off some of the genera features of cobalt.

Read [examples.md](/Examples.md) for more short examples that work. 
Check out [/cobalt-tests](/cobalt-1.0.0-tests/) for 100+ longer test examples.

```js
import("notareallibrary") // Not a real library, just an example

if (core.macros("__APPLE__") == "1"){ // Real C macros
    print("Seems that you are on an apple device");
}

print(device.os); // "windows" or "linux" or "darwin"
// This is a comment
var a = 1; // Type declarations are now optional
var b: number = 2; // This is a number, and will always be a number

for (i in range(0, 10)) {
    print(i);
}

var memoryaddress = core.memory.alloc(10); // Allocates 10 bytes of memory
var memoryaddress = core.memory.realloc(memoryaddress, 20); // Reallocates 20 bytes of memory
core.memory.free(memoryaddress); // Frees the memory

// Enums!
enum("myenum", {
    a = 1,
    b = 2,
    c = 3
})

print(myenum.a) // 1

var myarray = [1,2,3,4]
var mytable = {
    "a": 1,
    "b": 2,
    "c": 3
}

async.new("print('I am in thread 2')")->start(); // Real threads, using pthreads and windows threads
```

## Why?
C is a great language same for C++, but they dont appeal to the modern programmer. Cobalt is a language that is made from the groundwork of Lua & LuaJIT which are some of the fastest languages of all-time and we improved it
to have C like features like enums, memory, macros, and used ljs syntax to look like C rather than Lua's quirky
syntax.

## Speed?
Cobalt is as fast as Lua(JIT) which it is based upon. LuaJIT is considered to be as fast as C and C++ in some cases, and Rust in most cases. Cobalt has its own performance improvements over LuaJIT, so can be considered to
match C, C++, and Rust in nearly all cases.

## Features
- Direct memory access
- Type annotations and checking
- Real threads
- Ultra portable
- Real macros
- Enums
- Ultra fast
- Manual memory management
- Better syntax

## Why should I switch from what I am using?
Its not my choice to make but I believe regardless on what you are using cobalt should have something that applys to you, Cobalt is ultrafast and portable
since it is based on Lua, and LuaJIT source code. Not only that but it comes packaged with all the essentials
like memory access, typechecker, threads, and more. If you are using C/++ or Rust switching will give you
an overall better programming experience regardless if you are a beginner, a professional, or a hobbyist,
doing this as a job, or just for fun. Cobalt is a language that is made for everyone to give them
everything they need to make a great program with a great experience.

## [How to build and install"](/build.md)
## [Credits](/COPYRIGHTS.md)
***

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