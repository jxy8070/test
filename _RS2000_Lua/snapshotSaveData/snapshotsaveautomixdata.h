#ifndef SNAPSHOTSAVEAUTOMIXDATA_H
#define SNAPSHOTSAVEAUTOMIXDATA_H

#include "configItem/configitemsnapshot.h"
#include "snapshotSaveData/snapshotsaveobject.h"

class SnapshotSaveAutoMixData : public SnapshotSaveObject
{
public:
//    SnapshotSaveAutoMixData();

    void saveAutoMixData(ConfigItem *, int);
    void setAutoMixData(ConfigItem *, int);

    void saveToSetting(QSettings *, int, CfgType);
    void SettingToSnap(QSettings *, int, CfgType);
private:
    /*
     * threadLevel_db = threadLevel * 0.01 - 100;
     * depthenable,无效参数
     * depth_db = depth * 0.01 - 100;
     * noiseMuteEnable，无效参数
     * noiseGain_db = noiseGain * 0.01 - 100;
     * (priorGain_db)preInGain_db = preInGain * 0.01 - 100;	前置增益不放大信号，只是增加信号获得自动增益分配的权重
     * mixOutGain_db = mixOutGain * 0.01 - 100;
     * inMute 1静音的输入通道不参与自动混音,0不静音
     */
    #define IG_PARACTL_AUTOMIXGS_DEF(ch) 	\
    typedef struct{							\
        __OPACK32_T2(						\
            OPACKU,threadLevel,16,			\
            OPACKU,attackT_ms,16			\
        );									\
        __OPACK32_T2(						\
            OPACKU,releaseT_ms,16,			\
            OPACKU,holdT_ms,16				\
        );									\
        __OPACK32_T2(						\
            OPACKU,detectT_ms,16,			\
            OPACKU,depth,16					\
        );									\
        __OPACK32_T1S0(						\
            OPACKU,noiseGain,16				\
        );									\
        struct{								\
            __OPACK32_T2S0S1(				\
                OPACKU,inMute,8,			\
                OPACKU,priorGain,16			\
            );								\
        }inCtrl[ch];						\
        __OPACK32_T2S0S1(					\
            OPACKU,mixOutMute,8,			\
            OPACKU,mixOutGain,16			\
        );									\
    }
    IG_PARACTL_AUTOMIXGS_DEF(4)  IG_PARACTL_AUTOMIXGS4;
    IG_PARACTL_AUTOMIXGS_DEF(8)  IG_PARACTL_AUTOMIXGS8;
    IG_PARACTL_AUTOMIXGS_DEF(16) IG_PARACTL_AUTOMIXGS16;
    IG_PARACTL_AUTOMIXGS_DEF(32) IG_PARACTL_AUTOMIXGS32;

    /*
     * manual	0人控关闭1人控打开
     * postGateMute 1:通道输出静音，0，通道输出受推子控制.推子对音频传输到直通输出和混音输出同时有效果。推子不影响信号是否通过门限
     * postGateGain_db = postGateGain * 0.01 -100;
     * mixOutGain_db = mixOutGain * 0.01 - 100;
     * idGating 未使用，预留
     * noEffectPass	非人控通道无通道满足通过门限的情况：0不通过所有通道，1通过最后一个非人工使能通道，2通过默认通道, LastMicOn
     * defaultch 非人控通道无通道满足通过门限的情况下的默认通道
     * threAboveNoise_db = threAboveNoise * 0.01 - 100; [0,50]
     * depth_db = depth * 0.01 - 100; [0,60]
     * autoMaxPassChs 允许通过非人控最大通道数
     * attenStep_db = attenStep * 0.01 - 100; [0,20]
     * attenMax_db = attenMax * 0.01 - 100; [0,40]
     * attenGain_db = attenGain * 0.01 - 100;
     * attenuationType 0 no attenuation 1attenuationType 2attenuationType
     */
    #define IG_PARACTL_AUTOMIXNGRT_DEF(ch) 	\
    typedef struct{							\
        struct{								\
            __OPACK32_T3S0S1S2(				\
                OPACKU,manual,1,			\
                OPACKU,postGateMute,7,		\
                OPACKU,postGateGain,16		\
            );								\
        }inChAttr[ch];						\
        __OPACK32_T2S0S1(					\
            OPACKU,mixOutMute,8,			\
            OPACKU,mixOutGain,16			\
        );									\
        __OPACK32_T4(						\
            OPACKU,idGating,4,				\
            OPACKU,noEffectPass,4,			\
            OPACKU,defaultch,8,				\
            OPACKU,threAboveNoise,16		\
        );									\
        __OPACK32_T2(						\
            OPACKU,depth,16,				\
            OPACKU,gateHoldT_ms,16			\
        );									\
        __OPACK32_T4(						\
            OPACKU,autoMaxPassChs,8,		\
            OPACKU,directPassAttenEnable,4,	\
            OPACKU,attenuationType,4,		\
            OPACKU,attenResponseT_ms,16		\
        );									\
        __OPACK32_T2(						\
            OPACKU,attenStep,16,			\
            OPACKU,attenMax,16				\
        );									\
    }

