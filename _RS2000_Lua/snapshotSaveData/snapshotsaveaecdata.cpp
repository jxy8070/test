#include "snapshotsaveaecdata.h"

#include "configItem/configitemaec.h"
#include "configItem/configitemsoftaec.h"

void SnapshotSaveAecData::saveAecData(ConfigItem *cfgItem, int type)
{
    if(cfgItem->getCfgType() == CFG_TYPE_AEC_2_2)
    {
        ConfigItemAEC *aecItem = qgraphicsitem_cast<ConfigItemAEC *>(cfgItem);
        IG_PARACTL_HARDAEC body;
        body.micInGain    = (aecItem->getMicInGain() + 100) * 100;
        body.lineInGain   = (aecItem->getLineInGain() + 100) * 100;
        body.speakOutGain = (aecItem->getSpeakOutGain() + 100) * 100;
        body.lineOutGain  = (aecItem->getLineOutGain() + 100) * 100;
        body.micToSpeakGain = (aecItem->getSpeak2LineGain() + 100) * 100;
        body.micToSpeakSwitch = aecItem->getSpeak2LineSwitch();
        body.NRGain       = (aecItem->getNRGain() + 100) * 100;
        body.NRSwitch     = aecItem->getNRGainSwitch();

        _AEC_DataList[type].insert(aecItem->customName(), body);
        _moduleNum.insert(aecItem->customName(), aecItem->getModuleNum());
    }
    else if(cfgItem->getCfgType() == CFG_TYPE_SOFTAEC_8K128) {
        ConfigItemSoftAEC *softAECItem = qgraphicsitem_cast<ConfigItemSoftAEC *>(cfgItem);
        IG_PARACTL_SOFTAEC body;
        body.micToFarBypass   = softAECItem->getBypass();
        body.micToSpeakGain   = (softAECItem->getMicToSpeakGain() + 100) * 100;
        body.micToSpeakSwitch = softAECItem->getMicToSpeakSwitch();
        body.refGain     = (softAECItem->getRefGain() + 100) * 100;
        body.RESStrength = softAECItem->getRESStrength();
        body.RESSwitch   = softAECItem->getRESSwitch();
        body.NRGain      = (softAECItem->getNRGain() + 100) * 100;
        body.NRSwitch    = softAECItem->getNRSwitch();
        body.CNGain      = (softAECItem->getCNGain() + 100) * 100;
        body.CNSwitch    = softAECItem->getCNSwitch();

        _AEC_8K128_DataList[type].insert(softAECItem->customName(), body);
        _moduleNum.insert(softAECItem->customName(), softAECItem->getModuleNum());
    }
}

void SnapshotSaveAecData::setAecData(ConfigItem *cfgItem, int type)
{
    quint32 moduleNum = _moduleNum.find(cfgItem->customName()).value();
    cfgItem->setModuleNum(moduleNum);

    if(cfgItem->getCfgType() == CFG_TYPE_AEC_2_2)
    {
        if(!findCfgItemData<IG_PARACTL_HARDAEC>(_AEC_DataList[type], cfgItem)) return;

        ConfigItemAEC *aecItem = qgraphicsitem_cast<ConfigItemAEC *>(cfgItem);
        IG_PARACTL_HARDAEC body = _AEC_DataList[type].find(cfgItem->customName()).value();

        aecItem->setMicInGain(body.micInGain * 0.01 - 100);
        aecItem->setLineInGain(body.lineInGain * 0.01 - 100);
        aecItem->setSpeakOutGain(body.speakOutGain * 0.01 - 100);
        aecItem->setLineOutGain(body.lineOutGain * 0.01 - 100);
        aecItem->setSpeak2LineGain(body.micToSpeakGain * 0.01 - 100);
        aecItem->setSpeak2LineSwitch(body.micToSpeakSwitch);
        aecItem->setNRGain(body.NRGain * 0.01 - 100);
        aecItem->setNRGainSwitch(body.NRSwitch);

        aecItem->updateCfgSettingLoadedData();
    }
    else if(cfgItem->getCfgType() == CFG_TYPE_SOFTAEC_8K128) {
        if(!findCfgItemData<IG_PARACTL_SOFTAEC>(_AEC_8K128_DataList[type], cfgItem)) return;
        ConfigItemSoftAEC *softAECItem = qgraphicsitem_cast<ConfigItemSoftAEC *>(cfgItem);
        IG_PARACTL_SOFTAEC body = _AEC_8K128_DataList[type].find(cfgItem->customName()).value();

        softAECItem->setBypass(body.micToFarBypass);
        softAECItem->setRefGain(body.refGain * 0.01 - 100);

        softAECItem->setMicToSpeakGain(body.micToSpeakGain * 0.01 - 100);
        softAECItem->setMicToSpeakSwitch(body.micToSpeakSwitch);

        softAECItem->setRESStrength(body.RESStrength);
        softAECItem->setRESSwitch(body.RESSwitch);

        softAECItem->setNRGain(body.NRGain * 0.01 - 100);
        softAECItem->setNRSwitch(body.NRSwitch);

        softAECItem->setCNGain(body.CNGain * 0.01 - 100);
        softAECItem->setCNSwitch(body.CNSwitch);

        softAECItem->updateCfgSettingLoadedData();
    }
}

