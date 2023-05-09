#include "snapshotsavesplitdata.h"

#include "configItem/configitemsplit.h"

void SnapshotSaveSplitData::saveSplitData(ConfigItem *cfgItem, int type)
{
    ConfigItemSplit *splitItem = qgraphicsitem_cast<ConfigItemSplit *>(cfgItem);

    _moduleNum.insert(splitItem->customName(), splitItem->getModuleNum());
    int chNum = 0;
    if(splitItem->getCfgType() == CFG_TYPE_SPLIT_2)
    {
        chNum = 2;
        struct ItemSplitStr splitData;

        splitData.Input.mute  = splitItem->getInputMute();
        splitData.Input.level = splitItem->getInputLevel();
        for(int i = 0; i < chNum; i++)
        {
            splitData.Segment[i].Filter.setSyncL    = splitItem->getFilterSetSyncL(i);
            splitData.Segment[i].Filter.setSyncR    = splitItem->getFilterSetSyncR(i);
            splitData.Segment[i].Filter.filterTypeL = splitItem->getFilterFilterTypeL(i);
            splitData.Segment[i].Filter.filterTypeR = splitItem->getFilterFilterTypeR(i);
            splitData.Segment[i].Filter.freqL       = splitItem->getFilterFreqL(i);
            splitData.Segment[i].Filter.freqR       = splitItem->getFilterFreqR(i);

            splitData.Segment[i].Filter.setSyncLEnable    = splitItem->getFilterSetSyncLEnable(i);
            splitData.Segment[i].Filter.setSyncREnable    = splitItem->getFilterSetSyncREnable(i);
            splitData.Segment[i].Filter.filterTypeLEnable = splitItem->getFilterFilterTypeLEnable(i);
            splitData.Segment[i].Filter.filterTypeREnable = splitItem->getFilterFilterTypeREnable(i);
            splitData.Segment[i].Filter.freqLEnable       = splitItem->getFilterFreqLEnable(i);
            splitData.Segment[i].Filter.freqREnable       = splitItem->getFilterFreqREnable(i);
            splitData.Segment[i].Output.mute   = splitItem->getOutputMute(i);
            splitData.Segment[i].Output.invert = splitItem->getOutputInvert(i);
            splitData.Segment[i].Output.level  = splitItem->getOutputLevel(i);
//            qDebug()<<__FUNCTION__<<__LINE__<<splitItem->getFilterSetSyncL(i)<<splitItem->getFilterSetSyncR(i);
        }
        _split_2_DataList[type].insert(splitItem->customName(), splitData);
    }
    if(splitItem->getCfgType() == CFG_TYPE_SPLIT_3)
    {
        chNum = 3;
        struct ItemSplitStr splitData;

        splitData.Input.mute  = splitItem->getInputMute();
        splitData.Input.level = splitItem->getInputLevel();
        for(int i = 0; i < chNum; i++)
        {
            splitData.Segment[i].Filter.setSyncL    = splitItem->getFilterSetSyncL(i);
            splitData.Segment[i].Filter.setSyncR    = splitItem->getFilterSetSyncR(i);
            splitData.Segment[i].Filter.filterTypeL = splitItem->getFilterFilterTypeL(i);
            splitData.Segment[i].Filter.filterTypeR = splitItem->getFilterFilterTypeR(i);
            splitData.Segment[i].Filter.freqL       = splitItem->getFilterFreqL(i);
            splitData.Segment[i].Filter.freqR       = splitItem->getFilterFreqR(i);

            splitData.Segment[i].Filter.setSyncLEnable    = splitItem->getFilterSetSyncLEnable(i);
            splitData.Segment[i].Filter.setSyncREnable    = splitItem->getFilterSetSyncREnable(i);
            splitData.Segment[i].Filter.filterTypeLEnable = splitItem->getFilterFilterTypeLEnable(i);
            splitData.Segment[i].Filter.filterTypeREnable = splitItem->getFilterFilterTypeREnable(i);
            splitData.Segment[i].Filter.freqLEnable       = splitItem->getFilterFreqLEnable(i);
            splitData.Segment[i].Filter.freqREnable       = splitItem->getFilterFreqREnable(i);
            splitData.Segment[i].Output.mute   = splitItem->getOutputMute(i);
            splitData.Segment[i].Output.invert = splitItem->getOutputInvert(i);
            splitData.Segment[i].Output.level  = splitItem->getOutputLevel(i);
//            qDebug()<<__FUNCTION__<<__LINE__<<splitItem->getOutputLevel(i);
        }
        _split_3_DataList[type].insert(splitItem->customName(), splitData);
    }
    if(splitItem->getCfgType() == CFG_TYPE_SPLIT_4)
    {
        chNum = 4;
        struct ItemSplitStr splitData;

        splitData.Input.mute  = splitItem->getInputMute();
        splitData.Input.level = splitItem->getInputLevel();
        for(int i = 0; i < chNum; i++)
        {
            splitData.Segment[i].Filter.setSyncL    = splitItem->getFilterSetSyncL(i);
            splitData.Segment[i].Filter.setSyncR    = splitItem->getFilterSetSyncR(i);
            splitData.Segment[i].Filter.filterTypeL = splitItem->getFilterFilterTypeL(i);
            splitData.Segment[i].Filter.filterTypeR = splitItem->getFilterFilterTypeR(i);
            splitData.Segment[i].Filter.freqL       = splitItem->getFilterFreqL(i);
            splitData.Segment[i].Filter.freqR       = splitItem->getFilterFreqR(i);

            splitData.Segment[i].Filter.setSyncLEnable    = splitItem->getFilterSetSyncLEnable(i);
            splitData.Segment[i].Filter.setSyncREnable    = splitItem->getFilterSetSyncREnable(i);
            splitData.Segment[i].Filter.filterTypeLEnable = splitItem->getFilterFilterTypeLEnable(i);
            splitData.Segment[i].Filter.filterTypeREnable = splitItem->getFilterFilterTypeREnable(i);
            splitData.Segment[i].Filter.freqLEnable       = splitItem->getFilterFreqLEnable(i);
            splitData.Segment[i].Filter.freqREnable       = splitItem->getFilterFreqREnable(i);
            splitData.Segment[i].Output.mute   = splitItem->getOutputMute(i);
            splitData.Segment[i].Output.invert = splitItem->getOutputInvert(i);
            splitData.Segment[i].Output.level  = splitItem->getOutputLevel(i);
//            qDebug()<<__FUNCTION__<<__LINE__<<splitItem->getOutputLevel(i);
        }
        _split_4_DataList[type].insert(splitItem->customName(), splitData);
    }

}

