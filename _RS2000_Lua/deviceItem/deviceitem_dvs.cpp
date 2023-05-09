#include "deviceitem_dvs.h"


DeviceItem_DVS::DeviceItem_DVS(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 130, 249,/*设备长宽*/  parent)
{
    _textLength = 51;

//    QPointF pos;

//    int interval = 0;

//    if (devType == Dev_Type_VirtualSoundCard_Dante)
//    {
//        //16对单声道接口
//        for(int i = 0; i < 16; ++i)
//        {
//            pos = QPointF(Port_Start_X, Port_Start_Y+14*i+interval);
//            addPort(PortType_Dante, QString(), i, true, pos,  false);
//            pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y+14*i+interval);
//            addPort(PortType_Dante, QString(), i, false, pos,  true);
//            modifyDistanceByGroup8(i, 16, interval);
//        }
//        setHeight(25+14*16+interval);
//    } else if (devType == Dev_Type_VirtualSoundCard_AES67) {
//        //16对单声道接口
//        for(int i = 0; i < 16; ++i)
//        {
//            pos = QPointF(Port_Start_X, Port_Start_Y+14*i+interval);
//            addPort(PortType_AES67, QString(), i, true, pos,  false);
//            pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y+14*i+interval);
//            addPort(PortType_AES67, QString(), i, false, pos,  true);
//            modifyDistanceByGroup8(i, 16, interval);
//        }
//        setHeight(25+14*16+interval);
//    }else if (devType == Dev_Type_Zynq7010_AES3ToAES67_DongFangJiaLian) {
//        setHeight(25+14*8);
//        //16对单声道接口
//        for(int i = 0; i < 8; ++i) {
//            pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y+14*i);
//            addPort(PortType_AES67, QString(), i, false, pos,  true);
//        }
//    }
//	else if (devType == Dev_Type_RT1052_DR3020) {
//        setHeight(25+14*4);
//        //4对单声道接口
//        for(int i = 0; i < 4; ++i)
//        {
//            pos = QPointF(Port_Start_X, Port_Start_Y+14*i);
//            addPort(PortType_AES67, QString(), i, true, pos,  false);
//            pos = QPointF(_rectWidth-Port_End_X,Port_Start_Y+14*i);
//            addPort(PortType_AES67, QString(), i, false, pos,  true);
//        }
//    }
//    else if (devType == Dev_Type_RT1052_Amplifier_DongFangJiaLian) {
//        setHeight(25+14*8);
//        for(int i = 0; i < 8; ++i) {
//            pos = QPointF(Port_Start_X, Port_Start_Y+14*i);
//            addPort(PortType_AES67, QString(), i, true, pos,  false);
//        }
//    }
//    initDisplayInOutPortCount();
}

void DeviceItem_DVS::showSettingDialog()
{
    return ;
}



//DesignItem* DeviceItem_DVS::clone(DesignItem* toItem)
//{
//    qDebug() << __FUNCTION__ << __LINE__;
//    if(toItem == NULL) toItem = new DeviceItem_DVS(_devType, _defaultDeviceItemName, scenePos(), NULL);
//    return toItem;
//}

