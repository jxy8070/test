#ifndef SNAPSHOTSAVEGAINDATA_H
#define SNAPSHOTSAVEGAINDATA_H
#include "snapshotsaveobject.h"
#include "configItem/configitemsnapshot.h"

class ConfigItemGain;
class SnapshotSaveGainData : public SnapshotSaveObject
{
public:
//    SnapshotSaveGainData();

    void saveGainData(ConfigItem *, int);
    void setGainData(ConfigItem *, int);
    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    typedef struct{
        __OPACK32_T3S0S1S2(
            OPACKU,tsMode,4,			//2bypass,1静音,0正常输出
            OPACKU,antiPhase,4,			//1反相，0正向
            OPACKU,gain,16				//1开，0关,gain_db = gain * 0.01 - 100;
        );
    }IG_PARACTL_GAIN;

private:
    void snapToConfig(QHash<QString, IG_PARACTL_GAIN> data, ConfigItemGain *gainItem);
    void configToSnap(QHash<QString, IG_PARACTL_GAIN>& data, ConfigItemGain *gainItem);

    QHash<QString, IG_PARACTL_GAIN> _gain_1_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_GAIN> _gain_2_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_GAIN> _gain_4_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_GAIN> _gain_8_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_GAIN> _gain_16_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_GAIN> _gain_32_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, qint32> _moduleNum;
};

#endif // SNAPSHOTSAVEGAINDATA_H
