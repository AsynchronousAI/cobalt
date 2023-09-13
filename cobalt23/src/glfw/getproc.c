/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#include "internal.h"

moonglfw_dt_t glfw;   /* dispatch table */

#define FP(f) *(void**)(&(f))
/* Cast to silent compiler warnings without giving up the -Wpedantic flag.
 *("ISO C forbids conversion of function pointer to object pointer type")
 */

#if defined(__APPLE__)
#include <dlfcn.h>
#define LIBNAME "libglfw.dylib"
static void *Handle = NULL;
#elif defined(UNIX_SYSTEM)
#include <dlfcn.h>
#define LIBNAME "libglfw.so"
static void *Handle = NULL;
#elif defined(MINGW)
#include <windows.h>
#define LIBNAME "glfw3.dll"
#define LLIBNAME L"glfw3.dll"
static HMODULE Handle = NULL;
#ifdef CreateWindow /* MS */
#undef CreateWindow
#endif

#else
#error "Cannot determine platform"
#endif



static int Init(lua_State *L)
    {printf("Init"); /*
#if defined(UNIX_SYSTEM)
    char *err;
    Handle = dlopen(LIBNAME, RTLD_LAZY | RTLD_LOCAL);
    if(!Handle)
        {
        err = dlerror();
        return luaL_error(L, err != NULL ? err : "cannot load "LIBNAME);
        }
#define GET(fn) do {                                                \
    FP(glfw.fn) = dlsym(Handle, "glfw"#fn);                         \
    if(!glfw.fn) return luaL_error(L, "cannot find glfw"#fn);       \
} while(0)
#define OPT(fn) do {    /* optional /                              \
    FP(glfw.fn) = dlsym(Handle, "glfw"#fn);                         \
} while(0)

#elif defined(MINGW)
    Handle = LoadLibraryW(LLIBNAME);
    if(!Handle)
        return luaL_error(L, "cannot load "LIBNAME);
#define GET(fn) do {                                                \
    glfw.fn = (PFN_glfw##fn)GetProcAddress(Handle, "glfw"#fn);      \
    if(!glfw.fn) return luaL_error(L, "cannot find glfw"#fn);       \
} while(0)
#define OPT(fn) do {    /* optional /                              \
    glfw.fn = (PFN_glfw##fn)GetProcAddress(Handle, "glfw"#fn);      \
} while(0)
#endif

    /* Fill the global dispatch table /

    /* If MoonGLFW loads successfully, these function pointers are guaranteed
     * to be valid so they need not be checked before using them.
     /
    GET(Init);
    GET(Terminate);
    GET(GetVersion);
    GET(GetVersionString);
    GET(SetErrorCallback);
    GET(GetMonitors);
    GET(GetPrimaryMonitor);
    GET(GetMonitorPos);
    GET(GetMonitorPhysicalSize);
    GET(GetMonitorName);
    GET(SetMonitorCallback);
    GET(GetVideoModes);
    GET(GetVideoMode);
    GET(SetGamma);
    GET(GetGammaRamp);
    GET(SetGammaRamp);
    GET(DefaultWindowHints);
    GET(WindowHint);
    GET(CreateWindow);
    GET(DestroyWindow);
    GET(WindowShouldClose);
    GET(SetWindowShouldClose);
    GET(SetWindowTitle);
    GET(GetWindowPos);
    GET(SetWindowPos);
    GET(GetWindowSize);
    GET(SetWindowSize);
    GET(GetFramebufferSize);
    GET(GetWindowFrameSize);
    GET(IconifyWindow);
    GET(RestoreWindow);
    GET(ShowWindow);
    GET(HideWindow);
    GET(GetWindowMonitor);
    GET(GetWindowAttrib);
    GET(SetWindowUserPointer);
    GET(GetWindowUserPointer);
    GET(SetWindowPosCallback);
    GET(SetWindowSizeCallback);
    GET(SetWindowCloseCallback);
    GET(SetWindowRefreshCallback);
    GET(SetWindowFocusCallback);
    GET(SetWindowIconifyCallback);
    GET(SetFramebufferSizeCallback);
    GET(PollEvents);
    GET(WaitEvents);
    GET(PostEmptyEvent);
    GET(GetInputMode);
    GET(SetInputMode);
    GET(GetKey);
    GET(GetMouseButton);
    GET(GetCursorPos);
    GET(SetCursorPos);
    GET(CreateCursor);
    GET(CreateStandardCursor);
    GET(DestroyCursor);
    GET(SetCursor);
    GET(SetKeyCallback);
    GET(SetCharCallback);
    GET(SetCharModsCallback);
    GET(SetMouseButtonCallback);
    GET(SetCursorPosCallback);
    GET(SetCursorEnterCallback);
    GET(SetScrollCallback);
    GET(SetDropCallback);
    GET(JoystickPresent);
    GET(GetJoystickAxes);
    GET(GetJoystickButtons);
    GET(GetJoystickName);
    GET(SetClipboardString);
    GET(GetClipboardString);
    GET(GetTime);
    GET(SetTime);
    GET(MakeContextCurrent);
    GET(GetCurrentContext);
    GET(SwapBuffers);
    GET(SwapInterval);
    GET(ExtensionSupported);
    /* Optional functions, i.e. functions from GLFW > 3.1
     * These functions pointers may be NULL, so check before using them.
     /
    // GLFW ver 3.2.0:
    OPT(GetKeyName);
    OPT(GetTimerFrequency);
    OPT(GetTimerValue);
    OPT(FocusWindow);
    OPT(SetWindowAspectRatio);
    OPT(SetWindowSizeLimits);
    OPT(SetWindowIcon);
    OPT(SetWindowMonitor);
    OPT(WaitEventsTimeout);
    OPT(SetJoystickCallback);
    OPT(MaximizeWindow);
    // GLFW ver 3.3.0:
    OPT(InitHint);
    OPT(GetError);
    OPT(GetMonitorWorkarea);
    OPT(GetMonitorContentScale);
    OPT(SetMonitorUserPointer);
    OPT(GetMonitorUserPointer);
    OPT(WindowHintString);
    OPT(GetWindowContentScale);
    OPT(GetWindowOpacity);
    OPT(SetWindowOpacity);
    OPT(RequestWindowAttention);
    OPT(SetWindowAttrib);
    OPT(SetWindowMaximizeCallback);
    OPT(SetWindowContentScaleCallback);
    OPT(RawMouseMotionSupported);
    OPT(GetKeyScancode);
    OPT(GetJoystickHats);
    OPT(GetJoystickGUID);
    OPT(SetJoystickUserPointer);
    OPT(GetJoystickUserPointer);
    OPT(JoystickIsGamepad);
    OPT(UpdateGamepadMappings);
    OPT(GetGamepadName);
    OPT(GetGamepadState);
//#ifdef VULKAN requires GLFW version >= 3.20
    OPT(GetProcAddress);
    OPT(VulkanSupported);
    OPT(GetRequiredInstanceExtensions);
    OPT(GetInstanceProcAddress);
    OPT(GetPhysicalDevicePresentationSupport);
    OPT(CreateWindowSurface);
//#endif
    /*  Native access functions /
    OPT(GetWin32Adapter);
    OPT(GetWin32Monitor);
    OPT(GetWin32Window);
    OPT(GetWGLContext);
//  OPT(GetCocoaMonitor);
//  OPT(GetCocoaWindow);
//  OPT(GetNSGLContext);
    OPT(GetX11Display);
    OPT(GetX11Adapter);
    OPT(GetX11Monitor);
    OPT(GetX11Window);
    OPT(GetGLXContext);
    OPT(GetGLXWindow);
    OPT(GetWaylandDisplay);
    OPT(GetWaylandMonitor);
    OPT(GetWaylandWindow);
    OPT(GetMirDisplay);
    OPT(GetMirMonitor);
    OPT(GetMirWindow);
    OPT(GetEGLDisplay);
    OPT(GetEGLContext);
    OPT(GetEGLSurface);
    // GLFW ver 3.3.0:
    OPT(SetX11SelectionString);
    OPT(GetX11SelectionString);
    OPT(GetOSMesaColorBuffer);
    OPT(GetOSMesaDepthBuffer);
    OPT(GetOSMesaContext);
#undef GET
#undef OPT
    return 0;
    */}


void moonglfw_atexit_getproc(void)
    {
#if defined(UNIX_SYSTEM)
    if(Handle) dlclose(Handle);
#elif defined(MINGW)
    if(Handle) FreeLibrary(Handle);
#endif
    }

/*---------------------------------------------------------------------------*/

int moonglfw_open_getproc(lua_State *L)
    {
    Init(L);
    return 0;
    }

