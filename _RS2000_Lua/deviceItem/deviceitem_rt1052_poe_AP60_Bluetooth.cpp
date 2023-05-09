#include "deviceitem_rt1052_poe_AP60_Bluetooth.h".
#include "deviceSetting/devsetting_rt1052_poe_AP60_Bluetooth.h"

DeviceItem_RT1052_Poe_AP60_Bluetooth::DeviceItem_RT1052_Poe_AP60_Bluetooth(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 53,/*设备长宽*/ parent)
{
//    _textLength = 41;
//    QPointF pos;

//    pos = QPointF(Port_Start_X, Port_Start_Y+14*0);
//    addPort(PortType_MONAURAL, QString(), 0, true, pos, false);

//    pos = QPointF(Port_Start_X, Port_Start_Y+14*1);
//    addPort(PortType_AES67, QString(), 0, true, pos, false);

//    initDisplayInOutPortCount();
}

bool DeviceItem_RT1052_Poe_AP60_Bluetooth::canMonitor()
{
    return false;
//    if(_danteDev == NULL) return false;
    //    else return true;
}

QWidget *DeviceItem_RT1052_Poe_AP60_Bluetooth::getSettingDialog()
{
    return new DevSetting_RT1052_Poe_AP60_Bluetooth(this);
}


