# cobalt23/lua
Holds the CLI for converting Lua (5.3.5 not 5.4.6) source code to cobalt source code.

Based on LJS's lua2cobalt but with a few changes:
- CMake
- Supports cobalt rather than LJS
***
```bash
$ lua-cobalt myfile.lua -o myfile.cobalt
```