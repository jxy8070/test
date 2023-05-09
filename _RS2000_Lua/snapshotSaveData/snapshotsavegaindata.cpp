#include "snapshotsavegaindata.h"
#include "configItem/configitemgain.h"


//SnapshotSaveGainData::SnapshotSaveGainData()
//{

//}

void SnapshotSaveGainData::saveGainData(ConfigItem *cfgItem, int pattern)
{
    ConfigItemGain *gainItem = qgraphicsitem_cast<ConfigItemGain *>(cfgItem);
    switch(gainItem->getCfgType()) {
    case CFG_TYPE_GAIN_1:
        configToSnap(_gain_1_DataList[pattern], gainItem); break;
    case CFG_TYPE_GAIN_2:
        configToSnap(_gain_2_DataList[pattern], gainItem); break;
    case CFG_TYPE_GAIN_4:
        configToSnap(_gain_4_DataList[pattern], gainItem); break;
    case CFG_TYPE_GAIN_8:
        configToSnap(_gain_8_DataList[pattern], gainItem); break;
    case CFG_TYPE_GAIN_16:
        configToSnap(_gain_16_DataList[pattern], gainItem); break;
    case CFG_TYPE_GAIN_32:
        configToSnap(_gain_32_DataList[pattern], gainItem); break;
    default: break;
    }
}

void SnapshotSaveGainData::setGainData(ConfigItem *cfgItem, int pattern)
{
    ConfigItemGain *gainItem = qgraphicsitem_cast<ConfigItemGain *>(cfgItem);
    switch(gainItem->getCfgType()) {
    case CFG_TYPE_GAIN_1:
        snapToConfig(_gain_1_DataList[pattern], gainItem); break;
    case CFG_TYPE_GAIN_2:
        snapToConfig(_gain_2_DataList[pattern], gainItem); break;
    case CFG_TYPE_GAIN_4:
        snapToConfig(_gain_4_DataList[pattern], gainItem); break;
    case CFG_TYPE_GAIN_8:
        snapToConfig(_gain_8_DataList[pattern], gainItem); break;
    case CFG_TYPE_GAIN_16:
        snapToConfig(_gain_16_DataList[pattern], gainItem); break;
    case CFG_TYPE_GAIN_32:
        snapToConfig(_gain_32_DataList[pattern], gainItem); break;
    default: break;
    }
    gainItem->updateCfgSettingLoadedData();
}

void SnapshotSaveGainData::saveToSetting(QSettings *configIni, int pattern, CfgType type)
{

}

void SnapshotSaveGainData::SettingToSnap(QSettings *configIni, int pattern, CfgType type)
{

}

void SnapshotSaveGainData::snapToConfig(QHash<QString, IG_PARACTL_GAIN> data, ConfigItemGain *gainItem)
{
    if(!findCfgItemData<IG_PARACTL_GAIN>(data, gainItem)) return;
    IG_PARACTL_GAIN body = data.find(gainItem->customName()).value();
    gainItem->set_mode(body.tsMode);
    gainItem->set_phase(body.antiPhase);
    gainItem->set_gain(body.gain);

    qint32 moduleNum = _moduleNum.find(gainItem->customName()).value();
    gainItem->setModuleNum(moduleNum);
}

void SnapshotSaveGainData::configToSnap(QHash<QString, IG_PARACTL_GAIN>& data, ConfigItemGain *gainItem)
{
    IG_PARACTL_GAIN body;
    body.tsMode = gainItem->get_mode();
    body.antiPhase = gainItem->get_phase();
    body.gain = gainItem->get_gain();

    data.insert(gainItem->customName(), body);
    _moduleNum.insert(gainItem->customName(), gainItem->getModuleNum());
}

