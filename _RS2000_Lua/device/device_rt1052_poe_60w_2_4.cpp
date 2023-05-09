#include "stable.h"
#include "device_rt1052_poe_60w_2_4.h"
#include "protocal/packetbase.h"
#include "protocal/packet.h"
#include "device/devicemanager.h"
#include "deviceSetting/alarmconfig.h"
#include "Frame/mainwindow.h"


Device_RT1052_Poe_60W_2_4::Device_RT1052_Poe_60W_2_4(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _configData.initConfigData();
    _alarmCfg = new AlarmConfig(devName, mac);
    _factoryMode = false;
    _saveState = DevSave_None;
}


void Device_RT1052_Poe_60W_2_4::sendVolume(quint8 type, quint8 ch, qint8 vol)
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

void Device_RT1052_Poe_60W_2_4::sendMute(quint8 type, quint8 ch, qint8 vol)
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

void Device_RT1052_Poe_60W_2_4::sendPhase(quint8 ch, qint8 vol)
{
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    _configData.Aout_gain[ch].phase = vol;
    memcpy(&msgData._online._u.gain, &_configData.Aout_gain[ch], sizeof(struct ConfigData_RT1052_Gain));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Gain, packBuff);
}

void Device_RT1052_Poe_60W_2_4::sendDelay(quint8 ch, quint16 delay)
{
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    _configData.Aout_delay[ch].delay = delay;
    memcpy(&msgData._online._u.delay, &_configData.Aout_delay[ch], sizeof(struct ConfigData_RT1052_Delay));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Delay, packBuff);
}

void Device_RT1052_Poe_60W_2_4::sendEq(quint8 type, quint8 ch, bool bypass, EqData *eqPara)
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

void Device_RT1052_Poe_60W_2_4::sendDyn(quint8 ch, DynData &dynPara)
{
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    //hrp:修正 dyn数据保存位置（out:_configData.Aout_dyn[ch], channel in:_configData.channel_dyn）
    if(msgData._online.ch == 0)//out
    {
        memcpy(&_configData.Aout_dyn[ch], &dynPara, sizeof(DynData));
        memcpy(&msgData._online._u.dyn, &_configData.Aout_dyn[ch], sizeof(DynData));

        QByteArray packBuff;
        packBuff.append((const char*)&msgData, sizeof(msgData._online));
        sendPacket(Msg_Type_iMXRT1052_POE_Dyn, packBuff);
    }else if(msgData._online.ch == 1)//channel in
    {
        memcpy(&_configData.channel_dyn, &dynPara, sizeof(DynData));
        memcpy(&msgData._online._u.dyn, &_configData.channel_dyn, sizeof(DynData));

        QByteArray packBuff;
        packBuff.append((const char*)&msgData, sizeof(msgData._online));
        sendPacket(Msg_Type_iMXRT1052_POE_ChannelDyn,packBuff);
    }
}

#ifdef Logo_BeiJingHuiSheng
void Device_RT1052_Poe_60W_2_4::sendMixerMode(quint8 mode)
{
    _configData.mixer_mode = mode;

    MsgData_RT1052_Poe msgData;
    msgData._online.ch = 0;
    msgData._online._u.u8d = mode;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_HuiShengKeJi_AP60_MixerMode, packBuff);
}
#endif

void Device_RT1052_Poe_60W_2_4::sendMixer(quint8 outch, quint8 inch, bool enable)
{
    _configData.Aout_mixer.m[outch][inch] = enable;

    MsgData_RT1052_Poe msgData;
    memcpy(&msgData._online._u.mixer, &_configData.Aout_mixer, sizeof(ConfigData_RT1052_Mixer_2_2));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));

    sendPacket(Msg_Type_iMXRT1052_POE_Mixer, packBuff);
}

void Device_RT1052_Poe_60W_2_4::sendRoute(quint8 ch, quint8 mode)
{
//    _configData.Nout_route.r[ch] = mode;

//    MsgData_RT1052_Poe msgData;
//    memcpy(&msgData._online._u.route, &_configData.Nout_route, sizeof(ConfigData_Amp4_4_route));

//    QByteArray packBuff;
//    packBuff.append((const char*)&msgData, sizeof(msgData._online));
//    sendPacket(Msg_Type_iMXRT1052_POE_Route, packBuff);
}

void Device_RT1052_Poe_60W_2_4::sendXover(quint8 ch, bool bypass, int low_freq, int high_freq, int low_stages, int high_stages)
{
    if(ch == 0) {
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
    }else if(ch == 2) {//hrp：2022-10-13修改  （修正 A-In2 xover数据保存位置）
        _configData.Nout_xover.bypass = bypass;
        _configData.Nout_xover.freq[0] = low_freq;
        _configData.Nout_xover.freq[1] = high_freq;
        _configData.Nout_xover.stages[0] = low_stages;
        _configData.Nout_xover.stages[1] = high_stages;

        MsgData_RT1052_Poe msgData;
        msgData._online.ch = ch;
        memcpy(&msgData._online._u.xover, &_configData.Nout_xover, sizeof(ConfigData_Xover));

        QByteArray packBuff;
        packBuff.append((const char*)&msgData, sizeof(msgData._online));
        sendPacket(Msg_Type_iMXRT1052_POE_Xover, packBuff);
    }
}

