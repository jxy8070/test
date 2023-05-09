#include "stable.h"
#include "device_mic_quad.h"
#include "protocal/packetbase.h"
#include "protocal/packet.h"
#include "device/devicemanager.h"
#include "deviceSetting/alarmconfig.h"
#include "Frame/mainwindow.h"


Device_Mic_Quad::Device_Mic_Quad(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _configData.initConfigData();
    _alarmCfg = new AlarmConfig(devName, mac);
    _factoryMode = false;
    _saveState = DevSave_None;
}

void Device_Mic_Quad::sendMicGain(int ch, int value)
{
    MsgData_Mic_Quad msgData;
    msgData._online.ch = ch;
    msgData._online._u.mic_gain = value;

    _configData.gain[ch] = value;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Mic_Quad));
    sendPacket(Msg_Type_iMXRT1052_POE_MIC_Gain, packBuff);
}

void Device_Mic_Quad::sendMicPhantom(int ch, bool enable)
{
    MsgData_Mic_Quad msgData;
    msgData._online.ch = ch;
    msgData._online._u.mic_phantom = enable;

    _configData.phantom[ch] = enable;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Mic_Quad));
    sendPacket(Msg_Type_iMXRT1052_POE_MIC_Phantom, packBuff);
}

void Device_Mic_Quad::sendGain(int type, int ch, int vol)
{
    MsgType id;
    MsgData_Mic_Quad msgData;
    msgData._online.ch = ch;

    if (type == 0)//channel gain
    {
        _configData.N_gain[ch].vol = vol;
        memcpy(&msgData._online._u.gain, &_configData.N_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_ChannelGain;
    } else if (type == 1) {//N phase
        _configData.NOut_gain[ch].vol = vol;
        memcpy(&msgData._online._u.gain, &_configData.NOut_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_Gain;
    }

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Mic_Quad));
    sendPacket(id, packBuff);
}

void Device_Mic_Quad::sendMute(quint8 type, quint8 ch, qint8 vol)
{
    MsgType id;
    MsgData_RT1052_Poe msgData;
    msgData._online.ch = ch;

    if (type == 0)//channel
    {
        _configData.N_gain[ch].mute = vol;
        memcpy(&msgData._online._u.gain, &_configData.N_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_ChannelGain;
    } else if (type == 1) {//A out
        _configData.NOut_gain[ch].mute = vol;
        memcpy(&msgData._online._u.gain, &_configData.NOut_gain[ch], sizeof(struct ConfigData_RT1052_Gain));
        id = Msg_Type_iMXRT1052_POE_Gain;
    }

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(id, packBuff);
}

void Device_Mic_Quad::sendPhase(int ch, bool enable)
{
    MsgData_Mic_Quad msgData;
    msgData._online.ch = ch;

    _configData.N_gain[ch].phase = enable;
    memcpy(&msgData._online._u.gain, &_configData.N_gain[ch], sizeof(struct ConfigData_RT1052_Gain));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Mic_Quad));
    sendPacket(Msg_Type_iMXRT1052_POE_ChannelGain, packBuff);
}

void Device_Mic_Quad::sendXover(quint8 ch, bool bypass, int high_freq, int high_stages)
{
    _configData.N_xover[ch].bypass = bypass;
    _configData.N_xover[ch].freq[1] = high_freq;
    _configData.N_xover[ch].stages[1] = high_stages;

    MsgData_Mic_Quad msgData;
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.xover, &_configData.N_xover[ch], sizeof(ConfigData_Xover));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Mic_Quad));
    sendPacket(Msg_Type_iMXRT1052_POE_ChannelXover, packBuff);
}

void Device_Mic_Quad::sendMixer(quint8 outch, quint8 inch, bool enable)
{
    _configData.N_mixer.m[outch][inch] = enable;

    MsgData_Mic_Quad msgData;
    memcpy(&msgData._online._u.mixer, &_configData.N_mixer, sizeof(ConfigData_RT1052_Mixer_4_4));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Mic_Quad));
    sendPacket(Msg_Type_iMXRT1052_POE_Mixer, packBuff);
}