void SnapshotSaveAecData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    if(type == CFG_TYPE_AEC_2_2) {
        QList<QString> nameList = _AEC_DataList[pattern].keys();
        int unimoduleNum = _AEC_DataList[pattern].count();
        if(!unimoduleNum) return;
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AEC_TYPE).arg(type));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);

        for(int idx = 0; idx < unimoduleNum; idx++)
        {
            ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AEC).arg(idx));
            quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);

            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_MICINGAIN),
                                _AEC_DataList[pattern][nameList.at(idx)].micInGain * 0.01 - 100);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_LINEINGAIN),
                                _AEC_DataList[pattern][nameList.at(idx)].lineInGain * 0.01 - 100);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_SPEAKOUTGAIN),
                                _AEC_DataList[pattern][nameList.at(idx)].speakOutGain * 0.01 - 100);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_LINEOUTGAIN),
                                _AEC_DataList[pattern][nameList.at(idx)].lineOutGain * 0.01 - 100);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_SPEAK2LINEGAIN),
                                _AEC_DataList[pattern][nameList.at(idx)].micToSpeakGain * 0.01 - 100);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_SPEAK2LINESWITCH),
                                _AEC_DataList[pattern][nameList.at(idx)].micToSpeakSwitch);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_NRGAIN),
                                _AEC_DataList[pattern][nameList.at(idx)].NRGain * 0.01 - 100);
            ConfigIni->endGroup();
        }
        ConfigIni->endGroup();
    }
    else if(type == CFG_TYPE_SOFTAEC_8K128) {
        QList<QString> nameList = _AEC_8K128_DataList[pattern].keys();
        int unimoduleNum = _AEC_8K128_DataList[pattern].count();
        if(!unimoduleNum) return;
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AEC_TYPE).arg(type));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);

        for(int idx = 0; idx < unimoduleNum; idx++) {
            quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
            bool bypass    = _AEC_8K128_DataList[pattern][nameList.at(idx)].micToFarBypass;
            double refGain = _AEC_8K128_DataList[pattern][nameList.at(idx)].refGain * 0.01 - 100;
            double micGain = _AEC_8K128_DataList[pattern][nameList.at(idx)].micToSpeakGain * 0.01 - 100;
            bool micSwitch = _AEC_8K128_DataList[pattern][nameList.at(idx)].micToSpeakSwitch;
            int resStrength= _AEC_8K128_DataList[pattern][nameList.at(idx)].RESStrength;
            bool resSwitch = _AEC_8K128_DataList[pattern][nameList.at(idx)].RESSwitch;
            double NRGain  = _AEC_8K128_DataList[pattern][nameList.at(idx)].NRGain * 0.01 - 100;
            bool NRSwitch  = _AEC_8K128_DataList[pattern][nameList.at(idx)].NRSwitch;
            double CNGain  = _AEC_8K128_DataList[pattern][nameList.at(idx)].CNGain * 0.01 - 100;
            bool CNSwitch  = _AEC_8K128_DataList[pattern][nameList.at(idx)].CNSwitch;

            ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AEC).arg(idx));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_BYPASS), bypass);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_REFGAIN), refGain);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_MICTOSPEAK), micGain);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_MICENABLE), micSwitch);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_RESPERCENT), resStrength);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_RESENABLE), resSwitch);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_NRGAIN), NRGain);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_NRENABLE), NRSwitch);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_CNGAIN), CNGain);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AEC_8K128_CNENABLE), CNSwitch);
            ConfigIni->endGroup();
        }

        ConfigIni->endGroup();
    }
}

void SnapshotSaveAecData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AEC_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AEC).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();

        if(type == CFG_TYPE_AEC_2_2) {
            int micInGain    = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_MICINGAIN)).toInt();
            int lineInGain   = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_LINEINGAIN)).toInt();
            int speakOutGain = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_SPEAKOUTGAIN)).toInt();
            int lineOutGain  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_LINEOUTGAIN)).toInt();
            int micToSpeakGain = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_SPEAK2LINEGAIN)).toInt();
            int micToSpeakSwitch = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_SPEAK2LINESWITCH)).toInt();
            int NRGain       = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_NRGAIN)).toInt();

            IG_PARACTL_HARDAEC body;
            body.micInGain    = (micInGain + 100) * 100;
            body.lineInGain   = (lineInGain + 100) * 100;
            body.speakOutGain = (speakOutGain + 100) * 100;
            body.lineOutGain  = (lineOutGain + 100) * 100;
            body.micToSpeakGain = (micToSpeakGain + 100) * 100;
            body.micToSpeakSwitch = micToSpeakSwitch;
            body.NRGain       = (NRGain + 100) * 100;

            _AEC_DataList[pattern].insert(devName, body);
        }
        else if(type == CFG_TYPE_SOFTAEC_8K128) {
            bool bypass    = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_BYPASS)).toBool();
            double refGain = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_REFGAIN)).toDouble();
            double micGain = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_MICTOSPEAK)).toDouble();
            bool micSwitch = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_MICENABLE)).toBool();
            int resStrength= ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_RESPERCENT)).toInt();
            bool resSwitch = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_RESENABLE)).toBool();
            double NRGain  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_NRGAIN)).toDouble();
            bool NRSwitch  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_NRENABLE)).toBool();
            double CNGain  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_CNGAIN)).toDouble();
            bool CNSwitch  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AEC_8K128_CNENABLE)).toBool();

            IG_PARACTL_SOFTAEC body;
            body.micToFarBypass   = bypass;
            body.micToSpeakGain   = (micGain + 100) * 100;
            body.micToSpeakSwitch = micSwitch;
            body.refGain     = (refGain + 100) * 100;
            body.RESStrength = resStrength;
            body.RESSwitch   = resSwitch;
            body.NRGain      = (NRGain + 100) * 100;
            body.NRSwitch    = NRSwitch;
            body.CNGain      = (CNGain + 100) * 100;
            body.CNSwitch    = CNSwitch;
            _AEC_8K128_DataList[pattern].insert(devName, body);
        }
        _moduleNum.insert(devName, moduleNum);

        ConfigIni->endGroup();
    }
    ConfigIni->endGroup();
}