void SnapshotSaveSplitData::setSplitData(ConfigItem *cfgItem, int type)
{
    ConfigItemSplit *splitItem = qgraphicsitem_cast<ConfigItemSplit *>(cfgItem);

    quint32 moduleNum = _moduleNum.find(splitItem->customName()).value();
    qDebug() << __FUNCTION__ << __LINE__ << moduleNum << splitItem->getModuleNum();
    splitItem->setModuleNum(moduleNum);

    int chNum = 0;
    if(splitItem->getCfgType() == CFG_TYPE_SPLIT_2)
    {
        if(!findCfgItemData<ItemSplitStr>(_split_2_DataList[type], cfgItem)) return;

        chNum = 2;
        struct ItemSplitStr splitData = _split_2_DataList[type].find(splitItem->customName()).value();

        for(int i = 0; i < chNum; i++)
        {
            splitItem->setFilterSetSyncL(i, splitData.Segment[i].Filter.setSyncL);
            splitItem->setFilterSetSyncR(i, splitData.Segment[i].Filter.setSyncR);
            splitItem->setFilterFilterTypeL(i, splitData.Segment[i].Filter.filterTypeL);
            splitItem->setFilterFilterTypeR(i, splitData.Segment[i].Filter.filterTypeR);
            splitItem->setFilterFreqL(i, splitData.Segment[i].Filter.freqL);
            splitItem->setFilterFreqR(i, splitData.Segment[i].Filter.freqR);
            splitItem->setFilterSetSyncLEnable(i, splitData.Segment[i].Filter.setSyncLEnable);
            splitItem->setFilterSetSyncREnable(i, splitData.Segment[i].Filter.setSyncREnable);
            splitItem->setFilterFilterTypeLEnable(i, splitData.Segment[i].Filter.filterTypeLEnable);
            splitItem->setFilterFilterTypeREnable(i, splitData.Segment[i].Filter.filterTypeREnable);
            splitItem->setFilterFreqLEnable(i, splitData.Segment[i].Filter.freqLEnable);
            splitItem->setFilterFreqREnable(i, splitData.Segment[i].Filter.filterTypeR);

            splitItem->setOutputMute(i, splitData.Segment[i].Output.mute);
            splitItem->setOutputInvert(i, splitData.Segment[i].Output.invert);
            splitItem->setOutputLevel(i, splitData.Segment[i].Output.level);
//            qDebug()<<__FUNCTION__<<__LINE__<<splitItem->getFilterSetSyncL(i)<<splitItem->getFilterSetSyncR(i);
        }
        splitItem->setInputMute(splitData.Input.mute);
        splitItem->setInputLevel(splitData.Input.level);

        splitItem->updateCfgSettingLoadedData();
    }
    if(splitItem->getCfgType() == CFG_TYPE_SPLIT_3)
    {
        if(!findCfgItemData<ItemSplitStr>(_split_3_DataList[type], cfgItem)) return;

        chNum = 3;
        struct ItemSplitStr splitData = _split_3_DataList[type].find(splitItem->customName()).value();

        for(int i = 0; i < chNum; i++)
        {
            splitItem->setFilterSetSyncL(i, splitData.Segment[i].Filter.setSyncL);
            splitItem->setFilterSetSyncR(i, splitData.Segment[i].Filter.setSyncR);
            splitItem->setFilterFilterTypeL(i, splitData.Segment[i].Filter.filterTypeL);
            splitItem->setFilterFilterTypeR(i, splitData.Segment[i].Filter.filterTypeR);
            splitItem->setFilterFreqL(i, splitData.Segment[i].Filter.freqL);
            splitItem->setFilterFreqR(i, splitData.Segment[i].Filter.freqR);
            splitItem->setFilterSetSyncLEnable(i, splitData.Segment[i].Filter.setSyncLEnable);
            splitItem->setFilterSetSyncREnable(i, splitData.Segment[i].Filter.setSyncREnable);
            splitItem->setFilterFilterTypeLEnable(i, splitData.Segment[i].Filter.filterTypeLEnable);
            splitItem->setFilterFilterTypeREnable(i, splitData.Segment[i].Filter.filterTypeREnable);
            splitItem->setFilterFreqLEnable(i, splitData.Segment[i].Filter.freqLEnable);
            splitItem->setFilterFreqREnable(i, splitData.Segment[i].Filter.filterTypeR);

            splitItem->setOutputMute(i, splitData.Segment[i].Output.mute);
            splitItem->setOutputInvert(i, splitData.Segment[i].Output.invert);
            splitItem->setOutputLevel(i, splitData.Segment[i].Output.level);
        }
        splitItem->setInputMute(splitData.Input.mute);
        splitItem->setInputLevel(splitData.Input.level);

        splitItem->updateCfgSettingLoadedData();
    }
    if(splitItem->getCfgType() == CFG_TYPE_SPLIT_4)
    {
        if(!findCfgItemData<ItemSplitStr>(_split_4_DataList[type], cfgItem)) return;

        chNum = 4;
        struct ItemSplitStr splitData = _split_4_DataList[type].find(splitItem->customName()).value();

        for(int i = 0; i < chNum; i++)
        {
            splitItem->setFilterSetSyncL(i, splitData.Segment[i].Filter.setSyncL);
            splitItem->setFilterSetSyncR(i, splitData.Segment[i].Filter.setSyncR);
            splitItem->setFilterFilterTypeL(i, splitData.Segment[i].Filter.filterTypeL);
            splitItem->setFilterFilterTypeR(i, splitData.Segment[i].Filter.filterTypeR);
            splitItem->setFilterFreqL(i, splitData.Segment[i].Filter.freqL);
            splitItem->setFilterFreqR(i, splitData.Segment[i].Filter.freqR);
            splitItem->setFilterSetSyncLEnable(i, splitData.Segment[i].Filter.setSyncLEnable);
            splitItem->setFilterSetSyncREnable(i, splitData.Segment[i].Filter.setSyncREnable);
            splitItem->setFilterFilterTypeLEnable(i, splitData.Segment[i].Filter.filterTypeLEnable);
            splitItem->setFilterFilterTypeREnable(i, splitData.Segment[i].Filter.filterTypeREnable);
            splitItem->setFilterFreqLEnable(i, splitData.Segment[i].Filter.freqLEnable);
            splitItem->setFilterFreqREnable(i, splitData.Segment[i].Filter.filterTypeR);

            splitItem->setOutputMute(i, splitData.Segment[i].Output.mute);
            splitItem->setOutputInvert(i, splitData.Segment[i].Output.invert);
            splitItem->setOutputLevel(i, splitData.Segment[i].Output.level);
        }
        splitItem->setInputMute(splitData.Input.mute);
        splitItem->setInputLevel(splitData.Input.level);

        splitItem->updateCfgSettingLoadedData();
    }

}

