/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */


#include "internal.h"

#define N 32

static int EnumerateInstanceLayerProperties(lua_State *L)
    {
    int err;
    uint32_t count, remaining, tot, i;
    VkResult ec;
    VkLayerProperties* properties;//[N];
    int byname = optboolean(L, 1, 0);

#define CLEANUP zfreearrayVkLayerProperties(L, properties, N, 1) 
    properties = znewarrayVkLayerProperties (L, N, &err);
    if(err) { CLEANUP; lua_error(L); }
    lua_newtable(L);
    ec = vk.EnumerateInstanceLayerProperties(&remaining, NULL);
    if(ec) { CLEANUP; CheckError(L, ec); }
    if(remaining==0) { CLEANUP; return 1; }
    
    tot = 0;
    do {
        if(remaining > N)
            { count = N; remaining -= N; }
        else
            { count = remaining; remaining = 0; }

        ec = vk.EnumerateInstanceLayerProperties(&count, properties);
        if(ec && ec!=VK_INCOMPLETE) { CLEANUP; CheckError(L, ec); }
        
        if(byname)
            {
            for(i = 0; i < count; i++)
                {
                lua_pushstring(L, properties[i].layerName);
                zpushVkLayerProperties(L, &properties[i]);
                lua_rawset(L, -3);
                }
            }
        else
            {
            for(i = 0; i < count; i++)
                {
                zpushVkLayerProperties(L, &properties[i]);
                lua_rawseti(L, -2, ++tot);
                }
            }
        } while(remaining > 0);
    
    CLEANUP;
#undef CLEANUP
    return 1;
    }

static int EnumerateInstanceExtensionProperties(lua_State *L)
    {
    int err;
    uint32_t count, remaining, tot, i;
    VkResult ec;
    VkExtensionProperties* properties;//[N];
    const char *layer_name = luaL_optstring(L, 1, NULL);
    int byname = optboolean(L, 2, 0);

#define CLEANUP zfreearrayVkExtensionProperties(L, properties, N, 1) 
    properties = znewarrayVkExtensionProperties(L, N, &err);
    if(err) { CLEANUP; lua_error(L); }
    lua_newtable(L);
    ec = vk.EnumerateInstanceExtensionProperties(layer_name, &remaining, NULL);
    if(ec) { CLEANUP; CheckError(L, ec); }
    if(remaining==0) { CLEANUP; return 1; }

    tot = 0;
    do {
        if(remaining > N)
            { count = N; remaining -= N; }
        else
            { count = remaining; remaining = 0; }

        ec = vk.EnumerateInstanceExtensionProperties(layer_name, &count, properties);
        if(ec && ec!=VK_INCOMPLETE) { CLEANUP; CheckError(L, ec); }
        
        if(byname)
            {
            for(i = 0; i < count; i++)
                {
                lua_pushstring(L, properties[i].extensionName);
                zpushVkExtensionProperties(L, &properties[i]);
                lua_rawset(L, -3);
                }
            }
        else
            {
            for(i = 0; i < count; i++)
                {
                zpushVkExtensionProperties(L, &properties[i]);
                lua_rawseti(L, -2, ++tot);
                }
            }

        } while(remaining > 0);
    
    CLEANUP;
#undef CLEANUP
    return 1;
    }


static int EnumerateDeviceLayerProperties(lua_State *L) /* DEPRECATED */
    {
    int err;
    uint32_t count, remaining, tot, i;
    VkResult ec;
    ud_t *ud;
    VkLayerProperties* properties;//[N];
    VkPhysicalDevice physical_device = checkphysical_device(L, 1, &ud);
    int byname = optboolean(L, 2, 0);

#define CLEANUP zfreearrayVkLayerProperties(L, properties, N, 1) 
    properties = znewarrayVkLayerProperties(L, N, &err);
    if(err) { CLEANUP; lua_error(L); }
    lua_newtable(L);
    ec = ud->idt->EnumerateDeviceLayerProperties(physical_device, &remaining, NULL);
    if(ec) { CLEANUP; CheckError(L, ec); }
    if(remaining==0) { CLEANUP; return 1; }

    tot = 0;
    do {
        if(remaining > N)
            { count = N; remaining -= N; }
        else
            { count = remaining; remaining = 0; }

        ec = ud->idt->EnumerateDeviceLayerProperties(physical_device, &count, properties);
        if(ec && ec!=VK_INCOMPLETE) { CLEANUP; CheckError(L, ec); }
    
        if(byname)
            {
            for(i = 0; i < count; i++)
                {
                lua_pushstring(L, properties[i].layerName);
                zpushVkLayerProperties(L, &properties[i]);
                lua_rawset(L, -3);
                }
            }
        else
            {
            for(i = 0; i < count; i++)
                {
                zpushVkLayerProperties(L, &properties[i]);
                lua_rawseti(L, -2, ++tot);
                }
            }
    } while (remaining > 0);

    CLEANUP;
#undef CLEANUP
    return 1;
    }

static int EnumerateDeviceExtensionProperties(lua_State *L)
    {
    int err;
    uint32_t count, remaining, tot, i;
    VkResult ec;
    VkExtensionProperties* properties; //[N];
    ud_t *ud;
    VkPhysicalDevice physical_device = checkphysical_device(L, 1, &ud);
    const char* layername = luaL_optstring(L, 2, NULL);
    int byname = optboolean(L, 3, 0);
    
#define CLEANUP zfreearrayVkExtensionProperties(L, properties, N, 1) 
    properties = znewarrayVkExtensionProperties(L, N, &err);
    if(err) { CLEANUP; lua_error(L); }
    lua_newtable(L);
    ec = ud->idt->EnumerateDeviceExtensionProperties(physical_device, layername, &remaining, NULL);
    if(ec) { CLEANUP; CheckError(L, ec); }
    if(remaining==0) { CLEANUP; return 1; }

    tot = 0;
    do {
        if(remaining > N)
            { count = N; remaining -= N; }
        else
            { count = remaining; remaining = 0; }

        ec = ud->idt->EnumerateDeviceExtensionProperties(physical_device, layername, &count, properties);
        if(ec && ec!=VK_INCOMPLETE) { CLEANUP; CheckError(L, ec); }
    
        if(byname)
            {
            for(i = 0; i < count; i++)
                {
                lua_pushstring(L, properties[i].extensionName);
                zpushVkExtensionProperties(L, &properties[i]);
                lua_rawset(L, -3);
                }
            }
        else
            {
            for(i = 0; i < count; i++)
                {
                zpushVkExtensionProperties(L, &properties[i]);
                lua_rawseti(L, -2, ++tot);
                }
            }

    } while (remaining > 0);

    CLEANUP;
#undef CLEANUP
    return 1;
    }


static const struct luaL_Reg Functions[] = 
    {
        { "enumerate_instance_extension_properties", EnumerateInstanceExtensionProperties },
        { "enumerate_instance_layer_properties", EnumerateInstanceLayerProperties },
        { "enumerate_device_extension_properties", EnumerateDeviceExtensionProperties },
        { "enumerate_device_layer_properties", EnumerateDeviceLayerProperties },
        { NULL, NULL } /* sentinel */
    };


void moonvulkan_open_layers(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

