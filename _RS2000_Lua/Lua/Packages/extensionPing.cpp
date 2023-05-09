#include "lua.hpp"
#include <string.h>
#include <QtCore>
#include "extensionPing.h"
#include "../LuaIde/luautils.h" //wj


typedef PingObj CurType;
const char* PingObj::MetaTableName = "Ping";

void PingObj::pingHost(bool isSingle)
{
    _isSingle = isSingle;
    _QTimer.setSingleShot(true);
    _QTimer.start(100);
}

bool PingObj::pingHostWin()
{
    QString strArg = QString("ping %1 -n 1 -w %2").arg(_hostName).arg(_timeoutMs) ; //限制生存时间 "-i 2"

    QProcess pingProcess;
    pingProcess.start(strArg,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);
    int exitCode = pingProcess.exitCode();
    QString stdOut = QString::fromLocal8Bit( pingProcess.readAllStandardOutput());
    QStringList resultInfos = stdOut.split("\r\n", QString::SkipEmptyParts);
    if(resultInfos.length() < 2) return false;
    QString pingResult = resultInfos[1];
    bool bResult = (0 == exitCode);
    if (bResult) {
        //解析耗时时间
        int pos1 = pingResult.indexOf(QString::fromLocal8Bit("时间"));
        int pos2 = pingResult.indexOf("ms");
        if(pos1 != -1 && pos2 != -1){
            pingResult = pingResult.mid(pos1 +3, pos2 - pos1 -3);
            qDebug()<<"time:" << pingResult;
        }

        lua_getfield(_L, LUA_REGISTRYINDEX, getEventHandlerKey());
        if(lua_isfunction(_L, -1)){
            lua_newtable(_L);
            lua_pushstring(_L, _hostName);
            lua_setfield(_L, -2, "HostName");
            lua_pushstring(_L, pingResult.toStdString().c_str());
            lua_setfield(_L, -2, "ElapsedTime");
            lua_pcall(_L, 1, 0, 0);
        }
    }
    else {
        lua_getfield(_L, LUA_REGISTRYINDEX, getErrorHandlerKey());
        if(lua_isfunction(_L, -1)){
            lua_newtable(_L);
            lua_pushstring(_L, _hostName);
            lua_setfield(_L, -2, "HostName");
            lua_pushstring(_L, pingResult.toStdString().c_str());
            lua_setfield(_L, -2, "Error");
            lua_pcall(_L, 1, 0, 0);
        }
    }
    return bResult;
}


bool PingObj::pingHostLinux()
{
    QProcess pingProcess;
    pingProcess.start("ping", QStringList() << "-c 1" << "-t 2" << _hostName,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);
    int exitCode = pingProcess.exitCode();
    QString pingResult = QString::fromLocal8Bit( pingProcess.readAllStandardOutput());
    qDebug()<<pingResult ;
    bool bResult = (0 == exitCode);
    if (bResult) {

    }
    else {

    }
    return bResult;
}


PingObj* PingObj::GetMetaTablePtr(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, MetaTableName); //校验错误直接退出
    PingObj *ptr= (PingObj *)p;
    return ptr;
}

void PingObj::onTimeoutTriggered()
{
    if(_isSingle == false && _QTimer.isSingleShot() == true)
    {
        _QTimer.setSingleShot(_isSingle);
        _QTimer.start(_pingIntervalMs);
    }
    bool rs = false;
    #ifdef Q_OS_WIN
        rs = pingHostWin();
    #else
        rs = pingHostLinux();
    #endif
}

PingObj::PingObj(lua_State* L)
{
    _L = L;
    _isSingle = true;
    _timeoutMs = 4500;
    _pingIntervalMs = 5000;
    sprintf(_EventHandlerKey, "Ping_EventHdl_%s", QString::number((quint64)this, 16).toStdString().c_str());
    sprintf(_ErrorHandlerKey, "Ping_ErrorHdl_%s", QString::number((quint64)this, 16).toStdString().c_str());
    connect(&_QTimer, SIGNAL(timeout()), this, SLOT(onTimeoutTriggered()));
}

int PingObj::New(lua_State* L) {
   size_t iSize = sizeof(CurType);
   CurType* p = (CurType*)lua_newuserdata(L, iSize);
   p = new (p) CurType(L);
   if(!lua_isstring(L, 1))
   {
       luaL_error(L, "hostname is nil!");
   }
   p->setHostName(lua_tostring(L, 1));
   luaL_getmetatable(L, MetaTableName);
   lua_setmetatable(L, -2);
   return 1;
}


