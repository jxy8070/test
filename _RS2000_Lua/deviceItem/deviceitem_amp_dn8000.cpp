#include "deviceitem_amp_dn8000.h"
#include "deviceSetting/devsetting_amp_dn8000.h"

DeviceItem_Amp_DN8000::DeviceItem_Amp_DN8000(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 137,/*设备长宽*/ parent)
{

}

bool DeviceItem_Amp_DN8000::canMonitor()
{
    return false;
}

QWidget *DeviceItem_Amp_DN8000::getSettingDialog()
{
    if(_device != NULL){
        return new DevSetting_Amp_DN8000(this);
    }
    else{
        return NULL;
    }
}


