#include "deviceitem_stm32_poe_0_1.h"
#include "deviceSetting/devsetting_stm32_poe_0_1.h"

DeviceItem_STM32_POE_0_1::DeviceItem_STM32_POE_0_1(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 39,/*设备长宽*/ parent)
{
//    _textLength = 41;
//    QPointF pos;

//    if (devType == Dev_Type_STM32_POE_AES67_SPK_0_1)
//    {
//        //1路网络输入
//        pos = QPointF(Port_Start_X, Port_Start_Y);
//        addPort(PortType_AES67, QString(), 0, true, pos, false);
//    }

//    initDisplayInOutPortCount();
}

bool DeviceItem_STM32_POE_0_1::canMonitor()
{
    return false;
//    if(_danteDev == NULL) return false;
    //    else return true;
}

QWidget *DeviceItem_STM32_POE_0_1::getSettingDialog()
{
    return new DevSetting_STM32_POE_0_1(this);
}


