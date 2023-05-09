#include "deviceitem_amp_quad.h"
#include "deviceSetting/devsetting_amp_quad.h"
#include "deviceSetting/Fangtu/devsetting_amp_quad_fangtu.h"

DeviceItem_Amp_Quad::DeviceItem_Amp_Quad(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 137,/*设备长宽*/ parent)
{

}

bool DeviceItem_Amp_Quad::canMonitor()
{
    return false;
//    if(_danteDev == NULL) return false;
    //    else return true;
}

QWidget *DeviceItem_Amp_Quad::getSettingDialog()
{
    if(Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu)
        return new DevSetting_Amp_Quad_FangTu(this);
    else
        return new DevSetting_Amp_Quad(this);
}


