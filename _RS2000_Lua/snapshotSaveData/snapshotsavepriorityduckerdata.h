#ifndef SNAPSHOTSAVEPRIORITYDUCKERDATA_H
#define SNAPSHOTSAVEPRIORITYDUCKERDATA_H

#include "configItem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class SnapshotSavePriorityDuckerData : public SnapshotSaveObject
{
public:
//    SnapshotSavePriorityDuckerData();

    void savePriorityDuckerData(ConfigItem *, int);
    void setPriorityDuckerData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    typedef struct{
        __OPACK32_T2(
            OPACKU,attackT_ms,16,
            OPACKU,releaseT_ms,16
        );
        __OPACK32_T2(
            OPACKU,holdT_ms,16,
            OPACKU,DetectT_ms,16
        );
        __OPACK32_T2(
            OPACKU,threshhold,16,						//threshhold_dbfs = threshhold * 0.01 - 100;
            OPACKU,depth,16								//depth_dbfs = depth * 0.01 - 100;
        );
        __OPACK32_T2(
            OPACKU,priorChInGain,16,					//priorChInGain_dbfs = priorChInGain * 0.01 - 100;
            OPACKU,priorChOutGain,16					//priorChInGain_dbfs = priorChInGain * 0.01 - 100;
        );
        __OPACK32_T2S0S1(
            OPACKU,mixOutGain,16,
            OPACKU,bypass,8
        );
    }IG_PARACTL_PDUCKER;

    QHash<QString, IG_PARACTL_PDUCKER> _dyn_ducker_4_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_PDUCKER> _dyn_ducker_8_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_PDUCKER> _dyn_ducker_16_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_PDUCKER> _dyn_ducker_32_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVEPRIORITYDUCKERDATA_H
