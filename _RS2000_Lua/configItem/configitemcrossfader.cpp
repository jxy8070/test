#include "configitemcrossfader.h"
#include "configSetting/cfgsettingcrossfader.h"

ConfigItemCrossfader::ConfigItemCrossfader(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
   :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_body,         0, sizeof(IG_PARACTL_CROSSFADER));

    addSubType(CFG_TYPE_CROSSFADER_2X1);
    addSubType(CFG_TYPE_CROSSFADER_4X2);
    addSubType(CFG_TYPE_CROSSFADER_8X4);
    addSubType(CFG_TYPE_CROSSFADER_16X8);
    addSubType(CFG_TYPE_CROSSFADER_32X16);

    int in_port = 1, out_port = 1;
    switch(cfgType) {
    case CFG_TYPE_CROSSFADER_2X1:
        in_port = 2; out_port = 1;
        break;
    case CFG_TYPE_CROSSFADER_4X2:
        in_port = 4; out_port = 2;
        break;
    case CFG_TYPE_CROSSFADER_8X4:
        in_port = 8; out_port = 4;
        break;
    case CFG_TYPE_CROSSFADER_16X8:
        in_port = 16; out_port = 8;
        break;
    case CFG_TYPE_CROSSFADER_32X16:
        in_port = 32; out_port = 16;
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    initPort(in_port, out_port);

    _body.chBandSwT_ms = 1000;
    _body.targetChBand = 0;
    _body.sumType      = 0;

    _setting = NULL;
}

ConfigItemCrossfader::~ConfigItemCrossfader()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemCrossfader::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingCrossfader(this);
    }
    return _setting;
}


QByteArray ConfigItemCrossfader::getDetailData()
{
    QByteArray paraBuff;

    paraBuff.append((const char*)&_body, sizeof(IG_PARACTL_CROSSFADER));

    return paraBuff;
}

void ConfigItemCrossfader::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_CROSSFADER));
    if(_setting != NULL) {
        _setting->updateUiData();
    }
}

void ConfigItemCrossfader::setRtnData(unsigned int *d)
{
    if (_setting) {
        _setting->DispRtnData(d);
    }
}

void ConfigItemCrossfader::updateCfgSettingLoadedData()
{
    if(_setting != NULL) {
        _setting->updateUiData();
    }
}

void ConfigItemCrossfader::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_CROSSFADER_GROUP));
    set_time(ConfigIni->value(QString(CONFIG_DEVICE_DEV_CROSSFADER_TIME)).toInt());
    set_ch(ConfigIni->value(QString(CONFIG_DEVICE_DEV_CROSSFADER_CH)).toInt());
    set_type(ConfigIni->value(QString(CONFIG_DEVICE_DEV_CROSSFADER_TYPE)).toInt());
    ConfigIni->endGroup();
}


void ConfigItemCrossfader::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_CROSSFADER_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_CROSSFADER_TIME), get_time());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_CROSSFADER_CH), get_ch());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_CROSSFADER_TYPE), get_type());
    ConfigIni->endGroup();
}

QVariant ConfigItemCrossfader::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{    
    //该模块暂时没有Lua控制针脚
    Q_UNUSED(param)
    assert(false);
    return val;
}

void ConfigItemCrossfader::set_time(int value)
{
    _body.chBandSwT_ms = value;
    saveParamToDevice();
}

void ConfigItemCrossfader::set_ch(int ch)
{
    _body.targetChBand = ch;
    saveParamToDevice();
}

void ConfigItemCrossfader::set_type(int type)
{
    _body.sumType = type;
    saveParamToDevice();
}
