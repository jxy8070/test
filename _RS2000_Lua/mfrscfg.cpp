#include "mfrscfg.h"
#include <QTreeWidgetItem>
#include "config.h"

MfrsCfg MfrsCfg::_Fitcan = {false,                    //只管理该客户设备
                          0,                        //设备8分组的间隔
                          "Designer",               //主窗口标题
                          Mfrs_Fitcan,              //所属客户
#ifdef QT_DEBUG
                          {Skin_KemuGrey,Skin_KemuWhite,Skin_KemuBlack,Skin_FangTu,Skin_Default}
#else
                          {Skin_Default}
#endif
};
MfrsCfg MfrsCfg::_FangTu = {false,                     //只管理该客户设备
                          10,                       //设备8分组的间隔
                          "FionTu FT_Designer",     //主窗口标题
                          Mfrs_FangTu,              //所属客户
                          {Skin_FangTu}
};
MfrsCfg MfrsCfg::_DongFangJiaLian ={false,                            //只管理该客户设备
                                  0,                                //设备8分组的间隔
                                  "KEMU Designer",                  //主窗口标题
                                  Mfrs_KEMU,             //所属客户
#ifdef QT_DEBUG
                                  {Skin_KemuGrey,Skin_KemuWhite,Skin_KemuBlack,Skin_FangTu,Skin_Default}
#else
                                  {Skin_KemuGrey,Skin_KemuWhite}
#endif
};
MfrsCfg MfrsCfg::_ControlMax =   {false,                            //只管理该客户设备
                                  0,                                //设备8分组的间隔
                                  "ControlMax Designer",            //主窗口标题
                                  Mfrs_ControlMax,             //所属客户
                                  {Skin_KemuGrey,Skin_KemuWhite}
};
MfrsCfg MfrsCfg::_GuangZhouShengTuo ={true,                             //只管理该客户设备
                                  0,                                //设备8分组的间隔
                                  "EAJAX AudioDSP",                 //主窗口标题
                                  Mfrs_GuangZhouShengTuo,           //所属客户
                                  {Skin_Default}
};
MfrsCfg MfrsCfg::_Hushan ={true,                             //只管理该客户设备
                          0,                                //设备8分组的间隔
                          "HS_Designer",                    //主窗口标题
                          Mfrs_Hushan,                      //所属客户
                          {Skin_Default}
};
MfrsCfg MfrsCfg::_BeiJingHuiSheng ={true,                             //只管理该客户设备
                                  8,                                //设备8分组的间隔
                                  "Ceiadap",                        //主窗口标题
                                  Mfrs_BeiJingHuiSheng,             //所属客户
                                  {Skin_Default}
};
MfrsCfg MfrsCfg::_GuRui ={false,                            //只管理该客户设备
                          0,                                //设备8分组的间隔
                          "AoIP",                           //主窗口标题
                          Mfrs_GuangZhouShengTuo,           //所属客户
#ifdef QT_DEBUG
                                 {Skin_KemuGrey,Skin_KemuWhite,Skin_KemuBlack,Skin_Default}
#else
                                 {Skin_Default}
#endif
};
MfrsCfg MfrsCfg::_YiChuang ={true,                             //只管理该客户设备
                              0,                                //设备8分组的间隔
                              "EAV-martix",                     //主窗口标题
                              Mfrs_YiChuang,                    //所属客户
                              {Skin_Default}
};
MfrsCfg MfrsCfg::_BaiLianDa = {false,                    //只管理该客户设备
                                0,                        //设备8分组的间隔
                                "Designer",               //主窗口标题
                                Mfrs_BaiLianDa,           //所属客户
                                {Skin_Default}
};
MfrsCfg MfrsCfg::_FTYX = {false,                    //只管理该客户设备
                          0,                        //设备8分组的间隔
                          "Desiner",               //主窗口标题
                          Mfrs_FTYX,              //所属客户
#ifdef QT_DEBUG
                        {Skin_Default, Skin_KemuGrey,Skin_KemuWhite,Skin_KemuBlack,Skin_FangTu}
#else
                        {Skin_Default}
#endif
};

