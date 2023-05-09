#ifndef CONCRETEMONITORITEM_H
#define CONCRETEMONITORITEM_H

//#include "configItem/configitem.h"
#include "monitoritem.h"

class MonitorItem_meter : public MonitorItem
{
public:
    MonitorItem_meter(CfgType cfgtype, QString monitorName, QImage image,
                      QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
    inline MonitorItem_meter *getThis() { return this; }
private:
    CfgType _cfgType;
};


class MonitorItem_clipper : public MonitorItem
{
public:
    MonitorItem_clipper(CfgType cfgtype, QString monitorName, QImage image,
                      QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
    inline MonitorItem_clipper *getThis() { return this; }
private:
    CfgType _cfgType;
};

class MonitorItem_button : public MonitorItem
{
public:
    MonitorItem_button(CfgType cfgtype, QString monitorName, QImage image,
                      QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
    inline MonitorItem_button *getThis() { return this; }
private:
    CfgType _cfgType;
};


class MonitorItem_label : public MonitorItem
{
public:
    MonitorItem_label(CfgType cfgtype, QString monitorName, QImage image,
                      QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
    inline MonitorItem_label *getThis() { return this; }
private:
    CfgType _cfgType;
};

class Monitor_LiveStreaming : public MonitorItem
{
public:
    Monitor_LiveStreaming(CfgType cfgtype, QString monitorName, QImage image,
                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
    inline Monitor_LiveStreaming *getThis() { return this; }
private:
    CfgType _cfgType;
};
class Monitor_Scene : public MonitorItem   //场景
{
public:
    Monitor_Scene(CfgType cfgtype, QString monitorName, QImage image,
                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
    inline Monitor_Scene *getThis() { return this; }
private:
    CfgType _cfgType;
};
//////////////////////设备///////////////////////
class Monitor_NetworkDevice : public MonitorItem
{
public:
    Monitor_NetworkDevice(CfgType cfgtype, QString monitorName, QImage image,
                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
    inline Monitor_NetworkDevice *getThis() { return this; }
private:
    CfgType _cfgType;
};

//class Monitor_NAP_100 : public MonitorItem
//{
//public:
//    Monitor_NAP_100(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_NAP_100 *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};

//class Monitor_NAP_100i : public MonitorItem
//{
//public:
//    Monitor_NAP_100i(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_NAP_100i *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};

//class Monitor_Dante_Sound_Card : public MonitorItem
//{
//public:
//    Monitor_Dante_Sound_Card(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_Dante_Sound_Card *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};

//class Monitor_AES67_Sound_Card : public MonitorItem
//{
//public:
//    Monitor_AES67_Sound_Card(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_AES67_Sound_Card *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};
//class Monitor_SPK_Mono : public MonitorItem
//{
//public:
//    Monitor_SPK_Mono(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_SPK_Mono *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};
//class Monitor_SPK_Stereo : public MonitorItem
//{
//public:
//    Monitor_SPK_Stereo(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_SPK_Stereo *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};
//class Monitor_MIC_Stereo : public MonitorItem
//{
//public:
//    Monitor_MIC_Stereo(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_MIC_Stereo *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};
//class Monitor_MIC_Quad : public MonitorItem
//{
//public:
//    Monitor_MIC_Quad(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_MIC_Quad *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};
//class Monitor_AMP_Quad : public MonitorItem
//{
//public:
//    Monitor_AMP_Quad(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_AMP_Quad *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};
//class Monitor_TR_MIC_Stereo : public MonitorItem
//{
//public:
//    Monitor_TR_MIC_Stereo(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_TR_MIC_Stereo *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};
//class Monitor_RT_Line_Stereo : public MonitorItem
//{
//public:
//    Monitor_RT_Line_Stereo(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_RT_Line_Stereo *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};
//class Monitor_60W_Poe : public MonitorItem
//{
//public:
//    Monitor_60W_Poe(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_60W_Poe *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};
//class Monitor_AMP_Mondule: public MonitorItem
//{
//public:
//    Monitor_AMP_Mondule(CfgType cfgtype, QString monitorName, QImage image,
//                          QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);
//    inline Monitor_AMP_Mondule *getThis() { return this; }
//private:
//    CfgType _cfgType;
//};

#endif // CONCRETEMONITORITEM_H
