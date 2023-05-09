#include "snapshotsavedyndata.h"

#include "configItem/configitemdyn.h"


void SnapshotSaveDynData::saveDynData(ConfigItem *cfgItem, int type)
{
    ConfigItemDyn *dynItem = qgraphicsitem_cast<ConfigItemDyn *>(cfgItem);

    _moduleNum.insert(dynItem->customName(), dynItem->getModuleNum());
    if(dynItem->getCfgType() == CFG_TYPE_DYN_AUTO)
    {
        IG_PARACTL_DYN body;
        body.threshhold   = (dynItem->getThreshold() + 100) * 100;
        body.ratio        = dynItem->getRatio() * 100;
        body.oTargetGain  = (dynItem->getGain() + 100) * 100;
        body.responseT_ms = dynItem->getAttack();
        body.releaseT_ms  = dynItem->getRelease();
        body.bypass       = dynItem->getBypass();
        _dyn_Auto_DataList[type].insert(dynItem->customName(), body);
    }
    if(dynItem->getCfgType() == CFG_TYPE_DYN_COMP)
    {
        IG_PARACTL_DYN body;
        body.threshhold   = (dynItem->getThreshold() + 100) * 100;
        body.ratio        = dynItem->getRatio() * 100;
        //body.oTargetGain  = (dynItem->getGain() + 100) * 100;
        body.responseT_ms = dynItem->getAttack();
        body.releaseT_ms  = dynItem->getRelease();
        body.bypass       = dynItem->getBypass();
        _dyn_Comp_DataList[type].insert(dynItem->customName(), body);
    }
    if(dynItem->getCfgType() == CFG_TYPE_DYN_EXPAND)
    {
        IG_PARACTL_DYN body;
        body.threshhold   = (dynItem->getThreshold() + 100) * 100;
        body.ratio        = dynItem->getRatio() * 100;
        //body.oTargetGain  = (dynItem->getGain() + 100) * 100;
        body.responseT_ms = dynItem->getAttack();
        body.releaseT_ms  = dynItem->getRelease();
        body.bypass       = dynItem->getBypass();
        _dyn_Expand_DataList[type].insert(dynItem->customName(), body);
    }
    if(dynItem->getCfgType() == CFG_TYPE_DYN_LIMIT)
    {
        IG_PARACTL_DYN body;
        body.threshhold   = (dynItem->getThreshold() + 100) * 100;
//        body.ratio        = dynItem->getRatio() * 100;
//        body.oTargetGain  = (dynItem->getGain() + 100) * 100;
        body.responseT_ms = dynItem->getAttack();
        body.releaseT_ms  = dynItem->getRelease();
        body.bypass       = dynItem->getBypass();
        _dyn_Limit_DataList[type].insert(dynItem->customName(), body);
    }
    if(dynItem->getCfgType() == CFG_TYPE_DYN_NOISE)
    {
        IG_PARACTL_DYN body;
        body.threshhold   = (dynItem->getThreshold() + 100) * 100;
//        body.ratio        = dynItem->getRatio() * 100;
        body.eGain  = (dynItem->getGain() + 100) * 100;
        body.responseT_ms = dynItem->getAttack();
        body.releaseT_ms  = dynItem->getRelease();
        body.bypass       = dynItem->getBypass();
        _dyn_NoiseGate_DataList[type].insert(dynItem->customName(), body);
    }
    if(dynItem->getCfgType() == CFG_TYPE_DYN_PEAK)
    {
        IG_PARACTL_DYN body;
        body.threshhold   = (dynItem->getThreshold() + 100) * 100;
//        body.ratio        = dynItem->getRatio() * 100;
//        body.oTargetGain  = (dynItem->getGain() + 100) * 100;
//        body.responseT_ms = dynItem->getAttack();
//        body.releaseT_ms  = dynItem->getRelease();
        body.bypass       = dynItem->getBypass();
        _dyn_Peak_DataList[type].insert(dynItem->customName(), body);
    }
}

