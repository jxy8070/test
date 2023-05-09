#ifndef PACKCOMMON_H
#define PACKCOMMON_H

#include "common.h"
#include "eqpacket.h"
#include <QString>
#define TRN_IN_CH_NUM               32
#define TRN_OUT_CH_NUM              32
#define TRN_ANA_IN_NUM              16
class QDataStream;

struct SigFlowDef16
{
    quint16 layer;
    quint16 row;
};

struct SigFlowDef
{
    quint8 layer;
    quint8 row;
};

//struct OutputDef //小端对齐
//{
////    quint8 temp;
////    quint8 channel;
//    quint8 layer;
//    quint8 row;
//};

struct DynData_Net;
struct DynData{
    friend QDataStream& operator<<(QDataStream& stream, const DynData& str);
    friend QDataStream& operator>>(QDataStream& stream, DynData& str);
    quint16 atk;
    quint16 release;
    quint16 nRatio;
    quint16 ratio;
    qint16 th1;
    qint16 th2;
    qint16 offset1;
    qint16 offset2;
    bool bypass;
    DynData_Net getNetPack();
};

struct DynData_Net{
    quint16 atk;
    quint16 release;
    quint16 nRatio;
    quint16 ratio;
    qint16 th1;
    qint16 th2;
    qint16 offset1;
    qint16 offset2;
    bool bypass;
    DynData getDataPack();
};

struct ConfigData_Trn_Net;
struct ConfigData_Trn
{
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_Trn& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_Trn& data);

    quint8 CurrentSceneNum;
    qint8 InMicGain[TRN_ANA_IN_NUM];
    quint8 InMic48V[TRN_ANA_IN_NUM];
    quint8 NetworkMode[32];

    void initConfigData();
    ConfigData_Trn_Net getNetPack();
} ;

struct ConfigData_Trn_Net
{
    quint8 CurrentSceneNum;
    qint8 InMicGain[TRN_ANA_IN_NUM];
    quint8 InMic48V[TRN_ANA_IN_NUM];
    quint8 NetworkMode[32];

    ConfigData_Trn getDataPack();
} ;

struct NetworkInfo{
    unsigned char valid; //接口是否有效
    unsigned char static_ip;
    unsigned char mac[6];
    unsigned int ip;
    QString getIp();
    QString getMac();
    bool getStaticIP();
    bool getValid();
} ;

struct ADD_NetInfo{
    unsigned int software_version[2];
    NetworkInfo dante;
} ;

#define AUTOMIX_MAXCHANNEL      256
#define METER_MAXCHANNEL        100
typedef struct {
    __OPACK32_T4S0S1S2S3(
        OPACKU,modeUnitNum,8,
        OPACKU,channelNum,7,
        OPACKU,effect,1,
        OPACKU,modeUnitNumU,8
    );
    __OPACK32_T2(
        OPACKU,vu,16,
        OPACKU,ppm,16
    );
} FC_RTU_METER8Bit;

typedef struct {
    __OPACK32_T3S0S1S2(
        OPACKU,modeUnitNum,16,
        OPACKU,channelNum,7,
        OPACKU,effect,1
    );
    __OPACK32_T2(
        OPACKU,vu,16,
        OPACKU,ppm,16
    );
} FC_RTU_METER, FC_RTU_METER16Bit;


typedef struct {
    __OPACK32_T4(
        OPACKU,modeUnitNum,8,
        OPACKU,channelNum,7,
        OPACKU,effect,1,
        OPACKU,state,16     //0:停止 1:播放中 2:暂停 -1:文件错误
    );
    __OPACK32_T2(
        OPACKU,playedTime,16,   //时间:秒
        OPACKU,remainsTime,16
    );
} FC_RTU_PLAYINFO;


typedef struct {
    __OPACK32_T1S3(
        OPACKU,startIdx,16      //开始序号
    );
    __OPACK32_T2(
        OPACKU,stopIdx,16,       //结束序号
        OPACKU,totalCnt,16       //总数
    );

    unsigned char fileNames[1024];
} FC_RTU_PLAYFILES;


typedef struct {
    unsigned char vu_in[64];
    unsigned char vu_out[64];
} FC_RTU_InOutLevel;

typedef struct {
    unsigned char vu_in[16+128+2];
    unsigned char vu_out[16+128+2];
    unsigned char gpi[8];
    unsigned char gpo[8];
    quint16 adc[4];
} FC_RTU_InOutLevel128;



