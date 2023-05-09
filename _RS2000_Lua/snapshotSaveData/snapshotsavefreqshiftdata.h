#ifndef SNAPSHOTSAVEFREQSHIFTDATA_H
#define SNAPSHOTSAVEFREQSHIFTDATA_H

#include "configItem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class SnapshotSaveFreqShiftData : public SnapshotSaveObject
{
public:
//    SnapshotSaveFreqShiftData();

    void saveFreqShiftData(ConfigItem *, int);
    void setFreqShiftData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    typedef struct {
        __OPACK32_T2(
            OPACKU,shiftfreq_hz,16,				//hz，
            OPACKU,gain,16						//gain_db = gain * 0.01 - 100;
        );
    } IG_PARACTL_FREQSHIFT;

    typedef struct{
      __OPACK32_T2(
        OPACKU,shiftfreq_hz,16,        //hz，
        OPACKU,phase,16            //[0,360]realphase = phase - 180;
      );
      __OPACK32_T1S0(
        OPACKU,gain,16            //gain_db = gain * 0.01 - 100;
      );
      __OPACK32_T3(
          OPACKU,shiftfreqSpan_hz,16,      //hz，[0,2*shiftfreq_hz]
          OPACKU,shiftfreqSpeed_hz,8,      //hz[1,100];
          OPACKU,shiftfreqMode,8        //hz[1,100];
        );
    }IG_PARACTL_FREQSHIFTP;
private:
    void setFreqShiftData(QString devName, CfgType type, int pattern, uint8_t freq,
             int8_t gain, uint16_t phase, uint32_t freqMode, uint32_t freqSpan, uint32_t freqSpeed, quint32 moduleNum);


    QHash<QString, IG_PARACTL_FREQSHIFT>  _FreqShift_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_FREQSHIFTP> _FreqShiftPlus_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVEFREQSHIFTDATA_H
