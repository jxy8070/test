#include "lua.hpp"

static int Dir_get(lua_State* L) {
    const char *path = luaL_checkstring(L, 1);
    return 1;
}

static int Dir_create(lua_State* L) {
    const char *path = luaL_checkstring(L, 1);
    return 1;
}

static int Dir_remove(lua_State* L) {
    const char *path = luaL_checkstring(L, 1);
    return 1;
}

static int Dir_move(lua_State* L) {
    const char *path = luaL_checkstring(L, 1);
    return 1;
}

static int Dir_delete(lua_State* L) {
    const char *path = luaL_checkstring(L, 1);
    return 1;
}

static const luaL_Reg dir_lib[] = {
    {"get", Dir_get},
    {"create", Dir_create},
    {"remove", Dir_remove},
    {"move", Dir_move},
    {"delete", Dir_delete},
    {NULL, NULL}
};


QTLUA_EXTERNC int luaopen_dir(lua_State* L) {
    luaL_newlib(L, dir_lib);
    return 1;
}
