#include "stable.h"
#include "device_rt1052_AP400.h"
#include "protocal/packetbase.h"
#include "protocal/packet.h"
#include "device/devicemanager.h"
#include "deviceSetting/alarmconfig.h"
#include "Frame/mainwindow.h"


Device_RT1052_AP400::Device_RT1052_AP400(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _configData.initConfigData();
    _alarmCfg = new AlarmConfig(devName, mac);
    _factoryMode = false;
    _saveState = DevSave_None;
}


void Device_RT1052_AP400::sendVolume(quint8 type, quint8 ch, qint8 vol)
{
    MsgType id;
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    if (type == 0)//channel
    {
        _configData.channel_gain[ch].vol = vol;
        memcpy(&msgData._online._u.gain, &_configData.channel_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_ChannelGain;
    } else if (type == 1) {//A out
        _configData.Aout_gain[ch].vol = vol;
        memcpy(&msgData._online._u.gain, &_configData.Aout_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_Gain;
    }

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(id, packBuff);
}

void Device_RT1052_AP400::sendMute(quint8 type, quint8 ch, qint8 vol)
{
    MsgType id;
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    if (type == 0)//channel
    {
        _configData.channel_gain[ch].mute = vol;
        memcpy(&msgData._online._u.gain, &_configData.channel_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_ChannelGain;
    } else if (type == 1) {//A out
        _configData.Aout_gain[ch].mute = vol;
        memcpy(&msgData._online._u.gain, &_configData.Aout_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_Gain;
    }

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(id, packBuff);
}

void Device_RT1052_AP400::sendPhase(quint8 ch, qint8 vol)
{
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    _configData.Aout_gain[ch].phase = vol;
    memcpy(&msgData._online._u.gain, &_configData.Aout_gain[ch], sizeof(struct ConfigData_RT1052_Gain));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Gain, packBuff);
}

void Device_RT1052_AP400::sendDelay(quint8 ch, quint16 delay)
{
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    _configData.Aout_delay[ch].delay = delay;
    memcpy(&msgData._online._u.delay, &_configData.Aout_delay[ch], sizeof(struct ConfigData_RT1052_Delay));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Delay, packBuff);
}

void Device_RT1052_AP400::  sendEq(quint8 type, quint8 ch, bool bypass, EqData *eqPara)
{
    MsgType id;
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    if (type == 0)//channel
    {
        _configData.channel_eq[ch].bypass = bypass;
        memcpy(&_configData.channel_eq[ch].eq, eqPara, 4*sizeof(EqData));
        msgData._online._u.eq4B = _configData.channel_eq[ch].getNetPack();
        id = Msg_Type_iMXRT1052_POE_ChannelEq;
    } else if (type == 1) {//A out
        _configData.Aout_eq[ch].bypass = bypass;
        memcpy(&_configData.Aout_eq[ch].eq, eqPara, 15*sizeof(EqData));
        msgData._online._u.eq15B = _configData.Aout_eq[ch].getNetPack();
        id = Msg_Type_iMXRT1052_POE_Eq15B;
    }

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(id, packBuff);
}

void Device_RT1052_AP400::sendDyn(quint8 ch, DynData &dynPara)
{
    memcpy(&_configData.Aout_dyn[ch], &dynPara, sizeof(DynData));

    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.dyn, &_configData.Aout_dyn[ch], sizeof(DynData));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Dyn, packBuff);
}

void Device_RT1052_AP400::sendRoute(quint8 ch, quint8 mode)
{
    _configData.route.r[0] = mode;

    MsgData_RT1052_Poe msgData;
    memcpy(&msgData._online._u.route_AP400, &_configData.route, sizeof(struct ConfigData_AP400_Route));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    qDebug() << __FUNCTION__ << __LINE__ << packBuff.count();
    sendPacket(Msg_Type_iMXRT1052_POE_Route, packBuff);
}

