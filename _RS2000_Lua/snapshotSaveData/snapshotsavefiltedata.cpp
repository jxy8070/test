#include "snapshotsavefiltedata.h"

#include "configItem/configitemfilte.h"
#include "customWidget/eqgraph.h"

void SnapshotSaveFilteData::saveFilteData(ConfigItem *cfgItem, int type)
{
    ConfigItemFilte *filterItem = qgraphicsitem_cast<ConfigItemFilte *>(cfgItem);

    _moduleNum.insert(filterItem->customName(), filterItem->getModuleNum());
    if(filterItem->getCfgType() == CFG_TYPE_FILTE_LP)
    {
        FilterData filterData;
        filterData._eqData.freq = filterItem->getEqData()->freq;
        filterData._eqData.gain = filterItem->getEqData()->gain;
        filterData._eqData.q    = filterItem->getEqData()->q;
        filterData._filterType  = filterItem->getFilterType();
        filterData._eqType      = filterItem->getEqType();
        filterData._bypass      = filterItem->getBypass();

        _filter_LowPass_DataList[type].insert(filterItem->customName(), filterData);
    }
    if(filterItem->getCfgType() == CFG_TYPE_FILTE_HP)
    {
        FilterData filterData;
        filterData._eqData.freq = filterItem->getEqData()->freq;
        filterData._eqData.gain = filterItem->getEqData()->gain;
        filterData._eqData.q    = filterItem->getEqData()->q;
        filterData._filterType  = filterItem->getFilterType();
        filterData._eqType      = filterItem->getEqType();
        filterData._bypass      = filterItem->getBypass();

        _filter_HighPass_DataList[type].insert(filterItem->customName(), filterData);
    }
    if(filterItem->getCfgType() == CFG_TYPE_FILTE_LS)
    {
        FilterData filterData;
        filterData._eqData.freq = filterItem->getEqData()->freq;
        filterData._eqData.gain = filterItem->getEqData()->gain;
        filterData._eqData.q    = filterItem->getEqData()->q;
        filterData._filterType  = filterItem->getFilterType();
        filterData._eqType      = filterItem->getEqType();
        filterData._bypass      = filterItem->getBypass();

        _filter_LowShelf_DataList[type].insert(filterItem->customName(), filterData);
    }
    if(filterItem->getCfgType() == CFG_TYPE_FILTE_HS)
    {
        FilterData filterData;
        filterData._eqData.freq = filterItem->getEqData()->freq;
        filterData._eqData.gain = filterItem->getEqData()->gain;
        filterData._eqData.q    = filterItem->getEqData()->q;
        filterData._filterType  = filterItem->getFilterType();
        filterData._eqType      = filterItem->getEqType();
        filterData._bypass      = filterItem->getBypass();

        _filter_HighShelf_DataList[type].insert(filterItem->customName(), filterData);
    }

}

void SnapshotSaveFilteData::setFilteData(ConfigItem *cfgItem, int type)
{
    ConfigItemFilte *filterItem = qgraphicsitem_cast<ConfigItemFilte *>(cfgItem);

    quint32 moduleNum = _moduleNum.find(filterItem->customName()).value();
    filterItem->setModuleNum(moduleNum);

    if(filterItem->getCfgType() == CFG_TYPE_FILTE_LP)
    {
        if(!findCfgItemData<FilterData>(_filter_LowPass_DataList[type], cfgItem)) return;

        FilterData fData = _filter_LowPass_DataList[type].find(filterItem->customName()).value();

        filterItem->setEqPara(fData._eqData.freq, fData._eqData.gain, fData._eqData.q);
        filterItem->setFilterType(fData._filterType);
        filterItem->setBypass(fData._bypass);

        filterItem->updateCfgSettingLoadedData();
    }
    if(filterItem->getCfgType() == CFG_TYPE_FILTE_HP)
    {
        if(!findCfgItemData<FilterData>(_filter_HighPass_DataList[type], cfgItem)) return;

        FilterData fData = _filter_HighPass_DataList[type].find(filterItem->customName()).value();
        filterItem->setEqPara(fData._eqData.freq, fData._eqData.gain, fData._eqData.q);
        filterItem->setFilterType(fData._filterType);
        filterItem->setBypass(fData._bypass);

        filterItem->updateCfgSettingLoadedData();
    }
    if(filterItem->getCfgType() == CFG_TYPE_FILTE_LS)
    {
        if(!findCfgItemData<FilterData>(_filter_LowShelf_DataList[type], cfgItem)) return;

        FilterData fData = _filter_LowShelf_DataList[type].find(filterItem->customName()).value();
        filterItem->setEqPara(fData._eqData.freq, fData._eqData.gain, fData._eqData.q);
        filterItem->setFilterType(fData._filterType);
        filterItem->setBypass(fData._bypass);

        filterItem->updateCfgSettingLoadedData();
    }
    if(filterItem->getCfgType() == CFG_TYPE_FILTE_HS)
    {
        if(!findCfgItemData<FilterData>(_filter_HighShelf_DataList[type], cfgItem)) return;

        FilterData fData = _filter_HighShelf_DataList[type].find(filterItem->customName()).value();
        filterItem->setEqPara(fData._eqData.freq, fData._eqData.gain, fData._eqData.q);
        filterItem->setFilterType(fData._filterType);
        filterItem->setBypass(fData._bypass);

        filterItem->updateCfgSettingLoadedData();
    }

}

