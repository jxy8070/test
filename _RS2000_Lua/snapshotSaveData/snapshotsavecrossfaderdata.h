#ifndef SNAPSHOTSAVECROSSFADERDATA_H
#define SNAPSHOTSAVECROSSFADERDATA_H

#include "snapshotsaveobject.h"
#include "configItem/configitemsnapshot.h"

class ConfigItemCrossfader;
class SnapshotSaveCrossfaderData : public SnapshotSaveObject
{
public:
//    SnapshotSaveCrossfaderData();

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);

    void saveCrossfaderData(ConfigItem *, int);
    void setCrossfaderData(ConfigItem *, int);

private:
    typedef struct{
        __OPACK32_T3(
            OPACKU,chBandSwT_ms,24,				//通道组切换时间
            OPACKU,targetChBand,4,				//0,通道组A;1通道组B
            OPACKU,sumType,4					//0,Gain;1power
        );
    }IG_PARACTL_CROSSFADER;

    void snapToConfig(QHash<QString, IG_PARACTL_CROSSFADER> data, ConfigItemCrossfader *cfdItem);
private:
    QHash<QString, IG_PARACTL_CROSSFADER> _crossfader_2x1_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_CROSSFADER> _crossfader_4x2_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_CROSSFADER> _crossfader_8x4_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_CROSSFADER> _crossfader_16x8_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_CROSSFADER> _crossfader_32x16_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, qint32> _moduleNum;
};

#endif // SNAPSHOTSAVECROSSFADERDATA_H