void SnapshotSaveSplitData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    QList<QString> nameList = getSplitDataList(type,pattern).keys();
    int unimoduleNum = getSplitDataList(type, pattern).count();
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_SPLIT_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_SPLIT).arg(idx));
        quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));

        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_INPUT_LEVEL),
                            getSplitDataList(type, pattern)[nameList.at(idx)].Input.level);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_INPUT_MUTE),
                            getSplitDataList(type, pattern)[nameList.at(idx)].Input.mute);
        for (int i = 0; i < 4; ++i)
        {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_SETSYNC_L).arg(i),
                                getSplitDataList(type, pattern)[nameList.at(idx)].Segment[i].Filter.setSyncL);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_SETSYNC_R).arg(i),
                                getSplitDataList(type, pattern)[nameList.at(idx)].Segment[i].Filter.setSyncR);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_FILTERTYPE_L).arg(i),
                                getSplitDataList(type, pattern)[nameList.at(idx)].Segment[i].Filter.filterTypeL);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_FILTERTYPE_R).arg(i),
                                getSplitDataList(type, pattern)[nameList.at(idx)].Segment[i].Filter.filterTypeR);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_FREQ_L).arg(i),
                                getSplitDataList(type, pattern)[nameList.at(idx)].Segment[i].Filter.freqL);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_FREQ_R).arg(i),
                                getSplitDataList(type, pattern)[nameList.at(idx)].Segment[i].Filter.freqR);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_OUTPUT_MUTE).arg(i),
                                getSplitDataList(type, pattern)[nameList.at(idx)].Segment[i].Output.mute);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_OUTPUT_INVERT).arg(i),
                                getSplitDataList(type, pattern)[nameList.at(idx)].Segment[i].Output.invert);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SPLIT_OUTPUT_LEVLE).arg(i),
                                getSplitDataList(type, pattern)[nameList.at(idx)].Segment[i].Output.level);
        }

        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

