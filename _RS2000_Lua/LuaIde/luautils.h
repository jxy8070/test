#ifndef LUAUTILS_H
#define LUAUTILS_H

#include "lua.hpp"
class QTreeWidgetItem;

EXTERN_C int test1(lua_State *L); //临时测试函数
EXTERN_C int luaQ_printLog (lua_State *L);
EXTERN_C const char *luaQ_tolstring (lua_State *L, int idx, size_t *len);
EXTERN_C int showStackInfo(lua_State *L);
EXTERN_C void lua_Format_G(lua_State *L, QTreeWidgetItem * rootItem);

#endif // LUAUTILS_H
