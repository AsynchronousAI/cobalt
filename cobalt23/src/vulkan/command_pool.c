/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "internal.h"

static int freecommand_pool(lua_State *L, ud_t *ud)
    {
    VkCommandPool command_pool = (VkCommandPool)ud->handle;
    VkDevice device = ud->device;
    const VkAllocationCallbacks *allocator = ud->allocator;
    freechildren(L, COMMAND_BUFFER_MT, ud);
    if(!freeuserdata(L, ud))
        return 0; /* double call */
    TRACE_DELETE(command_pool, "command_pool");
    UD(device)->ddt->DestroyCommandPool(device, command_pool, allocator);
    return 0;
    }

static int Create(lua_State *L)
    {
    int err;
    ud_t *ud, *device_ud;
    VkResult ec;
    VkCommandPool command_pool;
    VkCommandPoolCreateInfo* info;
    const VkAllocationCallbacks *allocator = NULL;

    VkDevice device = checkdevice(L, 1, &device_ud);

#define CLEANUP zfreeVkCommandPoolCreateInfo(L, info, 1)
    if(lua_istable(L, 2))
        {
        allocator = optallocator(L, 3);
        info = zcheckVkCommandPoolCreateInfo(L, 2, &err);
        if(err) { CLEANUP; return argerror(L, 2); }
        }
    else
        {
        VkFlags flags = checkflags(L, 2);
        uint32_t queueFamilyIndex = luaL_checkinteger(L, 3);
        info = znewVkCommandPoolCreateInfo(L, &err);
        if(err) { CLEANUP; return lua_error(L); }
        info->flags = flags;
        info->queueFamilyIndex = queueFamilyIndex;
        }

    ec = device_ud->ddt->CreateCommandPool(device, info, allocator, &command_pool);
    CLEANUP;
    CheckError(L, ec);
#undef CLEANUP
    TRACE_CREATE(command_pool, "command_pool");
    ud = newuserdata_nondispatchable(L, command_pool, COMMAND_POOL_MT);
    ud->parent_ud = device_ud;
    ud->device = device;
    ud->instance = device_ud->instance;
    ud->allocator = allocator;
    ud->destructor = freecommand_pool;
    ud->ddt = device_ud->ddt;
    return 1;
    }

static int ResetCommandPool(lua_State *L)
    {
    VkResult ec;
    ud_t *ud;
    VkCommandPool command_pool = checkcommand_pool(L, 1, &ud);
    VkDevice device = ud->device;
    VkCommandPoolResetFlags flags = optflags(L, 2, 0);
    ec = ud->ddt->ResetCommandPool(device, command_pool, flags);
    CheckError(L, ec);
    return 0;
    }

static int TrimCommandPool(lua_State *L)
    {
    ud_t *ud;
    VkCommandPool command_pool = checkcommand_pool(L, 1, &ud);
    VkDevice device = ud->device;
    VkCommandPoolTrimFlags flags = optflags(L, 2, 0);
    CheckDevicePfn(L, ud, TrimCommandPool);
    ud->ddt->TrimCommandPool(device, command_pool, flags);
    return 0;
    }


RAW_FUNC(command_pool)
TYPE_FUNC(command_pool)
INSTANCE_FUNC(command_pool)
DEVICE_FUNC(command_pool)
PARENT_FUNC(command_pool)
DELETE_FUNC(command_pool)
DESTROY_FUNC(command_pool)

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
        { "create_command_pool",  Create },
        { "destroy_command_pool",  Destroy },
        { "reset_command_pool", ResetCommandPool },
        { "trim_command_pool", TrimCommandPool },
        { NULL, NULL } /* sentinel */
    };


void moonvulkan_open_command_pool(lua_State *L)
    {
    udata_define(L, COMMAND_POOL_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

