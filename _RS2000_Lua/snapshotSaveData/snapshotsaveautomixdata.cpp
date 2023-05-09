#include "snapshotsaveautomixdata.h"

#include "configItem/configitemautomaticmixer.h"

void SnapshotSaveAutoMixData::saveAutoMixData(ConfigItem *cfgItem, int type)
{
    ConfigItemAutomaticMixer *autoMixItem = qgraphicsitem_cast<ConfigItemAutomaticMixer *>(cfgItem);

    _moduleNum.insert(autoMixItem->customName(), autoMixItem->getModuleNum());

    switch(autoMixItem->getCfgType()) {
    case CFG_TYPE_AUTOMIX_GS_4:
    {
        IG_PARACTL_AUTOMIXGS4 body;
        body.threadLevel = (autoMixItem->GetThreadLevel() + 100) * 100;
        body.attackT_ms  = autoMixItem->GetAttackTime();
        body.releaseT_ms = autoMixItem->GetReleaseTime();
        body.holdT_ms    = autoMixItem->GetHoldTime();
        body.detectT_ms  = autoMixItem->GetDetectorTime();
        body.depth       = (autoMixItem->GetDepthValue() + 100) * 100;
        body.noiseGain   = (autoMixItem->GetNoiseValue() + 100) * 100;
        body.mixOutMute  = autoMixItem->GetMixOutMute();
        body.mixOutGain  = (autoMixItem->GetMixOutGain() + 100) * 100;
        for(int i  = 0; i < 4; i++)
        {
            body.inCtrl[i].inMute = autoMixItem->GetInputMute(i);
            body.inCtrl[i].priorGain = (autoMixItem->GetPreInGain(i) + 100) *100;
        }
        _AutoMixGS4_DataList[type].insert(autoMixItem->customName(), body);
        break;
    }
    case CFG_TYPE_AUTOMIX_GS_8:
    {
        IG_PARACTL_AUTOMIXGS8 body;
        body.threadLevel = (autoMixItem->GetThreadLevel() + 100) * 100;
        body.attackT_ms  = autoMixItem->GetAttackTime();
        body.releaseT_ms = autoMixItem->GetReleaseTime();
        body.holdT_ms    = autoMixItem->GetHoldTime();
        body.detectT_ms  = autoMixItem->GetDetectorTime();
        body.depth       = (autoMixItem->GetDepthValue() + 100) * 100;
        body.noiseGain   = (autoMixItem->GetNoiseValue() + 100) * 100;
        body.mixOutMute  = autoMixItem->GetMixOutMute();
        body.mixOutGain  = (autoMixItem->GetMixOutGain() + 100) * 100;
        for(int i  = 0; i < 8; i++)
        {
            body.inCtrl[i].inMute = autoMixItem->GetInputMute(i);
            body.inCtrl[i].priorGain = (autoMixItem->GetPreInGain(i) + 100) *100;
        }
        _AutoMixGS8_DataList[type].insert(autoMixItem->customName(), body);
        break;
    }
    case CFG_TYPE_AUTOMIX_GS_16:
    {
        IG_PARACTL_AUTOMIXGS16 body;
        body.threadLevel = (autoMixItem->GetThreadLevel() + 100) * 100;
        body.attackT_ms  = autoMixItem->GetAttackTime();
        body.releaseT_ms = autoMixItem->GetReleaseTime();
        body.holdT_ms    = autoMixItem->GetHoldTime();
        body.detectT_ms  = autoMixItem->GetDetectorTime();
        body.depth       = (autoMixItem->GetDepthValue() + 100) * 100;
        body.noiseGain   = (autoMixItem->GetNoiseValue() + 100) * 100;
        body.mixOutMute  = autoMixItem->GetMixOutMute();
        body.mixOutGain  = (autoMixItem->GetMixOutGain() + 100) * 100;
        for(int i  = 0; i < 16; i++)
        {
            body.inCtrl[i].inMute = autoMixItem->GetInputMute(i);
            body.inCtrl[i].priorGain = (autoMixItem->GetPreInGain(i) + 100) *100;
        }
        _AutoMixGS16_DataList[type].insert(autoMixItem->customName(), body);
        break;
    }
    case CFG_TYPE_AUTOMIX_GS_32:
    {
        IG_PARACTL_AUTOMIXGS32 body;
        body.threadLevel = (autoMixItem->GetThreadLevel() + 100) * 100;
        body.attackT_ms  = autoMixItem->GetAttackTime();
        body.releaseT_ms = autoMixItem->GetReleaseTime();
        body.holdT_ms    = autoMixItem->GetHoldTime();
        body.detectT_ms  = autoMixItem->GetDetectorTime();
        body.depth       = (autoMixItem->GetDepthValue() + 100) * 100;
        body.noiseGain   = (autoMixItem->GetNoiseValue() + 100) * 100;
        body.mixOutMute  = autoMixItem->GetMixOutMute();
        body.mixOutGain  = (autoMixItem->GetMixOutGain() + 100) * 100;
        for(int i  = 0; i < 32; i++)
        {
            body.inCtrl[i].inMute = autoMixItem->GetInputMute(i);
            body.inCtrl[i].priorGain = (autoMixItem->GetPreInGain(i) + 100) *100;
        }
        _AutoMixGS32_DataList[type].insert(autoMixItem->customName(), body);
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_4:
    {
        IG_PARACTL_AUTOMIXNGRT4 body;
        for(int i = 0; i < 4; i++)
        {
            body.inChAttr[i].manual = autoMixItem->GetChManual(i);
            body.inChAttr[i].postGateMute = autoMixItem->GetChPostGateMute(i);
            body.inChAttr[i].postGateGain = (autoMixItem->GetChPostGateGain(i) + 100) * 100;
        }
        body.mixOutMute = autoMixItem->GetMixOutMute();
        body.mixOutGain = (autoMixItem->GetMixOutGain() + 100) * 100;
//        body.idGating   = autoMixItem->GetIDGating();
        body.noEffectPass = autoMixItem->GetLastMicOn();
        body.defaultch    = autoMixItem->GetDefaultCh();
//        qDebug() << __FUNCTION__ << __LINE__ << body.noEffectPass << body.defaultch;
        body.threAboveNoise = (autoMixItem->GetThresholdLevelAboveNoise() + 100) * 100;
        body.depth          = (autoMixItem->GetDepthValue() + 100) * 100;
        body.gateHoldT_ms   = autoMixItem->GetHoldTime();
        body.autoMaxPassChs = autoMixItem->GetMaxMON();
        body.directPassAttenEnable = autoMixItem->GetDirectOutsNOMAtten();
        body.attenuationType   = autoMixItem->GetNOMAttenuationType();
        body.attenResponseT_ms = autoMixItem->GetResponseTime();
        body.attenStep = (autoMixItem->GetNOMAttenuationStep() + 100) * 100;
        body.attenMax  = (autoMixItem->GetMaxNOMAttenuation() + 100) * 100;

        _AutoMixNGRT4_DataList[type].insert(autoMixItem->customName(), body);
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_8:
    {
        IG_PARACTL_AUTOMIXNGRT8 body;
        for(int i = 0; i < 8; i++)
        {
            body.inChAttr[i].manual = autoMixItem->GetChManual(i);
            body.inChAttr[i].postGateMute = autoMixItem->GetChPostGateMute(i);
            body.inChAttr[i].postGateGain = (autoMixItem->GetChPostGateGain(i) + 100) * 100;
        }
        body.mixOutMute = autoMixItem->GetMixOutMute();
        body.mixOutGain = (autoMixItem->GetMixOutGain() + 100) * 100;
//        body.idGating   = autoMixItem->GetIDGating();
        body.noEffectPass = autoMixItem->GetLastMicOn();
        body.defaultch    = autoMixItem->GetDefaultCh();
        body.threAboveNoise = (autoMixItem->GetThresholdLevelAboveNoise() + 100) * 100;
        body.depth          = (autoMixItem->GetDepthValue() + 100) * 100;
        body.gateHoldT_ms   = autoMixItem->GetHoldTime();
        body.autoMaxPassChs = autoMixItem->GetMaxMON();
        body.directPassAttenEnable = autoMixItem->GetDirectOutsNOMAtten();
        body.attenuationType   = autoMixItem->GetNOMAttenuationType();
        body.attenResponseT_ms = autoMixItem->GetResponseTime();
        body.attenStep = (autoMixItem->GetNOMAttenuationStep() + 100) * 100;
        body.attenMax  = (autoMixItem->GetMaxNOMAttenuation() + 100) * 100;

        _AutoMixNGRT8_DataList[type].insert(autoMixItem->customName(), body);
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_16:
    {
        IG_PARACTL_AUTOMIXNGRT16 body;
        for(int i = 0; i < 16; i++)
        {
            body.inChAttr[i].manual = autoMixItem->GetChManual(i);
            body.inChAttr[i].postGateMute = autoMixItem->GetChPostGateMute(i);
            body.inChAttr[i].postGateGain = (autoMixItem->GetChPostGateGain(i) + 100) * 100;
        }
        body.mixOutMute = autoMixItem->GetMixOutMute();
        body.mixOutGain = (autoMixItem->GetMixOutGain() + 100) * 100;
//        body.idGating   = autoMixItem->GetIDGating();
        body.noEffectPass = autoMixItem->GetLastMicOn();
        body.defaultch    = autoMixItem->GetDefaultCh();
        body.threAboveNoise = (autoMixItem->GetThresholdLevelAboveNoise() + 100) * 100;
        body.depth          = (autoMixItem->GetDepthValue() + 100) * 100;
        body.gateHoldT_ms   = autoMixItem->GetHoldTime();
        body.autoMaxPassChs = autoMixItem->GetMaxMON();
        body.directPassAttenEnable = autoMixItem->GetDirectOutsNOMAtten();
        body.attenuationType   = autoMixItem->GetNOMAttenuationType();
        body.attenResponseT_ms = autoMixItem->GetResponseTime();
        body.attenStep = (autoMixItem->GetNOMAttenuationStep() + 100) * 100;
        body.attenMax  = (autoMixItem->GetMaxNOMAttenuation() + 100) * 100;

        _AutoMixNGRT16_DataList[type].insert(autoMixItem->customName(), body);
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_32:
    {
        IG_PARACTL_AUTOMIXNGRT32 body;
        for(int i = 0; i < 32; i++)
        {
            body.inChAttr[i].manual = autoMixItem->GetChManual(i);
            body.inChAttr[i].postGateMute = autoMixItem->GetChPostGateMute(i);
            body.inChAttr[i].postGateGain = (autoMixItem->GetChPostGateGain(i) + 100) * 100;
        }
        body.mixOutMute = autoMixItem->GetMixOutMute();
        body.mixOutGain = (autoMixItem->GetMixOutGain() + 100) * 100;
//        body.idGating   = autoMixItem->GetIDGating();
        body.noEffectPass = autoMixItem->GetLastMicOn();
        body.defaultch    = autoMixItem->GetDefaultCh();
        body.threAboveNoise = (autoMixItem->GetThresholdLevelAboveNoise() + 100) * 100;
        body.depth          = (autoMixItem->GetDepthValue() + 100) * 100;
        body.gateHoldT_ms   = autoMixItem->GetHoldTime();
        body.autoMaxPassChs = autoMixItem->GetMaxMON();
        body.directPassAttenEnable = autoMixItem->GetDirectOutsNOMAtten();
        body.attenuationType   = autoMixItem->GetNOMAttenuationType();
        body.attenResponseT_ms = autoMixItem->GetResponseTime();
        body.attenStep = (autoMixItem->GetNOMAttenuationStep() + 100) * 100;
        body.attenMax  = (autoMixItem->GetMaxNOMAttenuation() + 100) * 100;

        _AutoMixNGRT32_DataList[type].insert(autoMixItem->customName(), body);
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_4:
    {
        IG_PARACTL_AUTOMIXNGAT4 body;
        for(int i = 0; i < 4; i++)
        {
            body.inChAttr[i].manual = autoMixItem->GetChManual(i);
            body.inChAttr[i].gateMode = autoMixItem->GetChMode(i);
            body.inChAttr[i].priority = autoMixItem->GetChPriority(i);
            body.inChAttr[i].threshold = (autoMixItem->GetChThreshold(i) + 100) * 100;
        }
        body.mixOutMute = autoMixItem->GetMixOutMute();
        body.mixOutGain = (autoMixItem->GetMixOutGain() + 100) * 100;
        body.lastMicOn  = autoMixItem->GetLastMicOn();
        body.idGating   = autoMixItem->GetIDGating();
        body.bgpercentage = autoMixItem->GetBackgroundPercentage() * 100;
        body.depth      = (autoMixItem->GetDepthValue() + 100) * 100;
        body.gateHoldT_ms = autoMixItem->GetHoldTime();
        body.maxPassChs = autoMixItem->GetMaxMON();
        body.attenuationType   = autoMixItem->GetNOMAttenuationType();
        body.attenResponseT_ms = autoMixItem->GetResponseTime();
        body.attenStep  = (autoMixItem->GetNOMAttenuationStep() + 100) * 100;
        body.attenMax   = (autoMixItem->GetMaxNOMAttenuation() + 100) * 100;

        _AutoMixNGAT4_DataList[type].insert(autoMixItem->customName(), body);
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_8:
    {
        IG_PARACTL_AUTOMIXNGAT8 body;
        for(int i = 0; i < 8; i++)
        {
            body.inChAttr[i].manual = autoMixItem->GetChManual(i);
            body.inChAttr[i].gateMode = autoMixItem->GetChMode(i);
            body.inChAttr[i].priority = autoMixItem->GetChPriority(i);
            body.inChAttr[i].threshold = (autoMixItem->GetChThreshold(i) + 100) * 100;
        }
        body.mixOutMute = autoMixItem->GetMixOutMute();
        body.mixOutGain = (autoMixItem->GetMixOutGain() + 100) * 100;
        body.lastMicOn  = autoMixItem->GetLastMicOn();
        body.idGating   = autoMixItem->GetIDGating();
        body.bgpercentage = autoMixItem->GetBackgroundPercentage() * 100;
        body.depth      = (autoMixItem->GetDepthValue() + 100) * 100;
        body.gateHoldT_ms = autoMixItem->GetHoldTime();
        body.maxPassChs = autoMixItem->GetMaxMON();
        body.attenuationType   = autoMixItem->GetNOMAttenuationType();
        body.attenResponseT_ms = autoMixItem->GetResponseTime();
        body.attenStep  = (autoMixItem->GetNOMAttenuationStep() + 100) * 100;
        body.attenMax   = (autoMixItem->GetMaxNOMAttenuation() + 100) * 100;

        _AutoMixNGAT8_DataList[type].insert(autoMixItem->customName(), body);
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_16:
    {
        IG_PARACTL_AUTOMIXNGAT16 body;
        for(int i = 0; i < 16; i++)
        {
            body.inChAttr[i].manual = autoMixItem->GetChManual(i);
            body.inChAttr[i].gateMode = autoMixItem->GetChMode(i);
            body.inChAttr[i].priority = autoMixItem->GetChPriority(i);
            body.inChAttr[i].threshold = (autoMixItem->GetChThreshold(i) + 100) * 100;
        }
        body.mixOutMute = autoMixItem->GetMixOutMute();
        body.mixOutGain = (autoMixItem->GetMixOutGain() + 100) * 100;
        body.lastMicOn  = autoMixItem->GetLastMicOn();
        body.idGating   = autoMixItem->GetIDGating();
        body.bgpercentage = autoMixItem->GetBackgroundPercentage() * 100;
        body.depth      = (autoMixItem->GetDepthValue() + 100) * 100;
        body.gateHoldT_ms = autoMixItem->GetHoldTime();
        body.maxPassChs = autoMixItem->GetMaxMON();
        body.attenuationType   = autoMixItem->GetNOMAttenuationType();
        body.attenResponseT_ms = autoMixItem->GetResponseTime();
        body.attenStep  = (autoMixItem->GetNOMAttenuationStep() + 100) * 100;
        body.attenMax   = (autoMixItem->GetMaxNOMAttenuation() + 100) * 100;

        _AutoMixNGAT16_DataList[type].insert(autoMixItem->customName(), body);
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_32:
    {
        IG_PARACTL_AUTOMIXNGAT32 body;
        for(int i = 0; i < 32; i++)
        {
            body.inChAttr[i].manual = autoMixItem->GetChManual(i);
            body.inChAttr[i].gateMode = autoMixItem->GetChMode(i);
            body.inChAttr[i].priority = autoMixItem->GetChPriority(i);
            body.inChAttr[i].threshold = (autoMixItem->GetChThreshold(i) + 100) * 100;
        }
        body.mixOutMute = autoMixItem->GetMixOutMute();
        body.mixOutGain = (autoMixItem->GetMixOutGain() + 100) * 100;
        body.lastMicOn  = autoMixItem->GetLastMicOn();
        body.idGating   = autoMixItem->GetIDGating();
        body.bgpercentage = autoMixItem->GetBackgroundPercentage() * 100;
        body.depth      = (autoMixItem->GetDepthValue() + 100) * 100;
        body.gateHoldT_ms = autoMixItem->GetHoldTime();
        body.maxPassChs = autoMixItem->GetMaxMON();
        body.attenuationType   = autoMixItem->GetNOMAttenuationType();
        body.attenResponseT_ms = autoMixItem->GetResponseTime();
        body.attenStep  = (autoMixItem->GetNOMAttenuationStep() + 100) * 100;
        body.attenMax   = (autoMixItem->GetMaxNOMAttenuation() + 100) * 100;

        _AutoMixNGAT32_DataList[type].insert(autoMixItem->customName(), body);
        break;
    }
    default:
        break;
    }
}

void SnapshotSaveAutoMixData::setAutoMixData(ConfigItem *cfgItem, int type)
{
    ConfigItemAutomaticMixer *autoMixItem = qgraphicsitem_cast<ConfigItemAutomaticMixer *>(cfgItem);

    quint32 moduleNum = _moduleNum.find(autoMixItem->customName()).value();
    autoMixItem->setModuleNum(moduleNum);

    switch(autoMixItem->getCfgType()) {
    case CFG_TYPE_AUTOMIX_GS_4:
    {
        if(!findCfgItemData<IG_PARACTL_AUTOMIXGS4>(_AutoMixGS4_DataList[type], cfgItem)) return;

        IG_PARACTL_AUTOMIXGS4 body = _AutoMixGS4_DataList[type].find(autoMixItem->customName()).value();
        autoMixItem->SetThreadLevel(body.threadLevel * 0.01 - 100);
        autoMixItem->SetAttackTime(body.attackT_ms);
        autoMixItem->SetReleaseTime(body.releaseT_ms);
        autoMixItem->SetHoldTime(body.holdT_ms);
        autoMixItem->SetDetectorTime(body.detectT_ms);
        autoMixItem->SetDepthValue(body.depth * 0.01 - 100);
        autoMixItem->SetNoiseValue(body.noiseGain * 0.01 - 100);
        autoMixItem->SetMixOutMute(body.mixOutMute);
        autoMixItem->SetMixOutGain(body.mixOutGain * 0.01 - 100);
        for(int i = 0; i < 4; i++)
        {
            autoMixItem->SetInputMute(i, body.inCtrl[i].inMute);
            autoMixItem->SetPreInGain(i, body.inCtrl[i].priorGain * 0.01 - 100);
        }

        autoMixItem->updateCfgSettingLoadedData();
        break;
    }
    case CFG_TYPE_AUTOMIX_GS_8:
    {
        if(!findCfgItemData<IG_PARACTL_AUTOMIXGS8>(_AutoMixGS8_DataList[type], cfgItem)) return;

        IG_PARACTL_AUTOMIXGS8 body = _AutoMixGS8_DataList[type].find(autoMixItem->customName()).value();
        autoMixItem->SetThreadLevel(body.threadLevel * 0.01 - 100);
        autoMixItem->SetAttackTime(body.attackT_ms);
        autoMixItem->SetReleaseTime(body.releaseT_ms);
        autoMixItem->SetHoldTime(body.holdT_ms);
        autoMixItem->SetDetectorTime(body.detectT_ms);
        autoMixItem->SetDepthValue(body.depth * 0.01 - 100);
        autoMixItem->SetNoiseValue(body.noiseGain * 0.01 - 100);
        autoMixItem->SetMixOutMute(body.mixOutMute);
        autoMixItem->SetMixOutGain(body.mixOutGain * 0.01 - 100);
        for(int i = 0; i < 8; i++)
        {
            autoMixItem->SetInputMute(i, body.inCtrl[i].inMute);
            autoMixItem->SetPreInGain(i, body.inCtrl[i].priorGain * 0.01 - 100);
        }

        autoMixItem->updateCfgSettingLoadedData();
        break;
    }
    case CFG_TYPE_AUTOMIX_GS_16:
    {
        if(!findCfgItemData<IG_PARACTL_AUTOMIXGS16>(_AutoMixGS16_DataList[type], cfgItem)) return;

        IG_PARACTL_AUTOMIXGS16 body = _AutoMixGS16_DataList[type].find(autoMixItem->customName()).value();
        autoMixItem->SetThreadLevel(body.threadLevel * 0.01 - 100);
        autoMixItem->SetAttackTime(body.attackT_ms);
        autoMixItem->SetReleaseTime(body.releaseT_ms);
        autoMixItem->SetHoldTime(body.holdT_ms);
        autoMixItem->SetDetectorTime(body.detectT_ms);
        autoMixItem->SetDepthValue(body.depth * 0.01 - 100);
        autoMixItem->SetNoiseValue(body.noiseGain * 0.01 - 100);
        autoMixItem->SetMixOutMute(body.mixOutMute);
        autoMixItem->SetMixOutGain(body.mixOutGain * 0.01 - 100);
        for(int i = 0; i < 16; i++)
        {
            autoMixItem->SetInputMute(i, body.inCtrl[i].inMute);
            autoMixItem->SetPreInGain(i, body.inCtrl[i].priorGain * 0.01 - 100);
        }

        autoMixItem->updateCfgSettingLoadedData();
        break;
    }
    case CFG_TYPE_AUTOMIX_GS_32:
    {
        if(!findCfgItemData<IG_PARACTL_AUTOMIXGS32>(_AutoMixGS32_DataList[type], cfgItem)) return;

        IG_PARACTL_AUTOMIXGS32 body = _AutoMixGS32_DataList[type].find(autoMixItem->customName()).value();
        autoMixItem->SetThreadLevel(body.threadLevel * 0.01 - 100);
        autoMixItem->SetAttackTime(body.attackT_ms);
        autoMixItem->SetReleaseTime(body.releaseT_ms);
        autoMixItem->SetHoldTime(body.holdT_ms);
        autoMixItem->SetDetectorTime(body.detectT_ms);
        autoMixItem->SetDepthValue(body.depth * 0.01 - 100);
        autoMixItem->SetNoiseValue(body.noiseGain * 0.01 - 100);
        autoMixItem->SetMixOutMute(body.mixOutMute);
        autoMixItem->SetMixOutGain(body.mixOutGain * 0.01 - 100);
        for(int i = 0; i < 32; i++)
        {
            autoMixItem->SetInputMute(i, body.inCtrl[i].inMute);
            autoMixItem->SetPreInGain(i, body.inCtrl[i].priorGain * 0.01 - 100);
        }

        autoMixItem->updateCfgSettingLoadedData();
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_4:
    {
        if(!findCfgItemData<IG_PARACTL_AUTOMIXNGRT4>(_AutoMixNGRT4_DataList[type], cfgItem)) return;

        IG_PARACTL_AUTOMIXNGRT4 body = _AutoMixNGRT4_DataList[type].find(autoMixItem->customName()).value();
        for(int i = 0; i < 4; i++)
        {
            autoMixItem->SetChManual(i, body.inChAttr[i].manual);
            autoMixItem->SetChPostGateMute(i, body.inChAttr[i].postGateMute);
            autoMixItem->SetChPostGateGain(i, body.inChAttr[i].postGateGain * 0.01 - 100);
        }
        autoMixItem->SetMixOutMute(body.mixOutMute);
        autoMixItem->SetMixOutGain(body.mixOutGain * 0.01 - 100);
//        autoMixItem->SetIDGating(body.idGating);
        autoMixItem->SetLastMicOn(body.noEffectPass);
        autoMixItem->SetDefaultCh(body.defaultch);
//        qDebug() << __FUNCTION__ << __LINE__ << body.noEffectPass << body.defaultch;
        autoMixItem->SetThresholdLevelAboveNoise(body.threAboveNoise * 0.01 - 100);
        autoMixItem->SetDepthValue(body.depth * 0.01 - 100);
        autoMixItem->SetHoldTime(body.gateHoldT_ms);
        autoMixItem->SetMaxMON(body.autoMaxPassChs);
        autoMixItem->SetDirectOutsNOMAtten(body.directPassAttenEnable);
        autoMixItem->SetNOMAttenuationType(body.attenuationType);
        autoMixItem->SetResponseTime(body.attenResponseT_ms);
        autoMixItem->SetNOMAttenuationStep(body.attenStep * 0.01 - 100);
        autoMixItem->SetMaxNOMAttenuation(body.attenMax * 0.01 - 100);

        autoMixItem->updateCfgSettingLoadedData();
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_8:
    {
        if(!findCfgItemData<IG_PARACTL_AUTOMIXNGRT8>(_AutoMixNGRT8_DataList[type], cfgItem)) return;

        IG_PARACTL_AUTOMIXNGRT8 body = _AutoMixNGRT8_DataList[type].find(autoMixItem->customName()).value();
        for(int i = 0; i < 8; i++)
        {
            autoMixItem->SetChManual(i, body.inChAttr[i].manual);
            autoMixItem->SetChPostGateMute(i, body.inChAttr[i].postGateMute);
            autoMixItem->SetChPostGateGain(i, body.inChAttr[i].postGateGain * 0.01 - 100);
        }
        autoMixItem->SetMixOutMute(body.mixOutMute);
        autoMixItem->SetMixOutGain(body.mixOutGain * 0.01 - 100);
//        autoMixItem->SetIDGating(body.idGating);
        autoMixItem->SetLastMicOn(body.noEffectPass);
        autoMixItem->SetDefaultCh(body.defaultch);
        autoMixItem->SetThresholdLevelAboveNoise(body.threAboveNoise * 0.01 - 100);
        autoMixItem->SetDepthValue(body.depth * 0.01 - 100);
        autoMixItem->SetHoldTime(body.gateHoldT_ms);
        autoMixItem->SetMaxMON(body.autoMaxPassChs);
        autoMixItem->SetDirectOutsNOMAtten(body.directPassAttenEnable);
        autoMixItem->SetNOMAttenuationType(body.attenuationType);
        autoMixItem->SetResponseTime(body.attenResponseT_ms);
        autoMixItem->SetNOMAttenuationStep(body.attenStep * 0.01 - 100);
        autoMixItem->SetMaxNOMAttenuation(body.attenMax * 0.01 - 100);

        autoMixItem->updateCfgSettingLoadedData();
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_16:
    {
        if(!findCfgItemData<IG_PARACTL_AUTOMIXNGRT16>(_AutoMixNGRT16_DataList[type], cfgItem)) return;

        IG_PARACTL_AUTOMIXNGRT16 body = _AutoMixNGRT16_DataList[type].find(autoMixItem->customName()).value();
        for(int i = 0; i < 16; i++)
        {
            autoMixItem->SetChManual(i, body.inChAttr[i].manual);
            autoMixItem->SetChPostGateMute(i, body.inChAttr[i].postGateMute);
            autoMixItem->SetChPostGateGain(i, body.inChAttr[i].postGateGain * 0.01 - 100);
        }
        autoMixItem->SetMixOutMute(body.mixOutMute);
        autoMixItem->SetMixOutGain(body.mixOutGain * 0.01 - 100);
//        autoMixItem->SetIDGating(body.idGating);
        autoMixItem->SetLastMicOn(body.noEffectPass);
        autoMixItem->SetDefaultCh(body.defaultch);
        autoMixItem->SetThresholdLevelAboveNoise(body.threAboveNoise * 0.01 - 100);
        autoMixItem->SetDepthValue(body.depth * 0.01 - 100);
        autoMixItem->SetHoldTime(body.gateHoldT_ms);
        autoMixItem->SetMaxMON(body.autoMaxPassChs);
        autoMixItem->SetDirectOutsNOMAtten(body.directPassAttenEnable);
        autoMixItem->SetNOMAttenuationType(body.attenuationType);
        autoMixItem->SetResponseTime(body.attenResponseT_ms);
        autoMixItem->SetNOMAttenuationStep(body.attenStep * 0.01 - 100);
        autoMixItem->SetMaxNOMAttenuation(body.attenMax * 0.01 - 100);

        autoMixItem->updateCfgSettingLoadedData();
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_32:
    {
        if(!findCfgItemData<IG_PARACTL_AUTOMIXNGRT32>(_AutoMixNGRT32_DataList[type], cfgItem)) return;

        IG_PARACTL_AUTOMIXNGRT32 body = _AutoMixNGRT32_DataList[type].find(autoMixItem->customName()).value();
        for(int i = 0; i < 32; i++)
        {
            autoMixItem->SetChManual(i, body.inChAttr[i].manual);
            autoMixItem->SetChPostGateMute(i, body.inChAttr[i].postGateMute);
            autoMixItem->SetChPostGateGain(i, body.inChAttr[i].postGateGain * 0.01 - 100);
        }
        autoMixItem->SetMixOutMute(body.mixOutMute);
        autoMixItem->SetMixOutGain(body.mixOutGain * 0.01 - 100);
//        autoMixItem->SetIDGating(body.idGating);
        autoMixItem->SetLastMicOn(body.noEffectPass);
        autoMixItem->SetDefaultCh(body.defaultch);
        autoMixItem->SetThresholdLevelAboveNoise(body.threAboveNoise * 0.01 - 100);
        autoMixItem->SetDepthValue(body.depth * 0.01 - 100);
        autoMixItem->SetHoldTime(body.gateHoldT_ms);
        autoMixItem->SetMaxMON(body.autoMaxPassChs);
        autoMixItem->SetDirectOutsNOMAtten(body.directPassAttenEnable);
        autoMixItem->SetNOMAttenuationType(body.attenuationType);
        autoMixItem->SetResponseTime(body.attenResponseT_ms);
        autoMixItem->SetNOMAttenuationStep(body.attenStep * 0.01 - 100);
        autoMixItem->SetMaxNOMAttenuation(body.attenMax * 0.01 - 100);

        autoMixItem->updateCfgSettingLoadedData();
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_4:
    {
        if(!findCfgItemData<IG_PARACTL_AUTOMIXNGAT4>(_AutoMixNGAT4_DataList[type], cfgItem)) return;

        IG_PARACTL_AUTOMIXNGAT4 body = _AutoMixNGAT4_DataList[type].find(autoMixItem->customName()).value();
        for(int i = 0; i < 4; i++)
        {
            autoMixItem->SetChManual(i, body.inChAttr[i].manual);
            autoMixItem->SetChMode(i, body.inChAttr[i].gateMode);
            autoMixItem->SetChPriority(i, body.inChAttr[i].priority);
            autoMixItem->SetChThreshold(i, body.inChAttr[i].threshold * 0.01 - 100);
        }
        autoMixItem->SetMixOutMute(body.mixOutMute);
        autoMixItem->SetMixOutGain(body.mixOutGain * 0.01 - 100);
        autoMixItem->SetLastMicOn(body.lastMicOn);
        autoMixItem->SetIDGating(body.idGating);
        autoMixItem->SetBackgroundPercentage(body.bgpercentage * 0.01);
        autoMixItem->SetDepthValue(body.depth * 0.01 - 100);
        autoMixItem->SetHoldTime(body.gateHoldT_ms);
        autoMixItem->SetMaxMON(body.maxPassChs);
        autoMixItem->SetNOMAttenuationType(body.attenuationType);
        autoMixItem->SetResponseTime(body.attenResponseT_ms);
        autoMixItem->SetNOMAttenuationStep(body.attenStep * 0.01 - 100);
        autoMixItem->SetMaxNOMAttenuation(body.attenMax * 0.01 - 100);

        autoMixItem->updateCfgSettingLoadedData();
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_8:
    {
        if(!findCfgItemData<IG_PARACTL_AUTOMIXNGAT8>(_AutoMixNGAT8_DataList[type], cfgItem)) return;

        IG_PARACTL_AUTOMIXNGAT8 body = _AutoMixNGAT8_DataList[type].find(autoMixItem->customName()).value();
        for(int i = 0; i < 8; i++)
        {
            autoMixItem->SetChManual(i, body.inChAttr[i].manual);
            autoMixItem->SetChMode(i, body.inChAttr[i].gateMode);
            autoMixItem->SetChPriority(i, body.inChAttr[i].priority);
            autoMixItem->SetChThreshold(i, body.inChAttr[i].threshold * 0.01 - 100);
        }
        autoMixItem->SetMixOutMute(body.mixOutMute);
        autoMixItem->SetMixOutGain(body.mixOutGain * 0.01 - 100);
        autoMixItem->SetLastMicOn(body.lastMicOn);
        autoMixItem->SetIDGating(body.idGating);
        autoMixItem->SetBackgroundPercentage(body.bgpercentage * 0.01);
        autoMixItem->SetDepthValue(body.depth * 0.01 - 100);
        autoMixItem->SetHoldTime(body.gateHoldT_ms);
        autoMixItem->SetMaxMON(body.maxPassChs);
        autoMixItem->SetNOMAttenuationType(body.attenuationType);
        autoMixItem->SetResponseTime(body.attenResponseT_ms);
        autoMixItem->SetNOMAttenuationStep(body.attenStep * 0.01 - 100);
        autoMixItem->SetMaxNOMAttenuation(body.attenMax * 0.01 - 100);

        autoMixItem->updateCfgSettingLoadedData();
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_16:
    {
        if(!findCfgItemData<IG_PARACTL_AUTOMIXNGAT16>(_AutoMixNGAT16_DataList[type], cfgItem)) return;

        IG_PARACTL_AUTOMIXNGAT16 body = _AutoMixNGAT16_DataList[type].find(autoMixItem->customName()).value();
        for(int i = 0; i < 16; i++)
        {
            autoMixItem->SetChManual(i, body.inChAttr[i].manual);
            autoMixItem->SetChMode(i, body.inChAttr[i].gateMode);
            autoMixItem->SetChPriority(i, body.inChAttr[i].priority);
            autoMixItem->SetChThreshold(i, body.inChAttr[i].threshold * 0.01 - 100);
        }
        autoMixItem->SetMixOutMute(body.mixOutMute);
        autoMixItem->SetMixOutGain(body.mixOutGain * 0.01 - 100);
        autoMixItem->SetLastMicOn(body.lastMicOn);
        autoMixItem->SetIDGating(body.idGating);
        autoMixItem->SetBackgroundPercentage(body.bgpercentage * 0.01);
        autoMixItem->SetDepthValue(body.depth * 0.01 - 100);
        autoMixItem->SetHoldTime(body.gateHoldT_ms);
        autoMixItem->SetMaxMON(body.maxPassChs);
        autoMixItem->SetNOMAttenuationType(body.attenuationType);
        autoMixItem->SetResponseTime(body.attenResponseT_ms);
        autoMixItem->SetNOMAttenuationStep(body.attenStep * 0.01 - 100);
        autoMixItem->SetMaxNOMAttenuation(body.attenMax * 0.01 - 100);

        autoMixItem->updateCfgSettingLoadedData();
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_32:
    {
        if(!findCfgItemData<IG_PARACTL_AUTOMIXNGAT32>(_AutoMixNGAT32_DataList[type], cfgItem)) return;

        IG_PARACTL_AUTOMIXNGAT32 body = _AutoMixNGAT32_DataList[type].find(autoMixItem->customName()).value();
        for(int i = 0; i < 32; i++)
        {
            autoMixItem->SetChManual(i, body.inChAttr[i].manual);
            autoMixItem->SetChMode(i, body.inChAttr[i].gateMode);
            autoMixItem->SetChPriority(i, body.inChAttr[i].priority);
            autoMixItem->SetChThreshold(i, body.inChAttr[i].threshold * 0.01 - 100);
        }
        autoMixItem->SetMixOutMute(body.mixOutMute);
        autoMixItem->SetMixOutGain(body.mixOutGain * 0.01 - 100);
        autoMixItem->SetLastMicOn(body.lastMicOn);
        autoMixItem->SetIDGating(body.idGating);
        autoMixItem->SetBackgroundPercentage(body.bgpercentage * 0.01);
        autoMixItem->SetDepthValue(body.depth * 0.01 - 100);
        autoMixItem->SetHoldTime(body.gateHoldT_ms);
        autoMixItem->SetMaxMON(body.maxPassChs);
        autoMixItem->SetNOMAttenuationType(body.attenuationType);
        autoMixItem->SetResponseTime(body.attenResponseT_ms);
        autoMixItem->SetNOMAttenuationStep(body.attenStep * 0.01 - 100);
        autoMixItem->SetMaxNOMAttenuation(body.attenMax * 0.01 - 100);

        autoMixItem->updateCfgSettingLoadedData();
        break;
    }
    default:
        break;
    }
}

void SnapshotSaveAutoMixData::saveToSetting(QSettings *ConfigIni, int pattern, CfgType type)
{
    switch(type) {
    case CFG_TYPE_AUTOMIX_GS_4:
    {
        setDataToSettings_GS<QHash<QString, IG_PARACTL_AUTOMIXGS4>>(_AutoMixGS4_DataList[pattern], ConfigIni, type);
        break;
    }
    case CFG_TYPE_AUTOMIX_GS_8:
    {
        setDataToSettings_GS<QHash<QString, IG_PARACTL_AUTOMIXGS8>>(_AutoMixGS8_DataList[pattern], ConfigIni, type);
        break;
    }
    case CFG_TYPE_AUTOMIX_GS_16:
    {
        setDataToSettings_GS<QHash<QString, IG_PARACTL_AUTOMIXGS16>>(_AutoMixGS16_DataList[pattern], ConfigIni, type);
        break;
    }
    case CFG_TYPE_AUTOMIX_GS_32:
    {
        setDataToSettings_GS<QHash<QString, IG_PARACTL_AUTOMIXGS32>>(_AutoMixGS32_DataList[pattern], ConfigIni, type);
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_4:
    {
        setDataToSettings_NGAT<QHash<QString, IG_PARACTL_AUTOMIXNGAT4>>(_AutoMixNGAT4_DataList[pattern], ConfigIni, type);
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_8:
    {
        setDataToSettings_NGAT<QHash<QString, IG_PARACTL_AUTOMIXNGAT8>>(_AutoMixNGAT8_DataList[pattern], ConfigIni, type);
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_16:
    {
        setDataToSettings_NGAT<QHash<QString, IG_PARACTL_AUTOMIXNGAT16>>(_AutoMixNGAT16_DataList[pattern], ConfigIni, type);
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_32:
    {
        setDataToSettings_NGAT<QHash<QString, IG_PARACTL_AUTOMIXNGAT32>>(_AutoMixNGAT32_DataList[pattern], ConfigIni, type);
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_4:
    {
        setDataToSettings_NGRT<QHash<QString, IG_PARACTL_AUTOMIXNGRT4>>(_AutoMixNGRT4_DataList[pattern], ConfigIni, type);
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_8:
    {
        setDataToSettings_NGRT<QHash<QString, IG_PARACTL_AUTOMIXNGRT8>>(_AutoMixNGRT8_DataList[pattern], ConfigIni, type);
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_16:
    {
        setDataToSettings_NGRT<QHash<QString, IG_PARACTL_AUTOMIXNGRT16>>(_AutoMixNGRT16_DataList[pattern], ConfigIni, type);
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_32:
    {
        setDataToSettings_NGRT<QHash<QString, IG_PARACTL_AUTOMIXNGRT32>>(_AutoMixNGRT32_DataList[pattern], ConfigIni, type);
        break;
    }
    default:
        break;
    }
}

void SnapshotSaveAutoMixData::SettingToSnap(QSettings *ConfigIni, int pattern, CfgType type)
{
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE).arg(type));
    int unimoduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM)).toInt();

    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX).arg(idx));
        QString devName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME)).toString();
        quint32 moduleNum = ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt();

        switch (type) {
        case CFG_TYPE_AUTOMIX_GS_4:
        {
            IG_PARACTL_AUTOMIXGS4 body;

            body = getDataFromSettings_GS< IG_PARACTL_AUTOMIXGS4>(body, ConfigIni, type);
            setAutoMix_T_Data<IG_PARACTL_AUTOMIXGS4>
                    (devName, type, pattern, moduleNum, body);
            break;
        }
        case CFG_TYPE_AUTOMIX_GS_8:
        {
            IG_PARACTL_AUTOMIXGS8 body;

            body = getDataFromSettings_GS< IG_PARACTL_AUTOMIXGS8>(body, ConfigIni, type);
            setAutoMix_T_Data<IG_PARACTL_AUTOMIXGS8>
                    (devName, type, pattern, moduleNum, body);
            break;
        }
        case CFG_TYPE_AUTOMIX_GS_16:
        {
            IG_PARACTL_AUTOMIXGS16 body;

            body = getDataFromSettings_GS<IG_PARACTL_AUTOMIXGS16>(body, ConfigIni, type);
            setAutoMix_T_Data<IG_PARACTL_AUTOMIXGS16>
                    (devName, type, pattern, moduleNum, body);
            break;
        }
        case CFG_TYPE_AUTOMIX_GS_32:
        {
            IG_PARACTL_AUTOMIXGS32 body;

            body = getDataFromSettings_GS<IG_PARACTL_AUTOMIXGS32>(body, ConfigIni, type);
            setAutoMix_T_Data<IG_PARACTL_AUTOMIXGS32>
                    (devName, type, pattern, moduleNum, body);
            break;
        }
        case CFG_TYPE_AUTOMIXNGAT_4:
        {
            IG_PARACTL_AUTOMIXNGAT4 body;
            body = getDataFromSettings_NGAT<IG_PARACTL_AUTOMIXNGAT4>(body, ConfigIni, type);
            setAutoMix_T_Data<IG_PARACTL_AUTOMIXNGAT4>
                    (devName, type, pattern, moduleNum, body);
            break;
        }
        case CFG_TYPE_AUTOMIXNGAT_8:
        {
            IG_PARACTL_AUTOMIXNGAT8 body;
            body = getDataFromSettings_NGAT<IG_PARACTL_AUTOMIXNGAT8>(body, ConfigIni, type);
            setAutoMix_T_Data<IG_PARACTL_AUTOMIXNGAT8>
                    (devName, type, pattern, moduleNum, body);
            break;
        }
        case CFG_TYPE_AUTOMIXNGAT_16:
        {
            IG_PARACTL_AUTOMIXNGAT16 body;
            body = getDataFromSettings_NGAT<IG_PARACTL_AUTOMIXNGAT16>(body, ConfigIni, type);
            setAutoMix_T_Data<IG_PARACTL_AUTOMIXNGAT16>
                    (devName, type, pattern, moduleNum, body);
            break;
        }
        case CFG_TYPE_AUTOMIXNGAT_32:
        {
            IG_PARACTL_AUTOMIXNGAT32 body;
            body = getDataFromSettings_NGAT<IG_PARACTL_AUTOMIXNGAT32>(body, ConfigIni, type);
            setAutoMix_T_Data<IG_PARACTL_AUTOMIXNGAT32>
                    (devName, type, pattern, moduleNum, body);
            break;
        }
        case CFG_TYPE_AUTOMIXNGRT_4:
        {
            IG_PARACTL_AUTOMIXNGRT4 body;
            body = getDataFromSettings_NGRT<IG_PARACTL_AUTOMIXNGRT4>(body, ConfigIni, type);
            setAutoMix_T_Data<IG_PARACTL_AUTOMIXNGRT4>
                    (devName, type, pattern, moduleNum, body);
            break;
        }
        case CFG_TYPE_AUTOMIXNGRT_8:
        {
            IG_PARACTL_AUTOMIXNGRT8 body;
            body = getDataFromSettings_NGRT<IG_PARACTL_AUTOMIXNGRT8>(body, ConfigIni, type);
            setAutoMix_T_Data<IG_PARACTL_AUTOMIXNGRT8>
                    (devName, type, pattern, moduleNum, body);
            break;
        }
        case CFG_TYPE_AUTOMIXNGRT_16:
        {
            IG_PARACTL_AUTOMIXNGRT16 body;
            body = getDataFromSettings_NGRT<IG_PARACTL_AUTOMIXNGRT16>(body, ConfigIni, type);
            setAutoMix_T_Data<IG_PARACTL_AUTOMIXNGRT16>
                    (devName, type, pattern, moduleNum, body);
            break;
        }
        case CFG_TYPE_AUTOMIXNGRT_32:
        {
            IG_PARACTL_AUTOMIXNGRT32 body;
            body = getDataFromSettings_NGRT<IG_PARACTL_AUTOMIXNGRT32>(body, ConfigIni, type);
            setAutoMix_T_Data<IG_PARACTL_AUTOMIXNGRT32>
                    (devName, type, pattern, moduleNum, body);
            break;
        }
        default:
            break;
        }
      ConfigIni->endGroup();
    }

    ConfigIni->endGroup();
}

int SnapshotSaveAutoMixData::getInCtrlNum(CfgType type)
{
    switch (type) {
    case CFG_TYPE_AUTOMIX_GS_4:    return 4;
    case CFG_TYPE_AUTOMIX_GS_8:    return 8;
    case CFG_TYPE_AUTOMIX_GS_16:   return 16;
    case CFG_TYPE_AUTOMIX_GS_32:   return 32;
    case CFG_TYPE_AUTOMIXNGAT_4:  return 4;
    case CFG_TYPE_AUTOMIXNGAT_8:  return 8;
    case CFG_TYPE_AUTOMIXNGAT_16: return 16;
    case CFG_TYPE_AUTOMIXNGAT_32: return 32;
    case CFG_TYPE_AUTOMIXNGRT_4:  return 4;
    case CFG_TYPE_AUTOMIXNGRT_8:  return 8;
    case CFG_TYPE_AUTOMIXNGRT_16: return 16;
    case CFG_TYPE_AUTOMIXNGRT_32: return 32;
    default:                       return 0;
    }
}

template<class t_body>
void SnapshotSaveAutoMixData::setAutoMix_T_Data(QString devName, CfgType type, int pattern, quint32 moduleNum, const t_body body)
{
    switch (type) {
    case CFG_TYPE_AUTOMIX_GS_4:
    {
        IG_PARACTL_AUTOMIXGS4 _body;
        memcpy(&_body, &body, sizeof (IG_PARACTL_AUTOMIXGS4));
        _AutoMixGS4_DataList[pattern].insert(devName, _body);
        _moduleNum.insert(devName, moduleNum);
        break;
    }
    case CFG_TYPE_AUTOMIX_GS_8:
    {
        IG_PARACTL_AUTOMIXGS8 _body;
        memcpy(&_body, &body, sizeof (IG_PARACTL_AUTOMIXGS8));
        _AutoMixGS8_DataList[pattern].insert(devName, _body);
        _moduleNum.insert(devName, moduleNum);
        break;
    }
    case CFG_TYPE_AUTOMIX_GS_16:
    {
        IG_PARACTL_AUTOMIXGS16 _body;
        memcpy(&_body, &body, sizeof (IG_PARACTL_AUTOMIXGS16));
        _AutoMixGS16_DataList[pattern].insert(devName, _body);
        _moduleNum.insert(devName, moduleNum);
        break;
    }
    case CFG_TYPE_AUTOMIX_GS_32:
    {
        IG_PARACTL_AUTOMIXGS32 _body;
        memcpy(&_body, &body, sizeof (IG_PARACTL_AUTOMIXGS32));
        _AutoMixGS32_DataList[pattern].insert(devName, _body);
        _moduleNum.insert(devName, moduleNum);
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_4:
    {
        IG_PARACTL_AUTOMIXNGAT4 _body;
        memcpy(&_body, &body, sizeof(IG_PARACTL_AUTOMIXNGAT4));
        _AutoMixNGAT4_DataList[pattern].insert(devName, _body);
        _moduleNum.insert(devName, moduleNum);
    }
    case CFG_TYPE_AUTOMIXNGAT_8:
    {
        IG_PARACTL_AUTOMIXNGAT8 _body;
        memcpy(&_body, &body, sizeof(IG_PARACTL_AUTOMIXNGAT8));
        _AutoMixNGAT8_DataList[pattern].insert(devName, _body);
        _moduleNum.insert(devName, moduleNum);
    }
    case CFG_TYPE_AUTOMIXNGAT_16:
    {
        IG_PARACTL_AUTOMIXNGAT16 _body;
        memcpy(&_body, &body, sizeof(IG_PARACTL_AUTOMIXNGAT16));
        _AutoMixNGAT16_DataList[pattern].insert(devName, _body);
        _moduleNum.insert(devName, moduleNum);
    }
    case CFG_TYPE_AUTOMIXNGAT_32:
    {
        IG_PARACTL_AUTOMIXNGAT32 _body;
        memcpy(&_body, &body, sizeof(IG_PARACTL_AUTOMIXNGAT32));
        _AutoMixNGAT32_DataList[pattern].insert(devName, _body);
        _moduleNum.insert(devName, moduleNum);
    }
    case CFG_TYPE_AUTOMIXNGRT_4:
    {
        IG_PARACTL_AUTOMIXNGRT4 _body;
        memcpy(&_body, &body, sizeof(IG_PARACTL_AUTOMIXNGRT4));
        _AutoMixNGRT4_DataList[pattern].insert(devName, _body);
        _moduleNum.insert(devName, moduleNum);
    }
    case CFG_TYPE_AUTOMIXNGRT_8:
    {
        IG_PARACTL_AUTOMIXNGRT8 _body;
        memcpy(&_body, &body, sizeof(IG_PARACTL_AUTOMIXNGRT8));
        _AutoMixNGRT8_DataList[pattern].insert(devName, _body);
        _moduleNum.insert(devName, moduleNum);
    }
    case CFG_TYPE_AUTOMIXNGRT_16:
    {
        IG_PARACTL_AUTOMIXNGRT16 _body;
        memcpy(&_body, &body, sizeof(IG_PARACTL_AUTOMIXNGRT16));
        _AutoMixNGRT16_DataList[pattern].insert(devName, _body);
        _moduleNum.insert(devName, moduleNum);
    }
    case CFG_TYPE_AUTOMIXNGRT_32:
    {
        IG_PARACTL_AUTOMIXNGRT32 _body;
        memcpy(&_body, &body, sizeof(IG_PARACTL_AUTOMIXNGRT32));
        _AutoMixNGRT32_DataList[pattern].insert(devName, _body);
        _moduleNum.insert(devName, moduleNum);
    }
    default:
        break;
    }

}


template<typename t_Data>
t_Data SnapshotSaveAutoMixData::getDataFromSettings_GS(t_Data body, QSettings *ConfigIni, CfgType type)
{
    body.threadLevel = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_THREADLEVEL)).toInt() + 100) * 100;
    body.attackT_ms  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTACKTIME)).toUInt();
    body.releaseT_ms = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_RELEASETIME)).toUInt();
    body.holdT_ms    = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME)).toUInt();
    body.detectT_ms  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_DETECTORTIME)).toInt();
    body.depth       = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE)).toInt() + 100) * 100;
    body.noiseGain   = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_NOISEVALUE)).toInt() + 100) * 100;
    body.mixOutMute  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE)).toUInt();
    body.mixOutGain  = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN)).toInt() + 100) * 100;

    for(int ch = 0; ch < getInCtrlNum(type); ch++)
    {
        body.inCtrl[ch].inMute = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INPUTMUTE).arg(ch)).toUInt();
        body.inCtrl[ch].priorGain = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_PREINGAIN).arg(ch)).toInt() + 100) * 100;
    }

    return body;
}

