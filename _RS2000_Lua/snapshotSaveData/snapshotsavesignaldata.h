#ifndef SNAPSHOTSAVESIGNALDATA_H
#define SNAPSHOTSAVESIGNALDATA_H

#include "configitem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class SnapshotSaveSignalData : public SnapshotSaveObject
{
public:
//    SnapshotSaveSignalData();
    void saveSignalData(ConfigItem *, int);
    void setSignalData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    struct SignalData{
        bool _mute;
        double _level;
        double _freq;
    };
private:
    void setSignalData(QString devName, CfgType type, int pattern, bool mute,
                                           double freq, double level, quint32 moduleNum);
    QHash<QString, SignalData> getSignalDataList(CfgType type, int pattern);

    QHash<QString, SignalData> _signal_White_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, SignalData> _signal_Sine_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, SignalData> _signal_Pink_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVESIGNALDATA_H
