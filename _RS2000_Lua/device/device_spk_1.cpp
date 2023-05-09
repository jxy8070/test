#include "stable.h"
#include "device_spk_1.h"
#include "protocal/packetbase.h"
#include "protocal/packet.h"
#include "device/devicemanager.h"
#include "deviceSetting/alarmconfig.h"

Device_Spk_1::Device_Spk_1(const DanteDevice *danteDev)
    :DanteDevice(danteDev)
{
    _type = DEV_TYPE_DNTE_SPK_1;
    _configData.initConfigData();
    _alarmCfg = new AlarmConfig(danteDev->customName(), danteDev->getMac());
    _factoryMode = true;
    _saveState = DevSave_None;
}

Device_Spk_1::Device_Spk_1(QString mac, DevType devType, QString customName, QString serial)
    :DanteDevice(mac, devType, customName, serial)
{
    _type = DEV_TYPE_DNTE_SPK_1;
    _configData.initConfigData();
    _alarmCfg = new AlarmConfig(customName, mac);
    _factoryMode = true;
    _saveState = DevSave_None;
}

void Device_Spk_1::saveAlarmSetting()
{
    _alarmCfg->saveAlarmSetting();
}

void Device_Spk_1::loadAlarmSetting()
{
    _alarmCfg->loadAlarmSetting();
}

void Device_Spk_1::clearAlarmSetting()
{
    _alarmCfg->clearAlarmSetting();
}

void Device_Spk_1::processPacket(PacketBase* pack)
{
#ifdef QT_DEBUG
//    if(pack->getMsgType() != 126)
//        qDebug()<<"Device_Spk_1 receive Pack:" <<pack->getMsgType();
#endif
    switch(pack->getMsgType())
    {
    case MSG_TYPE_DEV_ISALIVING: //设备在线
        break;
    case MSG_TYPE_HEARTBEAT:
        switch (_saveState)
        {
        case DevSave_Save2Dev:
        {
            QMessageBox *box = new QMessageBox(QMessageBox::NoIcon, "info", tr("Save Success!"), QMessageBox::Ok);
            QThread thread;
            box->moveToThread(&thread);
            box->show();
        }
            break;
        case DevSave_LocalLoad:
        {
            QMessageBox *box = new QMessageBox(QMessageBox::NoIcon, "info", tr("Load Success!"), QMessageBox::Ok);
            QThread thread;
            box->moveToThread(&thread);
            box->show();
        }
            break;
        default:
            break;
        }
        _saveState = DevSave_None;
        break;
    case MSG_TYPE_INIT:
        _configData = pack->getBodySpk().InitData.getDataPack();

        if (!customName().isEmpty())
        {
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
        if (!customName().isEmpty())
        {
            MsgData_Spk msgData = pack->getBodySpk();
            memcpy(&_adcData, &msgData.ADC_Value, sizeof(ADC_Data));
            _alarmCfg->checkLimit(DEV_TYPE_DNTE_SPK_1, _adcData);

            emit adcDataChanged(_alarmCfg);
        }
        break;
    }
    default:
        qDebug()<< QString("Unknown packet type%1").arg(pack->getMsgType());
        break;
    }
}


void Device_Spk_1::sendInitPack()
{
    sendPacket_Nobody(MSG_TYPE_DEV_INIT);
}

void Device_Spk_1::sendHeartbeatPack()
{
    sendPacket_Nobody(MSG_TYPE_HEARTBEAT);
}

void Device_Spk_1::sendAllConfigPack(bool isNeedDisSuccess)
{
    MsgData_Spk cmmData;
    cmmData.InitData = _configData.getNetPack();

    if (isNeedDisSuccess)
    {
        _saveState = DevSave_LocalLoad;
    }

    QByteArray packBuff;
    packBuff.append((const char*)&cmmData, sizeof(ConfigData_Spk_Net));
    sendPacket(MSG_TYPE_INIT, packBuff);

    emit volumeChanged();
    emit muteChanged();
    emit phaseChanged();
    emit routerChanged();
}

void Device_Spk_1::sendVoltTypePack(quint8 voltType)
{
    _configData.volumeCurrentType = voltType;
    MsgData_Spk msgData;
    msgData.volumeCurrentType = voltType;
    QByteArray packBuff;
    packBuff.append((const char*)&msgData, sizeof(msgData.volumeCurrentType));
    sendPacket(MSG_TYPE_VOLUME_CURRENT, packBuff);
}

void Device_Spk_1::sendSwitch(bool flag)
{
    _configData.Switch = flag;
    sendCommonDataPack();
}

void Device_Spk_1::sendVolume(qint8 vol)
{
    _configData.ch1_ControlValue.volume = vol;
    sendCommonDataPack();

    emit volumeChanged();
}

void Device_Spk_1::sendMute(bool flag)
{
    _configData.ch1_ControlValue.mute = flag;
    sendCommonDataPack();

    emit muteChanged();
}

void Device_Spk_1::sendPhase(bool flag)
{
    _configData.ch1_ControlValue.phase = flag;
    sendCommonDataPack();

    emit phaseChanged();
}

void Device_Spk_1::sendRouter(quint8 routerCh)
{
    _configData.ch1_ControlValue.channel = routerCh;
    sendCommonDataPack();

    emit routerChanged();
}

void Device_Spk_1::sendCommonDataPack()
{
    MsgData_Spk cmmData;
    cmmData.Other.ch = 0x00;
    cmmData.Other.power = _configData.Switch;
    memcpy(&cmmData.Other.value, &_configData.ch1_ControlValue, sizeof(ControlValue));

    if (cmmData.Other.power == 0)
    {
        cmmData.Other.value.mute = 0x01;
    }

//    qDebug() <<cmmData.Other.power
//             <<cmmData.Other.ch
//             <<cmmData.Other.value.gain
//             <<cmmData.Other.value.channel
//             <<cmmData.Other.value.delay
//             <<cmmData.Other.value.mute
//             <<cmmData.Other.value.phase
//             <<cmmData.Other.value.volume;

    QByteArray packBuff;
    packBuff.append((const char*)&cmmData, sizeof(CommonData_Spk));
    sendPacket(MSG_TYPE_OTHER, packBuff);
}


//发送控制信号
void Device_Spk_1::sendEqPack(quint8 ch, bool bypass, EqData* eqPara)
{
    Q_ASSERT(ch == 1);
    _configData.ch1_Eq.bypass = bypass;
    memcpy(&_configData.ch1_Eq.eq, eqPara, 4*sizeof(EqData));
    MsgData_Spk msgData;
    msgData._EqData.ch = ch;
    msgData._EqData.Eq = _configData.ch1_Eq.getNetPack();

//    qDebug()<< msgData._EqData.ch
//            << msgData._EqData.Eq.bypass
//            << msgData._EqData.Eq.eq[0].freq
//            << msgData._EqData.Eq.eq[0].gain
//            << msgData._EqData.Eq.eq[0].q
//            << msgData._EqData.Eq.eq[1].freq
//            << msgData._EqData.Eq.eq[1].gain
//            << msgData._EqData.Eq.eq[1].q
//            << msgData._EqData.Eq.eq[2].freq
//            << msgData._EqData.Eq.eq[2].gain
//            << msgData._EqData.Eq.eq[2].q
//            << msgData._EqData.Eq.eq[3].freq
//            << msgData._EqData.Eq.eq[3].gain
//            << msgData._EqData.Eq.eq[3].q;

    QByteArray packBuff;
    packBuff.append((const char*)&msgData._EqData, sizeof(msgData._EqData));
    sendPacket(MSG_TYPE_EQ, packBuff);
}


void Device_Spk_1::sendDynPack(quint8 ch, DynData& dynPara)
{
    Q_ASSERT(ch == 1);
    memcpy(&_configData.ch1_Dyn, &dynPara, sizeof(DynData));
    MsgData_Spk msgData;
    msgData._Dyn.ch = ch;
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
    packBuff.append((const char*)&msgData._Dyn, sizeof(msgData._Dyn));
    sendPacket(MSG_TYPE_DYN, packBuff);
}


void Device_Spk_1::sendDividerPack(quint8 ch, DividerData& hf, DividerData& lf )
{
    Q_ASSERT(ch == 1);
    _configData.ch1_divider.hf = hf;
    _configData.ch1_divider.lf = lf;

    MsgData_Spk msgData;
    msgData._Divider.ch = ch;
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

void Device_Spk_1::sendSave2DevicePack()
{
    _saveState = DevSave_Save2Dev;
    sendPacket_Nobody(MSG_TYPE_SAVE);
}

void Device_Spk_1::sendUpdatePackage(quint32 totalCount, quint32 offset, quint32 paraLen, QByteArray buff)
{
    QByteArray packBuff;
    packBuff.append((const char *)&totalCount, sizeof(quint32));
    packBuff.append((const char *)&offset, sizeof(quint32));
    packBuff.append((const char *)&paraLen, sizeof(quint32));
    packBuff.append(buff);
    sendPacket(MSG_TYPE_UPDATE_PACKAGE, packBuff);
}

// 另存场景为文件
void Device_Spk_1::serialize(QString fileName)
{
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    QDataStream ds(&f);

    ds << _configData;

    f.close();
}

void Device_Spk_1::deserialize(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QDataStream ds(&file);
    ConfigData_Spk configData;

    ds >> configData;
    initData(configData);

    file.close();
}

void Device_Spk_1::initData(const ConfigData_Spk& configData)
{
    if (_factoryMode)
    {
        _configData.ch1_ControlValue.channel  = configData.ch1_ControlValue.channel;
        _configData.ch1_ControlValue.mute     = configData.ch1_ControlValue.mute;
        _configData.ch1_ControlValue.phase    = configData.ch1_ControlValue.phase;
        _configData.ch1_ControlValue.volume   = configData.ch1_ControlValue.volume;
    }
    else
    {
        quint8 swt = _configData.Switch;
        memcpy(&_configData, &configData, sizeof(ConfigData_Spk));
        _configData.Switch = swt;
    }
}

QString Device_Spk_1::getRouterInfo()
{
    QString str;
    switch(_configData.ch1_ControlValue.channel)
    {
    case 0x01:
        str = tr("Line");
        break;
    case 0x02:
        str = tr("Net#1");
        break;
    case 0x03:
        str = tr("Net#2");
        break;
    case 0x04:
        str = tr("Net#1#2");
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    return str;
}

