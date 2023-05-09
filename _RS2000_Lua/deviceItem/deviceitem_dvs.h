#ifndef DEVICEITEM_DVS_H
#define DEVICEITEM_DVS_H
#include "deviceItem.h"

class DeviceItem_DVS: public DeviceItem
{
    Q_OBJECT
public:
    DeviceItem_DVS(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);
//    DesignItem* clone(DesignItem* toItem) override;
    void showSettingDialog() override;
};

#endif // DEVICEITEM_DVS_H
