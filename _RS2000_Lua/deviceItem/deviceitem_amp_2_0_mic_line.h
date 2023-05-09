#ifndef DEVICEITEM_AMP_2_0_MIC_LINE_H
#define DEVICEITEM_AMP_2_0_MIC_LINE_H
#include "deviceItem.h"
#include "deviceSetting/Fangtu/devsetting_amp_2_0_mic_line_fangtu.h"

class DeviceItem_Amp_2_0_Mic_Line: public DeviceItem
{
public:
    DeviceItem_Amp_2_0_Mic_Line(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);

    virtual bool canMonitor();
    QWidget * getSettingDialog() override;
};

#endif // DEVICEITEM_AMP_2_0_MIC_LINE_H
