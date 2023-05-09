#include "snapshotsavegpidata.h"

#include "configItem/configitemgpi.h"

void SnapshotSaveGpiData::saveGpiData(ConfigItem *cfgItem, int type)
{
    ConfigItemGPI *gpiItem = qgraphicsitem_cast<ConfigItemGPI *>(cfgItem);
    quint32 gpiIdx = gpiItem->getGpiIdx();
    _Gpi_idx_DataList[type].insert(gpiItem->customName(), gpiIdx);
    _moduleNum.insert(gpiItem->customName(), gpiItem->getModuleNum());
}

void SnapshotSaveGpiData::setGpiData(ConfigItem *cfgItem, int type)
{
    ConfigItemGPI *gpiItem = qgraphicsitem_cast<ConfigItemGPI *>(cfgItem);

    quint32 moduleNum = _moduleNum.find(gpiItem->customName()).value();
    gpiItem->setModuleNum(moduleNum);

    if(!findCfgItemData<quint32>(_Gpi_idx_DataList[type], cfgItem)) return;

    quint32 gpiIdx = _Gpi_idx_DataList[type].find(gpiItem->customName()).value();
    gpiItem->setGpiIdx(gpiIdx);

    gpiItem->updateCfgSettingLoadedData();
}

void SnapshotSaveGpiData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    QList<QString> nameList = getGpiDataList(type, pattern).keys();
    int unimoduleNum = getGpiDataList(type, pattern).count();
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_GPI_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_GPI).arg(idx));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
        quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_GPI_GPINUM),
                            (quint32)getGpiDataList(type, pattern).find(nameList.at(idx)).value());

        ConfigIni->endGroup();
    }

    ConfigIni->endGroup();
}

void SnapshotSaveGpiData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_GPI_TYPE).arg(1));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_GPI).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();
        quint32 gpiIdx = ConfigIni->value(QString(CONFIG_DEVICE_DEV_GPI_GPINUM)).toUInt();

        setGpiData(devName, type, pattern, gpiIdx, moduleNum);
        ConfigIni->endGroup();
    }

    ConfigIni->endGroup();
}

QHash<QString, quint32> SnapshotSaveGpiData::getGpiDataList(CfgType type, int pattern)
{
    QHash<QString, quint32> list;
    if(type == CFG_TYPE_GPI)
    {
        list = _Gpi_idx_DataList[pattern];
    }
    return list;
}

void SnapshotSaveGpiData::setGpiData(QString devName, CfgType type, int pattern, quint32 gpiIdx, quint32 moduleNum)
{
    if(type == CFG_TYPE_GPI)
    {
        _Gpi_idx_DataList[pattern].insert(devName, gpiIdx);
        _moduleNum.insert(devName, moduleNum);
    }
}