#define CHS(x) QStringLiteral(x)
void MfrsCfg::InitDeviceTree()
{
    MfrsType curMfrs = Config::curMfrsCfg().CurMfrs;
    if(curMfrs == Mfrs_FangTu) {
        MfrsDevList = {
            {"DSP Microphone", CHS("网络DSP话筒"), "FionTu-DMIC", "FionTu-DMIC", true, false, Dev_Type_STM32_POE_AES67_MIC_2_0},
            {"Wireless 1to2/4/8(Network)", CHS("无线1to2/4/8(带网络传输版本)"), "FionTu-WMIC", "FionTu-WMIC", true, false, Dev_Type_FionTu_WMIC_FangTu},
            {"Meeting Host(Network)",CHS("会议主机(带网络传输版本)"), "I Mic Center/DN", "I Mic Center/DN", true, false, Dev_Type_I_Mic_Center_DN_FangTu},
            {"Sound Console", CHS("数字调音台"), "FD-16", "FD-16", true, false, Dev_Type_RT1052_DR3020},
            {"Network Central Machine", CHS("网络中心机"), "M-64*64", "M-64*64", true, false, Dev_Type_NAP_Zynq7020_16Analog_16AES67},
            {"Network Central Machine", CHS("网络中心机"), "M-128*128", "M-128*128", true, false, Dev_Type_Mixer_128_128_FangTu},
            {"Network Central Machine", CHS("网络中心机"), "M-256*256", "M-256*256", true, false, Dev_Type_Mixer_256_256_FangTu},
            {"Central Processing System", CHS("数字音频中央处理系统"), "MBP300", "MBP300", true, false, Dev_Type_NAP_300_FangTu},
            {"Central Processing System", CHS("数字音频中央处理系统"), "MBP400", "MBP400", true, false, Dev_Type_NAP_400_FangTu},
            {"Processors", CHS("数字网络音频矩阵处理器"), "MBP50", "MBP50", true, false, Dev_Type_NAP_STM32H750_8A8N},
            {"Processors", CHS("数字网络音频矩阵处理器"), "MBP80", "MBP80", true, false, Dev_Type_NAP_Zynq7020_16Analog_32Dante},
            {"Processors", CHS("数字网络音频矩阵处理器"), "MBP100", "MBP100", true, false, Dev_Type_NAP_Zynq7020_16Analog_48AES67},
            {"Processors", CHS("数字网络音频矩阵处理器"), "MBP150", "MBP150", true, false, Dev_Type_NAP_150_FangTu},
            {"Processors(DANTE)", CHS("数字网络音频矩阵处理器(DANTE版本)"), "MBP100-DANTE", "MBP100-DANTE", true, false, Dev_Type_NAP_100_FangTu},
//            {"Processors", CHS("音频处理器"), "MBP-100 Network", "MBP-100 Network", true, false, Dev_Type_NAP_Zynq7020_64AES67},//客户取消
            {"Boxes", CHS("网络音频DSP接口机"), "I/O-4", "I/O-4", true, false, Dev_Type_RT1052_POE_AES67_LINE_4_4},
            {"Boxes", CHS("网络音频DSP接口机"), "Input-2", "Input-2", true, false, Dev_Type_RT1052_POE_AES67_MIC_2_0},
            {"Boxes", CHS("网络音频DSP接口机"), "Output-2", "Output-2", true, false, Dev_Type_RT1052_POE_AES67_LINE_0_2},
            {"Boxes", CHS("网络音频DSP接口机"), "Input-4", "Input-4", true, false, Dev_Type_RT1052_POE_AES67_MIC_4_0},
            {"Boxes", CHS("网络音频DSP接口机"), "Input-8", "Input-8", true, false, Dev_Type_RT1052_POE_AES67_MIC_8_0},
            {"Boxes", CHS("网络音频DSP接口机"), "Output-8", "Output-8", true, false, Dev_Type_RT1052_POE_AES67_LINE_0_8},
            {"Virtual Sound Card", CHS("虚拟声卡"), "AES67 Sound Card", "AES67 Sound Card", true, false, Dev_Type_VirtualSoundCard_AES67},
            {"DSP Power Amplifier", CHS("模拟网络双备份DSP功放"), "FT_4A300N", "FT_4A300N", true, false, Dev_Type_FT_4D300N_FangTu},
            {"DSP Power Amplifier", CHS("模拟网络双备份DSP功放"), "FT_4A600N", "FT_4A600N", true, false, Dev_Type_FT_4D600N_FangTu},
            {"DSP Power Amplifier", CHS("模拟网络双备份DSP功放"), "FT_4A800N", "FT_4A800N", true, false, Dev_Type_RT1052_AMPLIFIER_AA4500},//暂时使用该设备类型
            {"DSP Power Amplifier", CHS("模拟网络双备份DSP功放"), "FT_4A1200N", "FT_4A1200N", true, false, Dev_Type_FT_4A1200N_FangTu},
            {"DSP Power Amplifier", CHS("模拟网络双备份DSP功放"), "FT_4A2000N", "FT_4A2000N", true, false, Dev_Type_FT_4A2000N_FangTu},
            {"Loudspeaker\\Network Speaker with Dual POE Power", CHS("音箱\\POE网络DSP吸顶音箱"), "POE-CS50", "POE-CS50", true, false, Dev_Type_CS50_FangTu},
            {"Loudspeaker\\Network Speaker with Dual POE Power", CHS("音箱\\双备份POE网络吸顶音箱"), "POE-CS50A","POE-CS50A", true, false, Dev_Type_CS50A_FangTu},
            {"Loudspeaker\\Network Speaker with Dual POE Power", CHS("音箱\\POE网络DSP吸顶音箱"), "POE-CS60", "POE-CS60", true, false, Dev_Type_RT1052_AP60},
            {"Loudspeaker\\Network Speaker with Dual POE Power", CHS("音箱\\双备份POE网络吸顶音箱"), "POE-CS60A","POE-CS60A", true, false, Dev_Type_CS60A_FangTu},
            {"Loudspeaker\\Network Speaker with Dual POE Power", CHS("音箱\\POE网络DSP吸顶音箱"), "POE-CS80", "POE-CS80", true, false, Dev_Type_CS80_FangTu},
            {"Loudspeaker\\Network Speaker with Dual POE Power", CHS("音箱\\双备份POE网络吸顶音箱"), "POE-CS80A", "POE-CS80A", true, false, Dev_Type_CS80A_FangTu},
            {"Loudspeaker\\Dual Backup Direction Network Columnspeaker", CHS("音箱\\网络可调指向性DSP阵列音柱"), "FT-L845A", "FT-L845A", true, false, Dev_Type_RT1052_AP400},
            {"Loudspeaker\\Dual Backup Direction Network Columnspeaker", CHS("音箱\\双备份可调指向性网络音柱"), "FT-L403N", "FT-L403N", true, false, Dev_Type_FT_L403N_FangTu},
            {"Loudspeaker\\Dual Backup Direction Network Columnspeaker", CHS("音箱\\双备份可调指向性网络音柱"), "Dir-L445A", "Dir-L445A", true, false, Dev_Type_Dir_L445_FangTu},
            {"Loudspeaker\\Dual Backup Direction Network Columnspeaker", CHS("音箱\\双备份可调指向性网络音柱"), "Dir-X(10,12,16)", "Dir-X(10,12,16)", true, false, Dev_Type_Dir_X_10_12_16_FangTu},
            {"Loudspeaker\\Dual Backup Network Coaxial Speaker", CHS("音箱\\双备份网络有源全频同轴音箱"), "6 Inches", CHS("6寸"), true, false, Dev_Type_AAF_CS_6_FangTu},
            {"Loudspeaker\\Dual Backup Network Coaxial Speaker", CHS("音箱\\双备份网络有源全频同轴音箱"), "8 Inches", CHS("8寸"), true, false, Dev_Type_AAF_CS_8_FangTu},
            {"Loudspeaker\\Dual Backup Network Coaxial Speaker", CHS("音箱\\双备份网络有源全频同轴音箱"), "10 Inches", CHS("10寸"), true, false, Dev_Type_AAF_CS_10_FangTu},
            {"Loudspeaker\\Dual Backup Network Coaxial Speaker", CHS("音箱\\双备份网络有源全频同轴音箱"), "12 Inches", CHS("12寸"), true, false, Dev_Type_AAF_CS_12_FangTu},
            {"Loudspeaker\\Dual Backup Network Coaxial Speaker", CHS("音箱\\双备份网络有源全频同轴音箱"), "15 Inches", CHS("15寸"), true, false, Dev_Type_AAF_CS_15_FangTu},
            {"Loudspeaker\\Dual Backup Network Coaxial Speaker", CHS("音箱\\双备份网络有源全频同轴音箱"), "16 Inches", CHS("18寸"), true, false, Dev_Type_AAF_CS_18_FangTu},
            {"Loudspeaker\\Dual Backup Network Columnspeaker", CHS("音箱\\双备份网络组合音柱"), "6 Unit", CHS("4单元音柱"), true, false, Dev_Type_DBN_4u_FangTu},
            {"Loudspeaker\\Dual Backup Network Columnspeaker", CHS("音箱\\双备份网络组合音柱"), "8/10 Unit", CHS("8/10单元音柱"), true, false, Dev_Type_DBN_8_10u_FangTu},
            {"Switch series", CHS("交换机系列"), "Switch", CHS("交换机"), true, false, Dev_Type_Switch_16x32_FangTu},
            {"Switch series", CHS("交换机系列"), "POE Switch", CHS("POE交换机"), true, false, Dev_Type_Switch_POE_16x32_FangTu},
        };
    }
    else if(curMfrs == Mfrs_KEMU || curMfrs == Mfrs_ControlMax) {
        MfrsDevList = {
            {"Virtual Sound Card", CHS("虚拟声卡"), "AES67 Sound Card", "AES67 Sound Card", true, false, Dev_Type_VirtualSoundCard_AES67},
            {"Processors", CHS("音频处理器"), "NX-0808i", "NX-0808i", true, false, Dev_Type_NAP_STM32H750_8A8N},
            {"Processors", CHS("音频处理器"), "NX-1616i", "NX-1616i", true, false, Dev_Type_NAP_Zynq7020_16Analog_48AES67},
            {"Processors", CHS("音频处理器"), "Master128", "master128", true, true, Dev_Type_NAP_X86_16Analog_128AES67},
            {"Processors", CHS("音频处理器"), "Master256", "master256", true, true, Dev_Type_NAP_X86_32Analog_256AES67},
            {"Other Device", CHS("其它设备"), "AES3 To AES67", "AES3 To AES67", true, false, Dev_Type_Zynq7010_AES3ToAES67_DongFangJiaLian},
            {"Other Device", CHS("其它设备"), "DN8150", "DN8150", true, false, Dev_Type_RT1052_AMPLIFIER_AA4500},
            {"Other Device", CHS("其它设备"), "DN8300", "DN8300", true, false, Dev_Type_RT1052_Amplifier_DongFangJiaLian},
        };
    }
    else if(curMfrs == Mfrs_GuangZhouShengTuo) {
        MfrsDevList = {
//            {"Virtual Sound Card", CHS("虚拟声卡"), "Dante Sound Card", "Dante Sound Card", true, false, Dev_Type_VirtualSoundCard_Dante},
            {"Virtual Sound Card", CHS("虚拟声卡"), "AES67 Sound Card", "AES67 Sound Card", true, false, Dev_Type_VirtualSoundCard_AES67},
            {"Processors", CHS("音频处理器"), "DSA64", "DSA64", true, false, Dev_Type_NAP_Zynq7020_16Analog_48AES67},
            {"POE Serial", CHS("POE系列"), "DSA4MT", "DSA4MT", true, false, Dev_Type_RT1052_POE_AES67_MIC_4_0},
            {"POE Serial", CHS("POE系列"), "DSA4RT", "DSA4RT", true, false, Dev_Type_RT1052_POE_AES67_LINE_4_4},
            {"POE Serial", CHS("POE系列"),  "DSA2T", "DSA2T", true, false, Dev_Type_RT1052_POE_AES67_MIC_2_0},
            {"POE Serial", CHS("POE系列"),  "DSA2R", "DSA2R", true, false, Dev_Type_RT1052_POE_AES67_LINE_0_2},
            {"POE Serial", CHS("POE系列"),  "AE580", "AE580", true, false, Dev_Type_RT1052_POE_AES67_MIC_2_0_Type2},
            {"POE Serial", CHS("POE系列"),  "AE680", "AE680", true, false, Dev_Type_RT1052_POE_AES67_MIC_2_0_Type3},
            {"POE Serial", CHS("POE系列"),  "AE880", "AE880", true, false, Dev_Type_RT1052_POE_AES67_MIC_2_0_Type1},
            {"POE Serial", CHS("POE系列"),  "AE60", "AE60", true, false, Dev_Type_RT1052_AP60},
            {"POE Serial", CHS("POE系列"),  "AE1200", "AE1200", true, false, Dev_Type_RT1052_AP60_Type1},
            {"POE Serial", CHS("POE系列"),  "AE650", "AE650", true, false, Dev_Type_RT1052_AP60_Type2},
            {"POE Serial", CHS("POE系列"),  "AE8000", "AE8000", true, false, Dev_Type_RT1052_AP400},
            {"Amplifier Module", CHS("POE系列"),  "AE408", "AE408", true, false, Dev_Type_RT1052_AMPLIFIER_AA4500},
            {"Amplifier Module", CHS("POE系列"),  "AE416", "AE416", true, false, Dev_Type_RT1052_AMPLIFIER_AA4500_Type1},
            {"Amplifier Module", CHS("POE系列"),  "AE500", "AE500", true, false, Dev_Type_RT1052_AMPLIFIER_AA2500},
        };
    }
    else if(curMfrs == Mfrs_BaiLianDa) {
        MfrsDevList = {
            {"Processors", CHS("音频处理器"), "NAP-200", "NAP-200", true, false, Dev_Type_NAP_Zynq7020_16Analog_48AES67},
        };
    }
    else if(curMfrs == Mfrs_FTYX){
        MfrsDevList = {
            {"Virtual Sound Card", CHS("虚拟声卡"), "AES67 Sound Card", "AES67 Sound Card", true, false, Dev_Type_VirtualSoundCard_AES67},
            {"Processors", CHS("音频处理器"), "DB3000F", "DB3000F", true, false, Dev_Type_NAP_FT2000_256AES67},
            {"Processors", CHS("音频处理器"), "master128", "master128", true, true, Dev_Type_NAP_X86_16Analog_128AES67},
            {"Processors", CHS("音频处理器"), "NAP-100", "NAP-100", true, false, Dev_Type_NAP_Zynq7020_16Analog_16AES67},  //富通亚讯：根据合同下面几款设备都认成NAP100i
            {"Processors", CHS("音频处理器"), "NAP-100i", "NAP-100i", true, false, Dev_Type_NAP_Zynq7020_16Analog_48AES67},
            {"Processors", CHS("音频处理器"), "NAP-d16_t0", "NAP-d16_t0", true, false, Dev_Type_NAP_Zynq7020_16Analog_16Dante},
            {"Processors", CHS("音频处理器"), "NAP-d16_t1", "NAP-d16_t1", true, false, Dev_Type_NAP_Zynq7020_16Analog_16Dante_Type1},
            {"Processors", CHS("音频处理器"), "NAP-d32", "NAP-d32", true, false, Dev_Type_NAP_Zynq7020_16Analog_32Dante},
            {"POE Serial", CHS("POE系列"), "SPK-Mono", "SPK-Mono", true, false, Dev_Type_STM32_POE_AES67_SPK_0_1},
            {"POE Serial", CHS("POE系列"),  "SPK-Stereo", "SPK-Stereo", true, false, Dev_Type_STM32_POE_AES67_SPK_0_2},
            {"POE Serial", CHS("POE系列"),  "MC 101P", "MC 101P", true, false, Dev_Type_STM32_POE_AES67_MIC_2_0},
            {"POE Serial", CHS("POE系列"),  "MC 402P", "MC 402P", true, false, Dev_Type_RT1052_POE_AES67_MIC_4_0},
            {"POE Serial", CHS("POE系列"),  "LC 442P", "LC 442P", true, false, Dev_Type_RT1052_POE_AES67_LINE_4_4},
            {"POE Serial", CHS("POE系列"),  "MC 201P", "MC 201P", true, false, Dev_Type_RT1052_POE_AES67_MIC_2_0},
            {"POE Serial", CHS("POE系列"),  "LC 102P", "LC 102P", true, false, Dev_Type_RT1052_POE_AES67_LINE_0_2},
            {"POE Serial", CHS("POE系列"),  "AP60", "AP60", true, false, Dev_Type_RT1052_AP60},
            {"Active Serial", CHS("功放设备"), "AA4500", "AA4500", true, false, Dev_Type_RT1052_AMPLIFIER_AA4500},
            {"Active Serial", CHS("功放设备"), "AE2500", "AE2500", true, false, Dev_Type_RT1052_AMPLIFIER_AA2500},
            {"Active Serial", CHS("功放设备"), "AP400", "AP400", true, false, Dev_Type_RT1052_AP400},
            {"Other Device", CHS("其它设备"), "AES3 To AES67", "AES3 To AES67", true, false, Dev_Type_Zynq7010_AES3ToAES67_DongFangJiaLian},
            {"Other Device", CHS("其它设备"), "DR3020", "DR3020", true, false, Dev_Type_RT1052_DR3020},
            {"Other Device", CHS("其它设备"), "AES67 To Analog", "AES67 To Analog", true, false, Dev_Type_RT1052_Amplifier_DongFangJiaLian},
        };
    }
    else
    {
        MfrsDevList = {
//            {"Virtual Sound Card", CHS("虚拟声卡"), "Dante Sound Card", "Dante Sound Card", true, false, Dev_Type_VirtualSoundCard_Dante},
            {"Virtual Sound Card", CHS("虚拟声卡"), "AES67 Sound Card", "AES67 Sound Card", true, false, Dev_Type_VirtualSoundCard_AES67},
            {"Processors", CHS("音频处理器"), "DB3000F", "DB3000F", true, false, Dev_Type_NAP_FT2000_256AES67},
            {"Processors", CHS("音频处理器"), "master128", "master128", true, true, Dev_Type_NAP_X86_16Analog_128AES67},
            {"Processors", CHS("音频处理器"), "NAP-8X8", "NAP-8X8", true, false, Dev_Type_NAP_STM32H750_8A8N},
            {"Processors", CHS("音频处理器"), "NAP-100", "NAP-100", true, false, Dev_Type_NAP_Zynq7020_16Analog_16AES67DanteMixer},
            {"Processors", CHS("音频处理器"), "NAP-100i", "NAP-100i", true, false, Dev_Type_NAP_Zynq7020_16Analog_16AES67},
            {"Processors", CHS("音频处理器"), "NAP-200", "NAP-200", true, false, Dev_Type_NAP_Zynq7020_16Analog_48AES67},
            {"Processors", CHS("音频处理器"), "NAP-200p", "NAP-200p", true, false, Dev_Type_NAP_Zynq7020_64AES67},
            {"Processors", CHS("音频处理器"), "NAP-d16_t0", "NAP-d16_t0", true, false, Dev_Type_NAP_Zynq7020_16Analog_16Dante},
            {"Processors", CHS("音频处理器"), "NAP-d16_t1", "NAP-d16_t1", true, false, Dev_Type_NAP_Zynq7020_16Analog_16Dante_Type1},
            {"Processors", CHS("音频处理器"), "NAP-d32", "NAP-d32", true, false, Dev_Type_NAP_Zynq7020_16Analog_32Dante},
            {"POE Serial", CHS("POE系列"), "SPK-Mono", "SPK-Mono", true, false, Dev_Type_STM32_POE_AES67_SPK_0_1},
            {"POE Serial", CHS("POE系列"),  "SPK-Stereo", "SPK-Stereo", true, false, Dev_Type_STM32_POE_AES67_SPK_0_2},
            {"POE Serial", CHS("POE系列"),  "MC 101P", "MC 101P", true, false, Dev_Type_STM32_POE_AES67_MIC_2_0},
            {"POE Serial", CHS("POE系列"),  "MC 402P", "MC 402P", true, false, Dev_Type_RT1052_POE_AES67_MIC_4_0},
            {"POE Serial", CHS("POE系列"),  "LC 442P", "LC 442P", true, false, Dev_Type_RT1052_POE_AES67_LINE_4_4},
            {"POE Serial", CHS("POE系列"),  "MC 201P", "MC 201P", true, false, Dev_Type_RT1052_POE_AES67_MIC_2_0},
            {"POE Serial", CHS("POE系列"),  "LC 102P", "LC 102P", true, false, Dev_Type_RT1052_POE_AES67_LINE_0_2},
            {"POE Serial", CHS("POE系列"),  "AP60", "AP60", true, false, Dev_Type_RT1052_AP60},
            {"Active Serial", CHS("功放设备"), "AA4500", "AA4500", true, false, Dev_Type_RT1052_AMPLIFIER_AA4500},
            {"Active Serial", CHS("功放设备"), "AE2500", "AE2500", true, false, Dev_Type_RT1052_AMPLIFIER_AA2500},
            {"Active Serial", CHS("功放设备"), "AP400", "AP400", true, false, Dev_Type_RT1052_AP400},
            {"Other Device", CHS("其它设备"), "AES3 To AES67", "AES3 To AES67", true, false, Dev_Type_Zynq7010_AES3ToAES67_DongFangJiaLian},
            {"Other Device", CHS("其它设备"), "DR3020", "DR3020", true, false, Dev_Type_RT1052_DR3020},
            {"Other Device", CHS("其它设备"), "AES67 To Analog", "AES67 To Analog", true, false, Dev_Type_RT1052_Amplifier_DongFangJiaLian},
        };

    }
}

