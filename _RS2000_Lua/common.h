#ifndef COMMON_H
#define COMMON_H

typedef unsigned int uint32;
typedef int int32;
#define OPACKU uint32
#define OPACKS int32

//#define PC_PREFIX      'V'//T:测试版  V:正式版
//#define TRANSCTRL_MAJOR 3 //大版本更新使用
//#define TRANSCTRL_MINOR 0 //协议变动使用
//#define TRANSCTRL_BUILD 0 //新增模块使用
//#define PC_BUILD        0

#define DSP_MAJOR       2
#define DSP_MINOR       1
#define DSP_BUILD       0

#define X86_MAJOR       1
#define X86_MINOR       0
#define X86_BUILD       0

#define ARM_MAJOR       1
#define ARM_MINOR       0
#define ARM_BUILD       0

#define Appoint '.'// 房间输入输出标签以‘.’开头

#define DEFAULT_DEBUG_DIR  "temp\\"
#define __OPACK32_T1(type1,v1) type1 v1

#define __OPACK32_T1S0(type1,v1,d1) 				\
    uint32 __pad_##v1:(32-d1);				\
    type1 v1:d1

#define __OPACK32_T1S3(type1,v1,d1) 				\
    type1 v1:d1;									\
    uint32 __pad_##v1:(32-d1)

#define __OPACK32_T1M1(type1,v1,p1,p2) 				\
    uint32 __pad2_##v1:(32-p2);						\
    type1 v1:(p2-p1);								\
    uint32 __pad1_##v1:(p1)

#define  __OPACK32_T1M2(type1,v1,p2,p3)  __OPACK32_T1M1(type1,v1,p2,p3)


#define __OPACK32_T2(type1,v1,d1,type2,v2,d2) 		\
    type2 v2:d2;									\
    type1 v1:d1

#define __OPACK32_T2S0S1(type1,v1,d1,type2,v2,d2) 	\
    uint32 __pad_##v1:(32-d1-d2);					\
    type2 v2:d2;									\
    type1 v1:d1

#define __OPACK32_T2S0S3(type1,v1,d1,type2,v2,d2) 	\
    type2 v2:d2;									\
    uint32 __pad_##v1:(32-d1-d2);					\
    type1 v1:d1

#define __OPACK32_T2S2S3(type1,v1,d1,type2,v2,d2) 	\
    type2 v2:d2;									\
    type1 v1:d1;									\
    uint32 __pad_##v1:(32-d1-d2)

#define __OPACK32_T2M0M2(type1,v1,type2,v2,p1,p2,p3)\
    uint32 __pad_##v2:(32-p3);						\
    type2 v2:(p3-p2);								\
    uint32 __pad_##v1:(p2-p1);						\
    type1 v1:p1

#define __OPACK32_T2M1M3(type1,v1,type2,v2,p1,p2,p3)\
    type2 v2:(32-p3);								\
    uint32 __pad_##v2:(p3-p2);						\
    type1 v1:(p2-p1);								\
    uint32 __pad_##v1:p1

#define __OPACK32_T3(type1,v1,d1,type2,v2,d2,type3,v3,d3) 				\
    type3 v3:d3;														\
    type2 v2:d2;														\
    type1 v1:d1

#define __OPACK32_T3S0S1S2(type1,v1,d1,type2,v2,d2,type3,v3,d3) 		\
    uint32 __pad_##v1:(32-d1-d2-d3);									\
    type3 v3:d3;														\
    type2 v2:d2;														\
    type1 v1:d1

#define __OPACK32_T3S0S1S3(type1,v1,d1,type2,v2,d2,type3,v3,d3) 		\
    type3 v3:d3;														\
    uint32 __pad_##v1:(32-d1-d2-d3);									\
    type2 v2:d2;														\
    type1 v1:d1

#define __OPACK32_T3S0S2S3(type1,v1,d1,type2,v2,d2,type3,v3,d3) 		\
    type3 v3:d3;														\
    type2 v2:d2;														\
    uint32 __pad_##v1:(32-d1-d2-d3);									\
    type1 v1:d1

#define __OPACK32_T3S1S2S3(type1,v1,d1,type2,v2,d2,type3,v3,d3) 		\
    type3 v3:d3;														\
    type2 v2:d2;														\
    type1 v1:d1;														\
    uint32 __pad_##v1:(32-d1-d2-d3)

#define __OPACK32_T4(type1,v1,d1,type2,v2,d2,type3,v3,d3,type4,v4,d4) 				\
    type4 v4:d4;																	\
    type3 v3:d3;																	\
    type2 v2:d2;																	\
    type1 v1:d1

#define __OPACK32_T4S0S1S2S3(type1,v1,d1,type2,v2,d2,type3,v3,d3,type4,v4,d4) 	\
    uint32 __pad_##v1:(32-d1-d2-d3-d4);									\
    type4 v4:d4;														\
    type3 v3:d3;														\
    type2 v2:d2;														\
    type1 v1:d1

typedef struct{
    __OPACK32_T4(OPACKU,channelA,8, OPACKU,channelB,8, OPACKU,channelC,8, OPACKU,channelD,8);
}IG_SIG_REAL_CHANNEL;
typedef struct{
    __OPACK32_T2(OPACKU,channelA,16, OPACKU,channelB,16);
}IG_SIG_REAL_CH16;
typedef struct{
    __OPACK32_T4(OPACKU,columnA,8, OPACKU,lineA,8, OPACKU,columnB,8, OPACKU,lineB,8);
}IG_SIGFLOW8_DOUBLE;
typedef struct{
    __OPACK32_T2(OPACKU,column,16, OPACKU,line,16);
}IG_SIGFLOW_SINGLE16;

typedef struct{
 IG_SIGFLOW8_DOUBLE inFlow[16];
 IG_SIGFLOW8_DOUBLE outFlow[16];
}IG_SIGFLOW8_32X32;
typedef struct{
    IG_SIGFLOW8_DOUBLE inFlow[24];
    IG_SIGFLOW8_DOUBLE outFlow[24];
}IG_SIGFLOW8_48X48;

//以下接口定义改为ConfigItem::getOverviewData根据是否16bit协议自适应格式化
#define IG_GLOBAL_HEAD_FLAG 0XA501
typedef struct{
    __OPACK32_T2(
        OPACKU,flag,16,
        OPACKU,moduleAmount,16
    );
    OPACKU remain;
}IG_Global_HEAD16B;

typedef struct{
    __OPACK32_T2S0S1(
        OPACKU,moduleAmountL,8,
        OPACKU,moduleAmountH,8
    );
}IG_Global_HEAD8B;

typedef struct{
    unsigned char current_idx;
    unsigned char b_start;
    short data_len;//unsigned int的长度
}IG_Algorithm_Head;

/* Begin: Project Save file Macro Define */
#define PROJECT_GROUP "Project"

#define PROJECT_INFOMATION_GROUP "Infomation"
#define PROJECT_SCENESIZE        "size"
#define PROJECT_DEVICE_COUNT     "totalDev"
#define PROJECT_LINE_COUNT       "totalLine"
#define PROJECT_LINELAB_COUNT    "totalLineLab"

#define PROJECT_LINE_GROUP    "Line_%1"
#define PROJECT_LINE_STARTPOS "start"
#define PROJECT_LINE_ENDPOS   "end"
#define PROJECT_LINE_STARTZVALUE    "startZValue"
#define PROJECT_LINE_ENDZVALUE      "endZValue"

#define PROJECT_LINELABEL_GROUP  "LineLabel_%1"
#define PROJECT_LINELABEL_POS    "pos"
#define PROJECT_LINELABEL_NAME   "name"
#define PROJECT_LINELABEL_ZVALUE "ZValue"

#define PROJECT_DEVICE_GROUP            "Dev_%1"
#define PROJECT_DEVICE_DEVTYPE          "type"
#define PROJECT_DEVICE_DEVPOS           "pos"
#define PROJECT_DEVICE_DEVNAME          "name"
#define PROJECT_DEVICE_ROOMPROJ         "roomProj"
#define PROJECT_DEVICE_JOINSCENE        "Joinscene"
#define PROJECT_DEVICE_DEVMAC           "mac"
#define PROJECT_DEVICE_DEVSTANDBYMAC    "standbymac"
#define PROJECT_DEVICE_ISMONITOR        "monitor"
#define PROJECT_DEVICE_ZValue           "ZValue"
/* End: Project Save file Macro Define */

//solution room input output
#define SOLUTION_ROOM_INPUT_COUNT       "InputCount"
#define SOLUTION_ROOM_OUTPUT_COUNT      "OutputCount"
#define SOLUTION_ROOM_INPUT_GROUP       "Input_%1"
#define SOLUTION_ROOM_OUTPUT_GROUP      "Ouput_%1"
#define SOLUTION_ROOM_PORT_TYPE         "Type"
#define SOLUTION_ROOM_PORT_NAME         "Name"


/* Begin: Config Save file Macro Define */
#define CONFIG_GROUP "Config_"

