#include "extensionTimer.h"

typedef MyTimer CurType;
const char* MyTimer::MetaTableName = "Timer";
void MyTimer::onTimeoutTriggered()
{
    lua_getfield(_L, LUA_REGISTRYINDEX, getEventHandlerKey());
    if(lua_isfunction(_L, -1)){
        lua_pcall(_L, 0, 0, 0);
    }
}

MyTimer::MyTimer(lua_State* L)
{
    _L = L;
    sprintf(_EventHandlerKey, "Timer_EventHdl_%s", QString::number((quint64)this, 16).toStdString().c_str());
    connect(&_QTimer, SIGNAL(timeout()), this, SLOT(onTimeoutTriggered()));
}

int MyTimer::New(lua_State* L) {
   size_t iSize = sizeof(CurType);
   CurType* p = (CurType*)lua_newuserdata(L, iSize);
   p = new (p) CurType(L);
   luaL_getmetatable(L, MetaTableName);
   lua_setmetatable(L, -2);
   return 1;
}

int MyTimer::Now(lua_State* L) {
    QDateTime now = QDateTime::currentDateTime();
    lua_Number ticks = now.toMSecsSinceEpoch() /1000.0;
    lua_pushnumber(L, ticks);
    return 1;
}

int MyTimer::CallAfter(lua_State* L)
{
    int isNum;
    CurType* ptr = new CurType(L);
    lua_Number fSec = lua_tonumberx(L, 2, &isNum);
    lua_pop(L, 1);
    lua_setfield(L, LUA_REGISTRYINDEX, ptr->getEventHandlerKey());
    ptr->_QTimer.setSingleShot(true);
    ptr->_QTimer.start(fSec*1000);
    return 0;
}

int MyTimer::Start(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, MetaTableName); //校验错误直接退出
    int nCount = lua_gettop(L);
    int isNum;
    CurType *ptr= (CurType *)p;
    lua_Number fSec = lua_tonumberx(L, 2, &isNum);
    if(nCount != 2 || isNum == 0){
        luaL_error(L, "need a number!");
    }

    ptr->_QTimer.start(fSec* 1000);
    return 0;
}

int MyTimer::Index(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, MetaTableName);
    CurType *ptr= (CurType *)p;
    const char *indexName = lua_tostring(L, 2);
    if (strcmp(indexName, "EventHandler") == 0) {
        const char* key = ptr->getEventHandlerKey();
        lua_getfield(L, LUA_REGISTRYINDEX, key);
    }
    else if (strcmp(indexName, "Start") == 0) {
        lua_pushcfunction(L, Start);
    }
    else {
        luaL_error(L, "not supported(%s)!", indexName);
    }
    return 1;
}

int MyTimer::Newindex(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, MetaTableName);
    CurType *ptr= (CurType *)p;
    const char *indexName = lua_tostring(L, 2);
    if (strcmp(indexName, "EventHandler") == 0)
    {
        if(!lua_isfunction(L, 3)){
            luaL_error(L, "need a function\n");
        }
        const char* key = ptr->getEventHandlerKey();
        lua_setfield(L, LUA_REGISTRYINDEX, key);
    }
    else {
        luaL_error(L, "not supported(%s)!", indexName);
    }
    return 0;
}

int MyTimer::Distroy(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, MetaTableName);
    CurType *ptr= (CurType *)p;
    ptr->_QTimer.stop();
    return 0;
}

const luaL_Reg MyTimer::Type_lib[] = {
    {"New", New},
    {"Now", Now},
    {"CallAfter", CallAfter},
    {NULL, NULL}
};

const luaL_Reg MyTimer::Type_m[] =
{
    {"__newindex", Newindex},
    {"__index", Index},
    {"__gc", Distroy},
    {NULL, NULL}
};

QTLUA_EXTERNC int luaopen_timer(lua_State* L) {
    luaL_newmetatable(L, CurType::MetaTableName);
    luaL_setfuncs(L, CurType::Type_m, 0);
    luaL_newlib(L, CurType::Type_lib);
    return 1;
}
