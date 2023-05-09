#ifndef SNAPSHOTSAVEFILTEDATA_H
#define SNAPSHOTSAVEFILTEDATA_H

#include "configItem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

enum EqType;
class SnapshotSaveFilteData : public SnapshotSaveObject
{
public:
//    SnapshotSaveFilteData();

    void saveFilteData(ConfigItem *, int);
    void setFilteData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    struct FilterData{
        EqData _eqData;
        quint16 _filterType;
        EqType _eqType;
        bool _bypass;
    };
private:
    QHash<QString, FilterData> getFilterDataList(CfgType type, int pattern);
    void setFilterData(QString devName, CfgType type, int pattern, bool bypass,
                                           quint16 filterType, quint16 freq, qint16 gain, quint32 moduleNum);
private:
    QHash<QString, FilterData> _filter_LowPass_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, FilterData> _filter_HighPass_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, FilterData> _filter_LowShelf_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, FilterData> _filter_HighShelf_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVEFILTEDATA_H
