#ifndef CONFIGITEMMIXER_H
#define CONFIGITEMMIXER_H

#include "configitem.h"

struct GainMute
{
    GainMute() {gain = 0; mute = false;}
    double gain;
    bool mute;
};

class CfgSettingMixer;
class CfgSettingMixer_Kemu;
class ConfigItemMixer : public ConfigItem
{
    Q_OBJECT
    Q_PROPERTY(bool NodesSwitch MEMBER _NodesSwitch NOTIFY nodesSwitchChanged_signal);

public:
    ConfigItemMixer(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemMixer();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    QByteArray getOnlineData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

public:
    quint32 getRouterInfo(int idx);
    bool getRouterMute(int row, int col);
    qint8  getRouterGain(int row, int col);
    GainMute getInGainMute(int idx);
    GainMute getOutGainMute(int idx);
    quint16 getInNum();
    quint16 getOutNum();

    void setRouterInfo(int idx, quint32 vlu, quint32 vlu_h = 0);
    void setRouterInfo(int row, int col, bool isChacked);
    void setRouterGain(int row, int col, qint8 value);
    void setInGainMute(int idx, double gain, bool mute);
    void setOutGainMute(int idx, double gain, bool mute);

    void createLabelTextList();
    QString getLabText(int bOutput, int idx);
    void setLabText(int bOutput, int idx, QString text);
    inline void setDisplayRange(int idx, int value) { _displayRange[idx] = value; }
    inline int getDisplayRange(int idx) { return _displayRange[idx]; }
signals:
    void nodesSwitchChanged_signal();
public:
    void nodesSwitchChanged_slot();
    inline bool getNodesSwitch() { return _NodesSwitch; }
//    inline void setNodesSwitch(bool enable) { _NodesSwitch = enable; }
private:
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
    }IG_PARACTL_VMIXER64X64;					//传输控制

    IG_PARACTL_VMIXER4X1 _body_4x1;
    IG_PARACTL_VMIXER4X2 _body_4x2;
    IG_PARACTL_VMIXER4X4 _body_4x4;
    IG_PARACTL_VMIXER8X1 _body_8x1;
    IG_PARACTL_VMIXER8X2 _body_8x2;
    IG_PARACTL_VMIXER8X4 _body_8x4;
    IG_PARACTL_VMIXER8X8 _body_8x8;
    IG_PARACTL_VMIXER16X16 _body_16x16;
    IG_PARACTL_VMIXER32X32 _body_32x32;
    IG_PARACTL_VMIXER64X64 _body_64x64;

    QStringList _inLabel;
    QStringList _outLabel;
    int _displayRange[4];

private:
    enum {
        NODE_SW_L32 = 0,
        NODE_SW_H32,
    };
    //0:低32位，1：高32位
    uint32 _nodeSw[64][2];//节点开关
    uint32 chSwIn[2];
    uint32 chSwOut[2];

    enum {
        IN_SELECT = 0,
        OUT_SELECT,
    };
    int _inAndOutGain[64][2];//输入输出通道增益（0,0：输入第一通道增益， 1,0：输出第一通道增益）

    enum {
        COL_SELECT = 0,
        ROW_SELECT,
    };
    int _nodeGain[64][64];//节点增益
    void prepare32BodyData(int idx, int order);
    void prepare64BodyData(int idx, int order);

    void setDev_32_Data();
    void setDev_64_Data();

    CfgSettingMixer *_setting  = NULL;
    CfgSettingMixer_Kemu *_setting_Kemu = NULL;
    bool _NodesSwitch;  //是否启用节点开关（此选项关闭节点增益调节）
};

#endif // CONFIGITEMMIXER_H
