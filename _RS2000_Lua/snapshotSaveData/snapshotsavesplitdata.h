#ifndef SNAPSHOTSAVESPLITDATA_H
#define SNAPSHOTSAVESPLITDATA_H

#include "configItem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class SnapshotSaveSplitData : public SnapshotSaveObject
{
public:
//    SnapshotSaveSplitData();
    void saveSplitData(ConfigItem *, int);
    void setSplitData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    struct ItemSplitStr {
        struct {
            bool mute;
            double level;
        }Input;
        struct {
            struct {
                bool setSyncL;
                bool setSyncR;
                int filterTypeL;
                int filterTypeR;
                double freqL;
                double freqR;

                bool setSyncLEnable;
                bool setSyncREnable;
                bool filterTypeLEnable;
                bool filterTypeREnable;
                bool freqLEnable;
                bool freqREnable;
            }Filter;
            struct {
                bool mute;
                bool invert;
                double level;
            }Output;
        } Segment[4];
    };
private:
    QHash<QString, ItemSplitStr> getSplitDataList(CfgType type, int pattern);
    void setSplitData(QString devName, CfgType type, int pattern, double inLevel, bool inMute, bool *setSyncL, bool *setSyncR,
              int *filterTypeL, int *filterTypeR, double *freqL, double *freqR, bool *mute, bool *invert, double *level, quint32 moduleNum);

private:
    QHash<QString, ItemSplitStr> _split_2_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, ItemSplitStr> _split_3_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, ItemSplitStr> _split_4_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVESPLITDATA_H
