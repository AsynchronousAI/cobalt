# Installing
Prebuilt binaries are available at GitHub Releases. But we reccomend building from source to custom
enable features and have the libraries folder made with the built in libraries.
# Building
## Step 1: Installing source code
```bash
$ git clone https://github.com/cobalt-lang/cobalt
$ cd cobalt
```
## Step 2: Preparing build dependencies
The following is needed for building Cobalt:
- CMake
- Make
- GCC (preferably but any other compiler should work)
- Python 3

Some of these are not available on windows, cygwin is going to be really helpful for this, or you can check [other-platforms.md](other-platforms.md) for more information
on how to build for Windows, XBOX, Nintendo, PS, and more.
## Step 3: Building
```bash
$ cmake .
```
You should have build files ready now run
```bash
$ make
$ make jit # This is an optional command if you want to install the JIT
```
this should build the binaries and you can find `cobalt`, `cobaltc`, and `cobaltjit`. The cobaltjit binary
will be hidden in the source files, step 4 will install it to your system.
## (OPTIONAL): lua-cobalt
If you would like to compile Lua source code to cobalt source code 
follow through this step to learn how to build lua-cobalt.
```bash
$ make lua-cobalt
```
The binary will be hidden in the source files, the next step will install it to your system.    

You can use lua-cobalt by running
```bash
$ lua-cobalt myLua.lua > myCobalt.cobalt
```
## Step 4: Installing
Python is required to install, run the following command depending on how you have python.
it will look for binaries, install it, and then build and install the libraries.
### For python3
```bash
$ make setup
```
### For python
```bash
$ make setuppy
```
### For py (Windows)
```bash
$ make setup-py
```
## Testing
```bash
$ cobalt
```
You can play around in the shell, or run a file with
```bash
$ cobalt myscript.cobalt
```

