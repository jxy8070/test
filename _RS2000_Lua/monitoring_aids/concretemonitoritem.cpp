#include "concretemonitoritem.h"

MonitorItem_meter::MonitorItem_meter(CfgType cfgtype, QString monitorName, QImage image,
                                     QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
{

}

MonitorItem_clipper::MonitorItem_clipper(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
{

}

MonitorItem_button::MonitorItem_button(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
{

}

MonitorItem_label::MonitorItem_label(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
{

}

Monitor_LiveStreaming::Monitor_LiveStreaming(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
{

}

Monitor_Scene::Monitor_Scene(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
{

}

Monitor_NetworkDevice::Monitor_NetworkDevice(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
{

}
//Monitor_NAP_100::Monitor_NAP_100(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}

//Monitor_NAP_100i::Monitor_NAP_100i(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}

//Monitor_Dante_Sound_Card::Monitor_Dante_Sound_Card(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}

//Monitor_AES67_Sound_Card::Monitor_AES67_Sound_Card(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}

//Monitor_SPK_Mono::Monitor_SPK_Mono(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}

//Monitor_SPK_Stereo::Monitor_SPK_Stereo(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}

//Monitor_MIC_Stereo::Monitor_MIC_Stereo(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}

//Monitor_MIC_Quad::Monitor_MIC_Quad(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}

//Monitor_AMP_Quad::Monitor_AMP_Quad(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}

//Monitor_TR_MIC_Stereo::Monitor_TR_MIC_Stereo(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}

//Monitor_RT_Line_Stereo::Monitor_RT_Line_Stereo(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}

//Monitor_AMP_Mondule::Monitor_AMP_Mondule(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}

//Monitor_60W_Poe::Monitor_60W_Poe(CfgType cfgtype, QString monitorName, QImage image, QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
//    : MonitorItem(cfgtype, monitorName, image, scenePos, rectWidth, rectHeight, parent)
//{

//}


