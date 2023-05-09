#ifndef DEVICEITEM_MONITORING_AIDS_H
#define DEVICEITEM_MONITORING_AIDS_H
#include "deviceItem.h"

class DevSetting_Monitoring_Aids;
class DeviceItem_Monitoring_Aids : public DeviceItem
{

public:
    DeviceItem_Monitoring_Aids(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);

    virtual void showSettingDialog();
    virtual bool canMonitor();
private:
    DevSetting_Monitoring_Aids *_setting;
};

#endif // DEVICEITEM_MONITORING_AIDS_H
