/*
** Standard Device library for luax
*/

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
        }
        lua_pushstring(L, "generalos");
        lua_pushstring(L, "windows");
        lua_settable(L, -3);
        return 1;
    
    return 1;
  #endif
  #if defined(LUA_USE_POSIX) || defined(LUA_USE_DLOPEN)
    // Get the system information
    struct utsname info;
    uname(&info);
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
    return 1;
#endif
}

static const struct luaL_Reg device_lib[] = {
  {"info", get_device_info},
  {NULL, NULL}
};

LUALIB_API int luaopen_device(lua_State* L) {
  luaL_newlib(L, device_lib);
  return 1;
}