#include "configitemsoftaec.h"
#include "configSetting/cfgsettingsoftaec.h"
#include "device/device_trn.h"

ConfigItemSoftAEC::ConfigItemSoftAEC(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_body, 0, sizeof(IG_PARACTL_SOFTAEC));

    initPort(2, 2);
    _inPortList[0]->setPortName("micIn");
    _inPortList[1]->setPortName("farIn");
    _outPortList[0]->setPortName("localOut");
    _outPortList[1]->setPortName("farOut");
    _body.micToSpeakGain = (0 + 100) * 100;
    _body.micToSpeakSwitch = 0;
    _body.refGain = (0 + 100) * 100;
    _body.RESStrength = 10;
    _body.RESSwitch = 1;
    _body.NRGain = (0 + 100) * 100;
    _body.CNGain = (-80 + 100) * 100;

    if((cfgType&FCT_FAMILY_MASK) == 0X1600)
    {
        if(_trnDev->getPlatformType() == Platform_DSP){
            addSubType(CFG_TYPE_SOFTAEC_8K64);
            addSubType(CFG_TYPE_SOFTAEC_8K96);
            addSubType(CFG_TYPE_SOFTAEC_8K128);
            addSubType(CFG_TYPE_SOFTAEC_16K64);
        }
        else{
            addSubType(CFG_TYPE_SOFTAEC_8K64);
            addSubType(CFG_TYPE_SOFTAEC_8K96);
            addSubType(CFG_TYPE_SOFTAEC_8K128);
            addSubType(CFG_TYPE_SOFTAEC_8K160);
            addSubType(CFG_TYPE_SOFTAEC_8K192);
            addSubType(CFG_TYPE_SOFTAEC_8K224);
            addSubType(CFG_TYPE_SOFTAEC_8K256);
            addSubType(CFG_TYPE_SOFTAEC_16K64);
            addSubType(CFG_TYPE_SOFTAEC_16K96);
            addSubType(CFG_TYPE_SOFTAEC_16K128);
            addSubType(CFG_TYPE_SOFTAEC_16K160);
            addSubType(CFG_TYPE_SOFTAEC_16K192);
            addSubType(CFG_TYPE_SOFTAEC_16K224);
            addSubType(CFG_TYPE_SOFTAEC_16K256);
        }
    }

    //控制针脚初始化
//    QList<CLuaPinParam_PC*> pinList;
//    pinList.append(new CLuaPinParam_PC(this, name, "MicInGain", 0x000C0001, 0x000C0002));
//    pinList.append(new CLuaPinParam_PC(this, name, "LineInGain", 0x000C0003, 0x000C0004));
//    pinList.append(new CLuaPinParam_PC(this, name, "SpeakOutGain", 0x000C0005, 0x000C0006));
//    pinList.append(new CLuaPinParam_PC(this, name, "LineOutGain", 0x000C0007,  0x000C0008));
//    pinList.append(new CLuaPinParam_PC(this, name, "MicToSpeakGain", 0x000C0009,  0x000C000A));
//    pinList.append(new CLuaPinParam_PC(this, name, "MicToSpeakGainOn", 0x000C000B,  0x000C000C));
//    pinList.append(new CLuaPinParam_PC(this, name, "NRGain", 0x000C000D,  0x000C000E));
//    pinList.append(new CLuaPinParam_PC(this, name, "NRGainOn", 0x000C000F,  0x000C0010));
//    initControlPins(pinList);
}

ConfigItemSoftAEC::~ConfigItemSoftAEC()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemSoftAEC::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingSoftAEC(this);
    }
    return _setting;
}


QByteArray ConfigItemSoftAEC::getDetailData()
{
    QByteArray paraBuff;
    paraBuff.append((const char *)&_body, sizeof(IG_PARACTL_SOFTAEC));
//    qDebug() << __FUNCTION__ << __LINE__
//             << _body.NRGain << _body.NRSwitch;
    return paraBuff;
}

void ConfigItemSoftAEC::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_SOFTAEC));
    if(_setting != NULL)
    {

    }
}

void ConfigItemSoftAEC::setRtnData(unsigned int *d)
{
    IG_PARAGET_SOFTAEC rtnData;
    memcpy(&rtnData, d, sizeof(IG_PARAGET_SOFTAEC));
    if(_setting)
    {
        double vuERLE = abs(rtnData.ERLE * 0.01 - 100);
        double vuRMLR = rtnData.RMLR * 0.01 - 100;
        _setting->updateRtnData(vuERLE, vuRMLR);
    }
}

void ConfigItemSoftAEC::updateCfgSettingLoadedData()
{
    if(_setting)
    {
        _setting->updateUiData();
    }
}

void ConfigItemSoftAEC::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_AEC_8K128_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    setBypass(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_BYPASS)).toBool());
    setRefGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_REFGAIN)).toDouble());
    setMicToSpeakGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_MICTOSPEAK)).toDouble());
    setMicToSpeakSwitch(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_MICENABLE)).toBool());
    setRESStrength(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_RESPERCENT)).toInt());
    setRESSwitch(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_RESENABLE)).toBool());
    setNRGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_NRGAIN)).toDouble());
    setNRSwitch(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_NRENABLE)).toBool());
    setCNGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_CNGAIN)).toDouble());
    setCNSwitch(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_CNENABLE)).toBool());
    ConfigIni->endGroup();
}

void ConfigItemSoftAEC::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_AEC_8K128_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_BYPASS), getBypass());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_REFGAIN), getRefGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_MICTOSPEAK), getMicToSpeakGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_MICENABLE), getMicToSpeakSwitch());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_RESPERCENT), getRESStrength());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_RESENABLE), getRESSwitch());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_NRGAIN), getNRGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_NRENABLE), getNRSwitch());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_CNGAIN), getCNGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_CNENABLE), getCNSwitch());
    ConfigIni->endGroup();
}

QVariant ConfigItemSoftAEC::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    return QVariant();
}
