#include "lua.hpp"
//wj


static const luaL_Reg tcpSocket_lib[] = {
    {NULL, NULL}
};

static luaL_Reg tcpSocket_m[] =
{
//    {"__newindex", tcpSocket_newindex},
//    {"__index", tcpSocket_index},
    {NULL, NULL}
};

QTLUA_EXTERNC int luaopen_tcpSocket(lua_State* L) {
//    luaL_newmetatable(L, "tcpSocket");
//    luaL_setfuncs(L, tcpSocket_m, 0);
    luaL_newlib(L, tcpSocket_lib);
    return 1;
}
