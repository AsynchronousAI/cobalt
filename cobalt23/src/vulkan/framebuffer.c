/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "internal.h"

static int freeframebuffer(lua_State *L, ud_t *ud)
    {
    VkFramebuffer framebuffer = (VkFramebuffer)ud->handle;
    const VkAllocationCallbacks *allocator = ud->allocator;
    VkDevice device = ud->device;
    if(!freeuserdata(L, ud))
        return 0; /* double call */
    TRACE_DELETE(framebuffer, "framebuffer");
    UD(device)->ddt->DestroyFramebuffer(device, framebuffer, allocator);
    return 0;
    }

static int Create(lua_State *L)
    {
    int err;
    ud_t *ud, *device_ud;
    VkResult ec;
    VkFramebuffer framebuffer;
    VkFramebufferCreateInfo* info;
    VkDevice device = checkdevice(L, 1, &device_ud);
    const VkAllocationCallbacks *allocator = optallocator(L, 3);
#define CLEANUP zfreeVkFramebufferCreateInfo(L, info, 1)
    info = zcheckVkFramebufferCreateInfo(L, 2, &err);
    if(err) { CLEANUP; return argerror(L, 2); }
    ec = device_ud->ddt->CreateFramebuffer(device, info, allocator, &framebuffer);
    CLEANUP;
#undef CLEANUP
    CheckError(L, ec);
    TRACE_CREATE(framebuffer, "framebuffer");
    ud = newuserdata_nondispatchable(L, framebuffer, FRAMEBUFFER_MT);
    ud->parent_ud = device_ud;
    ud->device = device;
    ud->instance = device_ud->instance;
    ud->allocator = allocator;
    ud->destructor = freeframebuffer;
    ud->ddt = device_ud->ddt;
    return 1;
    }

RAW_FUNC(framebuffer)
TYPE_FUNC(framebuffer)
INSTANCE_FUNC(framebuffer)
DEVICE_FUNC(framebuffer)
PARENT_FUNC(framebuffer)
DELETE_FUNC(framebuffer)
DESTROY_FUNC(framebuffer)

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
        { "create_framebuffer",  Create },
        { "destroy_framebuffer",  Destroy },
        { NULL, NULL } /* sentinel */
    };


void moonvulkan_open_framebuffer(lua_State *L)
    {
    udata_define(L, FRAMEBUFFER_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