void SnapshotSaveSplitData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_SPLIT_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();
    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_SPLIT).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();

        double inLevel = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_INPUT_LEVEL)).toDouble();
        bool inMute    = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_INPUT_MUTE)).toBool();

        const int num = 4;
        bool setSyncL[num] = {0}, setSyncR[num] = {0}, mute[num] = {0}, invert[num] = {0};
        int filterTypeL[num] = {0}, filterTypeR[num] = {0};
        double freqL[num] = {0}, freqR[num] = {0}, level[num] = {0};


        for(int i = 0; i < num; ++i)
        {
            setSyncL[i]    = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_SETSYNC_L).arg(i)).toBool();
            setSyncR[i]    = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_SETSYNC_R).arg(i)).toBool();
            filterTypeL[i] = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_FILTERTYPE_L).arg(i)).toInt();
            filterTypeR[i] = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_FILTER_FILTERTYPE_R).arg(i)).toInt();
            freqL[i]       = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_FREQ_L).arg(i)).toDouble();
            freqR[i]       = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_FREQ_R).arg(i)).toDouble();
            mute[i]        = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_OUTPUT_MUTE).arg(i)).toBool();
            invert[i]      = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_OUTPUT_INVERT).arg(i)).toBool();
            level[i]       = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SPLIT_OUTPUT_LEVLE).arg(i)).toDouble();
        }
        setSplitData(devName, type, pattern, inLevel, inMute, setSyncL, setSyncR, filterTypeL, filterTypeR, freqL, freqR, mute,
                           invert, level, moduleNum);
        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}

