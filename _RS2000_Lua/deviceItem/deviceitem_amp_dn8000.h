#ifndef DEVICEITEM_AMP_DN8000_H
#define DEVICEITEM_AMP_DN8000_H
#include "deviceItem.h"


class DeviceItem_Amp_DN8000: public DeviceItem
{
public:
    DeviceItem_Amp_DN8000(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);

    virtual bool canMonitor();
    QWidget * getSettingDialog() override;
};

#endif // DEVICEITEM_AMP_DN8000_H