void Device_RT1052_Poe_60W_2_4::sendSave()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Save);
}

void Device_RT1052_Poe_60W_2_4::sendStartMonitor()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Start_Monitor);
}

void Device_RT1052_Poe_60W_2_4::sendStopMonitor()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Stop_Monitor);
}

void Device_RT1052_Poe_60W_2_4::processPacket(PacketBase* pack)
{
#ifdef QT_DEBUG
    qDebug()<<"Device_Amp4_4 receive Pack:" <<pack->getMsgType();
#endif
    switch(pack->getMsgType()) {
    case Msg_Type_HeartbeatAck:
//        if(_saveState == DevSave_Save2Dev) {
//            MainWindow::instance()->showNotice(tr("Save Success!"));
//        }
//        else if(_saveState == DevSave_LocalLoad) {
//            MainWindow::instance()->showNotice(tr("Load Success!"));
//        }
        _saveState = DevSave_None;
        break;
    case Msg_Type_GetInitAck:
        _configData = pack->getBody_RT1052_Poe().Line_60W_2_4_InitData.getDataPack();
//qDebug() << _configData.Aout_eq[0].eq[0].freq
//        << _configData.Aout_eq[0].eq[1].freq
//        << _configData.Aout_eq[0].eq[2].freq;
        if(isAddedToProject()) {
            emit initConfigFromDevice();
            sendStartMonitor();
//            emit volumeChanged();
//            emit muteChanged();
//            emit phaseChanged();
//            emit routerChanged();
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
void Device_RT1052_Poe_60W_2_4::serialize(QString fileName)
{
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    QDataStream ds(&f);
    ds << _configData;
    f.close();
}

void Device_RT1052_Poe_60W_2_4::deserialize(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QDataStream ds(&file);

    ConfigData_RT1052_Poe_60W_2_4 configData;
    ds >> configData;
    _configData = configData;
//    initData(configData);

    file.close();
}

void Device_RT1052_Poe_60W_2_4::sendAllConfigPack(bool b)
{
    //dyn
    for(int ch = 0; ch < 2; ch++) {//1:input, 0:output
        QByteArray packBuff;
        MsgData_RT1052_Poe msgData;
        msgData._online.ch = ch;

        if(msgData._online.ch == 0)
        {
            memcpy(&msgData._online._u.dyn, &_configData.Aout_dyn[ch], sizeof(DynData));
            packBuff.append((const char*)&msgData, sizeof(msgData._online));
            sendPacket(Msg_Type_iMXRT1052_POE_Dyn, packBuff);
        }else if(msgData._online.ch == 1)
        {
            memcpy(&msgData._online._u.dyn, &_configData.channel_dyn, sizeof(DynData));
            packBuff.append((const char*)&msgData, sizeof(msgData._online));
            sendPacket(Msg_Type_iMXRT1052_POE_ChannelDyn,packBuff);
        }
    }

    //gain、mute、phase
    for(int ch = 0; ch < 3; ch++){//channel
        QByteArray packBuff;
        MsgData_RT1052_Poe msgData;
        msgData._online.ch = ch;
        memcpy(&msgData._online._u.gain, &_configData.channel_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        packBuff.append((const char*)&msgData, sizeof(msgData._online));
        sendPacket(Msg_Type_iMXRT1052_POE_ChannelGain, packBuff);
    }
    int ch = 0;//A out
    QByteArray packBuff;
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.gain, &_configData.Aout_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Gain, packBuff);

    //delay
    ch = 0;//A out
    packBuff.clear();
    memset(&msgData, 0, sizeof (MsgData_RT1052_Poe));
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.delay, &_configData.Aout_delay[ch], sizeof(struct ConfigData_RT1052_Delay));
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Delay, packBuff);

    //eq
    for(int ch = 0; ch < 3; ch++) {//channel
        packBuff.clear();
        memset(&msgData, 0, sizeof (MsgData_RT1052_Poe));
        msgData._online.ch = ch;
        msgData._online._u.eq4B = _configData.channel_eq[ch].getNetPack();
        packBuff.append((const char*)&msgData, sizeof(msgData._online));
        sendPacket(Msg_Type_iMXRT1052_POE_ChannelEq, packBuff);
        QThread::usleep(50);
    }
    ch = 0;//A out
    packBuff.clear();
    memset(&msgData, 0, sizeof (MsgData_RT1052_Poe));
    msgData._online.ch = ch;
    msgData._online._u.eq15B = _configData.Aout_eq[ch].getNetPack();
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Eq15B, packBuff);

    //mixer
    packBuff.clear();
    memset(&msgData, 0, sizeof (MsgData_RT1052_Poe));
    memcpy(&msgData._online._u.mixer, &_configData.Aout_mixer, sizeof(ConfigData_RT1052_Mixer_2_2));
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Mixer, packBuff);

    //xover
    ch = 2;//AIn1
    packBuff.clear();
    memset(&msgData, 0, sizeof (MsgData_RT1052_Poe));
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.xover, &_configData.Nout_xover, sizeof(ConfigData_Xover));
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Xover, packBuff);
    QThread::usleep(100);
    ch = 0;//AOut1
    packBuff.clear();
    memset(&msgData, 0, sizeof (MsgData_RT1052_Poe));
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.xover, &_configData.Aout_xover[ch], sizeof(ConfigData_Xover));
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Xover, packBuff);

}
