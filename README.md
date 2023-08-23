# Cobalt
cobalt is a Lua fork attempting to make a language as simple as lua into something as powerful as C.

## What Lua version is this based on?
This is based on Lua 5.4. It will be updated often to stay in sync with lua while having its own features.

## Features
- Preprocessor written 100% in Cobalt (`#define`, `#include`, `#ifdef`, `#ifndef`, `#endif`, `#else`, `#elseif`)
- Type system (`x: int = 0`)
- Improved syntax based on LJS (`local x = 0` is now `var x = 0`)
- Extended math library
- `core`, `device`, `struct`, `complex`, `Vector3`, `Vector2`, `Color`, `Transform`, `signal`, `file`, `msg`, and more libraries written for cobalt or ported from C or Lua.
- Arrays
- Major performance improvements
- Standard library with libraries written for cobalt or ported from C or Lua.
- Interpreter `ffi` based on `luaffifb`
- Time methods: `wait` (second), `swait` (second), `mwait` (millisecond), `umwait` (microsecond).
- Switch statements (with built in `switch` library)
- Enums
- Records (like teal records)
- Traceback mode for C by using: `#include <ltrace.c> && #include <ltrace.h>` over `#include <cobalt.h> && #include <cobalt.h>`
