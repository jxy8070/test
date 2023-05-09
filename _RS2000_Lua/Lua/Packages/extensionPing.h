#ifndef ExtensionPing_h
#define ExtensionPing_h

#include "lua.hpp"
#include <QTimer>

class PingObj: public QObject
{
    Q_OBJECT
public:
    PingObj(lua_State* L);
    inline const char* getEventHandlerKey()  { return _EventHandlerKey; }
    inline const char* getErrorHandlerKey()  { return _ErrorHandlerKey; }
    inline void setHostName(const char* hostName) {_hostName = hostName;}
    inline void setTimeoutInterval(double interval)  { _timeoutMs = (int)(interval* 1000); }
    inline void setPingInterval(double interval)  { _pingIntervalMs = (int)(interval* 1000); }

    void pingHost(bool isSingle);
    bool pingHostLinux();
    bool pingHostWin();
public:
    static int New(lua_State* L);
    static int Start(lua_State* L);
    static int Stop(lua_State* L);
    static int Index(lua_State* L);
    static int Newindex(lua_State* L);
    static int Distroy(lua_State* L);
    static int SetTimeoutInterval(lua_State* L);
    static int SetPingInterval(lua_State* L);

    static const char* MetaTableName;
    static const luaL_Reg Type_lib[];
    static const luaL_Reg Type_m[];

private:
    static PingObj* GetMetaTablePtr(lua_State* L);
protected slots:
    void onTimeoutTriggered();

private:
    QTimer _QTimer;
    char _EventHandlerKey[40];
    char _ErrorHandlerKey[40];
    lua_State* _L;
    bool _isSingle;
    int _timeoutMs;
    int _pingIntervalMs;
    const char* _hostName;
};

#endif //ExtensionPing_h
