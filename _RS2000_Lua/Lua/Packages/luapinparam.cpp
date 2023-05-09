#include "luapinparam.h"
#include "Lua/Packages/centerctrl.h"

CLuaPinParam::CLuaPinParam()
{

}


QVariant CLuaPinParam::getValueImpl(QString devIp, QString itemName, quint32 getCmdKey, quint8 exData1, quint8 exData2)
{
    CenterCtrl::instance()->setDevAddress(devIp);
    QVariant val;
    bool bVal = false;
    double dVal, dMaxVal, dMinVal;
    quint16 i16Val,i16MaxVal;
    quint8 i8Val = 0;
    bool rs = false;
    switch(getCmdKey)
    {
    case 0xFFFFFFFF: //根节点跳过设置命令
        assert(false);
        return QVariant();
    //================输入模块================//
    case 0x00000001:
        rs = CenterCtrl::instance()->getInputAntiPhase(itemName, exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x00000003:
        rs = CenterCtrl::instance()->getInputMute(itemName, exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x00000005:
        rs = CenterCtrl::instance()->getInputGain(itemName, exData1, dVal);
        val.setValue(dVal);
        break;
    //================输出模块================//
    case 0x00010001:
        rs = CenterCtrl::instance()->getOutputAntiPhase(itemName, exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x00010003:
        rs = CenterCtrl::instance()->getOutputMute(itemName, exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x00010005:
        rs = CenterCtrl::instance()->getOutputGain(itemName, exData1, dVal);
        val.setValue(dVal);
        break;
    //================延时模块================//
    case 0x00020001:
        rs = CenterCtrl::instance()->getDelayBypass(itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x00020003:
        rs = CenterCtrl::instance()->getDelayTime(itemName, dVal, dMaxVal);
        val.setValue(dVal);
        break;
    //================路由模块================//
    case 0x00030001:
        rs = CenterCtrl::instance()->getRouterOutput(itemName, exData1, i8Val);
        val.setValue(i8Val);
        break;
    //================混音模块================//
    case 0x00040001:
        rs = CenterCtrl::instance()->getMixerInputState(itemName, exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x00040003:
        rs = CenterCtrl::instance()->getMixerInputGain(itemName, exData1, dVal);
        val.setValue(dVal);
        break;
    case 0x00040005:
        rs = CenterCtrl::instance()->getMixerOutputState(itemName, exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x00040007:
        rs = CenterCtrl::instance()->getMixerOutputGain(itemName, exData1, dVal);
        val.setValue(dVal);
        break;
    case 0x00040009:
        rs = CenterCtrl::instance()->getMixerNodeState(itemName, exData1, exData2, bVal);
        val.setValue(bVal);
        break;
    case 0x0004000B:
        rs = CenterCtrl::instance()->getMixerNodeGain(itemName, exData1, exData2, dVal);
        val.setValue(dVal);
        break;
    //================混音模块================//
    case 0x00050001:
        rs = CenterCtrl::instance()->getDynThreshold(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x00050003:
        rs = CenterCtrl::instance()->getDynRatio(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x00050005:
        rs = CenterCtrl::instance()->getDynAttackTime(itemName, i16Val, i16MaxVal);
        val.setValue(i16Val);
        break;
    case 0x00050007:
        rs = CenterCtrl::instance()->getDynReleaseTime(itemName, i16Val, i16MaxVal);
        val.setValue(i16Val);
        break;
    case 0x00050009:
        rs = CenterCtrl::instance()->getDynGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x0005000B:
        rs = CenterCtrl::instance()->getDynTargetLevel(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x0005000D:
        rs = CenterCtrl::instance()->getDynBypass(itemName, bVal);
        val.setValue(bVal);
        break;
    //================均衡模块================//
    case 0x00060001:
        rs = CenterCtrl::instance()->getEqFreq(itemName, exData1, i16Val);
        val.setValue(i16Val);
        break;
    case 0x00060003:
        rs = CenterCtrl::instance()->getEqGain(itemName, exData1, dVal, dMaxVal, dMinVal);
        val.setValue(dVal);
        break;
    case 0x00060005:
        rs = CenterCtrl::instance()->getEqQVal(itemName, exData1, dVal);
        val.setValue(dVal);
        break;
    case 0x00060007:
        rs = CenterCtrl::instance()->getEqBypass(itemName, bVal);
        val.setValue(bVal);
        break;
    //================信号发生器模块================//
    case 0x00070001:
        rs = CenterCtrl::instance()->getSigFreq(itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x00070003:
        rs = CenterCtrl::instance()->getSigGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x00070005:
        rs = CenterCtrl::instance()->getSigMute(itemName, bVal);
        val.setValue(bVal);
        break;
    //================分频器模块================//
    case 0x00080001:
        rs = CenterCtrl::instance()->getSplitInMute(itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x00080003:
        rs = CenterCtrl::instance()->getSplitInGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x00080005:
        rs = CenterCtrl::instance()->getSplitType(itemName, exData1, i8Val);
        val.setValue(i8Val);
        break;
    case 0x00080007:
        rs = CenterCtrl::instance()->getSplitFreq(itemName, exData1, i16Val);
        val.setValue(i16Val);
        break;
    case 0x00080009:
        rs = CenterCtrl::instance()->getSplitOutMute(itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x0008000B:
        rs = CenterCtrl::instance()->getSplitOutGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x0008000D:
        rs = CenterCtrl::instance()->getSplitOutAntiPhase(itemName, bVal);
        val.setValue(bVal);
        break;
    //================滤波器模块================//
    case 0x00090001:
        rs = CenterCtrl::instance()->getFilterType(itemName, i8Val);
        val.setValue(i8Val);
        break;
    case 0x00090003:
        rs = CenterCtrl::instance()->getFilterFreq(itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x00090005:
        rs = CenterCtrl::instance()->getFilterGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x00090007:
        rs = CenterCtrl::instance()->getFilterBypass(itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x00090009:
        rs = CenterCtrl::instance()->getFilterBandwidth(itemName, dVal);
        val.setValue(dVal);
        break;
    //================啸叫抑制模块================//
    case 0x000A0001:
        rs = CenterCtrl::instance()->getFreqshiftFreq(itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000A0003:
        rs = CenterCtrl::instance()->getFreqshiftGain(itemName, dVal);
        val.setValue(dVal);
        break;
    //================自动混音模块================//
    case 0x000B0001:
        rs = CenterCtrl::instance()->getAutoMixerThreshold(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0003:
        rs = CenterCtrl::instance()->getAutoMixerAttackTime(itemName, i16Val, i16MaxVal);
        val.setValue(i16Val);
        break;
    case 0x000B0005:
        rs = CenterCtrl::instance()->getAutoMixerReleaseTime(itemName, i16Val, i16MaxVal);
        val.setValue(i16Val);
        break;
    case 0x000B0007:
        rs = CenterCtrl::instance()->getAutoMixerHoldTime(itemName, i16Val, i16MaxVal);
        val.setValue(i16Val);
        break;
    case 0x000B000B:
        rs = CenterCtrl::instance()->getAutoMixerDepth(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B000F:
        rs = CenterCtrl::instance()->getAutoMixerNoiseGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0011:
        rs = CenterCtrl::instance()->getAutoMixerInputMute(itemName, exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x000B0013:
        rs = CenterCtrl::instance()->getAutoMixerInputGain(itemName, exData1, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0015:
        rs = CenterCtrl::instance()->getAutoMixerOutputMute(itemName, exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x000B0017:
        rs = CenterCtrl::instance()->getAutoMixerOutputGain(itemName, exData1, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0019:
        rs = CenterCtrl::instance()->getAutoMixerInputManual(itemName, exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x000B001B:
        rs = CenterCtrl::instance()->getAutoMixerInputMode(itemName, exData1, i8Val);
        val.setValue(i8Val);
        break;
    case 0x000B001D:
        rs = CenterCtrl::instance()->getAutoMixerInputPriority(itemName, exData1, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000B001F:
        rs = CenterCtrl::instance()->getAutoMixerPercentage(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0023:
        rs = CenterCtrl::instance()->getAutoMixerLastMicOn(itemName, i8Val);
        val.setValue(i8Val);
        break;
    case 0x000B0025:
        rs = CenterCtrl::instance()->getAutoMixerDefaultCh(itemName, i8Val);
        val.setValue(i8Val);
        break;
    case 0x000B0027:
        rs = CenterCtrl::instance()->getAutoMixerThresholdLevelAboveNoise(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0029:
        rs = CenterCtrl::instance()->getAutoMixerMaxNOM(itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x000B002B:
        rs = CenterCtrl::instance()->getAutoMixerDirectOutsNOMAtten(itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x000B002D:
        rs = CenterCtrl::instance()->getAutoMixerNOMAttenuationType(itemName, i8Val);
        val.setValue(i8Val);
        break;
    case 0x000B002F:
        rs = CenterCtrl::instance()->getAutoMixerNOMAttenuationStep(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0031:
        rs = CenterCtrl::instance()->getAutoMixerMaxNOMAttenuation(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0033:
        rs = CenterCtrl::instance()->getAutoMixerResponseTime(itemName, i16Val);
        val.setValue(i16Val);
        break;
    //================回声消除模块================//
    case 0x000C0001:
        rs = CenterCtrl::instance()->getAECMicInGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000C0003:
        rs = CenterCtrl::instance()->getAECLineInGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000C0005:
        rs = CenterCtrl::instance()->getAECSpeakOutGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000C0007:
        rs = CenterCtrl::instance()->getAECLineOutGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000C0009:
        rs = CenterCtrl::instance()->getAECMicToSpeakGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000C000B:
        rs = CenterCtrl::instance()->getAECMicToSpeakGainOn(itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x000C000D:
        rs = CenterCtrl::instance()->getAECNRGain(itemName, i8Val);
        val.setValue(i8Val);
        break;
    case 0x000C000F:
        rs = CenterCtrl::instance()->getAECNRGainOn(itemName, bVal);
        val.setValue(bVal);
        break;
    //================闪避模块================//
    case 0x000D0001:
        rs = CenterCtrl::instance()->getDuckerAttackTime(itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000D0003:
        rs = CenterCtrl::instance()->getDuckerReleaseTime(itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000D0005:
        rs = CenterCtrl::instance()->getDuckerHoldTime(itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000D0007:
        rs = CenterCtrl::instance()->getDuckerDetectTime(itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000D0009:
        rs = CenterCtrl::instance()->getDuckerThreshold(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000D000B:
        rs = CenterCtrl::instance()->getDuckerDepth(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000D000D:
        rs = CenterCtrl::instance()->getDuckerInGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000D000F:
        rs = CenterCtrl::instance()->getDuckerPriorityGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000D0011:
        rs = CenterCtrl::instance()->getDuckerOutGain(itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000D0013:
        rs = CenterCtrl::instance()->getDuckerBypass(itemName, bVal);
        val.setValue(bVal);
        break;
    //================淡入淡出模块================//
    case 0x000E0001:
        rs = CenterCtrl::instance()->getGainRampTime(itemName, exData1, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000E0003:
        rs = CenterCtrl::instance()->getGainRampGain(itemName, exData1, dVal);
        val.setValue(dVal);
        break;
    case 0x000E0005:
        rs = CenterCtrl::instance()->getGainRampType(itemName, i8Val);
        val.setValue(i8Val);
        break;
    //================播放器模块================//
//    case 0x000F0001:
//        rs = CenterCtrl::instance()->getPlayerState(itemName, exData1); //wj 触发消息有没有更好处理方法
//        newVal = val.toInt();
//        break;

    default:
        break;
    }

    return val;
}


void CLuaPinParam::setValueImpl(QString devIp, QString itemName, quint32 setCmdKey, quint8 exData1, quint8 exData2, QVariant val)
{
    CenterCtrl::instance()->setDevAddress(devIp);
    bool rs = false;
    QVariant newVal;
    switch(setCmdKey)
    {
    case 0xFFFFFFFF: //根节点跳过设置命令
        rs = true;
        newVal = val;
        break;
    //================输入模块================//
    case 0x00000002:
        rs = CenterCtrl::instance()->setInputAntiPhase(itemName, exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00000004:
        rs = CenterCtrl::instance()->setInputMute(itemName, exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00000006:
        rs = CenterCtrl::instance()->setInputGain(itemName, exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    //================输出模块================//
    case 0x00010002:
        rs = CenterCtrl::instance()->setOutputAntiPhase(itemName, exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00010004:
        rs = CenterCtrl::instance()->setOutputMute(itemName, exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00010006:
        rs = CenterCtrl::instance()->setOutputGain(itemName, exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    //================延时模块================//
    case 0x00020002:
        rs = CenterCtrl::instance()->setDelayBypass(itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00020004:
        rs = CenterCtrl::instance()->setDelayTime(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    //================路由模块================//
    case 0x00030002:
        rs = CenterCtrl::instance()->setRouterOutput(itemName, exData1, val.toUInt());
        newVal = val.toUInt();
        break;
    //================混音模块================//
    case 0x00040002:
        rs = CenterCtrl::instance()->setMixerInputState(itemName, exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00040004:
        rs = CenterCtrl::instance()->setMixerInputGain(itemName, exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00040006:
        rs = CenterCtrl::instance()->setMixerOutputState(itemName, exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00040008:
        rs = CenterCtrl::instance()->setMixerOutputGain(itemName, exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x0004000A:
        rs = CenterCtrl::instance()->setMixerNodeState(itemName, exData1, exData2, val.toBool());
        newVal = val.toBool();
        break;
    case 0x0004000C:
        rs = CenterCtrl::instance()->setMixerNodeGain(itemName, exData1, exData2, val.toDouble());
        newVal = val.toDouble();
        break;
    //================混音模块================//
    case 0x00050002:
        rs = CenterCtrl::instance()->setDynThreshold(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00050004:
        rs = CenterCtrl::instance()->setDynRatio(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00050006:
        rs = CenterCtrl::instance()->setDynAttackTime(itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x00050008:
        rs = CenterCtrl::instance()->setDynReleaseTime(itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x0005000A:
        rs = CenterCtrl::instance()->setDynGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x0005000C:
        rs = CenterCtrl::instance()->setDynTargetLevel(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x0005000E:
        rs = CenterCtrl::instance()->setDynBypass(itemName, val.toBool());
        newVal = val.toBool();
        break;
    //================均衡模块================//
    case 0x00060002:
        rs = CenterCtrl::instance()->setEqFreq(itemName, exData1, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x00060004:
        rs = CenterCtrl::instance()->setEqGain(itemName, exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00060006:
        rs = CenterCtrl::instance()->setEqQVal(itemName, exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00060008:
        rs = CenterCtrl::instance()->setEqBypass(itemName, val.toBool());
        newVal = val.toBool();
        break;
    //================信号发生器模块================//
    case 0x00070002:
        rs = CenterCtrl::instance()->setSigFreq(itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x00070004:
        rs = CenterCtrl::instance()->setSigGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00070006:
        rs = CenterCtrl::instance()->setSigMute(itemName, val.toBool());
        newVal = val.toBool();
        break;
    //================分频器模块================//
    case 0x00080002:
        rs = CenterCtrl::instance()->setSplitInMute(itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00080004:
        rs = CenterCtrl::instance()->setSplitInGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00080006:
        rs = CenterCtrl::instance()->setSplitType(itemName, exData1, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x00080008:
        rs = CenterCtrl::instance()->setSplitFreq(itemName, exData1, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x0008000A:
        rs = CenterCtrl::instance()->setSplitOutMute(itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x0008000C:
        rs = CenterCtrl::instance()->setSplitOutGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x0008000E:
        rs = CenterCtrl::instance()->setSplitOutAntiPhase(itemName, val.toBool());
        newVal = val.toBool();
        break;
    //================滤波器模块================//
    case 0x00090002:
        rs = CenterCtrl::instance()->setFilterType(itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x00090004:
        rs = CenterCtrl::instance()->setFilterFreq(itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x00090006:
        rs = CenterCtrl::instance()->setFilterGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00090008:
        rs = CenterCtrl::instance()->setFilterBypass(itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x0009000A:
        rs = CenterCtrl::instance()->setFilterBandwidth(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    //================啸叫抑制模块================//
    case 0x000A0002:
        rs = CenterCtrl::instance()->setFreqshiftFreq(itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x000A0004:
        rs = CenterCtrl::instance()->setFreqshiftGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    //================自动混音模块================//
    case 0x000B0002:
        rs = CenterCtrl::instance()->setAutoMixerThreshold(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0004:
        rs = CenterCtrl::instance()->setAutoMixerAttackTime(itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x000B0006:
        rs = CenterCtrl::instance()->setAutoMixerReleaseTime(itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x000B0008:
        rs = CenterCtrl::instance()->setAutoMixerHoldTime(itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x000B000C:
        rs = CenterCtrl::instance()->setAutoMixerDepth(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0010:
        rs = CenterCtrl::instance()->setAutoMixerNoiseGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0012:
        rs = CenterCtrl::instance()->setAutoMixerInputMute(itemName, exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x000B0014:
        rs = CenterCtrl::instance()->setAutoMixerInputGain(itemName, exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0016:
        rs = CenterCtrl::instance()->setAutoMixerOutputMute(itemName, exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x000B0018:
        rs = CenterCtrl::instance()->setAutoMixerOutputGain(itemName, exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B001A:
        rs = CenterCtrl::instance()->setAutoMixerInputManual(itemName, exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x000B001C:
        rs = CenterCtrl::instance()->setAutoMixerInputMode(itemName, exData1, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000B001E:
        rs = CenterCtrl::instance()->setAutoMixerInputPriority(itemName, exData1, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000B0020:
        rs = CenterCtrl::instance()->setAutoMixerPercentage(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0024:
        rs = CenterCtrl::instance()->setAutoMixerLastMicOn(itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000B0026:
        rs = CenterCtrl::instance()->setAutoMixerDefaultCh(itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000B0028:
        rs = CenterCtrl::instance()->setAutoMixerThresholdLevelAboveNoise(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B002A:
        rs = CenterCtrl::instance()->setAutoMixerMaxNOM(itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x000B002C:
        rs = CenterCtrl::instance()->setAutoMixerDirectOutsNOMAtten(itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x000B002E:
        rs = CenterCtrl::instance()->setAutoMixerNOMAttenuationType(itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000B0030:
        rs = CenterCtrl::instance()->setAutoMixerNOMAttenuationStep(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0032:
        rs = CenterCtrl::instance()->setAutoMixerMaxNOMAttenuation(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0034:
        rs = CenterCtrl::instance()->setAutoMixerResponseTime(itemName, val.toInt());
        newVal = val.toInt();
        break;
    //================回声消除模块================//
    case 0x000C0002:
        rs = CenterCtrl::instance()->setAECMicInGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000C0004:
        rs = CenterCtrl::instance()->setAECLineInGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000C0006:
        rs = CenterCtrl::instance()->setAECSpeakOutGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000C0008:
        rs = CenterCtrl::instance()->setAECLineOutGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000C000A:
        rs = CenterCtrl::instance()->setAECMicToSpeakGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000C000C:
        rs = CenterCtrl::instance()->setAECMicToSpeakGainOn(itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x000C000E:
        rs = CenterCtrl::instance()->setAECNRGain(itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000C0010:
        rs = CenterCtrl::instance()->setAECNRGainOn(itemName, val.toBool());
        newVal = val.toBool();
        break;
    //================闪避模块================//
    case 0x000D0002:
        rs = CenterCtrl::instance()->setDuckerAttackTime(itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000D0004:
        rs = CenterCtrl::instance()->setDuckerReleaseTime(itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000D0006:
        rs = CenterCtrl::instance()->setDuckerHoldTime(itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000D0008:
        rs = CenterCtrl::instance()->setDuckerDetectTime(itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000D000A:
        rs = CenterCtrl::instance()->setDuckerThreshold(itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000D000C:
        rs = CenterCtrl::instance()->setDuckerDepth(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000D000E:
        rs = CenterCtrl::instance()->setDuckerInGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000D0010:
        rs = CenterCtrl::instance()->setDuckerPriorityGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000D0012:
        rs = CenterCtrl::instance()->setDuckerOutGain(itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000D0014:
        rs = CenterCtrl::instance()->setDuckerBypass(itemName, val.toBool());
        newVal = val.toBool();
        break;
    //================淡入淡出模块================//
    case 0x000E0002:
        rs = CenterCtrl::instance()->setGainRampTime(itemName, exData1, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000E0004:
        rs = CenterCtrl::instance()->setGainRampGain(itemName, exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000E0006:
        rs = CenterCtrl::instance()->setGainRampType(itemName, val.toInt());
        newVal = val.toInt();
        break;
    //================播放器模块================//
    case 0x000F0002:
        rs = CenterCtrl::instance()->setPlayerState(itemName, exData1); //wj 触发消息有没有更好处理方法
        newVal = val.toInt();
        break;

    default:
        break;
    }
}
