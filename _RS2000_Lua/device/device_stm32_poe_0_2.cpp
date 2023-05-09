#include "stable.h"
#include "device_stm32_poe_0_2.h"
#include "protocal/packetbase.h"
#include "protocal/packet.h"
#include "device/devicemanager.h"
#include "deviceSetting/alarmconfig.h"
#include "Frame/mainwindow.h"


Device_Amp::Device_Amp(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _alarmCfg = new AlarmConfig(devName, mac);
    _configData.initConfigData();
    _factoryMode = true;
    _saveState = DevSave_None;
}


void Device_Amp::processPacket(PacketBase* pack)
{
    switch(pack->getMsgType()) {
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
        _configData = pack->getBody_STM32_POE().InitData_0_2.getDataPack();
        switchStatus(Dev_Online);
        break;
    case MSG_TYPE_ADC_VALUE:
    {
        if (!isAddedToProject()) {
            MsgData_Amp msgData = pack->getBodyAmp();
            memcpy(&_adcData, &msgData.ADC_Value, sizeof(ADC_Data));
            _alarmCfg->checkLimit(Dev_Type_STM32_POE_AES67_SPK_0_2, _adcData);

            emit adcDataChanged(_alarmCfg);
        }
        break;
    }
    default:
        qDebug()<< QString("Unknown packet type%1").arg(pack->getMsgType());
        break;
    }
}

void Device_Amp::sendAllConfigPack(bool isNeedDisSuccess)
{
//    ConfigData_Amp_Net msgData = _configData.getNetPack();

//    if (isNeedDisSuccess)
//    {
//        _saveState = DevSave_LocalLoad;
//    }

//    QByteArray packBuff;
//    packBuff.append((const char*)&msgData, sizeof(ConfigData_Amp_Net));
//    sendPacket(MSG_TYPE_INIT, packBuff);

//    emit volumeChanged();
//    emit muteChanged();
//    emit phaseChanged();
//    emit routerChanged();
}

void Device_Amp::sendSwitch(bool flag)
{
//    _configData.Switch = flag;
//    sendCommonDataPack(1, _configData.ch1_ControlValue);
//    sendCommonDataPack(2, _configData.ch2_ControlValue);
}

void Device_Amp::sendVolume(quint8 ch, qint8 vol)
{
    MsgData_STM32_POE msgData;

    _configData.AD_gain[ch].vol = vol;

    msgData._online.ch = ch;
    memcpy(&msgData._online._u.gain, &_configData.AD_gain[ch], sizeof(struct ConfigData_STM32_Gain));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_STM32_POE_AD_Gain, packBuff);
}

void Device_Amp::sendMute(quint8 ch, bool flag)
{
    MsgData_STM32_POE msgData;

    _configData.AD_gain[ch].mute = flag;

    msgData._online.ch = ch;
    memcpy(&msgData._online._u.gain, &_configData.AD_gain[ch], sizeof(struct ConfigData_STM32_Gain));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_STM32_POE_AD_Gain, packBuff);
}

void Device_Amp::sendPhase(quint8 ch, bool flag)
{
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
//    emit phaseChanged();
}

void Device_Amp::sendRouter(quint8 ch, quint8 routerCh)
{
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

//    emit routerChanged();
}

void Device_Amp::sendCommonDataPack(quint8 ch, ControlValue vlu)
{
//    MsgData_Amp msgData;
//    msgData.Other.power = _configData.Switch;
//    msgData.Other.ch    = ch;
//    memcpy(&msgData.Other.value, &vlu, sizeof(ControlValue));

//    if (msgData.Other.power == 0x00)
//    {
//        msgData.Other.value.mute = 0x01;
//    }

//    QByteArray packBuff;
//    packBuff.append((const char*)&msgData.Other, sizeof(CommonData_Amp));
//    sendPacket(Msg_Type_STM32_POE_Other, packBuff);
}