int PingObj::Start(lua_State* L)
{
    CurType *ptr= GetMetaTablePtr(L);
    int nCount = lua_gettop(L);
    if(nCount != 2 || !lua_isboolean(L, 2)){
        luaL_error(L, "need a number!");
    }
    int isSingle = lua_toboolean(L, 2);
    ptr->pingHost(isSingle);
    return 0;
}

int PingObj::Stop(lua_State* L)
{
    CurType *ptr= GetMetaTablePtr(L);

    ptr->_QTimer.stop();
    return 0;
}

int PingObj::Index(lua_State* L)
{
    CurType *ptr= GetMetaTablePtr(L);
    const char *indexName = lua_tostring(L, 2);
    if (strcmp(indexName, "EventHandler") == 0) {
        const char* key = ptr->getEventHandlerKey();
        lua_getfield(L, LUA_REGISTRYINDEX, key);
    }
    else if (strcmp(indexName, "ErrorHandler") == 0) {
        const char* key = ptr->getErrorHandlerKey();
        lua_getfield(L, LUA_REGISTRYINDEX, key);
    }
    else if (strcmp(indexName, "start") == 0) {
        lua_pushcfunction(L, Start);
    }
    else if (strcmp(indexName, "stop") == 0) {
        lua_pushcfunction(L, Stop);
    }
    else if(strcmp(indexName, "setTimeoutInterval") == 0)
    {
        lua_pushcfunction(L, SetTimeoutInterval);
    }
    else if(strcmp(indexName, "setPingInterval") == 0)
    {
        lua_pushcfunction(L, SetPingInterval);
    }
    else {
        luaL_error(L, "not supported(%s)!", indexName);
    }
    return 1;
}

int PingObj::Newindex(lua_State* L)
{
    CurType *ptr= GetMetaTablePtr(L);
    const char *indexName = lua_tostring(L, 2);
    if (strcmp(indexName, "EventHandler") == 0)
    {
        if(!lua_isfunction(L, 3)){
            luaL_error(L, "need a function\n");
        }
        const char* key = ptr->getEventHandlerKey();
        lua_setfield(L, LUA_REGISTRYINDEX, key);
    }
    else if (strcmp(indexName, "ErrorHandler") == 0)
    {
        if(!lua_isfunction(L, 3)){
            luaL_error(L, "need a function\n");
        }
        const char* key = ptr->getErrorHandlerKey();
        lua_setfield(L, LUA_REGISTRYINDEX, key);
    }
    else {
        luaL_error(L, "not supported(%s)!", indexName);
    }
    return 0;
}

int PingObj::SetTimeoutInterval(lua_State* L)
{
    CurType *ptr= GetMetaTablePtr(L);
    int nCount = lua_gettop(L);
    int isNum;
    lua_Number fSec = lua_tonumberx(L, 2, &isNum);
    if(nCount != 2 || isNum == 0){
        luaL_error(L, "need a number!");
    }
    ptr->setTimeoutInterval(fSec);
    return 0;
}

int PingObj::SetPingInterval(lua_State* L)
{
    CurType *ptr= GetMetaTablePtr(L);
    int nCount = lua_gettop(L);
    int isNum;
    lua_Number fSec = lua_tonumberx(L, 2, &isNum);
    if(nCount != 2 || isNum == 0){
        luaL_error(L, "need a number!");
    }

    ptr->setPingInterval(fSec);
    return 0;
}

int PingObj::Distroy(lua_State* L)
{
    CurType *ptr= GetMetaTablePtr(L);
    ptr->_QTimer.stop();
    return 0;
}

const luaL_Reg PingObj::Type_lib[] = {
    {"New", New},
    {NULL, NULL}
};

const luaL_Reg PingObj::Type_m[] =
{
    {"__newindex", Newindex},
    {"__index", Index},
    {"__gc", Distroy},
    {NULL, NULL}
};

QTLUA_EXTERNC int luaopen_ping(lua_State* L) {
    luaL_newmetatable(L, CurType::MetaTableName);
    luaL_setfuncs(L, CurType::Type_m, 0);
    luaL_newlib(L, CurType::Type_lib);
    return 1;
}