QHash<QString, SnapshotSaveSplitData::ItemSplitStr> SnapshotSaveSplitData::getSplitDataList(CfgType type, int pattern)
{
    QHash<QString, ItemSplitStr> list;
    switch (type) {
    case CFG_TYPE_SPLIT_2:
        list = _split_2_DataList[pattern];
        break;
    case CFG_TYPE_SPLIT_3:
        list = _split_3_DataList[pattern];
        break;
    case CFG_TYPE_SPLIT_4:
        list = _split_4_DataList[pattern];
        break;
    default:
        break;
    }

    return list;
}

void SnapshotSaveSplitData::setSplitData(QString devName, CfgType type, int pattern, double inLevel, bool inMute, bool *setSyncL, bool *setSyncR,
          int *filterTypeL, int *filterTypeR, double *freqL, double *freqR, bool *mute, bool *invert, double *level, quint32 moduleNum)
{
    ItemSplitStr body;
    body.Input.level = inLevel;
    body.Input.mute  = inMute;

    for(int i = 0; i < 4; ++i)
    {
        body.Segment[i].Filter.setSyncL = setSyncL[i];
        body.Segment[i].Filter.setSyncR = setSyncR[i];
        body.Segment[i].Filter.filterTypeL = filterTypeL[i];
        body.Segment[i].Filter.filterTypeR = filterTypeR[i];
        body.Segment[i].Filter.freqL = freqL[i];
        body.Segment[i].Filter.freqR = freqR[i];

        body.Segment[i].Filter.setSyncLEnable = true;
        body.Segment[i].Filter.setSyncREnable = true;
        body.Segment[i].Filter.filterTypeLEnable = true;
        body.Segment[i].Filter.filterTypeREnable = true;
        body.Segment[i].Filter.freqLEnable = true;
        body.Segment[i].Filter.freqREnable = true;

        body.Segment[i].Output.mute  = mute[i];
        body.Segment[i].Output.level = level[i];
        body.Segment[i].Output.invert = invert[i];
    }


    _moduleNum.insert(devName, moduleNum);

    if(type == CFG_TYPE_SPLIT_2) _split_2_DataList[pattern].insert(devName, body);
    if(type == CFG_TYPE_SPLIT_3) _split_3_DataList[pattern].insert(devName, body);
    if(type == CFG_TYPE_SPLIT_4) _split_4_DataList[pattern].insert(devName, body);
}