struct ConfigData_Transctrl_x86Server_128_128_Net;
struct ConfigData_Transctrl_x86Server_128_128 {
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_Transctrl_x86Server_128_128& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_Transctrl_x86Server_128_128& data);
    quint8 mixer[128][128];

    void initConfigData();
    ConfigData_Transctrl_x86Server_128_128_Net getNetPack(quint8 bank);
};
struct ConfigData_Transctrl_x86Server_128_128_Net{
    quint8 bank;
    quint8 mixer[8][128];

    ConfigData_Transctrl_x86Server_128_128 getDataPack(ConfigData_Transctrl_x86Server_128_128 &dataPack);
};

struct MsgData_Trn
{
    MsgData_Trn() { memset(this, 0, sizeof(MsgData_Trn));}
    union {
        ConfigData_Trn_Net  config; 			//0x10

        qint8 InMicGain[TRN_IN_CH_NUM];		//0x1A
        quint8 InMic48V[TRN_ANA_IN_NUM];			//0x1B

        ADD_NetInfo NetInfo; //设备网络状态信息 0x20
        quint8 DeviceName[20];
        quint8 SceneNum;
        FC_RTU_METER8Bit meter8Bit[METER_MAXCHANNEL];
        FC_RTU_METER16Bit meter16Bit[METER_MAXCHANNEL];
        FC_RTU_PLAYFILES playFiles;
        FC_RTU_InOutLevel inOutLevel;
        FC_RTU_InOutLevel128 inOutLevel128;
        unsigned char file_data[1024];
        unsigned int para_data[256];

        struct {
            unsigned char transctrl_Major;      //主控固件版本号
            unsigned char transctrl_Minor;
            unsigned char transctrl_Build;
            unsigned char transctrl_HardwareV;  //与主控相关硬件号
            unsigned char dsp_Version[8];       //0~3:dsp固件版本号;  【2022-11-21】4:dsp相关硬件号; 5:dsp内部厂家号; 6、7:保留
        } _heartbeat;

        ConfigData_Transctrl_x86Server_128_128_Net _x86Server_128_128;
        struct Transctrl_x86Server_Mixer {
            quint32 out;
            quint32 in;
            quint8 on;
        } _x86Server_Mixer;
    };
};

struct SpkEq{
    quint8 ch;
    EqData Eq;
};

struct SpkDyn{
    quint8 ch;
    DynData dyn;
};

struct SpkGain{
    quint8 ch;
    qint16 gain;
};

struct ControlValue {
    friend QDataStream& operator<<(QDataStream& stream, const ControlValue& data);
    friend QDataStream& operator>>(QDataStream& stream, ControlValue& data);
    qint8 volume;
    bool phase;
    bool mute;
    qint8 gain;
    quint16 delay;
    quint8 channel;
};

struct CommonData_Spk
{
    friend QDataStream& operator<<(QDataStream& stream, const CommonData_Spk& data);
    friend QDataStream& operator>>(QDataStream& stream, CommonData_Spk& data);
    bool power;
    quint8 ch;
    struct ControlValue value;
};

typedef CommonData_Spk CommonData_Amp;

struct DividerData{
    quint16 q;//filter mode
    quint16 freq;
    bool bypass;
    qint16 gain;//filter order
};

struct Divider{
    friend QDataStream& operator<<(QDataStream& stream, const Divider& data);
    friend QDataStream& operator>>(QDataStream& stream, Divider& data);
    DividerData hf;
    DividerData lf;
};

struct ConfigData_Spk_Net;
struct ConfigData_Spk
{
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_Spk& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_Spk& data);
    quint8 Switch;//0:0ff		1: on
    ControlValue ch1_ControlValue;//8
    EqData4B ch1_Eq;//26
    DynData ch1_Dyn;//18
    Divider ch1_divider;//16
    quint8 volumeCurrentType;

    void initConfigData();
    ConfigData_Spk_Net getNetPack();
};

struct ConfigData_Spk_Net
{
    quint8 Switch;//0:0ff		1: on
    ControlValue ch1_ControlValue;//8
    EqData4B_Net ch1_Eq;//26
    DynData ch1_Dyn;//18
    Divider ch1_divider;//16
    quint8 volumeCurrentType;

    ConfigData_Spk getDataPack();
};
struct ConfigData_STM32_Gain {
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_STM32_Gain& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_STM32_Gain& data);
    int8_t vol;
    uint8_t mute;
};

struct ConfigData_Xover {
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_Xover& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_Xover& data);
    uint8_t bypass;
    uint8_t stages[2];
    short freq[2];
};

struct ConfigData_STM32_POE_0_1_Net;
struct ConfigData_STM32_POE_0_1{
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_STM32_POE_0_1& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_STM32_POE_0_1& data);
    struct ConfigData_STM32_Gain AD_gain;
    EqData4B AD_Eq;
    DynData AD_Dyn;
    void initConfigData();
    ConfigData_STM32_POE_0_1_Net getNetPack();
};

struct ConfigData_STM32_POE_0_1_Net{
    struct ConfigData_STM32_Gain AD_gain;
    EqData4B_Net AD_Eq;
    DynData AD_Dyn;
    ConfigData_STM32_POE_0_1 getDataPack();
};

struct ConfigData_STM32_POE_1_0_Net;
struct ConfigData_STM32_POE_1_0{
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_STM32_POE_1_0& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_STM32_POE_1_0& data);
    void initConfigData();
    ConfigData_STM32_POE_1_0_Net getNetPack();

    uint8_t phantom;
    int8_t Mic_Gain;
    struct ConfigData_Xover x;
    DynData Dyn;
    EqData4B Eq;
};
struct ConfigData_STM32_POE_1_0_Net{
    ConfigData_STM32_POE_1_0 getDataPack();

    uint8_t phantom;
    int8_t Mic_Gain;
    struct ConfigData_Xover x;
    DynData Dyn;
    EqData4B_Net Eq;
};

struct ConfigData_STM32_POE_0_2_Net;
struct ConfigData_STM32_POE_0_2{
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_STM32_POE_0_2& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_STM32_POE_0_2& data);
    struct ConfigData_STM32_Gain AD_gain[2];
    EqData4B AD_Eq[2];
    DynData AD_Dyn;
    void initConfigData();
    ConfigData_STM32_POE_0_2_Net getNetPack();
};

struct ConfigData_STM32_POE_0_2_Net{
    struct ConfigData_STM32_Gain AD_gain[2];
    EqData4B_Net AD_Eq[2];
    DynData AD_Dyn;
    ConfigData_STM32_POE_0_2 getDataPack();
};

struct MsgData_STM32_POE{
    MsgData_STM32_POE() { memset(this, 0, sizeof(MsgData_STM32_POE));}
    union {
        struct {
            uint8_t ch;
            union {
                uint8_t u8Value;
                int8_t Mic_Gain;
                struct ConfigData_STM32_Gain gain;
                EqData4B_Net eq;
                DynData dyn;
                struct ConfigData_Xover x;
            } _u;
        } _online;
        struct {
            int A_In;
            int N_Out;
            uint8_t phantom;
        } _display;
        ConfigData_STM32_POE_0_1_Net InitData_0_1;
        ConfigData_STM32_POE_1_0_Net InitData_1_0;
        ConfigData_STM32_POE_0_2_Net InitData_0_2;
    };
};

struct ConfigData_RT1052_Gain {
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Gain& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Gain& data);
    int8_t vol;
    uint8_t mute;
    uint8_t phase;
};

struct ConfigData_RT1052_Mixer_4_8 {
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Mixer_4_8& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Mixer_4_8& data);
    uint8_t m[4][8];
};

struct ConfigData_RT1052_Mixer_4_4 {
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Mixer_4_4& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Mixer_4_4& data);
    uint8_t m[4][4];
};

struct ConfigData_RT1052_Mixer_2_2 {
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Mixer_2_2& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Mixer_2_2& data);
    uint8_t m[2][2];
};

struct ConfigData_RT1052_Delay {
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Delay& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Delay& data);
    uint16_t delay;
};

struct ConfigData_Amp4_4_route {
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_Amp4_4_route& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_Amp4_4_route& data);
    uint8_t r[4];
};

struct ConfigData_AP400_Route {
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_AP400_Route& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_AP400_Route& data);
    uint8_t r[1];
};

struct ConfigData_Amp4_4_Net;
struct ConfigData_Amp4_4{
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_Amp4_4& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_Amp4_4& data);

    struct ConfigData_RT1052_Gain channel_gain[8];
    EqData4B channel_eq[4];
    struct ConfigData_RT1052_Mixer_4_8 Aout_mixer;
    struct ConfigData_Xover Aout_xover[4];
    EqData4B Aout_eq[4];
    struct ConfigData_RT1052_Gain Aout_gain[4];
    DynData Aout_dyn[4];
    struct ConfigData_RT1052_Delay Aout_delay[4];
    struct ConfigData_Amp4_4_route Nout_route;

    void initConfigData();
    ConfigData_Amp4_4_Net getNetPack();
};

struct ConfigData_Amp4_4_Net{
    struct ConfigData_RT1052_Gain channel_gain[8];
    EqData4B_Net channel_eq[4];
    struct ConfigData_RT1052_Mixer_4_8 Aout_mixer;
    struct ConfigData_Xover Aout_xover[4];
    EqData4B_Net Aout_eq[4];
    struct ConfigData_RT1052_Gain Aout_gain[4];
    DynData Aout_dyn[4];
    struct ConfigData_RT1052_Delay Aout_delay[4];
    struct ConfigData_Amp4_4_route Nout_route;

    ConfigData_Amp4_4 getDataPack();
};

struct ConfigData_RT1052_Poe_60W_2_4_Net;
struct ConfigData_RT1052_Poe_60W_2_4{
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Poe_60W_2_4& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Poe_60W_2_4& data);

    DynData channel_dyn;
    struct ConfigData_RT1052_Gain channel_gain[3];
    EqData4B channel_eq[3];
    struct ConfigData_RT1052_Mixer_2_2 Aout_mixer;
    struct ConfigData_Xover Aout_xover[2];
    EqData15B Aout_eq[2];
    struct ConfigData_RT1052_Gain Aout_gain[2];
    DynData Aout_dyn[2];
    struct ConfigData_RT1052_Delay Aout_delay[2];
    struct ConfigData_Xover Nout_xover;
#ifdef Logo_BeiJingHuiSheng
    uint8_t mixer_mode;
#endif
    void initConfigData();
    ConfigData_RT1052_Poe_60W_2_4_Net getNetPack();
};

struct ConfigData_RT1052_Poe_60W_2_4_Net{
    DynData channel_dyn;
    struct ConfigData_RT1052_Gain channel_gain[3];
    EqData4B_Net channel_eq[3];
    struct ConfigData_RT1052_Mixer_2_2 Aout_mixer;
    struct ConfigData_Xover Aout_xover[2];
    EqData15B_Net Aout_eq[2];
    struct ConfigData_RT1052_Gain Aout_gain[2];
    DynData Aout_dyn[2];
    struct ConfigData_RT1052_Delay Aout_delay[2];
    struct ConfigData_Xover Nout_xover;
#ifdef Logo_BeiJingHuiSheng
    uint8_t mixer_mode;
#endif

    ConfigData_RT1052_Poe_60W_2_4 getDataPack();
};

struct ConfigData_RT1052_Poe_AP60_Bluetooth_Net;
struct ConfigData_RT1052_Poe_AP60_Bluetooth{
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_Poe_AP60_Bluetooth& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_Poe_AP60_Bluetooth& data);

    struct ConfigData_RT1052_Gain channel_gain[3];
    EqData4B channel_eq[3];
    struct ConfigData_Xover Aout_xover[2];
    EqData15B Aout_eq[2];
    struct ConfigData_RT1052_Gain Aout_gain[2];
    DynData Aout_dyn[2];
    struct ConfigData_RT1052_Delay Aout_delay[2];
    struct ConfigData_Xover Nout_xover;

    void initConfigData();
    ConfigData_RT1052_Poe_AP60_Bluetooth_Net getNetPack();
};

struct ConfigData_RT1052_Poe_AP60_Bluetooth_Net{
    struct ConfigData_RT1052_Gain channel_gain[3];
    EqData4B_Net channel_eq[3];
    struct ConfigData_Xover Aout_xover[2];
    EqData15B_Net Aout_eq[2];
    struct ConfigData_RT1052_Gain Aout_gain[2];
    DynData Aout_dyn[2];
    struct ConfigData_RT1052_Delay Aout_delay[2];
    struct ConfigData_Xover Nout_xover;

    ConfigData_RT1052_Poe_AP60_Bluetooth getDataPack();
};

