#include "device_amp_module.h"
#include <QMessageBox>
#include "protocal/packetbase.h"
#include "deviceSetting/alarmconfig.h"
#include "device/devicemanager.h"
#include "monitoring_aids/monitorblackstage.h"
#include "Frame/mainwindow.h"

Device_Amp_Module::Device_Amp_Module(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _configData.initConfigData();
    _alarmCfg = new AlarmConfig(devName, mac);
    _factoryMode = false;
    _saveState = DevSave_None;
}
//另存场景为文件
void Device_Amp_Module::serialize(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QDataStream ds(&file);
    ds << _configData;
    file.close();
}

void Device_Amp_Module::deserialize(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QDataStream ds(&file);

    ConfigData_Amp4_4 configData;
    ds >> configData;
    file.close();
}

void Device_Amp_Module::processPacket(PacketBase *pack)
{
    switch(pack->getMsgType())
    {
    case Msg_Type_HeartbeatAck:
        _saveState = DevSave_None;
        break;
    case Msg_Type_GetInitAck:
        _configData = pack->getBody_RT1052_Poe().Line_4_4_InitData.getDataPack();
        if(isAddedToProject()) {
            emit initConfigFromDevice();
            sendStartMoniter();
        }
        switchStatus(Dev_Online);
        break;
    case Msg_Type_iMXRT1052_POE_Monitor_Msg:
//        qDebug()<<"------------------------------------------------------";
//        connect(this, SIGNAL(disPlaySig(MsgData_RT1052_Poe)), MonitorBlackStage::instance(), SLOT(onDisplaySlot(MsgData_RT1052_Poe)));
        emit disPlaySig(pack->getBody_RT1052_Poe());
        break;
    case Msg_Type_iMXRT1052_POE_Save_Ack:
        MainWindow::instance()->showNotice(tr("Save Success!"));
        break;
    default:
        qDebug()<<QString("Unknown packet type%1").arg(pack->getMsgType());
        break;
    }
}

void Device_Amp_Module::sendVolume(quint8 type, quint8 ch, qint8 vol)
{
    MsgType id;
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    if(type == 0)
    {
        _configData.channel_gain[ch].vol = vol;
        memcpy(&msgData._online._u.gain, &_configData.channel_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_ChannelGain;
    }else if(type == 1)
    {
        _configData.Aout_gain[ch].vol = vol;
        memcpy(&msgData._online._u.gain, &_configData.Aout_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_Gain;
    }
    QByteArray packBuff;
    packBuff.append((const char *)&msgData, sizeof(msgData._online));
    sendPacket(id,packBuff);
}

void Device_Amp_Module::sendMute(quint8 type, quint8 ch, qint8 vol)
{
    MsgType id;
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;
    if(type == 0)
    {
        _configData.channel_gain[ch].mute = vol;
        memcpy(&msgData._online._u.gain, &_configData.channel_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_ChannelGain;
    }else if(type == 1)
    {
        _configData.Aout_gain[ch].mute = vol;
        memcpy(&msgData._online._u.gain, &_configData.Aout_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_Gain;
    }
    QByteArray packBuff;
    packBuff.append((const char *)&msgData, sizeof(msgData._online));
    sendPacket(id,packBuff);
}

void Device_Amp_Module::sendPhase(quint8 ch, qint8 vol)
{
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    _configData.Aout_gain[ch].phase = vol;
    memcpy(&msgData._online._u.gain, &_configData.Aout_gain[ch], sizeof(struct ConfigData_RT1052_Gain));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Gain, packBuff);
}

void Device_Amp_Module::sendDelay(quint8 ch, quint16 delay)
{
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    _configData.Aout_delay[ch].delay = delay;
    memcpy(&msgData._online._u.delay, &_configData.Aout_delay[ch], sizeof(struct ConfigData_RT1052_Delay));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Delay, packBuff);
}

void Device_Amp_Module::sendEq(quint8 type, quint8 ch, bool bypass, EqData *eqPara)
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
        memcpy(&_configData.Aout_eq[ch].eq, eqPara, 4*sizeof(EqData));
        msgData._online._u.eq4B = _configData.Aout_eq[ch].getNetPack();
        id = Msg_Type_iMXRT1052_POE_Eq;
    }

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(id, packBuff);
}

void Device_Amp_Module::sendDyn(quint8 ch, DynData &dynPara)
{
    memcpy(&_configData.Aout_dyn[ch], &dynPara, sizeof(DynData));

    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.dyn, &_configData.Aout_dyn[ch], sizeof(DynData));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Dyn, packBuff);
}

void Device_Amp_Module::sendMixer(quint8 outch, quint8 inch, bool enable)
{
    _configData.Aout_mixer.m[outch][inch] = enable;

    MsgData_RT1052_Poe msgData;
    memcpy(&msgData._online._u.mixer, &_configData.Aout_mixer, sizeof(ConfigData_RT1052_Mixer_4_8));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Mixer, packBuff);
}

void Device_Amp_Module::sendRoute(quint8 ch, quint8 mode)
{
    _configData.Nout_route.r[ch] = mode;

    MsgData_RT1052_Poe msgData;
    memcpy(&msgData._online._u.route, &_configData.Nout_route, sizeof(ConfigData_Amp4_4_route));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Route, packBuff);
}

void Device_Amp_Module::sendXover(quint8 ch, bool bypass, int low_freq, int high_freq, int low_stages, int high_stages)
{
    _configData.Aout_xover[ch].bypass = bypass;
    _configData.Aout_xover[ch].freq[0] = low_freq;
    _configData.Aout_xover[ch].freq[1] = high_freq;
    _configData.Aout_xover[ch].stages[0] = low_stages;
    _configData.Aout_xover[ch].stages[1] = high_stages;

    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.xover, &_configData.Aout_xover[ch], sizeof(ConfigData_Xover));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Xover, packBuff);
}

void Device_Amp_Module::sendSave()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Save);
}

void Device_Amp_Module::sendStartMoniter()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Start_Monitor);
}

void Device_Amp_Module::sendStopMoniter()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Stop_Monitor);
}
