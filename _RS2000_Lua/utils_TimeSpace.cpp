#include "utils.h"
extern QMap<CfgType, float> dspSpaceMap;//dsp算法的空间占用
extern QMap<CfgType, float> dspCpuMap; //dsp算法的CPU占用
extern QMap<CfgType, float> master128_CpuMap; //master128算法的CPU占用

float Utils::getSpaceUsage(DevType devType, CfgType cfgType)
{
    //X86和ARM平台内存几乎无限制
    PlatformType platform = getPlatformType(devType);
    if(platform == Platform_X86 || devType == Platform_ARM){
        return 0.0001f; //
    }
    else{
        if(dspSpaceMap.contains(cfgType)){
            return dspSpaceMap[cfgType];
        }
        else {
            qDebug() <<"Utils::getSpaceUsage() not exiest dsp:cfgType:" << cfgType;
            return 0.00f;
        }
    }
}

float Utils::getTimeUsage(DevType devType, CfgType cfgType)
{
    if(devType == Dev_Type_NAP_X86_16Analog_128AES67){
        if(master128_CpuMap.contains(cfgType)){
            return master128_CpuMap[cfgType];
        }
        else {
            qDebug() <<"Utils::getTimeUsage() not exiest master128:cfgType:" << cfgType;
            return 0.00f;
        }
    }
    else if(devType == Dev_Type_NAP_X86_32Analog_256AES67){//
    }
    else{
        if(dspCpuMap.contains(cfgType)){
            return dspCpuMap[cfgType];
        }
        else {
            qDebug() <<"Utils::getTimeUsage() not exiest dsp:cfgType:" << cfgType;
            return 0.00f;
        }
    }
}


