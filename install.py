# Tool for installing cobalt Binaries to the system
import os
import sys

scriptPath = os.path.dirname(os.path.realpath(__file__))
installable = []
jitFound = False

# Check for the existence of the binaries, cobalt, cobaltc, cobaltjit, and lua-cobalt. Which ever ones are found, add them to the installable list.
print("[*] \033[32mChecking for binaries...\033[0m")
            ### Unix/Posix Specific ###
if os.path.isfile(scriptPath + "/cobalt"):
    print("[*] \033[32mFound cobalt binary!\033[0m")
    installable.append("cobalt")
if os.path.isfile(scriptPath + "/cobaltc"):
    print("[*] \033[32mFound cobaltc binary!\033[0m")
    installable.append("cobaltc")
if os.path.isfile(scriptPath + "/cobalt23/lua-cobalt/lua-cobalt"):
    print("[*] \033[32mFound lua-cobalt binary!\033[0m")
    installable.append("cobalt23/lua-cobalt/lua-cobalt")
if os.path.isfile(scriptPath + "/cobalt23/pre/cobaltpre"):
    print("[*] \033[32mFound cobalt preprocessor!\033[0m")
    installable.append("cobalt23/pre/cobaltpre")
    
            ### Windows Specific ###
if os.path.isfile(scriptPath + "/cobalt.exe"):
    print("[*] \033[32mFound cobalt binary!\033[0m")
    installable.append("cobalt.exe")
if os.path.isfile(scriptPath + "/cobaltc.exe"):
    print("[*] \033[32mFound cobaltc binary!\033[0m")
    installable.append("cobaltc.exe")
if os.path.isfile(scriptPath + "/cobalt23\\lua-cobalt\\lua-cobalt.exe"):
    print("[*] \033[32mFound lua-cobalt binary!\033[0m")
    installable.append("cobalt23\\lua-cobalt\\lua-cobalt.exe")
if os.path.isfile(scriptPath + "/cobalt23/pre/cobaltpre.exe"):
    print("[*] \033[32mFound cobalt preprocessor!\033[0m")
    installable.append("cobalt23/pre/cobaltpre.exe")
    

# Get the install path based on the users operating systems $PATH/%PATH%/etc. 
print("[*] \033[32mGetting install path...\033[0m")
installPath = ""

if sys.platform == "win32":
    installPath = os.getenv("PATH") # It will provide alot so pick one
    items = installPath.split(";")
    installPath = items[0]
elif sys.platform == "linux" or sys.platform == "linux2" or sys.platform == "darwin":
    installPath = os.getenv("PATH") # It will provide alot so pick one
    items = installPath.split(":")
    installPath = items[0]
else:
    print("Unsupported OS. Please provide the install path manually.")
    installPath = input("Install Path: ")
    
# Move each binary to the install path
print("[*] \033[32mInstalling binaries...\033[0m")
for binary in installable:
    oldbinary = binary
    if binary.endswith(".exe"):
        binary = binary.split("\\")[-1]
    else:
        binary = binary.split("/")[-1]
    
    if oldbinary == binary:
        os.rename(scriptPath + "/" + binary, installPath + "/" + binary)
    else:
        os.rename(scriptPath + "/" + oldbinary, installPath + "/" + binary)
    
    
####################################################################
### Binaries installed! Now install the libraries and header files #
####################################################################
            
# Create include path
print("[*] \033[32mCreating include path...\033[0m")
if sys.platform == "win32":
    if not os.path.isdir("\\include\\"):
        os.mkdir("\\include\\")
    if not os.path.isdir("\\include\\cobalt\\"):
        os.mkdir("\\include\\cobalt\\")
    if not os.path.isdir("\\..\\include\\cobalt\\"):
        os.mkdir("\\..\\include\\cobalt\\")
else:
    if not os.path.isdir("/usr/local/include/cobalt/"):
        os.mkdir("/usr/local/include/cobalt/")
        
# Move all header files to the include path
print("[*] \033[32mMoving header files...\033[0m")
# Create alias for the following:
# lauxlib.h
# cobalt.h
# luaconf.h
# lualib.h
# cobalt.hpp

alias = ["lauxlib.h", "cobalt.h", "luaconf.h", "lualib.h", "cobalt.hpp"]
# They are located in script-dir/cobalt23/src/
for item in os.listdir(scriptPath + "/cobalt23/src/"):
    if item in alias:
        if sys.platform == "win32":
            os.system("cp " + scriptPath + "/cobalt23/src/" + item + " " + "\\include\\cobalt\\" + item)
        else:
            os.system("cp " + scriptPath + "/cobalt23/src/" + item + " " + "/usr/local/include/cobalt/" + item)
        
