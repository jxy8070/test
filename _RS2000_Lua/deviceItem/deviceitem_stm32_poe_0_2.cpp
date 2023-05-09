#include "deviceitem_stm32_poe_0_2.h"
#include "deviceSetting/devsetting_stm32_poe_0_2.h"

DeviceItem_Amp::DeviceItem_Amp(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 53,/*设备长宽*/ parent)
{
    ;
}

bool DeviceItem_Amp::canMonitor()
{
    return false;
//    if(_danteDev == NULL) return false;
    //    else return true;
}

QWidget *DeviceItem_Amp::getSettingDialog()
{
    return new DevSetting_Amp(this);
}


