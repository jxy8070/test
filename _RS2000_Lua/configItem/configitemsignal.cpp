#include "configitemsignal.h"
#include "configSetting/cfgsettingsignal.h"

ConfigItemSignal::ConfigItemSignal(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    int inNum = 0, outNum =1;
    memset(&_body, 0, sizeof(IG_PARACTL_SIGGEN));

#if Logo_FangTu /* Function */
    addSubType(cfgType);
#else
    addSubType(CFG_TYPE_SIG_SINE);
    addSubType(CFG_TYPE_SIG_NOISE);
    addSubType(CFG_TYPE_SIG_PINKNOISE);
#endif
    initPort(inNum, outNum);    
    initData();

    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "Freq", 0x00070001, 0x00070002));
    pinList.append(new CLuaPinParam_PC(this, name, "Gain", 0x00070003, 0x00070004));
    pinList.append(new CLuaPinParam_PC(this, name, "Mute", 0x00070005, 0x00070006));
    initControlPins(pinList);

    _setting = NULL;
}

ConfigItemSignal::~ConfigItemSignal()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

void ConfigItemSignal::initData()
{
    _mute = true;
    _freq = 1000.0;
    _level = -100.0;
}

QWidget *ConfigItemSignal::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingSignal(this);
    }
    return _setting;
}



QByteArray ConfigItemSignal::getDetailData()
{
    QByteArray paraBuff;
    _body.freq_hz = _freq;
    _body.gain = (_level + 100) * 100;
    _body.mute = _mute;

    paraBuff.append((const char*)&_body, sizeof(IG_PARACTL_SIGGEN));

    return paraBuff;
}


void ConfigItemSignal::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_SIGGEN));
    _freq  = _body.freq_hz;
    _level = _body.gain / 100.0f - 100;
    _mute  = _body.mute;

    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemSignal::setRtnData(unsigned int *d)
{

}

void ConfigItemSignal::updateCfgSettingLoadedData()
{
    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemSignal::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SIG_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    setMute(ConfigIni->value(QString(CONFIG_DEVICE_DEV_SIG_MUTE)).toBool());
    setLevel(ConfigIni->value(QString(CONFIG_DEVICE_DEV_SIG_LEVEL)).toDouble());
    setFreq(ConfigIni->value(QString(CONFIG_DEVICE_DEV_SIG_FREQ)).toDouble());
    ConfigIni->endGroup();
}


void ConfigItemSignal::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SIG_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SIG_MUTE), getMute());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SIG_LEVEL), getLevel());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SIG_FREQ), getFreq());
    ConfigIni->endGroup();
}

QVariant ConfigItemSignal::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    switch(param->SetCmdKey())
    {
    case 0x00070002:
        setFreq(val.toDouble());
        return QVariant(val.toDouble());
    case 0x00070004:
        setLevel(val.toDouble());
        return QVariant(val.toDouble());
    case 0x00070006:
        setMute(val.toBool());
        return QVariant(val.toBool());
    default:
        return QVariant();
    }
}

void ConfigItemSignal::setMute(bool flag)
{
    _mute = flag;
    saveParamToDevice();
}

void ConfigItemSignal::setLevel(double value)
{
    _level = value;
    saveParamToDevice();
}

void ConfigItemSignal::setFreq(double value)
{
    _freq = value;
    saveParamToDevice();
}