void SnapshotSaveDynData::setDynData(ConfigItem *cfgItem, int type)
{
    ConfigItemDyn *dynItem = qgraphicsitem_cast<ConfigItemDyn *>(cfgItem);

    if(dynItem->getCfgType() == CFG_TYPE_DYN_AUTO)
    {
        if(!findCfgItemData<IG_PARACTL_DYN>(_dyn_Auto_DataList[type], cfgItem)) return;

        IG_PARACTL_DYN body = _dyn_Auto_DataList[type].find(dynItem->customName()).value();
        dynItem->setThreshold(body.threshhold * 0.01 - 100);
        dynItem->setRatio(body.ratio * 0.01);
        dynItem->setGain(body.oTargetGain * 0.01 - 100);
        dynItem->setAttack(body.responseT_ms);
        dynItem->setRelease(body.releaseT_ms);
        dynItem->setBypass(body.bypass);

        dynItem->updateCfgSettingLoadedData();
    }
    if(dynItem->getCfgType() == CFG_TYPE_DYN_COMP)
    {
        if(!findCfgItemData<IG_PARACTL_DYN>(_dyn_Comp_DataList[type], cfgItem)) return;

        IG_PARACTL_DYN body = _dyn_Comp_DataList[type].find(dynItem->customName()).value();
        dynItem->setThreshold(body.threshhold * 0.01 - 100);
        dynItem->setRatio(body.ratio * 0.01);
//        dynItem->setGain(body.oTargetGain * 0.01 - 100);
        dynItem->setAttack(body.responseT_ms);
        dynItem->setRelease(body.releaseT_ms);
        dynItem->setBypass(body.bypass);

        dynItem->updateCfgSettingLoadedData();
    }
    if(dynItem->getCfgType() == CFG_TYPE_DYN_EXPAND)
    {
        if(!findCfgItemData<IG_PARACTL_DYN>(_dyn_Expand_DataList[type], cfgItem)) return;

        IG_PARACTL_DYN body = _dyn_Expand_DataList[type].find(dynItem->customName()).value();
        dynItem->setThreshold(body.threshhold * 0.01 - 100);
        dynItem->setRatio(body.ratio * 0.01);
//        dynItem->setGain(body.oTargetGain * 0.01 - 100);
        dynItem->setAttack(body.responseT_ms);
        dynItem->setRelease(body.releaseT_ms);
        dynItem->setBypass(body.bypass);

        dynItem->updateCfgSettingLoadedData();
    }
    if(dynItem->getCfgType() == CFG_TYPE_DYN_LIMIT)
    {
        if(!findCfgItemData<IG_PARACTL_DYN>(_dyn_Limit_DataList[type], cfgItem)) return;

        IG_PARACTL_DYN body = _dyn_Limit_DataList[type].find(dynItem->customName()).value();
        dynItem->setThreshold(body.threshhold * 0.01 - 100);
//        dynItem->setRatio(body.ratio * 0.01);
//        dynItem->setGain(body.oTargetGain * 0.01 - 100);
        dynItem->setAttack(body.responseT_ms);
        dynItem->setRelease(body.releaseT_ms);
        dynItem->setBypass(body.bypass);

        dynItem->updateCfgSettingLoadedData();
    }
    if(dynItem->getCfgType() == CFG_TYPE_DYN_NOISE)
    {
        if(!findCfgItemData<IG_PARACTL_DYN>(_dyn_NoiseGate_DataList[type], cfgItem)) return;

        IG_PARACTL_DYN body = _dyn_NoiseGate_DataList[type].find(dynItem->customName()).value();
        dynItem->setThreshold(body.threshhold * 0.01 - 100);
        dynItem->setRatio(body.ratio * 0.01);
        dynItem->setGain(body.eGain * 0.01 - 100);
        dynItem->setAttack(body.responseT_ms);
        dynItem->setRelease(body.releaseT_ms);
        dynItem->setBypass(body.bypass);

        dynItem->updateCfgSettingLoadedData();
    }
    if(dynItem->getCfgType() == CFG_TYPE_DYN_PEAK)
    {
        if(!findCfgItemData<IG_PARACTL_DYN>(_dyn_Peak_DataList[type], cfgItem)) return;

        IG_PARACTL_DYN body = _dyn_Peak_DataList[type].find(dynItem->customName()).value();
        dynItem->setThreshold(body.threshhold * 0.01 - 100);
//        dynItem->setRatio(body.ratio * 0.01);
//        dynItem->setGain(body.oTargetGain * 0.01 - 100);
//        dynItem->setAttack(body.responseT_ms);
//        dynItem->setRelease(body.releaseT_ms);
        dynItem->setBypass(body.bypass);

        dynItem->updateCfgSettingLoadedData();
    }

    quint32 moduleNum = _moduleNum.find(dynItem->customName()).value();
    dynItem->setModuleNum(moduleNum);
}

void SnapshotSaveDynData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    QList<QString> nameList = getDynDataList(type, pattern).keys();
    int unimoduleNum = getDynDataList(type, pattern).count();
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DYN_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);
    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DYN).arg(idx));
        quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));

        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DYN_THRESHOLD), getDynThreshhold(type, pattern, nameList.at(idx)));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DYN_RATIO), getDynRadio(type, pattern, nameList.at(idx)));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DYN_GAIN), getDynGain(type, pattern, nameList.at(idx)));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DYN_ATTACK), getDynAttack(type, pattern, nameList.at(idx)));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DYN_RELEASE), getDynRelease(type, pattern, nameList.at(idx)));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DYN_BYPASS), getDynBypass(type, pattern, nameList.at(idx)));

        ConfigIni->endGroup();
    }

    ConfigIni->endGroup();
}

void SnapshotSaveDynData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DYN_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DYN).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();

        double threshhold = ConfigIni->value(QString(CONFIG_DEVICE_DEV_DYN_THRESHOLD)).toDouble();
        double ratio = ConfigIni->value(QString(CONFIG_DEVICE_DEV_DYN_RATIO)).toDouble();
        double gain = ConfigIni->value(QString(CONFIG_DEVICE_DEV_DYN_GAIN)).toDouble();
        double resonseT_ms = ConfigIni->value(QString(CONFIG_DEVICE_DEV_DYN_ATTACK)).toDouble();
        double releaseT_ms = ConfigIni->value(QString(CONFIG_DEVICE_DEV_DYN_RELEASE)).toDouble();
        bool bypass = ConfigIni->value(QString(CONFIG_DEVICE_DEV_DYN_BYPASS)).toBool();

        setDynData(devName, type, pattern, threshhold, ratio, gain, resonseT_ms, releaseT_ms, bypass, moduleNum);
        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

QHash<QString, SnapshotSaveDynData::IG_PARACTL_DYN> SnapshotSaveDynData::getDynDataList(CfgType type, int pattern)
{
    QHash<QString, IG_PARACTL_DYN> list;
    switch (type) {
    case CFG_TYPE_DYN_AUTO:
        list = _dyn_Auto_DataList[pattern];
        break;
    case CFG_TYPE_DYN_COMP:
        list = _dyn_Comp_DataList[pattern];
        break;
    case CFG_TYPE_DYN_EXPAND:
        list = _dyn_Expand_DataList[pattern];
        break;
    case CFG_TYPE_DYN_LIMIT:
        list = _dyn_Limit_DataList[pattern];
        break;
    case CFG_TYPE_DYN_NOISE:
        list = _dyn_NoiseGate_DataList[pattern];
        break;
    case CFG_TYPE_DYN_PEAK:
        list = _dyn_Peak_DataList[pattern];
        break;
    default:
        break;
    }
    return list;
}

double SnapshotSaveDynData::getDynThreshhold(CfgType type, int pattern, QString devName)
{
    switch (type) {
    case CFG_TYPE_DYN_AUTO:
       return _dyn_Auto_DataList[pattern][devName].threshhold * 0.01 - 100;
//        break;
    case CFG_TYPE_DYN_COMP:
        return _dyn_Comp_DataList[pattern][devName].threshhold * 0.01 - 100;
//        break;
    case CFG_TYPE_DYN_EXPAND:
        return _dyn_Expand_DataList[pattern][devName].threshhold * 0.01 - 100;
//        break;
    case CFG_TYPE_DYN_LIMIT:
        return _dyn_Limit_DataList[pattern][devName].threshhold * 0.01 - 100;
//        break;
    case CFG_TYPE_DYN_NOISE:
        return _dyn_NoiseGate_DataList[pattern][devName].threshhold * 0.01 - 100;
//        break;
    case CFG_TYPE_DYN_PEAK:
        return _dyn_Peak_DataList[pattern][devName].threshhold * 0.01 - 100;
//        break;
    default:
        return 0;
//        break;
    }
}

double SnapshotSaveDynData::getDynRadio(CfgType type, int pattern, QString devName)
{
    switch (type) {
    case CFG_TYPE_DYN_AUTO:
       return _dyn_Auto_DataList[pattern][devName].ratio * 0.01;
//        break;
    case CFG_TYPE_DYN_COMP:
        return _dyn_Comp_DataList[pattern][devName].ratio * 0.01;
//        break;
    case CFG_TYPE_DYN_EXPAND:
        return _dyn_Expand_DataList[pattern][devName].ratio * 0.01;
//        break;
    case CFG_TYPE_DYN_LIMIT:
        return _dyn_Limit_DataList[pattern][devName].ratio * 0.01;
//        break;
    case CFG_TYPE_DYN_NOISE:
        return _dyn_NoiseGate_DataList[pattern][devName].ratio * 0.01;
//        break;
    case CFG_TYPE_DYN_PEAK:
        return _dyn_Peak_DataList[pattern][devName].ratio * 0.01;
//        break;
    default:
        return 0;
//        break;
    }
}