template<typename t_Data>
t_Data SnapshotSaveAutoMixData::getDataFromSettings_NGAT(t_Data body, QSettings *ConfigIni, CfgType type)
{
//    IG_PARACTL_AUTOMIXNGAT4 body;
    body.mixOutMute   = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE)).toUInt();
    body.mixOutGain   = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN)).toInt() + 100) * 100;
    body.lastMicOn    = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_LASTMIXON)).toUInt();
    body.idGating     = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_IDGATING)).toUInt();
    body.bgpercentage = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_BGPERCENTAGE)).toInt() * 100;
    body.depth        = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE)).toInt() + 100) * 100;
    body.gateHoldT_ms = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME)).toInt();
    body.maxPassChs   = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MAXNOM)).toInt();
    body.attenuationType   = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENUATIONTYPE)).toInt();
    body.attenResponseT_ms = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENRESPONSETIME)).toInt();
    body.attenStep    = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENSTEP)).toInt() + 100) * 100;
    body.attenMax     = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENMAX)).toInt() + 100) * 100;
    for(int ch = 0; ch < getInCtrlNum(type); ch++)
    {
        body.inChAttr[ch].manual    = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_MANUAL).arg(ch)).toInt();
        body.inChAttr[ch].gateMode  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_GATEMODE).arg(ch)).toInt();
        body.inChAttr[ch].priority  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_PRIORITY).arg(ch)).toInt();
        body.inChAttr[ch].threshold = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_THRESHOLD).arg(ch)).toInt() + 100) * 100;
    }

    return body;
}