#define CONFIG_NMU_VERSION      "nmuVersion"
#define CONFIG_DEV_TYPE         "nmuDevType"
#define CONFIG_DEV_MAC          "nmuDevMac"
#define CONFIG_PAGELIST         "pageList"
#define CONFIG_CURPAGE          "curPage"
#define CONFIG_INFOMATION_GROUP "Infomation"
#define CONFIG_SCENESIZE        "size"
#define CONFIG_DEVICE_COUNT     "totalDev"
#define CONFIG_LINE_COUNT       "totalLine"
#define CONFIG_LINELABEL_COUNT  "totalLineLabel"
#define CONFIG_GRAPITEM_COUNT   "totalGraphicsItem"
#define CONFIG_SERCONITEM_COUNT "totalSerConItem"

#define CONFIG_LINELABEL_GROUP  "LineLabel_%1"
#define CONFIG_LINELABEL_POS    "pos"
#define CONFIG_LINELABEL_NAME   "name"
#define CONFIG_LINELABEL_ZVALUE "ZValue"

#define CONFIG_LINE_GROUP    "Line_%1"
#define CONFIG_LINE_STARTPOS "start"
#define CONFIG_LINE_ENDPOS   "end"
#define CONFIG_LINE_STARTZVALUE "start_zValue"
#define CONFIG_LINE_ENDZVALUE   "end_zValue"

#define CONFIG_SERCONITEM_GROUP         "SerConItem_%1"
#define CONFIG_SERCONITEM_POS           "Pos"
#define CONFIG_SERCONITEM_WIDTH         "Width"
#define CONFIG_SERCONITEM_HEIGHT        "Height"
#define CONFIG_SERCONITEM_PATHCOUNT     "PathCount"
#define CONFIG_SERCONITEM_BORDERPATH    "Path_%1"
#define CONFIG_SERCONITEM_BORDERCOLOR   "BorderColor"
#define CONFIG_SERCONITEM_BGDCOLOR      "BgdColor"
#define CONFIG_SERCONITEM_MODULENAME    "ModuleName"
#define CONFIG_SERCONiTEM_TYPE          "Type"
#define CONFIG_SERCONITEM_CHANNELCOUNT  "ChaCount"
#define CONFIG_SERCONITEM_CHAPORTTYPE   "ChaPortType"
#define CONFIG_SERCONITEM_ZVALUE        "zValue"

#define CONFIG_GRAPHICSIETM_GROUP   "graphicsItem_%1"
#define CONFIG_GRAPHICSITEM_POS     "Pos"
#define CONFIG_GRAPHICSITEM_TYPE    "Type"
#define CONFIG_GRAPHICSITEM_TEXT    "Text"
#define CONFIG_GRAPHICSITEM_ORIGINALTEXT "OglText"
#define CONFIG_GRAPHICSITEM_FONT         "Font"
#define CONFIG_GRAPHICSITEM_FONTSIZE     "FontSize"
#define CONFIG_GRAPHICSITEM_FONTFAMILY   "FontFamily"
#define CONFIG_GRAPHICSITEM_FONTBOLD     "FontBold"
#define CONFIG_GRAPHICSITEM_FONTITALIC   "FontItalic"
#define CONFIG_GRAPHICSITEM_FONTCOLOR    "FontColor"
#define CONFIG_GRAPHICSITEM_TEXTALIGN    "TextAlign"
#define CONFIG_GRAPHICSITEM_BORDERPATHCOUNT "BorderPathCount"
#define CONFIG_GRAPHICSITEM_BORDERPATH   "BorderPath_%1"
#define CONFIG_GRAPHICSITEM_BORDERWIDTH  "BorderWidth"
#define CONFIG_GRAPHICSITEM_BORDERRADIUS "BorderRadius"
#define CONFIG_GRAPHICSITEM_BORDERCOLOR   "BorderColor"
#define CONFIG_GRAPHICSITEM_BGDCOLOR     "BgdColor"

#define CONFIG_DEVICE_GROUP         "Dev_%1"
#define CONFIG_DEVICE_DEVTYPE       "type"
#define CONFIG_DEVICE_DEVPOS        "pos"
#define CONFIG_DEVICE_DEVNAME       "name"
#define CONFIG_DEVICE_LABEL_COLOR   "labelColor"
#define CONFIG_DEVICE_MODULE_ZVALUE "ZValue"

#define CONFIG_DEVICE_DEV_MODULE_NO     "ModuleNo"

#define CONFIG_DEVICE_DEV_INPUT_GROUP       "Input"
#define CONFIG_DEVICE_DEV_INPUT_UserLabel   "userLabel_%1"
#define CONFIG_DEVICE_DEV_INPUT_GAIN        "gain_%1"
#define CONFIG_DEVICE_DEV_INPUT_INVERT      "invert_%1"
#define CONFIG_DEVICE_DEV_INPUT_MUTE        "mute_%1"
#define CONFIG_DEVICE_DEV_INPUT_CHANNEL     "channel"
#define CONFIG_DEVICE_DEV_INPUT_Phantom     "phantom_%1"
#define CONFIG_DEVICE_DEV_INPUT_PreGain     "PreGain_%1"

#define CONFIG_DEVICE_DEV_OUTPUT_GROUP      "Output"
#define CONFIG_DEVICE_DEV_OUTPUT_UserLabel  "userLabel_%1"
#define CONFIG_DEVICE_DEV_OUTPUT_GAIN       "gain_%1"
#define CONFIG_DEVICE_DEV_OUTPUT_INVERT     "invert_%1"
#define CONFIG_DEVICE_DEV_OUTPUT_MUTE       "mute_%1"
#define CONFIG_DEVICE_DEV_OUTPUT_CHANNEL    "channel"

#define CONFIG_DEVICE_DEV_DELAY_GROUP   "Delay"
#define CONFIG_DEVICE_DEV_DELAY_DELAY   "delay"
#define CONFIG_DEVICE_DEV_DELAY_BYPASS  "bypass"

#define CONFIG_DEVICE_DEV_ROUTER_GROUP      "Router"
#define CONFIG_DEVICE_DEV_ROUTER_ROUTERINFO "routerInfo_%1"

#define CONFIG_DEVICE_DEV_MIXER_GROUP           "Mixer"
#define CONFIG_DEVICE_DEV_MIXER_INGAIN          "in_gain_%1"
#define CONFIG_DEVICE_DEV_MIXER_INMUTE          "in_mute_%1"
#define CONFIG_DEVICE_DEV_MIXER_InUserLabel     "in_label_%1"
#define CONFIG_DEVICE_DEV_MIXER_ROUTERGAIN      "router_gain_%1_%2"
#define CONFIG_DEVICE_DEV_MIXER_OUTROUTER_L     "out_router_L_%1"
#define CONFIG_DEVICE_DEV_MIXER_OUTROUTER_H     "out_router_H_%1"
#define CONFIG_DEVICE_DEV_MIXER_OUTGAIN         "out_gain_%1"
#define CONFIG_DEVICE_DEV_MIXER_OUTMUTE         "out_mute_%1"
#define CONFIG_DEVICE_DEV_MIXER_OutUserLabel    "out_label_%1"

#define CONFIG_DEVICE_DEV_DYN_GROUP     "Dyn"
#define CONFIG_DEVICE_DEV_DYN_THRESHOLD "threshold"
#define CONFIG_DEVICE_DEV_DYN_RATIO     "ratio"
#define CONFIG_DEVICE_DEV_DYN_GAIN      "gain"
#define CONFIG_DEVICE_DEV_DYN_ATTACK    "attack"
#define CONFIG_DEVICE_DEV_DYN_RELEASE   "release"
#define CONFIG_DEVICE_DEV_DYN_BYPASS    "bypass"

#define CONFIG_DEVICE_DEV_PRIORITYDUCKER_GROUP          "PriorityDucker"
#define CONFIG_DEVICE_DEV_PRIORITYDUCKER_ATTACKTIME     "AttackTime"
#define CONFIG_DEVICE_DEV_PRIORITYDUCKER_RELEASETIME    "ReleaseTime"
#define CONFIG_DEVICE_DEV_PRIORITYDUCKER_HOLDTIME       "HoldTime"
#define CONFIG_DEVICE_DEV_PRIORITYDUCKER_DETECTORTIME   "DetectorTime"
#define CONFIG_DEVICE_DEV_PRIORITYDUCKER_THRESHOLDLEVEL "ThresholdLevel"
#define CONFIG_DEVICE_DEV_PRIORITYDUCKER_DEPTH          "Depth"
#define CONFIG_DEVICE_DEV_PRIORITYDUCKER_PRIORCHINGAIN  "PriorChInGain"
#define CONFIG_DEVICE_DEV_PRIORITYDUCKER_PRIORCHOUTGAIN "PriorChOutGain"
#define CONFIG_DEVICE_DEV_PRIORITYDUCKER_MIXOUTGAIN     "MixOutGain"
#define CONFIG_DEVICE_DEV_PRIORITYDUCKER_BYPASS         "Bypass"

#define CONFIG_DEVICE_DEV_EQ_GROUP     "Eq"
#define CONFIG_DEVICE_DEV_EQ_BYPASS    "bypass"
#define CONFIG_DEVICE_DEV_EQ_SEGBYPASS "SegBypass_%1"
#define CONFIG_DEVICE_DEV_EQ_FREQ      "freq_%1"
#define CONFIG_DEVICE_DEV_EQ_GAIN      "gain_%1"
#define CONFIG_DEVICE_DEV_EQ_Q         "q_%1"

