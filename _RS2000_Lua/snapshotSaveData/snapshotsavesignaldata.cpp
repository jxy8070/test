#include "snapshotsavesignaldata.h"

#include "configItem/configitemsignal.h"

void SnapshotSaveSignalData::saveSignalData(ConfigItem *cfgItem, int type)
{
    ConfigItemSignal *signalItem = qgraphicsitem_cast<ConfigItemSignal *>(cfgItem);

    _moduleNum.insert(signalItem->customName(), signalItem->getModuleNum());

    if(signalItem->getCfgType() == CFG_TYPE_SIG_SINE)
    {
        SignalData signalData;
        signalData._freq  = signalItem->getFreq();
        signalData._level = signalItem->getLevel();
        signalData._mute  = signalItem->getMute();

        _signal_Sine_DataList[type].insert(signalItem->customName(), signalData);
//        qDebug()<<__FUNCTION__<<__LINE__<<signalItem->configName();
    }
    if(signalItem->getCfgType() == CFG_TYPE_SIG_NOISE)
    {
        SignalData signalData;
        signalData._freq  = signalItem->getFreq();
        signalData._level = signalItem->getLevel();
        signalData._mute  = signalItem->getMute();

        _signal_White_DataList[type].insert(signalItem->customName(), signalData);
    }
    if(signalItem->getCfgType() == CFG_TYPE_SIG_PINKNOISE)
    {
        SignalData signalData;
        signalData._freq  = signalItem->getFreq();
        signalData._level = signalItem->getLevel();
        signalData._mute  = signalItem->getMute();

        _signal_Pink_DataList[type].insert(signalItem->customName(), signalData);
    }

}

void SnapshotSaveSignalData::setSignalData(ConfigItem *cfgItem, int type)
{
    ConfigItemSignal *signalItem = qgraphicsitem_cast<ConfigItemSignal *>(cfgItem);

    quint32 moduleNum = _moduleNum.find(signalItem->customName()).value();
    signalItem->setModuleNum(moduleNum);

    if(signalItem->getCfgType() == CFG_TYPE_SIG_SINE)
    {
        if(!findCfgItemData<SignalData>(_signal_Sine_DataList[type], cfgItem)) return;

        SignalData signalData = _signal_Sine_DataList[type].find(signalItem->customName()).value();
        signalItem->setFreq(signalData._freq);
        signalItem->setLevel(signalData._level);
        signalItem->setMute(signalData._mute);

        signalItem->updateCfgSettingLoadedData();
    }
    else if(signalItem->getCfgType() == CFG_TYPE_SIG_NOISE)
    {
        if(!findCfgItemData<SignalData>(_signal_White_DataList[type], cfgItem)) return;

        SignalData signalData = _signal_White_DataList[type].find(signalItem->customName()).value();
        signalItem->setFreq(signalData._freq);
        signalItem->setLevel(signalData._level);
        signalItem->setMute(signalData._mute);

        signalItem->updateCfgSettingLoadedData();
    }
    else if(signalItem->getCfgType() == CFG_TYPE_SIG_PINKNOISE)
    {
        if(!findCfgItemData<SignalData>(_signal_Pink_DataList[type], cfgItem)) return;

        SignalData signalData = _signal_Pink_DataList[type].find(signalItem->customName()).value();
        signalItem->setFreq(signalData._freq);
        signalItem->setLevel(signalData._level);
        signalItem->setMute(signalData._mute);

        signalItem->updateCfgSettingLoadedData();
    }

}

void SnapshotSaveSignalData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    QList<QString> nameList = getSignalDataList(type, pattern).keys();
    int unimoduleNum = getSignalDataList(type, pattern).count();
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_SIGNAL_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_SIGNAL).arg(idx));
        quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
//        qDebug()<<__FUNCTION__<<__LINE__<<nameList.at(idx);

        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SIG_MUTE),
                            getSignalDataList(type, pattern)[nameList.at(idx)]._mute);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SIG_LEVEL),
                            getSignalDataList(type, pattern)[nameList.at(idx)]._level);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SIG_FREQ),
                            getSignalDataList(type, pattern)[nameList.at(idx)]._freq);

        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

void SnapshotSaveSignalData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_SIGNAL_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_SIGNAL).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();

        bool mute = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SIG_MUTE)).toBool();
        double freq = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SIG_FREQ)).toDouble();
        double level = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SIG_LEVEL)).toDouble();

        setSignalData(devName, type, pattern, mute, freq, level, moduleNum);

        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

void SnapshotSaveSignalData::setSignalData(QString devName, CfgType type, int pattern, bool mute,
                                       double freq, double level, quint32 moduleNum)
{
    SignalData body;
    body._mute = mute;
    body._freq = freq;
    body._level = level;

    _moduleNum.insert(devName, moduleNum);

    if(type == CFG_TYPE_SIG_NOISE)
    {
        _signal_White_DataList[pattern].insert(devName, body);
    }
    if(type == CFG_TYPE_SIG_SINE)
    {
        _signal_Sine_DataList[pattern].insert(devName, body);
    }
}

QHash<QString, SnapshotSaveSignalData::SignalData> SnapshotSaveSignalData::getSignalDataList(CfgType type, int pattern)
{
    QHash<QString, SignalData> list;
    if(type == CFG_TYPE_SIG_NOISE)
    {
        list = _signal_White_DataList[pattern];
    }
    if(type == CFG_TYPE_SIG_SINE)
    {
        list = _signal_Sine_DataList[pattern];
    }
    return list;
}