void Device_Mic_Quad::sendEq(quint8 ch, bool bypass, EqData *eqPara)
{
    MsgData_Mic_Quad msgData;
    msgData._online.ch = ch;

    _configData.N_eq[ch].bypass = bypass;
    memcpy(&_configData.N_eq[ch].eq, eqPara, 4*sizeof(EqData));
    msgData._online._u.eq = _configData.N_eq[ch].getNetPack();

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Mic_Quad));
    sendPacket(Msg_Type_iMXRT1052_POE_ChannelEq, packBuff);
}

void Device_Mic_Quad::sendDyn(quint8 ch, DynData &dynPara)
{
    memcpy(&_configData.N_dyn[ch], &dynPara, sizeof(DynData));

    MsgData_Mic_Quad msgData;
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.dyn, &_configData.N_dyn[ch], sizeof(DynData));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(MsgData_Mic_Quad));
    sendPacket(Msg_Type_iMXRT1052_POE_ChannelDyn, packBuff);
}

void Device_Mic_Quad::sendSave()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Save);
}

void Device_Mic_Quad::sendStartMonitor()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Start_Monitor);
}

void Device_Mic_Quad::sendStopMonitor()
{
    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Stop_Monitor);
}

void Device_Mic_Quad::sendGroup(int startPort, int endPort)
{
    MsgData_Mic_Quad MsgData;
    QByteArray packBuff;
    MsgData._huisheng_mic.start_port_no = startPort;
    MsgData._huisheng_mic.end_port_no = endPort;
    MsgData._huisheng_mic.group_no = _group;
    packBuff.append((const char *)&MsgData,sizeof(MsgData_Mic_Quad));
    sendPacket(Msg_Type_iMXRT1052_POE_HuiShengKeJi_Mic,packBuff);
}

void Device_Mic_Quad::sendAFCClear()
{
    sendPacket_Nobody(Msg_type_iMXRT1052_POE_Control_CMD);
}

void Device_Mic_Quad::processPacket(PacketBase* pack)
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
        _configData = pack->getBodyMicQuad().InitData.getDataPack();
        if(isAddedToProject()) {
            emit initConfigFromDevice();
            emit volumeChanged();
            emit muteChanged();
            emit phaseChanged();
            emit routerChanged();
            sendStartMonitor();
        }

        switchStatus(Dev_Online);
        break;
    case Msg_Type_iMXRT1052_POE_Monitor_Msg:
        emit displaySig(pack->getBodyMicQuad());
        break;
    case Msg_Type_iMXRT1052_POE_Save_Ack:
        MainWindow::instance()->showNotice(tr("Save Success!"));
        break;
    default:
        qDebug() << __FUNCTION__ << __LINE__
                 << "Unknown packet type" << pack->getMsgType();
        break;
    }
}


//void Device_Mic_Quad::sendAllConfigPack(bool isNeedDisSuccess)
//{
//    ConfigData_Mic_Quad_Net msgData = _configData.getNetPack();

//    if (isNeedDisSuccess)
//    {
//        _saveState = DevSave_LocalLoad;
//    }

//    QByteArray packBuff;
//    packBuff.append((const char*)&msgData, sizeof(ConfigData_Mic_Quad_Net));
//    sendPacket(MSG_TYPE_INIT, packBuff);

//    emit volumeChanged();
//    emit muteChanged();
//    emit phaseChanged();
//    emit routerChanged();
//}

//void Device_Mic_Quad::sendSwitch(bool flag)
//{
////    _configData.Switch = flag;
////    sendCommonDataPack(1, _configData.ch1_ControlValue);
////    sendCommonDataPack(2, _configData.ch2_ControlValue);
//}

//void Device_Mic_Quad::sendVolume(quint8 ch, qint8 vol)
//{
////    _configData.CH[ch].ControlValue.volume = vol;
////    sendCommonDataPack(ch, _configData.CH[ch].ControlValue);
////    switch (ch)
////    {
////    case AMP_CH1:
////        _configData.ch1_ControlValue.volume = vol;
////        sendCommonDataPack(ch, _configData.ch1_ControlValue);
////        break;
////    case AMP_CH2:
////        _configData.ch2_ControlValue.volume = vol;
////        sendCommonDataPack(ch, _configData.ch2_ControlValue);
////        break;
////    default:
////        break;
////    }

