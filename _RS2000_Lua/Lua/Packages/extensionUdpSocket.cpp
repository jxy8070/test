#include "lua.hpp"
#include <string.h>
#include "extensionUdpSocket.h"


typedef UdpSocketObj CurType;
const char* UdpSocketObj::MetaTableName = "UdpSocket";

void UdpSocketObj::recvUdpData()
{
    while(_UdpSocket->hasPendingDatagrams())
    {
        QByteArray data;
        QHostAddress fromAddress;
        quint16 fromPort;
        data.resize(_UdpSocket->pendingDatagramSize());
        _UdpSocket->readDatagram(data.data(),data.size(), &fromAddress, &fromPort);

        lua_getfield(_L, LUA_REGISTRYINDEX, getEventHandlerKey());
        if(lua_isfunction(_L, -1)){
            lua_pushnil(_L); //wj udp占位
            lua_newtable(_L);
            lua_pushstring(_L, fromAddress.toString().toStdString().c_str());
            lua_setfield(_L, -2, "Address");
            lua_pushinteger(_L, fromPort);
            lua_setfield(_L, -2, "Port");
            lua_pushlstring(_L, data.data(), data.length());
            lua_setfield(_L, -2, "Data");
            lua_pcall(_L, 2, 0, 0);
        }

        lua_getfield(_L, LUA_REGISTRYINDEX, getDataHandlerKey());
        if(lua_isfunction(_L, -1)){
            lua_pushnil(_L); //wj udp占位
            lua_newtable(_L);
            lua_pushstring(_L, fromAddress.toString().toStdString().c_str());
            lua_setfield(_L, -2, "Address");
            lua_pushinteger(_L, fromPort);
            lua_setfield(_L, -2, "Port");
            lua_pushlstring(_L, data.data(), data.length());
            lua_setfield(_L, -2, "Data");
            lua_pcall(_L, 2, 0, 0);
        }
    }
}

UdpSocketObj::UdpSocketObj(lua_State* L)
{
    _L = L;
    sprintf(_EventHandlerKey, "Ping_EventHdl_%s", QString::number((quint64)this, 16).toStdString().c_str());
    sprintf(_DataHandlerKey, "Ping_DataHdl_%s", QString::number((quint64)this, 16).toStdString().c_str());
}

int UdpSocketObj::New(lua_State* L) {
   size_t iSize = sizeof(CurType);
   CurType* p = (CurType*)lua_newuserdata(L, iSize);
   p = new (p) CurType(L);
   luaL_getmetatable(L, MetaTableName);
   lua_setmetatable(L, -2);
   return 1;
}


int UdpSocketObj::Open(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, MetaTableName); //校验错误直接退出
    CurType *ptr= (CurType *)p;

    int nCount = lua_gettop(L);
    if(lua_isstring(L, 2))
    {
        QString ipStr(lua_tostring(L, 2));
        quint16 port = 0;
        if(lua_isinteger(L, 3)) {
            port= lua_tointeger(L, 3);
        }
        ptr->_UdpSocket = new QUdpSocket();
        ptr->_UdpSocket->bind(QHostAddress(ipStr), port);
        connect(ptr->_UdpSocket, SIGNAL(readyRead()), ptr, SLOT(recvUdpData()));
    }
    else if(lua_isinteger(L, 2)) {
        quint16 port = lua_tointeger(L, 2);
        ptr->_UdpSocket = new QUdpSocket();
        ptr->_UdpSocket->bind(port);
        connect(ptr->_UdpSocket, SIGNAL(readyRead()), ptr, SLOT(recvUdpData()));
    }
    else if(nCount == 1) {
        ptr->_UdpSocket = new QUdpSocket();
        ptr->_UdpSocket->bind();
        connect(ptr->_UdpSocket, SIGNAL(readyRead()), ptr, SLOT(recvUdpData()));
    }
    else {
        return 0;
    }

    return 0;
}

int UdpSocketObj::Close(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, MetaTableName); //校验错误直接退出
    CurType *ptr= (CurType *)p;

    ptr->_UdpSocket->close();
    ptr->_UdpSocket = NULL;
    return 0;
}


