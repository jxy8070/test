#include "monitoringfactory.h"

QStringList MonitoringFactory::CreateInstance(DevType devType, CfgType type)
{
    //    Monitor_port lineDevPort;
        QStringList meterList;
        QStringList clippersList;
        QStringList buttonList;

    //    QMap<int, QStringList> lineDevPort;
    //    lineDevPort.clear();
    //    memset(&lineDevPort, 0, sizeof(Monitor_port));
        switch(devType)
        {
        case Dev_Type_Normal_PC:
        case Dev_Type_Normal_CD:
        case Dev_Type_Normal_MIC:
        case Dev_Type_Normal_SPK:
        case Dev_Type_Normal_AMP:
            break;
        case Dev_Type_NAP_Zynq7020_16Analog_16AES67:
        case Dev_Type_NAP_Zynq7020_16Analog_48AES67:
        case Dev_Type_NAP_Zynq7020_64AES67:
        case Dev_Type_NAP_Zynq7020_16Analog_16AES67DanteMixer:
        case Dev_Type_NAP_Zynq7020_16Analog_16Dante:
        case Dev_Type_NAP_Zynq7020_16Analog_16Dante_Type1:
        case Dev_Type_NAP_Zynq7020_16Analog_32Dante:
        case Dev_Type_NAP_X86_16Analog_128AES67:
            break;
        case Dev_Type_VirtualSoundCard_Dante:
        case Dev_Type_VirtualSoundCard_AES67:
            break;
        case Dev_Type_STM32_POE_AES67_SPK_0_1:
            meterList<<"None";
            clippersList<<"Volume(dB)";
            buttonList<<"M"<<"EQ"<<"DYN"<<"Bypass"<<"Save";
            break;
        case Dev_Type_STM32_POE_AES67_SPK_0_2:
            meterList<<"None";
            clippersList<<"None";
            buttonList<<"None";
            break;
        case Dev_Type_STM32_POE_AES67_MIC_2_0:
            meterList<<"None";
            clippersList<<"MicGain";
            buttonList<<"Xover"<<"Dyn"<<"Eq"<<"Save";
            break;
        case Dev_Type_RT1052_POE_AES67_MIC_4_0:
            meterList<<"A-In1"<<"A-In2"<<"A-In3"<<"A-In4";
            clippersList<<"Mic-In1(Gain)"<<"Mic-In2(Gain)"<<"Mic-In3(Gain)"<<"Mic-In4(Gain)"
                        <<"N-Out1(Gain)" <<"N-Out2(Gain)"<<"N-Out3(Gain)"<<"N-Out4(Gain)"
                        <<"Mic-In1&Mic-In2(Gain)"<<"Mic-In3&Mic-In4(Gain)";
            buttonList<<"Mic-In1(Phantom)"<<"Mic-In2(Phantom)"<<"Mic-In3(Phantom)"<<"Mic-In4(Phantom)"
                      <<"Mic-In1(Low Cut)"<<"Mic-In2(Low Cut)"<<"Mic-In3(Low Cut)"<<"Mic-In4(Low Cut)"
                      <<"Mic-In1(Dyn)"    <<"Mic-In2(Dyn)"    <<"Mic-In3(Dyn)"    <<"Mic-In4(Dyn)"
                      <<"Mic-In1(Eq)"     <<"Mic-In2(Eq)"     <<"Mic-In3(Eq)"     <<"Mic-In4(Eq)"
                      <<"N-Out1(Phase)"   <<"N-Out2(Eq)"      <<"N-Out3(Phase)"   <<"N-Out4(Phase)"
                      <<"Mixer";
            break;
        case Dev_Type_RT1052_POE_AES67_MIC_2_0:
        case Dev_Type_RT1052_POE_AES67_MIC_2_0_Type1:
        case Dev_Type_RT1052_POE_AES67_MIC_2_0_Type2:
        case Dev_Type_RT1052_POE_AES67_MIC_2_0_Type3:
        case Dev_Type_RT1052_POE_AES67_MIC_8_0:
            meterList<<"A-In1"<<"A-In2";
            clippersList<<"A-In1(Gain)"<<"A-In2(Gain)"<<"N-Out1(Gain)"<<"N-Out2(Gain)"<<"A-In1&A-In2(Gain)";
            buttonList<<"Mic_open"<<"Phantom"
                      <<"A-In1(Low Cut)"<<"A-In1(Dyn)"<<"A-In1(Eq)"<<"A-In1(Phase)"
                      <<"A-In2(Low Cut)"<<"A-In2(Dyn)"<<"A-In2(Eq)"<<"A-In2(Phase)"<<"Mixer";
            break;
        case Dev_Type_RT1052_POE_AES67_LINE_0_2:
        case Dev_Type_RT1052_POE_AES67_LINE_0_8:
            meterList<<"A-Out1"<<"A-Out2";
            clippersList<<"N-In1(Gain)"<<"N-In2(Gain)"<<"A-Out1(Gain)"<<"A-Out2(Gain)"<<"A-Out1(Delay)"<<"A-Out2(Delay)";
            buttonList<<"N-In1(mute)"  <<"N-In2(mute)"<<"Mixer"<<"Save"
                      <<"A-Out1(Xover)"<<"A-Out1(Eq)" <<"A-Out1(mute)"<<"A-Out1(DYN)"<<"A-Out1(Phase)"
                      <<"A-Out2(Xover)"<<"A-Out2(Eq)" <<"A-Out2(mute)"<<"A-Out2(DYN)"<<"A-Out2(Phase)";
            break;
        case Dev_Type_RT1052_AP60:
        case Dev_Type_RT1052_AP60_Type1:
        case Dev_Type_RT1052_AP60_Type2:
            meterList<<"A-In1"<<"A-In2"<<"A-Out1"<<"A-Out2";
            clippersList<<"A-In1(Gain)"<<"N-In1(Gain)" <<"A-In2(Gain)"<<"A-Out1(Gain)"<<"A-Out1(Delay)";
            buttonList<<"A-In1(DYN)"   <<"A-In1(mute)" <<"A-In1(Eq)"
                      <<"N-In1(mute)" <<"N-In1(Eq)"
                      <<"A-In2(mute)"  <<"A-In2(Eq)"   <<"A-In2(xover)"
                      <<"A-Out1(xover)"<<"A-Out1(Eq)"<<"A-Out1(mute)"<<"A-Out1(DYN)"<<"A-Out1(Phase)"<<"Save";
            break;
        case Dev_Type_RT1052_AP60_Bluetooth:
            break;
        case Dev_Type_RT1052_AP400:
            break;
        case Dev_Type_RT1052_POE_AES67_LINE_4_4:
            buttonList<<"Route";
        case Dev_Type_RT1052_AMPLIFIER_AA4500:
        case Dev_Type_RT1052_AMPLIFIER_AA4500_Type1:
        case Dev_Type_RT1052_AMPLIFIER_AA2500:
            meterList<<"A-In1"<<"A-In2"<<"A-In3"<<"A-In4"<<"A-Out1"<<"A-Out2"<<"A-Out3"<<"A-Out4";

            clippersList<<"A-In1(Gain)"  <<"A-In2(Gain)"  <<"A-In3(Gain)"  <<"A-In4(Gain)"
                        <<"N-In1(Gain)"  <<"N-In2(Gain)"  <<"N-In3(Gain)"  <<"N-In4(Gain)"
                        <<"A-Out1(Gain)" <<"A-Out2(Gain)" <<"A-Out3(Gain)" <<"A-Out4(Gain)"
                        <<"A-Out1(Delay)"<<"A-Out2(Delay)"<<"A-Out3(Delay)"<<"A-Out4(Delay)";

            buttonList<<"A-In1(mute)"  <<"A-In2(mute)"  <<"A-In3(mute)"  <<"A-In4(mute)"
                      <<"A-In1(Eq)"    <<"A-In2(Eq)"    <<"A-In3(Eq)"    <<"A-In4(Eq)"
                      <<"A-Out1(xover)"<<"A-Out2(xover)"<<"A-Out3(xover)"<<"A-Out4(xover)"
                      <<"A-Out1(Eq)"   <<"A-Out2(Eq)"   <<"A-Out3(Eq)"   <<"A-Out4(Eq)"
                      <<"A-Out1(mute)" <<"A-Out2(mute)" <<"A-Out3(mute)" <<"A-Out4(mute)"
                      <<"A-Out1(DYN)"  <<"A-Out2(DYN)"  <<"A-Out3(DYN)"  <<"A-Out4(DYN)"
                      <<"A-Out1(Phase)"<<"A-Out2(Phase)"<<"A-Out3(Phase)"<<"A-Out4(Phase)"
                      <<"N-In1(mute)"  <<"N-In2(mute)"  <<"N-In3(mute)"  <<"N-In4(mute)"
                      <<"Save";
            break;
        case Dev_Type_RT1052_MONITORING_AIDS:
            break;
        }
        qDebug()<<__FUNCTION__<<__LINE__<<"monitorItemType:"<<type;
        switch(type)
        {
        case CFG_TYPE_1:
            return meterList;
        case CFG_TYPE_2:
            return clippersList;
        case CFG_TYPE_3:
            return buttonList;
        default:
            QStringList NoneList;
            NoneList<<"None";
            return NoneList;
        }
}

MonitoringFactory::MonitoringFactory()
{

}
