#include "ctrlitemgpo.h"
#include "ctrlsettinggpo.h"

CtrlItemGpo::CtrlItemGpo(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    _rectHeight = 36;
    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "Gpo1", 0xFFFFFFFF, 0x00050002)); //wj1
    pinList.append(new CLuaPinParam_PC(this, name, "Gpo2", 0xFFFFFFFF, 0x00050004));
    pinList.append(new CLuaPinParam_PC(this, name, "Gpo3", 0xFFFFFFFF, 0x00050006));
    pinList.append(new CLuaPinParam_PC(this, name, "Gpo4", 0xFFFFFFFF, 0x00050008));
    pinList.append(new CLuaPinParam_PC(this, name, "Gpo5", 0xFFFFFFFF, 0x0005000A));
    pinList.append(new CLuaPinParam_PC(this, name, "Gpo6", 0xFFFFFFFF, 0x0005000A));
    pinList.append(new CLuaPinParam_PC(this, name, "Gpo7", 0xFFFFFFFF, 0x0005000A));
    pinList.append(new CLuaPinParam_PC(this, name, "Gpo8", 0xFFFFFFFF, 0x0005000A));
    initControlPins(pinList, true);
}

QWidget *CtrlItemGpo::getSettingForm()
{
    if (_setting == NULL) {
        _setting = new CtrlSettingGpo(this);
    }

    return _setting;
}


