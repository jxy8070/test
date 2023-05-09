#include "stable.h"
#include "device_monitor_spk.h"
#include "protocal/packetbase.h"
#include "protocal/packet.h"
#include "device/devicemanager.h"
#include "deviceSetting/alarmconfig.h"
#include "Frame/mainwindow.h"


Device_Monitor_SPK::Device_Monitor_SPK(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _configData.initConfigData();
    _alarmCfg = new AlarmConfig(devName, mac);
    _factoryMode = false;
    _saveState = DevSave_None;
}

void Device_Monitor_SPK::processPacket(PacketBase* pack)
{
#ifdef QT_DEBUG
//    qDebug()<<"Device_Amp4_4 receive Pack:" <<pack->getMsgType();
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
        _configData = pack->getBodyMonitorSPK().InitData.getDataPack();
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
            MsgData_Moniter_SPK msgData = pack->getBodyMonitorSPK();
            memcpy(&_adcData, &msgData.ADC_Value, sizeof(ADC_Data));
            _alarmCfg->checkLimit(Dev_Type_AES67_MONITOR_SPK, _adcData);

            emit adcDataChanged(_alarmCfg);
        }
        break;
    }
    default:
        qDebug()<< QString("Unknown packet type%1").arg(pack->getMsgType());
        break;
    }
}


void Device_Monitor_SPK::sendAllConfigPack(bool isNeedDisSuccess)
{
    ConfigData_Monitor_SPK_Net msgData = _configData.getNetPack();

    if (isNeedDisSuccess)
    {
        _saveState = DevSave_LocalLoad;
    }

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(ConfigData_Monitor_SPK_Net));
    sendPacket(MSG_TYPE_INIT, packBuff);

    emit volumeChanged();
    emit muteChanged();
    emit phaseChanged();
    emit routerChanged();
}

void Device_Monitor_SPK::sendSwitch(bool flag)
{
//    _configData.Switch = flag;
//    sendCommonDataPack(1, _configData.ch1_ControlValue);
//    sendCommonDataPack(2, _configData.ch2_ControlValue);
}

void Device_Monitor_SPK::sendVolume(quint8 ch, qint8 vol)
{
    _configData.CH[ch].ControlValue.volume = vol;
    sendCommonDataPack(ch, _configData.CH[ch].ControlValue);
//    switch (ch)
//    {
//    case AMP_CH1:
//        _configData.ch1_ControlValue.volume = vol;
//        sendCommonDataPack(ch, _configData.ch1_ControlValue);
//        break;
//    case AMP_CH2:
//        _configData.ch2_ControlValue.volume = vol;
//        sendCommonDataPack(ch, _configData.ch2_ControlValue);
//        break;
//    default:
//        break;
//    }

    emit volumeChanged();
}

void Device_Monitor_SPK::sendMute(quint8 ch, bool flag)
{
    _configData.CH[ch].ControlValue.mute = flag;
    sendCommonDataPack(ch, _configData.CH[ch].ControlValue);
//    switch (ch)
//    {
//    case AMP_CH1:
//        _configData.ch1_ControlValue.mute = flag;
//        sendCommonDataPack(ch, _configData.ch1_ControlValue);
//        break;
//    case AMP_CH2:
//        _configData.ch2_ControlValue.mute = flag;
//        sendCommonDataPack(ch, _configData.ch2_ControlValue);
//        break;
//    default:
//        break;
//    }

    emit muteChanged();
}

void Device_Monitor_SPK::sendPhase(quint8 ch, bool flag)
{
    _configData.CH[ch].ControlValue.phase = flag;
    sendCommonDataPack(ch, _configData.CH[ch].ControlValue);
//    switch (ch)
//    {
//    case AMP_CH1:
//        _configData.ch1_ControlValue.phase = flag;
//        sendCommonDataPack(ch, _configData.ch1_ControlValue);
//        break;
//    case AMP_CH2:
//        _configData.ch2_ControlValue.phase = flag;
//        sendCommonDataPack(ch, _configData.ch2_ControlValue);
//        break;
//    default:
//        break;
//    }

    emit phaseChanged();
}

void Device_Monitor_SPK::sendRouter(quint8 ch, quint8 routerCh)
{
    _configData.CH[ch].ControlValue.channel = routerCh;
    sendCommonDataPack(ch, _configData.CH[ch].ControlValue);
//    switch (ch)
//    {
//    case AMP_CH1:
//        _configData.ch1_ControlValue.channel = routerCh;
//        sendCommonDataPack(ch, _configData.ch1_ControlValue);
//        break;
//    case AMP_CH2:
//        _configData.ch2_ControlValue.channel = routerCh;
//        sendCommonDataPack(ch, _configData.ch2_ControlValue);
//        break;
//    default:
//        break;
//    }

    emit routerChanged();
}

void Device_Monitor_SPK::sendCommonDataPack(quint8 ch, ControlValue vlu)
{
    MsgData_Moniter_SPK msgData;
    msgData.Other.power = 1;
    msgData.Other.ch    = ch + 1;
    memcpy(&msgData.Other.value, &vlu, sizeof(ControlValue));

    if (msgData.Other.power == 0x00)
    {
        msgData.Other.value.mute = 0x01;
    }
//    qDebug() <<msgData.Other.power
//             <<msgData.Other.ch
//             <<msgData.Other.value.gain
//             <<msgData.Other.value.channel
//             <<msgData.Other.value.delay
//             <<msgData.Other.value.mute
//             <<msgData.Other.value.phase
//             <<msgData.Other.value.volume;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData.Other, sizeof(CommonData_Amp));
    sendPacket(MSG_TYPE_OTHER, packBuff);
}