void Device_Amp::sendEqPack(quint8 ch, bool bypass, EqData* eqPara)
{
    MsgData_STM32_POE msgData;

    _configData.AD_Eq[ch].bypass = bypass;
    memcpy(&_configData.AD_Eq[ch].eq, eqPara, 4*sizeof(EqData));

    msgData._online.ch = ch;
    msgData._online._u.eq = _configData.AD_Eq[ch].getNetPack();

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_STM32_POE_AD_Eq, packBuff);
}

void Device_Amp::sendDynPack(quint8 ch, DynData& dynPara)
{
    memcpy(&_configData.AD_Dyn, &dynPara, sizeof(DynData));

    MsgData_STM32_POE msgData;
    msgData._online.ch = ch;
    memcpy(&msgData._online._u.dyn, &_configData.AD_Dyn, sizeof(DynData));

    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData._online));
    sendPacket(Msg_Type_STM32_POE_AD_Dyn, packBuff);
}

void Device_Amp::sendDividerPack(quint8 ch, DividerData& hf, DividerData& lf)
{

}


void Device_Amp::sendSave2DevicePack()
{
    _saveState = DevSave_Save2Dev;
    sendPacket_Nobody(Msg_Type_STM32_POE_Save);
}

void Device_Amp::sendUpdatePackage(quint32 totalCount, quint32 offset, quint32 paraLen, QByteArray buff)
{
//    QByteArray packBuff;

//    packBuff.append((const char *)&totalCount, sizeof(quint32));
//    packBuff.append((const char *)&offset, sizeof(quint32));
//    packBuff.append((const char *)&paraLen, sizeof(quint32));
//    packBuff.append(buff);
//    sendPacket(MSG_TYPE_UPDATE_PACKAGE, packBuff);
}

void Device_Amp::sendVoltTypePack(quint8 voltType)
{
//    _configData.volumeCurrentType = voltType;
//    MsgData_Amp msgData;
//    msgData.volumeCurrentType = voltType;
//    QByteArray packBuff;
//    packBuff.append((const char*)&msgData, sizeof(msgData.volumeCurrentType));
//    sendPacket(MSG_TYPE_VOLUME_CURRENT, packBuff);
}

// 另存场景为文件
void Device_Amp::serialize(QString fileName)
{
//    QFile f(fileName);
//    f.open(QIODevice::WriteOnly);
//    QDataStream ds(&f);
//    ds << _configData;
//    f.close();
}

void Device_Amp::deserialize(QString fileName)
{
//    QFile file(fileName);
//    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
//    QDataStream ds(&file);

//    ConfigData_Amp configData;
//    ds >> configData;
//    initData(configData);

//    file.close();
}

void Device_Amp::initData(const ConfigData_STM32_POE_0_2& configData)
{
//    if (_factoryMode)
//    {
//        _configData.ch1_ControlValue = configData.ch1_ControlValue;
//        _configData.ch1_Eq           = configData.ch1_Eq;
//        _configData.ch1_divider      = configData.ch1_divider;
//        _configData.ch1_OutputGain   = configData.ch1_OutputGain;

//        _configData.ch2_ControlValue = configData.ch2_ControlValue;
//        _configData.ch2_Eq           = configData.ch2_Eq;
//        _configData.ch2_divider      = configData.ch2_divider;
//        _configData.ch2_OutputGain   = configData.ch2_OutputGain;
//    }
//    else
//    {
//        quint8 swt = _configData.Switch;
//        memcpy(&_configData, &configData, sizeof(ConfigData_Amp));
//        _configData.Switch = swt;
//    }
}

QString Device_Amp::getRouterInfo(quint8 ch)
{
    QString str;
    switch(ch)
    {
    case 0x00:
        str = tr("Line#1");
        break;
    case 0x01:
        str = tr("Line#2");
        break;
    case 0x02:
        str = tr("Line#1#2");
        break;
    case 0x03:
        str = tr("Net#1");
        break;
    case 0x04:
        str = tr("Net#2");
        break;
    case 0x05:
        str = tr("Net#1#2");
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    return str;
}
