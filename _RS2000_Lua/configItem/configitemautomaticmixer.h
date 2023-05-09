#ifndef CONFIGITEMAUTOMATICMIXER_H
#define CONFIGITEMAUTOMATICMIXER_H
#include "configitem.h"

class CfgSettingAutomaticMixerGainSharing;
class CfgSettingAutomaticMixerGatingAbsoluteThreshold;
class CfgSettingAutomaticMixerGatingRelativeThreshold;
class ConfigItemAutomaticMixer : public ConfigItem
{
public:    
    enum {
        INPUT_CH1 = 0,
        INPUT_CH2,
        INPUT_CH3,
        INPUT_CH4,
        INPUT_CH5,
        INPUT_CH6,
        INPUT_CH7,
        INPUT_CH8,
        INPUT_CH9,
        INPUT_CH10,
        INPUT_CH11,
        INPUT_CH12,
        INPUT_CH13,
        INPUT_CH14,
        INPUT_CH15,
        INPUT_CH16,
    };

    ConfigItemAutomaticMixer(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemAutomaticMixer();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    inline int getMaxChannel() { return _maxChannel; }

public:
    int8_t GetThreadLevel(void);
    uint16_t GetAttackTime(void);
    uint16_t GetReleaseTime(void);
    uint16_t GetHoldTime(void);
    uint16_t GetDetectorTime(void);
//    uint8_t GetDepthEnable(void);
    int16_t GetDepthValue(void);
//    uint8_t GetNoiseEnable(void);
    int8_t GetNoiseValue(void);
    uint8_t GetInputMute(int ch);
    int8_t GetPreInGain(int ch);
    uint8_t GetMixOutMute(void);
    int16_t GetMixOutGain(void);

    uint8_t GetChManual(int ch);
    uint8_t GetChMode(int ch);
    uint16_t GetChPriority(int ch);
    int16_t GetChThreshold(int ch);
    uint8_t GetChPostGateMute(int ch);
    int16_t GetChPostGateGain(int ch);
    uint8_t GetLastMicOn(void);
    uint8_t GetIDGating(void);
    float GetBackgroundPercentage(void);
    uint8_t GetDefaultCh(void);
    int16_t GetThresholdLevelAboveNoise(void);
    uint8_t GetMaxMON(void);
    uint8_t GetDirectOutsNOMAtten(void);
    uint8_t GetNOMAttenuationType(void);
    int16_t GetNOMAttenuationStep(void);
    int16_t GetMaxNOMAttenuation(void);
    uint16_t GetResponseTime(void);

    void SetThreadLevel(int8_t value);
    void SetAttackTime(uint16_t value);
    void SetReleaseTime(uint16_t value);
    void SetHoldTime(uint16_t value);
    void SetDetectorTime(uint16_t value);
//    void SetDepthEnable(uint8_t enable);
    void SetDepthValue(int16_t value);
//    void SetNoiseEnable(uint8_t enable);
    void SetNoiseValue(int8_t value);
    void SetInputMute(int ch, uint8_t enable);
    void SetPreInGain(int ch, int8_t value);
    void SetMixOutMute(uint8_t enable);
    void SetMixOutGain(int16_t value);

    void SetChManual(int ch, uint8_t enable);
    void SetChMode(int ch, uint8_t enable);
    void SetChPriority(int ch, uint16_t enable);
    void SetChThreshold(int ch, int16_t value);
    void SetChPostGateMute(int ch, uint8_t enable);
    void SetChPostGateGain(int ch, int16_t value);
    void SetLastMicOn(uint8_t enable);
    void SetIDGating(uint8_t enable);
    void SetBackgroundPercentage(float value);
    void SetDefaultCh(uint8_t ch);
    void SetThresholdLevelAboveNoise(int16_t value);
    void SetMaxMON(uint8_t max);
    void SetDirectOutsNOMAtten(uint8_t enable);
    void SetNOMAttenuationType(uint8_t type);
    void SetNOMAttenuationStep(int16_t value);
    void SetMaxNOMAttenuation(int16_t value);
    void SetResponseTime(uint16_t value);

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

    IG_PARACTL_AUTOMIXGS_DEF(4) IG_PARACTL_AUTOMIXGS4;
    IG_PARACTL_AUTOMIXGS4 _body_s4;

    IG_PARACTL_AUTOMIXGS_DEF(8) IG_PARACTL_AUTOMIXGS8;
    IG_PARACTL_AUTOMIXGS8 _body_s8;

    IG_PARACTL_AUTOMIXGS_DEF(16) IG_PARACTL_AUTOMIXGS16;
    IG_PARACTL_AUTOMIXGS16 _body_s16;

    IG_PARACTL_AUTOMIXGS_DEF(32) IG_PARACTL_AUTOMIXGS32;
    IG_PARACTL_AUTOMIXGS32 _body_s32;
    CfgSettingAutomaticMixerGainSharing *_setting_gainsharing;

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

    IG_PARACTL_AUTOMIXNGRT_DEF(4) IG_PARACTL_AUTOMIXNGRT4;
    IG_PARACTL_AUTOMIXNGRT4 _body_grt4;

    IG_PARACTL_AUTOMIXNGRT_DEF(8) IG_PARACTL_AUTOMIXNGRT8;
    IG_PARACTL_AUTOMIXNGRT8 _body_grt8;

    IG_PARACTL_AUTOMIXNGRT_DEF(16) IG_PARACTL_AUTOMIXNGRT16;
    IG_PARACTL_AUTOMIXNGRT16 _body_grt16;

    IG_PARACTL_AUTOMIXNGRT_DEF(32)	IG_PARACTL_AUTOMIXNGRT32;
    IG_PARACTL_AUTOMIXNGRT32 _body_grt32;
    CfgSettingAutomaticMixerGatingRelativeThreshold *_setting_gatingRelativeThreshold;

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

    IG_PARACTL_AUTOMIXNGAT_DEF(4) IG_PARACTL_AUTOMIXNGAT4;
    IG_PARACTL_AUTOMIXNGAT4 _body_gat4;

    IG_PARACTL_AUTOMIXNGAT_DEF(8) IG_PARACTL_AUTOMIXNGAT8;
    IG_PARACTL_AUTOMIXNGAT8 _body_gat8;

    IG_PARACTL_AUTOMIXNGAT_DEF(16) IG_PARACTL_AUTOMIXNGAT16;
    IG_PARACTL_AUTOMIXNGAT16 _body_gat16;

    IG_PARACTL_AUTOMIXNGAT_DEF(32)	IG_PARACTL_AUTOMIXNGAT32;
    IG_PARACTL_AUTOMIXNGAT32 _body_gat32;
    CfgSettingAutomaticMixerGatingAbsoluteThreshold *_setting_gatingAbsoluteThreshold;

    int _maxChannel;
};

#endif // CONFIGITEMAUTOMATICMIXER_H
