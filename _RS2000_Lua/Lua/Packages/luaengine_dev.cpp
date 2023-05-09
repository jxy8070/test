#include "luaengine_dev.h"
#include "qlogging.h"
//重定向输出




void writeLuaLog(const char *msg)
{
    static QMutex mutex;
    QString logFile = "lua_testlogs_" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".txt";
    QFile logFullFile("E:/" + logFile);
    mutex.lock();
    QString logTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString logMsg = QString("[%1] %2").arg(logTime).arg(msg);

    if (logFullFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream ts(&logFullFile);
        ts << logMsg << endl;
        logFullFile.flush();
        logFullFile.close();
    }
    mutex.unlock();
}

EXTERN_C int lua_print_dev (lua_State *L) {
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

    //把logMsg保存到日志文件
    writeLuaLog(logMsg.toUtf8().data());
    return 0;
}

LuaEngine_Dev::LuaEngine_Dev(QString luaScript, QList<IG_PARACTL_LUAPIN> luaPinList)
{
    _luaScript = luaScript;
    _luaPinList = luaPinList;
}

void LuaEngine_Dev::pushFunction(QString funcName, lua_CFunction func)
{
    if (funcName.isEmpty()) return;
    if (func == NULL)       return;

    lua_pushcfunction(L, func);
    lua_setglobal(L, funcName.toLocal8Bit().data());
}



void LuaEngine_Dev::initLuaEngine()
{
    //初始化lua引擎
    L = luaL_newstate();
    if (L == NULL)  return;

    luaL_openlibs(L);
    //加载extendlibs中定义的扩展库
    const luaL_Reg *lib;
    for (lib = extendlibs; lib->func; lib++) {
      luaL_requiref(L, lib->name, lib->func, 1);
      lua_pop(L, 1);  /* remove lib */
    }
    pushFunction("print", lua_print_dev);

    if(_luaPinList.length() > 0)
    {
        lua_getglobal(L, "Controls"); //重新放置Controls表到栈顶
        lua_newtable(L);
        for(int i = 0; i<8; i++) //初始化Outputs表元素 0~7代表8路输出端口
        {
            CLuaPinParam_Dev* pinParam = getLuaPinParam(i);
            lua_pushlightuserdata(L, (void*)pinParam);
            luaL_setmetatable(L, "LuaPin");
            lua_rawseti(L, -2, i+1);
            if(pinParam != NULL) _luaParamList.append(pinParam);
        }
        lua_setfield(L, -2, "Outputs"); //添加Outputs表

        lua_newtable(L);
        for(int i = 8; i< 16; i++) //初始化Inputs表元素 8~15代表8个输入端口
        {
            CLuaPinParam_Dev* pinParam = getLuaPinParam(i);
            lua_pushlightuserdata(L, (void*)pinParam);
            luaL_setmetatable(L, "LuaPin");
            lua_rawseti(L, -2, i+1);            
            if(pinParam != NULL) _luaParamList.append(pinParam);
        }
        lua_setfield(L, -2, "Inputs"); //添加Inputs表
        lua_pop(L, 1);
    }
}



CLuaPinParam_Dev* LuaEngine_Dev::getLuaPinParam(int pinId)
{
    QList<CLuaPinParam_Dev*> children;
    for(IG_PARACTL_LUAPIN pinData : _luaPinList)
    {
        if(pinData.parentId == pinId)
        {
            CLuaPinParam_Dev* childItem = getLuaPinParam(pinData.pinId);
            children.append(childItem);
        }
    }

    for(IG_PARACTL_LUAPIN pinData : _luaPinList)
    {
        if(pinData.pinId == pinId)
        {
            CLuaPinParam_Dev* item = new CLuaPinParam_Dev(pinData);
            item->setChildren(children);
            return item;
        }
    }

    return nullptr;
}

#include "LuaIde/luautils.h"

//执行脚本
void LuaEngine_Dev::startLua()
{
    writeLuaLog("start lua script");
    initLuaEngine();
    QByteArray baScript = _luaScript.toLatin1();
//    luaL_dostring(L, baScript.data());

    int error = luaL_loadstring(L, baScript.data());
    if (! error)
      error = lua_pcall(L, 0, LUA_MULTRET, -2);
    if (error){
        QString errorOld = QString::fromLatin1(lua_tostring(L, -1));
        QStringList errors = errorOld.split(':');
        QString error = QString("lua error(line %1) %2").arg(errors[1]).arg(errors[2]);
        writeLuaLog(error.toLatin1().data());
//        showStackInfo(L);
    }

    return;
}

//停止脚本
void LuaEngine_Dev::stopLua()
{
    for(CLuaPinParam_Dev* param : _luaParamList)
    {
        delete param;
    }
    _luaParamList.clear();
    lua_close(L);
    writeLuaLog("stop lua script");
}
