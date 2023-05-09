#include "stable.h"
#include "device_amp_quad.h"
#include "protocal/packetbase.h"
#include "protocal/packet.h"
#include "device/devicemanager.h"
#include "deviceSetting/alarmconfig.h"
#include "Frame/mainwindow.h"

Device_Amp_Quad::Device_Amp_Quad(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _configData.initConfigData();
    _alarmCfg = new AlarmConfig(devName, mac);
    _factoryMode = false;
    _saveState = DevSave_None;
}


void Device_Amp_Quad::sendVolume(quint8 type, quint8 ch, qint8 vol)
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

void Device_Amp_Quad::sendMute(quint8 type, quint8 ch, qint8 vol)
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

void Device_Amp_Quad::sendPhase(quint8 ch, qint8 vol)
{
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    _configData.Aout_gain[ch].phase = vol;
    memcpy(&msgData._online._u.gain, &_configData.Aout_gain[ch], sizeof(struct ConfigData_RT1052_Gain));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Gain, packBuff);
}

void Device_Amp_Quad::sendDelay(quint8 ch, quint16 delay)
{
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    _configData.Aout_delay[ch].delay = delay;
    memcpy(&msgData._online._u.delay, &_configData.Aout_delay[ch], sizeof(struct ConfigData_RT1052_Delay));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Delay, packBuff);
}

void Device_Amp_Quad::sendEq(quint8 type, quint8 ch, bool bypass, EqData *eqPara)
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

void Device_Amp_Quad::sendDyn(quint8 ch, DynData &dynPara)
{
    memcpy(&_configData.Aout_dyn[ch], &dynPara, sizeof(DynData));

    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.dyn, &_configData.Aout_dyn[ch], sizeof(DynData));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Dyn, packBuff);
}

void Device_Amp_Quad::sendMixer(quint8 outch, quint8 inch, bool enable)
{
    _configData.Aout_mixer.m[outch][inch] = enable;

    MsgData_RT1052_Poe msgData;
    memcpy(&msgData._online._u.mixer, &_configData.Aout_mixer, sizeof(ConfigData_RT1052_Mixer_4_8));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Mixer, packBuff);
}

void Device_Amp_Quad::sendRoute(quint8 ch, quint8 mode)
{
    _configData.Nout_route.r[ch] = mode;

    MsgData_RT1052_Poe msgData;
    memcpy(&msgData._online._u.route, &_configData.Nout_route, sizeof(ConfigData_Amp4_4_route));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_iMXRT1052_POE_Route, packBuff);
}

void Device_Amp_Quad::sendXover(quint8 ch, bool bypass, int low_freq, int high_freq, int low_stages, int high_stages)
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

void Device_Amp_Quad::sendSave()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Save);
}

void Device_Amp_Quad::sendStartMonitor()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Start_Monitor);
}

void Device_Amp_Quad::sendStopMonitor()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Stop_Monitor);
}

void Device_Amp_Quad::processPacket(PacketBase* pack)
{
#ifdef QT_DEBUG
//    qDebug()<<"Device_Amp4_4 receive Pack:" <<pack->getMsgType();
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
        _configData = pack->getBody_RT1052_Poe().Line_4_4_InitData.getDataPack();
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
//        qDebug() << QTime::currentTime();
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
void Device_Amp_Quad::serialize(QString fileName)
{
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    QDataStream ds(&f);
    ds << _configData;
    f.close();
}

void Device_Amp_Quad::deserialize(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QDataStream ds(&file);

    ConfigData_Amp4_4 configData;
    ds >> configData;
//    initData(configData);

    file.close();
}

//void Device_Amp_Quad::initData(const ConfigData_Amp4_4& configData)
//{
//    if (_factoryMode)
//    {
//        for (int i = 0; i < 4; ++i)
//        {
//            _configData.CH[i].ControlValue = configData.CH[i].ControlValue;
//            _configData.CH[i].Eq           = configData.CH[i].Eq;
//            _configData.CH[i].divider      = configData.CH[i].divider;
//        }
////        _configData.ch1_ControlValue = configData.ch1_ControlValue;
////        _configData.ch1_Eq           = configData.ch1_Eq;
////        _configData.ch1_divider      = configData.ch1_divider;
////        _configData.ch1_OutputGain   = configData.ch1_OutputGain;

////        _configData.ch2_ControlValue = configData.ch2_ControlValue;
////        _configData.ch2_Eq           = configData.ch2_Eq;
////        _configData.ch2_divider      = configData.ch2_divider;
////        _configData.ch2_OutputGain   = configData.ch2_OutputGain;
//    }
//    else
//    {
////        quint8 swt = _configData.Switch;
//        memcpy(&_configData, &configData, sizeof(ConfigData_Amp4_4));
////        _configData.Switch = 1;
//    }
//}

//QString Device_Amp_Quad::getRouterInfo(quint8 ch)
//{
//    QString str;
//    switch(ch)
//    {
//    case Amp4_4Router::INPUT_SELECT_A1:
//        str = tr("Line#1");
//        break;
//    case Amp4_4Router::INPUT_SELECT_A2:
//        str = tr("Line#2");
//        break;
//    case Amp4_4Router::INPUT_SELECT_A3:
//        str = tr("Line#3");
//        break;
//    case Amp4_4Router::INPUT_SELECT_A4:
//        str = tr("Line#4");
//        break;
//    case Amp4_4Router::INPUT_SELECT_N1:
//        str = tr("Net#1");
//        break;
//    case Amp4_4Router::INPUT_SELECT_N2:
//        str = tr("Net#2");
//        break;
//    case Amp4_4Router::INPUT_SELECT_N3:
//        str = tr("Net#3");
//        break;
//    case Amp4_4Router::INPUT_SELECT_N4:
//        str = tr("Net#4");
//        break;
//    default:
//        Q_ASSERT(false);
//        break;
//    }
//    return str;
//}