#define CONFIG_DEVICE_DEV_FILTER_GROUP      "Filter"
#define CONFIG_DEVICE_DEV_FILTER_BYPASS     "bypass"
#define CONFIG_DEVICE_DEV_FILTER_FILTERTYPE "filter_type"
#define CONFIG_DEVICE_DEV_FILTER_FREQ       "freq"
#define CONFIG_DEVICE_DEV_FILTER_GAIN       "gain"

#define CONFIG_DEVICE_DEV_SPLIT_GROUP               "Split"
#define CONFIG_DEVICE_DEV_SPLIT_INPUT_LEVEL         "input_level"
#define CONFIG_DEVICE_DEV_SPLIT_INPUT_MUTE          "input_mute"
#define CONFIG_DEVICE_DEV_SPLIT_FILTER_SETSYNC_L    "filter_setsync_L_%1"
#define CONFIG_DEVICE_DEV_SPLIT_FILTER_SETSYNC_R    "filter_setsync_R_%1"
#define CONFIG_DEVICE_DEV_SPLIT_FILTER_FILTERTYPE_L "filter_filtertype_L_%1"
#define CONFIG_DEVICE_DEV_SPLIT_FILTER_FILTERTYPE_R "filter_filtertype_R_%1"
#define CONFIG_DEVICE_DEV_SPLIT_FREQ_L              "freq_L_%1"
#define CONFIG_DEVICE_DEV_SPLIT_FREQ_R              "freq_R_%1"
#define CONFIG_DEVICE_DEV_SPLIT_OUTPUT_MUTE         "output_mute_%1"
#define CONFIG_DEVICE_DEV_SPLIT_OUTPUT_INVERT       "output_invert_%1"
#define CONFIG_DEVICE_DEV_SPLIT_OUTPUT_LEVLE        "output_level_%1"

#define CONFIG_DEVICE_DEV_SIG_GROUP "Signal"
#define CONFIG_DEVICE_DEV_SIG_MUTE  "mute"
#define CONFIG_DEVICE_DEV_SIG_LEVEL "level"
#define CONFIG_DEVICE_DEV_SIG_FREQ  "freq"

#define CONFIG_DEVICE_DEV_METER_GROUP    "Meter"
#define CONFIG_DEVICE_DEV_METER_USERNAME "userName_%1"

#define CONFIG_DEVICE_DEV_GPI_GROUP     "GPI"
#define CONFIG_DEVICE_DEV_GPI_GPINUM    "GPINUM"

#define CONFIG_DEVICE_DEV_FREQSHIFT_GROUP        "FreqShift"
#define CONFIG_DEVICE_DEV_FREQSHIFT_FREQ         "Freq"
#define CONFIG_DEVICE_DEV_FREQSHIFT_GAIN         "Gain"
#define CONFIG_DEVICE_DEV_FREQSHIFT_PHASE        "Phase"
#define CONFIG_DEVICE_DEV_FREQSHIFT_FREQMODE     "FreqMode"
#define CONFIG_DEVICE_DEV_FREQSHIFT_FREQSPAN    "FreqSpan"
#define CONFIG_DEVICE_DEV_FREQSHIFT_FREQSPEED    "FreqSpeed"

#define CONFIG_DEVICE_DEV_AUTOMIX_GROUP             "AutoMix"
#define CONFIG_DEVICE_DEV_AUTOMIX_THREADLEVEL       "ThreadLevel"
#define CONFIG_DEVICE_DEV_AUTOMIX_ATTACKTIME        "AttackTime"
#define CONFIG_DEVICE_DEV_AUTOMIX_ATTENSTEP         "AttenStep"
#define CONFIG_DEVICE_DEV_AUTOMIX_ATTENMAX          "AttenMax"
#define CONFIG_DEVICE_DEV_AUTOMIX_ATTENUATIONTYPE   "AttenuationType"
#define CONFIG_DEVICE_DEV_AUTOMIX_ATTENRESPONSETIME "AttenResponseTime"
#define CONFIG_DEVICE_DEV_AUTOMIX_RELEASETIME       "ReleaseTime"
#define CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME          "HoldTime"
#define CONFIG_DEVICE_DEV_AUTOMIX_DETECTORTIME      "DetectorTime"
#define CONFIG_DEVICE_DEV_AUTOMIX_DEPTHENABLE       "DepthEnable"
#define CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE        "DepthValue"
#define CONFIG_DEVICE_DEV_AUTOMIX_DEFAULTCH         "DefaultCh"
#define CONFIG_DEVICE_DEV_AUTOMIX_THREABOCENOISE    "ThreAboveNoise"
#define CONFIG_DEVICE_DEV_AUTOMIX_NOISEENABLE       "NoiseEnable"
#define CONFIG_DEVICE_DEV_AUTOMIX_NOISEVALUE        "NoiseValue"
#define CONFIG_DEVICE_DEV_AUTOMIX_INPUTMUTE         "InputMute_%1"
#define CONFIG_DEVICE_DEV_AUTOMIX_PREINGAIN         "PreInGain_%1"
#define CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE        "MixOutMute"
#define CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN        "MixOutGain"
#define CONFIG_DEVICE_DEV_AUTOMIX_LASTMIXON         "LastMicOn"
#define CONFIG_DEVICE_DEV_AUTOMIX_IDGATING          "IdGating"
#define CONFIG_DEVICE_DEV_AUTOMIX_BGPERCENTAGE      "BgPercentage"
#define CONFIG_DEVICE_DEV_AUTOMIX_MAXNOM            "MaxNOM"
#define CONFIG_DEVICE_DEV_AUTOMIX_DIRECTOUTSNOMATTEN  "DirectOutsNOMAtten"
#define CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_MANUAL       "InChAttr[%1]_manual"
#define CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_GATEMODE     "InChAttr[%1]_gateMode"
#define CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_PRIORITY     "InChAttr[%1]_priority"
#define CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_THRESHOLD    "InChAttr[%1]_threshold"
#define CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_POSTGATEMUTE "InChAttr[%1]_postGateMute"
#define CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_POSTGATEGAIN "InChAttr[%1]_postGateGain"

#define CONFIG_DEVICE_DEV_GAINRAMP_GROUP        "GainRamp"
#define CONFIG_DEVICE_DEV_GAINRAMP_TARGETTIMEA  "TargetTimeA"
#define CONFIG_DEVICE_DEV_GAINRAMP_TARGETTIMEB  "TargetTimeB"
#define CONFIG_DEVICE_DEV_GAINRAMP_TARGETGAINA  "TargetGainA"
#define CONFIG_DEVICE_DEV_GAINRAMP_TARGETGAINB  "TargetGainB"
#define CONFIG_DEVICE_DEV_GAINRAMP_MUTETIME     "MuteTime"
#define CONFIG_DEVICE_DEV_GAINRAMP_EFFECTGAIN   "EffectGain"

#define CONFIG_DEVICE_DEV_AEC_GROUP             "AEC"
#define CONFIG_DEVICE_DEV_AEC_MICINGAIN         "MicInGain"
#define CONFIG_DEVICE_DEV_AEC_LINEINGAIN        "LineInGain"
#define CONFIG_DEVICE_DEV_AEC_SPEAKOUTGAIN      "SpeakOutGain"
#define CONFIG_DEVICE_DEV_AEC_LINEOUTGAIN       "LineOutGain"
#define CONFIG_DEVICE_DEV_AEC_SPEAK2LINEGAIN    "Speak2LineGain"
#define CONFIG_DEVICE_DEV_AEC_SPEAK2LINESWITCH  "Speak2LineSwitch"
#define CONFIG_DEVICE_DEV_AEC_NRGAIN            "NRGain"

#define CONFIG_DEVICE_DEV_AEC_8K128_GROUP       "AEC_8K128"
#define CONFIG_DEVICE_DEV_AEC_8K128_BYPASS      "Bypass"
#define CONFIG_DEVICE_DEV_AEC_8K128_REFGAIN     "RefGain"
#define CONFIG_DEVICE_DEV_AEC_8K128_MICTOSPEAK  "MicToSpeak"
#define CONFIG_DEVICE_DEV_AEC_8K128_MICENABLE   "MicEnable"
#define CONFIG_DEVICE_DEV_AEC_8K128_RESPERCENT  "ResPercent"
#define CONFIG_DEVICE_DEV_AEC_8K128_RESENABLE   "ResEnable"
#define CONFIG_DEVICE_DEV_AEC_8K128_NRGAIN      "NRGain"
#define CONFIG_DEVICE_DEV_AEC_8K128_NRENABLE    "NREnable"
#define CONFIG_DEVICE_DEV_AEC_8K128_CNGAIN      "CNGain"
#define CONFIG_DEVICE_DEV_AEC_8K128_CNENABLE    "CNEnable"

