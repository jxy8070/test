#include "snapshotsavedelaydata.h"
#include "configItem/configitemdelay.h"


void SnapshotSaveDelayData::saveDelayData(ConfigItem *cfgItem, int type)
{
    ConfigItemDelay *delayItem = qgraphicsitem_cast<ConfigItemDelay *>(cfgItem);
    IG_PARACTL_DELAY body;
    body.delaySwitch = delayItem->getbypass();
    body.delayT_ms   = delayItem->getDelay();
//    qDebug()<<__FUNCTION__<<__LINE__<<type<<delayItem->customName()<<body.delaySwitch<<body.delayT_ms;
    _delayDataList[type].insert(delayItem->customName(), body);
    _moduleNum.insert(delayItem->customName(), delayItem->getModuleNum());
}

void SnapshotSaveDelayData::setDelayData(ConfigItem *cfgItem, int type)
{
    ConfigItemDelay *delayItem = qgraphicsitem_cast<ConfigItemDelay *>(cfgItem);

    if(!findCfgItemData<IG_PARACTL_DELAY>(_delayDataList[type], cfgItem)) return;

    quint32 moduleNum = _moduleNum.find(delayItem->customName()).value();
    delayItem->setModuleNum(moduleNum);

    IG_PARACTL_DELAY body = _delayDataList[type].find(delayItem->customName()).value();
    delayItem->setBypass(body.delaySwitch);
    delayItem->setDelay(body.delayT_ms);

    delayItem->updateCfgSettingLoadedData();
    //    qDebug()<<__FUNCTION__<<__LINE__<<type<<delayItem->customName()<<body.delaySwitch<<body.delayT_ms;
}

void SnapshotSaveDelayData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    if(!_delayDataList[pattern].isEmpty())
    {
        QList<QString> nameList = _delayDataList[pattern].keys();
        int unimoduleNum = _delayDataList[pattern].count();

        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DELAY_TYPE).arg(type));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);

        for(int idx = 0; idx < unimoduleNum; idx++)
        {
            ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DELAY).arg(idx));
            quint32 ModuleNum = _moduleNum.find(nameList.at(idx)).value();
            int delayT_ms = _delayDataList[pattern][nameList.at(idx)].delayT_ms;
            bool delaySwitch = _delayDataList[pattern][nameList.at(idx)].delaySwitch;
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), ModuleNum);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DELAY_DELAY), delayT_ms);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_DELAY_BYPASS), delaySwitch);
            ConfigIni->endGroup();
        }
        ConfigIni->endGroup();
    }
}

void SnapshotSaveDelayData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DELAY_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();
//    qDebug() << __FUNCTION__ << __LINE__ << unimoduleNum << pattern;
    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DELAY).arg(idx));
        QString devName   = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        int delayT_ms     = ConfigIni->value(QString(CONFIG_DEVICE_DEV_DELAY_DELAY)).toInt();
        bool delaySwitch  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_DELAY_BYPASS)).toBool();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();
        ConfigIni->endGroup();
//        qDebug() << __FUNCTION__ << __LINE__ << unimoduleNum << pattern << delayT_ms << delaySwitch;
        IG_PARACTL_DELAY body;
        body.delayT_ms   = delayT_ms;
        body.delaySwitch = delaySwitch;

        _delayDataList[pattern].insert(devName, body);
        _moduleNum.insert(devName, moduleNum);
//        _buttonPattern[idx] = true;
    }
    ConfigIni->endGroup();
}

