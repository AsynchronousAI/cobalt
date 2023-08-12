# cobalt23/lua
Holds the CLI for converting Lua (5.3.5) source code to cobalt source code.

Based on cobalt's lua2cobalt but with a few changes:
- CMake
- Supports cobalt rather than cobalt
***
```bash
$ lua-cobalt myfile.lua -o myfile.cobalt
```