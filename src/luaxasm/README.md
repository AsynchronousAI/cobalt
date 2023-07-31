# luaxasm
## What is it?
This is made to make standalone executables for luax programs. It requires a C and C++ compiler to work (ex: GNU, Clang).
## How to use it?
- Use build.py to compile luaxasm, luax, luaxc, and required dependencies
- Move /dist/luaxasm to PATH/$PATH (build.py will ask you if you want to do this)
- Run `luaxasm <file>` and a new folder called `<filename>_luaxasmout` will be created and will have a bunch of C, C++, and header files.
- Use `make` to compile the program
- You should get a new executable called `<filename>` in the folder
- Run it and it should work