void Device_RT1052_AP400::sendXover(quint8 ch, bool bypass, int low_freq, int high_freq, int low_stages, int high_stages)
{
    MsgData_RT1052_Poe msgData;

    if (ch < 8)
    {
        _configData.Aout_xover[ch].bypass = bypass;
        _configData.Aout_xover[ch].freq[0] = low_freq;
        _configData.Aout_xover[ch].freq[1] = high_freq;
        _configData.Aout_xover[ch].stages[0] = low_stages;
        _configData.Aout_xover[ch].stages[1] = high_stages;

        msgData._online.ch = ch;
        memcpy(&msgData._online._u.xover, &_configData.Aout_xover[ch], sizeof(ConfigData_Xover));
    } else {
        _configData.Nout_xover.bypass = bypass;
        _configData.Nout_xover.freq[0] = low_freq;
        _configData.Nout_xover.freq[1] = high_freq;
        _configData.Nout_xover.stages[0] = low_stages;
        _configData.Nout_xover.stages[1] = high_stages;

        msgData._online.ch = ch;
        memcpy(&msgData._online._u.xover, &_configData.Nout_xover, sizeof(ConfigData_Xover));
    }

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Xover, packBuff);
}

void Device_RT1052_AP400::sendSave()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Save);
}

void Device_RT1052_AP400::sendStartMonitor()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Start_Monitor);
}

void Device_RT1052_AP400::sendStopMonitor()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Stop_Monitor);
}

void Device_RT1052_AP400::sendDirectivityAngle(int8_t d)
{
    _configData.directivity = d;


    MsgData_RT1052_Poe msgData;
    msgData._online._u.i8_value = d;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_DIRECTIVITY, packBuff);
}

#ifdef Logo_BeiJingHuiSheng
void Device_RT1052_AP400::sendRouteMode(uint8_t d)
{
    _configData.route_mode = d;

    MsgData_RT1052_Poe msgData;
    msgData._online.ch = 0;
    msgData._online._u.u8d = d;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_HuiShengKeJi_AP400_RouteMode, packBuff);
}

void Device_RT1052_AP400::sendThreshold(int8_t d)
{
    _configData.threshold = d;

    MsgData_RT1052_Poe msgData;
    msgData._online._u.i8_value = d;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_HuiShengKeJi_SwitchThreshold, packBuff);
}

void Device_RT1052_AP400::sendAutoRecovery(int d)
{
    _configData.auto_recovery = d;

    MsgData_RT1052_Poe msgData;
    msgData._online._u.u8d = d;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_HuiShengKeJi_AutoRecovery, packBuff);
}

void Device_RT1052_AP400::sendDefaultOutput(int d)
{
    _configData.default_route = d;

    MsgData_RT1052_Poe msgData;
    msgData._online._u.u8d = d;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_HuiShengKeJi_DefaultOutput, packBuff);
}
#endif

void Device_RT1052_AP400::processPacket(PacketBase* pack)
{
#ifdef QT_DEBUG
//    qDebug()<<"Device_Amp4_4 receive Pack:" <<pack->getMsgType();
#endif
    switch(pack->getMsgType()) {
    case Msg_Type_HeartbeatAck:
        _saveState = DevSave_None;
        break;
    case Msg_Type_GetInitAck:
        _configData = pack->getBody_RT1052_Poe().AP400_InitData.getDataPack();
        if(isAddedToProject()) {
            emit initConfigFromDevice();
//            emit volumeChanged();
//            emit muteChanged();
//            emit phaseChanged();
//            emit routerChanged();
            sendStartMonitor();
        }

        switchStatus(Dev_Online);
        break;
    case Msg_Type_iMXRT1052_POE_Monitor_Msg:
//        qDebug() <<"/////////////////////////"<< QTime::currentTime();
        emit displaySig(pack->getBody_RT1052_Poe());
        break;
    case Msg_Type_iMXRT1052_POE_Save_Ack:        
        MainWindow::instance()->showNotice(tr("Save Success!"));
        break;
    default:
        qDebug()<< QString("Unknown packet type%1").arg(pack->getMsgType());
        break;
    }
}


// 另存场景为文件
void Device_RT1052_AP400::serialize(QString fileName)
{
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    QDataStream ds(&f);
    ds << _configData;
    f.close();
}

void Device_RT1052_AP400::deserialize(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QDataStream ds(&file);

    ConfigData_RT1052_AP400 configData;
    ds >> configData;
//    initData(configData);

    file.close();
}