template<typename t_Data>
t_Data SnapshotSaveAutoMixData::getDataFromSettings_NGRT(t_Data body, QSettings *ConfigIni, CfgType type)
{
//    IG_PARACTL_AUTOMIXNGRT4 body;
    body.mixOutMute    = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE)).toUInt();
    body.mixOutGain    = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN)).toInt() + 100) * 100;
    body.noEffectPass  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_LASTMIXON)).toInt();
    body.defaultch     = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEFAULTCH)).toInt();
    body.threAboveNoise = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_THREABOCENOISE)).toInt() + 100) * 100;
    body.depth          = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE)).toInt() + 100) * 100;
    body.gateHoldT_ms   = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME)).toInt();
    body.autoMaxPassChs = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MAXNOM)).toInt());
    body.directPassAttenEnable = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_DIRECTOUTSNOMATTEN)).toInt();
    body.attenuationType   = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENUATIONTYPE)).toInt();
    body.attenResponseT_ms = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENRESPONSETIME)).toInt();
    body.attenStep = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENSTEP)).toInt() + 100) * 100;
    body.attenMax  = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENMAX)).toInt() + 100) * 100;
    for(int ch = 0; ch < getInCtrlNum(type); ch++)
    {
        body.inChAttr[ch].manual = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_MANUAL).arg(ch)).toInt();
        body.inChAttr[ch].postGateMute = ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_POSTGATEMUTE).arg(ch)).toInt();
        body.inChAttr[ch].postGateGain = (ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_POSTGATEGAIN).arg(ch)).toInt() + 100) * 100;
    }

    return body;
}


