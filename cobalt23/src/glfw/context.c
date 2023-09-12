/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#include "internal.h"


static int MakeContextCurrent(lua_State *L)
    {
    win_t * win = checkwindow(L, 1);
    glfw.MakeContextCurrent(win->window);
    return 0;
    }

static int GetCurrentContext(lua_State *L)
    {
    GLFWwindow* window = glfw.GetCurrentContext();
    lua_pushinteger(L, (intptr_t)glfw.GetWindowUserPointer(window));
    return 1;
    }

static int SwapInterval(lua_State *L)
    {
    int interval = luaL_checkinteger(L, 1);
    glfw.SwapInterval(interval);
    return 0;
    }

static int ExtensionSupported(lua_State *L)
    {
    const char* extension = luaL_checkstring(L, 1);
    lua_pushboolean(L, glfw.ExtensionSupported(extension));
    return 1;
    }

#if 0
        { "get_proc_address", GetProcAddress },
static int GetProcAddress(lua_State *L) //@@ 
    {
//GLFWglproc glfw.GetProcAddress(const char* procname);
    (void)L;
    return 0;
    }
#endif


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/


static const struct luaL_Reg Functions[] = 
    {
        { "make_context_current", MakeContextCurrent },
        { "get_current_context", GetCurrentContext },
        { "swap_interval", SwapInterval },
        { "extension_supported", ExtensionSupported },
        { NULL, NULL } /* sentinel */
    };

void moonglfw_open_context(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

