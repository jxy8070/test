#ifndef SNAPSHOTSAVENFCDATA_H
#define SNAPSHOTSAVENFCDATA_H

#include "configitem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class ConfigItemNFC;
class SnapshotSaveNfcData : public SnapshotSaveObject
{
public:
//    SnapshotSaveNfcData();

    void saveNfcData(ConfigItem *, int);
    void setNfcData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    typedef struct{
        __OPACK32_T4(
            OPACKU,outGain,16,							//outGain_dbfs = outGain * 0.01 - 100;
            OPACKU,outMode,4,							//0commen，1bypass，2mute
            OPACKU,clearfilter,4,						//0。filter work;1.clear dymatic filter;2.clear all filter;
            OPACKU,bandwidth,8							//[4,128]
        );
        __OPACK32_T3(
            OPACKU,fixfilters,8,
            OPACKU,dynfilters,8,
            OPACKU,maxNotchDepth,16						//maxNotchDepth_dbfs = maxNotchDepth * 0.01 - 100;
        );
    }IG_PARACTL_NFC;

private:
    void snapToConfig(QHash<QString, IG_PARACTL_NFC>, ConfigItemNFC *);
    QHash<QString, IG_PARACTL_NFC> getNFCDataList(CfgType, int);

    QHash<QString, IG_PARACTL_NFC> _NFC_8_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_NFC> _NFC_16_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_NFC> _NFC_24_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_NFC> _NFC_32_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVENFCDATA_H
