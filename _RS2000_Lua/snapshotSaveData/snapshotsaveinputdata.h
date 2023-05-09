#ifndef SNAPSHOTSAVEINPUTDATA_H
#define SNAPSHOTSAVEINPUTDATA_H

#include "snapshotSaveData/snapshotsaveobject.h"
#include "configItem/configitemsnapshot.h"

class ConfigItemInput;
class SnapshotSaveInputData : public SnapshotSaveObject
{
public:
    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);

    void saveInputData(ConfigItem *, int);
    void setInputData(ConfigItem *, int);
signals:

private:

    typedef struct{
        __OPACK32_T3S0S1S2(
            OPACKU,mute,4,				//1静音，0正常输出
            OPACKU,antiPhase,4,			//1反相，0正向
            OPACKU,gain,16				//1开，0关,gain_db = gain * 0.01 - 100;
        );
    }IG_PARACTL_INPUT1;
    typedef struct{
        unsigned int mute;					//1静音，0正常输出
        unsigned int antiPhase;				//1反相，0正向
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16				//gain_db = gain * 0.01 - 100;
            );
        }gain[1];
    }IG_PARACTL_INPUT2;
    typedef struct{
        unsigned int mute;					//1静音，0正常输出
        unsigned int antiPhase;				//1反相，0正向
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16				//gain_db = gain * 0.01 - 100;
            );
        }gain[2];
    }IG_PARACTL_INPUT4;
    typedef struct{
        unsigned int mute;					//1静音，0正常输出
        unsigned int antiPhase;				//1反相，0正向
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16				//gain_db = gain * 0.01 - 100;
            );
        }gain[4];
    }IG_PARACTL_INPUT8;
    typedef struct{
        unsigned int mute;					//1静音，0正常输出
        unsigned int antiPhase;				//1反相，0正向
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16				//gain_db = gain * 0.01 - 100;
            );
        }gain[8];
    }IG_PARACTL_INPUT16;

    typedef struct Input_NewData{
        int ch = 0;
        bool phantom[16] = {0};
        int preampGain[16] = {0};
        char *userLab[16] = {0};
        IG_PARACTL_INPUT16 body_4;
        Input_NewData() {
            memset(&body_4, 0, sizeof(IG_PARACTL_INPUT16));
        }
    }Input_NewData_t;

    void saveInput_2_16_Data(ConfigItemInput *inputItem, int pattern);
    void setInput_2_16_data(ConfigItemInput *inputItem, int pattern);

    template<typename T_Value>
    void setDataToSetting(QHash<QString, T_Value> dataList, QSettings *ConfigIni, int type);
    template<typename T_Value>
    void getDataFromSetting(T_Value& body, QSettings *ConfigIni,int pattern, int type);

    int getchannelNum(int type);
private:
    QHash<QString, Input_NewData_t> _input_4_DataList[ConfigItemSnapshot::DataCount];

};

#endif // SNAPSHOTSAVEINPUTDATA_H
