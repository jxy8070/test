#ifndef DEVICEITEM_SPK_H
#define DEVICEITEM_SPK_H

#include "deviceItem.h"



class DeviceItem_Spk: public DeviceItem
{
public:
    DeviceItem_Spk(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);
    virtual void showSettingDialog();
    virtual bool canMonitor();
    void initInOutPorts() override;
};
#endif // DEVICEITEM_SPK_H
