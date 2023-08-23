  
/// Provide a simple interface to get the following information:
  // CPU Name
  // GPU Name
  // RAM Size
  // RAM Speed
  // RAM Usage
  // CPU Usage
  // GPU Usage
/// Used in device.info() and io

// Path: src/system.c
// @AsynchronousAI - 2023

#include <stdio.h>
#include <stdlib.h>

/*
#ifdef _WIN32
#include <windows.h>
#elif CTL_KERN || defined(LUA_USE_POSIX)
#include <sys/types.h> // for u_int
#include <sys/_types/_u_char.h> // for u_char

#include <sys/sysctl.h>
#else
#define DISABLED
#endif

#include "cobalt.h"
#include "lauxlib.h"

static char* getCPUName(){
  // return the CPU name
  #ifdef DISABLED
    //luaL_error("Data not available on this platform");
    return "CPU Name";
  #endif

  #ifdef _WIN32
    // Windows
    char* cpuName = (char*)malloc(sizeof(char) * 256);
    DWORD bufSize = sizeof(cpuName);
    GetComputerNameA(cpuName, &bufSize);
    return cpuName;
  #elif __APPLE__ && !defined(LUA_USE_POSIX)
    // Apple device, not a mac
    char* cpuName = (char*)malloc(sizeof(char) * 256);
    size_t bufSize = sizeof(cpuName);
    sysctlbyname("hw.machine", cpuName, &bufSize, NULL, 0);
    return cpuName;
  #elif defined(LUA_USE_POSIX) || __unix__ || __unix
    // Mac, Linux, Unix, etc.
    char* cpuName = (char*)malloc(sizeof(char) * 256);
    size_t bufSize = sizeof(cpuName);
    sysctlbyname("machdep.cpu.brand_string", cpuName, &bufSize, NULL, 0);
    return cpuName;
  #else
    //luaL_error("Data not available on this platform");
    return "CPU Name";
  #endif
}
*/