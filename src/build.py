"""Handles building luax by downloading dependencies, configuring, and compiling, and installing."""
import sys
import os

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
    
# Begin building
print("\n\n\n")
print("Building...")
exitcode = os.system("make")
if exitcode != 0:
    print("Build failed!")
    sys.exit(1)
print("Build succeeded!")

# Install
print("\n\n\n")
print("Please specify what extension would be used.")
res = input("(ex: .exe, or nothing for no extension usually on UNIX systems): ")
os.system("mkdir ../dist")
os.system("mv luax"+res+" ../dist/luax" + res)
os.system("mv luaxc"+res+" ../dist/luaxc" + res)
        
# Inform user
print("\n\n\n")
print("Done! luax is now in dist/luax" + res + " and luaxc is in dist/luaxc" + res + ".")
print("Move them to your PATH/$PATH to use them.")

# Extra options
print("\n\n\n")
print("More options:")
print("\n")
print("1- Read install data")
print("2- Clean up build files")
print("3- Build again for another platform")
print("4- Move dist/luax" + res + " and dist/luaxc" + res + " to your PATH/$PATH")
print("5- Exit")
inp = input("What do you want to do? (1, 2, 3, etc.): ")

if inp == "1":
    os.system("make echo")
elif inp == "2":
    os.system("python3 build.py clean")
elif inp == "3":
    os.system("python3 build.py")
elif inp == "4":
    # Step number one get the path for PATH
    print("\n\n\n")
    print("Please specify the path for PATH/$PATH.")
    print("If you don't know what this is, please search it up.")
    print("If you don't want to do this, please press Ctrl+C.")
    val = input("PATH/$PATH: ")

    # Step number two move the files
    print("\n\n\n")
    print("Moving files...")
    os.system("mv dist/luax" + res + " " + val)
    os.system("mv dist/luaxc" + res + " " + val)
    print("Done!")
else:
    print("Exiting...")
    sys.exit(0)