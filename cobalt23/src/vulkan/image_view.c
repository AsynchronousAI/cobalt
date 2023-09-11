/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "internal.h"

static int freeimage_view(lua_State *L, ud_t *ud)
    {
    VkImageView image_view = (VkImageView)ud->handle;
    const VkAllocationCallbacks *allocator = ud->allocator;
    VkDevice device = ud->device;
    if(!freeuserdata(L, ud))
        return 0; /* double call */
    TRACE_DELETE(image_view, "image_view");
    UD(device)->ddt->DestroyImageView(device, image_view, allocator);
    return 0;
    }

static int Create(lua_State *L)
    {
    int err;
    ud_t *ud, *image_ud;
    VkResult ec;
    VkImageView image_view;
    VkImageViewCreateInfo* info;
    const VkAllocationCallbacks *allocator = optallocator(L, 3);
    VkImage image = checkimage(L, 1, &image_ud);
    VkDevice device = image_ud->device;

#define CLEANUP zfreeVkImageViewCreateInfo(L, info, 1)
    info = zcheckVkImageViewCreateInfo(L, 2, &err);
    if(err) { CLEANUP; return argerror(L, 2); }
    info->image = image;
    ec = UD(device)->ddt->CreateImageView(device, info, allocator, &image_view);
    CLEANUP;
#undef CLEANUP
    CheckError(L, ec);
    TRACE_CREATE(image_view, "image_view");
    ud = newuserdata_nondispatchable(L, image_view, IMAGE_VIEW_MT);
    ud->parent_ud = image_ud;
    ud->device = device;
    ud->instance = UD(device)->instance;
    ud->allocator = allocator;
    ud->destructor = freeimage_view;
    ud->ddt = UD(device)->ddt;
    return 1;
    }


RAW_FUNC(image_view)
TYPE_FUNC(image_view)
INSTANCE_FUNC(image_view)
DEVICE_FUNC(image_view)
PARENT_FUNC(image_view)
DELETE_FUNC(image_view)
DESTROY_FUNC(image_view)

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
        { "create_image_view",  Create },
        { "destroy_image_view",  Destroy },
        { NULL, NULL } /* sentinel */
    };


void moonvulkan_open_image_view(lua_State *L)
    {
    udata_define(L, IMAGE_VIEW_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

