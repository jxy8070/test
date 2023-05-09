#include "lua.hpp"
//wj


static const luaL_Reg tcpSocketServer_lib[] = {
    {NULL, NULL}
};

static luaL_Reg tcpSocketServer_m[] =
{
//    {"__newindex", tcpSocketServer_newindex},
//    {"__index", tcpSocketServer_index},
    {NULL, NULL}
};

QTLUA_EXTERNC int luaopen_tcpSocketServer(lua_State* L) {
//    luaL_newmetatable(L, "tcpSocketServer");
//    luaL_setfuncs(L, tcpSocketServer_m, 0);
    luaL_newlib(L, tcpSocketServer_lib);
    return 1;
}
