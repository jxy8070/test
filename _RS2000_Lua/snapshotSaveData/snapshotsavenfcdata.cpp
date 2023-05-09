#include "snapshotsavenfcdata.h"

#include "configItem/configitemNFC.h"

void SnapshotSaveNfcData::saveNfcData(ConfigItem *cfgItem, int type)
{
    ConfigItemNFC *nfcItem = qgraphicsitem_cast<ConfigItemNFC *>(cfgItem);

    IG_PARACTL_NFC body;
    body.bandwidth = nfcItem->getBandwidth();
    body.clearfilter = 0;
    body.dynfilters = nfcItem->getDynfilters();
    body.fixfilters = nfcItem->getFixfilters();
    body.maxNotchDepth = (nfcItem->getMaxNotchDepth() + 100) * 100;
    body.outGain = (nfcItem->getOutGain() + 100) * 100;
    body.outMode = nfcItem->getOutMode();

    if(nfcItem->getCfgType() == CFG_TYPE_NFC1_8NF) _NFC_8_DataList[type].insert(nfcItem->customName(), body);
    if(nfcItem->getCfgType() == CFG_TYPE_NFC1_16NF) _NFC_16_DataList[type].insert(nfcItem->customName(), body);
    if(nfcItem->getCfgType() == CFG_TYPE_NFC1_24NF) _NFC_24_DataList[type].insert(nfcItem->customName(), body);
    if(nfcItem->getCfgType() == CFG_TYPE_NFC1_32NF) _NFC_32_DataList[type].insert(nfcItem->customName(), body);
    _moduleNum.insert(nfcItem->customName(), nfcItem->getModuleNum());
}

void SnapshotSaveNfcData::setNfcData(ConfigItem *cfgItem, int type)
{
    ConfigItemNFC *nfcItem = qgraphicsitem_cast<ConfigItemNFC *>(cfgItem);

    if(nfcItem->getCfgType() == CFG_TYPE_NFC1_8NF)
    {
        snapToConfig(_NFC_8_DataList[type], nfcItem);
    }else if(nfcItem->getCfgType() == CFG_TYPE_NFC1_16NF)
    {
        snapToConfig(_NFC_16_DataList[type], nfcItem);
    }else if(nfcItem->getCfgType() == CFG_TYPE_NFC1_24NF)
    {
        snapToConfig(_NFC_24_DataList[type], nfcItem);
    }else if(nfcItem->getCfgType() == CFG_TYPE_NFC1_32NF)
    {
        snapToConfig(_NFC_32_DataList[type], nfcItem);
    }else {
        return ;
    }

    nfcItem->updateCfgSettingLoadedData();
}

void SnapshotSaveNfcData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    QList<QString> nameList = getNFCDataList(type, pattern).keys();
//    qDebug() << __FUNCTION__ << __LINE__ << nameList;
    int unimoduleNum = getNFCDataList(type, pattern).count();
    if(unimoduleNum <= 0) return ;
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_NFC_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_NFC).arg(idx));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));

        quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_NFC_OUTGAIN),
                            getNFCDataList(type, pattern)[nameList.at(idx)].outGain * 0.01 - 100);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_NFC_OUTMODEL),
                            getNFCDataList(type, pattern)[nameList.at(idx)].outMode);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_NFC_BANDWIDTH),
                            getNFCDataList(type, pattern)[nameList.at(idx)].bandwidth);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_NFC_FIXFILTERS),
                            getNFCDataList(type, pattern)[nameList.at(idx)].fixfilters);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_NFC_DYNFILTERS),
                            getNFCDataList(type, pattern)[nameList.at(idx)].dynfilters);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_NFC_MAXNOTCHDEPTH),
                            getNFCDataList(type, pattern)[nameList.at(idx)].maxNotchDepth * 0.01 - 100);

        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

void SnapshotSaveNfcData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_NFC_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_NFC).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toInt();

        int outGain = ConfigIni->value(QString(CONFIG_DEVICE_DEV_NFC_OUTGAIN)).toInt();
        int outModel = ConfigIni->value(QString(CONFIG_DEVICE_DEV_NFC_OUTMODEL)).toInt();
        int bandWidth = ConfigIni->value(QString(CONFIG_DEVICE_DEV_NFC_BANDWIDTH)).toInt();
        int fixFilters = ConfigIni->value(QString(CONFIG_DEVICE_DEV_NFC_FIXFILTERS)).toInt();
        int dynFilters = ConfigIni->value(QString(CONFIG_DEVICE_DEV_NFC_DYNFILTERS)).toInt();
        int maxNotchDepth = ConfigIni->value(QString(CONFIG_DEVICE_DEV_NFC_MAXNOTCHDEPTH)).toInt();

        ConfigIni->endGroup();

        IG_PARACTL_NFC body;
        body.outGain = (outGain + 100) * 100;
        body.outMode = outModel;
        body.bandwidth = bandWidth;
        body.fixfilters = fixFilters;
        body.dynfilters = dynFilters;
        body.maxNotchDepth = (maxNotchDepth + 100) * 100;
        switch(type) {
        case CFG_TYPE_NFC1_8NF:
            _NFC_8_DataList[pattern].insert(devName, body);
            _moduleNum.insert(devName, moduleNum);
            break;
        case CFG_TYPE_NFC1_16NF:
            _NFC_16_DataList[pattern].insert(devName, body);
            _moduleNum.insert(devName, moduleNum);
            break;
        case CFG_TYPE_NFC1_24NF:
            _NFC_24_DataList[pattern].insert(devName, body);
            _moduleNum.insert(devName, moduleNum);
            break;
        case CFG_TYPE_NFC1_32NF:
            _NFC_32_DataList[pattern].insert(devName, body);
            _moduleNum.insert(devName, moduleNum);
            break;
        default:
            break;
        }
    }
    ConfigIni->endGroup();
}

void SnapshotSaveNfcData::snapToConfig(QHash<QString, IG_PARACTL_NFC> data, ConfigItemNFC *nfcItem)
{
    if(!findCfgItemData<IG_PARACTL_NFC>(data, nfcItem)) return;
    IG_PARACTL_NFC body = data.find(nfcItem->customName()).value();

    nfcItem->setBandwidth(body.bandwidth);
    nfcItem->setOutGain(body.outGain * 0.01 - 100);
    nfcItem->setOutMode(body.outMode);
    nfcItem->setClearFilter(body.clearfilter);
    nfcItem->setFixfilters(body.fixfilters);
    nfcItem->setDynfilters(body.dynfilters);
    nfcItem->setMaxNotchDepth(body.maxNotchDepth * 0.01 - 100);

    quint32 moduleNum = _moduleNum.find(nfcItem->customName()).value();
    nfcItem->setModuleNum(moduleNum);

}

QHash<QString, SnapshotSaveNfcData::IG_PARACTL_NFC> SnapshotSaveNfcData::getNFCDataList(CfgType type, int pattern)
{
    QHash<QString, IG_PARACTL_NFC> list;
    switch (type) {
    case CFG_TYPE_NFC1_8NF:  list = _NFC_8_DataList[pattern]; break;
    case CFG_TYPE_NFC1_16NF: list = _NFC_16_DataList[pattern]; break;
    case CFG_TYPE_NFC1_24NF: list = _NFC_24_DataList[pattern]; break;
    case CFG_TYPE_NFC1_32NF: list = _NFC_32_DataList[pattern]; break;
    default: break;
    }
    return list;
}
