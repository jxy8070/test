#include "deviceitem_amp_0_2_line.h"
#include "deviceSetting/devsetting_amp_0_2_line.h"
#include "deviceSetting/Fangtu/devsetting_amp_0_2_line_fangtu.h"

DeviceItem_Amp_0_2_Line::DeviceItem_Amp_0_2_Line(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 53,/*设备长宽*/ parent)
{
//    _textLength = 41;
//    QPointF pos;
//    if(devType == Dev_Type_RT1052_POE_AES67_LINE_0_2) {
//        for(int i = 0; i < 2; ++i)
//        {
//            pos = QPointF(Port_Start_X, Port_Start_Y+14*i);
//            addPort(PortType_AES67, QString(), i, true, pos,  false);

//            if(Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu) {
//                pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y+14*i);
//                addPort(PortType_MONAURAL, QString(), i, false, pos,  true);
//            }
//        }
//    }else if(Dev_Type_RT1052_POE_AES67_LINE_0_8) {
//        for(int i = 0; i < 8; ++i)
//        {
//            pos = QPointF(Port_Start_X, Port_Start_Y+14*i);
//            addPort(PortType_AES67, QString(), i, true, pos,  false);

//            if(Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu) {
//                pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y+14*i);
//                addPort(PortType_MONAURAL, QString(), i, false, pos,  true);
//            }
//        }
//        setHeight(25+14*8);
//    }

//    initDisplayInOutPortCount();
}

bool DeviceItem_Amp_0_2_Line::canMonitor()
{
    return false;
//    if(_danteDev == NULL) return false;
    //    else return true;
}

QWidget *DeviceItem_Amp_0_2_Line::getSettingDialog()
{
    if(Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu)
        return new DevSetting_Amp_0_2_Line_FangTu(this);
    else
        return new DevSetting_Amp_0_2_Line(this);
}