bool MfrsCfg::isLuaDevice(DevType type)
{
    if(!Config::isLuaSupported()){
        return false; //全局lua支持关闭，则忽略设备lua支持
    }

    bool isLuaDev = false;
    for(MfrsDev dev : MfrsDevList)
    {
        if(dev.Type == type){
            isLuaDev = dev.HasLua;
            break;
        }
    }
    return isLuaDev;
}

bool MfrsCfg::isValidDevice(DevType type)
{
    bool isValid = false;
    for(MfrsDev dev : MfrsDevList)
    {
        if(dev.Type == type){
            isValid = true;
            break;
        }
    }
    return isValid;
}

QString MfrsCfg::getDevTypeName(DevType type)
{
    for(MfrsDev dev : MfrsDevList)
    {
        if(dev.Type == type){
            if (Config::instance()->getLanguage() == LANGUAGE_ENGLISH) {
                return dev.TypeNameEn;
            }
            else{
                 return dev.TypeNameChs;
            }
            break;
        }
    }
    return QString("dType:( %1 )").arg(type);
}


QString MfrsCfg::getDevGroupName(DevType type)
{
    QString devGroupName = "unknown";
    for(MfrsDev dev : MfrsDevList)
    {
        if(dev.Type == type){
            if (Config::instance()->getLanguage() == LANGUAGE_ENGLISH) {
                devGroupName = dev.GroupNameEn;
            }
            else{
                 devGroupName = dev.GroupNameChs;
            }
            break;
        }
    }
    return devGroupName;
}



