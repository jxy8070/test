#include "deviceitem_monitor_spk.h"
#include "deviceSetting/devsetting_monitor_spk.h"

DeviceItem_Monitor_SPK::DeviceItem_Monitor_SPK(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 137,/*设备长宽*/ parent)
{
//    _textLength = 41;
//    QPointF pos;

//    for(int i = 0; i < 4; ++i)
//    {
//        pos = QPointF(Port_Start_X, Port_Start_Y+14*i);
//        addPort(PortType_MONAURAL, QString(), i, true, pos,  false);
//        pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y+14*i);
//        addPort(PortType_MONAURAL, QString(), i, false, pos,  true);
//    }

//    if (devType == DEV_TYPE_AES67_MONITOR_SPK)
//    {
//        for(int i = 0; i < 4; ++i)
//        {
//            pos = QPointF(Port_Start_X, Port_Start_Y+56+14*i);
//            addPort(PortType_AES67, QString(), i, true, pos, false);
//            pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y+56+14*i);
//            addPort(PortType_AES67, QString(), i, false, pos,  true);
//        }
//    }
//    initDisplayInOutPortCount();
}

bool DeviceItem_Monitor_SPK::canMonitor()
{
    return false;
//    if(_danteDev == NULL) return false;
    //    else return true;
}

//QWidget *DeviceItem_Monitor_SPK::getSettingDialog()
//{
//    return new DevSetting_Monitor_SPK(this);
//}

void DeviceItem_Monitor_SPK::showSettingDialog()
{
    DevSetting_Monitor_SPK frm(this);
    frm.exec();
}
