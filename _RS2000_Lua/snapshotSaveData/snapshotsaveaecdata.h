#ifndef SNAPSHOTSAVEAECDATA_H
#define SNAPSHOTSAVEAECDATA_H

#include "configItem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class SnapshotSaveAecData : public SnapshotSaveObject
{
public:
//    SnapshotSaveAecData();

    void saveAecData(ConfigItem *, int);
    void setAecData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    typedef struct{
        __OPACK32_T2(
            OPACKU,micInGain,16,						//gain_db = micIngain * 0.01 - 100;
            OPACKU,lineInGain,16
        );
        __OPACK32_T2(
            OPACKU,speakOutGain,16,
            OPACKU,lineOutGain,16
        );
        __OPACK32_T2S0S1(
            OPACKU,micToSpeakGain,16,
            OPACKU,micToSpeakSwitch,8
        );
        __OPACK32_T2S0S1(
          OPACKU,NRGain,16,
          OPACKU,NRSwitch,8
         );
    }IG_PARACTL_HARDAEC;

    //AEC-8k128
    typedef struct{
        __OPACK32_T3(
            OPACKU,micToSpeakGain,16,
            OPACKU,micToSpeakSwitch,8,
            OPACKU,micToFarBypass,8
        );
        __OPACK32_T3(
            OPACKU,refGain,16,
            OPACKU,RESStrength,8,		//(0~100)
            OPACKU,RESSwitch,8			//residual echo suppress
        );
        __OPACK32_T2S0S1(				//noise reduce
            OPACKU,NRGain,16,
            OPACKU,NRSwitch,8
        );
        __OPACK32_T2S0S1(				//comfort Noise
            OPACKU,CNGain,16,
            OPACKU,CNSwitch,8
        );
        unsigned int remain[2];
    }IG_PARACTL_SOFTAEC;


    QHash<QString, IG_PARACTL_HARDAEC> _AEC_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_SOFTAEC> _AEC_8K128_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVEAECDATA_H
