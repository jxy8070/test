#ifndef LUAEXTENSION_H
#define LUAEXTENSION_H

#include "lua.hpp"

extern "C" int luaopen_controls(lua_State* L);
extern "C" int luaopen_component(lua_State* L);
extern "C" int luaopen_dir(lua_State* L);
extern "C" int luaopen_httpClient(lua_State* L);
extern "C" int luaopen_log(lua_State* L);
extern "C" int luaopen_ping(lua_State* L);
extern "C" int luaopen_serialPorts(lua_State* L);
extern "C" int luaopen_tcpSocket(lua_State* L);
extern "C" int luaopen_tcpSocketServer(lua_State* L);
extern "C" int luaopen_timer(lua_State* L);
extern "C" int luaopen_udpSocket(lua_State* L);

static const luaL_Reg extendlibs[] = {
  {"Component",  luaopen_component},
  {"Controls", luaopen_controls},
  {"dir", luaopen_dir},
//  {"Log",  luaopen_log},
//  {"SerialPorts",  luaopen_serialPorts},
  {"Timer", luaopen_timer},
  {"Ping",  luaopen_ping},
  {"HttpClient",  luaopen_httpClient},
    //  {"TcpSocket",  luaopen_tcpSocket},
    //  {"TcpSocketServer",  luaopen_tcpSocketServer},
  {"UdpSocket",  luaopen_udpSocket},
  {NULL, NULL}
};


#endif // LUAEXTENSION_H