//dsp算法的空间占用
QMap<CfgType, float> dspSpaceMap = {
    {CFG_TYPE_DELAY_5, 0.11f},          //延时器
    {CFG_TYPE_DELAY_10, 0.11f},
    {CFG_TYPE_DELAY_20, 0.2f},
    {CFG_TYPE_DELAY_50, 0.5f},
    {CFG_TYPE_DELAY_100, 1.0f},
    {CFG_TYPE_DELAY_500, 5.0f},
    {CFG_TYPE_DELAY_1000, 10.0f},
    {CFG_TYPE_HPDELY_1, 0.41f},
    {CFG_TYPE_HPDELY_5, 0.11f},
    {CFG_TYPE_HPDELY_10, 0.11f},
    {CFG_TYPE_HPDELY_20, 0.2f},
    {CFG_TYPE_HPDELY_50, 0.5f},
    {CFG_TYPE_ROUTER_4, 0.12f},         //路由器
    {CFG_TYPE_ROUTER_8, 0.2f},
    {CFG_TYPE_ROUTER_16, 0.35f},
    {CFG_TYPE_ROUTER_32, 0.6f},
    {CFG_TYPE_ROUTER_64, 1.0f},
    {CFG_TYPE_MIXER_4X1, 0.21f},        //混音器
    {CFG_TYPE_MIXER_4X2, 0.27f},
    {CFG_TYPE_MIXER_4X4, 0.4f},
    {CFG_TYPE_MIXER_8X1, 0.31f},
    {CFG_TYPE_MIXER_8X2, 0.42f},
    {CFG_TYPE_MIXER_8X4, 0.65f},
    {CFG_TYPE_MIXER_8X8, 1.1f},
    {CFG_TYPE_MIXER_16X16, 3.7f},
    {CFG_TYPE_MIXER_32X32, 14.09f},
    {CFG_TYPE_MIXER_64X64, 53.67f},
    {CFG_TYPE_DYN_AUTO, 0.14f},         //动态处理
    {CFG_TYPE_DYN_COMP, 0.14f},
    {CFG_TYPE_DYN_LIMIT, 0.14f},
    {CFG_TYPE_DYN_NOISE, 0.12f},
    {CFG_TYPE_DYN_EXPAND, 0.12f},
    {CFG_TYPE_DYN_PEAK, 0.14f},
    {CFG_TYPE_EQ_1, 0.2f},        //均衡器
    {CFG_TYPE_EQ_3, 0.37f},
    {CFG_TYPE_EQ_5, 0.53f},
    {CFG_TYPE_EQ_7, 0.69f},
    {CFG_TYPE_EQ_10, 0.94f},
    {CFG_TYPE_EQ_16, 1.42f},
    {CFG_TYPE_EQ_3X3, 0.94f},
    {CFG_TYPE_EQ_2X3, 1.34f},
    {CFG_TYPE_EQ_1X3, 2.64f},
    {CFG_TYPE_FILTE_LP, 0.45f},        //滤波器
    {CFG_TYPE_FILTE_HP, 0.45f},
    {CFG_TYPE_FILTE_LS, 0.2f},
    {CFG_TYPE_FILTE_HS, 0.2f},
    {CFG_TYPE_FILTER_BP, 0.2f},
    {CFG_TYPE_FILTER_AP, 0.2f},
    {CFG_TYPE_SPLIT_2, 0.94f},        //分频器
    {CFG_TYPE_SPLIT_3, 1.71f},
    {CFG_TYPE_SPLIT_4, 2.48f},
    {CFG_TYPE_SIG_SINE, 0.04f},        //信号发生器
    {CFG_TYPE_SIG_NOISE, 0.03f},
    {CFG_TYPE_SIG_PINKNOISE, 0.07f},
    {CFG_TYPE_METER_1, 0.14f},        //仪表显示
    {CFG_TYPE_METER_2, 0.25f},
    {CFG_TYPE_METER_4, 0.46f},
    {CFG_TYPE_METER_8, 0.89f},
    {CFG_TYPE_INPUT_1, 0.05f},        //输入输出端口
    {CFG_TYPE_INPUT_2, 0.10f},
    {CFG_TYPE_INPUT_4, 0.20f},
    {CFG_TYPE_INPUT_8, 0.40f},
    {CFG_TYPE_INPUT_16, 0.80f},
//    {CFG_TYPE_PDUCKER_4, 0.00f},        //PRIORITY DUCKER
//    {CFG_TYPE_PDUCKER_8, 0.00f},
//    {CFG_TYPE_PDUCKER_16, 0.00f},
//    {CFG_TYPE_PDUCKER_32, 0.00f},
//    {CFG_TYPE_GAINRAMP_1, 0.00f},        //GAIN RAMP
//    {CFG_TYPE_GAINRAMP_2, 0.00f},
//    {CFG_TYPE_GAINRAMP_4, 0.00f},
//    {CFG_TYPE_GAINRAMP_8, 0.00f},
//    {CFG_TYPE_GAINRAMP_16, 0.00f},
//    {CFG_TYPE_GAINRAMP_32, 0.00f},
//    {CFG_TYPE_GAIN_1, 0.00f},        //Gain
//    {CFG_TYPE_GAIN_2, 0.00f},
//    {CFG_TYPE_GAIN_4, 0.00f},
//    {CFG_TYPE_GAIN_8, 0.00f},
//    {CFG_TYPE_GAIN_16, 0.00f},
//    {CFG_TYPE_GAIN_32, 0.00f},
//    {CFG_TYPE_CROSSFADER_2X1, 0.00f},        //Crossfader
//    {CFG_TYPE_CROSSFADER_4X2, 0.00f},
//    {CFG_TYPE_CROSSFADER_8X4, 0.00f},
//    {CFG_TYPE_CROSSFADER_16X8, 0.00f},
//    {CFG_TYPE_CROSSFADER_32X16, 0.00f},
    {CFG_TYPE_GPI, 0.05f},      //其它
    {CFG_TYPE_FREQSHIFT, 2.3f},
    {CFG_TYPE_FREQSHIFT_PLUS, 4.13f},
    {CFG_TYPE_AEC_2_2, 0.1f},
    {CFG_TYPE_SOFTAEC_8K64, 44.69f},
    {CFG_TYPE_SOFTAEC_8K96, 58.35f},
    {CFG_TYPE_SOFTAEC_8K128, 72.0f},
    {CFG_TYPE_SOFTAEC_16K64, 83.56f},
    {CFG_TYPE_SOFTFIR_MONO256, 4.87f},        //FIR Custom
    {CFG_TYPE_SOFTFIR_MONO512, 9.67f},
    {CFG_TYPE_SOFTFIR_MONO1024, 19.26f},
    {CFG_TYPE_SOFTFIR_STEREO256, 6.5f},
    {CFG_TYPE_SOFTFIR_STEREO512, 12.9f},
    {CFG_TYPE_SOFTFIR_STEREO1024, 25.69f},
    {CFG_TYPE_SOFTFIR_4R256, 9.76f},
    {CFG_TYPE_SOFTFIR_4R512, 19.36f},
    {CFG_TYPE_SOFTFIR_4R1024, 38.54f},
    {CFG_TYPE_AUTOMIX_GS_4, 2.07f},         //自动混音
    {CFG_TYPE_AUTOMIX_GS_8, 4.03f},
    {CFG_TYPE_AUTOMIX_GS_16, 7.94f},
    {CFG_TYPE_AUTOMIX_GS_32, 15.76f},
    {CFG_TYPE_AUTOMIXNGAT_4, 3.34f},
    {CFG_TYPE_AUTOMIXNGAT_8, 6.22f},
    {CFG_TYPE_AUTOMIXNGAT_16, 12.0f},
    {CFG_TYPE_AUTOMIXNGAT_32, 23.55f},
    {CFG_TYPE_AUTOMIXNGRT_4, 2.97f},
    {CFG_TYPE_AUTOMIXNGRT_8, 5.65f},
    {CFG_TYPE_AUTOMIXNGRT_16, 11.03f},
    {CFG_TYPE_AUTOMIXNGRT_32, 21.79f},
//    {CFG_TYPE_NFC1_8NF, 0.00f},
//    {CFG_TYPE_NFC1_16NF, 0.00f},
//    {CFG_TYPE_NFC1_24NF, 0.00f},
//    {CFG_TYPE_NFC1_32NF, 0.00f},
    {CFG_TYPE_VOICETUBE_1, 8.6f},
    {CFG_TYPE_VOICETUBE_2, 8.6f} };


