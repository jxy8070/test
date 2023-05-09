#ifndef SNAPSHOTSAVEEQDATA_H
#define SNAPSHOTSAVEEQDATA_H

#include "configItem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class ConfigItemEq;
class SnapshotSaveEqData : public SnapshotSaveObject
{
public:
//    SnapshotSaveEqData();
    void saveEqData(ConfigItem *, int);
    void setEqData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    typedef struct{
        quint32 _smooth;
        quint32 _bypess;
        QList<EqData> _eqList;
    }EqModuleData;

private:
    void snapToConfig(QHash<QString, EqModuleData>, ConfigItemEq *);
    QHash<QString, EqModuleData> getEqDataList(CfgType type, int pattern);
    void setEqData(QString devName, CfgType type, int pattern, bool bypass, QList<EqData> eqDataList, quint32 moduleNum);


    QHash<QString, EqModuleData> _Eq_1_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, EqModuleData> _Eq_3_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, EqModuleData> _Eq_5_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, EqModuleData> _Eq_7_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, EqModuleData> _Eq_10_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, EqModuleData> _Eq_16_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, EqModuleData> _Eq_1x3_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, EqModuleData> _Eq_2x3_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, EqModuleData> _Eq_3x3_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVEEQDATA_H
