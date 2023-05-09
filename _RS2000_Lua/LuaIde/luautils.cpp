#include "luautils.h"
#include <QTreeWidgetItem>
#include "luawindow.h"

EXTERN_C int luaQ_printLog (lua_State *L);
EXTERN_C int showTableInfo(lua_State *L);

EXTERN_C int test1(lua_State *L)
{
    lua_pushstring(L, "hello");
    int n1 = lua_gettop(L);
    lua_getmetatable(L, -1);
    showStackInfo(L);
    showTableInfo(L);
    return 0;
}


//重定向输出到窗口
EXTERN_C int luaQ_printLog (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  int i;
  QString logMsg;
  for (i = 1; i <= n; i++) {  /* for each argument */
    size_t l;
    const char *s = luaL_tolstring(L, i, &l);  /* convert it to string */
    if (i > 1)  /* not the first element? */
    {
      lua_writestring("\t", 1);  /* add a tab before it */
      logMsg.append(QString("\t"));
    }
    lua_writestring(s, l);  /* print it */
    //合并字符串
    logMsg.append(QString(s));
    lua_pop(L, 1);  /* pop result */
  }
  lua_writeline();

  lua_getfield(L, LUA_REGISTRYINDEX, "pLuaWindow");
  LuaWindow* pLuaWnd = (LuaWindow*)lua_touserdata(L, -1);
  if(pLuaWnd != NULL)
  {
      pLuaWnd->addLog(logMsg);
  }
  return 0;
}

//自定义数据的tostring、保持堆栈平衡
EXTERN_C const char *luaQ_tolstring (lua_State *L, int idx, size_t *len) {
  if (luaL_callmeta(L, idx, "__tostring")) {  /* metafield? */
    if (!lua_isstring(L, -1))
      luaL_error(L, "'__tostring' must return a string");
  }
  else {
    switch (lua_type(L, idx)) {
      case LUA_TNUMBER: {
        if (lua_isinteger(L, idx))
          lua_pushfstring(L, "%I:integer", (LUAI_UACINT)lua_tointeger(L, idx));
        else
          lua_pushfstring(L, "%f:number", (LUAI_UACNUMBER)lua_tonumber(L, idx));
        break;
      }
      case LUA_TSTRING:
        lua_pushfstring(L, "%s:string", lua_tostring(L, idx));
        break;
      case LUA_TBOOLEAN:
        lua_pushstring(L, (lua_toboolean(L, idx) ? "true:boolean" : "false:boolean"));
        break;
      case LUA_TNIL:
        lua_pushliteral(L, "nil");
        break;
      default: {
        const void * ptr =  lua_topointer(L, idx);
        int tt = luaL_getmetafield(L, idx, "__name");  /* try name */
        const char *kind = (tt == LUA_TSTRING) ? lua_tostring(L, -1) :
                                                 luaL_typename(L, idx);
        lua_pushfstring(L, "%s:%p", kind, ptr);
        if (tt != LUA_TNIL)
          lua_remove(L, -2);  /* remove '__name' */
        break;
      }
    }
  }
  const char* str = lua_tolstring(L, -1, len);
  lua_pop(L, 1);
  return str;
}

//调试函数，消息框显示当前堆栈
EXTERN_C int showStackInfo(lua_State *L)
{
    int nCount = lua_gettop(L);
    QString logMsg;
    if(nCount > 0)
    {
        logMsg.append("index\t type    \t  value\n");
        for(int i = nCount; i > 0; --i){
            logMsg.append(QString("(%1:%2)\t %3    \t  %4\n").arg(i).arg(i - nCount -1)
             .arg(luaL_typename(L,i)).arg(luaQ_tolstring(L,i, NULL)));
        }
    }
    else {
        logMsg.append("stack is empty!!!\n");
    }
    QMessageBox msgBox;
    msgBox.setText(logMsg);
    msgBox.exec();
    return 0;
}