#define CONFIG_DEVICE_DEV_NFC_GROUP             "NFC"
#define CONFIG_DEVICE_DEV_NFC_OUTGAIN           "OutGain"
#define CONFIG_DEVICE_DEV_NFC_OUTMODEL          "OutMode"
#define CONFIG_DEVICE_DEV_NFC_BANDWIDTH         "BandWidth"
#define CONFIG_DEVICE_DEV_NFC_FIXFILTERS        "Fixfilters"
#define CONFIG_DEVICE_DEV_NFC_DYNFILTERS        "Dynfilters"
#define CONFIG_DEVICE_DEV_NFC_MAXNOTCHDEPTH     "maxNotchDepth"

#define CONFIG_DEVICE_DEV_GAIN_GROUP            "Gain"
#define CONFIG_DEVICE_DEV_GAIN_MODE             "Mode"
#define CONFIG_DEVICE_DEV_GAIN_PHASE            "Phase"
#define CONFIG_DEVICE_DEV_GAIN_GAINVALUE        "GainValue"

#define CONFIG_DEVICE_DEV_CROSSFADER_GROUP      "Crossfader"
#define CONFIG_DEVICE_DEV_CROSSFADER_TIME       "Time"
#define CONFIG_DEVICE_DEV_CROSSFADER_CH         "Ch"
#define CONFIG_DEVICE_DEV_CROSSFADER_TYPE       "Type"

////////////////////Snapshot/////////////////////
#define CONFIG_DEVICE_DEV_SNAPSHOT_GROUP         "Snapshot"
#define CONFIG_DEVICE_DEV_SHAPSHOT_CURRENTPATTERN "CurrentPattern"
#define CONFIG_DEVICE_DEV_SHAPSHOT_SNAPNUM       "SnapNum"
//#define CONFIG_DEVICE_DEV_SHAPSHOT_RELEVANCETYPE "RelevanceType"
#define CONFIG_DEVICE_DEV_SNAPSHOT_PATTERN       "Pattern_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM "Unimodule_Num"
#define CONFIG_DEVICE_DEV_SNAPSHOT_DEVOUTCOUNT   "DevOutCount"
#define CONFIG_DEVICE_DEV_SNAPSHOT_DEVINCOUNT    "DevInCount"
#define CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME       "DevName"
#define CONFIG_DEVICE_DEV_SNAPSHOT_INPUTLAB      "InputLab_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUTLAB      "OutputLab_%1"

#define CONFIG_DEVICE_DEV_SNAPSHOT_DELAY_TYPE    "DelayType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_DELAY         "delay_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER_TYPE   "RouterType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_ROUTER        "router_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_MIXER_TYPE    "MixerType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_MIXER         "mixer_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_DYN_TYPE      "dynType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_DYN           "dyn_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_DUCKER_TYPE   "DuckerType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_DUCKER        "Ducker_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_EQ_TYPE       "eqType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_EQLISTNUM     "eqListNum"
#define CONFIG_DEVICE_DEV_SNAPSHOT_EQ            "eq_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_SPLIT_TYPE    "SplitType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_SPLIT         "Split_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_FILTER_TYPE   "filterType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_FILTER        "filter_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_SIGNAL_TYPE   "signalType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_SIGNAL        "signal_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_METER_TYPE    "MeterType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_METER         "meter_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_GPI_TYPE      "GpiType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_GPI           "gpi_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_FREQSHIFT_TYPE "freqshiftType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_FREQSHIFT     "freqshift_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE  "AutomixType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX       "automix_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_GAINRAMP_TYPE "GainRampType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_GAINRAMP      "GainRamp_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_AEC_TYPE      "AecType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_AEC           "Aec_%1"
//#define CONFIG_DEVICE_DEV_SNAPSHOT_AEC8K128      "Aec_8k128_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_NFC_TYPE      "NFCType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_NFC           "NFC_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_INPUT_TYPE    "InputType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_INPUT         "Input_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUT_TYPE   "OutputType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_OUTPUT        "Output_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_GAIN_TYPE     "GainType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_GAIN          "Gain_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_GAINRAMP_TYPE "GainRampType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_GAINRAMP      "GainRamp_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_CROSSFADER_TYPE  "CrossfaderType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_CROSSFADER       "Crossfader_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_DUCKER_TYPE      "DuckerType_%1"
#define CONFIG_DEVICE_DEV_SNAPSHOT_DUCKER           "Ducker_%1"
/* End: Config Save file Macro Define */

/* Begin: Alarm Config Save file Macro Define */
#define ALARM_GROUP "Alarm_%1"
#define ALARM_CONFIG_ENABLE "enable"
#define ALARM_CONFIG_CH1_VOLTAGE_CHECKMAX "ch1_voltage_checkmax"
#define ALARM_CONFIG_CH1_VOLTAGE_MAXVALUE "ch1_voltage_maxvalue"
#define ALARM_CONFIG_CH1_VOLTAGE_CHECKMIN "ch1_voltage_checkmin"
#define ALARM_CONFIG_CH1_VOLTAGE_MINVALUE "ch1_voltage_minvalue"
#define ALARM_CONFIG_CH2_VOLTAGE_CHECKMAX "ch2_voltage_checkmax"
#define ALARM_CONFIG_CH2_VOLTAGE_MAXVALUE "ch2_voltage_maxvalue"
#define ALARM_CONFIG_CH2_VOLTAGE_CHECKMIN "ch2_voltage_checkmin"
#define ALARM_CONFIG_CH2_VOLTAGE_MINVALUE "ch2_voltage_minvalue"
#define ALARM_CONFIG_CH1_CURRENT_CHECKMAX "ch1_current_checkmax"
#define ALARM_CONFIG_CH1_CURRENT_MAXVALUE "ch1_current_maxvalue"
#define ALARM_CONFIG_CH1_CURRENT_CHECKMIN "ch1_current_checkmin"
#define ALARM_CONFIG_CH1_CURRENT_MINVALUE "ch1_current_minvalue"
#define ALARM_CONFIG_CH2_CURRENT_CHECKMAX "ch2_current_checkmax"
#define ALARM_CONFIG_CH2_CURRENT_MAXVALUE "ch2_current_maxvalue"
#define ALARM_CONFIG_CH2_CURRENT_CHECKMIN "ch2_current_checkmin"
#define ALARM_CONFIG_CH2_CURRENT_MINVALUE "ch2_current_minvalue"
#define ALARM_CONFIG_TEMPERATURE_CHECKMAX "ch1_temperature_checkmax"
#define ALARM_CONFIG_TEMPERATURE_MAXVALUE "ch1_temperature_maxvalue"
#define ALARM_CONFIG_TEMPERATURE_CHECKMIN "ch1_temperature_checkmin"
#define ALARM_CONFIG_TEMPERATURE_MINVALUE "ch1_temperature_minvalue"
#define ALARM_CONFIG_CH1_METER_CHECKMAX "ch1_meter_checkmax"
#define ALARM_CONFIG_CH1_METER_MAXVALUE "ch1_meter_maxvalue"
#define ALARM_CONFIG_CH1_METER_CHECKMIN "ch1_meter_checkmin"
#define ALARM_CONFIG_CH1_METER_MINVALUE "ch1_meter_minvalue"
#define ALARM_CONFIG_CH2_METER_CHECKMAX "ch2_meter_checkmax"
#define ALARM_CONFIG_CH2_METER_MAXVALUE "ch2_meter_maxvalue"
#define ALARM_CONFIG_CH2_METER_CHECKMIN "ch2_meter_checkmin"
#define ALARM_CONFIG_CH2_METER_MINVALUE "ch2_meter_minvalue"
/* End: Alarm Config Save file Macro Define */

/* Begin: Monitor Save file Macro Define */
#define MONITOR_GROUP       "Monitor"
#define MONITOR_VIEW_GROUP  "Monitor_view_%1"
#define MONITOR_DEV_GROUP   "MonDev_%1"
#define MONITOR_DEV_TYPE    "MonDevType"
#define MONITOR_DEV_POS     "MonDevPos"
#define MONITOR_DEV_NAME    "MonDevName"
#define MONITOR_DEV_DEVIP   "MonDevIp"
#define MONITOR_DEV_PORT    "MonDevPort"
#define MONITOR_INFOMATION_GROUP    "Infomation"
#define MONITOR_SCENESIZE           "SceneSize"
#define MONITOR_GROUP_COUNT         "groupCount"
#define MONITOR_VIEW_DEV_COUNT      "view_%1_devCount"
/* End: Monitor Save file Marco Define */

/* Begin: UCI Save file Marco Define */
#define UCI_GROUP               "UCI_%1"
#define UCI_DEV_NAME            "UciDevName"
#define UCI_DEV_AES70DEV_MAC    "UciAES70Dev_Mac"
#define UCI_DEV_AES70DEV_IP     "UciAES70Dev_Ip"
#define UCI_DEV_AES70DEV_DEVTYPE    "UciAES70Dev_DevType"
#define UCI_DEV_AES70DEV_CNAME  "UciAES70Dev_CustomName"
#define UCI_CTRLWID_COUNT       "Uci_CtrlWid_Count"
#define UCI_CTRLWID_NO          "Uci_CtrlWid_No_%1"
#define UCI_CTRLWID_TYPE        "Uci_CtrlWid_type"
#define UCI_CTRLWID_TYPENAME    "Uci_CtrlWid_TypeName"
#define UCI_CTRLWID_DEVNAME     "Uci_CtrlWid_DevName"
#define UCI_CTRLWID_USERLAB     "Uci_CtrlWid_UserLab"
#define UCI_CTRLWID_MAXGAIN     "Uci_CtrlWid_MaxGain"
#define UCI_CTRLWID_MINGAIN     "Uci_CtrlWid_MinGain"
#define UCI_CTRLWID_MIXERNODESSWITCH    "uci_CtrlWid_MixerNodesSwitch"
#define UCI_CTRLWID_CH          "Uci_CtrlWid_Ch"
#define UCI_CTRLWID_VISIBLE     "Uci_CtrlWid_Visible"
#define UCI_CTRLWID_PAGE        "Uci_CtrlWid_Page"
#define UCI_CTRLWID_POS         "Uci_CtrlWid_Pos"

