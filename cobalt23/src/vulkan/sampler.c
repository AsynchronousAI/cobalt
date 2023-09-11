/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "internal.h"

static int freesampler(lua_State *L, ud_t *ud)
    {
    VkSampler sampler = (VkSampler)ud->handle;
    const VkAllocationCallbacks *allocator = ud->allocator;
    VkDevice device = ud->device;
    if(!freeuserdata(L, ud))
        return 0; /* double call */
    TRACE_DELETE(sampler, "sampler");
    UD(device)->ddt->DestroySampler(device, sampler, allocator);
    return 0;
    }

static int Create(lua_State *L)
    {
    int err;
    ud_t *ud, *device_ud;
    VkResult ec;
    VkSampler sampler;
    VkSamplerCreateInfo* info;
    VkDevice device = checkdevice(L, 1, &device_ud);
    const VkAllocationCallbacks *allocator = optallocator(L, 3);
#define CLEANUP zfreeVkSamplerCreateInfo(L, info, 1)
    info = zcheckVkSamplerCreateInfo(L, 2, &err);
    if(err) { CLEANUP; return argerror(L, 2); }
    ec = device_ud->ddt->CreateSampler(device, info, allocator, &sampler);
    CLEANUP;
    CheckError(L, ec);
#undef CLEANUP
    TRACE_CREATE(sampler, "sampler");
    ud = newuserdata_nondispatchable(L, sampler, SAMPLER_MT);
    ud->parent_ud = device_ud;
    ud->device = device;
    ud->instance = device_ud->instance;
    ud->allocator = allocator;
    ud->destructor = freesampler;
    ud->ddt = device_ud->ddt;
    return 1;
    }

RAW_FUNC(sampler)
TYPE_FUNC(sampler)
INSTANCE_FUNC(sampler)
DEVICE_FUNC(sampler)
PARENT_FUNC(sampler)
DELETE_FUNC(sampler)
DESTROY_FUNC(sampler)

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
        { "create_sampler",  Create },
        { "destroy_sampler",  Destroy },
        { NULL, NULL } /* sentinel */
    };

void moonvulkan_open_sampler(lua_State *L)
    {
    udata_define(L, SAMPLER_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

