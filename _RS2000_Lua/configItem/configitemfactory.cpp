#include "configitemfactory.h"
#include "utils.h"
#include "configitem.h"
#include "configiteminput.h"
#include "configitemoutput.h"
#include "configitemdelay.h"
#include "configitemrouter.h"
#include "configitemmixer.h"
#include "configitemdyn.h"
#include "configitemeq.h"
#include "configitemfilte.h"
#include "configitemmeter.h"
#include "configitemsignal.h"
#include "configitemsplit.h"
#include "configitemgpi.h"
#include "configitemautomaticmixer.h"
#include "configitemfreqshift.h"
#include "configitemFIRCustom.h"
#include "Frame/absgraphview.h"
#include "configitemaec.h"
#include "configitemmicrophonecontrol.h"
#include "configitemNFC.h"
#include "configitemsnapshot.h"
#include "configitempriorityducker.h"
#include "configitemgainramp.h"
#include "configitemaudioplayer.h"
#include "configitemaudiorecord.h"
#include "configitemgain.h"
#include "configitemcrossfader.h"
#include "configitemsoftaec.h"
#include "CtrlItem/ctrlItem.h"
#include "configitemuci.h"
#include "device/device_trn.h"

ConfigItem *ConfigItemFactory::createConfigItem(Device_Trn* trnDev, CfgType cfgType, QString customName, QPointF scenePos)
{
    QString cfgTypeName = Utils::getCfgTypeName(cfgType);
    ConfigItem *cfgItem = NULL;
    switch(cfgType) {
    case CFG_TYPE_INPUT_1:
    case CFG_TYPE_INPUT_2:
    case CFG_TYPE_INPUT_4:
    case CFG_TYPE_INPUT_8:
    case CFG_TYPE_INPUT_16:
        cfgItem = new ConfigItemInput(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_OUTPUT_1:
    case CFG_TYPE_OUTPUT_2:
    case CFG_TYPE_OUTPUT_4:
    case CFG_TYPE_OUTPUT_8:
    case CFG_TYPE_OUTPUT_16:
        cfgItem = new ConfigItemOutput(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_DELAY_5:
    case CFG_TYPE_DELAY_10:
    case CFG_TYPE_DELAY_20:
    case CFG_TYPE_DELAY_50:
    case CFG_TYPE_DELAY_100:
    case CFG_TYPE_DELAY_500:
    case CFG_TYPE_DELAY_1000:
    case CFG_TYPE_HPDELY_1:
    case CFG_TYPE_HPDELY_5:
    case CFG_TYPE_HPDELY_10:
    case CFG_TYPE_HPDELY_20:
    case CFG_TYPE_HPDELY_50:
        cfgItem = new ConfigItemDelay(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_ROUTER_4:
    case CFG_TYPE_ROUTER_8:
    case CFG_TYPE_ROUTER_16:
    case CFG_TYPE_ROUTER_32:
    case CFG_TYPE_ROUTER_48:
    case CFG_TYPE_ROUTER_64:
    case CFG_TYPE_ROUTER_96:
    case CFG_TYPE_ROUTER_128:
    case CFG_TYPE_ROUTER_192:
    case CFG_TYPE_ROUTER_256:
        cfgItem = new ConfigItemRouter(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_MIXER_4X1:
    case CFG_TYPE_MIXER_4X2:
    case CFG_TYPE_MIXER_4X4:
    case CFG_TYPE_MIXER_8X1:
    case CFG_TYPE_MIXER_8X2:
    case CFG_TYPE_MIXER_8X4:
    case CFG_TYPE_MIXER_8X8:
    case CFG_TYPE_MIXER_16X16:
    case CFG_TYPE_MIXER_32X32:
    case CFG_TYPE_MIXER_64X64:
//    case CFG_TYPE_MIXER_20X20:
        cfgItem = new ConfigItemMixer(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_DYN_AUTO:
    case CFG_TYPE_DYN_COMP:
    case CFG_TYPE_DYN_LIMIT:
    case CFG_TYPE_DYN_NOISE:
    case CFG_TYPE_DYN_EXPAND:
    case CFG_TYPE_DYN_PEAK:
        cfgItem = new ConfigItemDyn(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_EQ_1:
    case CFG_TYPE_EQ_3:
    case CFG_TYPE_EQ_5:
    case CFG_TYPE_EQ_7:
    case CFG_TYPE_EQ_10:
    case CFG_TYPE_EQ_16:
    case CFG_TYPE_EQ_1X3:
    case CFG_TYPE_EQ_2X3:
    case CFG_TYPE_EQ_3X3:
        cfgItem = new ConfigItemEq(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_FILTE_LP:
    case CFG_TYPE_FILTE_HP:
    case CFG_TYPE_FILTE_LS:
    case CFG_TYPE_FILTE_HS:
    case CFG_TYPE_FILTER_BP:
    case CFG_TYPE_FILTER_AP:
        cfgItem = new ConfigItemFilte(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_SPLIT_2:
    case CFG_TYPE_SPLIT_3:
    case CFG_TYPE_SPLIT_4:
        cfgItem = new ConfigItemSplit(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_SIG_SINE:
    case CFG_TYPE_SIG_NOISE:
    case CFG_TYPE_SIG_PINKNOISE:
        cfgItem = new ConfigItemSignal(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_METER_1:
    case CFG_TYPE_METER_2:
    case CFG_TYPE_METER_4:
    case CFG_TYPE_METER_8:
        cfgItem = new ConfigItemMeter(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_GPI:
        cfgItem = new ConfigItemGPI(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_FREQSHIFT:
    case CFG_TYPE_FREQSHIFT_PLUS:
        cfgItem = new ConfigItemFreqShift(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_AUTOMIX_GS_4:
    case CFG_TYPE_AUTOMIX_GS_8:
    case CFG_TYPE_AUTOMIX_GS_16:
    case CFG_TYPE_AUTOMIX_GS_32:
    case CFG_TYPE_AUTOMIXNGAT_4:
    case CFG_TYPE_AUTOMIXNGAT_8:
    case CFG_TYPE_AUTOMIXNGAT_16:
    case CFG_TYPE_AUTOMIXNGAT_32:
    case CFG_TYPE_AUTOMIXNGRT_4:
    case CFG_TYPE_AUTOMIXNGRT_8:
    case CFG_TYPE_AUTOMIXNGRT_16:
    case CFG_TYPE_AUTOMIXNGRT_32:
        cfgItem = new ConfigItemAutomaticMixer(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_AEC_2_2:
        cfgItem = new ConfigItemAEC(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_VOICETUBE_1:
    case CFG_TYPE_VOICETUBE_2:
        cfgItem = new ConfigItemMicrophoneControl(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_SOFTFIR_MONO256:
    case CFG_TYPE_SOFTFIR_MONO512:
    case CFG_TYPE_SOFTFIR_MONO1024:
    case CFG_TYPE_SOFTFIR_STEREO256:
    case CFG_TYPE_SOFTFIR_STEREO512:
    case CFG_TYPE_SOFTFIR_STEREO1024:
    case CFG_TYPE_SOFTFIR_4R256:
    case CFG_TYPE_SOFTFIR_4R512:
    case CFG_TYPE_SOFTFIR_4R1024:
        cfgItem = new ConfigItemFIRCustom(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_NFC1_8NF:
    case CFG_TYPE_NFC1_16NF:
    case CFG_TYPE_NFC1_24NF:
    case CFG_TYPE_NFC1_32NF:
        cfgItem = new ConfigItemNFC(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_PDUCKER_4:
    case CFG_TYPE_PDUCKER_8:
    case CFG_TYPE_PDUCKER_16:
    case CFG_TYPE_PDUCKER_32:
        cfgItem = new ConfigItemPriorityDucker(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_AUDIO_PLAYER1:
    case CFG_TYPE_AUDIO_PLAYER2:
    case CFG_TYPE_AUDIO_PLAYER4:
    case CFG_TYPE_AUDIO_PLAYER8:
    case CFG_TYPE_AUDIO_PLAYER16:
        cfgItem = new ConfigItemAudioPlayer(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_AUDIO_RECORD1:
    case CFG_TYPE_AUDIO_RECORD2:
    case CFG_TYPE_AUDIO_RECORD4:
        cfgItem = new ConfigItemAudioRecord(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_GAINRAMP_1:
    case CFG_TYPE_GAINRAMP_2:
    case CFG_TYPE_GAINRAMP_4:
    case CFG_TYPE_GAINRAMP_8:
    case CFG_TYPE_GAINRAMP_16:
    case CFG_TYPE_GAINRAMP_32:
        cfgItem = new ConfigItemGainRamp(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_GAIN_1:
    case CFG_TYPE_GAIN_2:
    case CFG_TYPE_GAIN_4:
    case CFG_TYPE_GAIN_8:
    case CFG_TYPE_GAIN_16:
    case CFG_TYPE_GAIN_32:
        cfgItem = new ConfigItemGain(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_CROSSFADER_2X1:
    case CFG_TYPE_CROSSFADER_4X2:
    case CFG_TYPE_CROSSFADER_8X4:
    case CFG_TYPE_CROSSFADER_16X8:
    case CFG_TYPE_CROSSFADER_32X16:
        cfgItem = new ConfigItemCrossfader(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_SOFTAEC_8K64://AEC-Soft
    case CFG_TYPE_SOFTAEC_8K96:
    case CFG_TYPE_SOFTAEC_8K128:
    case CFG_TYPE_SOFTAEC_8K160:
    case CFG_TYPE_SOFTAEC_8K192:
    case CFG_TYPE_SOFTAEC_8K224:
    case CFG_TYPE_SOFTAEC_8K256:
    case CFG_TYPE_SOFTAEC_16K64:
    case CFG_TYPE_SOFTAEC_16K96:
    case CFG_TYPE_SOFTAEC_16K128:
    case CFG_TYPE_SOFTAEC_16K160:
    case CFG_TYPE_SOFTAEC_16K192:
    case CFG_TYPE_SOFTAEC_16K224:
    case CFG_TYPE_SOFTAEC_16K256:
    case CFG_TYPE_SOFTAEC_IK_8K64:
    case CFG_TYPE_SOFTAEC_IK_8K96:
    case CFG_TYPE_SOFTAEC_IK_8K128:
    case CFG_TYPE_SOFTAEC_IK_8K160:
    case CFG_TYPE_SOFTAEC_IK_8K192:
    case CFG_TYPE_SOFTAEC_IK_8K256:
    case CFG_TYPE_SOFTAEC_IK_16K64:
    case CFG_TYPE_SOFTAEC_IK_16K96:
    case CFG_TYPE_SOFTAEC_IK_16K128:
    case CFG_TYPE_SOFTAEC_IK_16K160:
    case CFG_TYPE_SOFTAEC_IK_16K192:
    case CFG_TYPE_SOFTAEC_IK_16K256:
//    case CFG_TYPE_SOFTAEC_IK_48K64:
//    case CFG_TYPE_SOFTAEC_IK_48K96:
//    case CFG_TYPE_SOFTAEC_IK_48K128:
//    case CFG_TYPE_SOFTAEC_IK_48K160:
//    case CFG_TYPE_SOFTAEC_IK_48K192:
//    case CFG_TYPE_SOFTAEC_IK_48K256:
        cfgItem = new ConfigItemSoftAEC(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_SNAPSHOT:
    case CFG_TYPE_SNAPSHOT_BANK:
        cfgItem = new ConfigItemSnapshot(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_UCI:
        cfgItem = new ConfigItemUCI(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_CTRL_LUA:
        cfgItem = new CtrlItemLua(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_CTRL_GPI:
        cfgItem = new CtrlItemGpi(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_CTRL_GPO:
        cfgItem = new CtrlItemGpo(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_CTRL_AD:
        cfgItem = new CtrlItemADC(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_CTRL_RS232:
        cfgItem = new CtrlItemRs232(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    case CFG_TYPE_CTRL_RS485:
        cfgItem = new CtrlItemRs485(trnDev, cfgType, customName, cfgTypeName, scenePos);
        break;
    default:
        qDebug()<< "unknown type:" << cfgType;
        assert(false);
    }

    if (cfgItem != NULL && trnDev != NULL) {
//        cfgItem->setUserZValue(customName.split('_').last().toInt());
        cfgItem->setUserZValue(trnDev->getItemUserZValueNum(true));//保存设计文件时，分层定位
//        cfgItem->setZValue(trnDev->getItemUserZValueNum(true));//复制粘贴，保持相同的堆叠层次
    }
    return cfgItem;
}

bool ConfigItemFactory::isStartCfgItem(CfgType cfgType)
{
    uint16_t family_type = cfgType & FCT_FAMILY_MASK;
    if(family_type == CFG_TYPE_INPUT_1 || family_type == CFG_TYPE_SIG_SINE || family_type == CFG_TYPE_AUDIO_PLAYER1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ConfigItemFactory::isEndCfgItem(CfgType cfgType)
{
    uint16_t family_type = cfgType & FCT_FAMILY_MASK;
    if(family_type == CFG_TYPE_OUTPUT_1 || family_type == CFG_TYPE_METER_1 || family_type == CFG_TYPE_AUDIO_RECORD1)
    {
        return true;
    }
    else
    {
        return false;
    }
}



