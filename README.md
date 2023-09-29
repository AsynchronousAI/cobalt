# Cobalt
> **NOTICE** Cobalt is heavily indevelopment, read the bottom of this file for a TODO.

Cobalt is a language based on Lua, LJS, and Pluto aiming to be a language that can fill many tasks, a
low-level language like C++, a quick and easy language like python, and a lightweight language like Lua.

Add to docs:
- SDL bindings (optional) for 2D graphics
- Memory optimizations
- type system (typechecker in preprocessor)
- preprocessor
- `switch`, `case`, `default`
- Real OOP with `class`, `parent`, `extends`, `new`, and `of`
- allocation tracker (track bytes and pool allocator stats)
- Easy interface to get device information `device`, `device.specs().CPU`, etc
- C API extended to allow C datastructures to be posted to Cobalt without needing to be wrapped
- lpeg-labels built in
- `$ismethod` & `$isstatic`

Tested on:
- MacOS (multiple configurations)
- Linux (multiple, and extreme configurations)

Todo:
- Default args
- Named args
- Switch statements
- OOP (`extends`, `parent`, `new`, `of` all lexer prebuilding operators)
- Typechecker
- LS
- try & catch