//    emit volumeChanged();
//}

//void Device_Mic_Quad::sendMute(quint8 ch, bool flag)
//{
////    _configData.CH[ch].ControlValue.mute = flag;
////    sendCommonDataPack(ch, _configData.CH[ch].ControlValue);
////    switch (ch)
////    {
////    case AMP_CH1:
////        _configData.ch1_ControlValue.mute = flag;
////        sendCommonDataPack(ch, _configData.ch1_ControlValue);
////        break;
////    case AMP_CH2:
////        _configData.ch2_ControlValue.mute = flag;
////        sendCommonDataPack(ch, _configData.ch2_ControlValue);
////        break;
////    default:
////        break;
////    }

//    emit muteChanged();
//}

//void Device_Mic_Quad::sendPhase(quint8 ch, bool flag)
//{
////    _configData.CH[ch].ControlValue.phase = flag;
////    sendCommonDataPack(ch, _configData.CH[ch].ControlValue);
////    switch (ch)
////    {
////    case AMP_CH1:
////        _configData.ch1_ControlValue.phase = flag;
////        sendCommonDataPack(ch, _configData.ch1_ControlValue);
////        break;
////    case AMP_CH2:
////        _configData.ch2_ControlValue.phase = flag;
////        sendCommonDataPack(ch, _configData.ch2_ControlValue);
////        break;
////    default:
////        break;
////    }

//    emit phaseChanged();
//}

//void Device_Mic_Quad::sendRouter(quint8 ch, quint8 routerCh)
//{
////    _configData.CH[ch].ControlValue.channel = routerCh;
////    sendCommonDataPack(ch, _configData.CH[ch].ControlValue);
////    switch (ch)
////    {
////    case AMP_CH1:
////        _configData.ch1_ControlValue.channel = routerCh;
////        sendCommonDataPack(ch, _configData.ch1_ControlValue);
////        break;
////    case AMP_CH2:
////        _configData.ch2_ControlValue.channel = routerCh;
////        sendCommonDataPack(ch, _configData.ch2_ControlValue);
////        break;
////    default:
////        break;
////    }

//    emit routerChanged();
//}

//void Device_Mic_Quad::sendCommonDataPack(quint8 ch, ControlValue vlu)
//{
//    MsgData_Mic_Quad msgData;
////    msgData.Other.power = 1;
////    msgData.Other.ch    = ch + 1;
////    memcpy(&msgData.Other.value, &vlu, sizeof(ControlValue));

////    if (msgData.Other.power == 0x00)
////    {
////        msgData.Other.value.mute = 0x01;
////    }
//////    qDebug() <<msgData.Other.power
//////             <<msgData.Other.ch
//////             <<msgData.Other.value.gain
//////             <<msgData.Other.value.channel
//////             <<msgData.Other.value.delay
//////             <<msgData.Other.value.mute
//////             <<msgData.Other.value.phase
//////             <<msgData.Other.value.volume;

////    QByteArray packBuff;
////    packBuff.append((const char*)&msgData.Other, sizeof(CommonData_Amp));
//    //    sendPacket(MSG_TYPE_OTHER, packBuff);
//}

//void Device_Mic_Quad::sendMicGain(uint8_t ch, uint8_t gain)
//{
//    _configData.CH[ch].gain = gain;

//    MsgData_Mic_Quad msgData;
//    msgData.MGain.ch = ch;
//    msgData.MGain.gain = gain;

//    QByteArray packBuff;
//    packBuff.append((const char*)&msgData.MGain, 4);
//    sendPacket(Msg_Type_iMXRT1052_POE_MIC_Gain, packBuff);
//}

//void Device_Mic_Quad::sendPhantom(uint8_t ch, uint8_t enable)
//{
//    _configData.CH[ch].phantom = enable;
//    MsgData_Mic_Quad msgData;
//    msgData.Phantom_Enable.ch = ch;
//    msgData.Phantom_Enable.phantom_enable = enable;

