#include "Timer.h"

int MyTimer::_TimerIndex = 0;
void MyTimer::onTimeoutTriggered()
{
    lua_getfield(_L, LUA_REGISTRYINDEX, getEventHandlerKey());
    if(lua_isfunction(_L, -1)){
        lua_pcall(_L, 0, 0, 0);
    }
}

MyTimer::MyTimer(lua_State* L):QTimer()
{
    _L = L;
    sprintf(_EventHandlerKey, "EventHandler_%d", _TimerIndex++);
    connect(this, SIGNAL(timeout()), this, SLOT(onTimeoutTriggered()));
}

static int Timer_New(lua_State* L) {
   size_t iSize = sizeof(MyTimer);
   MyTimer* p = (MyTimer*)lua_newuserdata(L, iSize);
   p = new (p) MyTimer(L);
   luaL_getmetatable(L, "Timer");
   lua_setmetatable(L, -2);
   return 1;
}

static int Timer_Now(lua_State* L) {
    QDateTime now = QDateTime::currentDateTime();
    lua_Number ticks = now.toMSecsSinceEpoch() /1000.0;
    lua_pushnumber(L, ticks);
    return 1;
}

static int Timer_CallAfter(lua_State* L)
{
    int isNum;
    MyTimer* p = new MyTimer(L);
    lua_Number fSec = lua_tonumberx(L, 2, &isNum);
    lua_pop(L, 1);
    lua_setfield(L, LUA_REGISTRYINDEX, p->getEventHandlerKey());
    p->setSingleShot(true);
    p->start(fSec*1000);
    return 0;
}

static int Timer_Start(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, "Timer"); //校验错误直接退出
    int nCount = lua_gettop(L);
    int isNum;
    MyTimer *pTimer= (MyTimer *)p;
    lua_Number fSec = lua_tonumberx(L, 2, &isNum);
    if(nCount != 2 || isNum == 0){
        luaL_error(L, "need a second num(exp:2.5)\n");
    }

    pTimer->start(fSec* 1000);
    return 0;
}

static int Timer_index(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, "Timer");
    MyTimer *pTimer= (MyTimer *)p;
    const char *indexName = lua_tostring(L, 2);
    if (strcmp(indexName, "EventHandler") == 0) {
        const char* key = pTimer->getEventHandlerKey();
        lua_getfield(L, LUA_REGISTRYINDEX, key);
    }
    else if (strcmp(indexName, "Start") == 0) {
        lua_pushcfunction(L, Timer_Start);
    }
    else {
        luaL_error(L, "not supported(%s)!", indexName);
    }
    return 1;
}

static int Timer_newindex(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, "Timer");
    MyTimer *pTimer= (MyTimer *)p;
    const char *indexName = lua_tostring(L, 2);
    if (strcmp(indexName, "EventHandler") == 0)
    {
        if(!lua_isfunction(L, 3)){
            luaL_error(L, "need a function\n");
        }
        const char* key = pTimer->getEventHandlerKey();
        lua_setfield(L, LUA_REGISTRYINDEX, key);
    }
    else {
        luaL_error(L, "not supported(%s)!", indexName);
    }
    return 0;
}

static int Timer_distroy(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, "Timer");
    MyTimer *pTimer= (MyTimer *)p;
    pTimer->stop();
    return 0;
}

static const luaL_Reg Timer[] = {
    {"New", Timer_New},
    {"Now", Timer_Now},
    {"CallAfter", Timer_CallAfter},
    {NULL, NULL}
};

static luaL_Reg Timer_m[] =
{
    {"__newindex", Timer_newindex},
    {"__index", Timer_index},
    {"__gc", Timer_distroy},
    {NULL, NULL}
};

QTLUA_EXTERNC int luaopen_timer(lua_State* L) {
    luaL_newmetatable(L, "Timer");
    luaL_setfuncs(L, Timer_m, 0);
    luaL_newlib(L, Timer);
    return 1;
}
