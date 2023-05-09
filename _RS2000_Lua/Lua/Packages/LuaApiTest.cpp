#include "lua.hpp"


//int  luaL_loadfile(lua_State*L,char* file,int mode)  在Lua中加载文件

static int TestMetaTable(lua_State* L)
{
//    int luaL_getmetatable (lua_State *L, const char *tname);  //将注册表中tname对应的元表压栈。如果没有则将 nil 压栈并返回假。
//    int luaL_newmetatable (lua_State *L, const char *tname);  //若没创建过，创建mTab; mTab.__name = tname;registry.name = mTab;
    int lua_getmetatable (lua_State *L, int idx);

}

static int TestRegistry(lua_State* L)
{
    luaL_dostring(L,"function foo()\n  print(\"hello world\")\n end");
    lua_getglobal(L,"foo");
    printf("stack size:%d,%d\n",lua_gettop(L),lua_type(L,-1));
    // 存放函数到注册表中并返回引用
    int ref =  luaL_ref(L,LUA_REGISTRYINDEX);
    printf("stack size:%d\n",lua_gettop(L));
   // 从注册表中读取该函数并调用
    lua_rawgeti(L,LUA_REGISTRYINDEX,ref);
    printf("stack size:%d,%d\n",lua_gettop(L),lua_type(L,-1));

    //printf("stack size:%d,%d\n",lua_gettop(L),lua_type(L,-1));
    lua_pcall(L,0,0,0);
    printf("stack size:%d\n",lua_gettop(L));
    printf("------------------------华丽的分割线------------\n");
    lua_getglobal(L,"foo");
    printf("stack size:%d\n",lua_gettop(L));
    lua_setfield(L,LUA_REGISTRYINDEX,"sb");
    printf("stack size:%d\n",lua_gettop(L));
    lua_getfield(L,LUA_REGISTRYINDEX,"sb");
    lua_pcall(L,0,0,0);
    printf("------------------------又一次华丽的分割线------------\n");
    printf("stack size:%d,%d\n",lua_gettop(L),lua_type(L,-1));
    luaL_unref(L,LUA_REGISTRYINDEX,ref);
    lua_rawgeti(L,LUA_REGISTRYINDEX,ref);
    printf("stack size:%d\n",lua_gettop(L));
    lua_getfield(L,LUA_REGISTRYINDEX,"sb");
    lua_pcall(L,0,0,0);
    return 0;
}

static int TestTable(lua_State* L)
{
    lua_createtable(L, 1, 1);
    lua_pushstring(L, "key");
    // table一开始是在栈顶，即-1处的，但上面的语句压入了一个值，栈顶变-2了。
    // lua_gettable的作用就是以栈顶的值作为key来访问-2位置上的table。
    lua_gettable(L, -2);
    lua_pushnil(L);  // 第一个 key
    //    while (lua_next(L, t) != 0) {
    //     // 用一下 'key' （在索引 -2 处） 和 'value' （在索引 -1 处）
    //     printf("%s - %s\n",
    //            lua_typename(L, lua_type(L, -2)),
    //            lua_typename(L, lua_type(L, -1)));
    //     // 移除 'value' ；保留 'key' 做下一次迭代
    //     lua_pop(L, 1);
    //   }

    //table <<== ==>>stack的操作
    //void lua_getglobal(lua_State* L,char* name)          将Lua全局环境中名为name的变量压入栈顶
    //void lua_setglobal(lua_State* L,char* name)          把名为name的Lua全局变量的值设置为栈顶值并弹栈

    //void lua_getfield(lua_State* L,int idx,char* key)    将idx位置的table的key对应的值压入栈顶
    //void lua_setfield(lua_State* L,int idx,char* key)    将idx位置的table的key对应的值设置为栈顶元素的值并弹栈

    //void lua_gettable(lua_State* L,int idx)  当前栈顶为key,把idx位置的table的key对应的值压入栈顶
    //void lua_settable(lua_State* L,int idx)  当前栈顶为value,栈顶下为key,把idx位置的table的key的值置为value

    //void lua_rawget(lua_State* L,int idx) 类lua_gettable，效率高
    //void lua_rawset(lua_State* L,int idx) 类lua_settable，效率高
    //void lua_rawgeti(lua_State* L,int idx, lua_Integer n)
    //void lua_rawseti(lua_State* L,int idx, lua_Integer n)
    //void lua_newtable(lua_State* L) 创建一个新表并压入栈顶

    return 0;
}

