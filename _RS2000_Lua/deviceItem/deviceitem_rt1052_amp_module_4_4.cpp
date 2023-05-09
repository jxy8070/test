#include "deviceitem_rt1052_amp_module_4_4.h"
#include "deviceSetting/devsetting_amp_module.h"

DeviceItem_RT1052_AMP_MODULE_4_4::DeviceItem_RT1052_AMP_MODULE_4_4(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 137,/*设备长宽*/ parent)
{

}



bool DeviceItem_RT1052_AMP_MODULE_4_4::canMonitor()
{
    return false;
}

#include "deviceSetting/Fangtu/devsetting_amp_module_fangtu.h"
QWidget *DeviceItem_RT1052_AMP_MODULE_4_4::getSettingDialog()
{
    if(Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu)
        return new DevSetting_Amp_Module_FangTu(this);
    else
        return new DevSetting_Amp_Module(this);
}