# Create library path
print("[*] \033[32mCreating library path\033[0m")
"""C Refrence:
// VDIR is "cobalt"

// Unix

#define LUA_ROOT	"/usr/local/"
#define LUA_LDIR	LUA_ROOT "share/lua/" LUA_VDIR "/"
#define LUA_CDIR	LUA_ROOT "lib/lua/" LUA_VDIR "/"

// Windows
#define LUA_LDIR	"!\\lua\\"
#define LUA_CDIR	"!\\"
#define LUA_SHRDIR	"!\\..\\share\\lua\\" LUA_VDIR "\\"
"""

if sys.platform == "win32":
    print("[*] \033[32mRunning windows operations...\033[0m")
    # Check if the LDIR and CDIR exists, if either dont exist, create them
    if not os.path.isdir("\\lua\\"):
        os.mkdir("\\lua\\")
    if not os.path.isdir("\\lua\\cobalt\\"):
        os.mkdir("\\lua\\cobalt\\")
    if not os.path.isdir("\\share\\lua\\"):
        os.mkdir("\\share\\lua\\")
    if not os.path.isdir("\\..\\share\\lua\\cobalt\\"):
        os.mkdir("\\..\\share\\lua\\cobalt\\")
else:
    Root = "/usr/local/"
    LDIR = Root + "share/lua/cobalt/"
    CDIR = Root + "lib/lua/cobalt/"
    if not os.path.isdir(LDIR):
        os.mkdir(LDIR)
    if not os.path.isdir(CDIR):
        os.mkdir(CDIR)
    if not os.path.isdir(Root + "share/lua/cobalt/"):
        os.mkdir(Root + "share/lua/cobalt/")
    if not os.path.isdir(Root + "lib/lua/cobalt/"):
        os.mkdir(Root + "lib/lua/cobalt/")
        
print("[*] \033[32mCreated library paths\033[0m")
print("[*] \033[32mInstalling standard libraries...\033[0m")
# If the install path has items clear themq
if os.listdir(LDIR) != []:
    print("[!] \033[33mWarning: Library path is not empty.\033[0m")
elif os.listdir(CDIR) != []:
    print("[!] \033[33mWarning: Library path is not empty.\033[0m")
        
# Go through all cobalt23/lib and compile all directories
for item in os.listdir(scriptPath + "/cobalt23/lib"):
    if os.path.isdir(scriptPath + "/cobalt23/lib/" + item):
        if item == "DISABLED":
            continue
        else:
            print("[*] \033[32mInstalling " + item + "\033[0m")
        # Otherwise check if a Makefile exists
        if os.path.isfile(scriptPath + "/cobalt23/lib/" + item + "/Makefile"):
            print("[*] \033[32mBuilding Makefile for " + item + "\033[0m")
            # Change cwd to the directory
            os.chdir(scriptPath + "/cobalt23/lib/" + item)
            # Compile the library
            os.system("make -w")
            # Move the library to the install path
            # Verify .dll or .so exists
            if not os.path.isfile(scriptPath + "/cobalt23/lib/" + item + "/" + item + ".dll") and not os.path.isfile(scriptPath + "/cobalt23/lib/" + item + "/" + item + ".so"):
                print("[!] \033[33mWarning: Library " + item + " was not compiled correctly.\033[0m")
                continue
            
            if sys.platform == "win32":
                os.rename(scriptPath + "/cobalt23/lib/" + item + "/" + item + ".dll", LDIR + item + ".dll")
            else:
                os.rename(scriptPath + "/cobalt23/lib/" + item + "/" + item + ".so", LDIR + item + ".so")
            print("[*] \033[32mInstalled " + item + "\033[0m")
            continue
        else:   
            # Simply copy the directory to the install path as it is a pure cobalt library
            os.system("cp -r " + scriptPath + "/cobalt23/lib/" + item + " " + LDIR + item)
            # if a /jit is available in the install path also copy it there
            #if jitFound:
            #    if os.path.isdir(LDIR + "jit"):
            #        os.system("cp -r " + scriptPath + "/cobalt23/lib/" + item + "/jit " + LDIR + item + "/jit")
            #    elif os.path.isdir(scriptPath + "/cobalt23/lib/" + item + "/jit"):
            #        os.system("cp -r " + scriptPath + "/cobalt23/lib/" + item + "/jit " + LDIR + item + "/jit")
            continue
        pass
    elif os.path.isfile(scriptPath + "/cobalt23/lib/" + item):
        # Make sure it is a cobalt file
        if item.endswith(".cobalt"):
            os.system("cp " + scriptPath + "/cobalt23/lib/" + item + " " + LDIR + item)
