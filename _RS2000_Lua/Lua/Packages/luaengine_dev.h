#ifndef LUAENGINE_DEV_H
#define LUAENGINE_DEV_H

#include "lua.hpp"
#include "luaExtension.h"
#include "luapinparam_dev.h"


class LuaEngine_Dev
{
public:
    LuaEngine_Dev(QString luaScript, QList<IG_PARACTL_LUAPIN> luaPinList);

    void startLua();
    void stopLua();
    void pushFunction(QString funcName, lua_CFunction func);

    CLuaPinParam_Dev* getLuaPinParam(int pinId);

private:
    void initLuaEngine();

private slots:

private:
    lua_State* L;
    QString _luaScript;
    QList<CLuaPinParam_Dev*> _luaParamList;
    QList<IG_PARACTL_LUAPIN> _luaPinList;
};

#endif // LUAENGINE_DEV_H
