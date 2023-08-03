/*
** Standard Device library for luax
*/
// (c) 2023, AsynchronousAI 

#define ldevice_c
#define LUA_LIB

#include "lprefix.h"


#include <stdlib.h>
#include <math.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

#include "system.h"

#include <string.h>
#if defined(LUA_USE_POSIX) || defined(LUA_USE_DLOPEN)
#include <sys/utsname.h>
#endif 
#if defined(LUA_USE_WINDOWS)
#include <windows.h>
#endif
#if defined(TARGET_OS_IOS)
    #include <stdio.h>
    #include <string.h>

    #include <objc/runtime.h>
    #include <objc/message.h>

    const char* get_ios_version() {
    Class UIDeviceClass = objc_getClass("UIDevice");
    if (UIDeviceClass == NULL) {
        return "Unknown";
    }
    id device = objc_msgSend((id)UIDeviceClass, sel_registerName("currentDevice"));
    if (device == NULL) {
        return "Unknown";
    }
    id systemVersion = objc_msgSend(device, sel_registerName("systemVersion"));
    if (systemVersion == NULL) {
        return "Unknown";
    }
    const char* version = ((const char*)objc_msgSend(systemVersion, sel_registerName("UTF8String")));
    if (version == NULL) {
        return "Unknown";
    }
    return version;
    }
#endif
/* TODO:
    - Detect Android device
    - Detect Xbox - Done
    - Detect PS4/PS5
    - Detect visionOS
    - Detect Nintendo Switch
    - Watchos device.info
    - Get iPadOS version
    - Get tvOS version
    - Get CPU Usage
    - Get CPU name
    - Get GPU name
    - Get GPU Usage
    - Get RAM Speed
    - Get RAM Size
    - Get RAM Usage
*/