void Device_Monitor_SPK::sendEqPack(quint8 ch, bool bypass, EqData* eqPara)
{
    MsgData_Moniter_SPK msgData;
    msgData._EqData.ch = ch + 1;

    _configData.CH[ch].Eq.bypass = bypass;
    memcpy(&_configData.CH[ch].Eq, eqPara, 8*sizeof(EqData));
    msgData._EqData.Eq = _configData.CH[ch].Eq.getNetPack();

    QByteArray packBuff;
    packBuff.append((const char*)&msgData._EqData, sizeof(msgData._EqData));
    sendPacket(MSG_TYPE_EQ, packBuff);
}

void Device_Monitor_SPK::sendDynPack(quint8 ch, DynData& dynPara)
{
//    if(ch == AMP_CH1) memcpy(&_configData.ch1_Dyn, &dynPara, sizeof(DynData));
//    else if(ch == AMP_CH2)  memcpy(&_configData.ch2_Dyn, &dynPara, sizeof(DynData));
    memcpy(&_configData.CH[ch].Dyn, &dynPara, sizeof(DynData));

    MsgData_Moniter_SPK msgData;
    msgData._Dyn.ch = ch + 1;
    msgData._Dyn.dyn = dynPara;

//    qDebug()<< msgData._Dyn.ch
//            << msgData._Dyn.dyn.bypass
//            << msgData._Dyn.dyn.atk
//            << msgData._Dyn.dyn.release
//            << msgData._Dyn.dyn.offset1
//            << msgData._Dyn.dyn.offset2
//            << msgData._Dyn.dyn.th1
//            << msgData._Dyn.dyn.th2
//            << msgData._Dyn.dyn.ratio
//            << msgData._Dyn.dyn.nRatio;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._Dyn));
    sendPacket(MSG_TYPE_DYN, packBuff);
}

void Device_Monitor_SPK::sendDividerPack(quint8 ch, DividerData& hf, DividerData& lf)
{
//    if(ch == AMP_CH1){
//        _configData.ch1_divider.hf = hf;
//        _configData.ch1_divider.lf = lf;
//    }
//    else if(ch == AMP_CH2){
//        _configData.ch2_divider.hf = hf;
//        _configData.ch2_divider.lf = lf;
//    }
//    else Q_ASSERT(false);
    _configData.CH[ch].divider.hf = hf;
    _configData.CH[ch].divider.lf = lf;

    MsgData_Moniter_SPK msgData;
    msgData._Divider.ch = ch + 1;
    msgData._Divider.divider.hf = hf;
    msgData._Divider.divider.lf = lf;

//    qDebug() << msgData._Divider.ch
//             << msgData._Divider.divider.hf.bypass
//             << msgData._Divider.divider.hf.freq
//             << msgData._Divider.divider.hf.gain
//             << msgData._Divider.divider.hf.q
//             << msgData._Divider.divider.lf.bypass
//             << msgData._Divider.divider.lf.freq
//             << msgData._Divider.divider.lf.gain
//             << msgData._Divider.divider.lf.q;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData._Divider, sizeof(msgData._Divider));
    sendPacket(MSG_TYPE_DIVIDER, packBuff);
}


void Device_Monitor_SPK::sendSave2DevicePack()
{
    _saveState = DevSave_Save2Dev;
    sendPacket_Nobody(MSG_TYPE_SAVE);
}

void Device_Monitor_SPK::sendUpdatePackage(quint32 totalCount, quint32 offset, quint32 paraLen, QByteArray buff)
{
    QByteArray packBuff;

    packBuff.append((const char *)&totalCount, sizeof(quint32));
    packBuff.append((const char *)&offset, sizeof(quint32));
    packBuff.append((const char *)&paraLen, sizeof(quint32));
    packBuff.append(buff);
    sendPacket(MSG_TYPE_UPDATE_PACKAGE, packBuff);
}

void Device_Monitor_SPK::sendVoltTypePack(quint8 voltType)
{
//    _configData.volumeCurrentType = voltType;
//    MsgData_RT1052_Poe msgData;
//    msgData.volumeCurrentType = voltType;
//    QByteArray packBuff;
//    packBuff.append((const char*)&msgData, sizeof(msgData.volumeCurrentType));
//    sendPacket(MSG_TYPE_VOLUME_CURRENT, packBuff);
}

// 另存场景为文件
void Device_Monitor_SPK::serialize(QString fileName)
{
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    QDataStream ds(&f);
    ds << _configData;
    f.close();
}

void Device_Monitor_SPK::deserialize(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QDataStream ds(&file);

    ConfigData_Monitor_SPK configData;
    ds >> configData;
    initData(configData);

    file.close();
}

void Device_Monitor_SPK::initData(const ConfigData_Monitor_SPK& configData)
{
    if (_factoryMode)
    {
        for (int i = 0; i < 4; ++i)
        {
            _configData.CH[i].ControlValue = configData.CH[i].ControlValue;
            _configData.CH[i].Eq           = configData.CH[i].Eq;
            _configData.CH[i].divider      = configData.CH[i].divider;
        }
    }
    else
    {
//        quint8 swt = _configData.Switch;
        memcpy(&_configData, &configData, sizeof(ConfigData_Monitor_SPK));
//        _configData.Switch = 1;
    }
}

QString Device_Monitor_SPK::getRouterInfo(quint8 ch)
{
    QString str;
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
    return str;
}
