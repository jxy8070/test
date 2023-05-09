#include "ctrlitemgpi.h"
#include "ctrlsettinggpi.h"

CtrlItemGpi::CtrlItemGpi(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    _rectHeight = 36;
    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "Gpi1", 0x00050002, 0xFFFFFFFF));  //wj1
    pinList.append(new CLuaPinParam_PC(this, name, "Gpi2", 0x00050004, 0xFFFFFFFF));
    pinList.append(new CLuaPinParam_PC(this, name, "Gpi3", 0x00050006, 0xFFFFFFFF));
    pinList.append(new CLuaPinParam_PC(this, name, "Gpi4", 0x00050008, 0xFFFFFFFF));
    pinList.append(new CLuaPinParam_PC(this, name, "Gpi5", 0x0005000A, 0xFFFFFFFF));
    pinList.append(new CLuaPinParam_PC(this, name, "Gpi6", 0x0005000A, 0xFFFFFFFF));
    pinList.append(new CLuaPinParam_PC(this, name, "Gpi7", 0x0005000A, 0xFFFFFFFF));
    pinList.append(new CLuaPinParam_PC(this, name, "Gpi8", 0x0005000A, 0xFFFFFFFF));
    initControlPins(pinList, true);
}


QWidget *CtrlItemGpi::getSettingForm()
{
    if (_setting == NULL) {
        _setting = new CtrlSettingGpi(this);
    }

    return _setting;
}

