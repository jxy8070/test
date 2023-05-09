#include "concretedeviceitem.h"

NormalPC_Device::NormalPC_Device(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    : DeviceItem(devType, devName, QImage("://Image/pc.png"), scenePos, 49, 68/*设备长宽*/)
{
    _textLength = 0;
    //2出单声道接口
    addOutPort(PortType_MONAURAL, QString(), 0);
    addOutPort(PortType_MONAURAL, QString(), 1);
    adjustPortPosition();
}

bool NormalPC_Device::isNetworkDevice()
{
    return false;
}


NormalCD_Device::NormalCD_Device(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage("://Image/cd.png"), scenePos, 49, 68/*设备长宽*/)
{
    _textLength = 0;
    //2出单声道接口
    addOutPort(PortType_MONAURAL, QString(), 0);
    addOutPort(PortType_MONAURAL, QString(), 1);
    adjustPortPosition();
}

bool NormalCD_Device::isNetworkDevice()
{
    return false;
}

NormalMicrophone_Device::NormalMicrophone_Device(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage("://Image/microphone.png"), scenePos, 49, 68/*设备长宽*/)
{
    _textLength = 0;
    addOutPort(PortType_MONAURAL, QString(), 0);
    adjustPortPosition();
}

bool NormalMicrophone_Device::isNetworkDevice()
{
    return false;
}

NormalSpeaker_Device::NormalSpeaker_Device(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage("://Image/speaker.png"), scenePos, 49, 68/*设备长宽*/)
{
    _textLength = 0;
    addInPort(PortType_MONAURAL, QString(), 0);
    adjustPortPosition();
}

bool NormalSpeaker_Device::isNetworkDevice()
{
    return false;
}

NormalAmplifier_Device::NormalAmplifier_Device(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage("://Image/amplifier.png"), scenePos, 49, 68/*设备长宽*/)
{
    _textLength = 0;

    addInPort(PortType_MONAURAL, QString(), 0);
    addOutPort(PortType_MONAURAL, QString(), 0);
    adjustPortPosition();
}

bool NormalAmplifier_Device::isNetworkDevice()
{
    return false;
}