//dsp算法的CPU占用
QMap<CfgType, float> dspCpuMap = {
    {CFG_TYPE_DELAY_5, 0.76f},   //延时器
    {CFG_TYPE_DELAY_10, 0.76f},
    {CFG_TYPE_DELAY_20, 0.76f},
    {CFG_TYPE_DELAY_50, 0.76f},
    {CFG_TYPE_DELAY_100, 0.76f},
    {CFG_TYPE_DELAY_500, 0.76f},
    {CFG_TYPE_DELAY_1000, 0.76f},
    {CFG_TYPE_HPDELY_1, 0.59f},
    {CFG_TYPE_HPDELY_5, 1.06f},
    {CFG_TYPE_HPDELY_10, 1.06f},
    {CFG_TYPE_HPDELY_20, 1.06f},
    {CFG_TYPE_HPDELY_50, 1.06f},
    {CFG_TYPE_ROUTER_4, 0.22f},    //路由器
    {CFG_TYPE_ROUTER_8, 0.42f},
    {CFG_TYPE_ROUTER_16, 0.82f},
    {CFG_TYPE_ROUTER_32, 1.6f},
    {CFG_TYPE_ROUTER_64, 3.2f},
    {CFG_TYPE_MIXER_4X1, 0.18f},    //混音器
    {CFG_TYPE_MIXER_4X2, 0.28f},
    {CFG_TYPE_MIXER_4X4, 0.48f},
    {CFG_TYPE_MIXER_8X1, 0.25f},
    {CFG_TYPE_MIXER_8X2, 0.43f},
    {CFG_TYPE_MIXER_8X4, 0.76f},
    {CFG_TYPE_MIXER_8X8, 1.43f},
    {CFG_TYPE_MIXER_16X16, 5.03f},
    {CFG_TYPE_MIXER_32X32, 18.97f},
    {CFG_TYPE_MIXER_64X64, 73.86f},
    {CFG_TYPE_DYN_AUTO, 0.4f},    //动态处理
    {CFG_TYPE_DYN_COMP, 0.4f},
    {CFG_TYPE_DYN_LIMIT, 0.4f},
    {CFG_TYPE_DYN_NOISE, 0.35f},
    {CFG_TYPE_DYN_EXPAND, 0.39f},
    {CFG_TYPE_DYN_PEAK, 0.4f},
    {CFG_TYPE_EQ_1, 0.21f},    //均衡器
    {CFG_TYPE_EQ_3, 0.52f},
    {CFG_TYPE_EQ_5, 0.83f},
    {CFG_TYPE_EQ_7, 1.13f},
    {CFG_TYPE_EQ_10, 1.6f},
    {CFG_TYPE_EQ_16, 2.53f},
    {CFG_TYPE_EQ_3X3, 1.6f},
    {CFG_TYPE_EQ_2X3, 2.37f},
    {CFG_TYPE_EQ_1X3, 4.84f},
    {CFG_TYPE_FILTE_LP, 0.67f},//滤波器
    {CFG_TYPE_FILTE_HP, 0.67f},
    {CFG_TYPE_FILTE_LS, 0.21f},
    {CFG_TYPE_FILTE_HS, 0.21f},
    {CFG_TYPE_FILTER_BP, 0.21f},
    {CFG_TYPE_FILTER_AP, 0.21f},
    {CFG_TYPE_SPLIT_2, 1.36f},    //分频器
    {CFG_TYPE_SPLIT_3, 2.65f},
    {CFG_TYPE_SPLIT_4, 3.93f},
    {CFG_TYPE_SIG_SINE, 1.3f},    //信号发生器
    {CFG_TYPE_SIG_NOISE, 0.66f},
    {CFG_TYPE_SIG_PINKNOISE, 2.2f},
    {CFG_TYPE_METER_1, 0.4f},    //仪表显示
    {CFG_TYPE_METER_2, 0.77f},
    {CFG_TYPE_METER_4, 1.53f},
    {CFG_TYPE_METER_8, 3.03f},
    {CFG_TYPE_INPUT_1, 0.05f},    //输入输出端口
    {CFG_TYPE_INPUT_2, 0.05f},
    {CFG_TYPE_INPUT_4, 0.05f},
    {CFG_TYPE_INPUT_8, 0.05f},
    {CFG_TYPE_INPUT_16, 0.05f},
    {CFG_TYPE_OUTPUT_1, 0.05f},
    {CFG_TYPE_OUTPUT_2, 0.05f},
    {CFG_TYPE_OUTPUT_4, 0.05f},
    {CFG_TYPE_OUTPUT_8, 0.05f},
    {CFG_TYPE_OUTPUT_16, 0.05f},
//    {CFG_TYPE_PDUCKER_4, 0.00f},    //PRIORITY DUCKER
//    {CFG_TYPE_PDUCKER_8, 0.00f},
//    {CFG_TYPE_PDUCKER_16, 0.00f},
//    {CFG_TYPE_PDUCKER_32, 0.00f},
//    {CFG_TYPE_GAINRAMP_1, 0.00f},    //GAIN RAMP
//    {CFG_TYPE_GAINRAMP_2, 0.00f},
//    {CFG_TYPE_GAINRAMP_4, 0.00f},
//    {CFG_TYPE_GAINRAMP_8, 0.00f},
//    {CFG_TYPE_GAINRAMP_16, 0.00f},
//    {CFG_TYPE_GAINRAMP_32, 0.00f},
//    {CFG_TYPE_GAIN_1, 0.00f},    //Gain
//    {CFG_TYPE_GAIN_2, 0.00f},
//    {CFG_TYPE_GAIN_4, 0.00f},
//    {CFG_TYPE_GAIN_8, 0.00f},
//    {CFG_TYPE_GAIN_16, 0.00f},
//    {CFG_TYPE_GAIN_32, 0.00f},
//    {CFG_TYPE_CROSSFADER_2X1, 0.00f},//Crossfader
//    {CFG_TYPE_CROSSFADER_4X2, 0.00f},
//    {CFG_TYPE_CROSSFADER_8X4, 0.00f},
//    {CFG_TYPE_CROSSFADER_16X8, 0.00f},
//    {CFG_TYPE_CROSSFADER_32X16, 0.00f},
    {CFG_TYPE_GPI, 0.08f},    //其它
    {CFG_TYPE_FREQSHIFT, 13.37f},
    {CFG_TYPE_FREQSHIFT_PLUS, 13.28f},
    {CFG_TYPE_AEC_2_2, 0.73f},
    {CFG_TYPE_SOFTAEC_8K64, 41.79f},
    {CFG_TYPE_SOFTAEC_8K96, 42.51f},
    {CFG_TYPE_SOFTAEC_8K128, 43.24f},
    {CFG_TYPE_SOFTAEC_16K64, 78.98f},
    {CFG_TYPE_SOFTFIR_MONO256, 5.01f},  //FIR Custom
    {CFG_TYPE_SOFTFIR_MONO512, 9.24f},
    {CFG_TYPE_SOFTFIR_MONO1024, 17.69f},
    {CFG_TYPE_SOFTFIR_STEREO256, 10.0f},
    {CFG_TYPE_SOFTFIR_STEREO512, 18.44f},
    {CFG_TYPE_SOFTFIR_STEREO1024, 35.34f},
    {CFG_TYPE_SOFTFIR_4R256, 19.96f},
    {CFG_TYPE_SOFTFIR_4R512, 36.86f},
    {CFG_TYPE_SOFTFIR_4R1024, 70.65f},
    {CFG_TYPE_AUTOMIX_GS_4, 0.94f}, //AutoMixer
    {CFG_TYPE_AUTOMIX_GS_8, 1.09f},
    {CFG_TYPE_AUTOMIX_GS_16, 1.39f},
    {CFG_TYPE_AUTOMIX_GS_32, 1.99f},
    {CFG_TYPE_AUTOMIXNGAT_4, 1.05f},
    {CFG_TYPE_AUTOMIXNGAT_8, 1.33f},
    {CFG_TYPE_AUTOMIXNGAT_16, 1.88f},
    {CFG_TYPE_AUTOMIXNGAT_32, 2.98f},
    {CFG_TYPE_AUTOMIXNGRT_4, 1.1f},
    {CFG_TYPE_AUTOMIXNGRT_8, 1.43f},
    {CFG_TYPE_AUTOMIXNGRT_16, 2.08f},
    {CFG_TYPE_AUTOMIXNGRT_32, 3.38f},
//    {CFG_TYPE_NFC1_8NF, 0.00f},
//    {CFG_TYPE_NFC1_16NF, 0.00f},
//    {CFG_TYPE_NFC1_24NF, 0.00f},
//    {CFG_TYPE_NFC1_32NF, 0.00f},
    {CFG_TYPE_VOICETUBE_1, 43.24f},
    {CFG_TYPE_VOICETUBE_2, 43.24f} };