static int TestStack(lua_State* L)
{
//    int  lua_gettop(lua_State *L)             获取栈中元素个数
//    void lua_settop(lua_State *L, int idx)    重置栈顶指针指向idx的位置
//    void lua_pushvalue(lua_State *L, int idx) 复制idx位置元素并压入到栈顶
//    void lua_remove(lua_State *L, int idx)    移除指定idx位置的元素，其上元素依次下移
//    void lua_insert(lua_State *L, int idx)    移动栈顶元素到idx位置
//    void lua_replace(lua_State *L, int idx)   把栈顶元素复制到idx位置，并弹出栈顶元素
//    lua_pop(lua_State *L, int n)              弹出栈顶的n个元素， -1表示弹出全部元素，宏定义函数，通过lua_settop实现
      lua_pushboolean(L, 1);
      lua_pushnumber(L, 10);
      lua_pushnil(L);
      lua_pushstring(L, "hello");   /* true  10  nil  `hello'  */
      lua_pushvalue(L, -4);         /* true  10  nil  `hello'  true  */
      lua_replace(L, 3);            /* true  10  true  `hello'  */
      lua_settop(L, 6);             /* true  10  true  `hello'  nil  nil  */
      lua_remove(L, -3);            /* true  10  true  nil  nil  */
      lua_settop(L, -5);            /* true  */
      lua_close(L);
    return 0;
}
static int TestPackage(lua_State* L)
{
    lua_getglobal(L, "package");                         // L: package，获得package，在栈定
    lua_getfield(L, -1, "loaded");                       // L: package loaded,获得表，在栈顶
    lua_pushnil(L);                                     // L: package loaded nil
    while ( 0 != lua_next(L, -2 ) ) // L: package loaded, key, value，上一个栈顶为nil，弹出nil，获得表的第一个key和value，压入栈
    {
        //CCLOG("%s - %s \n", tolua_tostring(stack, -2, ""), lua_typename(stack, lua_type(stack, -1)));
        std::string key = lua_tostring(L, -2);  //这时候栈顶的下一个,是key
        std::string tableKey =key;                      //下面是对key的一段处理
        int found = tableKey.rfind(".lua");
        if (found!=std::string::npos)
            tableKey = tableKey.substr(0,found);
//        tableKey=replaceAll(tableKey,".","/");
//        tableKey=replaceAll(tableKey,"\\","/");
        tableKey.append(".lua");
        std::string fileName; //fileName可从外部传入
        found = fileName.rfind(tableKey);
        if (0 == found || ( found!=std::string::npos && fileName.at(found-1) == '/'))
        {
            lua_pushstring(L, key.c_str());  //package loaded, key, value，newkey, 将key,压入栈顶
            lua_pushnil(L);                 // pakage,loaded(table)(-5),key(-4),value(-3),key(-2),nil(-1)
            if (lua_istable(L, -5)) //判读栈顶往下第五个是不是table
            {
                //结果将key对应的值置为nil
                lua_settable(L, -5);//pakage,loaded(table),key,value,  将栈顶两个元素作为key和value设置给table，弹出栈顶两个元素
            }
        }
        lua_pop(L, 1); //pakage,loaded(table),key  弹出value,留下key作为下一个next
    }
    lua_pop(L, 2); //栈平衡/
    return 0;
}
