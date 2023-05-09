#include "deviceitem_mic_quad.h"
#include "deviceSetting/devsetting_mic_quad.h"
#include "devicesetting/Fangtu/devsetting_wmic_and_miccenterdn_fangtu.h"
#include "deviceSetting/Fangtu/devsetting_mic_quad_fangtu.h"

DeviceItem_Mic_Quad::DeviceItem_Mic_Quad(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 80,/*设备长宽*/ parent)
{
//    _textLength = 41;
//    QPointF pos;

//    if (devType == Dev_Type_RT1052_POE_AES67_MIC_4_0)
//    {
//        for(int i = 0; i < 4; ++i)
//        {
//            pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y+14*i);
//            addPort(PortType_AES67, QString(), i, false, pos,  true);
//        }
//    }
//    initDisplayInOutPortCount();
}

bool DeviceItem_Mic_Quad::canMonitor()
{
    return false;
//    if(_danteDev == NULL) return false;
    //    else return true;
}

QWidget *DeviceItem_Mic_Quad::getSettingDialog()
{
    if(_devType == Dev_Type_RT1052_POE_AES67_MIC_4_0) {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
            return new DevSetting_Mic_Quad_FangTu(this);
        else
            return new DevSetting_Mic_Quad(this);
    }
    else if(_devType == Dev_Type_FionTu_WMIC_FangTu)
        return new DevSetting_WMIC_And_MicCenterDN_FangTu(this);
    else if(_devType == Dev_Type_I_Mic_Center_DN_FangTu)
        return new DevSetting_WMIC_And_MicCenterDN_FangTu(this);
}


