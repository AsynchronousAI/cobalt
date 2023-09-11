/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#include "internal.h"
    
static int Type(lua_State *L)
    {
#define TRY(xxx) do { if(test##xxx(L, 1, NULL) != 0) { lua_pushstring(L, ""#xxx); return 1; } } while(0)
    TRY(instance);
    TRY(physical_device);
    TRY(device);
    TRY(queue);
    TRY(command_pool);
    TRY(command_buffer);
    TRY(semaphore);
    TRY(fence);
    TRY(buffer);
    TRY(device_memory);
    TRY(image);
    TRY(event);
    TRY(buffer_view);
    TRY(image_view);
    TRY(shader_module);
    TRY(sampler);
    TRY(render_pass);
    TRY(framebuffer);
    TRY(descriptor_set_layout);
    TRY(descriptor_pool);
    TRY(pipeline_layout);
    TRY(pipeline_cache);
    TRY(query_pool);
    TRY(descriptor_set);
    TRY(pipeline);
    TRY(debug_report_callback);
    TRY(display);
    TRY(display_mode);
    TRY(descriptor_update_template);
    TRY(validation_cache);
    TRY(sampler_ycbcr_conversion);
    TRY(debug_utils_messenger);
    return 0;
#undef TRY
    }

int trace_objects = 0;

static int TraceObjects(lua_State *L)
    {
    trace_objects = checkboolean(L, 1);
    return 0;
    }

static int Now(lua_State *L)
    {
    lua_pushnumber(L, now());
    return 1;
    }

static int Since(lua_State *L)
    {
    double t = luaL_checknumber(L, 1);
    lua_pushnumber(L, since(t));
    return 1;
    }

/* ----------------------------------------------------------------------- */

static const struct luaL_Reg Functions[] = 
    {
        { "type", Type },
        { "trace_objects", TraceObjects },
        { "now", Now },
        { "since", Since },
        { NULL, NULL } /* sentinel */
    };

void moonvulkan_open_tracing(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }


