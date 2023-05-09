#include "configitemdyn.h"
#include "configSetting/cfgsettingdyn.h"

ConfigItemDyn::ConfigItemDyn(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    int inNum = 1, outNum = 1;
    memset(&_body, 0, sizeof(IG_PARACTL_DYN));

    addSubType(cfgType);
//    addSubType(CFG_TYPE_DYN_AUTO);
//    addSubType(CFG_TYPE_DYN_COMP);
//    addSubType(CFG_TYPE_DYN_LIMIT);
//    addSubType(CFG_TYPE_DYN_NOISE);
//    addSubType(CFG_TYPE_DYN_EXPAND);
//    addSubType(CFG_TYPE_DYN_PEAK);

    initPort(inNum, outNum);
    switch(cfgType) {
    case CFG_TYPE_DYN_AUTO:
    case CFG_TYPE_DYN_COMP:
        initDyn(4.0);
        break;
    case CFG_TYPE_DYN_LIMIT:
    case CFG_TYPE_DYN_NOISE:
    case CFG_TYPE_DYN_PEAK:
        initDyn(0.0);
        break;
    case CFG_TYPE_DYN_EXPAND:
        initDyn(0.5);
        break;
    default:
        Q_ASSERT(false);
        break;
    }


    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "Threshold", 0x00050001, 0x00050002));
    pinList.append(new CLuaPinParam_PC(this, name, "Ratio", 0x00050003, 0x00050004));
    pinList.append(new CLuaPinParam_PC(this, name, "AttackTime", 0x00050005, 0x00050006));
    pinList.append(new CLuaPinParam_PC(this, name, "ReleaseTime", 0x00050007, 0x00050008));
    pinList.append(new CLuaPinParam_PC(this, name, "Gain", 0x00050009, 0x0005000A));
//    pinList.append(new CLuaPinParam_PC(this, name, "TargetLevel", 0x0005000B, 0x0005000C)); //暂时没有这个功能
    pinList.append(new CLuaPinParam_PC(this, name, "Bypass", 0x0005000D, 0x0005000E));
    initControlPins(pinList);

    _setting = NULL;
}

ConfigItemDyn::~ConfigItemDyn()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemDyn::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingDyn(this);
    }
    return _setting;
}

void ConfigItemDyn::initDyn(double ratio)
{
    _body.bypass = true;
    _body.responseT_ms = 200;
    _body.releaseT_ms = 400;
    _body.ratio = ratio * 100;
    _body.threshhold = (0 + 100) * 100;
    _body.eGain = (0 + 100) * 100;
    _body.oTargetGain = (0 + 100) * 100;
}


QByteArray ConfigItemDyn::getDetailData()
{
    QByteArray paraBuff;

    paraBuff.append((const char*)&_body, sizeof(IG_PARACTL_DYN));

    return paraBuff;
}

void ConfigItemDyn::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_DYN));

    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemDyn::setRtnData(unsigned int *d)
{
    IG_PARAGET_DYN *p = (IG_PARAGET_DYN  *)d;
    if (_setting)
    {
        _setting->setMeterDisp(p->iLevel * 0.01 - 100, p->oLevel * 0.01 - 100);
    }
}

void ConfigItemDyn::updateCfgSettingLoadedData()
{
    if(_setting != NULL)
    {
        _setting->updateUiData();
    }
}

void ConfigItemDyn::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_DYN_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
    setThreshold(ConfigIni->value(QString(CONFIG_DEVICE_DEV_DYN_THRESHOLD)).toDouble());
    setRatio(ConfigIni->value(QString(CONFIG_DEVICE_DEV_DYN_RATIO)).toDouble());
    setGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_DYN_GAIN)).toDouble());
    setAttack(ConfigIni->value(QString(CONFIG_DEVICE_DEV_DYN_ATTACK)).toDouble());
    setRelease(ConfigIni->value(QString(CONFIG_DEVICE_DEV_DYN_RELEASE)).toDouble());
    setBypass(ConfigIni->value(QString(CONFIG_DEVICE_DEV_DYN_BYPASS)).toBool());
    ConfigIni->endGroup();
}

void ConfigItemDyn::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_DYN_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DYN_THRESHOLD), getThreshold());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DYN_RATIO), getRatio());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DYN_GAIN), getGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DYN_ATTACK), getAttack());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DYN_RELEASE), getRelease());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DYN_BYPASS), getBypass());
    ConfigIni->endGroup();
}

QVariant ConfigItemDyn::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    switch(param->SetCmdKey())
    {
    case 0x00050002:
        setThreshold(val.toDouble());
        return QVariant(val.toDouble());
    case 0x00050004:
        setRatio(val.toDouble());
        return QVariant(val.toDouble());
    case 0x00050006:
        setAttack(val.toInt());
        return QVariant(val.toInt());
    case 0x00050008:
        setRelease(val.toInt());
        return QVariant(val.toInt());
    case 0x0005000A:
        setGain(val.toDouble());
        return QVariant(val.toDouble());
    case 0x0005000C:
        assert(false); //暂时没有这个功能
        return QVariant(val.toDouble());
    case 0x0005000E:
        setBypass(val.toBool());
        return QVariant(val.toBool());
    default:
        return QVariant();
    }
}

double ConfigItemDyn::getGain()
{
    switch(_cfgType) {
    case CFG_TYPE_DYN_AUTO:
        return _body.oTargetGain * 0.01 - 100;
        break;
    case CFG_TYPE_DYN_NOISE:
        return _body.eGain * 0.01 - 100;
        break;
    case CFG_TYPE_DYN_COMP:
    case CFG_TYPE_DYN_LIMIT:
    case CFG_TYPE_DYN_PEAK:
    case CFG_TYPE_DYN_EXPAND:
    default:
        break;
    }

    return 0;
}

void ConfigItemDyn::setBypass(bool flag)
{
    _body.bypass = flag;
    saveParamToDevice();
}

void ConfigItemDyn::setThreshold(double value)
{
    _body.threshhold = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemDyn::setRatio(double value)
{
    _body.ratio = value * 100;
    saveParamToDevice();
}

void ConfigItemDyn::setGain(double value)
{
    switch(_cfgType) {
    case CFG_TYPE_DYN_AUTO:
        _body.oTargetGain = (value + 100) * 100;
        break;
    case CFG_TYPE_DYN_NOISE:
        _body.eGain = (value + 100) * 100;
        break;
    case CFG_TYPE_DYN_COMP:
    case CFG_TYPE_DYN_LIMIT:
    case CFG_TYPE_DYN_PEAK:
    case CFG_TYPE_DYN_EXPAND:
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemDyn::setAttack(double value)
{
    _body.responseT_ms = value;
    saveParamToDevice();
}

void ConfigItemDyn::setRelease(double value)
{
    _body.releaseT_ms = value;
    saveParamToDevice();
}
