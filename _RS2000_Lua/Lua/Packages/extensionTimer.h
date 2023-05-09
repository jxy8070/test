#ifndef MyTimer_h
#define MyTimer_h

#include "lua.hpp"
#include <QTimer>
#include <QDateTime>

class MyTimer: public QObject
{
    Q_OBJECT
public:
    MyTimer(lua_State* L);
    const char* getEventHandlerKey()  { return _EventHandlerKey; }

public:
    static int New(lua_State* L);
    static int Now(lua_State* L);
    static int CallAfter(lua_State* L);
    static int Start(lua_State* L);
    static int Index(lua_State* L);
    static int Newindex(lua_State* L);
    static int Distroy(lua_State* L);

    static const char* MetaTableName;
    static const luaL_Reg Type_lib[];
    static const luaL_Reg Type_m[];

protected slots:
    void onTimeoutTriggered();

private:
    QTimer _QTimer;
    char _EventHandlerKey[40];
    lua_State* _L;

};
#endif //MyTimer_h
