"""Handles building lxx by downloading dependencies, configuring, and compiling, and installing."""

# @AsynchronousAI - 2023



import sys
import os
import subprocess
import time


#########################
# Prepare
os.chdir(os.path.dirname(__file__))

# Get the platform to build for or if it is clean
print("\n\n\n")
arg2 = None
try:
    arg2 = sys.argv[1]
    if arg2 == None:
        raise IndexError
    elif arg2 == "clean":
        print("Cleaning")
        os.system("make clean")
        # delete ffi.so
        if os.path.exists("ffi.so"):
            os.system("rm ffi.so")
        
        sys.exit(0)
    elif arg2 == "-h":
        print("Usage: python3 build.py <platform> (or clean to clean up build files)")
        print("Platforms: aix bsd c89 freebsd generic linux macosx mingw posix solaris")
        sys.exit(0)
except IndexError:
    value = input("What platform do you want to build for?\n\naix bsd c89 freebsd generic linux macosx mingw posix solaris: ")
    # Open the makefile and replace <--platform--> with the platform
    # It is in the same directory as this file
    arg2 = value

# Prepare the makefile
print("\n\n\n")
MAKEFILEPATH = os.path.join(os.path.dirname(__file__), "Makefile")
with open(MAKEFILEPATH, "r") as makefile:
    makefiledata = makefile.read()
    makefiledata = makefiledata.replace("<--platform-->", arg2)
print("Configuring...")
with open(MAKEFILEPATH, "w") as makefile:
    makefile.write(makefiledata)
    
### BUILD
item = input("Would you like to install to dist or bin (1/2): ")
if item == "1":
    print("Installing to dist/")
elif item == "2":
    print("Installing to usr/bin/.../")
else:
    print("Invalid input!")
    sys.exit(1)
    
print("Building lxx...")

# Run make
if item == "2":
    exitcode = os.system("make install")
else:
    exitcode = os.system("make")
if exitcode != 0:
    print("Build failed!")
    sys.exit(1)

### WAIT UNTIL BUILD IS DONE
while True:
    if os.path.exists("lxx") and os.path.exists("lxxc"):
        break
    else:
        continue

    

# Go back to the main directory
os.system("cd ..")
print("\n\n")
# Delete /lxx and /lxxc, they are prebuild
os.system("rm lxx")
os.system("rm lxxc")

# Move the contents of PostMakefile to Makefile

print("Building lxx...")
with open(MAKEFILEPATH, "r") as makefile:
    makefiledata = makefile.read()
    makefiledata = makefiledata.replace("#<--so-->", "ffi.so")
exitcode = os.system("make")
if exitcode != 0:
    print("lxx Build failed!")
    sys.exit(1)
while True:
    if os.path.exists("lxx") and os.path.exists("lxxc"):
        break
    else:
        continue
print("Build succeeded!")
os.system("mv Makefile PostMakefile")
os.system("mv PostMakefile Makefile")
# Install
print("\n\n\n")
print("Please specify what extension would be used.")
res = input("(ex: .exe, or nothing for no extension usually on UNIX systems): ")
os.system("mkdir ../dist")
os.system("mv lxx"+res+" ../dist/lxx" + res)
os.system("mv lxxc"+res+" ../dist/lxxc" + res)
        
# Inform user
print("\n\n\n")
print("Done! lxx is now in dist/lxx" + res + " and lxxc is in dist/lxxc" + res + ".")
print("Move them to your PATH/$PATH to use them.")

# Ask to install libraries
print("\n\n\nLibraries are available for lxx which will offer immense functionality.\nWould you like to install them? (y/n): ")
newinput = input("Would you like to install libraries? (y/n): ")
if newinput == "y":
    # TODO: Install libraries
    pass
else:
    pass

# Extra options
print("\n\n\n")
print("More options:")
print("\n")
print("1- Read install data")
print("2- Clean uneeded up build files")
print("3- Build again for another platform")
print("4- Exit")
inp = input("What do you want to do? (1, 2, 3, etc.): ")

if inp == "1":
    print("\n\n\nlxx")
    os.system("make echo")
elif inp == "2":
    os.system("python3 build.py clean")
elif inp == "3":
    os.system("python3 build.py")
else:
    print("Exiting...")
    sys.exit(0)