#include "snapshotsavepriorityduckerdata.h"

#include "configItem/configitempriorityducker.h"

void SnapshotSavePriorityDuckerData::savePriorityDuckerData(ConfigItem *cfgItem, int type)
{
    ConfigItemPriorityDucker *PriortyDuckerItem = qgraphicsitem_cast<ConfigItemPriorityDucker *>(cfgItem);

    _moduleNum.insert(PriortyDuckerItem->customName(), PriortyDuckerItem->getModuleNum());
    IG_PARACTL_PDUCKER body;
    body.attackT_ms  = PriortyDuckerItem->GetAttackTime();
    body.releaseT_ms = PriortyDuckerItem->GetReleaseTime();
    body.holdT_ms    = PriortyDuckerItem->GetHoldTime();
    body.DetectT_ms  = PriortyDuckerItem->GetDetectorTime();
    body.threshhold  = PriortyDuckerItem->GetThresholdLevel();
    body.depth       = PriortyDuckerItem->GetDepth();
    body.priorChInGain  = PriortyDuckerItem->GetInGain();
    body.priorChOutGain = PriortyDuckerItem->GetPriorityGain();
    body.mixOutGain     = PriortyDuckerItem->GetOutGain();
    body.bypass         = PriortyDuckerItem->GetBypass();

    switch(PriortyDuckerItem->getCfgType()) {
    case CFG_TYPE_PDUCKER_4:
        _dyn_ducker_4_DataList[type].insert(PriortyDuckerItem->customName(), body);
        break;
    case CFG_TYPE_PDUCKER_8:
        _dyn_ducker_8_DataList[type].insert(PriortyDuckerItem->customName(), body);
        break;
    case CFG_TYPE_PDUCKER_16:
        _dyn_ducker_16_DataList[type].insert(PriortyDuckerItem->customName(), body);
        break;
    case CFG_TYPE_PDUCKER_32:
        _dyn_ducker_32_DataList[type].insert(PriortyDuckerItem->customName(), body);
        break;
    default:
        break;
    }
}

void SnapshotSavePriorityDuckerData::setPriorityDuckerData(ConfigItem *cfgItem, int type)
{
    ConfigItemPriorityDucker *priorityDuckerItem = qgraphicsitem_cast<ConfigItemPriorityDucker *>(cfgItem);

    quint32 moduleNum = _moduleNum.find(priorityDuckerItem->customName()).value();
    priorityDuckerItem->setModuleNum(moduleNum);

    IG_PARACTL_PDUCKER body;
    switch (priorityDuckerItem->getCfgType()) {
    case CFG_TYPE_PDUCKER_4:
    {
        if(!findCfgItemData<IG_PARACTL_PDUCKER>(_dyn_ducker_4_DataList[type], cfgItem)) return;

        body = _dyn_ducker_4_DataList[type].find(priorityDuckerItem->customName()).value();
        break;
    }
    case CFG_TYPE_PDUCKER_8:
    {
        if(!findCfgItemData<IG_PARACTL_PDUCKER>(_dyn_ducker_8_DataList[type], cfgItem)) return;

        body = _dyn_ducker_8_DataList[type].find(priorityDuckerItem->customName()).value();
        break;
    }
    case CFG_TYPE_PDUCKER_16:
    {
        if(!findCfgItemData<IG_PARACTL_PDUCKER>(_dyn_ducker_16_DataList[type], cfgItem)) return;

        body = _dyn_ducker_16_DataList[type].find(priorityDuckerItem->customName()).value();
        break;
    }
    case CFG_TYPE_PDUCKER_32:
    {
        if(!findCfgItemData<IG_PARACTL_PDUCKER>(_dyn_ducker_32_DataList[type], cfgItem)) return;

        body = _dyn_ducker_32_DataList[type].find(priorityDuckerItem->customName()).value();
        break;
    }
        break;
    default:
        return;
    }

    priorityDuckerItem->SetAttatkTime(body.attackT_ms);
    priorityDuckerItem->SetReleaseTime(body.releaseT_ms);
    priorityDuckerItem->SetHoldTime(body.holdT_ms);
    priorityDuckerItem->SetDetectorTime(body.DetectT_ms);
    priorityDuckerItem->SetThresholdLevel(body.threshhold);
    priorityDuckerItem->SetDepth(body.depth);
    priorityDuckerItem->SetInGain(body.priorChInGain);
    priorityDuckerItem->SetPriorityGain(body.priorChOutGain);
    priorityDuckerItem->SetOutGain(body.mixOutGain);
    priorityDuckerItem->SetBypass(body.bypass);

    priorityDuckerItem->updateCfgSettingLoadedData();
}

void SnapshotSavePriorityDuckerData::saveToSetting(QSettings *, int, CfgType)
{

}

void SnapshotSavePriorityDuckerData::SettingToSnap(QSettings *, int, CfgType)
{

}