int UdpSocketObj::Send(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, MetaTableName); //校验错误直接退出
    CurType *ptr= (CurType *)p;
    int nCount = lua_gettop(L);
    if(nCount != 4 || !lua_isstring(L, 2)
            || !lua_isinteger(L, 3) || !lua_isstring(L, 4)){
        luaL_error(L, "args are error:(string ip, int port, string data)");
    }

    QString sendTo(lua_tostring(L,2));
    quint16 port = lua_tointeger(L, 3);
    size_t len;
    const char* data = lua_tolstring(L, 4, &len);
    ptr->_UdpSocket->writeDatagram(data, len, QHostAddress(sendTo), port);
    return 0;
}



int UdpSocketObj::JoinMulticast(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, MetaTableName); //校验错误直接退出
    CurType *ptr= (CurType *)p;
    int nCount = lua_gettop(L);
    if(nCount == 2 && lua_isstring(L, 2)){
        QString groupIp(lua_tostring(L,2));
        ptr->_UdpSocket->joinMulticastGroup(QHostAddress(groupIp));
    }
    else if(nCount == 3 && lua_isstring(L, 2) && lua_isstring(L, 3))
    {
        QString groupIp(lua_tostring(L,2));
//        QString localIp(lua_tostring(L,3)); //wj 根据IP获取网卡
//        QNetworkInterface localInterface;
//        ptr->_UdpSocket->joinMulticastGroup(QHostAddress(groupIp), localInterface);
        ptr->_UdpSocket->joinMulticastGroup(QHostAddress(groupIp));
    }
    return 0;
}


int UdpSocketObj::Index(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, MetaTableName);
    CurType *ptr= (CurType *)p;
    const char *indexName = lua_tostring(L, 2);
    if (strcmp(indexName, "EventHandler") == 0) {
        const char* key = ptr->getEventHandlerKey();
        lua_getfield(L, LUA_REGISTRYINDEX, key);
    }
    else if (strcmp(indexName, "Data") == 0) {
        const char* key = ptr->getDataHandlerKey();
        lua_getfield(L, LUA_REGISTRYINDEX, key);
    }
    else if (strcmp(indexName, "Open") == 0) {
        lua_pushcfunction(L, Open);
    }
    else if (strcmp(indexName, "Close") == 0) {
        lua_pushcfunction(L, Close);
    }
    else if (strcmp(indexName, "Send") == 0) {
        lua_pushcfunction(L, Send);
    }
    else if (strcmp(indexName, "JoinMulticast") == 0) {
        lua_pushcfunction(L, JoinMulticast);
    }
    else {
        luaL_error(L, "not supported(%s)!", indexName);
    }
    return 1;
}

int UdpSocketObj::Newindex(lua_State* L)
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
    else if (strcmp(indexName, "Data") == 0)
    {
        if(!lua_isfunction(L, 3)){
            luaL_error(L, "need a function\n");
        }
        const char* key = ptr->getDataHandlerKey();
        lua_setfield(L, LUA_REGISTRYINDEX, key);
    }
    else if(strcmp(indexName, "MulticastTtl") == 0)
    {
        int isNum;
        lua_Integer ttl = lua_tointegerx(L, 3, &isNum);
        if (!isNum){
            luaL_error(L, "need a number\n");
        }
        ptr->setMulticastTtl(ttl);
    }
    else {
        luaL_error(L, "not supported(%s)!", indexName);
    }
    return 0;
}

int UdpSocketObj::Distroy(lua_State* L)
{
    void *p =  luaL_checkudata(L, 1, MetaTableName);
    CurType *ptr= (CurType *)p;
    ptr->_UdpSocket->close();
    ptr->_UdpSocket = NULL;
    return 0;
}

const luaL_Reg UdpSocketObj::Type_lib[] = {
    {"New", New},
    {NULL, NULL}
};

const luaL_Reg UdpSocketObj::Type_m[] =
{
    {"__newindex", Newindex},
    {"__index", Index},
    {"__gc", Distroy},
    {NULL, NULL}
};

QTLUA_EXTERNC int luaopen_udpSocket(lua_State* L) {
    luaL_newmetatable(L, CurType::MetaTableName);
    luaL_setfuncs(L, CurType::Type_m, 0);
    luaL_newlib(L, CurType::Type_lib);
    return 1;
}

