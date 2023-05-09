#ifndef DEVICEITEM_MIC_QUAD_H
#define DEVICEITEM_MIC_QUAD_H
#include "deviceItem.h"


class DeviceItem_Mic_Quad: public DeviceItem
{
public:
    DeviceItem_Mic_Quad(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);

    virtual bool canMonitor();
    QWidget * getSettingDialog() override;
};

#endif // DEVICEITEM_AMP_QUAD_H
