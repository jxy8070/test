#ifndef DEVICEITEM_RT1052_AMP_MODULE_4_4_H
#define DEVICEITEM_RT1052_AMP_MODULE_4_4_H

#include "deviceItem.h"

class DeviceItem_RT1052_AMP_MODULE_4_4 : public DeviceItem
{
public:
    DeviceItem_RT1052_AMP_MODULE_4_4(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);

    virtual bool canMonitor();
    QWidget * getSettingDialog() override;
};

#endif // DEVICEITEM_RT1052_AMP_MODULE_4_4_H