//Components中显示
QString MfrsCfg::getCfgModelName(CfgType type)
{
    if(CurMfrs == Mfrs_FangTu){
        return Utils::getCfgModelName_FangTu(type);
    }
    else {
        return Utils::getCfgModelName_Other(type);
    }
}

QColor MfrsCfg::getDeviceFillColor(DevType type)
{
    QString groupName = getDevGroupName(type);
    MfrsType curMfrs = Config::curMfrsCfg().CurMfrs;
    QStringList devGroupNames;
    foreach(MfrsDev item, MfrsDevList) {

        if (Config::instance()->getLanguage() == LANGUAGE_ENGLISH) {
            if(devGroupNames.contains(item.GroupNameEn)) continue;
            devGroupNames.append(item.GroupNameEn);
        }else {
            if(devGroupNames.contains(item.GroupNameChs)) continue;
            devGroupNames.append(item.GroupNameChs);
        }
    }

    if(curMfrs == Mfrs_FangTu) {
        switch (devGroupNames.indexOf(groupName)) {
        case 0: return QColor(255,204,204);
        case 1:
        case 2:
        case 3:
        case 4:
        case 5: return QColor(121,238,165);
        case 6:
        case 7:
        case 8: return QColor(255,204,153);

        default: return QColor(165,182,133,255);
        }
    }else {
        return QColor(150,140,210,255);
    }
}
