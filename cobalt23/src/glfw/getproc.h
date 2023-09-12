/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#ifndef getprocDEFINED
#define getprocDEFINED

#include "pfn.h"

#define CheckPfn(L, pfn, major, minor, patch) do {          \
    if(glfw.pfn==NULL)                                      \
        {                                                   \
        if(!checkminversion((major), (minor), (patch)))     \
            return luaL_error((L),                          \
                "glfw"#pfn" requires GLFW version >= %d.%d.%d", (major), (minor), (patch));\
        else                                                \
            return luaL_error((L), "glfw"#pfn" address not found");  \
        }                                                   \
} while(0)

#define CheckVulkanPfn(L, pfn) CheckPfn(L, pfn, 3, 2, 0)

#define CheckNativePfn(L, pfn) do {                             \
    if(glfw.pfn==NULL)                                          \
        return luaL_error((L), "glfw"#pfn" address not found"); \
} while(0)


/* Dispatch tables */

/* Global functions */
typedef struct {
    PFN_glfwInit Init;
    PFN_glfwTerminate Terminate;
    PFN_glfwGetVersion GetVersion;
    PFN_glfwGetVersionString GetVersionString;
    PFN_glfwSetErrorCallback SetErrorCallback;
    PFN_glfwGetMonitors GetMonitors;
    PFN_glfwGetPrimaryMonitor GetPrimaryMonitor;
    PFN_glfwGetMonitorPos GetMonitorPos;
    PFN_glfwGetMonitorPhysicalSize GetMonitorPhysicalSize;
    PFN_glfwGetMonitorName GetMonitorName;
    PFN_glfwSetMonitorCallback SetMonitorCallback;
    PFN_glfwGetVideoModes GetVideoModes;
    PFN_glfwGetVideoMode GetVideoMode;
    PFN_glfwSetGamma SetGamma;
    PFN_glfwGetGammaRamp GetGammaRamp;
    PFN_glfwSetGammaRamp SetGammaRamp;
    PFN_glfwDefaultWindowHints DefaultWindowHints;
    PFN_glfwWindowHint WindowHint;
    PFN_glfwCreateWindow CreateWindow;
    PFN_glfwDestroyWindow DestroyWindow;
    PFN_glfwWindowShouldClose WindowShouldClose;
    PFN_glfwSetWindowShouldClose SetWindowShouldClose;
    PFN_glfwSetWindowTitle SetWindowTitle;
    PFN_glfwSetWindowIcon SetWindowIcon;
    PFN_glfwGetWindowPos GetWindowPos;
    PFN_glfwSetWindowPos SetWindowPos;
    PFN_glfwGetWindowSize GetWindowSize;
    PFN_glfwSetWindowSizeLimits SetWindowSizeLimits;
    PFN_glfwSetWindowAspectRatio SetWindowAspectRatio;
    PFN_glfwSetWindowSize SetWindowSize;
    PFN_glfwGetFramebufferSize GetFramebufferSize;
    PFN_glfwGetWindowFrameSize GetWindowFrameSize;
    PFN_glfwIconifyWindow IconifyWindow;
    PFN_glfwRestoreWindow RestoreWindow;
    PFN_glfwMaximizeWindow MaximizeWindow;
    PFN_glfwShowWindow ShowWindow;
    PFN_glfwHideWindow HideWindow;
    PFN_glfwFocusWindow FocusWindow;
    PFN_glfwGetWindowMonitor GetWindowMonitor;
    PFN_glfwSetWindowMonitor SetWindowMonitor;
    PFN_glfwGetWindowAttrib GetWindowAttrib;
    PFN_glfwSetWindowUserPointer SetWindowUserPointer;
    PFN_glfwGetWindowUserPointer GetWindowUserPointer;
    PFN_glfwSetWindowPosCallback SetWindowPosCallback;
    PFN_glfwSetWindowSizeCallback SetWindowSizeCallback;
    PFN_glfwSetWindowCloseCallback SetWindowCloseCallback;
    PFN_glfwSetWindowRefreshCallback SetWindowRefreshCallback;
    PFN_glfwSetWindowFocusCallback SetWindowFocusCallback;
    PFN_glfwSetWindowIconifyCallback SetWindowIconifyCallback;
    PFN_glfwSetFramebufferSizeCallback SetFramebufferSizeCallback;
    PFN_glfwPollEvents PollEvents;
    PFN_glfwWaitEvents WaitEvents;
    PFN_glfwWaitEventsTimeout WaitEventsTimeout;
    PFN_glfwPostEmptyEvent PostEmptyEvent;
    PFN_glfwGetInputMode GetInputMode;
    PFN_glfwSetInputMode SetInputMode;
    PFN_glfwGetKeyName GetKeyName;
    PFN_glfwGetKey GetKey;
    PFN_glfwGetMouseButton GetMouseButton;
    PFN_glfwGetCursorPos GetCursorPos;
    PFN_glfwSetCursorPos SetCursorPos;
    PFN_glfwCreateCursor CreateCursor;
    PFN_glfwCreateStandardCursor CreateStandardCursor;
    PFN_glfwDestroyCursor DestroyCursor;
    PFN_glfwSetCursor SetCursor;
    PFN_glfwSetKeyCallback SetKeyCallback;
    PFN_glfwSetCharCallback SetCharCallback;
    PFN_glfwSetCharModsCallback SetCharModsCallback;
    PFN_glfwSetMouseButtonCallback SetMouseButtonCallback;
    PFN_glfwSetCursorPosCallback SetCursorPosCallback;
    PFN_glfwSetCursorEnterCallback SetCursorEnterCallback;
    PFN_glfwSetScrollCallback SetScrollCallback;
    PFN_glfwSetDropCallback SetDropCallback;
    PFN_glfwJoystickPresent JoystickPresent;
    PFN_glfwGetJoystickAxes GetJoystickAxes;
    PFN_glfwGetJoystickButtons GetJoystickButtons;
    PFN_glfwGetJoystickName GetJoystickName;
    PFN_glfwSetJoystickCallback SetJoystickCallback;
    PFN_glfwSetClipboardString SetClipboardString;
    PFN_glfwGetClipboardString GetClipboardString;
    PFN_glfwGetTime GetTime;
    PFN_glfwSetTime SetTime;
    PFN_glfwGetTimerValue GetTimerValue;
    PFN_glfwGetTimerFrequency GetTimerFrequency;
    PFN_glfwMakeContextCurrent MakeContextCurrent;
    PFN_glfwGetCurrentContext GetCurrentContext;
    PFN_glfwSwapBuffers SwapBuffers;
    PFN_glfwSwapInterval SwapInterval;
    PFN_glfwExtensionSupported ExtensionSupported;
    // GLFW 3.3
    PFN_glfwInitHint InitHint;
    PFN_glfwGetError GetError;
    PFN_glfwGetMonitorWorkarea GetMonitorWorkarea;
    PFN_glfwGetMonitorContentScale GetMonitorContentScale;
    PFN_glfwSetMonitorUserPointer SetMonitorUserPointer;
    PFN_glfwGetMonitorUserPointer GetMonitorUserPointer;
    PFN_glfwWindowHintString WindowHintString;
    PFN_glfwGetWindowContentScale GetWindowContentScale;
    PFN_glfwGetWindowOpacity GetWindowOpacity;
    PFN_glfwSetWindowOpacity SetWindowOpacity;
    PFN_glfwRequestWindowAttention RequestWindowAttention;
    PFN_glfwSetWindowAttrib SetWindowAttrib;
    PFN_glfwSetWindowMaximizeCallback SetWindowMaximizeCallback;
    PFN_glfwSetWindowContentScaleCallback SetWindowContentScaleCallback;
    PFN_glfwRawMouseMotionSupported RawMouseMotionSupported;
    PFN_glfwGetKeyScancode GetKeyScancode;
    PFN_glfwGetJoystickHats GetJoystickHats;
    PFN_glfwGetJoystickGUID GetJoystickGUID;
    PFN_glfwSetJoystickUserPointer SetJoystickUserPointer;
    PFN_glfwGetJoystickUserPointer GetJoystickUserPointer;
    PFN_glfwJoystickIsGamepad JoystickIsGamepad;
    PFN_glfwUpdateGamepadMappings UpdateGamepadMappings;
    PFN_glfwGetGamepadName GetGamepadName;
    PFN_glfwGetGamepadState GetGamepadState;

//#ifdef VULKAN
    PFN_glfwGetProcAddress GetProcAddress;
    PFN_glfwVulkanSupported VulkanSupported;
    PFN_glfwGetRequiredInstanceExtensions GetRequiredInstanceExtensions;
    PFN_glfwGetInstanceProcAddress GetInstanceProcAddress;
    PFN_glfwGetPhysicalDevicePresentationSupport GetPhysicalDevicePresentationSupport;
    PFN_glfwCreateWindowSurface CreateWindowSurface;
//#endif
//  Native access functions
    PFN_glfwGetWin32Adapter GetWin32Adapter;
    PFN_glfwGetWin32Monitor GetWin32Monitor;
    PFN_glfwGetWin32Window GetWin32Window;
    PFN_glfwGetWGLContext GetWGLContext;
//  PFN_glfwGetCocoaMonitor GetCocoaMonitor;
//  PFN_glfwGetCocoaWindow GetCocoaWindow;
//  PFN_glfwGetNSGLContext GetNSGLContext;
    PFN_glfwGetX11Display GetX11Display;
    PFN_glfwGetX11Adapter GetX11Adapter;
    PFN_glfwGetX11Monitor GetX11Monitor;
    PFN_glfwGetX11Window GetX11Window;
    PFN_glfwGetGLXContext GetGLXContext;
    PFN_glfwGetGLXWindow GetGLXWindow;
    PFN_glfwGetWaylandDisplay GetWaylandDisplay;
    PFN_glfwGetWaylandMonitor GetWaylandMonitor;
    PFN_glfwGetWaylandWindow GetWaylandWindow;
    PFN_glfwGetMirDisplay GetMirDisplay;
    PFN_glfwGetMirMonitor GetMirMonitor;
    PFN_glfwGetMirWindow GetMirWindow;
    PFN_glfwGetEGLDisplay GetEGLDisplay;
    PFN_glfwGetEGLContext GetEGLContext;
    PFN_glfwGetEGLSurface GetEGLSurface;
    // GLFW 3.3
    PFN_glfwSetX11SelectionString SetX11SelectionString;
    PFN_glfwGetX11SelectionString GetX11SelectionString;
    PFN_glfwGetOSMesaColorBuffer GetOSMesaColorBuffer;
    PFN_glfwGetOSMesaDepthBuffer GetOSMesaDepthBuffer;
    PFN_glfwGetOSMesaContext GetOSMesaContext;
} moonglfw_dt_t;

#define glfw moonglfw_glfw
extern moonglfw_dt_t glfw;

#endif /* getprocDEFINED */




