# Cobalt
> **NOTICE** Cobalt is heavily indevelopment, read the bottom of this file for a TODO.

Cobalt is a language based on Lua, LJS, and Pluto aiming to be a language that can fill many tasks, a
low-level language like C++, a quick and easy language like python, and a lightweight language like Lua.

Cobalt includes:
- SDL bindings (optional) for 2D graphics
- FFI
- Memory optimizations
- type system (typechecker in preprocessor)
- preprocessor
- `switch`, `case`, `default`
- Real OOP with `class`, `parent`, `extends`, `new`, and `instanceof`
- `enum`'s
- cleaner syntax errors and stack traces with colors
- `_INCLUDES` global variable is a table of all libraries built with cobalt (ex: `if (!_INCLUDES["sdl"]) error("SDL is required")`)
- allocation tracker (track bytes and pool allocator stats)
- Manipulate dynamic libraries like `dyn.load`, `.get`, and `.close`. 
- `unix`, `win`, and `core` (core is cross plat) for lowlevel system calls
- 2x speed improved gc (optional) using libgc
- improved vm
- locked tables (like frozen tables in pluto) using `table.lock` and `table.islock`
- Easy interface to get device information `device`, `device.specs().CPU`, etc
- C API extended to allow C datastructures to be posted to Cobalt without needing to be wrapped
- 30% increase with pool allocator
- lpeg-labels built in