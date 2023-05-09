#include "Lua/Packages/luaengine_dev.h"



int main1(int argc, char *argv[])
{
    try
    {
        QApplication app(argc, argv);
        QList<IG_PARACTL_LUAPIN> luaPinList;
        luaPinList.append(IG_PARACTL_LUAPIN("Lua_76", 0xFFFFFFFF, 0, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF));
        luaPinList.append(IG_PARACTL_LUAPIN("Lua_76", 0xFFFFFFFF, 1, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF));
        luaPinList.append(IG_PARACTL_LUAPIN("Ply_77", 0, 22, 1, 0, 0x000F0001, 0x000F0002));
        luaPinList.append(IG_PARACTL_LUAPIN("Ply_77", 1, 23, 0, 0, 0x000F0001, 0x000F0002));

        QString script ="print('hello')";
        QFile file("E:/test.lua");
        if (file.open(QFile::ReadOnly)) {
            script = QLatin1String(file.readAll());
        }
        LuaEngine_Dev engine(script, luaPinList);
        engine.startLua();

//        CLuaPinParam_Dev paramDev(new IG_PARACTL_LUAPIN("Sig_36", 0xFFFFFFFF, 2, 0, 0, 0x00070005, 0x00070006));
//        paramDev.setValue(QVariant(true));
        return app.exec();
    }
    catch(std::exception ex)
    {
        QMessageBox::information(NULL, "Error", ex.what());
        return -1;
    }
}
