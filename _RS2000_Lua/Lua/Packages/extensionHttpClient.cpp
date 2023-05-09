#include "lua.hpp"

static int HttpClient_CreateUrl(lua_State* L) {

    return 1;
}

static int HttpClient_EncodeString(lua_State* L) {

    return 1;
}

static int HttpClient_DecodeString(lua_State* L) {

    return 1;
}

static int HttpClient_EncodeParams(lua_State* L) {

    return 1;
}

static int HttpClient_DecodeParams(lua_State* L) {

    return 1;
}

static int HttpClient_DecodeUrl(lua_State* L) {

    return 1;
}

static int HttpClient_Download(lua_State* L) {

    return 1;
}

static int HttpClient_Upload(lua_State* L) {

    return 1;
}

static int HttpClient_Get(lua_State* L) {

    return 1;
}

static int HttpClient_Put(lua_State* L) {

    return 1;
}

static int HttpClient_Delete(lua_State* L) {

    return 1;
}

static int HttpClient_Patch(lua_State* L) {

    return 1;
}

static int HttpClient_Post(lua_State* L) {

    return 1;
}

static const luaL_Reg httpClient_lib[] = {
    {"CreateUrl", HttpClient_CreateUrl},
    {"EncodeString", HttpClient_EncodeString},
    {"DecodeString", HttpClient_DecodeString},
    {"EncodeParams", HttpClient_EncodeParams},
    {"DecodeParams", HttpClient_DecodeParams},
    {"DecodeUrl", HttpClient_DecodeUrl},

    {"Download", HttpClient_Download},
    {"Upload", HttpClient_Upload},
    {"Get", HttpClient_Get},
    {"Put", HttpClient_Put},
    {"Delete", HttpClient_Delete},
    {"Patch", HttpClient_Patch},
    {"Post", HttpClient_Post},
    {NULL, NULL}
};


QTLUA_EXTERNC int luaopen_httpClient(lua_State* L) {
    luaL_newlib(L, httpClient_lib);
    return 1;
}