#define CONFIG_DEVICE_DEV_UCI_GROUP         "UserControlInterface"
#define CONFIG_DEVICE_DEV_UCI_PROC_GROUP    "UciProcessor"
#define CONFIG_DEVICE_DEV_UCI_PROC_COUNT    "UciProcessorCount"
#define CONFIG_DEVICE_DEV_UCI_PROC_DATA     "UciProcessorData_%1"
#define CONFIG_DEVICE_DEV_UCI_PROC_MAC      "MAC"
#define CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_GROUP    "PeripheralDevice"
#define CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_COUNT    "PeripheralDeviceCount"
#define CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DATA     "PeripheralDeviceData_%1"
#define CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DEVNAME  "PeripheralDevice_DevName"
#define CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DEVTYPE  "PeripheralDevice_DevType"
#define CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DEVIP    "PeripheralDevice_Ip"
#define CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DEVMAC   "PeripheralDevice_Mac"

/* End: UCI Save file Marco Define */

/* Begin: Solution Explorer Marco Define */
#define SOLUTION_DATA_GROUP                "SolutionDataGroup"
#define SOLUTION_DATA_OPENPAGES         "OpenPages"
#define SOLUTION_DATA_CUROPENPAGES         "CurrentOpenPages"
#define SOLUTION_DATA_ACTIVEWINNAME        "ActiveWinName"
/* End: Solution Save file Marco Define */

/* Begin: AppData Explorer Marco Define */
#define APPDATA_PROJECTEXPLORER     "ProjectExplorer"
#define APPDATA_RECENTPROJECTS      "RecentProjects"
#define APPDATA_FILENAMES           "FileNames"
#define APPDATA_WORINGPATH          "WorkingPath"
#define APPDATA_DEFAULTWORKINGPATH  "DefaultWorkingPath"
/* End: AppData Explorer Marco Define */


enum MsgType{
    Msg_Type_Unknown = 0xFFFF,
    Msg_Type_Probe = 0x0000,
    Msg_Type_ProbeAck = 0x0001,
    Msg_Type_GetInit = 0x0002,
    Msg_Type_GetInitAck = 0x0003,
    Msg_Type_Heartbeat = 0x0004,
    Msg_Type_HeartbeatAck = 0x0005,

    Msg_Type_NAP_FileList = 0x0100,

    Msg_Type_NAP_Phantom = 0x8000,
    Msg_Type_NAP_MicGain = 0x8001,
    Msg_Type_NAP_AlgorithmOverview = 0x8002,
    Msg_Type_NAP_AlgorithmDetail = 0x8003,
    Msg_Type_NAP_AlgorithmStart = 0x8004,
    Msg_Type_NAP_AlgorithmSave = 0x8005,
    Msg_Type_NAP_OnlineUpdate = 0x8006,
    Msg_Type_NAP_SetIP = 0x8007,
    Msg_Type_NAP_MonitorMeter = 0x8008,//Meter模块
    Msg_Type_NAP_AutomaticMixerMeter = 0x8009,//混音模块电平表
    Msg_Type_NAP_NetworkStreamModeSelect = 0x800A,
    Msg_Type_NAP_Mixer = 0x800B,
    Msg_Type_NAP_FileToDeviceStart = 0x800C,
    Msg_Type_NAP_FileToDevice = 0x800D,
    Msg_Type_NAP_FileToDeviceEnd = 0x800E,
    Msg_Type_NAP_FileFromDeviceStart = 0x800F,
    Msg_Type_NAP_FileFromDevice = 0x8010, /*=0x0810,*/
    Msg_Type_NAP_FileFromDeviceEnd = 0x8011,
    Msg_Type_NAP_ModuleNameOverview = 0x8012,
    Msg_Type_NAP_ModuleNameOverviewSave = 0x8013,
    Msg_Type_NAP_ParaChangedFromUser = 0x8014,
    Msg_Type_NAP_SceneChangedFromUser = 0x8015,
    Msg_Type_NAP_ModuleRtnValue = 0x8016,
    Msg_Type_NAP_InOutLevel = 0x8017,
    Msg_Type_NAP_CenterCtrl_Ack = 0x8018,

    Msg_Type_NAP_MicGroupSet = 0x8100,  //分组序号
    Msg_Type_NAP_ClearScene = 0x8FFF,   //清除场景

    Msg_Type_STM32_POE_AD_Gain = 0x8000,
    Msg_Type_STM32_POE_AD_Gain_Ack,
    Msg_Type_STM32_POE_AD_Eq,
    Msg_Type_STM32_POE_AD_Eq_Ack,
    Msg_Type_STM32_POE_AD_Dyn,
    Msg_Type_STM32_POE_AD_Dyn_Ack,
    Msg_Type_STM32_POE_Save,
    Msg_Type_STM32_POE_Save_Ack,
    Msg_Type_STM32_POE_Eq,
    Msg_Type_STM32_POE_Eq_Ack,
    Msg_Type_STM32_POE_Dyn,
    Msg_Type_STM32_POE_Dyn_Ack,
    Msg_Type_STM32_POE_Xover,
    Msg_Type_STM32_POE_Xover_Ack,
    Msg_Type_STM32_POE_Phantom,
    Msg_Type_STM32_POE_Phantom_Ack,
    Msg_Type_STM32_POE_Monitor_Msg,
    Msg_Type_STM32_POE_Monitor_Msg_Ack,
    Msg_Type_STM32_POE_Start_Monitor,
    Msg_Type_STM32_POE_Start_Monitor_Ack,
    Msg_Type_STM32_POE_Stop_Monitor,
    Msg_Type_STM32_POE_Stop_Monitor_Ack,

//    Msg_Type_STM32_POE_Other = 0x8000,
//    Msg_Type_STM32_POE_Other_Ack,
//    Msg_Type_STM32_POE_EQ,
//    Msg_Type_STM32_POE_EQ_Ack,
//    Msg_Type_STM32_POE_DYN,
//    Msg_Type_STM32_POE_DYN_Ack,
//    Msg_Type_STM32_POE_Save,
//    Msg_Type_STM32_POE_Save_Ack,
//    Msg_Type_STM32_POE_Loading,
//    Msg_Type_STM32_POE_Loading_Ack,

    Msg_Type_iMXRT1052_POE_ChannelGain = 0x8000,
    Msg_Type_iMXRT1052_POE_ChannelGain_Ack,
    Msg_Type_iMXRT1052_POE_ChannelEq,
    Msg_Type_iMXRT1052_POE_ChannelEq_Ack,
    Msg_Type_iMXRT1052_POE_Mixer,
    Msg_Type_iMXRT1052_POE_Mixer_Ack,
    Msg_Type_iMXRT1052_POE_Xover,
    Msg_Type_iMXRT1052_POE_Xover_Ack,
    Msg_Type_iMXRT1052_POE_Eq,
    Msg_Type_iMXRT1052_POE_Eq_Ack,
    Msg_Type_iMXRT1052_POE_Gain,
    Msg_Type_iMXRT1052_POE_Gain_Ack,
    Msg_Type_iMXRT1052_POE_Dyn,
    Msg_Type_iMXRT1052_POE_Dyn_Ack,
    Msg_Type_iMXRT1052_POE_Delay,
    Msg_Type_iMXRT1052_POE_Delay_Ack,
    Msg_Type_iMXRT1052_POE_Route = 0x8010,
    Msg_Type_iMXRT1052_POE_Route_Ack,
    Msg_Type_iMXRT1052_POE_Save,
    Msg_Type_iMXRT1052_POE_Save_Ack,
    Msg_Type_iMXRT1052_POE_MIC_Gain,
    Msg_Type_iMXRT1052_POE_MIC_Gain_Ack,
    Msg_Type_iMXRT1052_POE_MIC_Phantom,
    Msg_Type_iMXRT1052_POE_MIC_Phantom_Ack,
    Msg_Type_iMXRT1052_POE_MIC_Open,
    Msg_Type_iMXRT1052_POE_MIC_Open_Ack,
    Msg_Type_iMXRT1052_POE_Start_Monitor,
    Msg_Type_iMXRT1052_POE_Start_Monitor_Ack,
    Msg_Type_iMXRT1052_POE_Stop_Monitor,
    Msg_Type_iMXRT1052_POE_Stop_Monitor_Ack,
    Msg_Type_iMXRT1052_POE_Monitor_Msg,
    Msg_Type_iMXRT1052_POE_ChannelDyn,
    Msg_Type_iMXRT1052_POE_ChannelDyn_Ack = 0x8020,
    Msg_Type_iMXRT1052_POE_ChannelXover,
    Msg_Type_iMXRT1052_POE_ChannelXover_Ack,
    Msg_Type_iMXRT1052_POE_Eq15B,
    Msg_Type_iMXRT1052_POE_Eq15B_Ack,
    Msg_Type_iMXRT1052_POE_DIRECTIVITY,
    Msg_Type_iMXRT1052_POE_DIRECTIVITY_Ack,
    Msg_type_iMXRT1052_POE_Control_CMD,
    Msg_type_iMXRT1052_POE_Control_CMD_Ack,

