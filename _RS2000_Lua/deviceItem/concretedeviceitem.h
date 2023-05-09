#ifndef CONCRETEDEVICEITEM_H
#define CONCRETEDEVICEITEM_H

#include "deviceItem.h"

//////////////////////////////////////////////////////////////
/// /普通设备
class NormalPC_Device: public DeviceItem
{
public:
    NormalPC_Device(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);
    virtual bool isNetworkDevice();
};

class NormalCD_Device: public DeviceItem
{
public:
    NormalCD_Device(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);
    virtual bool isNetworkDevice();
};

class NormalMicrophone_Device: public DeviceItem
{
public:
    NormalMicrophone_Device(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);
    virtual bool isNetworkDevice();
};

class NormalSpeaker_Device: public DeviceItem
{
public:
    NormalSpeaker_Device(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);
    virtual bool isNetworkDevice();
};

class NormalAmplifier_Device: public DeviceItem
{
public:
    NormalAmplifier_Device(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent);
    virtual bool isNetworkDevice();
};

#endif // CONCRETEDEVICEITEM_H
