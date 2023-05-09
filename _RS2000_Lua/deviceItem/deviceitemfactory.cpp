#include "stable.h"
#include "deviceitemfactory.h"

#include "deviceitem_trn.h"
#include "deviceitem_stm32_poe_0_2.h"
#include "deviceitem_stm32_poe_0_1.h"
#include "deviceitem_amp_quad.h"
#include "deviceitem_amp_dn8000.h"
#include "deviceitem_stm32_poe_1_0.h"
#include "deviceitem_dvs.h"
#include "deviceitem_spk.h"
#include "deviceitem_monitor_spk.h"
#include "deviceitem_mic_quad.h"
#include "deviceitem_amp_0_2_line.h"
#include "deviceitem_amp_2_0_mic_line.h"
#include "deviceitem_rt1052_poe_60w_2_4.h"
#include "deviceitem_rt1052_poe_AP60_Bluetooth.h"
#include "deviceitem_rt1052_amp_module_4_4.h"
#include "concretedeviceitem.h"
#include "deviceitem_monitoring_aids.h"
#include "deviceitem_rt1052_AP400.h"
#include "deviceitem_room.h"
#include "Frame/mainwindow.h"

DeviceItem* DeviceItemFactory::CreateInstance(DevType devType, QString devName, QPointF scenePos,
                                        QGraphicsItem *parent)
{
//    qDebug() << __FUNCTION__ << __LINE__ << devType << devName;
    DeviceItem* device = NULL;
    switch(devType) {
    case Dev_Type_VirtualSoundCard_Dante:
    case Dev_Type_VirtualSoundCard_AES67:
    case Dev_Type_Zynq7010_AES3ToAES67_DongFangJiaLian:
    case Dev_Type_RT1052_DR3020:
        device = new DeviceItem_DVS(devType, devName, scenePos, parent);
        break;
    case Dev_Type_NAP_Zynq7020_16Analog_16AES67:
    case Dev_Type_NAP_Zynq7020_16Analog_48AES67:
    case Dev_Type_NAP_Zynq7020_64AES67:
    case Dev_Type_NAP_Zynq7020_16Analog_16AES67DanteMixer:
    case Dev_Type_NAP_Zynq7020_16Analog_16Dante:
    case Dev_Type_NAP_Zynq7020_16Analog_16Dante_Type1:
    case Dev_Type_NAP_Zynq7020_16Analog_32Dante:
    case Dev_Type_NAP_X86_16Analog_128AES67:
    case Dev_Type_NAP_X86_32Analog_256AES67:
    case Dev_Type_NAP_FT2000_256AES67: //基于arm的飞腾服务器
    //FT
    case Dev_Type_NAP_STM32H750_8A8N:
    case Dev_Type_NAP_150_FangTu:
    case Dev_Type_Mixer_128_128_FangTu:
    case Dev_Type_Mixer_256_256_FangTu:
        device = new DeviceItem_Trn(devType, devName, scenePos, parent);
        break;
    case Dev_Type_STM32_POE_AES67_SPK_0_2:
        device = new DeviceItem_Amp(devType, devName, scenePos, parent);
        break;
    case Dev_Type_STM32_POE_AES67_SPK_0_1:
        device = new DeviceItem_STM32_POE_0_1(devType, devName, scenePos, parent);
        break;
    case Dev_Type_RT1052_POE_AES67_LINE_4_4:
        device = new DeviceItem_Amp_Quad(devType, devName, scenePos, parent);
        break;
    case Dev_Type_RT1052_POE_AES67_MIC_4_0:
        //FT
    case Dev_Type_FionTu_WMIC_FangTu:
    case Dev_Type_I_Mic_Center_DN_FangTu:
        device = new DeviceItem_Mic_Quad(devType, devName, scenePos, parent);
        break;
    case Dev_Type_RT1052_POE_AES67_MIC_2_0:
    case Dev_Type_RT1052_POE_AES67_MIC_2_0_Type1:
    case Dev_Type_RT1052_POE_AES67_MIC_2_0_Type2:
    case Dev_Type_RT1052_POE_AES67_MIC_2_0_Type3:
    case Dev_Type_RT1052_POE_AES67_MIC_8_0:
        device = new DeviceItem_Amp_2_0_Mic_Line(devType, devName, scenePos, parent);
        break;
    case Dev_Type_RT1052_POE_AES67_LINE_0_2:
    case Dev_Type_RT1052_POE_AES67_LINE_0_8:
    case Dev_Type_AAF_CS_6_FangTu:
    case Dev_Type_AAF_CS_8_FangTu:
    case Dev_Type_AAF_CS_10_FangTu:
    case Dev_Type_AAF_CS_12_FangTu:
    case Dev_Type_AAF_CS_15_FangTu:
    case Dev_Type_AAF_CS_18_FangTu:
    case Dev_Type_DBN_4u_FangTu:
    case Dev_Type_DBN_8_10u_FangTu:
        device = new DeviceItem_Amp_0_2_Line(devType, devName, scenePos, parent);
        break;
    case Dev_Type_RT1052_AP60:
    case Dev_Type_RT1052_AP60_Type1:
    case Dev_Type_RT1052_AP60_Type2:
    //FT
    case Dev_Type_CS50_FangTu:
    case Dev_Type_CS80_FangTu:
    case Dev_Type_CS50A_FangTu:
    case Dev_Type_CS60A_FangTu:
    case Dev_Type_CS80A_FangTu:
        device = new DeviceItem_RT1052_Poe_60W_2_4(devType, devName, scenePos, parent);
        break;
    case Dev_Type_RT1052_AP60_Bluetooth:
        device = new DeviceItem_RT1052_Poe_AP60_Bluetooth(devType, devName, scenePos, parent);
        break;
    case Dev_Type_RT1052_AMPLIFIER_AA4500:
    case Dev_Type_RT1052_AMPLIFIER_AA4500_Type1:
    case Dev_Type_RT1052_AMPLIFIER_AA2500:
    //FT
    case Dev_Type_FT_4D300N_FangTu:
    case Dev_Type_FT_4D600N_FangTu:
    case Dev_Type_FT_4D800N_FangTu:
    case Dev_Type_FT_4A1200N_FangTu:
    case Dev_Type_FT_4A2000N_FangTu:
        device = new DeviceItem_RT1052_AMP_MODULE_4_4(devType, devName, scenePos, parent);
        break;        
    case Dev_Type_RT1052_Amplifier_DongFangJiaLian:
        device = new DeviceItem_Amp_DN8000(devType, devName, scenePos, parent);
        break;
    case Dev_Type_RT1052_AP400:
    case Dev_Type_Dir_L445_FangTu:
    case Dev_Type_Dir_X_10_12_16_FangTu:
        device = new DeviceItem_RT1052_AP400(devType, devName, scenePos, parent);
        break;
     ////////////////////monitering_aids/////////////////////
    case Dev_Type_RT1052_MONITORING_AIDS:
    //FT: 暂时用作显示
    case Dev_Type_Switch_16x32_FangTu:
    case Dev_Type_Switch_POE_16x32_FangTu:
        device = new DeviceItem_Monitoring_Aids(devType, devName, scenePos, parent);
        break;
    case Dev_Type_AES67_MONITOR_SPK:
        device = new DeviceItem_Monitor_SPK(devType, devName, scenePos, parent);
        break;
    case Dev_Type_DNTE_SPK:
    case Dev_Type_AES67_SPK:
        device = new DeviceItem_Spk(devType, devName, scenePos, parent);
        break;
    case Dev_Type_STM32_POE_AES67_MIC_2_0:
        device = new DeviceItem_Mic(devType, devName, scenePos, parent);
        break;
    case Dev_Type_Normal_PC:
        device = new NormalPC_Device(devType, devName, scenePos, parent);
        break;
    case Dev_Type_Normal_CD:
        device = new NormalCD_Device(devType, devName, scenePos, parent);
        break;
    case Dev_Type_Normal_MIC:
        device = new NormalMicrophone_Device(devType, devName, scenePos, parent);
        break;
    case Dev_Type_Normal_SPK:
        device = new NormalSpeaker_Device(devType, devName, scenePos, parent);
        break;
    case Dev_Type_Normal_AMP:
        device = new NormalAmplifier_Device(devType, devName, scenePos, parent);
        break;
    case Dev_Type_Room:
        device = new DeviceItem_Room(devName, scenePos, parent);
        break;
    default:
        MainWindow::instance()->showNotice(QObject::tr("not implement device type:%1.")
                                           .arg(Config::curMfrsCfg().getDevTypeName(devType)));
    }

    if (device != NULL) {
//        device->setUserZValue(devName.split('_').last().toInt());
        static int zValue = 0;
        device->setUserZValue(zValue++);
    }
    return device;
}
