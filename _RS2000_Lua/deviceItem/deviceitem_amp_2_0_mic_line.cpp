#include "deviceitem_amp_2_0_mic_line.h"
#include "deviceSetting/devsetting_amp_2_0_mic_line.h"

DeviceItem_Amp_2_0_Mic_Line::DeviceItem_Amp_2_0_Mic_Line(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 53,/*设备长宽*/ parent)
{
//    _textLength = 41;
    QPointF pos;

//    if (devType == Dev_Type_RT1052_POE_AES67_MIC_2_0 ||
//            devType == Dev_Type_RT1052_POE_AES67_MIC_2_0_Type1 ||
//            devType == Dev_Type_RT1052_POE_AES67_MIC_2_0_Type2 ||
//            devType == Dev_Type_RT1052_POE_AES67_MIC_2_0_Type3)
//    {
//        for(int i = 0; i < 2; ++i)
//        {
//            if(Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu) {
//                pos = QPointF(Port_Start_X, Port_Start_Y+14*i);
//                addPort(PortType_MONAURAL, QString(), i, true, pos,  false);
//            }
//            pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y+14*i);
//            addPort(PortType_AES67, QString(), i, false, pos,  true);
//        }
//    }
//    if(devType == Dev_Type_RT1052_POE_AES67_MIC_8_0) {
//        for(int i = 0; i < 8; ++i)
//        {
//            if(Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu) {
//                pos = QPointF(Port_Start_X, Port_Start_Y+14*i);
//                addPort(PortType_MONAURAL, QString(), i, true, pos,  false);
//            }
//            pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y+14*i);
//            addPort(PortType_AES67, QString(), i, false, pos,  true);
//        }
//        setHeight(25+14*8);
//    }
//    initDisplayInOutPortCount();
}

bool DeviceItem_Amp_2_0_Mic_Line::canMonitor()
{
    return false;
//    if(_danteDev == NULL) return false;
    //    else return true;
}

QWidget *DeviceItem_Amp_2_0_Mic_Line::getSettingDialog()
{
    if(Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu)
        return new DevSetting_Amp_2_0_Mic_Line_FangTu(this);
    else
        return new DevSetting_Amp_2_0_Mic_Line(this);
}



