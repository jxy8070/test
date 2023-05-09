#ifndef SNAPSHOTSAVEGAINRAMPDATA_H
#define SNAPSHOTSAVEGAINRAMPDATA_H

#include "configItem/configitemsnapshot.h"
#include "snapshotsaveobject.h"

class ConfigItemGainRamp;
class SnapshotSaveGainRampData : public SnapshotSaveObject
{
public:
//    SnapshotSaveGainRampData();
    void saveGainRampData(ConfigItem *, int);
    void setGainRampData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);

private:
    typedef struct{
        __OPACK32_T2(
            OPACKU,targetAT_ms,16,
            OPACKU,targetBT_ms,16
        );
        __OPACK32_T2S0S1(
            OPACKU,unmuteT_ms,16,
            OPACKU,effectGain,8							//0 mute ,1 targetGainA ,2 targetGainB
        );
        __OPACK32_T2(
            OPACKU,targetGainA,16,						//targetGainA_dbfs = targetGainA * 0.01 - 100;
            OPACKU,targetGainB,16						//targetGainA_dbfs = targetGainA * 0.01 - 100;
        );
    }IG_PARACTL_GAINRAMP;

private:
    void snapToConfig(QHash<QString, IG_PARACTL_GAINRAMP> data, ConfigItemGainRamp *gainRampItem);

    QHash<QString, IG_PARACTL_GAINRAMP> _gainRamp_1_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_GAINRAMP> _gainRamp_2_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_GAINRAMP> _gainRamp_4_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_GAINRAMP> _gainRamp_8_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_GAINRAMP> _gainRamp_16_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_GAINRAMP> _gainRamp_32_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVEGAINRAMPDATA_H
