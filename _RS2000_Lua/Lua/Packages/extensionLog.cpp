#include "lua.hpp"
//wj


static const luaL_Reg log_lib[] = {
    {NULL, NULL}
};

static luaL_Reg log_m[] =
{
//    {"__newindex", log_newindex},
//    {"__index", log_index},
    {NULL, NULL}
};

QTLUA_EXTERNC int luaopen_log(lua_State* L) {
//    luaL_newmetatable(L, "log");
//    luaL_setfuncs(L, log_m, 0);
    luaL_newlib(L, log_lib);
    return 1;
}