struct ConfigData_RT1052_AP400_Net;
struct ConfigData_RT1052_AP400{
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_RT1052_AP400& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_RT1052_AP400& data);

    struct ConfigData_RT1052_Gain channel_gain[3];
    EqData4B channel_eq[3];
    struct ConfigData_Xover Nout_xover;

    struct ConfigData_AP400_Route route;

    struct ConfigData_Xover Aout_xover[8];
    EqData15B Aout_eq[8];
    struct ConfigData_RT1052_Gain Aout_gain[8];
    DynData Aout_dyn[8];
    struct ConfigData_RT1052_Delay Aout_delay[8];

    int8_t directivity;
	
#ifdef Logo_BeiJingHuiSheng
	uint8_t route_mode;
    int8_t threshold;
    uint8_t default_route;
    uint8_t auto_recovery;
#endif

    void initConfigData();
    ConfigData_RT1052_AP400_Net getNetPack();
};

struct ConfigData_RT1052_AP400_Net{
    struct ConfigData_RT1052_Gain channel_gain[3];
    EqData4B_Net channel_eq[3];
    struct ConfigData_Xover Nout_xover;

    struct ConfigData_AP400_Route route;

    struct ConfigData_Xover Aout_xover[8];
    EqData15B_Net Aout_eq[8];
    struct ConfigData_RT1052_Gain Aout_gain[8];
    DynData Aout_dyn[8];
    struct ConfigData_RT1052_Delay Aout_delay[8];

    int8_t directivity;

#ifdef Logo_BeiJingHuiSheng
    uint8_t route_mode;
    int8_t threshold;
    uint8_t default_route;
    uint8_t auto_recovery;
#endif
    ConfigData_RT1052_AP400 getDataPack();
};

struct ConfigData_Mic_Quad_Net;
struct ConfigData_Mic_Quad {
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_Mic_Quad& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_Mic_Quad& data);
    quint8 gain[2];
    quint8 phantom[4];
    struct ConfigData_RT1052_Gain N_gain[4];
    struct ConfigData_Xover N_xover[4];
    DynData N_dyn[4];
    EqData4B N_eq[4];
    struct ConfigData_RT1052_Mixer_4_4 N_mixer;
    struct ConfigData_RT1052_Gain NOut_gain[4];

    void initConfigData();
    ConfigData_Mic_Quad_Net getNetPack();
};
struct ConfigData_Mic_Quad_Net{
    quint8 gain[2];
    quint8 phantom[4];
    struct ConfigData_RT1052_Gain N_gain[4];
    struct ConfigData_Xover N_xover[4];
    DynData N_dyn[4];
    EqData4B_Net N_eq[4];
    struct ConfigData_RT1052_Mixer_4_4 N_mixer;
    struct ConfigData_RT1052_Gain NOut_gain[4];

    ConfigData_Mic_Quad getDataPack();
};

struct ConfigData_Amp_2_0_Mic_Line_Net;
struct ConfigData_Amp_2_0_Mic_Line {
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_Amp_2_0_Mic_Line& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_Amp_2_0_Mic_Line& data);
    quint8 mic_open;
    quint8 mic_gain;
    quint8 mic_phantom;
    struct ConfigData_RT1052_Gain gain[2];
    struct ConfigData_Xover xover[2];
    DynData dyn[2];
    EqData4B eq[2];
    struct ConfigData_RT1052_Mixer_2_2 Nout_mixer;
    struct ConfigData_RT1052_Gain Nout_gain[2];

    void initConfigData();
    ConfigData_Amp_2_0_Mic_Line_Net getNetPack();
};
struct ConfigData_Amp_2_0_Mic_Line_Net{
    quint8 mic_open;
    quint8 mic_gain;
    quint8 mic_phantom;
    struct ConfigData_RT1052_Gain gain[2];
    struct ConfigData_Xover xover[2];
    DynData dyn[2];
    EqData4B_Net eq[2];
    struct ConfigData_RT1052_Mixer_2_2 Nout_mixer;
    struct ConfigData_RT1052_Gain Nout_gain[2];

    ConfigData_Amp_2_0_Mic_Line getDataPack();
};

