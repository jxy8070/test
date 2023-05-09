#ifndef EXTENTIONBASEOBJ_H
#define EXTENTIONBASEOBJ_H
#include "lua.hpp"

class LuaExtentionObj
{
public:
    virtual QString TypeName() = 0;

    static void Index(lua_State* L);
    static void NewIndex(lua_State* L);
};

#endif // EXTENTIONBASEOBJ_H