template<typename t_Data>
void SnapshotSaveAutoMixData::setDataToSettings_GS(t_Data body, QSettings *ConfigIni, CfgType type)
{
    QList<QString> nameList = body.keys();
    int unimoduleNum = body.count();
    if(unimoduleNum <= 0) return ;
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);
    for(int idx = 0; idx < unimoduleNum; idx++)
    { 
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX).arg(idx));
        quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_THREADLEVEL),
                            body[nameList.at(idx)].threadLevel * 0.01 - 100);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTACKTIME),
                            body[nameList.at(idx)].attackT_ms);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_RELEASETIME),
                            body[nameList.at(idx)].releaseT_ms);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME),
                            body[nameList.at(idx)].holdT_ms);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_DETECTORTIME),
                            body[nameList.at(idx)].detectT_ms);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE),
                            body[nameList.at(idx)].depth * 0.01 - 100);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_NOISEVALUE),
                            body[nameList.at(idx)].noiseGain * 0.01 - 100);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE),
                            body[nameList.at(idx)].mixOutMute);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN),
                            body[nameList.at(idx)].mixOutGain * 0.01 - 100);
        for(int ch = 0; ch < getInCtrlNum(type); ch ++)
        {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INPUTMUTE).arg(ch),
                                body[nameList.at(idx)].inCtrl[ch].inMute);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_PREINGAIN).arg(ch),
                                body[nameList.at(idx)].inCtrl[ch].priorGain * 0.01 - 100);
        }
        ConfigIni->endGroup();
    }

    ConfigIni->endGroup();
}

