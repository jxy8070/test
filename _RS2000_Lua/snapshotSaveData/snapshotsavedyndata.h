#ifndef SNAPSHOTSAVEDYNDATA_H
#define SNAPSHOTSAVEDYNDATA_H

#include "configitem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class SnapshotSaveDynData : public SnapshotSaveObject
{
public:
//    SnapshotSaveDynData();

    void saveDynData(ConfigItem *, int);
    void setDynData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    typedef struct{
        __OPACK32_T2(
            OPACKU,threshhold,16,
            OPACKU,ratio,16					//expand:oiratio = ratio*0.01f;comp,auto:oiratio = 1.0f/ratio
        );
        __OPACK32_T2(
            OPACKU,responseT_ms,16,
            OPACKU,releaseT_ms,16
        );
        __OPACK32_T2(
            OPACKU,eGain,16,
            OPACKU,oTargetGain,16
        );
        __OPACK32_T1S0(
            OPACKU,bypass,8
        );
    }IG_PARACTL_DYN;

private:
    QHash<QString, IG_PARACTL_DYN> getDynDataList(CfgType type, int pattern);
    double getDynThreshhold(CfgType type, int pattern, QString devName);
    double getDynRadio(CfgType type, int pattern, QString devName);
    double getDynGain(CfgType type, int pattern, QString devName);
    double getDynAttack(CfgType type, int pattern, QString devName);
    double getDynRelease(CfgType type, int pattern, QString devName);
    bool   getDynBypass(CfgType type, int pattern, QString devName);
    void setDynData(QString devName, CfgType type, int pattern, double threshhold,
                           double ratio, double gain, double resonseT_ms, double relaseT_ms, bool bypass, quint32 moduleNum);
private:

    QHash<QString, IG_PARACTL_DYN> _dyn_Auto_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_DYN> _dyn_Comp_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_DYN> _dyn_Limit_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_DYN> _dyn_NoiseGate_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_DYN> _dyn_Expand_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_DYN> _dyn_Peak_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;

};

#endif // SNAPSHOTSAVEDYNDATA_H
