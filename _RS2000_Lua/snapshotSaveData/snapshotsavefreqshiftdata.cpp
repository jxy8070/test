#include "snapshotsavefreqshiftdata.h"

#include "configItem/configitemfreqshift.h"
#include <QHash>

void SnapshotSaveFreqShiftData::saveFreqShiftData(ConfigItem *cfgItem, int type)
{
    ConfigItemFreqShift *freqShiftItem = qgraphicsitem_cast<ConfigItemFreqShift *>(cfgItem);

    _moduleNum.insert(freqShiftItem->customName(), freqShiftItem->getModuleNum());
    if(freqShiftItem->getCfgType() == CFG_TYPE_FREQSHIFT)
    {
        IG_PARACTL_FREQSHIFT body;
        body.shiftfreq_hz = freqShiftItem->GetFreq();
        body.gain = (freqShiftItem->GetGain() + 100) * 100;

        _FreqShift_DataList[type].insert(freqShiftItem->customName(), body);
    }
    if(freqShiftItem->getCfgType() == CFG_TYPE_FREQSHIFT_PLUS)
    {
        IG_PARACTL_FREQSHIFTP body;
        body.shiftfreq_hz = freqShiftItem->GetFreq();
        body.gain = (freqShiftItem->GetGain() + 100) * 100;
        body.phase = (freqShiftItem->GetPhase() + 180);
        body.shiftfreqMode = freqShiftItem->GetFreqMode();
        body.shiftfreqSpan_hz = freqShiftItem->GetFreqSpan();
        body.shiftfreqSpeed_hz = freqShiftItem->GetFreqSpeed();

        _FreqShiftPlus_DataList[type].insert(freqShiftItem->customName(), body);
    }

}

void SnapshotSaveFreqShiftData::setFreqShiftData(ConfigItem *cfgItem, int type)
{
    ConfigItemFreqShift *freqShiftItem = qgraphicsitem_cast<ConfigItemFreqShift *>(cfgItem);

    quint32 moduleNum = _moduleNum.find(freqShiftItem->customName()).value();
    freqShiftItem->setModuleNum(moduleNum);

    if(freqShiftItem->getCfgType() == CFG_TYPE_FREQSHIFT)
    {
        if(!findCfgItemData<IG_PARACTL_FREQSHIFT>(_FreqShift_DataList[type], cfgItem)) return;

        IG_PARACTL_FREQSHIFT body = _FreqShift_DataList[type].find(freqShiftItem->customName()).value();

        freqShiftItem->SetFreq(body.shiftfreq_hz);
        freqShiftItem->SetGain(body.gain *0.01 - 100);

        freqShiftItem->updateCfgSettingLoadedData();
    }
    if(freqShiftItem->getCfgType() == CFG_TYPE_FREQSHIFT_PLUS)
    {
        if(!findCfgItemData<IG_PARACTL_FREQSHIFTP>(_FreqShiftPlus_DataList[type], cfgItem)) return;

        IG_PARACTL_FREQSHIFTP body = _FreqShiftPlus_DataList[type].find(freqShiftItem->customName()).value();

        freqShiftItem->SetFreq(body.shiftfreq_hz);
        freqShiftItem->SetGain(body.gain * 0.01 - 100);
        freqShiftItem->SetPhase(body.phase - 180);
        freqShiftItem->SetFreqMode(body.shiftfreqMode);
        freqShiftItem->SetFreqSpan(body.shiftfreqSpan_hz);
        freqShiftItem->SetFreqSpeed(body.shiftfreqSpeed_hz);

        freqShiftItem->updateCfgSettingLoadedData();
    }

}

