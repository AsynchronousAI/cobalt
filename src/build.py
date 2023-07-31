"""Handles building luax by downloading dependencies, configuring, and compiling, and installing."""
import sys
import os
import subprocess

#########################
# FUNCTIONS
CC = "gcc -std=gnu99"
CFLAGS = "-O2 -fPIC -Wall -Wextra -DLUA_LJS -DLUA_COMPAT_5_2"
LDFLAGS = ""
LIBS = "-lm"

SOURCE_FILES = [
    "cffi.c",
    "call.c",
    "ctype.c",
    "parser.c",
]

OBJECT_FILES = [os.path.splitext(source)[0] + ".o" for source in SOURCE_FILES]
HEADERS = [
    "lprefix.h",
    "lua.h",
    "luaconf.h",
    "lauxlib.h",
    "lualib.h",
    "ffi.h",
    "call_arm.h",
    "call_x64.h",
    "call_x64win.h",
    "call_x86.h",
    "dynasm/dasm_arm.h",
    "dynasm/dasm_x86.h",
    "dynasm/dasm_ppc.h",
    "dynasm/dasm_proto.h",
]

TARGET_SHARED_OBJECT = "ffi.so"

def compile_source(source_file):
    cmd = f"{CC} {CFLAGS} -c {source_file} -o {os.path.splitext(source_file)[0]}.o"
    subprocess.run(cmd, shell=True)

def link_shared_object(object_files):
    cmd = f"{CC} -shared {' '.join(object_files)} {LDFLAGS} -o {TARGET_SHARED_OBJECT} {LIBS}"
    subprocess.run(cmd, shell=True)

def clean():
    for obj_file in OBJECT_FILES:
        if os.path.exists(obj_file):
            os.remove(obj_file)
    if os.path.exists(TARGET_SHARED_OBJECT):
        os.remove(TARGET_SHARED_OBJECT)

def main():
    try:
        for source_file in SOURCE_FILES:
            if not os.path.exists(source_file):
                raise FileNotFoundError(f"Source file '{source_file}' not found.")

        for header in HEADERS:
            if not os.path.exists(header):
                raise FileNotFoundError(f"Header file '{header}' not found.")

        for source_file in SOURCE_FILES:
            compile_source(source_file)

        link_shared_object(OBJECT_FILES+["liblua.a"])

        #print("Compilation successful!")
    except Exception as e:
        print(f"Error: {e}")
        print("Compilation failed.")


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
        clean()
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
    
# Begin building
## If luax and luaxc already exist in /src delete them
if os.path.exists("luax"):
    os.system("rm luax")
if os.path.exists("luaxc"):
    os.system("rm luaxc")

print("\n\n\n")
print("\n\n")
print("Configuring Luax Prebuild...")
# This builds luax and luaxc but without ffi.so and usb.so
# Luax is a dependency for Luax FFI/USB library and that is a dependency for Luax
# So we have a chicken and egg problem

originalmakefile = ""
originallinit = ""
originalliblua = ""

# 1- Disable ffi.so and usb.so in the makefile
with open(MAKEFILEPATH, "r") as makefile:
    makefiledata = makefile.read()
    originalmakefile = makefiledata
    
    makefiledata = makefiledata.replace("ffi.so", "")
    makefiledata = makefiledata.replace("usb.so", "")
# 2- Write the makefile
with open(MAKEFILEPATH, "w") as makefile:
    makefile.write(makefiledata)
# 3- Remove ffi and usb fro linit.c and liblua.c
# How you will do this is that you will loop through all lines and if the line includes //SAFEDELETE then you will delete the line
global linitdata
global libluadata

with open("linit.c", "r") as linit: 
    linitdata = linit.readlines()
    originallinit = linit.read()
    for line in linitdata:
        if "//SAFEDELETE" in line:
            linitdata.remove(line)  
with open("linit.c", "w") as linit:
    linit.write("".join(linitdata))

with open("lualib.h", "r") as liblua:
    libluadata = liblua.readlines()
    originalliblua = liblua.read()
    for line in libluadata:
        if "//SAFEDELETE" in line:
            libluadata.remove(line)
with open("lualib.h", "w") as liblua:
    liblua.write("".join(libluadata))


### PREBUILD
print("Prebuilding Luax...")
exitcode = os.system("make")
if exitcode != 0:
    print("Build failed!")
    sys.exit(1)

### WAIT UNTIL PREBUILD IS DONE
while True:
    if os.path.exists("luax") and os.path.exists("luaxc"):
        break
    else:
        continue

### FFI
print("Building Luax FFI...")
main()
print("\n\n")

### USB
print("Building Luax USB library...")
os.system("cd usb")
exitcode = os.system("make")
if exitcode != 0:
    print("Build failed!")
    sys.exit(1)
    
### MAIN
# Wait until USB and FFI are done (ffi.so and usb.so)
while True:
    if os.path.exists("ffi.so"): #and os.path.exists("usb.so"):
        break
    else:
        continue
# Restore the makefile
with open(MAKEFILEPATH, "w") as makefile:
    makefile.write(originalmakefile)
# Restore linit.c and liblua.c

with open("linit.c", "w") as linit:
    linit.write(originallinit)
    print(originallinit)
with open("lualib.h", "w") as liblua:
    liblua.write(originalliblua)
    print(originalliblua)

# Go back to the main directory
os.system("cd ..")
print("\n\n")
print("Building Luax...")
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
print("2- Clean uneeded up build files")
print("3- Build again for another platform")
print("4- Move dist/luax" + res + " and dist/luaxc" + res + " to your PATH/$PATH")
print("5- Exit")
inp = input("What do you want to do? (1, 2, 3, etc.): ")

if inp == "1":
    print("\n\n\nLUAX")
    os.system("make echo")
    print("\n\n\nLUAX USB LIB")
    os.system("cd usb && make echo")
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