#ifndef DEVICEITEM_MIC_H
#define DEVICEITEM_MIC_H
#include "deviceItem.h"


class DeviceItem_Mic: public DeviceItem
{
public:
    DeviceItem_Mic(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);

    virtual bool canMonitor();
    QWidget * getSettingDialog() override;
};

#endif // DEVICEITEM_MIC_H
