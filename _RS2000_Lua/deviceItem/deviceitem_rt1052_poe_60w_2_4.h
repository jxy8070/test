#ifndef DEVICEITEM_RT1052_POE_60W_2_4_H
#define DEVICEITEM_RT1052_POE_60W_2_4_H
#include "deviceItem.h"


class DeviceItem_RT1052_Poe_60W_2_4: public DeviceItem
{
public:
    DeviceItem_RT1052_Poe_60W_2_4(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);

    virtual bool canMonitor();
    QWidget * getSettingDialog() override;
};

#endif // DEVICEITEM_RT1052_POE_60W_2_4_H
