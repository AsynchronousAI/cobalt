/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#include "internal.h"
 
static int GetTouchDevices(lua_State *L)
    {
    SDL_TouchID id;
    SDL_TouchDeviceType type;
    int numfingers;
    const char *name;
    int numdevices = SDL_GetNumTouchDevices();
    lua_newtable(L);
    for(int i = 0; i <numdevices; i++)
        {
        lua_newtable(L); // touchdevice
        id = SDL_GetTouchDevice(i);
        lua_pushinteger(L, id); lua_setfield(L, -2, "touch_id");
        name = SDL_GetTouchName(i);
        lua_pushstring(L, name); lua_setfield(L, -2, "name");
        numfingers = SDL_GetNumTouchFingers(id);
        lua_pushinteger(L, numfingers); lua_setfield(L, -2, "num_fingers");
        type = SDL_GetTouchDeviceType(id);
        pushtouchdevicetype(L, type); lua_setfield(L, -2, "type");
        lua_rawseti(L, -2, i+1);
        }
    return 1;
    }

static int GetTouchFinger(lua_State *L)
    {
    SDL_TouchID touch_id = luaL_checkinteger(L, 1);
    int index = checkindex(L, 2); // 1 .. numfingers
    SDL_Finger *finger = SDL_GetTouchFinger(touch_id, index);
    if(!finger) GetError(L);
    lua_newtable(L);
    lua_pushinteger(L, finger->id); lua_setfield(L, -2, "finger_id");
    lua_pushnumber(L, finger->x); lua_setfield(L, -2, "x");
    lua_pushnumber(L, finger->y); lua_setfield(L, -2, "y");
    lua_pushnumber(L, finger->pressure); lua_setfield(L, -2, "pressure");
    return 1;
    }

static int RecordGesture(lua_State *L)
    {
    SDL_TouchID touch_id = luaL_optinteger(L, 1, -1);
    if(SDL_RecordGesture(touch_id) != 1) GetError(L);
    return 0;
    }

static int SaveAllDollarTemplates(lua_State *L)
    {
    int rc;
    const char *file = luaL_checkstring(L, 1);
    SDL_RWops *dst = SDL_RWFromFile(file, "w");
    if(!dst) GetError(L);
    rc = SDL_SaveAllDollarTemplates(dst);
    SDL_RWclose(dst);
    if(rc==0) GetError(L);
    lua_pushinteger(L, rc); // num saved
    return 1;
    }

static int SaveDollarTemplate(lua_State *L)
    {
    int rc;
    SDL_GestureID gesture_id = luaL_checkinteger(L, 1);
    const char *file = luaL_checkstring(L, 2);
    SDL_RWops *dst = SDL_RWFromFile(file, "w");
    if(!dst) GetError(L);
    rc = SDL_SaveDollarTemplate(gesture_id, dst);
    SDL_RWclose(dst);
    if(rc!=1) GetError(L);
    return 0;
    }

static int LoadDollarTemplates(lua_State *L)
    {
    int rc;
    SDL_TouchID touch_id = luaL_checkinteger(L, 1);
    const char *file = luaL_checkstring(L, 2);
    SDL_RWops *src = SDL_RWFromFile(file, "r");
    rc = SDL_LoadDollarTemplates(touch_id, src);
    SDL_RWclose(src);
    if(rc<0) GetError(L);
    lua_pushinteger(L, rc); // num loaded
    return 1;
    }

/* ----------------------------------------------------------------------- */

static const struct luaL_Reg Functions[] = 
    {
        { "get_touch_devices", GetTouchDevices },
        { "get_touch_finger", GetTouchFinger },
        { "record_gesture", RecordGesture },
        { "save_all_dollar_templates", SaveAllDollarTemplates },
        { "save_dollar_template", SaveDollarTemplate },
        { "load_dollar_templates", LoadDollarTemplates },
        { NULL, NULL } /* sentinel */
    };

void moonsdl2_open_touch(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
#define ADD(what)   do { lua_pushinteger(L, SDL_##what); lua_setfield(L, -2, #what); } while(0)
    ADD(MOUSE_TOUCHID);
#undef ADD
    }

#if 0
Rfr:
https://wiki.libsdl.org/SDL2/README/gesture
https://wiki.libsdl.org/SDL2/README/touch
#endif

