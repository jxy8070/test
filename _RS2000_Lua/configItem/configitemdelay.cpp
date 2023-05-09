#include "configitemdelay.h"
#include "configSetting/cfgsettingdelay.h"

ConfigItemDelay::ConfigItemDelay(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_body, 0, sizeof(IG_PARACTL_DELAY));

#if Logo_FangTu /* Function */
//    addSubType(CFG_TYPE_DELAY_100);
    addSubType(cfgType);
#else
    addSubType(CFG_TYPE_DELAY_5);
    addSubType(CFG_TYPE_DELAY_10);
    addSubType(CFG_TYPE_DELAY_20);
    addSubType(CFG_TYPE_DELAY_50);
    addSubType(CFG_TYPE_DELAY_100);
    addSubType(CFG_TYPE_DELAY_500);
    addSubType(CFG_TYPE_DELAY_1000);
    addSubType(CFG_TYPE_HPDELY_1);
    addSubType(CFG_TYPE_HPDELY_5);
    addSubType(CFG_TYPE_HPDELY_10);
    addSubType(CFG_TYPE_HPDELY_20);
    addSubType(CFG_TYPE_HPDELY_50);
#endif


    switch(cfgType)
    {
    case CFG_TYPE_DELAY_5:
        _maxDelay = 5;
        break;
    case CFG_TYPE_DELAY_10:
        _maxDelay = 10;
        break;
    case CFG_TYPE_DELAY_20:
        _maxDelay = 20;
        break;
    case CFG_TYPE_DELAY_50:
        _maxDelay = 50;
        break;
    case CFG_TYPE_DELAY_100:
        _maxDelay = 100;
        break;
    case CFG_TYPE_DELAY_500:
        _maxDelay = 500;
        break;
    case CFG_TYPE_DELAY_1000:
        _maxDelay = 1000;
        break;
    case CFG_TYPE_HPDELY_1:
        _maxDelay = 1000;
        break;
    case CFG_TYPE_HPDELY_5:
        _maxDelay = 5000;
        break;
    case CFG_TYPE_HPDELY_10:
        _maxDelay = 10000;
        break;
    case CFG_TYPE_HPDELY_20:
        _maxDelay = 20000;
        break;
    case CFG_TYPE_HPDELY_50:
        _maxDelay = 50000;
        break;
    default:
        Q_ASSERT(false);
    }
    _body.delaySwitch = true;
//    _body.delayT_ms = _maxDelay;

    initPort(1, 1);

    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "Bypass", 0x00020001, 0x00020002));
    pinList.append(new CLuaPinParam_PC(this, name, "Time", 0x00020003, 0x00020004));
    initControlPins(pinList);

    _setting = NULL;
}

ConfigItemDelay::~ConfigItemDelay()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemDelay::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingDelay(this);
    }

    return _setting;
}


QByteArray ConfigItemDelay::getDetailData()
{
    QByteArray paraBuff;

    paraBuff.append((const char *)&_body, sizeof(IG_PARACTL_DELAY));

    return paraBuff;
}


void ConfigItemDelay::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_DELAY));
    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemDelay::setRtnData(unsigned int *d)
{

}

void ConfigItemDelay::updateCfgSettingLoadedData()
{
    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemDelay::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_DELAY_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    setDelay(ConfigIni->value(QString(CONFIG_DEVICE_DEV_DELAY_DELAY)).toInt());
    setBypass(ConfigIni->value(QString(CONFIG_DEVICE_DEV_DELAY_BYPASS)).toBool());
    ConfigIni->endGroup();
}

void ConfigItemDelay::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_DELAY_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DELAY_DELAY), getDelay());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DELAY_BYPASS), getbypass());
    ConfigIni->endGroup();
}

QVariant ConfigItemDelay::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    switch(param->SetCmdKey())
    {
    case 0x00020002:
        setBypass(val.toBool());
        return QVariant(val.toBool());
    case 0x00020004:
        setDelay(val.toInt());
        return QVariant(val.toInt());
    default:
        return QVariant();
    }
}

void ConfigItemDelay::setBypass(bool bypass)
{
//    qDebug() << __FUNCTION__ << __LINE__ << bypass;
    _body.delaySwitch = !bypass;
    saveParamToDevice();
}

void ConfigItemDelay::setDelay(int delay)
{
//    qDebug() << __FUNCTION__ << __LINE__ << delay;
    _body.delayT_ms =  delay;
    saveParamToDevice();
}