template<typename t_Data>
void SnapshotSaveAutoMixData::setDataToSettings_NGAT(t_Data body, QSettings *ConfigIni, CfgType type)
{
    QList<QString> nameList = body.keys();
    int unimoduleNum = body.count();
    if(unimoduleNum <= 0) return ;

    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);
    for(int idx = 0; idx < unimoduleNum; idx++)
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX).arg(idx));
        quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE),
                            body[nameList.at(idx)].mixOutMute);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN),
                            body[nameList.at(idx)].mixOutGain * 0.01 - 100);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_LASTMIXON),
                            body[nameList.at(idx)].lastMicOn);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_IDGATING),
                            body[nameList.at(idx)].idGating);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_BGPERCENTAGE),
                            body[nameList.at(idx)].bgpercentage * 0.01);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE),
                            body[nameList.at(idx)].depth*0.01-100);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME),
                            body[nameList.at(idx)].gateHoldT_ms);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MAXNOM),
                            body[nameList.at(idx)].maxPassChs);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENUATIONTYPE),
                            body[nameList.at(idx)].attenuationType);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENRESPONSETIME),
                            body[nameList.at(idx)].attenResponseT_ms);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENSTEP),
                            body[nameList.at(idx)].attenStep*0.01-100);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENMAX),
                            body[nameList.at(idx)].attenMax*0.01-100);
        for(int ch = 0; ch < getInCtrlNum(type); ch++)
        {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_MANUAL).arg(ch),
                                body[nameList.at(idx)].inChAttr[ch].manual);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_GATEMODE).arg(ch),
                                body[nameList.at(idx)].inChAttr[ch].gateMode);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_PRIORITY).arg(ch),
                                body[nameList.at(idx)].inChAttr[ch].priority);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_THRESHOLD).arg(ch),
                                body[nameList.at(idx)].inChAttr[ch].threshold*0.01-100);
        }

        ConfigIni->endGroup();
    }

    ConfigIni->endGroup();
}

