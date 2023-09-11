/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "internal.h"

static int freecommand_buffer(lua_State *L, ud_t *ud)
    {
    VkCommandBuffer command_buffer = (VkCommandBuffer)(uintptr_t)ud->handle;
    VkCommandPool command_pool = (VkCommandPool)ud->parent_ud->handle;
    VkDevice device = ud->device;
    if(!freeuserdata(L, ud))
        return 0; /* double call */
    TRACE_DELETE(command_buffer, "command_buffer");
    UD(device)->ddt->FreeCommandBuffers(device, command_pool, 1, &command_buffer);
    return 0;
    }

static int Create(lua_State *L)
    {
    int err;
    ud_t *ud, *command_pool_ud;
    VkResult ec;
    uint32_t i, count;
    VkCommandBuffer *command_buffer;
    VkCommandBufferAllocateInfo* info;
    VkCommandPool command_pool = checkcommand_pool(L, 1, &command_pool_ud);

#define CLEANUP zfreeVkCommandBufferAllocateInfo(L, info, 1)
    if(lua_istable(L, 2))
        {
        info = zcheckVkCommandBufferAllocateInfo(L, 2, &err);
        if(err) { CLEANUP; return argerror(L, 2); }
        }
    else
        {
        VkCommandBufferLevel level = checkcommandbufferlevel(L, 2);
        uint32_t commandBufferCount = luaL_checkinteger(L, 3);
        info = znewVkCommandBufferAllocateInfo(L, &err);
        if(err) { CLEANUP; return lua_error(L); }
        info->level = level;
        info->commandBufferCount = commandBufferCount;
        }

    info->commandPool = command_pool;
    count = info->commandBufferCount;
    if(count == 0) { CLEANUP; return luaL_error(L, "invalid count"); }

    command_buffer = (VkCommandBuffer*)MallocNoErr(L, sizeof(VkCommandBuffer)*count);
    if(!command_buffer) { CLEANUP; return errmemory(L); }

    ec = command_pool_ud->ddt->AllocateCommandBuffers(command_pool_ud->device, info, command_buffer);
    CLEANUP;
#undef CLEANUP
    if(ec) { Free(L, command_buffer); CheckError(L, ec); return 0; }

    lua_newtable(L);
    for(i=0; i<count; i++)
        {
        TRACE_CREATE(command_buffer[i], "command_buffer");
        ud = newuserdata_dispatchable(L, command_buffer[i], COMMAND_BUFFER_MT);
        ud->parent_ud = command_pool_ud;
        ud->device = command_pool_ud->device;
        ud->instance = command_pool_ud->instance;
        ud->destructor = freecommand_buffer;
        ud->ddt = command_pool_ud->ddt;
        lua_rawseti(L, -2, i+1);
        }
    return 1;
    }

static int FreeCmdBuffers(lua_State *L)
    {
    int err;
    uint32_t count, i;
    ud_t *ud;
    VkDevice device;
    VkCommandPool command_pool;
    VkCommandBuffer *command_buffer = checkcommand_bufferlist(L, 1, &count, &err, NULL);
    if(err) return argerrorc(L, 1, err);
    
    ud = UD(command_buffer[0]);
    /* check that they all are from the same pool */
    for(i = 1; i < count; i++)
        {
        if(UD(command_buffer[i])->parent_ud != ud->parent_ud)
            {
            Free(L, command_buffer);
            return argerrorc(L, 1, ERR_POOL);
            }
        }
    command_pool = (VkCommandPool)ud->parent_ud->handle;
    device = ud->device;
    for(i = 0; i < count; i++)
        {
        freeuserdata(L, UD(command_buffer[i]));
        TRACE_DELETE(command_buffer[i], "command_buffer");
        }
    
    UD(device)->ddt->FreeCommandBuffers(device, command_pool, count, command_buffer);
    Free(L, command_buffer);
    return 0;
    }

static int BeginCommandBuffer(lua_State *L)
    {
    int err;
    VkResult ec;
    ud_t *ud;
    VkCommandBufferBeginInfo* info;
    VkCommandBuffer command_buffer = checkcommand_buffer(L, 1, &ud);

#define CLEANUP zfreeVkCommandBufferBeginInfo(L, info, 1)
    if(lua_istable(L, 2))
        {
        info = zcheckVkCommandBufferBeginInfo(L, 2, &err);
        if(err) { CLEANUP; return argerror(L, 2); }
        }
    else
        {
        info = znewVkCommandBufferBeginInfo(L, &err);
        if(err) { CLEANUP; return lua_error(L); }
        info->flags = optflags(L, 2, 0);
        }

    ec = ud->ddt->BeginCommandBuffer(command_buffer, info);
    CLEANUP;
    CheckError(L, ec);
#undef CLEANUP
    return 0;
    }

static int EndCommandBuffer(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer command_buffer = checkcommand_buffer(L, 1, &ud);
    VkResult ec = ud->ddt->EndCommandBuffer(command_buffer);
    CheckError(L, ec);
    return 0;
    }

static int ResetCommandBuffer(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer command_buffer = checkcommand_buffer(L, 1, &ud);
    VkCommandBufferResetFlags flags = optflags(L, 2, 0);
    VkResult ec = ud->ddt->ResetCommandBuffer(command_buffer, flags);
    CheckError(L, ec);
    return 0;
    }


RAW_FUNC_DISPATCHABLE(command_buffer)
TYPE_FUNC(command_buffer)
INSTANCE_FUNC(command_buffer)
DEVICE_FUNC(command_buffer)
PARENT_FUNC(command_buffer)
DELETE_FUNC(command_buffer)

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
        { "allocate_command_buffers",  Create },
        { "free_command_buffers",  FreeCmdBuffers },
        { "begin_command_buffer", BeginCommandBuffer },
        { "end_command_buffer", EndCommandBuffer },
        { "reset_command_buffer", ResetCommandBuffer },
        { NULL, NULL } /* sentinel */
    };


void moonvulkan_open_command_buffer(lua_State *L)
    {
    udata_define(L, COMMAND_BUFFER_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

