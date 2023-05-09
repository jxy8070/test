#ifndef SNAPSHOTSAVEMIXERDATA_H
#define SNAPSHOTSAVEMIXERDATA_H

#include "configItem/configitemsnapshot.h"
#include "configItem/configitemmixer.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class SnapshotSaveMixerData : public SnapshotSaveObject
{
public:
//    SnapshotSaveMixerData();
    void saveMixerData(ConfigItem *, int);
    void setMixerData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
public:
    enum {
        NODE_SW_L32 = 0,
        NODE_SW_H32,
    };
    enum {
        IN_SELECT = 0,
        OUT_SELECT,
    };

private:
    //mixer4*1
    typedef struct{
        __OPACK32_T2(
            OPACKU,inSwitch,16,				//输入开关，每一位对应一路
            OPACKU,outSwitch,16				//输出开关，每一位对应一路
        );
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }ingain[2];
        struct{
            __OPACK32_T1S0(
                OPACKU,gain,16
            );
        }outgain[1];
        struct{
            __OPACK32_T1S0(
                OPACKU,sw,16				//每一位对应一个输入到输出开关
            );
        }iToOSwitch[1];						//1对应一个输出通道
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }iToOGain[1][2];					//iToOGain[0][0].gainA代表第一个输入通道到第一个输出通道的增益，iToOGain[0][0].gainB代表第二个输入通道到第一个输出通道的增益，
    }IG_PARACTL_VMIXER4X1;

    //mixer4*2
    typedef struct{
        __OPACK32_T2(
            OPACKU,inSwitch,16,				//输入开关，每一位对应一路
            OPACKU,outSwitch,16				//输出开关，每一位对应一路
        );
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }ingain[2];
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }outgain[1];
        struct{
            __OPACK32_T2(
                OPACKU,swA,16,				//每一位对应一个输入到输出开关
                OPACKU,swB,16				//每一位对应一个输入到输出开关
            );
        }iToOSwitch[1];						//1对应一个输出通道应一个输出通道
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }iToOGain[2][2];					//iToOGain[0][0].gainA代表第一个输入通道到第一个输出通道的增益，iToOGain[0][0].gainB代表第二个输入通道到第一个输出通道的增益，
    }IG_PARACTL_VMIXER4X2;

    //mixer4*4
    typedef struct{
        __OPACK32_T2(
            OPACKU,inSwitch,16,				//输入开关，每一位对应一路
            OPACKU,outSwitch,16				//输出开关，每一位对应一路
        );
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }ingain[2];
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }outgain[2];
        struct{
            __OPACK32_T2(
                OPACKU,swA,16,				//每一位对应一个输入到输出开关
                OPACKU,swB,16				//每一位对应一个输入到输出开关
            );
        }iToOSwitch[2];						//1对应一个输出通道应一个输出通道
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }iToOGain[4][2];					//iToOGain[0][0].gainA代表第一个输入通道到第一个输出通道的增益，iToOGain[0][0].gainB代表第二个输入通道到第一个输出通道的增益，
    }IG_PARACTL_VMIXER4X4;

    //mixer8*1
    typedef struct{
        __OPACK32_T2(
            OPACKU,inSwitch,16,				//输入开关，每一位对应一路
            OPACKU,outSwitch,16				//输出开关，每一位对应一路
        );
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }ingain[4];
        struct{
            __OPACK32_T1S0(
                OPACKU,gain,16
            );
        }outgain[1];
        struct{
            __OPACK32_T1S0(
                OPACKU,sw,16				//每一位对应一个输入到输出开关
            );
        }iToOSwitch[1];						//1对应一个输出通道
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }iToOGain[1][4];					//iToOGain[0][0].gainA代表第一个输入通道到第一个输出通道的增益，iToOGain[0][0].gainB代表第二个输入通道到第一个输出通道的增益，
    }IG_PARACTL_VMIXER8X1;

    //mixer8*2
    typedef struct{
        __OPACK32_T2(
            OPACKU,inSwitch,16,				//输入开关，每一位对应一路
            OPACKU,outSwitch,16				//输出开关，每一位对应一路
        );
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }ingain[4];
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }outgain[1];
        struct{
            __OPACK32_T2(
                OPACKU,swA,16,				//每一位对应一个输入到输出开关
                OPACKU,swB,16				//每一位对应一个输入到输出开关
            );
        }iToOSwitch[1];						//1对应一个输出通道应一个输出通道
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }iToOGain[2][4];					//iToOGain[0][0].gainA代表第一个输入通道到第一个输出通道的增益，iToOGain[0][0].gainB代表第二个输入通道到第一个输出通道的增益，
    }IG_PARACTL_VMIXER8X2;

    //mixer8*4
    typedef struct{
        __OPACK32_T2(
            OPACKU,inSwitch,16,				//输入开关，每一位对应一路
            OPACKU,outSwitch,16				//输出开关，每一位对应一路
        );
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }ingain[4];
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }outgain[2];
        struct{
            __OPACK32_T2(
                OPACKU,swA,16,				//每一位对应一个输入到输出开关
                OPACKU,swB,16				//每一位对应一个输入到输出开关
            );
        }iToOSwitch[2];						//1对应一个输出通道应一个输出通道
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }iToOGain[4][4];					//iToOGain[0][0].gainA代表第一个输入通道到第一个输出通道的增益，iToOGain[0][0].gainB代表第二个输入通道到第一个输出通道的增益，
    }IG_PARACTL_VMIXER8X4;

    //mixer8*8
    typedef struct{
        __OPACK32_T2(
            OPACKU,inSwitch,16,				//输入开关，每一位对应一路
            OPACKU,outSwitch,16				//输出开关，每一位对应一路
        );
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }ingain[4];
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }outgain[4];
        struct{
            __OPACK32_T2(
                OPACKU,swA,16,				//每一位对应一个输入到输出开关
                OPACKU,swB,16				//每一位对应一个输入到输出开关
            );
        }iToOSwitch[4];						//1对应一个输出通道应一个输出通道
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }iToOGain[8][4];					//iToOGain[0][0].gainA代表第一个输入通道到第一个输出通道的增益，iToOGain[0][0].gainB代表第二个输入通道到第一个输出通道的增益，
    }IG_PARACTL_VMIXER8X8;

    //mixer16*16
    typedef struct{
        __OPACK32_T2(
            OPACKU,inSwitch,16,				//输入开关，每一位对应一路
            OPACKU,outSwitch,16				//输出开关，每一位对应一路
        );
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }ingain[8];
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }outgain[8];
        struct{
            __OPACK32_T2(
                OPACKU,swA,16,				//每一位对应一个输入到输出开关
                OPACKU,swB,16				//每一位对应一个输入到输出开关
            );
        }iToOSwitch[8];						//1对应一个输出通道应一个输出通道
        struct{
            __OPACK32_T2(
                OPACKU,gainA,16,
                OPACKU,gainB,16
            );
        }iToOGain[16][8];					//iToOGain[0][0].gainA代表第一个输入通道到第一个输出通道的增益，iToOGain[0][0].gainB代表第二个输入通道到第一个输出通道的增益，
    }IG_PARACTL_VMIXER16X16;

    //mixer32*32
    typedef struct{
        struct{
            __OPACK32_T4(
                OPACKU,chNo,8,					//0 - 31
                OPACKU,chType,7,				//0输入,simo有效，1输出，miso有效
                OPACKU,chSw,1,					//通道开关
                OPACKU,chGain,16				//通道增益
            );
        }cfgChInfo;
        union{
            struct{
                unsigned int simoSwitch[1];		//每一位为输入通道cfgCh对应的一路输出开关(节点开关)
                struct{
                    __OPACK32_T2(
                        OPACKU,gainA,16,
                        OPACKU,gainB,16
                    );
                }simoGain[16];					//输入通道cfgCh对应的所有输出的节点增益，simoGain[0].gainA代表输入通道cfgCh到第1个输出通道的增益，simoGain[0].gainB代表输入通道cfgCh到第2个输出通道的增益，
            }simo;
            struct{
                unsigned int misoSwitch[1];		//每一位为输出通道cfgCh对应的一个输入开关(节点开关)
                struct{
                    __OPACK32_T2(
                        OPACKU,gainA,16,
                        OPACKU,gainB,16
                    );
                }misoGain[16];					//输出通道cfgCh对应的所有输入的节点增益，misoGain[0].gainA代表第1个输入通道到输出通道cfgCh的增益，misoGain[0].gainB代表第2个输入通道到输出通道cfgCh的增益，
            }miso;
        };
    }IG_PARACTL_VMIXER32X32;					//传输控制

    //mixer64*64
    typedef struct{
        struct{
            __OPACK32_T4(
                OPACKU,chNo,8,					//0 - 63
                OPACKU,chType,7,				//0输入,simo有效，1输出，miso有效
                OPACKU,chSw,1,					//通道开关
                OPACKU,chGain ,16				//通道增益
            );
        }cfgChInfo;
        union{
            struct{
                unsigned int simoSwitch[2];		//每一位为输入通道cfgCh对应的一路输出开关(节点开关)
                struct{
                    __OPACK32_T2(
                        OPACKU,gainA,16,
                        OPACKU,gainB,16
                    );
                }simoGain[32];					//输入通道cfgCh对应的所有输出的节点增益，simoGain[0].gainA代表输入通道cfgCh到第1个输出通道的增益，simoGain[0].gainB代表输入通道cfgCh到第2个输出通道的增益，
            }simo;
            struct{
                unsigned int misoSwitch[2];		//每一位为输出通道cfgCh对应的一个输入开关(节点开关)
                struct{
                    __OPACK32_T2(
                        OPACKU,gainA,16,
                        OPACKU,gainB,16
                    );
                }misoGain[32];					//输出通道cfgCh对应的所有输入的节点增益，misoGain[0].gainA代表第1个输入通道到输出通道cfgCh的增益，misoGain[0].gainB代表第2个输入通道到输出通道cfgCh的增益，
            }miso;
        };
    }IG_PARACTL_VMIXER64X64;

    struct MixerData{
        //0:低32位，1：高32位
        uint32 _nodeSw[64][2];//节点开关
        uint32 chSwIn[2];
        uint32 chSwOut[2];
        int _nodeGain[64][64];//节点增益
        int _inAndOutGain[64][2];//输入输出通道增益（0,0：输入第一通道增益， 1,0：输出第一通道增益）
//        MixerData() {
//            memset(_nodeSw, 0, 2*64*sizeof(uint32));
//            memset(chSwIn, 0, 2*sizeof(uint32));
//            memset(chSwOut, 0, 2*sizeof(uint32));
//            memset(_nodeGain, 0, 64*64*sizeof(int));
//            memset(_inAndOutGain, 0, 64*2*sizeof(int));
//        }
    };

    struct mixer_x_data{
        union{
            IG_PARACTL_VMIXER4X1 body4x1;
            IG_PARACTL_VMIXER4X2 body4x2;
            IG_PARACTL_VMIXER4X4 body4x4;
            IG_PARACTL_VMIXER8X1 body8x1;
            IG_PARACTL_VMIXER8X2 body8x2;
            IG_PARACTL_VMIXER8X4 body8x4;
            IG_PARACTL_VMIXER8X8 body8x8;
            IG_PARACTL_VMIXER16X16 body16x16;
            MixerData body32x32;
            MixerData body64x64;
        };

        char *_inAndOutLabel[2][64];
    };