struct ConfigData_Amp_0_2_Line_Net;
struct ConfigData_Amp_0_2_Line{
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_Amp_0_2_Line& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_Amp_0_2_Line& data);

    struct ConfigData_RT1052_Gain channel_gain[2];
    struct ConfigData_RT1052_Mixer_2_2 Aout_mixer;
    struct ConfigData_Xover Aout_xover[2];
    EqData4B Aout_eq[2];
    struct ConfigData_RT1052_Gain Aout_gain[2];
    DynData Aout_dyn[2];
    struct ConfigData_RT1052_Delay Aout_delay[2];

    void initConfigData();
    ConfigData_Amp_0_2_Line_Net getNetPack();
};

struct ConfigData_Amp_0_2_Line_Net{
    struct ConfigData_RT1052_Gain channel_gain[2];
    struct ConfigData_RT1052_Mixer_2_2 Aout_mixer;
    struct ConfigData_Xover Aout_xover[2];
    EqData4B_Net Aout_eq[2];
    struct ConfigData_RT1052_Gain Aout_gain[2];
    DynData Aout_dyn[2];
    struct ConfigData_RT1052_Delay Aout_delay[2];

    ConfigData_Amp_0_2_Line getDataPack();
};

struct ConfigData_Monitor_SPK_Net;
struct ConfigData_Monitor_SPK{
    friend QDataStream& operator<<(QDataStream& stream, const ConfigData_Monitor_SPK& data);
    friend QDataStream& operator>>(QDataStream& stream, ConfigData_Monitor_SPK& data);
//    quint8 Switch;//2
    struct {
        ControlValue ControlValue;
        EqData8B Eq;
        DynData Dyn;//18
        Divider divider;
    } CH[4];
    void initConfigData();
    ConfigData_Monitor_SPK_Net getNetPack();
};

struct ConfigData_Monitor_SPK_Net{
    struct {
        ControlValue ControlValue;
        EqData8B_Net Eq;
        DynData Dyn;//18
        Divider divider;
    } CH[4];
    ConfigData_Monitor_SPK getDataPack();
};

struct ADC_Data{
    quint16 volt1Ch;
    quint16 volt2Ch;
    quint16 curr1Ch;
    quint16 curr2Ch;
    quint16 Temperature;
    float volumeL;
    float volumeR;
    quint8 AMP_Error;
};

struct MsgData_Spk{
    MsgData_Spk() { memset(this, 0, sizeof(MsgData_Spk));}
    union {
        quint8 max_len[300];
        char name[16];
        quint8 volumeCurrentType;

        CommonData_Spk Other;
        struct {
            quint8 ch;
            EqData4B_Net Eq;
        }_EqData;
        struct {
            quint8 ch;
            DynData dyn;
        }_Dyn;
        struct {
            quint8 ch;
            qint16 gain;
        }_Gain;
        struct {
            quint8 ch;
            Divider divider;
        }_Divider;
        ADC_Data ADC_Value;
        ConfigData_Spk_Net InitData;
    };
} ;

struct MsgData_Amp{
    MsgData_Amp() { memset(this, 0, sizeof(MsgData_Amp));}
    union {
        //quint8 max_len[300];
        char name[16];
        quint8 volumeCurrentType;

        CommonData_Amp Other;
        struct {
            quint8 ch;
            EqData4B_Net Eq;
        }_EqData;
        struct {
            quint8 ch;
            DynData dyn;
        }_Dyn;
        struct {
            quint8 ch;
            qint16 gain;
        }_Gain;
        struct {
            quint8 ch;
            Divider divider;
        }_Divider;
        ADC_Data ADC_Value;
//        ConfigData_Amp_Net InitData;
    };
} ;

struct MsgData_RT1052_Poe{
    MsgData_RT1052_Poe() { memset(this, 0, sizeof(MsgData_RT1052_Poe));}
    union {
        struct {
            uint8_t ch;
            union {
                uint8_t u8d;
                int8_t i8_value;
                struct ConfigData_RT1052_Gain gain;
                struct ConfigData_RT1052_Delay delay;
                EqData4B_Net eq4B;
                EqData15B_Net eq15B;
                DynData dyn;
                struct ConfigData_Xover xover;
                struct ConfigData_RT1052_Mixer_4_8 mixer;
                struct ConfigData_RT1052_Mixer_4_4 mixer44;
                struct ConfigData_RT1052_Mixer_2_2 mixer22;
                struct ConfigData_Amp4_4_route route;
                struct ConfigData_AP400_Route route_AP400;
                uint8_t control_cmd[16];
            } _u;
        } _online;

