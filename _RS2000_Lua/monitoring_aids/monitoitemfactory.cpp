#include "monitoitemfactory.h"
#include "concretemonitoritem.h"

MonitorItem *MonitoItemFactory::CreateInstance(CfgType cfgtype, QString monitorName,
                          /* QImage image,*/ QPointF scenePos, qreal rectWidth, qreal rectHeight/*, QGraphicsItem *parent*/)
{
    MonitorItem *monitorItem = NULL;
    QGraphicsItem *parent = NULL;
    switch (cfgtype) {
    case CFG_TYPE_1:
        monitorItem = new MonitorItem_meter(cfgtype, monitorName, QImage("://Image/meter1.png"), scenePos, 60, 297, parent);
        break;
    case CFG_TYPE_2:
        monitorItem = new MonitorItem_clipper(cfgtype, monitorName, QImage("://Image/clipper1.png"), scenePos, 80, 297, parent);
        break;
    case CFG_TYPE_3:
        monitorItem = new MonitorItem_button(cfgtype, monitorName, QImage(NULL), scenePos, 40, 40, parent);
        break;
    case CFG_TYPE_4:
        monitorItem = new MonitorItem_label(cfgtype, monitorName, QImage("://Image/label.png"), scenePos, rectWidth, rectHeight, parent);
        break;
    case CFG_TYPE_5:
        monitorItem = new Monitor_LiveStreaming(cfgtype, monitorName, QImage("://Image/livestreaming1.png"), scenePos, 646, 364, parent);
        break;
    case CFG_TYPE_SCENE:
        monitorItem = new Monitor_Scene(cfgtype, "Scene", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
        break;
    case CFG_TYPE_NETWORKDEVICE:
        monitorItem = new Monitor_NetworkDevice(cfgtype, "NetWorkDevice", QImage("://Image/dev.jpg"), scenePos, 100, 100, parent);
        break;
        //////////////////////设备//////////////////////////////////////
//    case CFG_TYPE_DEV_NAP_100:
//        monitorItem = new Monitor_NAP_100(cfgtype, "NAP_100", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
//        break;
//    case CFG_TYPE_DEV_NAP_100i:
//        monitorItem = new Monitor_NAP_100i(cfgtype, "NAP_100i", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
//        break;
////    case CFG_TYPE_DEV_Dante_Sound_Card:
////        monitorItem = new Monitor_Dante_Sound_Card(cfgtype, "Dante_Sound_Card", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
////        break;
////    case CFG_TYPE_DEV_AES67_Sound_Card:
////        monitorItem = new Monitor_AES67_Sound_Card(cfgtype, "AES67_Sound_Card", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
////        break;
//    case CFG_TYPE_DEV_SPK_Mono:
//        monitorItem = new Monitor_SPK_Mono(cfgtype, "SPK_Mono", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
//        break;
//    case CFG_TYPE_DEV_SPK_Stereo:
//        monitorItem = new Monitor_SPK_Stereo(cfgtype, "SPK_Stereo", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
//        break;
//    case CFG_TYPE_DEV_MIC_Stereo:
//        monitorItem = new Monitor_MIC_Stereo(cfgtype, "MIC_Stereo", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
//        break;
//    case CFG_TYPE_DEV_MIC_Quad:
//        monitorItem = new Monitor_MIC_Quad(cfgtype, "MIC_Quad", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
//        break;
//    case CFG_TYPE_DEV_AMP_Quad:
//        monitorItem = new Monitor_AMP_Quad(cfgtype, "AMP_Quad", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
//        break;
//    case CFG_TYPE_DEV_RT_MIC_Stereo:
//        monitorItem = new Monitor_TR_MIC_Stereo(cfgtype, "RT_MIC_Stereo", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
//        break;
//    case CFG_TYPE_DEV_RT_Line_Stereo:
//        monitorItem = new Monitor_RT_Line_Stereo(cfgtype, "RT_Line_Stereo", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
//        break;
//    case CFG_TYPE_DEV_60W_Poe:
//        monitorItem = new Monitor_60W_Poe(cfgtype, "60W_Poe", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
//        break;
//    case CFG_TYPE_DEV_AMP_Module:
//        monitorItem = new Monitor_AMP_Mondule(cfgtype, "AMP_Module", QImage("://Image/dev.jpg"), scenePos, rectWidth, rectHeight, parent);
//        break;
    default:
        break;
    }
    return monitorItem;
}

MonitoItemFactory::MonitoItemFactory()
{

}
