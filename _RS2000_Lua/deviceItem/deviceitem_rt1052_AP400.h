#ifndef DEVICEITEM_RT1052_AP400_H
#define DEVICEITEM_RT1052_AP400_H
#include "deviceItem.h"


class DeviceItem_RT1052_AP400: public DeviceItem
{
public:
    DeviceItem_RT1052_AP400(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);

    virtual bool canMonitor();
    QWidget *getSettingDialog() override;
};

#endif // DEVICEITEM_RT1052_AP400_H
