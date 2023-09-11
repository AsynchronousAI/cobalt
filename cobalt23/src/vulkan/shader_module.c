/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#include "internal.h"

static int freeshader_module(lua_State *L, ud_t *ud)
    {
    VkShaderModule shader_module = (VkShaderModule)ud->handle;
    const VkAllocationCallbacks *allocator = ud->allocator;
    VkDevice device = ud->device;
    if(!freeuserdata(L, ud))
        return 0; /* double call */
    TRACE_DELETE(shader_module, "shader_module");
    UD(device)->ddt->DestroyShaderModule(device, shader_module, allocator);
    return 0;
    }

static int Create(lua_State *L)
    {
    int err;
    ud_t *ud, *device_ud;
    VkResult ec;
    VkShaderModule shader_module;
    VkShaderModuleCreateInfo* info;
    size_t size;
    const char *code;
    VkDevice device = checkdevice(L, 1, &device_ud);
    const VkAllocationCallbacks *allocator = NULL;
#define CLEANUP zfreeVkShaderModuleCreateInfo(L, info, 1)
    if(lua_istable(L, 2))
        {
        allocator = optallocator(L, 3);
        info = zcheckVkShaderModuleCreateInfo(L, 2, &err);
        if(err) { CLEANUP; return argerror(L, 2); }
        lua_getfield(L, 2, "code");
        code = luaL_optlstring(L, -1, NULL, &size);
        }
    else
        {
        VkFlags flags = checkflags(L, 2);
        info = znewVkShaderModuleCreateInfo(L, &err);
        if(err) { CLEANUP; return lua_error(L); }
        info->flags = flags;
        code = luaL_optlstring(L, 3, NULL, &size);
        }

    if(!code || (size == 0))
        return luaL_error(L, "missing shader code");
    info->codeSize = size;
    info->pCode = (uint32_t*)code;
    ec = device_ud->ddt->CreateShaderModule(device, info, allocator, &shader_module);
    CLEANUP;
    CheckError(L, ec);
#undef CLEANUP
    TRACE_CREATE(shader_module, "shader_module");
    ud = newuserdata_nondispatchable(L, shader_module, SHADER_MODULE_MT);
    ud->parent_ud = device_ud;
    ud->device = device;
    ud->instance = device_ud->instance;
    ud->allocator = allocator;
    ud->destructor = freeshader_module;
    ud->ddt = device_ud->ddt;
    return 1;
    }

RAW_FUNC(shader_module)
TYPE_FUNC(shader_module)
INSTANCE_FUNC(shader_module)
DEVICE_FUNC(shader_module)
PARENT_FUNC(shader_module)
DELETE_FUNC(shader_module)
DESTROY_FUNC(shader_module)

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
        { "create_shader_module",  Create },
        { "destroy_shader_module",  Destroy },
        { NULL, NULL } /* sentinel */
    };

void moonvulkan_open_shader_module(lua_State *L)
    {
    udata_define(L, SHADER_MODULE_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