EXTERN_C int showTableInfo(lua_State *L)
{
    luaL_checktype(L, -1, LUA_TTABLE);
    int nTop = lua_gettop(L);
    lua_pushnil(L);             //nil作为其实Key值

    QString logMsg;
    int idx = 0;
    while ( 0 != lua_next(L, -2 ) ) //key, value，弹出栈顶值nil，获得表的下一个key和value，压入栈
    {
        char buff[256] = {0};
        const char* key = luaL_tolstring(L, -2, NULL);
        lua_pop(L, 1);
        int type = lua_type(L, -1);
        const void *pVal = lua_topointer(L, -1);
        if(type == LUA_TTABLE)
        {
            sprintf(buff, "%s (tab:%p)", key, pVal);
        }
        else if(type == LUA_TFUNCTION)
        {
            sprintf(buff, "f: %s(%p)", key, pVal);
        }
        else
        {
            sprintf(buff, "v: %s(%s)", key, luaQ_tolstring(L, -1, NULL));
        }

        logMsg.append(QString("%1:\t  %2 \n").arg(idx++).arg(buff));
        lua_pop(L, 1); //弹出value,留下key作为下一个next
    }
    lua_settop(L, nTop);

    QMessageBox msgBox;
    msgBox.setText(logMsg);
    msgBox.exec();
    return 0;
}


struct TableDef
{
    bool expand;
    QString tableName;
    QTreeWidgetItem* parentItem;
};

QMap<const void*, TableDef> mapTableDef;
QMap<const void*, TableDef> mapTableDef1;
EXTERN_C void lua_FormatTabelInfo(lua_State *L, TableDef& tabDef)
{
    tabDef.expand = true;
    int nTop = lua_gettop(L);
    QStringList nameList = tabDef.tableName.split('.');
    lua_getglobal(L, nameList[0].toLatin1().data());
    for(int i = nameList.count() -1; i > 0; i--)
    {
        lua_getfield(L, -1, nameList[1].toLatin1().data());
    }
    Q_ASSERT(lua_istable(L, -1));

    lua_pushnil(L);             //nil作为其实Key值
    QList<QTreeWidgetItem *> otherItems;
    QTreeWidgetItem * tabItem = new QTreeWidgetItem();
    while ( 0 != lua_next(L, -2 ) ) //key, value，弹出栈顶值nil，获得表的下一个key和value，压入栈
    {
        char buff[256] = {0};
        const char* key = luaL_tolstring(L, -2, NULL);
        lua_pop(L, 1);
        int type = lua_type(L, -1);
        const void *pVal = lua_topointer(L, -1);
        if(type == LUA_TTABLE)
        {
            sprintf(buff, "%s (tab:%p)", key, pVal);
            QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() <<buff);
            tabItem->addChild(item);
            if(!mapTableDef.contains(pVal))
            {
                TableDef tabDefChiled;
                tabDefChiled.expand = false;
                if(tabDef.tableName == "_G") {
                    tabDefChiled.tableName = QString(key);
                }
                else{
                    tabDefChiled.tableName = tabDef.tableName + "." + key;
                }
                tabDefChiled.parentItem = item;
                mapTableDef1.insert(pVal, tabDefChiled);
            }
        }
        else if(type == LUA_TFUNCTION)
        {
            sprintf(buff, "f: %s(%p)", key, pVal);
            QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() <<buff);
            otherItems.append(item);
        }
        else
        {
            sprintf(buff, "v: %s(%s)", key, luaQ_tolstring(L, -1, NULL));
            QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() <<buff);
            otherItems.append(item);
        }
        lua_pop(L, 1); //弹出value,留下key作为下一个next
    }

    lua_settop(L, nTop);
    if(tabItem->childCount() > 0){
        tabItem->setText(0, QString("_tab(%1)").arg(tabItem->childCount()));
        tabDef.parentItem->addChild(tabItem);
    }
    tabDef.parentItem->addChildren(otherItems);

    return;
}


EXTERN_C void lua_Format_G(lua_State *L, QTreeWidgetItem * rootItem)
{
    mapTableDef.clear();
    TableDef gDev;
    gDev.expand = false;
    gDev.tableName = "_G";
    gDev.parentItem = rootItem;
    lua_getglobal(L, "_G");
    const void *ptr_G = lua_topointer(L, 1);
    lua_pop(L, 1);
    mapTableDef.insert(ptr_G, gDev);

    while(true)
    {
        QMap<const void*, TableDef>::iterator iter = mapTableDef.begin();
        while (iter != mapTableDef.end())
        {
            if( !iter->expand)
            {
                lua_FormatTabelInfo(L, iter.value());
            }
            iter++;
        }
        if(mapTableDef1.size() > 0){
            QMap<const void*, TableDef>::iterator iter1 = mapTableDef1.begin();
            while (iter1 != mapTableDef1.end())
            {
                mapTableDef.insert(iter1.key(), iter1.value());
                iter1++;
            }
            mapTableDef1.clear();
        }
        else{
            break;
        }
    }
}