//dsp算法的CPU占用
QMap<CfgType, float> master128_CpuMap = {
    {CFG_TYPE_DELAY_5, 0.04f},   //延时器
    {CFG_TYPE_DELAY_10, 0.04f},
    {CFG_TYPE_DELAY_20, 0.04f},
    {CFG_TYPE_DELAY_50, 0.04f},
    {CFG_TYPE_DELAY_100, 0.04f},
    {CFG_TYPE_DELAY_500, 0.04f},
    {CFG_TYPE_DELAY_1000, 0.04f},
    {CFG_TYPE_HPDELY_1, 0.05f},
    {CFG_TYPE_HPDELY_5, 0.05f},
    {CFG_TYPE_HPDELY_10, 0.05f},
    {CFG_TYPE_HPDELY_20, 0.05f},
    {CFG_TYPE_HPDELY_50, 0.05f},
    {CFG_TYPE_ROUTER_4, 0.11f},    //路由器
    {CFG_TYPE_ROUTER_8, 0.21f},
    {CFG_TYPE_ROUTER_16, 0.43f},
    {CFG_TYPE_ROUTER_32, 0.85f},
    {CFG_TYPE_ROUTER_64, 1.70f},
    {CFG_TYPE_ROUTER_96, 2.45f},
    {CFG_TYPE_ROUTER_128, 3.25f},
    {CFG_TYPE_MIXER_4X1, 0.08f},    //混音器
    {CFG_TYPE_MIXER_4X2, 0.12f},
    {CFG_TYPE_MIXER_4X4, 0.20f},
    {CFG_TYPE_MIXER_8X1, 0.12f},
    {CFG_TYPE_MIXER_8X2, 0.20f},
    {CFG_TYPE_MIXER_8X4, 0.26f},
    {CFG_TYPE_MIXER_8X8, 0.50f},
    {CFG_TYPE_MIXER_16X16, 1.30f},
    {CFG_TYPE_MIXER_32X32, 2.80f},
    {CFG_TYPE_MIXER_64X64, 8.90f},
    {CFG_TYPE_DYN_AUTO, 0.07f},    //动态处理
    {CFG_TYPE_DYN_COMP, 0.07f},
    {CFG_TYPE_DYN_LIMIT, 0.07f},
    {CFG_TYPE_DYN_NOISE, 0.07f},
    {CFG_TYPE_DYN_EXPAND, 0.07f},
    {CFG_TYPE_DYN_PEAK, 0.07f},
    {CFG_TYPE_EQ_1, 0.08f},    //均衡器
    {CFG_TYPE_EQ_3, 0.11f},
    {CFG_TYPE_EQ_5, 0.17f},
    {CFG_TYPE_EQ_7, 0.23f},
    {CFG_TYPE_EQ_10, 0.29f},
    {CFG_TYPE_EQ_16, 0.45f},
    {CFG_TYPE_EQ_3X3, 0.83f},
    {CFG_TYPE_EQ_2X3, 0.42f},
    {CFG_TYPE_EQ_1X3, 0.29f},
    {CFG_TYPE_FILTE_LP, 0.14f},//滤波器
    {CFG_TYPE_FILTE_HP, 0.14f},
    {CFG_TYPE_FILTE_LS, 0.06f},
    {CFG_TYPE_FILTE_HS, 0.06f},
    {CFG_TYPE_FILTER_BP, 0.06f},
    {CFG_TYPE_FILTER_AP, 0.08f},
    {CFG_TYPE_SPLIT_2, 0.27f},    //分频器
    {CFG_TYPE_SPLIT_3, 0.50f},
    {CFG_TYPE_SPLIT_4, 0.74f},
    {CFG_TYPE_SIG_SINE, 0.07f},    //信号发生器
    {CFG_TYPE_SIG_NOISE, 0.37f},
    {CFG_TYPE_SIG_PINKNOISE, 0.16f},
    {CFG_TYPE_METER_1, 0.05f},    //仪表显示
    {CFG_TYPE_METER_2, 0.08f},
    {CFG_TYPE_METER_4, 0.14f},
    {CFG_TYPE_METER_8, 0.28f},
    {CFG_TYPE_INPUT_1, 0.04f},    //输入输出端口
    {CFG_TYPE_INPUT_2, 0.04f},
    {CFG_TYPE_INPUT_4, 0.04f},
    {CFG_TYPE_INPUT_8, 0.04f},
    {CFG_TYPE_INPUT_16, 0.04f},
    {CFG_TYPE_OUTPUT_1, 0.04f},
    {CFG_TYPE_OUTPUT_2, 0.04f},
    {CFG_TYPE_OUTPUT_4, 0.04f},
    {CFG_TYPE_OUTPUT_8, 0.04f},
    {CFG_TYPE_OUTPUT_16, 0.04f},
    {CFG_TYPE_PDUCKER_4, 0.19f},    //PRIORITY DUCKER
    {CFG_TYPE_PDUCKER_8, 0.34f},
    {CFG_TYPE_PDUCKER_16, 0.67f},
    {CFG_TYPE_PDUCKER_32, 1.40f},
    {CFG_TYPE_GAINRAMP_1, 0.03f},    //GAIN RAMP
    {CFG_TYPE_GAINRAMP_2, 0.07f},
    {CFG_TYPE_GAINRAMP_4, 0.14f},
    {CFG_TYPE_GAINRAMP_8, 0.27f},
    {CFG_TYPE_GAINRAMP_16, 0.54f},
    {CFG_TYPE_GAINRAMP_32, 1.03f},
    {CFG_TYPE_GAIN_1, 0.05f},    //Gain
    {CFG_TYPE_GAIN_2, 0.10f},
    {CFG_TYPE_GAIN_4, 0.21f},
    {CFG_TYPE_GAIN_8, 0.42f},
    {CFG_TYPE_GAIN_16, 0.83f},
    {CFG_TYPE_GAIN_32, 1.66f},
    {CFG_TYPE_CROSSFADER_2X1, 0.04f},//Crossfader
    {CFG_TYPE_CROSSFADER_4X2, 0.09f},
    {CFG_TYPE_CROSSFADER_8X4, 0.16f},
    {CFG_TYPE_CROSSFADER_16X8, 0.33f},
    {CFG_TYPE_CROSSFADER_32X16, 0.65f},
    {CFG_TYPE_GPI, 0.05f},    //其它
    {CFG_TYPE_FREQSHIFT, 1.69f},
    {CFG_TYPE_FREQSHIFT_PLUS, 1.69f},
    {CFG_TYPE_AEC_2_2, 0.07f},
    {CFG_TYPE_SOFTAEC_IK_8K64, 0.00f},
    {CFG_TYPE_SOFTAEC_IK_8K96, 0.00f},
    {CFG_TYPE_SOFTAEC_IK_8K128, 0.00f},
    {CFG_TYPE_SOFTAEC_IK_16K64, 0.00f},
//    {CFG_TYPE_SOFTFIR_MONO256, 5.01f},  //FIR Custom
//    {CFG_TYPE_SOFTFIR_MONO512, 9.24f},
//    {CFG_TYPE_SOFTFIR_MONO1024, 17.69f},
//    {CFG_TYPE_SOFTFIR_STEREO256, 10.0f},
//    {CFG_TYPE_SOFTFIR_STEREO512, 18.44f},
//    {CFG_TYPE_SOFTFIR_STEREO1024, 35.34f},
//    {CFG_TYPE_SOFTFIR_4R256, 19.96f},
//    {CFG_TYPE_SOFTFIR_4R512, 36.86f},
//    {CFG_TYPE_SOFTFIR_4R1024, 70.65f},
    {CFG_TYPE_AUTOMIX_GS_4, 0.26f}, //AutoMixer
    {CFG_TYPE_AUTOMIX_GS_8, 0.53f},
    {CFG_TYPE_AUTOMIX_GS_16, 0.98f},
    {CFG_TYPE_AUTOMIX_GS_32, 2.10f},
    {CFG_TYPE_AUTOMIXNGAT_4, 0.31f},
    {CFG_TYPE_AUTOMIXNGAT_8, 0.58f},
    {CFG_TYPE_AUTOMIXNGAT_16, 1.05f},
    {CFG_TYPE_AUTOMIXNGAT_32, 2.10f},
    {CFG_TYPE_AUTOMIXNGRT_4, 0.42f},
    {CFG_TYPE_AUTOMIXNGRT_8, 0.79f},
    {CFG_TYPE_AUTOMIXNGRT_16, 1.53f},
    {CFG_TYPE_AUTOMIXNGRT_32, 2.95f},
    {CFG_TYPE_NFC1_8NF, 1.09f},
    {CFG_TYPE_NFC1_16NF, 1.51f},
    {CFG_TYPE_NFC1_24NF, 1.71f},
    {CFG_TYPE_NFC1_32NF, 1.94f},
//    {CFG_TYPE_VOICETUBE_1, 0.00f},
//    {CFG_TYPE_VOICETUBE_2, 0.00f}
};
