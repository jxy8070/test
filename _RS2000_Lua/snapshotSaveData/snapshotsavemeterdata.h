#ifndef SNAPSHOTSAVEMETERDATA_H
#define SNAPSHOTSAVEMETERDATA_H

#include "configitem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class ConfigItemMeter;
class SnapshotSaveMeterData : public SnapshotSaveObject
{
public:
//    SnapshotSaveMeterData();

    void saveMeterData(ConfigItem *, int);
    void setMeterData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    struct MeterData{
        int _holdTime[8];
        bool _holdEnable[8];
        bool _infiniteHold[8];
        int _meterNum;
        quint32 _rowcol;
        char *_userName[8];
    };

private:
    void snapToConfig(QHash<QString, MeterData>, ConfigItemMeter *);
    QHash<QString, MeterData> getMeterDataList(CfgType type, int pattern);
    void setMeterData(QString devName, CfgType type, int pattern, QString *userName, quint32 moduleNum);
    int getMeterNum(CfgType );

    QHash<QString, MeterData> _meter_1_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, MeterData> _meter_2_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, MeterData> _meter_4_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, MeterData> _meter_8_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVEMETERDATA_H
