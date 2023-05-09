#ifndef DEVICEITEM_STM32_POE_0_1_H
#define DEVICEITEM_STM32_POE_0_1_H
#include "deviceItem.h"


class DeviceItem_STM32_POE_0_1: public DeviceItem
{
public:
    DeviceItem_STM32_POE_0_1(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);

    virtual bool canMonitor();
    QWidget * getSettingDialog() override;
};

#endif // DEVICEITEM_STM32_POE_0_1_H
