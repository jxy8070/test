#ifndef SNAPSHOTSAVEGPIDATA_H
#define SNAPSHOTSAVEGPIDATA_H

#include "configItem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class SnapshotSaveGpiData : public SnapshotSaveObject
{
public:
//    SnapshotSaveGpiData();

    void saveGpiData(ConfigItem *, int);
    void setGpiData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    QHash<QString, quint32> getGpiDataList(CfgType type, int pattern);
    void setGpiData(QString devName, CfgType type, int pattern, quint32 gpiIdx, quint32 moduleNum);

    QHash<QString, quint32> _Gpi_idx_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVEGPIDATA_H
