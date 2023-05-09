#include "configitemaec.h"
#include "configSetting/cfgsettingaec.h"

ConfigItemAEC::ConfigItemAEC(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_body, 0, sizeof(IG_PARACTL_HARDAEC));

    initPort(2, 2);
    addSubType(CFG_TYPE_AEC_2_2);
    _body.micInGain = (0 + 100) * 100;
    _body.lineInGain = (0 + 100) * 100;
    _body.speakOutGain = (0 + 100) * 100;
    _body.lineOutGain = (0 + 100) * 100;
    _body.micToSpeakGain = (0 + 100) * 100;
    _body.micToSpeakSwitch = 1;
#ifndef Logo_BeiJingHuiSheng
    _body.NRGain = (6 + 100) * 100;
    _body.NRSwitch = 0;
#endif

    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "MicInGain", 0x000C0001, 0x000C0002));
    pinList.append(new CLuaPinParam_PC(this, name, "LineInGain", 0x000C0003, 0x000C0004));
    pinList.append(new CLuaPinParam_PC(this, name, "SpeakOutGain", 0x000C0005, 0x000C0006));
    pinList.append(new CLuaPinParam_PC(this, name, "LineOutGain", 0x000C0007,  0x000C0008));
    pinList.append(new CLuaPinParam_PC(this, name, "MicToSpeakGain", 0x000C0009,  0x000C000A));
    pinList.append(new CLuaPinParam_PC(this, name, "MicToSpeakGainOn", 0x000C000B,  0x000C000C));
    pinList.append(new CLuaPinParam_PC(this, name, "NRGain", 0x000C000D,  0x000C000E));
    pinList.append(new CLuaPinParam_PC(this, name, "NRGainOn", 0x000C000F,  0x000C0010));
    initControlPins(pinList);
    _setting = NULL;
}

ConfigItemAEC::~ConfigItemAEC()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemAEC::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingAEC(this);
    }
    return _setting;
}


QByteArray ConfigItemAEC::getDetailData()
{
    QByteArray paraBuff;
    paraBuff.append((const char *)&_body, sizeof(IG_PARACTL_HARDAEC));
//    qDebug() << __FUNCTION__ << __LINE__
//             << _body.NRGain << _body.NRSwitch;
    return paraBuff;
}

void ConfigItemAEC::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_HARDAEC));

    if(_setting != NULL)
    {

    }
}

void ConfigItemAEC::setRtnData(unsigned int *d)
{

}

void ConfigItemAEC::updateCfgSettingLoadedData()
{
    if(_setting)
    {
        _setting->updateUiData();
    }
}

void ConfigItemAEC::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_AEC_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    setMicInGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_MICINGAIN)).toInt());
    setLineInGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_LINEINGAIN)).toInt());
    setSpeakOutGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_SPEAKOUTGAIN)).toInt());
    setLineOutGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_LINEOUTGAIN)).toInt());
    setSpeak2LineGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_SPEAK2LINEGAIN)).toInt());
    setSpeak2LineSwitch(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_SPEAK2LINESWITCH)).toInt());
    setNRGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_NRGAIN)).toInt());
    ConfigIni->endGroup();
}

void ConfigItemAEC::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_AEC_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_MICINGAIN), getMicInGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_LINEINGAIN), getLineInGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_SPEAKOUTGAIN), getSpeakOutGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_LINEOUTGAIN), getLineOutGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_SPEAK2LINEGAIN), getSpeak2LineGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_SPEAK2LINESWITCH), getSpeak2LineSwitch());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_NRGAIN), getNRGain());
    ConfigIni->endGroup();
}

QVariant ConfigItemAEC::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    switch(param->SetCmdKey())
    {
    case 0x000C0002:
        setMicInGain(val.toDouble());
        return QVariant(val.toDouble());
    case 0x000C0004:
        setLineInGain(val.toDouble());
        return QVariant(val.toDouble());
    case 0x000C0006:
        setSpeakOutGain(val.toDouble());
        return QVariant(val.toBool());
    case 0x000C0008:
        setLineOutGain(val.toDouble());
        return QVariant(val.toDouble());
    case 0x000C000A:
        setSpeak2LineGain(val.toDouble());
        return QVariant(val.toDouble());
    case 0x000C000C:
        setSpeak2LineSwitch(val.toBool());
        return QVariant(val.toBool());
    case 0x000C000E:
        setNRGain(val.toInt());
        return QVariant(val.toInt());
    case 0x000C0010:
        setNRGainSwitch(val.toBool());
        return QVariant(val.toBool());

    default:
        return QVariant();
    }
}

void ConfigItemAEC::setMicInGain(int value)
{
    _body.micInGain = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemAEC::setLineInGain(int value)
{
    _body.lineInGain = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemAEC::setSpeakOutGain(int value)
{
    _body.speakOutGain = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemAEC::setLineOutGain(int value)
{
    _body.lineOutGain = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemAEC::setSpeak2LineGain(int value)
{
    _body.micToSpeakGain = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemAEC::setSpeak2LineSwitch(int value)
{
    _body.micToSpeakSwitch = value;
    saveParamToDevice();
}

void ConfigItemAEC::setNRGain(int value)
{
#ifndef Logo_BeiJingHuiSheng
    _body.NRGain = (value + 100) * 100;
    saveParamToDevice();
#endif
}

void ConfigItemAEC::setNRGainSwitch(bool enable)
{
#ifndef Logo_BeiJingHuiSheng
    _body.NRSwitch = enable;
    saveParamToDevice();
#endif
}
