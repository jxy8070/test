#include "deviceitem_monitoring_aids.h"
//#include "deviceSetting/devsetting_monitoring_aids.h"

DeviceItem_Monitoring_Aids::DeviceItem_Monitoring_Aids(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent):
    DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 80,/*设备长宽*/ parent)
{
    _setting = NULL;
}


void DeviceItem_Monitoring_Aids::showSettingDialog()
{
//    if (_setting == NULL)
//    {
//        _setting = new DevSetting_Monitoring_Aids(this);
//    }
//    _setting->exec();
//    frm.exec();
}

bool DeviceItem_Monitoring_Aids::canMonitor()
{
    return false;
}


