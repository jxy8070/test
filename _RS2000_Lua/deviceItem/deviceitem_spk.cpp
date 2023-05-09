#include "deviceitem_spk.h"
#include "deviceSetting/devsetting_spk.h"
#include "deviceSetting/devsetting_spk_1.h"
#include "deviceSetting/devsetting_spk_2.h"

DeviceItem_Spk::DeviceItem_Spk(DevType devType, QString devName, QPointF scenePos, QGraphicsItem *parent)
    :DeviceItem(devType,devName, QImage(NULL), scenePos, 110, 67,/*设备长宽*/ parent)
{
//    _textLength = 41;

//    initDisplayInOutPortCount();
}

//bool DeviceItem_Spk::canMonitor()
//{
//    if(_danteDev == NULL) return false;
//    else return true;
//}

void DeviceItem_Spk::initInOutPorts()
{
    QPointF pos;
    if (_devType == Dev_Type_DNTE_SPK)
    {
        //1路单声道接口 + 1路监控接口
        for(int i = 0; i < 1; ++i)
        {
            addInPort(PortType_MONAURAL, QString(), i);
            addOutPort(PortType_Dante, "Monitor", i);
        }

        //2路进网络接口
        for(int i = 0; i < 2; ++i)
        {
            addInPort(PortType_Dante, QString(), i);
        }
    } else if (_devType == Dev_Type_AES67_SPK) {
        //1路单声道接口 + 1路监控接口
        for(int i = 0; i < 1; ++i)
        {
            addInPort(PortType_MONAURAL, QString(), i);
            addOutPort(PortType_AES67, "Monitor", i);
        }

        //2路进网络接口
        for(int i = 0; i < 2; ++i)
        {
            addInPort(PortType_AES67, QString(), i);
        }
    }
}

bool DeviceItem_Spk::canMonitor()
{
    if(_device == NULL) return false;
    else return true;
}

void DeviceItem_Spk::showSettingDialog()
{
    if (getType() == Dev_Type_AES67_SPK)
    {
        DevSetting_Spk frm(this);
        frm.exec();
    }
}

