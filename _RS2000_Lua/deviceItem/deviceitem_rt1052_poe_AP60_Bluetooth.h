#ifndef DEVICEITEM_RT1052_POE_AP60_BLUETOOTH_H
#define DEVICEITEM_RT1052_POE_AP60_BLUETOOTH_H
#include "deviceItem.h"


class DeviceItem_RT1052_Poe_AP60_Bluetooth: public DeviceItem
{
public:
    DeviceItem_RT1052_Poe_AP60_Bluetooth(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);

    virtual bool canMonitor();
    QWidget * getSettingDialog() override;
};

#endif // DEVICEITEM_RT1052_POE_AP60_BLUETOOTH_H