//    QByteArray packBuff;
//    packBuff.append((const char*)&msgData.Phantom_Enable, 4);
//    sendPacket(Msg_Type_iMXRT1052_POE_MIC_Phantom, packBuff);
//}

//void Device_Mic_Quad::sendEqPack(quint8 ch, bool bypass, EqData* eqPara)
//{
////    MsgData_Mic_Quad msgData;
////    msgData._EqData.ch = ch + 1;

////    _configData.CH[ch].Eq.bypass = bypass;
////    memcpy(&_configData.CH[ch].Eq, eqPara, 4*sizeof(EqData));
////    msgData._EqData.Eq = _configData.CH[ch].Eq.getNetPack();
//////    if(ch == AMP_CH1)
//////    {
//////        _configData.ch1_Eq.bypass = bypass;
//////        memcpy(&_configData.ch1_Eq.eq, eqPara, 4*sizeof(EqData));

//////        msgData._EqData.Eq = _configData.ch1_Eq.getNetPack();
//////    }
//////    else if(ch == AMP_CH2)
//////    {
//////        _configData.ch2_Eq.bypass = bypass;
//////        memcpy(&_configData.ch2_Eq.eq, eqPara, 4*sizeof(EqData));

//////        msgData._EqData.Eq = _configData.ch2_Eq.getNetPack();
//////    }
//////    else
//////    {
//////        Q_ASSERT(false);
//////    }

//////    qDebug()<< msgData._EqData.ch
//////            << msgData._EqData.Eq.bypass
//////            << msgData._EqData.Eq.eq[0].freq
//////            << msgData._EqData.Eq.eq[0].gain
//////            << msgData._EqData.Eq.eq[0].q
//////            << msgData._EqData.Eq.eq[1].freq
//////            << msgData._EqData.Eq.eq[1].gain
//////            << msgData._EqData.Eq.eq[1].q
//////            << msgData._EqData.Eq.eq[2].freq
//////            << msgData._EqData.Eq.eq[2].gain
//////            << msgData._EqData.Eq.eq[2].q
//////            << msgData._EqData.Eq.eq[3].freq
//////            << msgData._EqData.Eq.eq[3].gain
//////            << msgData._EqData.Eq.eq[3].q;

////    QByteArray packBuff;
////    packBuff.append((const char*)&msgData._EqData, sizeof(msgData._EqData));
////    sendPacket(MSG_TYPE_EQ, packBuff);
//}

//void Device_Mic_Quad::sendDynPack(quint8 ch, DynData& dynPara)
//{
//////    if(ch == AMP_CH1) memcpy(&_configData.ch1_Dyn, &dynPara, sizeof(DynData));
//////    else if(ch == AMP_CH2)  memcpy(&_configData.ch2_Dyn, &dynPara, sizeof(DynData));
////    memcpy(&_configData.CH[ch].Dyn, &dynPara, sizeof(DynData));

////    MsgData_Mic_Quad msgData;
////    msgData._Dyn.ch = ch + 1;
////    msgData._Dyn.dyn = dynPara;

//////    qDebug()<< msgData._Dyn.ch
//////            << msgData._Dyn.dyn.bypass
//////            << msgData._Dyn.dyn.atk
//////            << msgData._Dyn.dyn.release
//////            << msgData._Dyn.dyn.offset1
//////            << msgData._Dyn.dyn.offset2
//////            << msgData._Dyn.dyn.th1
//////            << msgData._Dyn.dyn.th2
//////            << msgData._Dyn.dyn.ratio
//////            << msgData._Dyn.dyn.nRatio;

////    QByteArray packBuff;
////    packBuff.append((const char*)&msgData, sizeof(msgData._Dyn));
////    sendPacket(MSG_TYPE_DYN, packBuff);
//}

//void Device_Mic_Quad::sendDividerPack(quint8 ch, DividerData& hf, DividerData& lf)
//{
//////    if(ch == AMP_CH1){
//////        _configData.ch1_divider.hf = hf;
//////        _configData.ch1_divider.lf = lf;
//////    }
//////    else if(ch == AMP_CH2){
//////        _configData.ch2_divider.hf = hf;
//////        _configData.ch2_divider.lf = lf;
//////    }
//////    else Q_ASSERT(false);
////    _configData.CH[ch].divider.hf = hf;
////    _configData.CH[ch].divider.lf = lf;