void SnapshotSaveFilteData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    QList<QString> nameList = getFilterDataList(type, pattern).keys();
    int unimoduleNum = getFilterDataList(type, pattern).count();
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_FILTER_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_FILTER).arg(idx));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
        quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);

        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FILTER_BYPASS),
                            getFilterDataList(type, pattern)[nameList.at(idx)]._bypass);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FILTER_FILTERTYPE),
                            getFilterDataList(type, pattern)[nameList.at(idx)]._filterType);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FILTER_FREQ),
                            getFilterDataList(type, pattern)[nameList.at(idx)]._eqData.freq);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FILTER_GAIN),
                            getFilterDataList(type, pattern)[nameList.at(idx)]._eqData.gain);

        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

void SnapshotSaveFilteData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_FILTER_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_FILTER).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();

        bool bypass = ConfigIni->value(QString(CONFIG_DEVICE_DEV_FILTER_BYPASS)).toBool();
        quint16 filterType = ConfigIni->value(QString(CONFIG_DEVICE_DEV_FILTER_FILTERTYPE)).toUInt();
        quint16 freq = ConfigIni->value(QString(CONFIG_DEVICE_DEV_FILTER_FREQ)).toUInt();
        qint16 gain = ConfigIni->value(QString(CONFIG_DEVICE_DEV_FILTER_GAIN)).toInt();

        setFilterData(devName, type, pattern, bypass, filterType, freq, gain, moduleNum);
        ConfigIni->endGroup();
    }

    ConfigIni->endGroup();
}

QHash<QString, SnapshotSaveFilteData::FilterData> SnapshotSaveFilteData::getFilterDataList(CfgType type, int pattern)
{
    QHash<QString, FilterData> list;
    switch (type) {
    case CFG_TYPE_FILTE_HP:
        list = _filter_HighPass_DataList[pattern];
        break;
    case CFG_TYPE_FILTE_HS:
        list = _filter_HighShelf_DataList[pattern];
        break;
    case CFG_TYPE_FILTE_LP:
        list = _filter_LowPass_DataList[pattern];
        break;
    case CFG_TYPE_FILTE_LS:
        list = _filter_LowShelf_DataList[pattern];
        break;
    default:
        break;
    }

    return list;
}

void SnapshotSaveFilteData::setFilterData(QString devName, CfgType type, int pattern, bool bypass,
                                       quint16 filterType, quint16 freq, qint16 gain, quint32 moduleNum)
{
    FilterData body;
    body._bypass = bypass;
    body._filterType = filterType;
    body._eqData.freq = freq;
    body._eqData.gain = gain;
    body._eqData.q    = 100;


    switch (type) {
    case CFG_TYPE_FILTE_HP:
        body._eqType = EQ_HIGHPASS_CONFIG;
        _filter_HighPass_DataList[pattern].insert(devName, body);
        break;
    case CFG_TYPE_FILTE_HS:
        body._eqType = EQ_HIGHSHELF_CONFIG;
        _filter_HighShelf_DataList[pattern].insert(devName, body);
        break;
    case CFG_TYPE_FILTE_LP:
        body._eqType = EQ_LOWPASS_CONFIG;
        _filter_LowPass_DataList[pattern].insert(devName, body);
        break;
    case CFG_TYPE_FILTE_LS:
        body._eqType = EQ_LOWSHELF_CONFIG;
        _filter_LowShelf_DataList[pattern].insert(devName, body);
        break;
    default:
        break;
    }
}

