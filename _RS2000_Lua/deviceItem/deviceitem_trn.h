#ifndef DEVICEITEM_TRN_H
#define DEVICEITEM_TRN_H

#include "deviceItem.h"
class ConfigItem;
class DeviceItem_Trn: public DeviceItem
{
public:
    DeviceItem_Trn(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);
    void showSettingDialog() override;
    bool canCustomAlgorithm() override;
    void initAfterAdd() override;
    void clearBeforeDelete() override;
    void showNetworkStreamModeSelect(void);

};


#endif // DEVICEITEM_TRN_H