        union {
            struct {
                int meter_In[8];
                int meter_Out[8];
            } _line_4_4;
            struct {
                int meter_In[8];
                int meter_Out[4];
                int8_t gain[4];
#if Logo_HuShan
                float ADC_Value[10];
#else
                uint16_t temperature[2];
                float ADC_Value[6];
#endif
            } _AAX500;
            struct {
                int meter_In[3];
                int meter_Out[3];
                uint16_t temperature;
                float AD1_Value;
            } _line_60w_4_4;
            struct {
                int meter_In[2];
                int meter_Out[1];
                quint8 bluetooth_state;
                quint8 bluetooth_vol;
            } _ap60_bluetooth;
            struct {
                int meter_In[3];
                int meter_Out[9];
                uint16_t temperature;
                float AD1_Value;
#ifdef Logo_BeiJingHuiSheng
                uint8_t current_route_src;
#endif
            } _AP400;
        } _monitor;
        ConfigData_Amp4_4_Net Line_4_4_InitData;
        ConfigData_RT1052_Poe_60W_2_4_Net Line_60W_2_4_InitData;
        ConfigData_RT1052_Poe_AP60_Bluetooth_Net AP60_Bluetooth_InitData;
        ConfigData_RT1052_AP400_Net AP400_InitData;
    };
} ;

struct MsgData_Mic_Quad{
    MsgData_Mic_Quad() { memset(this, 0, sizeof(MsgData_Mic_Quad));}
    union {
        struct {
            uint8_t ch;
            union {
                uint8_t mic_gain;
                uint8_t mic_phantom;
                struct ConfigData_RT1052_Gain gain;
                EqData4B_Net eq;
                DynData dyn;
                struct ConfigData_Xover xover;
                struct ConfigData_RT1052_Mixer_4_4 mixer;
            } _u;
        } _online;
        struct {
            int meter_In[4];
            int meter_Out[4];
        } _monitor;
        struct {
           uint16_t group_no;
            uint8_t start_port_no;
           uint8_t  end_port_no;
        } _huisheng_mic;             //HuiShengKeJi
        ConfigData_Mic_Quad_Net InitData;
    };
};

struct MsgData_Amp_2_0_Mic_Line{
    MsgData_Amp_2_0_Mic_Line() { memset(this, 0, sizeof(MsgData_Amp_2_0_Mic_Line));}
    union {
        struct {
            uint8_t ch;
            union {
                uint8_t mic_open;
                uint8_t mic_gain;
                uint8_t mic_phantom;
                struct ConfigData_RT1052_Gain gain;
                EqData4B_Net eq;
                DynData dyn;
                struct ConfigData_Xover xover;
                struct ConfigData_RT1052_Mixer_2_2 mixer;
            } _u;
        } _online;
        struct {
            int meter_In[2];
            int meter_Out[2];
        } _monitor;
        ConfigData_Amp_2_0_Mic_Line_Net InitData;
    };
};

struct MsgData_Amp_0_2_Line{
    MsgData_Amp_0_2_Line() { memset(this, 0, sizeof(MsgData_Amp_0_2_Line));}
    union {
        struct {
            uint8_t ch;
            union {
                struct ConfigData_RT1052_Gain gain;
                struct ConfigData_RT1052_Delay delay;
                EqData4B_Net eq;
                DynData dyn;
                struct ConfigData_Xover xover;
                struct ConfigData_RT1052_Mixer_2_2 mixer;
            } _u;
        } _online;
        struct {
            int meter_In[2];
            int meter_Out[2];
        } _monitor;
        ConfigData_Amp_0_2_Line_Net InitData;
    };
} ;

struct MsgData_Moniter_SPK{
    MsgData_Moniter_SPK() { memset(this, 0, sizeof(MsgData_Moniter_SPK));}
    union {
        //quint8 max_len[300];
        char name[16];
//        quint8 volumeCurrentType;

        CommonData_Amp Other;
        struct {
            quint8 ch;
            EqData8B_Net Eq;
        }_EqData;
        struct {
            quint8 ch;
            DynData dyn;
        }_Dyn;
        struct {
            quint8 ch;
            qint16 gain;
        }_Gain;
        struct {
            quint8 ch;
            Divider divider;
        }_Divider;
        ADC_Data ADC_Value;
        ConfigData_Monitor_SPK_Net InitData;
    };
} ;

#endif // PACKCOMMON_H