double SnapshotSaveDynData::getDynGain(CfgType type, int pattern, QString devName)
{
    switch (type) {
    case CFG_TYPE_DYN_AUTO:
       return _dyn_Auto_DataList[pattern][devName].oTargetGain * 0.01 - 100;
//        break;
    case CFG_TYPE_DYN_COMP:
        return 0;
//        break;
    case CFG_TYPE_DYN_EXPAND:
        return 0;
//        break;
    case CFG_TYPE_DYN_LIMIT:
        return 0;
//        break;
    case CFG_TYPE_DYN_NOISE:
        return _dyn_NoiseGate_DataList[pattern][devName].eGain * 0.01 - 100;
//        break;
    case CFG_TYPE_DYN_PEAK:
        return 0;
//        break;
    default:
        return 0;
//        break;
    }
}

double SnapshotSaveDynData::getDynAttack(CfgType type, int pattern, QString devName)
{
    switch (type) {
    case CFG_TYPE_DYN_AUTO:
       return _dyn_Auto_DataList[pattern][devName].responseT_ms;
//        break;
    case CFG_TYPE_DYN_COMP:
        return _dyn_Comp_DataList[pattern][devName].responseT_ms;
//        break;
    case CFG_TYPE_DYN_EXPAND:
        return _dyn_Expand_DataList[pattern][devName].responseT_ms;
//        break;
    case CFG_TYPE_DYN_LIMIT:
        return _dyn_Limit_DataList[pattern][devName].responseT_ms;
//        break;
    case CFG_TYPE_DYN_NOISE:
        return _dyn_NoiseGate_DataList[pattern][devName].responseT_ms;
//        break;
    case CFG_TYPE_DYN_PEAK:
        return _dyn_Peak_DataList[pattern][devName].responseT_ms;
//        break;
    default:
        return 0;
//        break;
    }
}

double SnapshotSaveDynData::getDynRelease(CfgType type, int pattern, QString devName)
{
    switch (type) {
    case CFG_TYPE_DYN_AUTO:
       return _dyn_Auto_DataList[pattern][devName].releaseT_ms;
//        break;
    case CFG_TYPE_DYN_COMP:
        return _dyn_Comp_DataList[pattern][devName].releaseT_ms;
//        break;
    case CFG_TYPE_DYN_EXPAND:
        return _dyn_Expand_DataList[pattern][devName].releaseT_ms;
//        break;
    case CFG_TYPE_DYN_LIMIT:
        return _dyn_Limit_DataList[pattern][devName].releaseT_ms;
//        break;
    case CFG_TYPE_DYN_NOISE:
        return _dyn_NoiseGate_DataList[pattern][devName].releaseT_ms;
//        break;
    case CFG_TYPE_DYN_PEAK:
        return _dyn_Peak_DataList[pattern][devName].releaseT_ms;
//        break;
    default:
        return 0;
//        break;
    }
}

bool SnapshotSaveDynData::getDynBypass(CfgType type, int pattern, QString devName)
{
    switch (type) {
    case CFG_TYPE_DYN_AUTO:
       return _dyn_Auto_DataList[pattern][devName].bypass;
//        break;
    case CFG_TYPE_DYN_COMP:
        return _dyn_Comp_DataList[pattern][devName].bypass;
//        break;
    case CFG_TYPE_DYN_EXPAND:
        return _dyn_Expand_DataList[pattern][devName].bypass;
//        break;
    case CFG_TYPE_DYN_LIMIT:
        return _dyn_Limit_DataList[pattern][devName].bypass;
//        break;
    case CFG_TYPE_DYN_NOISE:
        return _dyn_NoiseGate_DataList[pattern][devName].bypass;
//        break;
    case CFG_TYPE_DYN_PEAK:
        return _dyn_Peak_DataList[pattern][devName].bypass;
//        break;
    default:
        return 0;
//        break;
    }
}

void SnapshotSaveDynData::setDynData(QString devName, CfgType type, int pattern, double threshhold,
                       double ratio, double gain, double resonseT_ms, double relaseT_ms, bool bypass, quint32 moduleNum)
{
    IG_PARACTL_DYN body;

    body.threshhold = (threshhold + 100) * 100;
    body.ratio      = ratio * 100;
    if(type == CFG_TYPE_DYN_AUTO)
    {
        body.oTargetGain = (gain + 100) * 100;
    }else if(type == CFG_TYPE_DYN_NOISE)
    {
        body.eGain = (gain + 100) * 100;
    }
    body.responseT_ms = resonseT_ms;
    body.releaseT_ms  = relaseT_ms;
    body.bypass       = bypass;

    switch(type)
    {
    case CFG_TYPE_DYN_AUTO:
        _dyn_Auto_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_DYN_COMP:
        _dyn_Comp_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_DYN_EXPAND:
        _dyn_Expand_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_DYN_LIMIT:
        _dyn_Limit_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_DYN_NOISE:
        _dyn_NoiseGate_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    case CFG_TYPE_DYN_PEAK:
        _dyn_Peak_DataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
        break;
    default:
        qDebug()<<__FUNCTION__<<__LINE__<<"No find devType";
        break;
    }
}
