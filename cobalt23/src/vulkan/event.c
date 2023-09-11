/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "internal.h"

static int freeevent(lua_State *L, ud_t *ud)
    {
    VkEvent event = (VkEvent)ud->handle;
    const VkAllocationCallbacks *allocator = ud->allocator;
    VkDevice device = ud->device;
    if(!freeuserdata(L, ud))
        return 0; /* double call */
    TRACE_DELETE(event, "event");
    UD(device)->ddt->DestroyEvent(device, event, allocator);
    return 0;
    }

static int Create(lua_State *L)
    {
    int err;
    ud_t *ud, *device_ud;
    VkResult ec;
    VkEvent event;
    VkEventCreateInfo* info;
    VkDevice device = checkdevice(L, 1, &device_ud);
    const VkAllocationCallbacks *allocator = NULL;
#define CLEANUP zfreeVkEventCreateInfo(L, info, 1)
    if(lua_istable(L, 2))
        {
        allocator = optallocator(L, 3);
        info = zcheckVkEventCreateInfo(L, 2, &err);
        if(err) { CLEANUP; return argerror(L, 2); }
        }
    else
        {
        VkFlags flags = optflags(L, 2, 0);
        info = znewVkEventCreateInfo(L, &err);
        if(err) { CLEANUP; return lua_error(L); }
        info->flags = flags;
        }
    ec = device_ud->ddt->CreateEvent(device, info, allocator, &event);
    CLEANUP;
    CheckError(L, ec);
#undef CLEANUP
    TRACE_CREATE(event, "event");
    ud = newuserdata_nondispatchable(L, event, EVENT_MT);
    ud->parent_ud = device_ud;
    ud->device = device;
    ud->instance = device_ud->instance;
    ud->allocator = allocator;
    ud->destructor = freeevent;
    ud->ddt = device_ud->ddt;
    return 1;
    }

static int GetEventStatus(lua_State *L)
    {
    ud_t *ud;
    VkEvent event = checkevent(L, 1, &ud);
    VkDevice device = ud->device;
    VkResult ec = ud->ddt->GetEventStatus(device, event);
    switch(ec)
        {
        case VK_EVENT_SET: lua_pushboolean(L, 1); return 1;
        case VK_EVENT_RESET: lua_pushboolean(L, 0); return 1;
        default:
            CheckError(L, ec);
        }
    return 0;
    }

static int SetEvent_windows (lua_State *L)
    {
    ud_t *ud;
    VkEvent event = checkevent(L, 1, &ud);
    VkDevice device = ud->device;
    VkResult ec = ud->ddt->SetEvent(device, event);
    CheckError(L, ec);
    return 0;
    }

static int ResetEvent_windows (lua_State *L)
    {
    ud_t *ud;
    VkEvent event = checkevent(L, 1, &ud);
    VkDevice device = ud->device;
    VkResult ec = ud->ddt->ResetEvent(device, event);
    CheckError(L, ec);
    return 0;
    }

RAW_FUNC(event)
TYPE_FUNC(event)
INSTANCE_FUNC(event)
DEVICE_FUNC(event)
PARENT_FUNC(event)
DELETE_FUNC(event)
DESTROY_FUNC(event)

static const struct luaL_Reg Methods[] = 
    {
        { "raw", Raw },
        { "type", Type },
        { "instance", Instance },
        { "device", Device },
        { "parent", Parent },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Delete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "create_event",  Create },
        { "destroy_event",  Destroy },
        { "get_event_status", GetEventStatus },
        { "set_event", SetEvent_windows },
        { "reset_event", ResetEvent_windows },
        { NULL, NULL } /* sentinel */
    };


void moonvulkan_open_event(lua_State *L)
    {
    udata_define(L, EVENT_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

