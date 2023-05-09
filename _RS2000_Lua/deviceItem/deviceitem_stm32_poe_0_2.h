#ifndef DEVICEITEM_AMP_H
#define DEVICEITEM_AMP_H
#include "deviceItem.h"


class DeviceItem_Amp: public DeviceItem
{
public:
    DeviceItem_Amp(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);

    virtual bool canMonitor();
    QWidget * getSettingDialog() override;
};

#endif // DEVICEITEM_AMP_H