    Msg_Type_iMXRT1052_POE_HuiShengKeJi_Mic = 0x8100,
    Msg_Type_iMXRT1052_POE_HuiShengKeJi_Mic_Ack,
    Msg_Type_iMXRT1052_POE_HuiShengKeJi_AP60_MixerMode,
    Msg_Type_iMXRT1052_POE_HuiShengKeJi_AP60_MixerMode_Ack,
    Msg_Type_iMXRT1052_POE_HuiShengKeJi_AP400_RouteMode,
    Msg_Type_iMXRT1052_POE_HuiShengKeJi_AP400_RouteMode_Ack,
    Msg_Type_iMXRT1052_POE_HuiShengKeJi_SwitchThreshold,
    Msg_Type_iMXRT1052_POE_HuiShengKeJi_SwitchThreshold_Ack,
    Msg_Type_iMXRT1052_POE_HuiShengKeJi_DefaultOutput,
    Msg_Type_iMXRT1052_POE_HuiShengKeJi_DefaultOutput_Ack,
    Msg_Type_iMXRT1052_POE_HuiShengKeJi_AutoRecovery,
    Msg_Type_iMXRT1052_POE_HuiShengKeJi_AutoRecovery_Ack,
//    Msg_Type_iMXRT1052_POE_MIC_Gain = 0x8000,
//    Msg_Type_iMXRT1052_POE_MIC_Gain_Ack,
//    Msg_Type_iMXRT1052_POE_MIC_Phantom,
//    Msg_Type_iMXRT1052_POE_MIC_Phantom_Ack,
//    Msg_Type_iMXRT1052_POE_Other,
//    Msg_Type_iMXRT1052_POE_Other_Ack,
//    Msg_Type_iMXRT1052_POE_EQ,
//    Msg_Type_iMXRT1052_POE_EQ_Ack,
//    Msg_Type_iMXRT1052_POE_DYN,
//    Msg_Type_iMXRT1052_POE_DYN_Ack,
//    Msg_Type_iMXRT1052_POE_Divider,
//    Msg_Type_iMXRT1052_POE_Divider_Ack,
//    Msg_Type_iMXRT1052_POE_Save,
//    Msg_Type_iMXRT1052_POE_Save_Ack,

    MSG_TYPE_OTHER = 0x00,
    MSG_TYPE_EQ = 0x01,
    MSG_TYPE_DYN = 0x02,
    MSG_TYPE_DIVIDER = 0x03,
    MSG_TYPE_SAVE = 0x04,
    MSG_TYPE_INIT = 0x05,
    MSG_TYPE_OUTPUT_GAIN = 0x06,
    MSG_TYPE_VOLUME_CURRENT = 0x07,
    MSG_TYPE_UPDATE_PACKAGE = 0x08, /* 网络更新固件功能 */
    MSG_TYPE_ADC_VALUE = 0x7E,
    MSG_TYPE_DEV_ISALIVING = 0x7D,

//    /* TRN */
//    MSG_TYPE_TRN_INIT = 0x00,
//    MSG_TYPE_DOWNLOAD_MIC_48V,
//    MSG_TYPE_DOWNLOAD_MIC_GAIN,
//    MSG_TYPE_DOWNLOAD_ALGORITHM_OVERVIEW,
//    MSG_TYPE_DOWNLOAD_ALGORITHM_DETAIL,
//    MSG_TYPE_DOWNLOAD_ALGORITHM_START,
//    MSG_TYPE_REALTIME_UPDATE,
//    MSG_TYPE_SET_IP,
//    MSG_TYPE_SAVE_ALGORITHM,
//    MSG_TYPE_GET_METERA,//Meter模块
//    MSG_TYPE_AUTOMATIC_MIXER_METER,//混音模块电平表

//    MSG_TYPE_SEARCH_DEVS = 0x70,
//    MSG_TYPE_HEARTBEAT = 0x7F,
//    MSG_TYPE_DEV_INIT = 0x80 //获取设备初始化信息
};

enum PlatformType{
    Platform_DSP = 1,
    Platform_X86,
    Platform_ARM,
};

enum  DevPortType {
    PortType_MONAURAL = 0,//模拟
    PortType_Dante,
    PortType_AES67,
    PortType_Config,
    PortType_CtrlPin,     //控制端口
    PortType_Inflection,  //线段拐点
    PortType_ChannelBus,  //蛇形连接器总线
    PortType_RS232,       //串口
    PortType_USB,         //USB
};

/*
 * bit7  bit6  bit5 bit4 bit3 bit2 bit1 bit0
 * Dante AES67 trn  amp  spk
 * */
enum DevType {
    Dev_Type_PC = 0,
    Dev_Type_Normal_PC = 1,
    Dev_Type_Normal_CD = 2,
    Dev_Type_Normal_MIC = 3,
    Dev_Type_Normal_SPK = 4,
    Dev_Type_Normal_AMP = 5,
    Dev_Type_NAP_Zynq7020_16Analog_16AES67 = 6,
    Dev_Type_NAP_Zynq7020_16Analog_16AES67DanteMixer = 7,
    Dev_Type_VirtualSoundCard_Dante = 8,
    Dev_Type_VirtualSoundCard_AES67 = 9,
    Dev_Type_STM32_POE_AES67_SPK_0_1 = 10,
    Dev_Type_STM32_POE_AES67_SPK_0_2 = 11,
    Dev_Type_STM32_POE_AES67_MIC_2_0 = 12,
    Dev_Type_RT1052_POE_AES67_MIC_4_0 = 13,
    Dev_Type_RT1052_POE_AES67_LINE_4_4 = 14,
    Dev_Type_RT1052_POE_AES67_MIC_2_0 = 15,
    Dev_Type_RT1052_POE_AES67_LINE_0_2 = 16,
    Dev_Type_RT1052_AP60 = 17,
    Dev_Type_RT1052_AMPLIFIER_AA4500 = 18,
    Dev_Type_RT1052_MONITORING_AIDS = 19,
    Dev_Type_NAP_X86_16Analog_128AES67 = 20,   //KEMU:128 X86
    Dev_Type_NAP_Zynq7020_16Analog_48AES67 = 21,
    Dev_Type_RT1052_AP400 = 22,
    Dev_Type_RT1052_AP60_Bluetooth = 23,
    Dev_Type_Zynq7010_AES3ToAES67_DongFangJiaLian = 24,
    Dev_Type_NAP_Zynq7020_16Analog_16Dante = 25,
    Dev_Type_NAP_Zynq7020_16Analog_16Dante_Type1 = 26,
    Dev_Type_RT1052_AP60_Type1 = 27,
    Dev_Type_RT1052_POE_AES67_MIC_2_0_Type1 = 28,
    Dev_Type_RT1052_POE_AES67_MIC_2_0_Type2 = 29,
    Dev_Type_RT1052_POE_AES67_MIC_2_0_Type3 = 30,
    Dev_Type_RT1052_AP60_Type2 = 31,
    Dev_Type_RT1052_AMPLIFIER_AA2500 = 32,
    Dev_Type_RT1052_AMPLIFIER_AA4500_Type1 = 33,
    Dev_Type_RT1052_DR3020 = 34,
    Dev_Type_RT1052_Amplifier_DongFangJiaLian = 35,
    Dev_Type_NAP_Zynq7020_64AES67 = 36,
    Dev_Type_NAP_Zynq7020_16Analog_32Dante = 37,
    Dev_Type_NAP_X86_32Analog_256AES67 = 38,        //KEMU:256  X86
    Dev_Type_NAP_FT2000_256AES67 = 39,              //飞腾arm处理器

    Dev_Type_AES67_SPK = 0x50,
    Dev_Type_AES67_MONITOR_SPK,

    Dev_Type_DNTE_TRN = 0xA0,
    Dev_Type_DNTE_AMP = 0x90,
    Dev_Type_DNTE_SPK = 0x88,

    Dev_Type_NAP_STM32H750_8A8N = 0x0100,   //(8*8处理器) FT:MBP-50
    //方图定制设备
    Dev_Type_NAP_150_FangTu,                //FT: MBP-150（ARM 模拟16X16）
    Dev_Type_NAP_100_FangTu,                //FT: MBP100-DANTE  2023-2-15新增
    Dev_Type_NAP_300_FangTu,                //FT: MBP300  数字音频中央处理系统  2023-2-15新增
    Dev_Type_NAP_400_FangTu,                //FT: MBP400  数字音频中央处理系统  2023-2-15新增

//    Dev_Type_Mixer_64_64_FangTu,            //FT: M-64*64 （原型号：Mixer-64X64 原有MBP100i更改）
    Dev_Type_Mixer_128_128_FangTu,          //FT: M-128*128 （原型号：Mixer-128X128 ARM版本）
    Dev_Type_Mixer_256_256_FangTu,          //FT: M-256*256 （原型号：Mixer-256X256 ARM版本）