    IG_PARACTL_AUTOMIXNGRT_DEF(4)  IG_PARACTL_AUTOMIXNGRT4;
    IG_PARACTL_AUTOMIXNGRT_DEF(8)  IG_PARACTL_AUTOMIXNGRT8;
    IG_PARACTL_AUTOMIXNGRT_DEF(16) IG_PARACTL_AUTOMIXNGRT16;
    IG_PARACTL_AUTOMIXNGRT_DEF(32) IG_PARACTL_AUTOMIXNGRT32;

    /*
     * manual	0人控关闭1人控打开
     * gateMode 0自动模式automatic，超过阈值可打开门限；1优先级模式priority，同属于优先级模式的通道只有最高优先级超过阈值的通道的可打开； 2抢占模式，filibuster同属于抢占模式的信号只有第一个超过阈值的通道的可打开
     * priority 优先级等级(0-1000)越大优先级越高
     * threshold_db = threshold * 0.01 - 100;
     * mixOutGain_db = mixOutGain * 0.01 - 100;
     * depth_db = depth * 0.01 - 100;
     * attenStep_db = attenStep * 0.01 - 100;
     * attenMax_db = attenMax * 0.01 - 100;
     * bgpercentage_f = (bgpercentage * 0.01)%;
     * attenGain_db = attenGain * 0.01 - 100;
     * attenuationType 0 no attenuation 1attenuationType 2attenuationType
     */
    #define IG_PARACTL_AUTOMIXNGAT_DEF(ch) 	\
    typedef struct{							\
        struct{								\
            __OPACK32_T4(					\
                OPACKU,manual,1,			\
                OPACKU,gateMode,3,			\
                OPACKU,priority,12,			\
                OPACKU,threshold,16			\
            );								\
        }inChAttr[ch];						\
        __OPACK32_T2S0S1(					\
            OPACKU,mixOutMute,8,			\
            OPACKU,mixOutGain,16			\
        );									\
        __OPACK32_T3(						\
            OPACKU,lastMicOn,8,				\
            OPACKU,idGating,8,				\
            OPACKU,bgpercentage,16			\
        );									\
        __OPACK32_T2(						\
            OPACKU,depth,16,				\
            OPACKU,gateHoldT_ms,16			\
        );									\
        __OPACK32_T3(						\
            OPACKU,maxPassChs,8,			\
            OPACKU,attenuationType,8,		\
            OPACKU,attenResponseT_ms,16		\
        );									\
        __OPACK32_T2(						\
            OPACKU,attenStep,16,			\
            OPACKU,attenMax,16				\
        );									\
    }

    IG_PARACTL_AUTOMIXNGAT_DEF(4)  IG_PARACTL_AUTOMIXNGAT4;
    IG_PARACTL_AUTOMIXNGAT_DEF(8)  IG_PARACTL_AUTOMIXNGAT8;
    IG_PARACTL_AUTOMIXNGAT_DEF(16) IG_PARACTL_AUTOMIXNGAT16;
    IG_PARACTL_AUTOMIXNGAT_DEF(32) IG_PARACTL_AUTOMIXNGAT32;

private:
    int getInCtrlNum(CfgType);

    template<typename t_Data>
    t_Data getDataFromSettings_GS(t_Data, QSettings *, CfgType);

    template<typename t_Data>
    t_Data getDataFromSettings_NGAT(t_Data, QSettings *, CfgType);

    template<typename t_Data>
    t_Data getDataFromSettings_NGRT(t_Data, QSettings *, CfgType);

    template<typename t_Data>
    void setDataToSettings_GS(t_Data, QSettings *, CfgType);

    template<typename t_Data>
    void setDataToSettings_NGAT(t_Data, QSettings *, CfgType);

    template<typename t_Data>
    void setDataToSettings_NGRT(t_Data, QSettings *, CfgType);
    template<class t_body>
    void setAutoMix_T_Data(QString devName, CfgType type, int pattern, quint32 moduleNum, const t_body body);


    QHash<QString, IG_PARACTL_AUTOMIXGS4>  _AutoMixGS4_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_AUTOMIXGS8>  _AutoMixGS8_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_AUTOMIXGS16> _AutoMixGS16_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_AUTOMIXGS32> _AutoMixGS32_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, IG_PARACTL_AUTOMIXNGRT4>  _AutoMixNGRT4_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_AUTOMIXNGRT8>  _AutoMixNGRT8_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_AUTOMIXNGRT16> _AutoMixNGRT16_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_AUTOMIXNGRT32> _AutoMixNGRT32_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, IG_PARACTL_AUTOMIXNGAT4>  _AutoMixNGAT4_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_AUTOMIXNGAT8>  _AutoMixNGAT8_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_AUTOMIXNGAT16> _AutoMixNGAT16_DataList[ConfigItemSnapshot::DataCount];
    QHash<QString, IG_PARACTL_AUTOMIXNGAT32> _AutoMixNGAT32_DataList[ConfigItemSnapshot::DataCount];

    QHash<QString, quint32> _moduleNum;
};

#endif // SNAPSHOTSAVEAUTOMIXDATA_H