void SnapshotSaveFreqShiftData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    if(type == CFG_TYPE_FREQSHIFT) {
        QHash<QString, IG_PARACTL_FREQSHIFT> dataList = _FreqShift_DataList[pattern];

        QList<QString> nameList = dataList.keys();
        int unimoduleNum = dataList.count();
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_FREQSHIFT_TYPE).arg(type));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);

        for(int idx = 0; idx < unimoduleNum; idx++)
        {
            ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_FREQSHIFT).arg(idx));
            quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);

            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQ),
                                dataList[nameList.at(idx)].shiftfreq_hz);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_GAIN),
                                dataList[nameList.at(idx)].gain * 0.01 - 100);

            ConfigIni->endGroup();
        }

        ConfigIni->endGroup();
    }else if(type == CFG_TYPE_FREQSHIFT_PLUS) {
        QHash<QString, IG_PARACTL_FREQSHIFTP> dataList = _FreqShiftPlus_DataList[pattern];

        QList<QString> nameList = dataList.keys();
        int unimoduleNum = dataList.count();
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_FREQSHIFT_TYPE).arg(type));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);

        for(int idx = 0; idx < unimoduleNum; idx++)
        {
            ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_FREQSHIFT).arg(idx));
            quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);

            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQ),
                                dataList[nameList.at(idx)].shiftfreq_hz);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_GAIN),
                                dataList[nameList.at(idx)].gain * 0.01 - 100);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_PHASE),
                                dataList[nameList.at(idx)].phase - 180);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQMODE),
                                dataList[nameList.at(idx)].shiftfreqMode);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQSPAN),
                                dataList[nameList.at(idx)].shiftfreqSpan_hz);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQSPEED),
                                dataList[nameList.at(idx)].shiftfreqSpeed_hz);

            ConfigIni->endGroup();
        }

        ConfigIni->endGroup();
    }
}

void SnapshotSaveFreqShiftData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
//    if(type == CFG_TYPE_FREQSHIFT)
//    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_FREQSHIFT_TYPE).arg(type));
//    }else if(type == CFG_TYPE_FREQSHIFT_PLUS)
//    {
//        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_FREQSHIFT_TYPE).arg(type));
//    }else {
//        return ;
//    }

    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_FREQSHIFT).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();

        uint8_t freq = ConfigIni->value(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQ)).toUInt();
        int8_t gain = ConfigIni->value(QString(CONFIG_DEVICE_DEV_FREQSHIFT_GAIN)).toInt();
        uint16_t phase = ConfigIni->value(QString(CONFIG_DEVICE_DEV_FREQSHIFT_PHASE)).toUInt();
        uint32_t freqmode = ConfigIni->value(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQMODE)).toUInt();
        uint32_t freqspan = ConfigIni->value(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQSPAN)).toUInt();
        uint32_t freqspeed = ConfigIni->value(QString(CONFIG_DEVICE_DEV_FREQSHIFT_FREQSPEED)).toUInt();

        setFreqShiftData(devName, type, pattern, freq, gain, phase,
                               freqmode, freqspan, freqspeed, moduleNum);
        ConfigIni->endGroup();
    }

    ConfigIni->endGroup();
}

void SnapshotSaveFreqShiftData::setFreqShiftData(QString devName, CfgType type, int pattern, uint8_t freq,
         int8_t gain, uint16_t phase, uint32_t freqMode, uint32_t freqSpan, uint32_t freqSpeed, quint32 moduleNum)
{
    if(type == CFG_TYPE_FREQSHIFT)
    {
        IG_PARACTL_FREQSHIFT body;
        body.shiftfreq_hz = freq;
        body.gain = (gain + 100) * 100;

        _FreqShift_DataList[pattern].insert(devName, body);
    }
    if(type == CFG_TYPE_FREQSHIFT_PLUS)
    {
        IG_PARACTL_FREQSHIFTP body;
        body.shiftfreq_hz = freq;
        body.gain = (gain + 100) * 100;
        body.phase = phase + 180;
        body.shiftfreqMode = freqMode;
        body.shiftfreqSpan_hz = freqSpan;
        body.shiftfreqSpeed_hz = freqSpeed;

        _FreqShiftPlus_DataList[pattern].insert(devName, body);
    }

    _moduleNum.insert(devName, moduleNum);
}