private:
    void getInAndOutNum(int &, int &, CfgType);
    QHash<QString, mixer_x_data> getMixerDataList(int , CfgType);
    QString getMixerInAndOutLab(int t, int idx, CfgType type, int pattern, QString devName);
    GainMute getInGainMute(int idx, CfgType type, int pattern, QString devName);
    qint8 getRouterGain(int row, int col, CfgType type, int pattern, QString devName);
    quint32 getRouterInfo(int idx, CfgType type, int pattern, QString devName);
    GainMute getOutGainMute(int idx, CfgType type, int pattern, QString devName);
    void setMixerData(QString devName, CfgType type, int pattern, int inNum, int outNum, GainMute *inGainMute,
                GainMute *outGainMute, int (*routerGain)[64], quint32 (*routerInfo)[2], QString (*inAndOutLab)[64], quint32 moduleNum);
private:

    QHash<QString, mixer_x_data> _mixer_4x1_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, mixer_x_data> _mixer_4x2_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, mixer_x_data> _mixer_4x4_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, mixer_x_data> _mixer_8x1_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, mixer_x_data> _mixer_8x2_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, mixer_x_data> _mixer_8x4_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, mixer_x_data> _mixer_8x8_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, mixer_x_data> _mixer_16x16_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, mixer_x_data> _mixer_32x32_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, mixer_x_data> _mixer_64x64_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVEMIXERDATA_H
