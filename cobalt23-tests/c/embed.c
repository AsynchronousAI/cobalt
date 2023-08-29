#include <cobalt.h>
#include <lauxlib.h>
#include <lualib.h>

int main(){
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dostring(L, "if (true) print('hi')");
    lua_close(L);
}