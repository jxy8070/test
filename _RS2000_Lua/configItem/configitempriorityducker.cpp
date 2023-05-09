#include "configitempriorityducker.h"
#include "configSetting/cfgsettingpriorityducker.h"

ConfigItemPriorityDucker::ConfigItemPriorityDucker(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos,  int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    memset(&_body,     0, sizeof(IG_PARACTL_PDUCKER));

    addSubType(CFG_TYPE_PDUCKER_4);
    addSubType(CFG_TYPE_PDUCKER_8);
#if !Logo_FangTu
    addSubType(CFG_TYPE_PDUCKER_16);
    addSubType(CFG_TYPE_PDUCKER_32);
#endif

    int portNum = 4;
    switch(cfgType) {
    case CFG_TYPE_PDUCKER_4:
        portNum = 4;
        break;
    case CFG_TYPE_PDUCKER_8:
        portNum = 8;
        break;
    case CFG_TYPE_PDUCKER_16:
        portNum = 16;
        break;
    case CFG_TYPE_PDUCKER_32:
        portNum = 32;
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    QStringList iPortNameList;
    for (int i = 0; i < portNum; ++i) {
        iPortNameList.append(QString("Input %1").arg(i+1));
    }
    iPortNameList.append(QString("Priority"));

    initPort(portNum + 1, portNum, iPortNameList);

    _body.attackT_ms     = 10;
    _body.releaseT_ms    = 100;
    _body.holdT_ms       = 100;
    _body.DetectT_ms     = 5;
    _body.threshhold     = (-20 + 100) * 100;
    _body.depth          = (100 + 100) * 100;
    _body.priorChInGain  = (0 + 100) * 100;
    _body.priorChOutGain = (0 + 100) * 100;
    _body.mixOutGain     = (0 + 100) * 100;
//    _body.bypass         = false;
    _body.bypass         = true;

    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "AttackTime", 0x000D0001, 0x000D0002));
    pinList.append(new CLuaPinParam_PC(this, name, "ReleaseTime", 0x000D0003, 0x000D0004));
    pinList.append(new CLuaPinParam_PC(this, name, "HoldTime", 0x000D0005, 0x000D0006));
    pinList.append(new CLuaPinParam_PC(this, name, "DetectTime", 0x000D0007,  0x000D0008));
    pinList.append(new CLuaPinParam_PC(this, name, "Threshold", 0x000D0009,  0x000D000A));
    pinList.append(new CLuaPinParam_PC(this, name, "Depth", 0x000D000B,  0x000D000C));
    pinList.append(new CLuaPinParam_PC(this, name, "InGain", 0x000D000D,  0x000D000E));
    pinList.append(new CLuaPinParam_PC(this, name, "PriorityGain", 0x000D000F,  0x000D0010));
    pinList.append(new CLuaPinParam_PC(this, name, "OutGain", 0x000D0011,  0x000D0012));
    pinList.append(new CLuaPinParam_PC(this, name, "Bypass", 0x000D0013,  0x000D0014));
    initControlPins(pinList);
    _setting = NULL;
}

ConfigItemPriorityDucker::~ConfigItemPriorityDucker()
{
    if (_setting != NULL)
    {
        delete _setting;
    }
}

QWidget *ConfigItemPriorityDucker::getSettingForm()
{
    if (_setting == NULL)
    {
        _setting = new CfgSettingPriorityDucker(this);
    }
    return _setting;
}


QByteArray ConfigItemPriorityDucker::getDetailData()
{
    QByteArray paraBuff;

    paraBuff.append((const char*)&_body, sizeof(IG_PARACTL_PDUCKER));

    return paraBuff;
}


void ConfigItemPriorityDucker::setDetailData(unsigned int *d)
{
    memcpy(&_body, d, sizeof(IG_PARACTL_PDUCKER));
    if(_setting != NULL) {
        _setting->updateUiData();
    }
}

void ConfigItemPriorityDucker::setRtnData(unsigned int *d)
{
    if (_setting) {
        _setting->DispRtnData(d);
    }
}

void ConfigItemPriorityDucker::updateCfgSettingLoadedData()
{
    if(_setting != NULL) {
        _setting->updateUiData();
    }
}

void ConfigItemPriorityDucker::loadParaFromSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_GROUP));
    setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toInt());
    SetAttatkTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_ATTACKTIME)).toInt());
    SetReleaseTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_RELEASETIME)).toInt());
    SetHoldTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_HOLDTIME)).toInt());
    SetDetectorTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_DETECTORTIME)).toInt());
    SetThresholdLevel(ConfigIni->value(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_THRESHOLDLEVEL)).toInt());
    SetDepth(ConfigIni->value(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_DEPTH)).toInt());
    SetInGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_PRIORCHINGAIN)).toInt());
    SetPriorityGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_PRIORCHOUTGAIN)).toInt());
    SetOutGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_MIXOUTGAIN)).toInt());
    SetBypass(ConfigIni->value(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_BYPASS)).toBool());
    ConfigIni->endGroup();
}


void ConfigItemPriorityDucker::saveParaToSetting(QSettings *ConfigIni)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_GROUP));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_ATTACKTIME), GetAttackTime());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_RELEASETIME), GetReleaseTime());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_HOLDTIME), GetHoldTime());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_DETECTORTIME), GetDetectorTime());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_THRESHOLDLEVEL), GetThresholdLevel());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_DEPTH), GetDepth());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_PRIORCHINGAIN), GetInGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_PRIORCHOUTGAIN), GetPriorityGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_MIXOUTGAIN), GetOutGain());
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_PRIORITYDUCKER_BYPASS), GetBypass());
    ConfigIni->endGroup();
}

QVariant ConfigItemPriorityDucker::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{

    switch(param->SetCmdKey())
    {
    case 0x000D0002:
        SetAttatkTime(val.toInt());
        return QVariant(val.toInt());
    case 0x000D0004:
        SetReleaseTime(val.toInt());
        return QVariant(val.toInt());
    case 0x000D0006:
        SetHoldTime(val.toInt());
        return QVariant(val.toInt());
    case 0x000D0008:
        SetDetectorTime(val.toInt());
        return QVariant(val.toInt());
    case 0x000D000A:
        SetThresholdLevel(val.toDouble());
        return QVariant(val.toDouble());
    case 0x000D000C:
        SetDepth(val.toDouble());
        return QVariant(val.toDouble());
    case 0x000D000E:
        SetInGain(val.toDouble());
        return QVariant(val.toDouble());
    case 0x000D0010:
        SetPriorityGain(val.toDouble());
        return QVariant(val.toDouble());
    case 0x000D0012:
        SetOutGain(val.toDouble());
        return QVariant(val.toDouble());
    case 0x000D0014:
        SetBypass(val.toBool());
        return QVariant(val.toBool());
    default:
        return QVariant();
    }
}

void ConfigItemPriorityDucker::SetAttatkTime(uint16_t value)
{
    _body.attackT_ms = value;
    saveParamToDevice();
}

void ConfigItemPriorityDucker::SetReleaseTime(uint16_t value)
{
    _body.releaseT_ms = value;
    saveParamToDevice();
}

void ConfigItemPriorityDucker::SetHoldTime(uint16_t value)
{
    _body.holdT_ms = value;
    saveParamToDevice();
}

void ConfigItemPriorityDucker::SetDetectorTime(uint16_t value)
{
    _body.DetectT_ms = value;
    saveParamToDevice();
}

void ConfigItemPriorityDucker::SetThresholdLevel(int value)
{
    _body.threshhold = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemPriorityDucker::SetDepth(int value)
{
    _body.depth = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemPriorityDucker::SetInGain(int value)
{
    _body.priorChInGain = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemPriorityDucker::SetPriorityGain(int value)
{
    _body.priorChOutGain = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemPriorityDucker::SetOutGain(int value)
{
    _body.mixOutGain = (value + 100) * 100;
    saveParamToDevice();
}

void ConfigItemPriorityDucker::SetBypass(bool checked)
{
    _body.bypass = checked;
    saveParamToDevice();
}
