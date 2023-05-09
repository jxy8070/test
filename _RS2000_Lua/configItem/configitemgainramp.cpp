#include "configitemgainramp.h"
#include "configSetting/cfgsettinggainramp.h"

ConfigItemGainRamp::ConfigItemGainRamp(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
   :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_body,      0, sizeof(IG_PARACTL_GAINRAMP));

    addSubType(CFG_TYPE_GAINRAMP_1);
    addSubType(CFG_TYPE_GAINRAMP_2);
    addSubType(CFG_TYPE_GAINRAMP_4);
    addSubType(CFG_TYPE_GAINRAMP_8);
    addSubType(CFG_TYPE_GAINRAMP_16);
    addSubType(CFG_TYPE_GAINRAMP_32);
    switch(cfgType) {
    case CFG_TYPE_GAINRAMP_1:
        initPort(1, 1);
        break;
    case CFG_TYPE_GAINRAMP_2:
        initPort(2, 2);
        break;
    case CFG_TYPE_GAINRAMP_4:
        initPort(4, 4);
        break;
    case CFG_TYPE_GAINRAMP_8:
        initPort(8, 8);
        break;
    case CFG_TYPE_GAINRAMP_16:
        initPort(16, 16);
        break;
    case CFG_TYPE_GAINRAMP_32:
        initPort(32, 32);
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    _body.effectGain  = 1;
    _body.targetAT_ms = 1000;
    _body.targetBT_ms = 1000;
    _body.unmuteT_ms  = 1000;
    _body.targetGainA = (0 + 100) * 100;
    _body.targetGainB = (0 + 100) * 100;

    _setting = NULL;
}

ConfigItemGainRamp::~ConfigItemGainRamp()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemGainRamp::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingGainRamp(this);
    }
    return _setting;
}

QByteArray ConfigItemGainRamp::getDetailData()
{
    QByteArray paraBuff;

    paraBuff.append((const char*)&_body, sizeof(IG_PARACTL_GAINRAMP));

    return paraBuff;
}

void ConfigItemGainRamp::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_GAINRAMP));
    if(_setting != NULL) {
        _setting->updateUiData();
    }
}

void ConfigItemGainRamp::setRtnData(unsigned int *d)
{
    if (_setting) {
        _setting->DispRtnData(d);
    }
}

void ConfigItemGainRamp::updateCfgSettingLoadedData()
{
    if(_setting != NULL) {
        _setting->updateUiData();
    }
}

void ConfigItemGainRamp::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_GAINRAMP_GROUP));
    set_targetTimeA(ConfigIni->value(QString(CONFIG_DEVICE_DEV_GAINRAMP_TARGETTIMEA)).toInt());
    set_targetTimeB(ConfigIni->value(QString(CONFIG_DEVICE_DEV_GAINRAMP_TARGETTIMEB)).toInt());
    set_targetGainA(ConfigIni->value(QString(CONFIG_DEVICE_DEV_GAINRAMP_TARGETGAINA)).toInt());
    set_targetGainB(ConfigIni->value(QString(CONFIG_DEVICE_DEV_GAINRAMP_TARGETGAINB)).toInt());
    set_muteTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_GAINRAMP_MUTETIME)).toInt());
    set_effectGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_GAINRAMP_EFFECTGAIN)).toInt());
    ConfigIni->endGroup();
}


void ConfigItemGainRamp::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_GAINRAMP_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_GAINRAMP_TARGETTIMEA), get_targetTimeA());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_GAINRAMP_TARGETTIMEB), get_targetTimeB());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_GAINRAMP_TARGETGAINA), get_targetGainA());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_GAINRAMP_TARGETGAINB), get_targetGainB());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_GAINRAMP_MUTETIME), get_muteTime());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_GAINRAMP_EFFECTGAIN), get_effectGain());
    ConfigIni->endGroup();
}

QVariant ConfigItemGainRamp::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    //该模块暂时没有Lua控制针脚
    Q_UNUSED(param)
    assert(false);
    return val;
}

void ConfigItemGainRamp::set_targetTimeA(int value)
{
    _body.targetAT_ms = value;
    saveParamToDevice();
}

void ConfigItemGainRamp::set_targetTimeB(int value)
{
    _body.targetBT_ms = value;
    saveParamToDevice();
}

void ConfigItemGainRamp::set_targetGainA(int value)
{
    _body.targetGainA = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemGainRamp::set_targetGainB(int value)
{
    _body.targetGainB = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemGainRamp::set_muteTime(int value)
{
    _body.unmuteT_ms = value;
    saveParamToDevice();
}

void ConfigItemGainRamp::set_effectGain(int value)
{
    _body.effectGain = value;
    saveParamToDevice();
}
