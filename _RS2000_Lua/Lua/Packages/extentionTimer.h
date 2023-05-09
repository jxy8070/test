#ifndef MyTimer_h
#define MyTimer_h

#include "lua.hpp"
#include <QTimer>
#include <QDateTime>

class MyTimer : public QTimer
{
    Q_OBJECT
    static int _TimerIndex;
public:
    MyTimer(lua_State* L);
    const char* getEventHandlerKey() { return _EventHandlerKey; }

protected slots:
    void onTimeoutTriggered();

private:
    char _EventHandlerKey[40];
    lua_State* _L;

};
#endif //MyTimer_h
