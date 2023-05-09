#include "deviceitem_rt1052_poe_60w_2_4.h"
#include "deviceSetting/devsetting_rt1052_poe_60w_2_4.h"

#include "deviceSetting/Fangtu/devsetting_rt1052_poe_60w_2_4_fangtu.h"

DeviceItem_RT1052_Poe_60W_2_4::DeviceItem_RT1052_Poe_60W_2_4(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType, devName, QImage(NULL), scenePos, 110, 53,/*设备长宽*/ parent)
{
//    _textLength = 41;
//    QPointF pos;

//    pos = QPointF(Port_Start_X, Port_Start_Y+14*0);
//    addPort(PortType_MONAURAL, QString(), 0, true, pos, false);

//    pos = QPointF(_rectWidth-Port_End_X, Port_Start_Y+14*0);
//    addPort(PortType_AES67, QString(), 0, false, pos, true);

//    pos = QPointF(Port_Start_X, Port_Start_Y+14*1);
//    addPort(PortType_AES67, QString(), 0, true, pos, false);
//    ///////////////////////////////////////////
//    for(int i = 0; i < 2; ++i)
//    {
////        pos = QPointF(1, Port_Start_Y+14*i);
////        addPort(PortType_MONAURAL, QString(), i, true, pos,  false);
////        pos = QPointF(_rectWidth-11,Port_Start_Y+14*i);
////        addPort(PortType_MONAURAL, QString(), i, false, pos,  true);
//    }
//    for(int i = 2; i < 4; ++i)
//    {/*
//        pos = QPointF(_rectWidth-11,Port_Start_Y+14*i);
//        addPort(PortType_MONAURAL, QString(), i, false, pos,  true);*/
//    }

//    for(int i = 0; i < 2; ++i)
//    {
////        pos = QPointF(1, Port_Start_Y+56+14*i);
////        addPort(PortType_AES67, QString(), i, true, pos, false);
////        pos = QPointF(_rectWidth-11,Port_Start_Y+56+14*i);
////        addPort(PortType_AES67, QString(), i, false, pos,  true);
//    }
//    for(int i = 2; i < 4; ++i)
//    {/*
//        pos = QPointF(_rectWidth-11,Port_Start_Y+56+14*i);
//        addPort(PortType_AES67, QString(), i, false, pos,  true);*/
//    }

//    initDisplayInOutPortCount();
}

bool DeviceItem_RT1052_Poe_60W_2_4::canMonitor()
{
    return false;
//    if(_danteDev == NULL) return false;
    //    else return true;
}



QWidget *DeviceItem_RT1052_Poe_60W_2_4::getSettingDialog()
{
    if(Config::instance()->curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        return new DevSetting_RT1052_Poe_60W_2_4_FangTu(this);
    }else {
        return new DevSetting_RT1052_Poe_60W_2_4(this);
    }
}
