#include "stable.h"
#include "device_stm32_poe_0_1.h"
#include "protocal/packetbase.h"
#include "protocal/packet.h"
#include "device/devicemanager.h"
#include "deviceSetting/alarmconfig.h"
#include <QMessageBox>
#include "Frame/mainwindow.h"

Device_STM32_POE_0_1::Device_STM32_POE_0_1(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _configData.initConfigData();
    _alarmCfg = new AlarmConfig(devName, mac);
    _factoryMode = false;
    _saveState = DevSave_None;
}


void Device_STM32_POE_0_1::processPacket(PacketBase* pack)
{
//    qDebug()<<"Device_STM32_POE_0_1 receive Pack:" <<pack->getMsgType();
    switch(pack->getMsgType())
    {
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
        _configData = pack->getBody_STM32_POE().InitData_0_1.getDataPack();
        if(isAddedToProject()) {
            emit initConfigFromDevice();
            emit volumeChanged();
            emit muteChanged();
            emit phaseChanged();
            emit routerChanged();
        }

        switchStatus(Dev_Online);
        break;
    case MSG_TYPE_ADC_VALUE:
    {
        if(isAddedToProject()) {
            MsgData_STM32_POE msgData = pack->getBody_STM32_POE();
//            memcpy(&_adcData, &msgData.ADC_Value, sizeof(ADC_Data));
//            _alarmCfg->checkLimit(Dev_Type_STM32_POE_AES67_SPK_0_1, _adcData);

//            emit adcDataChanged(_alarmCfg);
        }
        break;
    }
    case Msg_Type_STM32_POE_Save_Ack:
        MainWindow::instance()->showNotice(tr("Save Success!"));
        break;
    default:
        qDebug()<< QString("Unknown packet type%1").arg(pack->getMsgType());
        break;
    }
}

void Device_STM32_POE_0_1::sendVol(int vol)
{
    MsgData_STM32_POE msgData;

    _configData.AD_gain.vol = vol;
    memcpy(&msgData._online._u.gain, &_configData.AD_gain, sizeof(struct ConfigData_STM32_Gain));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_STM32_POE));
    sendPacket(Msg_Type_STM32_POE_AD_Gain, packBuff);
}

void Device_STM32_POE_0_1::sendMute(bool mute)
{
    MsgData_STM32_POE msgData;

    _configData.AD_gain.mute = mute;
    memcpy(&msgData._online._u.gain, &_configData.AD_gain, sizeof(struct ConfigData_STM32_Gain));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_STM32_POE));
    sendPacket(Msg_Type_STM32_POE_AD_Gain, packBuff);
}

void Device_STM32_POE_0_1::sendEq(bool bypass, EqData* eqPara)
{
    MsgData_STM32_POE msgData;

    _configData.AD_Eq.bypass = bypass;
    memcpy(&_configData.AD_Eq.eq, eqPara, 4*sizeof(EqData));
    msgData._online._u.eq = _configData.AD_Eq.getNetPack();

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_STM32_POE));
    sendPacket(Msg_Type_STM32_POE_AD_Eq, packBuff);
}

void Device_STM32_POE_0_1::sendDyn(DynData& dynPara)
{
    memcpy(&_configData.AD_Dyn, &dynPara, sizeof(DynData));

    MsgData_STM32_POE msgData;
    memcpy(&msgData._online._u.dyn, &_configData.AD_Dyn, sizeof(DynData));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_STM32_POE));
    sendPacket(Msg_Type_STM32_POE_AD_Dyn, packBuff);
}

void Device_STM32_POE_0_1::sendSave()
{
    sendPacket_Nobody(Msg_Type_STM32_POE_Save);
}

// 另存场景为文件
void Device_STM32_POE_0_1::serialize(QString fileName)
{
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    QDataStream ds(&f);
    ds << _configData;
    f.close();
}

void Device_STM32_POE_0_1::deserialize(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QDataStream ds(&file);

    ConfigData_STM32_POE_0_1 configData;
    ds >> configData;
//    initData(configData);

    file.close();
}
