#include "stable.h"
#include "device_stm32_poe_1_0.h"
#include "protocal/packetbase.h"
#include "protocal/packet.h"
#include "device/devicemanager.h"
#include "deviceSetting/alarmconfig.h"
#include "Frame/mainwindow.h"


Device_Mic::Device_Mic(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _configData.initConfigData();
    _alarmCfg = new AlarmConfig(devName, mac);
    _factoryMode = true;
    _saveState = DevSave_None;
}


void Device_Mic::processPacket(PacketBase* pack)
{
#ifdef QT_DEBUG
//    qDebug()<<"Device_Amp receive Pack:" <<pack->getMsgType();
#endif
    switch(pack->getMsgType())
    {
    case MSG_TYPE_DEV_ISALIVING: //设备在线
        break;
    case Msg_Type_HeartbeatAck:
        if(_saveState == DevSave_Save2Dev) {
            MainWindow::instance()->showNotice(tr("Save Success!"));
        }
        else if(_saveState == DevSave_LocalLoad) {
            MainWindow::instance()->showNotice(tr("Load Success!"));
        }
        _saveState = DevSave_None;
        break;
    case Msg_Type_GetInitAck:
        _configData = pack->getBody_STM32_POE().InitData_1_0.getDataPack();

//        if(isAddedToProject()) {
//            emit initConfigFromDevice();
//            emit volumeChanged();
//            emit muteChanged();
//            emit phaseChanged();
//            emit routerChanged();
//        }

        switchStatus(Dev_Online);
        break;
    case Msg_Type_STM32_POE_Monitor_Msg:
//        qDebug() <<"/////////////////////////"<< QTime::currentTime();
        emit displaySig(pack->getBody_STM32_POE());
        break;
    case Msg_Type_STM32_POE_AD_Gain_Ack:
        break;
    default:
        qDebug() << __FUNCTION__ << __LINE__
                 << QString("Unknown packet type%1").arg(pack->getMsgType());
        break;
    }
}

void Device_Mic::sendPhantom(uint8_t enable)
{
    _configData.phantom = enable;

    MsgData_STM32_POE msgData;
    msgData._online._u.u8Value = _configData.phantom;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_STM32_POE));
    sendPacket(Msg_Type_STM32_POE_Phantom, packBuff);
}

void Device_Mic::sendMicGain(int8_t value)
{
    _configData.Mic_Gain = value;

    MsgData_STM32_POE msgData;
    msgData._online._u.Mic_Gain = _configData.Mic_Gain;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_STM32_POE));
    sendPacket(Msg_Type_STM32_POE_AD_Gain, packBuff);
}

void Device_Mic::sendEq(bool bypass, EqData *eqPara)
{
    MsgData_STM32_POE msgData;

    _configData.Eq.bypass = bypass;
    memcpy(&_configData.Eq.eq, eqPara, 4*sizeof(EqData));
    msgData._online._u.eq = _configData.Eq.getNetPack();

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_STM32_POE));
    sendPacket(Msg_Type_STM32_POE_Eq, packBuff);
}

void Device_Mic::sendDyn(DynData &dynPara)
{
    memcpy(&_configData.Dyn, &dynPara, sizeof(DynData));

    MsgData_STM32_POE msgData;
    memcpy(&msgData._online._u.dyn, &_configData.Dyn, sizeof(DynData));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_STM32_POE));
    sendPacket(Msg_Type_STM32_POE_Dyn, packBuff);
}

void Device_Mic::sendXover(struct ConfigData_Xover xData)
{
    memcpy(&_configData.x, &xData, sizeof(struct ConfigData_Xover));

    MsgData_STM32_POE msgData;
    memcpy(&msgData._online._u.x, &_configData.x, sizeof(struct ConfigData_Xover));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_STM32_POE));
    sendPacket(Msg_Type_STM32_POE_Xover, packBuff);
}

void Device_Mic::sendStartMonitor()
{
    sendPacket_Nobody(Msg_Type_STM32_POE_Start_Monitor);
}

void Device_Mic::sendStopMonitor()
{
    sendPacket_Nobody(Msg_Type_STM32_POE_Stop_Monitor);
}

//void Device_Mic::sendSave2DevicePack()
//{
//    _saveState = DevSave_Save2Dev;
//    sendPacket_Nobody(Msg_Type_STM32_POE_Save);
//}

// 另存场景为文件
void Device_Mic::serialize(QString fileName)
{
//    QFile f(fileName);
//    f.open(QIODevice::WriteOnly);
//    QDataStream ds(&f);
//    ds << _configData;
//    f.close();
}

void Device_Mic::deserialize(QString fileName)
{
//    QFile file(fileName);
//    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
//    QDataStream ds(&file);

//    ConfigData_STM32_POE_0_2 configData;
//    ds >> configData;
//    initData(configData);

//    file.close();
}
