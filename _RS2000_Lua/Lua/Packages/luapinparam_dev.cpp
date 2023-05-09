#include "luapinparam_dev.h"
#include "centerctrl.h"

IG_PARACTL_LUAPIN::IG_PARACTL_LUAPIN()
{
    memset(this, 0, sizeof(IG_PARACTL_LUAPIN));
}

IG_PARACTL_LUAPIN::IG_PARACTL_LUAPIN(const char* name, uint32 pId, uint32 id,
                  uint32 data1, uint32 data2, uint32 getKey, uint32 setKey)
{
    memset(this, 0, sizeof(IG_PARACTL_LUAPIN));
    int len = strlen(name);
    if(len > 8) len = 8;
    memcpy(itemName, name, len);
    parentId = pId;
    pinId = id;
    exData1 = data1;
    exData2 = data2;
    getCmdKey = getKey;
    setCmdKey = setKey;
}


CLuaPinParam_Dev::CLuaPinParam_Dev(IG_PARACTL_LUAPIN pinData)
{
    memcpy(&_pinData, &pinData, sizeof(IG_PARACTL_LUAPIN));
}

CLuaPinParam_Dev::~CLuaPinParam_Dev()
{
    for(CLuaPinParam_Dev* param : _children)
    {
        delete param;
    }
}

