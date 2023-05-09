#include "stable.h"
#include "device_amp_0_2_line.h"
#include "protocal/packetbase.h"
#include "protocal/packet.h"
#include "device/devicemanager.h"
#include "deviceSetting/alarmconfig.h"
#include "monitoring_aids/monitorblackstage.h"
#include "Frame/mainwindow.h"


Device_Amp_0_2_Line::Device_Amp_0_2_Line(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _configData.initConfigData();
    _alarmCfg = new AlarmConfig(devName, mac);
    _factoryMode = false;
    _saveState = DevSave_None;
}


void Device_Amp_0_2_Line::sendVolume(quint8 type, quint8 ch, qint8 vol)
{qDebug() << __FUNCTION__ << __LINE__ << type << ch << vol;
    MsgType id;
    MsgData_Amp_0_2_Line msgData;
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
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_0_2_Line));
    sendPacket(id, packBuff);
}

void Device_Amp_0_2_Line::sendMute(quint8 type, quint8 ch, qint8 vol)
{
    MsgType id;
    MsgData_Amp_0_2_Line msgData;
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
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_0_2_Line));
    sendPacket(id, packBuff);
}

void Device_Amp_0_2_Line::sendPhase(quint8 ch, qint8 vol)
{
    MsgData_Amp_0_2_Line msgData;
    msgData._online.ch = ch;

    _configData.Aout_gain[ch].phase = vol;
    memcpy(&msgData._online._u.gain, &_configData.Aout_gain[ch], sizeof(struct ConfigData_RT1052_Gain));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_0_2_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_Gain, packBuff);
}

void Device_Amp_0_2_Line::sendDelay(quint8 ch, quint16 delay)
{
    MsgData_Amp_0_2_Line msgData;
    msgData._online.ch = ch;

    _configData.Aout_delay[ch].delay = delay;
    memcpy(&msgData._online._u.delay, &_configData.Aout_delay[ch], sizeof(struct ConfigData_RT1052_Delay));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_0_2_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_Delay, packBuff);
}

void Device_Amp_0_2_Line::sendEq(quint8 ch, bool bypass, EqData *eqPara)
{
    MsgData_Amp_0_2_Line msgData;
    msgData._online.ch = ch;

    _configData.Aout_eq[ch].bypass = bypass;
    memcpy(&_configData.Aout_eq[ch].eq, eqPara, 4*sizeof(EqData));
    msgData._online._u.eq = _configData.Aout_eq[ch].getNetPack();

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_0_2_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_Eq, packBuff);
}

void Device_Amp_0_2_Line::sendDyn(quint8 ch, DynData &dynPara)
{
    memcpy(&_configData.Aout_dyn[ch], &dynPara, sizeof(DynData));

    MsgData_Amp_0_2_Line msgData;
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.dyn, &_configData.Aout_dyn[ch], sizeof(DynData));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_0_2_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_Dyn, packBuff);
}

void Device_Amp_0_2_Line::sendMixer(quint8 outch, quint8 inch, bool enable)
{
    _configData.Aout_mixer.m[outch][inch] = enable;

    MsgData_Amp_0_2_Line msgData;
    memcpy(&msgData._online._u.mixer, &_configData.Aout_mixer, sizeof(ConfigData_RT1052_Mixer_2_2));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_0_2_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_Mixer, packBuff);
}

void Device_Amp_0_2_Line::sendXover(quint8 ch, bool bypass, int low_freq, int high_freq, int low_stages, int high_stages)
{
    _configData.Aout_xover[ch].bypass = bypass;
    _configData.Aout_xover[ch].freq[0] = low_freq;
    _configData.Aout_xover[ch].freq[1] = high_freq;
    _configData.Aout_xover[ch].stages[0] = low_stages;
    _configData.Aout_xover[ch].stages[1] = high_stages;

    MsgData_Amp_0_2_Line msgData;
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.xover, &_configData.Aout_xover[ch], sizeof(ConfigData_Xover));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_0_2_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_Xover, packBuff);
}

void Device_Amp_0_2_Line::sendSave()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Save);
}

void Device_Amp_0_2_Line::sendStartMonitor()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Start_Monitor);
}

void Device_Amp_0_2_Line::sendStopMonitor()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Stop_Monitor);
}

void Device_Amp_0_2_Line::processPacket(PacketBase* pack)
{
#ifdef QT_DEBUG
//    qDebug()<<"Device_Amp4_4 receive Pack:" <<pack->getMsgType();
#endif
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
        _configData = pack->getBodyAmp_0_2_Line().InitData.getDataPack();
        if(isAddedToProject()) {
            emit initConfigFromDevice();
            sendStartMonitor();
        }

        switchStatus(Dev_Online);
        break;
    case Msg_Type_iMXRT1052_POE_Monitor_Msg:
        connect(this, SIGNAL(displaySig(MsgData_Amp_0_2_Line)), MonitorBlackStage::instance(), SLOT(onDisplaySlot(MsgData_Amp_0_2_Line)));
        emit displaySig(pack->getBodyAmp_0_2_Line());
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
void Device_Amp_0_2_Line::serialize(QString fileName)
{
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    QDataStream ds(&f);
    ds << _configData;
    f.close();
}

void Device_Amp_0_2_Line::deserialize(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QDataStream ds(&file);

    ConfigData_Amp_0_2_Line configData;
    ds >> configData;
//    initData(configData);

    file.close();
}
