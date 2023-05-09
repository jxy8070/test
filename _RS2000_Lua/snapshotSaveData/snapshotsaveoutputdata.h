#ifndef SNAPSHOTSAVEOUTPUTDATA_H
#define SNAPSHOTSAVEOUTPUTDATA_H

#include "snapshotsaveobject.h"
#include "configItem/configitemsnapshot.h"
class ConfigItemOutput;
class SnapshotSaveOutputData : public SnapshotSaveObject
{
    Q_OBJECT
public:
    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);

    void saveOutputData(ConfigItem *cfgItem, int pattern);
    void setOutputData(ConfigItem *cfgItem, int pattern);

private:
    typedef struct{
        __OPACK32_T3S0S1S2(
            OPACKU,mute,4,				//1静音，0正常输出
            OPACKU,antiPhase,4,			//1反相，0正向
            OPACKU,gain,16				//1开，0关,gain_db = gain * 0.01 - 100;
        );
    }IG_PARACTL_OUTPUT1;
    typedef struct{
        unsigned int mute;					//1静音，0正常输出
        unsigned int antiPhase;				//1反相，0正向
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16				//gain_db = gain * 0.01 - 100;
            );
        }gain[1];
    }IG_PARACTL_OUTPUT2;
    typedef struct{
        unsigned int mute;					//1静音，0正常输出
        unsigned int antiPhase;				//1反相，0正向
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16				//gain_db = gain * 0.01 - 100;
            );
        }gain[2];
    }IG_PARACTL_OUTPUT4;
    typedef struct{
        unsigned int mute;					//1静音，0正常输出
        unsigned int antiPhase;				//1反相，0正向
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16				//gain_db = gain * 0.01 - 100;
            );
        }gain[4];
    }IG_PARACTL_OUTPUT8;
    typedef struct{
        unsigned int mute;					//1静音，0正常输出
        unsigned int antiPhase;				//1反相，0正向
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16				//gain_db = gain * 0.01 - 100;
            );
        }gain[8];
    }IG_PARACTL_OUTPUT16;

    typedef struct Output_NewData{
        int s_ch;
        IG_PARACTL_OUTPUT16 s_body;
        char *s_userLab[16];
        Output_NewData() {
            memset(&s_ch, 0, sizeof(int));
            memset(&s_body, 0, sizeof(IG_PARACTL_OUTPUT16));
            memset(s_userLab, 0, 4*sizeof(char *));
        }
    }Output_NewData_t;

    template<typename T_Value>
    void setDataToSetting(QHash<QString, T_Value> dataList, QSettings *ConfigIni, int type);
    template<typename T_Value>
    void getDataFromSetting(T_Value& body, QSettings *ConfigIni,int pattern, int type);
    int getchannelNum(int type);
private:
    QHash<QString, Output_NewData_t> _output_4_DataList[ConfigItemSnapshot::DataCount];
};

#endif // SNAPSHOTSAVEOUTPUTDATA_H
