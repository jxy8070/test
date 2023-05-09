#include "lua.hpp"
#include "luapinparam.h"

static int control_index(lua_State* L)
{
    if(lua_topointer(L, 1) == NULL) {
        return 0;
    }
    void *p =  luaL_checkudata(L, 1, "LuaPin");
    CLuaPinParam* ctrlPin= (CLuaPinParam*)p;
    QVariant ctrlValue = ctrlPin->getValue();
    const char *indexName = lua_tostring(L, 2);
    if (strcmp(indexName, "Boolean") == 0)
    {
        lua_pushboolean(L, ctrlValue.toBool());
    }
    else if(strcmp(indexName, "Number") == 0){
        lua_pushnumber(L, ctrlValue.toDouble());
    }
    else if(strcmp(indexName, "String") == 0){
        lua_pushstring(L, ctrlValue.toString().toLatin1().data());
    }
    else{
        luaL_error(L, "not supported(%s)!", indexName);
    }
    return 1;
}

static int control_newindex(lua_State* L)
{
    if(lua_topointer(L, 1) == NULL) {
        return 0;
    }
    void *p =  luaL_checkudata(L, 1, "LuaPin");
    CLuaPinParam* ctrlPin= (CLuaPinParam*)p;
    QVariant newPinData;
    const char *indexName = lua_tostring(L, 2);
    if (strcmp(indexName, "Boolean") == 0)
    {
        luaL_checktype(L, 3, LUA_TBOOLEAN);
        bool val = lua_toboolean(L, 3)? true : false;
        newPinData = val;
    }
    else if(strcmp(indexName, "Number") == 0){
        luaL_checktype(L, 3, LUA_TNUMBER);
        double val = lua_tonumber(L, 3);
        newPinData = val;
    }
    else if(strcmp(indexName, "String") == 0){        
        luaL_checktype(L, 3, LUA_TSTRING);
        const char* val = lua_tostring(L, 3);
        newPinData = val;
    }
    else{
        luaL_error(L, "not supported(%s)!", indexName);
    }
    ctrlPin->setValue(newPinData);
    return 0;
}



static const luaL_Reg controlsLib[] = {
    {NULL, NULL}
};

static luaL_Reg control_m[] =
{
    {"__newindex", control_newindex},
    {"__index", control_index},
    {NULL, NULL}
};

QTLUA_EXTERNC int luaopen_controls(lua_State* L) {
    luaL_newmetatable(L, "LuaPin");
    luaL_setfuncs(L, control_m, 0);
    luaL_newlib(L, controlsLib);
    return 1;
}
