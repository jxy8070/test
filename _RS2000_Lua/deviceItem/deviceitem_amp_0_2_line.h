#ifndef DEVICEITEM_AMP_0_2_LINE_H
#define DEVICEITEM_AMP_0_2_LINE_H
#include "deviceItem.h"


class DeviceItem_Amp_0_2_Line: public DeviceItem
{
public:
    DeviceItem_Amp_0_2_Line(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);

    virtual bool canMonitor();
    QWidget * getSettingDialog() override;
};

#endif // DEVICEITEM_AMP_0_2_LINE_H