QVariant CLuaPinParam_Dev::getValue()
{
    QVariant val;
    bool bVal = false;
    double dVal, dMaxVal, dMinVal;
    quint16 i16Val,i16MaxVal;
    quint8 i8Val = 0;
    bool rs = false;
    switch(_pinData.getCmdKey)
    {
    case 0xFFFFFFFF: //根节点跳过设置命令
        rs = true;
        break;
    //================输入模块================//
    case 0x00000001:
        rs = CenterCtrl::instance()->getInputAntiPhase(_pinData.itemName, _pinData.exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x00000003:
        rs = CenterCtrl::instance()->getInputMute(_pinData.itemName, _pinData.exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x00000005:
        rs = CenterCtrl::instance()->getInputGain(_pinData.itemName, _pinData.exData1, dVal);
        val.setValue(dVal);
        break;
    //================输出模块================//
    case 0x00010001:
        rs = CenterCtrl::instance()->getOutputAntiPhase(_pinData.itemName, _pinData.exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x00010003:
        rs = CenterCtrl::instance()->getOutputMute(_pinData.itemName, _pinData.exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x00010005:
        rs = CenterCtrl::instance()->getOutputGain(_pinData.itemName, _pinData.exData1, dVal);
        val.setValue(dVal);
        break;
    //================延时模块================//
    case 0x00020001:
        rs = CenterCtrl::instance()->getDelayBypass(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x00020003:
        rs = CenterCtrl::instance()->getDelayTime(_pinData.itemName, dVal, dMaxVal);
        val.setValue(dVal);
        break;
    //================路由模块================//
    case 0x00030001:
        rs = CenterCtrl::instance()->getRouterOutput(_pinData.itemName, _pinData.exData1, i8Val);
        val.setValue(i8Val);
        break;
    //================混音模块================//
    case 0x00040001:
        rs = CenterCtrl::instance()->getMixerInputState(_pinData.itemName, _pinData.exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x00040003:
        rs = CenterCtrl::instance()->getMixerInputGain(_pinData.itemName, _pinData.exData1, dVal);
        val.setValue(dVal);
        break;
    case 0x00040005:
        rs = CenterCtrl::instance()->getMixerOutputState(_pinData.itemName, _pinData.exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x00040007:
        rs = CenterCtrl::instance()->getMixerOutputGain(_pinData.itemName, _pinData.exData1, dVal);
        val.setValue(dVal);
        break;
    case 0x00040009:
        rs = CenterCtrl::instance()->getMixerNodeState(_pinData.itemName, _pinData.exData1, _pinData.exData2, bVal);
        val.setValue(bVal);
        break;
    case 0x0004000B:
        rs = CenterCtrl::instance()->getMixerNodeGain(_pinData.itemName, _pinData.exData1, _pinData.exData2, dVal);
        val.setValue(dVal);
        break;
    //================混音模块================//
    case 0x00050001:
        rs = CenterCtrl::instance()->getDynThreshold(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x00050003:
        rs = CenterCtrl::instance()->getDynRatio(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x00050005:
        rs = CenterCtrl::instance()->getDynAttackTime(_pinData.itemName, i16Val, i16MaxVal);
        val.setValue(i16Val);
        break;
    case 0x00050007:
        rs = CenterCtrl::instance()->getDynReleaseTime(_pinData.itemName, i16Val, i16MaxVal);
        val.setValue(i16Val);
        break;
    case 0x00050009:
        rs = CenterCtrl::instance()->getDynGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x0005000B:
        rs = CenterCtrl::instance()->getDynTargetLevel(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x0005000D:
        rs = CenterCtrl::instance()->getDynBypass(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    //================均衡模块================//
    case 0x00060001:
        rs = CenterCtrl::instance()->getEqFreq(_pinData.itemName, _pinData.exData1, i16Val);
        val.setValue(i16Val);
        break;
    case 0x00060003:
        rs = CenterCtrl::instance()->getEqGain(_pinData.itemName, _pinData.exData1, dVal, dMaxVal, dMinVal);
        val.setValue(dVal);
        break;
    case 0x00060005:
        rs = CenterCtrl::instance()->getEqQVal(_pinData.itemName, _pinData.exData1, dVal);
        val.setValue(dVal);
        break;
    case 0x00060007:
        rs = CenterCtrl::instance()->getEqBypass(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    //================信号发生器模块================//
    case 0x00070001:
        rs = CenterCtrl::instance()->getSigFreq(_pinData.itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x00070003:
        rs = CenterCtrl::instance()->getSigGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x00070005:
        rs = CenterCtrl::instance()->getSigMute(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    //================分频器模块================//
    case 0x00080001:
        rs = CenterCtrl::instance()->getSplitInMute(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x00080003:
        rs = CenterCtrl::instance()->getSplitInGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x00080005:
        rs = CenterCtrl::instance()->getSplitType(_pinData.itemName, _pinData.exData1, i8Val);
        val.setValue(i8Val);
        break;
    case 0x00080007:
        rs = CenterCtrl::instance()->getSplitFreq(_pinData.itemName, _pinData.exData1, i16Val);
        val.setValue(i16Val);
        break;
    case 0x00080009:
        rs = CenterCtrl::instance()->getSplitOutMute(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x0008000B:
        rs = CenterCtrl::instance()->getSplitOutGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x0008000D:
        rs = CenterCtrl::instance()->getSplitOutAntiPhase(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    //================滤波器模块================//
    case 0x00090001:
        rs = CenterCtrl::instance()->getFilterType(_pinData.itemName, i8Val);
        val.setValue(i8Val);
        break;
    case 0x00090003:
        rs = CenterCtrl::instance()->getFilterFreq(_pinData.itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x00090005:
        rs = CenterCtrl::instance()->getFilterGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x00090007:
        rs = CenterCtrl::instance()->getFilterBypass(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x00090009:
        rs = CenterCtrl::instance()->getFilterBandwidth(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    //================啸叫抑制模块================//
    case 0x000A0001:
        rs = CenterCtrl::instance()->getFreqshiftFreq(_pinData.itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000A0003:
        rs = CenterCtrl::instance()->getFreqshiftGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    //================自动混音模块================//
    case 0x000B0001:
        rs = CenterCtrl::instance()->getAutoMixerThreshold(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0003:
        rs = CenterCtrl::instance()->getAutoMixerAttackTime(_pinData.itemName, i16Val, i16MaxVal);
        val.setValue(i16Val);
        break;
    case 0x000B0005:
        rs = CenterCtrl::instance()->getAutoMixerReleaseTime(_pinData.itemName, i16Val, i16MaxVal);
        val.setValue(i16Val);
        break;
    case 0x000B0007:
        rs = CenterCtrl::instance()->getAutoMixerHoldTime(_pinData.itemName, i16Val, i16MaxVal);
        val.setValue(i16Val);
        break;
    case 0x000B000B:
        rs = CenterCtrl::instance()->getAutoMixerDepth(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B000F:
        rs = CenterCtrl::instance()->getAutoMixerNoiseGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0011:
        rs = CenterCtrl::instance()->getAutoMixerInputMute(_pinData.itemName, _pinData.exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x000B0013:
        rs = CenterCtrl::instance()->getAutoMixerInputGain(_pinData.itemName, _pinData.exData1, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0015:
        rs = CenterCtrl::instance()->getAutoMixerOutputMute(_pinData.itemName, _pinData.exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x000B0017:
        rs = CenterCtrl::instance()->getAutoMixerOutputGain(_pinData.itemName, _pinData.exData1, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0019:
        rs = CenterCtrl::instance()->getAutoMixerInputManual(_pinData.itemName, _pinData.exData1, bVal);
        val.setValue(bVal);
        break;
    case 0x000B001B:
        rs = CenterCtrl::instance()->getAutoMixerInputMode(_pinData.itemName, _pinData.exData1, i8Val);
        val.setValue(i8Val);
        break;
    case 0x000B001D:
        rs = CenterCtrl::instance()->getAutoMixerInputPriority(_pinData.itemName, _pinData.exData1, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000B001F:
        rs = CenterCtrl::instance()->getAutoMixerPercentage(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0023:
        rs = CenterCtrl::instance()->getAutoMixerLastMicOn(_pinData.itemName, i8Val);
        val.setValue(i8Val);
        break;
    case 0x000B0025:
        rs = CenterCtrl::instance()->getAutoMixerDefaultCh(_pinData.itemName, i8Val);
        val.setValue(i8Val);
        break;
    case 0x000B0027:
        rs = CenterCtrl::instance()->getAutoMixerThresholdLevelAboveNoise(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0029:
        rs = CenterCtrl::instance()->getAutoMixerMaxNOM(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x000B002B:
        rs = CenterCtrl::instance()->getAutoMixerDirectOutsNOMAtten(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x000B002D:
        rs = CenterCtrl::instance()->getAutoMixerNOMAttenuationType(_pinData.itemName, i8Val);
        val.setValue(i8Val);
        break;
    case 0x000B002F:
        rs = CenterCtrl::instance()->getAutoMixerNOMAttenuationStep(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0031:
        rs = CenterCtrl::instance()->getAutoMixerMaxNOMAttenuation(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000B0033:
        rs = CenterCtrl::instance()->getAutoMixerResponseTime(_pinData.itemName, i16Val);
        val.setValue(i16Val);
        break;
    //================回声消除模块================//
    case 0x000C0001:
        rs = CenterCtrl::instance()->getAECMicInGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000C0003:
        rs = CenterCtrl::instance()->getAECLineInGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000C0005:
        rs = CenterCtrl::instance()->getAECSpeakOutGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000C0007:
        rs = CenterCtrl::instance()->getAECLineOutGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000C0009:
        rs = CenterCtrl::instance()->getAECMicToSpeakGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000C000B:
        rs = CenterCtrl::instance()->getAECMicToSpeakGainOn(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    case 0x000C000D:
        rs = CenterCtrl::instance()->getAECNRGain(_pinData.itemName, i8Val);
        val.setValue(i8Val);
        break;
    case 0x000C000F:
        rs = CenterCtrl::instance()->getAECNRGainOn(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    //================闪避模块================//
    case 0x000D0001:
        rs = CenterCtrl::instance()->getDuckerAttackTime(_pinData.itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000D0003:
        rs = CenterCtrl::instance()->getDuckerReleaseTime(_pinData.itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000D0005:
        rs = CenterCtrl::instance()->getDuckerHoldTime(_pinData.itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000D0007:
        rs = CenterCtrl::instance()->getDuckerDetectTime(_pinData.itemName, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000D0009:
        rs = CenterCtrl::instance()->getDuckerThreshold(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000D000B:
        rs = CenterCtrl::instance()->getDuckerDepth(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000D000D:
        rs = CenterCtrl::instance()->getDuckerInGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000D000F:
        rs = CenterCtrl::instance()->getDuckerPriorityGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000D0011:
        rs = CenterCtrl::instance()->getDuckerOutGain(_pinData.itemName, dVal);
        val.setValue(dVal);
        break;
    case 0x000D0013:
        rs = CenterCtrl::instance()->getDuckerBypass(_pinData.itemName, bVal);
        val.setValue(bVal);
        break;
    //================淡入淡出模块================//
    case 0x000E0001:
        rs = CenterCtrl::instance()->getGainRampTime(_pinData.itemName, _pinData.exData1, i16Val);
        val.setValue(i16Val);
        break;
    case 0x000E0003:
        rs = CenterCtrl::instance()->getGainRampGain(_pinData.itemName, _pinData.exData1, dVal);
        val.setValue(dVal);
        break;
    case 0x000E0005:
        rs = CenterCtrl::instance()->getGainRampType(_pinData.itemName, i8Val);
        val.setValue(i8Val);
        break;
    //================播放器模块================//
//    case 0x000F0001:
//        rs = CenterCtrl::instance()->getPlayerState(_pinData.itemName, _pinData.exData1); //wj 触发消息有没有更好处理方法
//        newVal = val.toInt();
//        break;

    default:
        break;
    }

    return val;
}


void CLuaPinParam_Dev::setValue(QVariant val)
{
    bool rs = false;
    QVariant newVal;
    switch(_pinData.setCmdKey)
    {
    case 0xFFFFFFFF: //根节点跳过设置命令
        rs = true;
        newVal = val;
        break;
    //================输入模块================//
    case 0x00000002:
        rs = CenterCtrl::instance()->setInputAntiPhase(_pinData.itemName, _pinData.exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00000004:
        rs = CenterCtrl::instance()->setInputMute(_pinData.itemName, _pinData.exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00000006:
        rs = CenterCtrl::instance()->setInputGain(_pinData.itemName, _pinData.exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    //================输出模块================//
    case 0x00010002:
        rs = CenterCtrl::instance()->setOutputAntiPhase(_pinData.itemName, _pinData.exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00010004:
        rs = CenterCtrl::instance()->setOutputMute(_pinData.itemName, _pinData.exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00010006:
        rs = CenterCtrl::instance()->setOutputGain(_pinData.itemName, _pinData.exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    //================延时模块================//
    case 0x00020002:
        rs = CenterCtrl::instance()->setDelayBypass(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00020004:
        rs = CenterCtrl::instance()->setDelayTime(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    //================路由模块================//
    case 0x00030002:
        rs = CenterCtrl::instance()->setRouterOutput(_pinData.itemName, _pinData.exData1, val.toUInt());
        newVal = val.toUInt();
        break;
    //================混音模块================//
    case 0x00040002:
        rs = CenterCtrl::instance()->setMixerInputState(_pinData.itemName, _pinData.exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00040004:
        rs = CenterCtrl::instance()->setMixerInputGain(_pinData.itemName, _pinData.exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00040006:
        rs = CenterCtrl::instance()->setMixerOutputState(_pinData.itemName, _pinData.exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00040008:
        rs = CenterCtrl::instance()->setMixerOutputGain(_pinData.itemName, _pinData.exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x0004000A:
        rs = CenterCtrl::instance()->setMixerNodeState(_pinData.itemName, _pinData.exData1, _pinData.exData2, val.toBool());
        newVal = val.toBool();
        break;
    case 0x0004000C:
        rs = CenterCtrl::instance()->setMixerNodeGain(_pinData.itemName, _pinData.exData1, _pinData.exData2, val.toDouble());
        newVal = val.toDouble();
        break;
    //================混音模块================//
    case 0x00050002:
        rs = CenterCtrl::instance()->setDynThreshold(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00050004:
        rs = CenterCtrl::instance()->setDynRatio(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00050006:
        rs = CenterCtrl::instance()->setDynAttackTime(_pinData.itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x00050008:
        rs = CenterCtrl::instance()->setDynReleaseTime(_pinData.itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x0005000A:
        rs = CenterCtrl::instance()->setDynGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x0005000C:
        rs = CenterCtrl::instance()->setDynTargetLevel(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x0005000E:
        rs = CenterCtrl::instance()->setDynBypass(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    //================均衡模块================//
    case 0x00060002:
        rs = CenterCtrl::instance()->setEqFreq(_pinData.itemName, _pinData.exData1, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x00060004:
        rs = CenterCtrl::instance()->setEqGain(_pinData.itemName, _pinData.exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00060006:
        rs = CenterCtrl::instance()->setEqQVal(_pinData.itemName, _pinData.exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00060008:
        rs = CenterCtrl::instance()->setEqBypass(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    //================信号发生器模块================//
    case 0x00070002:
        rs = CenterCtrl::instance()->setSigFreq(_pinData.itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x00070004:
        rs = CenterCtrl::instance()->setSigGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00070006:
        rs = CenterCtrl::instance()->setSigMute(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    //================分频器模块================//
    case 0x00080002:
        rs = CenterCtrl::instance()->setSplitInMute(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x00080004:
        rs = CenterCtrl::instance()->setSplitInGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00080006:
        rs = CenterCtrl::instance()->setSplitType(_pinData.itemName, _pinData.exData1, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x00080008:
        rs = CenterCtrl::instance()->setSplitFreq(_pinData.itemName, _pinData.exData1, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x0008000A:
        rs = CenterCtrl::instance()->setSplitOutMute(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x0008000C:
        rs = CenterCtrl::instance()->setSplitOutGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x0008000E:
        rs = CenterCtrl::instance()->setSplitOutAntiPhase(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    //================滤波器模块================//
    case 0x00090002:
        rs = CenterCtrl::instance()->setFilterType(_pinData.itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x00090004:
        rs = CenterCtrl::instance()->setFilterFreq(_pinData.itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x00090006:
        rs = CenterCtrl::instance()->setFilterGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x00090008:
        rs = CenterCtrl::instance()->setFilterBypass(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x0009000A:
        rs = CenterCtrl::instance()->setFilterBandwidth(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    //================啸叫抑制模块================//
    case 0x000A0002:
        rs = CenterCtrl::instance()->setFreqshiftFreq(_pinData.itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x000A0004:
        rs = CenterCtrl::instance()->setFreqshiftGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    //================自动混音模块================//
    case 0x000B0002:
        rs = CenterCtrl::instance()->setAutoMixerThreshold(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0004:
        rs = CenterCtrl::instance()->setAutoMixerAttackTime(_pinData.itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x000B0006:
        rs = CenterCtrl::instance()->setAutoMixerReleaseTime(_pinData.itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x000B0008:
        rs = CenterCtrl::instance()->setAutoMixerHoldTime(_pinData.itemName, val.toUInt());
        newVal = val.toUInt();
        break;
    case 0x000B000C:
        rs = CenterCtrl::instance()->setAutoMixerDepth(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0010:
        rs = CenterCtrl::instance()->setAutoMixerNoiseGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0012:
        rs = CenterCtrl::instance()->setAutoMixerInputMute(_pinData.itemName, _pinData.exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x000B0014:
        rs = CenterCtrl::instance()->setAutoMixerInputGain(_pinData.itemName, _pinData.exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0016:
        rs = CenterCtrl::instance()->setAutoMixerOutputMute(_pinData.itemName, _pinData.exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x000B0018:
        rs = CenterCtrl::instance()->setAutoMixerOutputGain(_pinData.itemName, _pinData.exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B001A:
        rs = CenterCtrl::instance()->setAutoMixerInputManual(_pinData.itemName, _pinData.exData1, val.toBool());
        newVal = val.toBool();
        break;
    case 0x000B001C:
        rs = CenterCtrl::instance()->setAutoMixerInputMode(_pinData.itemName, _pinData.exData1, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000B001E:
        rs = CenterCtrl::instance()->setAutoMixerInputPriority(_pinData.itemName, _pinData.exData1, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000B0020:
        rs = CenterCtrl::instance()->setAutoMixerPercentage(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0024:
        rs = CenterCtrl::instance()->setAutoMixerLastMicOn(_pinData.itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000B0026:
        rs = CenterCtrl::instance()->setAutoMixerDefaultCh(_pinData.itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000B0028:
        rs = CenterCtrl::instance()->setAutoMixerThresholdLevelAboveNoise(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B002A:
        rs = CenterCtrl::instance()->setAutoMixerMaxNOM(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x000B002C:
        rs = CenterCtrl::instance()->setAutoMixerDirectOutsNOMAtten(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x000B002E:
        rs = CenterCtrl::instance()->setAutoMixerNOMAttenuationType(_pinData.itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000B0030:
        rs = CenterCtrl::instance()->setAutoMixerNOMAttenuationStep(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0032:
        rs = CenterCtrl::instance()->setAutoMixerMaxNOMAttenuation(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000B0034:
        rs = CenterCtrl::instance()->setAutoMixerResponseTime(_pinData.itemName, val.toInt());
        newVal = val.toInt();
        break;
    //================回声消除模块================//
    case 0x000C0002:
        rs = CenterCtrl::instance()->setAECMicInGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000C0004:
        rs = CenterCtrl::instance()->setAECLineInGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000C0006:
        rs = CenterCtrl::instance()->setAECSpeakOutGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000C0008:
        rs = CenterCtrl::instance()->setAECLineOutGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000C000A:
        rs = CenterCtrl::instance()->setAECMicToSpeakGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000C000C:
        rs = CenterCtrl::instance()->setAECMicToSpeakGainOn(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    case 0x000C000E:
        rs = CenterCtrl::instance()->setAECNRGain(_pinData.itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000C0010:
        rs = CenterCtrl::instance()->setAECNRGainOn(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    //================闪避模块================//
    case 0x000D0002:
        rs = CenterCtrl::instance()->setDuckerAttackTime(_pinData.itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000D0004:
        rs = CenterCtrl::instance()->setDuckerReleaseTime(_pinData.itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000D0006:
        rs = CenterCtrl::instance()->setDuckerHoldTime(_pinData.itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000D0008:
        rs = CenterCtrl::instance()->setDuckerDetectTime(_pinData.itemName, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000D000A:
        rs = CenterCtrl::instance()->setDuckerThreshold(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000D000C:
        rs = CenterCtrl::instance()->setDuckerDepth(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000D000E:
        rs = CenterCtrl::instance()->setDuckerInGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000D0010:
        rs = CenterCtrl::instance()->setDuckerPriorityGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000D0012:
        rs = CenterCtrl::instance()->setDuckerOutGain(_pinData.itemName, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000D0014:
        rs = CenterCtrl::instance()->setDuckerBypass(_pinData.itemName, val.toBool());
        newVal = val.toBool();
        break;
    //================淡入淡出模块================//
    case 0x000E0002:
        rs = CenterCtrl::instance()->setGainRampTime(_pinData.itemName, _pinData.exData1, val.toInt());
        newVal = val.toInt();
        break;
    case 0x000E0004:
        rs = CenterCtrl::instance()->setGainRampGain(_pinData.itemName, _pinData.exData1, val.toDouble());
        newVal = val.toDouble();
        break;
    case 0x000E0006:
        rs = CenterCtrl::instance()->setGainRampType(_pinData.itemName, val.toInt());
        newVal = val.toInt();
        break;
    //================播放器模块================//
    case 0x000F0002:
        rs = CenterCtrl::instance()->setPlayerState(_pinData.itemName, _pinData.exData1); //wj 触发消息有没有更好处理方法
        newVal = val.toInt();
        break;

    default:
        break;
    }

    //设置下一级变量
    if(rs == true)
    {
        QVariant newVal = val.toBool();
        for(CLuaPinParam_Dev* child : _children)
        {
            child->setValue(newVal);
        }
    }
    else if(rs == false)
    {
        ;//记录错误日志
    }
}


void CLuaPinParam_Dev::setChildren(QList<CLuaPinParam_Dev*> children)
{
    _children.clear();
    _children.append(children);
}


