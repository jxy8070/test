#ifndef SNAPSHOTSAVEDELAYDATA_H
#define SNAPSHOTSAVEDELAYDATA_H

#include <QObject>
#include <QSettings>
#include "snapshotSaveData/snapshotsaveobject.h"
#include "configItem/configitemsnapshot.h"



///1.00ms、5.00ms、10.00ms不可用

class SnapshotSaveObject;
class SnapshotSaveDelayData : public SnapshotSaveObject
{
public:
//    SnapshotSaveDelayData(ConfigItem *);

    void saveDelayData(ConfigItem *, int);
    void setDelayData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    typedef struct{
        __OPACK32_T2S0S1(
            OPACKU,delaySwitch,8,		//1开，0关
            OPACKU,delayT_ms,16			//延时
        );
    }IG_PARACTL_DELAY;

    QHash<QString, IG_PARACTL_DELAY> _delayDataList[ConfigItemSnapshot::DataCount];
    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVEDELAYDATA_H
