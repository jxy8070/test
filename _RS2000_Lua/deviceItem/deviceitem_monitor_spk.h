#ifndef DEVICEITEM_Monitor_SPK_H
#define DEVICEITEM_Monitor_SPK_H
#include "deviceItem.h"


class DeviceItem_Monitor_SPK: public DeviceItem
{
public:
    DeviceItem_Monitor_SPK(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);
    virtual void showSettingDialog();
    virtual bool canMonitor();
};

#endif // DEVICEITEM_Monitor_SPK_H
