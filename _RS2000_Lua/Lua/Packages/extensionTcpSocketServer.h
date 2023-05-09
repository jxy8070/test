#ifndef ExtensionTcpSocketServer_h
#define ExtensionTcpSocketServer_h

#include "lua.hpp"
#include <QDateTime>

class TcpSocketServerObj: public QObject
{
    Q_OBJECT
public:
    TcpSocketServerObj(lua_State* L);
    inline const char* getEventHandlerKey()  { return _EventHandlerKey; }
    inline const char* getDataHandlerKey()  { return _DataHandlerKey; }
    inline void setMulticastTtl(long long ttl)  { _MulticastTtl = ttl; }

public:
    static int New(lua_State* L);
    static int Open(lua_State* L);
    static int Close(lua_State* L);
    static int Send(lua_State* L);
    static int JoinMulticast(lua_State* L);
    static int Index(lua_State* L);
    static int Newindex(lua_State* L);
    static int Distroy(lua_State* L);

    static const char* MetaTableName;
    static const luaL_Reg Type_lib[];
    static const luaL_Reg Type_m[];

protected slots:
//    void onTimeoutTriggered();

private:
    QTimer _QTimer; //wj 待删
    char _EventHandlerKey[40];
    char _DataHandlerKey[40];
    lua_State* _L;
    long long _MulticastTtl;
};
#endif //ExtensionTcpSocketServer_h
