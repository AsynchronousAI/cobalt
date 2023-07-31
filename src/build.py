"""Handles building luax by downloading dependencies, configuring, and compiling, and installing."""
import sys
import os
arg2 = None

try:
    arg2 = sys.argv[1]
    if arg2 == None:
        raise IndexError
    elif arg2 == "clean":
        os.chdir(os.path.dirname(__file__))
        os.system("rm -f *.o *.so *.dylib")
except IndexError:
    value = input("What platform do you want to build for?\n\naix bsd c89 freebsd generic linux macosx mingw posix solaris: ")
    # Open the makefile and replace <--platform--> with the platform
    # It is in the same directory as this file
    arg2 = value
MAKEFILEPATH = os.path.join(os.path.dirname(__file__), "Makefile")

with open(MAKEFILEPATH, "r") as makefile:
    makefiledata = makefile.read()
    makefiledata = makefiledata.replace("<--platform-->", arg2)
print("Configuring...")
with open(MAKEFILEPATH, "w") as makefile:
    makefile.write(makefiledata)
    
print("Building...")
os.chdir(os.path.dirname(__file__))
os.system("make")
