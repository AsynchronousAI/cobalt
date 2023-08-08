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
if os.path.isfile(scriptPath + "/cobalt-1.0.0/jit/src/cobaltjit"):
    print("[*] \033[32mFound cobaltjit binary!\033[0m")
    installable.append("cobalt-1.0.0/jit/src/cobaltjit")
    jitFound = True
if os.path.isfile(scriptPath + "/cobalt-1.0.0/lua-cobalt/lua-cobalt"):
    print("[*] \033[32mFound lua-cobalt binary!\033[0m")
    installable.append("cobalt-1.0.0/lua-cobalt/lua-cobalt")
    
            ### Windows Specific ###
if os.path.isfile(scriptPath + "/cobalt.exe"):
    print("[*] \033[32mFound cobalt binary!\033[0m")
    installable.append("cobalt.exe")
if os.path.isfile(scriptPath + "/cobaltc.exe"):
    print("[*] \033[32mFound cobaltc binary!\033[0m")
    installable.append("cobaltc.exe")
if os.path.isfile(scriptPath + "/cobaltjit.exe"):
    print("[*] \033[32mFound cobaltjit binary!\033[0m")
    installable.append("cobaltjit.exe")
    jitFound = True
if os.path.isfile(scriptPath + "/cobalt-1.0.0\\lua-cobalt\\lua-cobalt.exe"):
    print("[*] \033[32mFound lua-cobalt binary!\033[0m")
    installable.append("cobalt-1.0.0\\lua-cobalt\\lua-cobalt.exe")
    

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
# lua.h
# luaconf.h
# lualib.h
# lua.hpp

alias = ["lauxlib.h", "lua.h", "luaconf.h", "lualib.h", "lua.hpp"]
# They are located in script-dir/cobalt-1.0.0/src/
for item in os.listdir(scriptPath + "/cobalt-1.0.0/src/"):
    if item in alias:
        if sys.platform == "win32":
            os.system("cp " + scriptPath + "/cobalt-1.0.0/src/" + item + " " + "\\include\\cobalt\\" + item)
        else:
            os.system("cp " + scriptPath + "/cobalt-1.0.0/src/" + item + " " + "/usr/local/include/cobalt/" + item)
        
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

# Go through all cobalt-1.0.0/lib and compile all directories
for item in os.listdir(scriptPath + "/cobalt-1.0.0/lib"):
    if os.path.isdir(scriptPath + "/cobalt-1.0.0/lib/" + item):
        if item == "jit":
            print("[*] \033[32mFound JIT Library\033[0m")
            # Duplicate JIT library directory to the install path
            os.system("cp -r " + scriptPath + "/cobalt-1.0.0/lib/" + item + " " + CDIR + item)
            continue
        # Change cwd to the directory
        os.chdir(scriptPath + "/cobalt-1.0.0/lib/" + item)
        # Compile the library
        os.system("make -w")
        # Move the library to the install path
        os.rename(scriptPath + "/cobalt-1.0.0/lib/" + item + "/" + item + ".so", CDIR + item + ".so")
        print("\033[33m[!] " + item + " is not a C script and unsupported right now. Skipping...\033[0m")
        pass
    elif os.path.isfile(scriptPath + "/cobalt-1.0.0/lib/" + item):
        # Make sure it is a cobalt file
        if item.endswith(".cblt"):
            os.system("cp " + scriptPath + "/cobalt-1.0.0/lib/" + item + " " + LDIR + item)
