#include "deviceitem_stm32_poe_1_0.h"
#include "deviceSetting/devsetting_stm32_poe_1_0.h"

DeviceItem_Mic::DeviceItem_Mic(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 39,/*设备长宽*/ parent)
{
//    _textLength = 41;
//    QPointF pos;

//    pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y);
//    addPort(PortType_AES67, QString(), 0, false, pos,  true);

//    initDisplayInOutPortCount();
}

bool DeviceItem_Mic::canMonitor()
{
    if(_device == NULL) return false;
    else return true;
}

QWidget *DeviceItem_Mic::getSettingDialog()
{
    if(Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu)
        return new DevSetting_Mic(this);
    else
        return new DevSetting_Mic(this);
}