template<typename t_Data>
void SnapshotSaveAutoMixData::setDataToSettings_NGRT(t_Data body, QSettings *ConfigIni, CfgType type)
{
    QList<QString> nameList = body.keys();
    int unimoduleNum = body.count();
    if(unimoduleNum <= 0) return ;
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX_TYPE).arg(type));
    ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_UNIMODULE_NUM), unimoduleNum);
    for(int idx = 0; idx < unimoduleNum; idx++)
    {
//        QHash<QString, IG_PARACTL_AUTOMIXNGRT4> body;
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_SNAPSHOT_AUTOMIX).arg(idx));
        quint32 moduleNum = _moduleNum.find(nameList.at(idx)).value();
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_SNAPSHOT_DEVNAME), nameList.at(idx));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), moduleNum);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE),
                            body[nameList.at(idx)].mixOutMute);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN),
                            body[nameList.at(idx)].mixOutGain*0.01-100);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_LASTMIXON),
                            body[nameList.at(idx)].noEffectPass);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEFAULTCH),
                            body[nameList.at(idx)].defaultch);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_THREABOCENOISE),
                            body[nameList.at(idx)].threAboveNoise*0.01-100);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE),
                            body[nameList.at(idx)].depth*0.01-100);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME),
                            body[nameList.at(idx)].gateHoldT_ms);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MAXNOM),
                            body[nameList.at(idx)].autoMaxPassChs);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_DIRECTOUTSNOMATTEN),
                            body[nameList.at(idx)].directPassAttenEnable);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENUATIONTYPE),
                            body[nameList.at(idx)].attenuationType);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENRESPONSETIME),
                            body[nameList.at(idx)].attenResponseT_ms);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENSTEP),
                            body[nameList.at(idx)].attenStep*0.01-100);
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENMAX),
                            body[nameList.at(idx)].attenMax*0.01-100);
        for(int ch = 0; ch < getInCtrlNum(type); ch++)
        {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_MANUAL).arg(ch),
                                body[nameList.at(idx)].inChAttr[ch].manual);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_POSTGATEMUTE).arg(ch),
                                body[nameList.at(idx)].inChAttr[ch].postGateMute);
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_POSTGATEGAIN).arg(ch),
                                body[nameList.at(idx)].inChAttr[ch].postGateGain*0.01-100);
        }
        ConfigIni->endGroup();
    }

    ConfigIni->endGroup();
}

