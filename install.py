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
if os.path.isfile(scriptPath + "/lua-lxx"):
    installable.append("lua-lxx")
    
            ### Windows Specific ###
if os.path.isfile(scriptPath + "/lxx.exe"):
    installable.append("lxx.exe")
if os.path.isfile(scriptPath + "/lxxc.exe"):
    installable.append("lxxc.exe")
if os.path.isfile(scriptPath + "/lxxjit.exe"):
    installable.append("lxxjit.exe")
if os.path.isfile(scriptPath + "/lua-lxx.exe"):
    installable.append("lua-lxx.exe")
    

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
    os.rename(scriptPath + "/" + binary, installPath + "/" + binary)