////    MsgData_Mic_Quad msgData;
////    msgData._Divider.ch = ch + 1;
////    msgData._Divider.divider.hf = hf;
////    msgData._Divider.divider.lf = lf;

//////    qDebug() << msgData._Divider.ch
//////             << msgData._Divider.divider.hf.bypass
//////             << msgData._Divider.divider.hf.freq
//////             << msgData._Divider.divider.hf.gain
//////             << msgData._Divider.divider.hf.q
//////             << msgData._Divider.divider.lf.bypass
//////             << msgData._Divider.divider.lf.freq
//////             << msgData._Divider.divider.lf.gain
//////             << msgData._Divider.divider.lf.q;

////    QByteArray packBuff;
////    packBuff.append((const char*)&msgData._Divider, sizeof(msgData._Divider));
////    sendPacket(MSG_TYPE_DIVIDER, packBuff);
//}


//void Device_Mic_Quad::sendSave2DevicePack()
//{
//    qDebug() << "Device_Amp4_4::sendSave2DevicePack";
//    _saveState = DevSave_Save2Dev;
//    sendPacket_Nobody(Msg_Type_iMXRT1052_POE_Save);
//}

//void Device_Mic_Quad::sendUpdatePackage(quint32 totalCount, quint32 offset, quint32 paraLen, QByteArray buff)
//{
//    QByteArray packBuff;

//    packBuff.append((const char *)&totalCount, sizeof(quint32));
//    packBuff.append((const char *)&offset, sizeof(quint32));
//    packBuff.append((const char *)&paraLen, sizeof(quint32));
//    packBuff.append(buff);
//    sendPacket(MSG_TYPE_UPDATE_PACKAGE, packBuff);
//}

//void Device_Mic_Quad::sendVoltTypePack(quint8 voltType)
//{
////    _configData.volumeCurrentType = voltType;
////    MsgData_Mic_Quad msgData;
////    msgData.volumeCurrentType = voltType;
////    QByteArray packBuff;
////    packBuff.append((const char*)&msgData, sizeof(msgData.volumeCurrentType));
////    sendPacket(MSG_TYPE_VOLUME_CURRENT, packBuff);
//}

// 另存场景为文件
void Device_Mic_Quad::serialize(QString fileName)
{
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    QDataStream ds(&f);
    ds << _configData;
    f.close();
}

void Device_Mic_Quad::deserialize(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QDataStream ds(&file);

    ConfigData_Mic_Quad configData;
    ds >> configData;
//    initData(configData);

    file.close();
}

//void Device_Mic_Quad::initData(const ConfigData_Mic_Quad& configData)
//{
//    if (_factoryMode)
//    {
//        for (int i = 0; i < 2; ++i)
//        {
//            _configData.CH[i].gain = configData.CH[i].gain;
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
//        memcpy(&_configData, &configData, sizeof(ConfigData_Mic_Quad));
////        _configData.Switch = 1;
//    }
//}

//QString Device_Mic_Quad::getRouterInfo(quint8 ch)
//{
//    QString str;
////    switch(ch)
////    {
////    case Amp4_4Router::INPUT_SELECT_A1:
////        str = tr("Line#1");
////        break;
////    case Amp4_4Router::INPUT_SELECT_A2:
////        str = tr("Line#2");
////        break;
////    case Amp4_4Router::INPUT_SELECT_A3:
////        str = tr("Line#3");
////        break;
////    case Amp4_4Router::INPUT_SELECT_A4:
////        str = tr("Line#4");
////        break;
////    case Amp4_4Router::INPUT_SELECT_N1:
////        str = tr("Net#1");
////        break;
////    case Amp4_4Router::INPUT_SELECT_N2:
////        str = tr("Net#2");
////        break;
////    case Amp4_4Router::INPUT_SELECT_N3:
////        str = tr("Net#3");
////        break;
////    case Amp4_4Router::INPUT_SELECT_N4:
////        str = tr("Net#4");
////        break;
////    default:
////        Q_ASSERT(false);
////        break;
////    }
//    return str;
//}
