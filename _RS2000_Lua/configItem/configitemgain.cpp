#include "configitemgain.h"
#include "configSetting/cfgsettinggain.h"

ConfigItemGain::ConfigItemGain(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
   :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    int port_num = 1;
    memset(&_body,      0, sizeof(IG_PARACTL_GAIN));

    addSubType(CFG_TYPE_GAIN_1);
    addSubType(CFG_TYPE_GAIN_2);
    addSubType(CFG_TYPE_GAIN_4);
    addSubType(CFG_TYPE_GAIN_8);
    addSubType(CFG_TYPE_GAIN_16);
    addSubType(CFG_TYPE_GAIN_32);

    switch(cfgType) {
    case CFG_TYPE_GAIN_1:
        port_num = 1;
        break;
    case CFG_TYPE_GAIN_2:
        port_num = 2;
        break;
    case CFG_TYPE_GAIN_4:
        port_num = 4;
        break;
    case CFG_TYPE_GAIN_8:
        port_num = 8;
        break;
    case CFG_TYPE_GAIN_16:
        port_num = 16;
        break;
    case CFG_TYPE_GAIN_32:
        port_num = 32;
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    initPort(port_num, port_num);
//    _body.tsMode  = 0;
    _body.tsMode  = 2;
    _body.antiPhase = 0;
    _body.gain = (0 + 100) * 100;

//    QList<CLuaPinParam_PC*> pinList;
//    pinList.append(new CLuaPinParam_PC(this, name, "RampTime", 0x000E0002));
//    pinList.append(new CLuaPinParam_PC(this, name, "GainRampGain", 0x000E0004));
//    pinList.append(new CLuaPinParam_PC(this, name, "GainRampType", 0x000E0006));
//    initControlPins(pinList);
    _setting = NULL;
}

ConfigItemGain::~ConfigItemGain()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemGain::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingGain(this);
    }
    return _setting;
}


QByteArray ConfigItemGain::getDetailData()
{
    QByteArray paraBuff;

    paraBuff.append((const char*)&_body, sizeof(IG_PARACTL_GAIN));

    return paraBuff;
}

void ConfigItemGain::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_GAIN));
    if(_setting != NULL) {
        _setting->updateUiData();
    }
}

void ConfigItemGain::setRtnData(unsigned int *d)
{
    if (_setting) {
        _setting->DispRtnData(d);
    }
}

void ConfigItemGain::updateCfgSettingLoadedData()
{
    if(_setting != NULL) {
        _setting->updateUiData();
    }
}

void ConfigItemGain::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_GAIN_GROUP));
    set_mode(ConfigIni->value(QString(CONFIG_DEVICE_DEV_GAIN_MODE)).toInt());
    set_phase(ConfigIni->value(QString(CONFIG_DEVICE_DEV_GAIN_PHASE)).toInt());
    set_gain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_GAIN_GAINVALUE)).toInt());
    ConfigIni->endGroup();
}


void ConfigItemGain::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_GAIN_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_GAIN_MODE), get_mode());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_GAIN_PHASE), get_phase());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_GAIN_GAINVALUE), get_gain());
    ConfigIni->endGroup();
}

QVariant ConfigItemGain::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    switch(param->SetCmdKey())
    {
    case 0x000E0002:
        assert(false); //wj
//        bool setGainRampTime(const QString& itemName, quint8 type, quint16 ms);
//        void set_phase(int en);
//        setFreq(val.toDouble());
        return QVariant(val.toDouble());
    case 0x000E0004:
        assert(false); //wj
//        bool setGainRampGain(const QString& itemName, quint8 type, double gain);
//        set_gain((val.toDouble() + 100) * 100);
        return QVariant(val.toDouble());
    case 0x000E0006:
        set_mode(val.toInt());
        return QVariant(val.toInt());
    default:
        return QVariant();
    }
}

void ConfigItemGain::set_mode(int en)
{
    _body.tsMode = en;
    saveParamToDevice();
}

void ConfigItemGain::set_phase(int en)
{
    _body.antiPhase = en;
    saveParamToDevice();
}

void ConfigItemGain::set_gain(double value)
{
    _body.gain = (value + 100) * 100;
    saveParamToDevice();
}