    Dev_Type_RT1052_POE_AES67_MIC_8_0,      //FT: input-80接口盒
    Dev_Type_RT1052_POE_AES67_LINE_0_8,     //FT: output-08接口盒

    Dev_Type_CS50_FangTu,                   //FT: CS50  5寸POE吸顶网络音箱
    Dev_Type_CS80_FangTu,                   //FT: CS80  8寸POE吸顶网络音箱
    Dev_Type_CS50A_FangTu,                  //FT: CS50A （A表示：模拟+网络接口）
    Dev_Type_CS60A_FangTu,                  //FT: CS60A （A表示：模拟+网络接口）
    Dev_Type_CS80A_FangTu,                  //FT: CS80A （A表示：模拟+网络接口）

    Dev_Type_FT_4D300N_FangTu,              //FT: FT-4A300N  4X300W四通道网络功放
    Dev_Type_FT_4D600N_FangTu,              //FT: FT-4A600N  4X600W四通道网络功放
    Dev_Type_FT_4D800N_FangTu,              //FT: FT-4A800N  4X800W四通道网络功放
    Dev_Type_FT_4A1200N_FangTu,             //FT: FT-4A1200N
    Dev_Type_FT_4A2000N_FangTu,             //FT: FT-4A2000N

    Dev_Type_FT_L403N_FangTu,               //FT: FT-L403N   2023-2-15新增
    Dev_Type_Dir_L445_FangTu,               //FT: Dir-L445
    Dev_Type_Dir_X_10_12_16_FangTu,         //FT: Dir-X(10,12,16)

    Dev_Type_AAF_CS_6_FangTu,               //FT: 双备份网络有源全频同轴音箱 6寸（LC102P：小辫子）
    Dev_Type_AAF_CS_8_FangTu,               //FT: 双备份网络有源全频同轴音箱 8寸
    Dev_Type_AAF_CS_10_FangTu,              //FT: 双备份网络有源全频同轴音箱 10寸
    Dev_Type_AAF_CS_12_FangTu,              //FT: 双备份网络有源全频同轴音箱 12寸
    Dev_Type_AAF_CS_15_FangTu,              //FT: 双备份网络有源全频同轴音箱 15寸
    Dev_Type_AAF_CS_18_FangTu,              //FT: 双备份网络有源全频同轴音箱 18寸

    Dev_Type_DBN_4u_FangTu,                 //FT: 双备份网络组合音柱 4单元（LC102P：小辫子）
    Dev_Type_DBN_8_10u_FangTu,              //FT: 双备份网络组合音柱 8/10单元（LC102P：小辫子）

    Dev_Type_FionTu_WMIC_FangTu,            //FT: FionTu-WMIC (6x8模组+ADC DAC单板)
    Dev_Type_I_Mic_Center_DN_FangTu,        //FT: I Mic Center/DN （6X8模组 +ADC DAC单板）

    Dev_Type_Switch_16x32_FangTu,           //FT: 交换机
    Dev_Type_Switch_POE_16x32_FangTu,       //FT: POE交换机
    Dev_Type_Room = 0xFFFE,   //房间
    Dev_Type_Unknow = 0xFFFF, //未确定设备
};

enum CfgType{
    FCT_FAMILY_MASK  = 0x0000FF00,

    CFG_TYPE_2_GROUP_TYPE = 8,//子类型到父类型的偏移量
    //延时器
    CFG_TYPE_DELAY_5 = 0x0100,  //5ms
    CFG_TYPE_DELAY_10,          //10ms
    CFG_TYPE_DELAY_20,          //20ms
    CFG_TYPE_DELAY_50,          //50ms
    CFG_TYPE_DELAY_100,         //100ms
    CFG_TYPE_DELAY_500,         //500ms
    CFG_TYPE_DELAY_1000,        //1000ms
    CFG_TYPE_HPDELY_1   = 0x0110, //最大延时1ms
    CFG_TYPE_HPDELY_5 	 = 0x0111, //最大延时5ms
    CFG_TYPE_HPDELY_10  = 0x0112,
    CFG_TYPE_HPDELY_20  = 0x0113,
    CFG_TYPE_HPDELY_50  = 0x0114,

    //路由器
    CFG_TYPE_ROUTER_4 = 0x0200, //4*4
    CFG_TYPE_ROUTER_8,          //8*8
    CFG_TYPE_ROUTER_16,         //16*16
    CFG_TYPE_ROUTER_32,         //32*32
    CFG_TYPE_ROUTER_48,         //48*48
    CFG_TYPE_ROUTER_64,         //64*64
    CFG_TYPE_ROUTER_96,         //96*96
    CFG_TYPE_ROUTER_128,        //128*128
    CFG_TYPE_ROUTER_192,        //192*192
    CFG_TYPE_ROUTER_256,        //256*256

    //混音器
    CFG_TYPE_MIXER_4X1 = 0x0300,//4*1
    CFG_TYPE_MIXER_4X2,         //4*2
    CFG_TYPE_MIXER_4X4,         //4*4
    CFG_TYPE_MIXER_8X1,         //8*1
    CFG_TYPE_MIXER_8X2,         //8*2
    CFG_TYPE_MIXER_8X4,         //8*4
    CFG_TYPE_MIXER_8X8,         //8*8
    CFG_TYPE_MIXER_16X16,       //16*16
    CFG_TYPE_MIXER_32X32,       //32*32
    CFG_TYPE_MIXER_64X64,       //64*64

    //动态处理
    CFG_TYPE_DYN_AUTO = 0x0400, //自动增益
    CFG_TYPE_DYN_COMP,          //压缩器
    CFG_TYPE_DYN_LIMIT,         //限制器
    CFG_TYPE_DYN_NOISE,         //噪声门
    CFG_TYPE_DYN_EXPAND,        //扩展器
    CFG_TYPE_DYN_PEAK,          //削峰器,取消该模型

    //均衡器
    CFG_TYPE_EQ_1 = 0x0500,     //1波段
    CFG_TYPE_EQ_3,              //3波段
    CFG_TYPE_EQ_5,              //5波段
    CFG_TYPE_EQ_7,              //7波段
    CFG_TYPE_EQ_10,             //10波段
    CFG_TYPE_EQ_16,             //16波段
    CFG_TYPE_EQ_1X3,            //31geq,1/3倍频
    CFG_TYPE_EQ_2X3,            //15geq,2/3倍频
    CFG_TYPE_EQ_3X3,            //10geq,1倍频

    //滤波器
    CFG_TYPE_FILTE_LP = 0x0600, //低通
    CFG_TYPE_FILTE_HP,          //高通
    CFG_TYPE_FILTE_LS,          //低通斜坡
    CFG_TYPE_FILTE_HS,          //高通斜坡
    CFG_TYPE_FILTER_BP = 0x0604,//带通滤波器
    CFG_TYPE_FILTER_AP = 0x0605,//全通滤波器
    //分频器
    CFG_TYPE_SPLIT_2 = 0x0700,  //2输出
    CFG_TYPE_SPLIT_3,           //3输出
    CFG_TYPE_SPLIT_4,           //4输出

    //信号发生器
    CFG_TYPE_SIG_SINE = 0x0800, //正弦波
    CFG_TYPE_SIG_NOISE,         //白噪声
    CFG_TYPE_SIG_PINKNOISE,

    //仪表显示
    CFG_TYPE_METER_1 = 0x0900,  //1通道
    CFG_TYPE_METER_2,           //2通道
    CFG_TYPE_METER_4,           //4通道
    CFG_TYPE_METER_8,           //8通道

    //输入输出
    CFG_TYPE_INPUT_1   = 0x0A00,
    CFG_TYPE_INPUT_2   = 0x0A01,
    CFG_TYPE_INPUT_4   = 0x0A02,
    CFG_TYPE_INPUT_8   = 0x0A03,
    CFG_TYPE_INPUT_16  = 0x0A04,

    CFG_TYPE_OUTPUT_1      = 0x0B00,
    CFG_TYPE_OUTPUT_2      = 0x0B01,
    CFG_TYPE_OUTPUT_4      = 0x0B02,
    CFG_TYPE_OUTPUT_8      = 0x0B03,
    CFG_TYPE_OUTPUT_16     = 0x0B04,

    CFG_TYPE_GPI     = 0x0C00,  //GPI消防强切，根据GPI信号选择输出

    //移频
    CFG_TYPE_FREQSHIFT = 0x0D00,//啸叫抑制（移频法）
    CFG_TYPE_FREQSHIFT_PLUS,

