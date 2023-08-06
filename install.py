# Tool for installing LXX Binaries to the system
import os
import sys

scriptPath = os.path.dirname(os.path.realpath(__file__))
installable = []

# Check for the existence of the binaries, lxx, lxxc, lxxjit, and lua-lxx. Which ever ones are found, add them to the installable list.
print("[*] \033[32mChecking for binaries...\033[0m")
            ### Unix/Posix Specific ###
if os.path.isfile(scriptPath + "/lxx"):
    installable.append("lxx")
if os.path.isfile(scriptPath + "/lxxc"):
    installable.append("lxxc")
if os.path.isfile(scriptPath + "/lxxjit"):
    installable.append("lxxjit")
if os.path.isfile(scriptPath + "/lxx-1.0.0/lua-lxx/lua-lxx"):
    installable.append("lxx-1.0.0/lua-lxx/lua-lxx")
    
            ### Windows Specific ###
if os.path.isfile(scriptPath + "/lxx.exe"):
    installable.append("lxx.exe")
if os.path.isfile(scriptPath + "/lxxc.exe"):
    installable.append("lxxc.exe")
if os.path.isfile(scriptPath + "/lxxjit.exe"):
    installable.append("lxxjit.exe")
if os.path.isfile(scriptPath + "\\lxx-1.0.0\\lua-lxx\\lua-lxx.exe"):
    installable.append("lxx-1.0.0\\lua-lxx\\lua-lxx.exe")
    

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
    if not os.path.isdir("\\include\\lxx\\"):
        os.mkdir("\\include\\lxx\\")
    if not os.path.isdir("\\..\\include\\lxx\\"):
        os.mkdir("\\..\\include\\lxx\\")
else:
    if not os.path.isdir("/usr/local/include/lxx/"):
        os.mkdir("/usr/local/include/lxx/")
        
# Move all header files to the include path
print("[*] \033[32mMoving header files...\033[0m")
# Create alias for the following:
# lauxlib.h
# lua.h
# luaconf.h
# lualib.h
# lua.hpp

alias = ["lauxlib.h", "lua.h", "luaconf.h", "lualib.h", "lua.hpp"]
# They are located in script-dir/lxx-1.0.0/src/
for item in os.listdir(scriptPath + "/lxx-1.0.0/src/"):
    if item in alias:
        if sys.platform == "win32":
            os.system("cp " + scriptPath + "/lxx-1.0.0/src/" + item + " " + "\\include\\lxx\\" + item)
        else:
            os.system("cp " + scriptPath + "/lxx-1.0.0/src/" + item + " " + "/usr/local/include/lxx/" + item)
        
# Create library path
print("[*] \033[32mCreating library path\033[0m")
"""C Refrence:
// VDIR is "lxx"

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
    if not os.path.isdir("\\lua\\lxx\\"):
        os.mkdir("\\lua\\lxx\\")
    if not os.path.isdir("\\share\\lua\\"):
        os.mkdir("\\share\\lua\\")
    if not os.path.isdir("\\..\\share\\lua\\lxx\\"):
        os.mkdir("\\..\\share\\lua\\lxx\\")
else:
    Root = "/usr/local/"
    LDIR = Root + "share/lua/lxx/"
    CDIR = Root + "lib/lua/lxx/"
    if not os.path.isdir(LDIR):
        os.mkdir(LDIR)
    if not os.path.isdir(CDIR):
        os.mkdir(CDIR)
    if not os.path.isdir(Root + "share/lua/lxx/"):
        os.mkdir(Root + "share/lua/lxx/")
    if not os.path.isdir(Root + "lib/lua/lxx/"):
        os.mkdir(Root + "lib/lua/lxx/")
        
print("[*] \033[32mCreated library paths\033[0m")
print("[*] \033[32mInstalling standard libraries...\033[0m")

# Go through all lxx-1.0.0/lib and compile all directories
for item in os.listdir(scriptPath + "/lxx-1.0.0/lib"):
    if os.path.isdir(scriptPath + "/lxx-1.0.0/lib/" + item):
        # Change cwd to the directory
        #os.chdir(scriptPath + "/lxx-1.0.0/lib/" + item)
        # Compile the library
        #os.system("make -w")
        # Move the library to the install path
        #os.rename(scriptPath + "/lxx-1.0.0/lib/" + item + "/" + item + ".so", CDIR + item + ".so")
        pass
    elif os.path.isfile(scriptPath + "/lxx-1.0.0/lib/" + item):
        # Make sure it is a lxx file
        if item.endswith(".lxx"):
            os.system("cp " + scriptPath + "/lxx-1.0.0/lib/" + item + " " + LDIR + item)
