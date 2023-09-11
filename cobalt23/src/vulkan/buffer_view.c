/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "internal.h"

static int freebuffer_view(lua_State *L, ud_t *ud)
    {
    VkBufferView buffer_view = (VkBufferView)ud->handle;
    const VkAllocationCallbacks *allocator = ud->allocator;
    VkDevice device = ud->device;
    PFN_vkDestroyBufferView DestroyBufferView = ud->ddt->DestroyBufferView;
    if(!freeuserdata(L, ud))
        return 0; /* double call */
    TRACE_DELETE(buffer_view, "buffer_view");
    DestroyBufferView(device, buffer_view, allocator);
    return 0;
    }

static int Create(lua_State *L)
    {
    int err;
    ud_t *ud, *buffer_ud;
    VkResult ec;
    VkBufferView buffer_view;
    VkBufferViewCreateInfo* info;

    VkBuffer buffer = checkbuffer(L, 1, &buffer_ud);
    VkDevice device = buffer_ud->device;
    const VkAllocationCallbacks *allocator = optallocator(L, 3);

#define CLEANUP zfreeVkBufferViewCreateInfo(L, info, 1)
    info = zcheckVkBufferViewCreateInfo(L, 2, &err);
    if(err) { CLEANUP; return argerror(L, 2); }
    info->buffer = buffer;

    ec = buffer_ud->ddt->CreateBufferView(device, info, allocator, &buffer_view);
    CLEANUP;
    CheckError(L, ec);
#undef CLEANUP
    TRACE_CREATE(buffer_view, "buffer_view");
    ud = newuserdata_nondispatchable(L, buffer_view, BUFFER_VIEW_MT);
    ud->parent_ud = buffer_ud;
    ud->device = device;
    ud->instance = UD(device)->instance;
    ud->allocator = allocator;
    ud->destructor = freebuffer_view;
    ud->ddt = buffer_ud->ddt;
    return 1;
    }


RAW_FUNC(buffer_view)
TYPE_FUNC(buffer_view)
INSTANCE_FUNC(buffer_view)
DEVICE_FUNC(buffer_view)
PARENT_FUNC(buffer_view)
DELETE_FUNC(buffer_view)
DESTROY_FUNC(buffer_view)

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
        { "create_buffer_view",  Create },
        { "destroy_buffer_view",  Destroy },
        { NULL, NULL } /* sentinel */
    };


void moonvulkan_open_buffer_view(lua_State *L)
    {
    udata_define(L, BUFFER_VIEW_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

