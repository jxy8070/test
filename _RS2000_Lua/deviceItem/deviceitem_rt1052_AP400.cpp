#include "deviceitem_rt1052_AP400.h"
#include "deviceSetting/devsetting_rt1052_AP400.h"
#include "deviceSetting/Fangtu/devsetting_rt1052_ap400_fangtu.h"

DeviceItem_RT1052_AP400::DeviceItem_RT1052_AP400(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 53,/*设备长宽*/ parent)
{
//    _textLength = 41;
//    QPointF pos;

//    pos = QPointF(Port_Start_X, Port_Start_Y+14*0);
//    addPort(PortType_MONAURAL, QString(), 0, true, pos, false);

//    pos = QPointF(Port_Start_X, Port_Start_Y+14*1);
//    addPort(PortType_AES67, QString(), 0, true, pos, false);

//    pos = QPointF(_rectWidth-Port_End_X, Port_Start_Y+14*0);
//    addPort(PortType_AES67, QString(), 0, false, pos, true);

//    initDisplayInOutPortCount();
}

bool DeviceItem_RT1052_AP400::canMonitor()
{
    return false;
//    if(_danteDev == NULL) return false;
    //    else return true;
}

QWidget *DeviceItem_RT1052_AP400::getSettingDialog()
{
    if(Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        return new DevSetting_RT1052_AP400_FangTu(this);
    }else {
        return new DevSetting_RT1052_AP400(this);
    }
}