const char *format_memory_size(int size_kb) {
  static const char *units[] = {"KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
  static const int num_units = sizeof(units) / sizeof(units[0]);
  double size = (double)size_kb;
  int unit_index = 0;
  while (size >= 1024.0 && unit_index < num_units - 1) {
    size /= 1024.0;
    unit_index++;
  }
  static char result[32];
  snprintf(result, sizeof(result), "%.2f %s", size, units[unit_index]);
  return result;
}

static int get_device_info(lua_State* L) {
  int disabled = 0;
  int platform = 0;
  #if !(defined(LUA_USE_WINDOWS) || defined(LUA_USE_POSIX) || defined(LUA_USE_DLOPEN))
    disabled = 1;
  #endif
  if (disabled == 1) {
    return luaL_error(L, "Device information is not accessible on this platform");
  }
  // Get the number of arguments
  int argc = lua_gettop(L);

  #if defined(LUA_USE_WINDOWS)
    OSVERSIONINFOEX info;
    ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    // Return all system information
        lua_newtable(L);
        if (GetVersionEx((LPOSVERSIONINFO)&info)) {
            lua_pushstring(L, "release");
            lua_pushstring(L, info.szCSDVersion);
            lua_settable(L, -3);
            lua_pushstring(L, "version");
            lua_pushstring(L, info.szCSDVersion);
            lua_settable(L, -3);
        } else {
            lua_pushstring(L, "release");
            lua_pushstring(L, "Unknown");
            lua_settable(L, -3);
            lua_pushstring(L, "version");
            lua_pushstring(L, "Unknown");
            lua_settable(L, -3);
        }
        char computerName[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = sizeof(computerName);
        if (GetComputerName(computerName, &size)) {
            lua_pushstring(L, "nodename");
            lua_pushstring(L, computerName);
            lua_settable(L, -3);
        } else {
            lua_pushstring(L, "nodename");
            lua_pushstring(L, "Unknown");
            lua_settable(L, -3);
        }
        SYSTEM_INFO info;
        GetNativeSystemInfo(&info);
        switch (info.wProcessorArchitecture) {
            case PROCESSOR_ARCHITECTURE_INTEL:
            lua_pushstring(L, "machine");
            lua_pushstring(L, "x86");
            lua_settable(L, -3);
            break;
            case PROCESSOR_ARCHITECTURE_ARM:
            lua_pushstring(L, "machine");
            lua_pushstring(L, "Arm");
            lua_settable(L, -3);
            break;
            case PROCESSOR_ARCHITECTURE_AMD64:
            lua_pushstring(L, "machine");
            lua_pushstring(L, "x64");
            lua_settable(L, -3);
            break;
            default:
            lua_pushstring(L, "machine");
            lua_pushstring(L, "Unknown");
            lua_settable(L, -3);
            break;
        }
        lua_pushstring(L, "generalos");
        lua_pushstring(L, "windows");
        lua_settable(L, -3);
        lua_pushstring(L, "sysname");
        lua_pushstring(L, "Windows");
        lua_settable(L, -3);
        lua_pushstring(L, "scriptmemoryint"); // How much memory is luax using
        lua_pushnumber(L, lua_gc(L, LUA_GCCOUNT, 0));
        lua_settable(L, -3);
        lua_pushstring(L, "scriptmemory");
        lua_pushfstring(L, "%s", format_memory_size(lua_gc(L, LUA_GCCOUNT, 0)));
        lua_settable(L, -3);
        return 1;

    return 1;
  #elif defined(LUA_USE_POSIX) || defined(LUA_USE_DLOPEN)
    // Get the system information
    struct utsname info;
    uname(&info);
    char* suffix = ".local";
    char* pos = strstr(info.machine, suffix);
    if (pos != NULL) {
        *pos = '\0';
    }
    
    lua_newtable(L);
    lua_pushstring(L, "sysname");
    lua_pushstring(L, info.sysname);
    lua_settable(L, -3);
    lua_pushstring(L, "nodename");
    lua_pushstring(L, info.nodename);
    lua_settable(L, -3);
    lua_pushstring(L, "release");
    lua_pushstring(L, info.release);
    lua_settable(L, -3);
    lua_pushstring(L, "version");
    lua_pushstring(L, info.version);
    lua_settable(L, -3);
    lua_pushstring(L, "machine");
    lua_pushstring(L, info.machine);
    lua_settable(L, -3);
    lua_pushstring(L, "generalos");
    lua_pushstring(L, "unix");
    lua_settable(L, -3);
    lua_pushstring(L, "scriptmemoryint"); // How much memory is luax using
    lua_pushnumber(L, lua_gc(L, LUA_GCCOUNT, 0));
    lua_settable(L, -3);
    lua_pushstring(L, "scriptmemory");
    lua_pushfstring(L, "%s", format_memory_size(lua_gc(L, LUA_GCCOUNT, 0)));
    lua_settable(L, -3);
    return 1;
  #elif defined(__APPLE__)
   #if defined(TARGET_OS_IOS)
    lua_newtable(L);
    lua_pushstring(L, "sysname");
    lua_pushstring(L, "iOS");
    lua_settable(L, -3);
    lua_pushstring(L, "nodename");
    lua_pushstring(L, "Unknown");
    lua_settable(L, -3);
    lua_pushstring(L, "release");
    lua_pushstring(L, get_ios_version());
    lua_settable(L, -3);
    lua_pushstring(L, "version");
    lua_pushstring(L, get_ios_version());
    lua_settable(L, -3);
    lua_pushstring(L, "machine");
    lua_pushstring(L, "Unknown");
    lua_settable(L, -3);
    lua_pushstring(L, "generalos");
    lua_pushstring(L, "ios");
    lua_settable(L, -3);
    lua_pushstring(L, "scriptmemoryint"); // How much memory is luax using
    lua_pushnumber(L, lua_gc(L, LUA_GCCOUNT, 0));
    lua_settable(L, -3);
    lua_pushstring(L, "scriptmemory");
    lua_pushfstring(L, "%s", format_memory_size(lua_gc(L, LUA_GCCOUNT, 0)));
    lua_settable(L, -3);
    return 1;
  #elif defined(TARGET_OS_TV)
    lua_newtable(L);
    lua_pushstring(L, "sysname");
    lua_pushstring(L, "tvOS");
    lua_settable(L, -3);
    lua_pushstring(L, "nodename");
    lua_pushstring(L, "Unknown");
    lua_settable(L, -3);
    lua_pushstring(L, "release");
    lua_pushstring(L, get_ios_version());
    lua_settable(L, -3);
    lua_pushstring(L, "version");
    lua_pushstring(L, get_ios_version());
    lua_settable(L, -3);
    lua_pushstring(L, "machine");
    lua_pushstring(L, "Unknown");
    lua_settable(L, -3);
    lua_pushstring(L, "generalos");
    lua_pushstring(L, "tvos");
    lua_settable(L, -3);
    lua_pushstring(L, "scriptmemoryint"); // How much memory is luax using
    lua_pushnumber(L, lua_gc(L, LUA_GCCOUNT, 0));
    lua_settable(L, -3);
    lua_pushstring(L, "scriptmemory");
    lua_pushfstring(L, "%s", format_memory_size(lua_gc(L, LUA_GCCOUNT, 0)));
    lua_settable(L, -3);
    return 1;
   #endif
  #else
    lua_newtable(L);
    lua_pushstring(L, "sysname");
    lua_pushstring(L, "Unknown");
    lua_settable(L, -3);
    lua_pushstring(L, "nodename");
    lua_pushstring(L, "Unknown");
    lua_settable(L, -3);
    lua_pushstring(L, "release");
    lua_pushstring(L, "Unknown");
    lua_settable(L, -3);
    lua_pushstring(L, "version");
    lua_pushstring(L, "Unknown");
    lua_settable(L, -3);
    lua_pushstring(L, "machine");
    lua_pushstring(L, "Unknown");
    lua_settable(L, -3);
    lua_pushstring(L, "generalos");
    lua_pushstring(L, "Unknown");
    lua_settable(L, -3);
    lua_pushstring(L, "scriptmemoryint"); // How much memory is luax using
    lua_pushnumber(L, lua_gc(L, LUA_GCCOUNT, 0));
    lua_settable(L, -3);
    lua_pushstring(L, "scriptmemory");
    lua_pushfstring(L, "%s", format_memory_size(lua_gc(L, LUA_GCCOUNT, 0)));
    lua_settable(L, -3);
    return 1;
  #endif
}

static const struct luaL_Reg device_lib[] = {
  {"info", get_device_info},

  {NULL, NULL}
};

LUALIB_API int luaopen_device(lua_State* L) {
  luaL_newlib(L, device_lib);
  #if defined(LUA_USE_WINDOWS)
    lua_pushstring(L, "Windows");
    lua_setfield(L, -2, "os");
  #elif defined(__APPLE__)
    #if TARGET_OS_IOS
      lua_pushstring(L, "iOS");
    #elif TARGET_OS_TV
        lua_pushstring(L, "tvOS");
    #elif TARGET_OS_WATCH
        lua_pushstring(L, "watchOS");
    #elif TARGET_OS_MAC // Note for language hackers: TARGET_OS_MAC only work when compiled using Xcode
        lua_pushstring(L, "macOS");
    #elif TARGET_OS_IPHONE
      lua_pushstring(L, "iPadOS");
    #else
      lua_pushstring(L, "Apple OS");
    #endif
    lua_setfield(L, -2, "os");
  #elif defined(LUA_USE_POSIX) || defined(LUA_USE_DLOPEN)
    struct utsname info;
    uname(&info);
    lua_pushstring(L, info.sysname);
    lua_setfield(L, -2, "os");
  #elif defined(_XBOX_ONE) && defined(_TITLE)
   lua_pushstring(L, "Xbox One");
  #elif defined(_DURANGO)
   lua_pushstring(L, "Xbox One X");
  #else
    lua_pushstring(L, "Unknown");
    lua_setfield(L, -2, "os");
  #endif

  return 1;
}