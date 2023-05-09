#include "snapshotsavecrossfaderdata.h"
#include "configItem/configitemcrossfader.h"


void SnapshotSaveCrossfaderData::saveToSetting(QSettings *, int, CfgType)
{

}

void SnapshotSaveCrossfaderData::SettingToSnap(QSettings *, int, CfgType)
{

}

void SnapshotSaveCrossfaderData::saveCrossfaderData(ConfigItem *cfgItem, int pattern)
{
    ConfigItemCrossfader *cfdItem = qgraphicsitem_cast<ConfigItemCrossfader *>(cfgItem);
    IG_PARACTL_CROSSFADER body;
    body.chBandSwT_ms = cfdItem->get_time();
    body.targetChBand = cfdItem->get_ch();
    body.sumType      = cfdItem->get_type();
    switch(cfdItem->getCfgType()) {
    case CFG_TYPE_CROSSFADER_2X1:
        _crossfader_2x1_DataList[pattern].insert(cfdItem->customName(), body);
        _moduleNum.insert(cfdItem->customName(), cfdItem->getModuleNum());
        break;
    case CFG_TYPE_CROSSFADER_4X2:
        _crossfader_4x2_DataList[pattern].insert(cfdItem->customName(), body);
        _moduleNum.insert(cfdItem->customName(), cfdItem->getModuleNum());
        break;
    case CFG_TYPE_CROSSFADER_8X4:
        _crossfader_8x4_DataList[pattern].insert(cfdItem->customName(), body);
        _moduleNum.insert(cfdItem->customName(), cfdItem->getModuleNum());
        break;
    case CFG_TYPE_CROSSFADER_16X8:
        _crossfader_16x8_DataList[pattern].insert(cfdItem->customName(), body);
        _moduleNum.insert(cfdItem->customName(), cfdItem->getModuleNum());
        break;
    case CFG_TYPE_CROSSFADER_32X16:
        _crossfader_32x16_DataList[pattern].insert(cfdItem->customName(), body);
        _moduleNum.insert(cfdItem->customName(), cfdItem->getModuleNum());
        break;
    default:
        break;
    }
}

void SnapshotSaveCrossfaderData::setCrossfaderData(ConfigItem *cfgItem, int pattern)
{
    ConfigItemCrossfader *cfdItem = qgraphicsitem_cast<ConfigItemCrossfader *>(cfgItem);
    switch(cfdItem->getCfgType()) {
    case CFG_TYPE_CROSSFADER_2X1:
        snapToConfig(_crossfader_2x1_DataList[pattern], cfdItem); break;
    case CFG_TYPE_CROSSFADER_4X2:
        snapToConfig(_crossfader_4x2_DataList[pattern], cfdItem); break;
    case CFG_TYPE_CROSSFADER_8X4:
        snapToConfig(_crossfader_8x4_DataList[pattern], cfdItem); break;
    case CFG_TYPE_CROSSFADER_16X8:
        snapToConfig(_crossfader_16x8_DataList[pattern], cfdItem); break;
    case CFG_TYPE_CROSSFADER_32X16:
        snapToConfig(_crossfader_32x16_DataList[pattern], cfdItem); break;
    default: break;
    }

    cfdItem->updateCfgSettingLoadedData();
}

void SnapshotSaveCrossfaderData::snapToConfig(QHash<QString, IG_PARACTL_CROSSFADER> data, ConfigItemCrossfader *cfdItem)
{
    if(!findCfgItemData<IG_PARACTL_CROSSFADER>(data, cfdItem)) return;

    IG_PARACTL_CROSSFADER body = data.find(cfdItem->customName()).value();
    cfdItem->set_time(body.chBandSwT_ms);
    cfdItem->set_ch(body.targetChBand);
    cfdItem->set_type(body.sumType);
    qint32 moduleNum = _moduleNum.find(cfdItem->customName()).value();
    cfdItem->setModuleNum(moduleNum);
}