    //自动混音
    CFG_TYPE_AUTOMIX_GS_4 = 0x0E00, //自动混音（共享增益法）
    CFG_TYPE_AUTOMIX_GS_8,
    CFG_TYPE_AUTOMIX_GS_16,
    CFG_TYPE_AUTOMIX_GS_32,
    CFG_TYPE_AUTOMIXNGAT_4			= 0x0E10,
    CFG_TYPE_AUTOMIXNGAT_8			= 0x0E11,
    CFG_TYPE_AUTOMIXNGAT_16         = 0x0E12,
    CFG_TYPE_AUTOMIXNGAT_32         = 0x0E13,
    CFG_TYPE_AUTOMIXNGRT_4			= 0x0E20,
    CFG_TYPE_AUTOMIXNGRT_8			= 0x0E21,
    CFG_TYPE_AUTOMIXNGRT_16		= 0x0E22,
    CFG_TYPE_AUTOMIXNGRT_32		= 0x0E23,

    CFG_TYPE_AEC_2_2 = 0x0F00,  //硬件AEC

    CFG_TYPE_SOFTFIR_MONO256    = 0x1000, //FIR滤波器
    CFG_TYPE_SOFTFIR_MONO512,
    CFG_TYPE_SOFTFIR_MONO1024,
    CFG_TYPE_SOFTFIR_STEREO256  = 0x1010,
    CFG_TYPE_SOFTFIR_STEREO512,
    CFG_TYPE_SOFTFIR_STEREO1024,
    CFG_TYPE_SOFTFIR_4R256          = 0x1020,
    CFG_TYPE_SOFTFIR_4R512,
    CFG_TYPE_SOFTFIR_4R1024,

    CFG_TYPE_NFC1_8NF				= 0x1100,//反馈抑制器
    CFG_TYPE_NFC1_16NF				= 0x1101,
    CFG_TYPE_NFC1_24NF				= 0x1102,
    CFG_TYPE_NFC1_32NF				= 0x1103,

    CFG_TYPE_PDUCKER_4				= 0x1200,//闪避器
    CFG_TYPE_PDUCKER_8				= 0x1201,
    CFG_TYPE_PDUCKER_16             = 0x1202,
    CFG_TYPE_PDUCKER_32             = 0x1203,

    CFG_TYPE_GAINRAMP_1             = 0x1300,//淡入淡出增益模块
    CFG_TYPE_GAINRAMP_2             = 0x1301,
    CFG_TYPE_GAINRAMP_4             = 0x1302,
    CFG_TYPE_GAINRAMP_8             = 0x1303,
    CFG_TYPE_GAINRAMP_16			= 0x1304,
    CFG_TYPE_GAINRAMP_32			= 0x1305,

    CFG_TYPE_GAIN_1                 = 0x1400,//增益模块
    CFG_TYPE_GAIN_2                 = 0x1401,
    CFG_TYPE_GAIN_4                 = 0x1402,
    CFG_TYPE_GAIN_8                 = 0x1403,
    CFG_TYPE_GAIN_16				= 0x1404,
    CFG_TYPE_GAIN_32				= 0x1405,

    CFG_TYPE_CROSSFADER_2X1         = 0x1500,//信道交叉
    CFG_TYPE_CROSSFADER_4X2         = 0x1501,
    CFG_TYPE_CROSSFADER_8X4         = 0x1502,
    CFG_TYPE_CROSSFADER_16X8		= 0x1503,
    CFG_TYPE_CROSSFADER_32X16		= 0x1504,

    CFG_TYPE_SOFTAEC_8K64           = 0X1600,//软件AEC
    CFG_TYPE_SOFTAEC_8K96           = 0X1601,
    CFG_TYPE_SOFTAEC_8K128          = 0X1602,
    CFG_TYPE_SOFTAEC_8K160          = 0X1603,
    CFG_TYPE_SOFTAEC_8K192          = 0X1604,
    CFG_TYPE_SOFTAEC_8K224          = 0X1605,
    CFG_TYPE_SOFTAEC_8K256          = 0X1606,
    CFG_TYPE_SOFTAEC_16K64          = 0X1610,
    CFG_TYPE_SOFTAEC_16K96          = 0X1611,
    CFG_TYPE_SOFTAEC_16K128         = 0X1612,
    CFG_TYPE_SOFTAEC_16K160         = 0X1613,
    CFG_TYPE_SOFTAEC_16K192         = 0X1614,
    CFG_TYPE_SOFTAEC_16K224         = 0X1615,
    CFG_TYPE_SOFTAEC_16K256         = 0X1616,

    CFG_TYPE_SOFTAEC_IK_8K64        = 0X1700,//软件AEC独立核
    CFG_TYPE_SOFTAEC_IK_8K96        = 0X1701,
    CFG_TYPE_SOFTAEC_IK_8K128       = 0X1702,
    CFG_TYPE_SOFTAEC_IK_8K160       = 0X1703,
    CFG_TYPE_SOFTAEC_IK_8K192       = 0X1704,
    CFG_TYPE_SOFTAEC_IK_8K224       = 0X1705,
    CFG_TYPE_SOFTAEC_IK_8K256       = 0X1706,
    CFG_TYPE_SOFTAEC_IK_16K64       = 0X1710,
    CFG_TYPE_SOFTAEC_IK_16K96       = 0X1711,
    CFG_TYPE_SOFTAEC_IK_16K128      = 0X1712,
    CFG_TYPE_SOFTAEC_IK_16K160      = 0X1713,
    CFG_TYPE_SOFTAEC_IK_16K192      = 0X1714,
    CFG_TYPE_SOFTAEC_IK_16K224      = 0X1715,
    CFG_TYPE_SOFTAEC_IK_16K256      = 0X1716,
//    CFG_TYPE_SOFTAEC_IK_48K64       = 0X1720,
//    CFG_TYPE_SOFTAEC_IK_48K96       = 0X1721,
//    CFG_TYPE_SOFTAEC_IK_48K128      = 0X1722,
//    CFG_TYPE_SOFTAEC_IK_48K160      = 0X1723,
//    CFG_TYPE_SOFTAEC_IK_48K192      = 0X1724,
//    CFG_TYPE_SOFTAEC_IK_48K256      = 0X1726,

    CFG_TYPE_UCI                    = 0x3000,//用户控制

    CFG_TYPE_CTRL_LUA               = 0x4000,
    CFG_TYPE_CTRL_GPI               = 0x4001,//GPI控制
    CFG_TYPE_CTRL_GPO               = 0x4002,//GPO控制
    CFG_TYPE_CTRL_AD                = 0x4003,//AD控制
    CFG_TYPE_CTRL_RS232             = 0x4004,
    CFG_TYPE_CTRL_RS485             = 0x4005,

    CFG_TYPE_AUDIO_PLAYER1          = 0xD000,       //多轨播放器
    CFG_TYPE_AUDIO_PLAYER2          = 0xD001,
    CFG_TYPE_AUDIO_PLAYER4          = 0xD002,
    CFG_TYPE_AUDIO_PLAYER8          = 0xD003,
    CFG_TYPE_AUDIO_PLAYER16         = 0xD004,

    CFG_TYPE_AUDIO_RECORD1          = 0xD100,       //录音器
    CFG_TYPE_AUDIO_RECORD2          = 0xD101,
    CFG_TYPE_AUDIO_RECORD4          = 0xD102,

    CFG_TYPE_VOICETUBE_1            = 0xE000,
    CFG_TYPE_VOICETUBE_2,

    CFG_TYPE_SNAPSHOT = 0xF000,
    CFG_TYPE_SNAPSHOT_BANK =  0xF100,

    //////监控设备///////
    CFG_TYPE_1 = 0xFF00,
    CFG_TYPE_2,
    CFG_TYPE_3,
    CFG_TYPE_4,
    CFG_TYPE_5,
    CFG_TYPE_SCENE,
    CFG_TYPE_NETWORKDEVICE,
 };

enum SubWnd_Type {
    VIEW_NONE = 0x01,
    VIEW_PROJECT = 0x02,
    VIEW_CONFIG = 0x04,
    VIEW_MONITOR_AIDS = 0x08,//监控设备
    VIEW_USERCONTROLINTERFACE = 0x10, //UCI
    VIEW_SOLUTION = 0x20, //solution

    VIEW_Mask = 0x3f
};

enum QGraphicsItemDataKey {
    ALIGN_DISTENCE = 6,
    DataKey_OldScenePos = 101,      //用于记录移动幅度
    DataKey_PressedItemPos = 102,   //按下位置（item坐标）用于对齐线
    DataKey_PressedScenePos = 103,  //用于忽略小的移动
    DataKey_TempLineItem = 104,     //临时连接线
    DataKey_Type,                   //拖拽时存储Type
    DataKey_SubType,                //拖拽时存储SubType
    DataKey_ItemPtr,                //拖拽时存储对象指针
};

enum QGraphicsItemZValue
{
    DesignItemMinZVal = 10,
    DesignItemMaxZVal = 20,
    LineItemZVal = 30,
    SELECT_ZVALUE = 40,
    CftSettingZVal = 50,
    LabelEditZVal = 60,
    TopZVal = 70,
};

//enum MonitorGroupType{
//    MONITOR_GROUP_METER = 0x10,
//    MONITOR_GROUP_CLIPPER = 0x20,
//    MONITOR_GROUP_BUTTON = 0x30,
//    MONITOR_GROUP_LABEL = 0x40,
//};

#endif // COMMON_H
