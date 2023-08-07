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

Some of these are not available on windows, cygwin is going to be really helpful for this.
## Step 3: Building
```bash
$ cmake .
```
You should have build files ready now run
```bash
$ make
```
this should build the binaries.
## (OPTIONAL): lua-cobalt
If you would like to compile Lua source code to cobalt source code 
follow through this step to learn how to build lua-cobalt.
```bash
$ make lua-cobalt
```
You shouldnt see any new files, but the next step will help you install it.

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

