#include "lua.hpp"
//wj


static const luaL_Reg serialPorts_lib[] = {
    {NULL, NULL}
};

static luaL_Reg serialPorts_m[] =
{
//    {"__newindex", serialPorts_newindex},
//    {"__index", serialPorts_index},
    {NULL, NULL}
};

QTLUA_EXTERNC int luaopen_serialPorts(lua_State* L) {
//    luaL_newmetatable(L, "serialPorts");
//    luaL_setfuncs(L, serialPorts_m, 0);
    luaL_newlib(L, serialPorts_lib);
    return 1;
}
