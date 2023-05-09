#include "lua.hpp"
#include "configItem/configitem.h"

static int Component_New(lua_State* L) {

    lua_pushnil(L);
    return 1;
}

static int component_index(lua_State* L)
{
    if(lua_topointer(L, 1) == NULL) {
        return 0;
    }
    void *p =  luaL_checkudata(L, 1, "component");
    ConfigItem* cfgItem= (ConfigItem*)p;
    const char *indexName = lua_tostring(L, 2);
//    cfgItem->getValue(indexName);//wj
//    QVariant ctrlValue = ctrlPin->getValue();
//    if (strcmp(indexName, "Boolean") == 0)
//    {
//        lua_pushboolean(L, ctrlValue.toBool());
//    }
//    else if(strcmp(indexName, "Number") == 0){
//        lua_pushnumber(L, ctrlValue.toDouble());
//    }
//    else if(strcmp(indexName, "String") == 0){
//        lua_pushstring(L, ctrlValue.toString().toLatin1().data());
//    }
//    else{
//        luaL_error(L, "not supported(%s)!", indexName);
//    }
    return 1;
}

static int component_newindex(lua_State* L)
{
    if(lua_topointer(L, 1) == NULL) {
        return 0;
    }
    void *p =  luaL_checkudata(L, 1, "component");
    ConfigItem* cfgItem= (ConfigItem*)p;
//    QVariant newPinData;
//    const char *indexName = lua_tostring(L, 2);
//    if (strcmp(indexName, "Boolean") == 0)
//    {
//        luaL_checktype(L, 3, LUA_TBOOLEAN);
//        bool val = lua_toboolean(L, 3)? true : false;
//        newPinData = val;
//    }
//    else if(strcmp(indexName, "Number") == 0){
//        luaL_checktype(L, 3, LUA_TNUMBER);
//        double val = lua_tonumber(L, 3);
//        newPinData = val;
//    }
//    else if(strcmp(indexName, "String") == 0){
//        luaL_checktype(L, 3, LUA_TSTRING);
//        const char* val = lua_tostring(L, 3);
//        newPinData = val;
//    }
//    else{
//        luaL_error(L, "not supported(%s)!", indexName);
//    }
//    ctrlPin->setValue(newPinData);
    return 0;
}



static const luaL_Reg component_lib[] = {
    {"New", Component_New},
    {NULL, NULL}
};

static luaL_Reg component_m[] =
{
    {"__newindex", component_newindex},
    {"__index", component_index},
    {NULL, NULL}
};

QTLUA_EXTERNC int luaopen_component(lua_State* L) {
    luaL_newmetatable(L, "component");
    luaL_setfuncs(L, component_m, 0);
    luaL_newlib(L, component_lib);
    return 1;
}
