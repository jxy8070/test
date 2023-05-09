#include "snapshotsavegainrampdata.h"
#include "configItem/configitemgainramp.h"
//SnapshotSaveGainRampData::SnapshotSaveGainRampData()
//{

//}

void SnapshotSaveGainRampData::saveGainRampData(ConfigItem *cfgItem, int pattern)
{
    ConfigItemGainRamp *gainRampItem = qgraphicsitem_cast<ConfigItemGainRamp *>(cfgItem);
    IG_PARACTL_GAINRAMP body;
    body.targetAT_ms = gainRampItem->get_targetTimeA();
    body.targetBT_ms = gainRampItem->get_targetTimeB();
    body.unmuteT_ms  = gainRampItem->get_muteTime();
    body.effectGain  = gainRampItem->get_effectGain();
    body.targetGainA = gainRampItem->get_targetGainA();
    body.targetGainB = gainRampItem->get_targetGainB();

    switch(gainRampItem->getCfgType()) {
    case CFG_TYPE_GAINRAMP_1:
        _gainRamp_1_DataList[pattern].insert(gainRampItem->customName(), body); break;
    case CFG_TYPE_GAINRAMP_2:
        _gainRamp_2_DataList[pattern].insert(gainRampItem->customName(), body); break;
    case CFG_TYPE_GAINRAMP_4:
        _gainRamp_4_DataList[pattern].insert(gainRampItem->customName(), body); break;
    case CFG_TYPE_GAINRAMP_8:
        _gainRamp_8_DataList[pattern].insert(gainRampItem->customName(), body); break;
    case CFG_TYPE_GAINRAMP_16:
        _gainRamp_16_DataList[pattern].insert(gainRampItem->customName(), body); break;
    case CFG_TYPE_GAINRAMP_32:
        _gainRamp_32_DataList[pattern].insert(gainRampItem->customName(), body); break;
    default:
        break;
    }
}

void SnapshotSaveGainRampData::setGainRampData(ConfigItem *cfgItem, int pattern)
{
    ConfigItemGainRamp *gainRampItem = qgraphicsitem_cast<ConfigItemGainRamp *>(cfgItem);
    switch(gainRampItem->getCfgType()) {
    case CFG_TYPE_GAINRAMP_1:
        snapToConfig(_gainRamp_1_DataList[pattern], gainRampItem); break;
    case CFG_TYPE_GAINRAMP_2:
        snapToConfig(_gainRamp_2_DataList[pattern], gainRampItem); break;
    case CFG_TYPE_GAINRAMP_4:
        snapToConfig(_gainRamp_4_DataList[pattern], gainRampItem); break;
    case CFG_TYPE_GAINRAMP_8:
        snapToConfig(_gainRamp_8_DataList[pattern], gainRampItem); break;
    case CFG_TYPE_GAINRAMP_16:
        snapToConfig(_gainRamp_16_DataList[pattern], gainRampItem); break;
    case CFG_TYPE_GAINRAMP_32:
        snapToConfig(_gainRamp_32_DataList[pattern], gainRampItem); break;
    default: break;
    }

    gainRampItem->updateCfgSettingLoadedData();
}

void SnapshotSaveGainRampData::saveToSetting(QSettings *configIni, int pattern, CfgType type)
{

}

void SnapshotSaveGainRampData::SettingToSnap(QSettings *, int pattern, CfgType type)
{

}

void SnapshotSaveGainRampData::snapToConfig(QHash<QString, IG_PARACTL_GAINRAMP> data, ConfigItemGainRamp *gainRampItem)
{
    if(!findCfgItemData<IG_PARACTL_GAINRAMP>(data, gainRampItem)) return;
    IG_PARACTL_GAINRAMP body = data.find(gainRampItem->customName()).value();
    gainRampItem->set_targetGainA(body.targetGainA);
    gainRampItem->set_targetGainB(body.targetGainB);
    gainRampItem->set_effectGain(body.effectGain);
    gainRampItem->set_muteTime(body.unmuteT_ms);
    gainRampItem->set_targetTimeA(body.targetAT_ms);
    gainRampItem->set_targetTimeB(body.targetBT_ms);

    quint32 moduleNum = _moduleNum.find(gainRampItem->customName()).value();
    gainRampItem->setModuleNum(moduleNum);
}
