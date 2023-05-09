#include "stable.h"
#include "device_amp_2_0_mic_line.h"
#include "protocal/packetbase.h"
#include "protocal/packet.h"
#include "device/devicemanager.h"
#include "deviceSetting/alarmconfig.h"
#include "Frame/mainwindow.h"

Device_Amp_2_0_Mic_Line::Device_Amp_2_0_Mic_Line(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _configData.initConfigData();
    _alarmCfg = new AlarmConfig(devName, mac);
    _factoryMode = false;
    _saveState = DevSave_None;
}



void Device_Amp_2_0_Mic_Line::sendMicGain(int value)
{
    MsgData_Amp_2_0_Mic_Line msgData;
    msgData._online._u.mic_gain = value;

    _configData.mic_gain = value;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_2_0_Mic_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_MIC_Gain, packBuff);
}

void Device_Amp_2_0_Mic_Line::sendMicPhantom(bool enable)
{
    MsgData_Amp_2_0_Mic_Line msgData;
    msgData._online._u.mic_phantom = enable;

    _configData.mic_phantom = enable;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_2_0_Mic_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_MIC_Phantom, packBuff);
}

void Device_Amp_2_0_Mic_Line::sendGain(int type, int ch, int vol)
{
    MsgType id;
    MsgData_Amp_2_0_Mic_Line msgData;
    msgData._online.ch = ch;

    if (type == 0)//channel gain
    {
        _configData.gain[ch].vol = vol;
        memcpy(&msgData._online._u.gain, &_configData.gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_ChannelGain;
    } else if (type == 1) {//N phase
        _configData.Nout_gain[ch].vol = vol;
        memcpy(&msgData._online._u.gain, &_configData.Nout_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_Gain;
    }

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_2_0_Mic_Line));
    sendPacket(id, packBuff);
}

void Device_Amp_2_0_Mic_Line::sendMute(quint8 type, quint8 ch, qint8 vol)
{
    MsgType id;
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    if (type == 0)//channel
    {
        _configData.gain[ch].mute = vol;
        memcpy(&msgData._online._u.gain, &_configData.gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_ChannelGain;
    } else if (type == 1) {//A out
        _configData.Nout_gain[ch].mute = vol;
        memcpy(&msgData._online._u.gain, &_configData.Nout_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_Gain;
    }

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(id, packBuff);
}

void Device_Amp_2_0_Mic_Line::sendPhase(int ch, bool enable)
{
    MsgData_Amp_2_0_Mic_Line msgData;
    msgData._online.ch = ch;

    _configData.gain[ch].phase = enable;
    memcpy(&msgData._online._u.gain, &_configData.gain[ch], sizeof(struct ConfigData_RT1052_Gain));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_2_0_Mic_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_ChannelGain, packBuff);
}

void Device_Amp_2_0_Mic_Line::sendXover(quint8 ch, bool bypass, int low_freq, int high_freq, int low_stages, int high_stages)
{
    _configData.xover[ch].bypass = bypass;
    _configData.xover[ch].freq[0] = low_freq;
    _configData.xover[ch].freq[1] = high_freq;
    _configData.xover[ch].stages[0] = low_stages;
    _configData.xover[ch].stages[1] = high_stages;

    MsgData_Amp_2_0_Mic_Line msgData;
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.xover, &_configData.xover[ch], sizeof(ConfigData_Xover));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_2_0_Mic_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_ChannelXover, packBuff);
}

void Device_Amp_2_0_Mic_Line::sendMixer(quint8 outch, quint8 inch, bool enable)
{
    _configData.Nout_mixer.m[outch][inch] = enable;

    MsgData_Amp_2_0_Mic_Line msgData;
    memcpy(&msgData._online._u.mixer, &_configData.Nout_mixer, sizeof(ConfigData_RT1052_Mixer_2_2));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_2_0_Mic_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_Mixer, packBuff);
}

void Device_Amp_2_0_Mic_Line::sendEq(quint8 ch, bool bypass, EqData *eqPara)
{
    MsgData_Amp_2_0_Mic_Line msgData;
    msgData._online.ch = ch;

    _configData.eq[ch].bypass = bypass;
    memcpy(&_configData.eq[ch].eq, eqPara, 4*sizeof(EqData));
    msgData._online._u.eq = _configData.eq[ch].getNetPack();

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_2_0_Mic_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_ChannelEq, packBuff);
}

void Device_Amp_2_0_Mic_Line::sendDyn(quint8 ch, DynData &dynPara)
{
    memcpy(&_configData.dyn[ch], &dynPara, sizeof(DynData));

    MsgData_Amp_2_0_Mic_Line msgData;
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.dyn, &_configData.dyn[ch], sizeof(DynData));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Amp_2_0_Mic_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_ChannelDyn, packBuff);
}

void Device_Amp_2_0_Mic_Line::sendSave()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Save);
}

void Device_Amp_2_0_Mic_Line::sendStartMonitor()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Start_Monitor);
}

void Device_Amp_2_0_Mic_Line::sendStopMonitor()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Stop_Monitor);
}

void Device_Amp_2_0_Mic_Line::sendMic_open(quint8 open)
{
    _configData.mic_open = open;
    MsgData_Amp_2_0_Mic_Line msgData;
    memcpy(&msgData._online._u.mic_open, &_configData.mic_open, sizeof(quint8));
    QByteArray packBuff;
    packBuff.append((const char *)&msgData,sizeof(MsgData_Amp_2_0_Mic_Line));
    sendPacket(Msg_Type_iMXRT1052_POE_MIC_Open,packBuff);
}

void Device_Amp_2_0_Mic_Line::processPacket(PacketBase* pack)
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
        _configData = pack->getBodyAmp_2_0_Mic_Line().InitData.getDataPack();
        if(isAddedToProject()) {
            emit initConfigFromDevice();
            sendStartMonitor();
        }
        switchStatus(Dev_Online);
        break;
    case Msg_Type_iMXRT1052_POE_Monitor_Msg:
        emit displaySig(pack->getBodyAmp_2_0_Mic_Line());
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
void Device_Amp_2_0_Mic_Line::serialize(QString fileName)
{
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    QDataStream ds(&f);
    ds << _configData;
    f.close();
}

void Device_Amp_2_0_Mic_Line::deserialize(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QDataStream ds(&file);

    ConfigData_Amp_2_0_Mic_Line configData;
    ds >> configData;
//    initData(configData);

    file.close();
}
