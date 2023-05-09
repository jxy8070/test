#include "configitemautomaticmixer.h"
#include "configSetting/cfgsettingautomaticmixerGainSharing.h"
#include "configSetting/cfgsettingautomaticmixergatingabsolutethreshold.h"
#include "configSetting/cfgsettingautomaticmixergatingrelativethreshold.h"
#include "Protocal/packet.h"

ConfigItemAutomaticMixer::ConfigItemAutomaticMixer(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
#if Logo_FangTu /* Function */
    addSubType(CFG_TYPE_AUTOMIXNGAT_4);
    addSubType(CFG_TYPE_AUTOMIXNGAT_8);
#else
    addSubType(CFG_TYPE_AUTOMIX_GS_4);
    addSubType(CFG_TYPE_AUTOMIX_GS_8);
    addSubType(CFG_TYPE_AUTOMIX_GS_16);
    addSubType(CFG_TYPE_AUTOMIX_GS_32);
    addSubType(CFG_TYPE_AUTOMIXNGAT_4);
    addSubType(CFG_TYPE_AUTOMIXNGAT_8);
    addSubType(CFG_TYPE_AUTOMIXNGAT_16);
    addSubType(CFG_TYPE_AUTOMIXNGAT_32);
    addSubType(CFG_TYPE_AUTOMIXNGRT_4);
    addSubType(CFG_TYPE_AUTOMIXNGRT_8);
    addSubType(CFG_TYPE_AUTOMIXNGRT_16);
    addSubType(CFG_TYPE_AUTOMIXNGRT_32);
#endif

    switch (cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        memset(&_body_s4, 0, sizeof(IG_PARACTL_AUTOMIXGS4));
        _body_s4.threadLevel = (-40 + 100) * 100;
        _body_s4.attackT_ms = 10;
        _body_s4.releaseT_ms = 100;
        _body_s4.holdT_ms = 100;
        _body_s4.detectT_ms = 10;
        _body_s4.depth = (-40 + 100) * 100;
        _body_s4.noiseGain = (-30 + 100) * 100;
        for (int i = 0; i < 4; ++i)
        {
            _body_s4.inCtrl[i].inMute = 0;
            _body_s4.inCtrl[i].priorGain = (0 + 100) * 100;
        }
        _body_s4.mixOutMute = 0;
        _body_s4.mixOutGain = (0 + 100) * 100;

        _maxChannel = 4;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        memset(&_body_s8, 0, sizeof(IG_PARACTL_AUTOMIXGS8));
        _body_s8.threadLevel = (-40 + 100) * 100;
        _body_s8.attackT_ms = 10;
        _body_s8.releaseT_ms = 100;
        _body_s8.holdT_ms = 100;
        _body_s8.detectT_ms = 10;
        _body_s8.depth = (-40 + 100) * 100;
        _body_s8.noiseGain = (-30 + 100) * 100;
        for (int i = 0; i < 8; ++i)
        {
            _body_s8.inCtrl[i].inMute = 0;
            _body_s8.inCtrl[i].priorGain = (0 + 100) * 100;
        }
        _body_s8.mixOutMute = 0;
        _body_s8.mixOutGain = (0 + 100) * 100;

        _maxChannel = 8;
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        memset(&_body_s16, 0, sizeof(IG_PARACTL_AUTOMIXGS16));
        _body_s16.threadLevel = (-40 + 100) * 100;
        _body_s16.attackT_ms = 10;
        _body_s16.releaseT_ms = 100;
        _body_s16.holdT_ms = 100;
        _body_s16.detectT_ms = 10;
        _body_s16.depth = (-40 + 100) * 100;
        _body_s16.noiseGain = (-30 + 100) * 100;
        for (int i = 0; i < 16; ++i)
        {
            _body_s16.inCtrl[i].inMute = 0;
            _body_s16.inCtrl[i].priorGain = (0 + 100) * 100;
        }
        _body_s16.mixOutMute = 0;
        _body_s16.mixOutGain = (0 + 100) * 100;

        _maxChannel = 16;
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        memset(&_body_s32, 0, sizeof(IG_PARACTL_AUTOMIXGS32));
        _body_s32.threadLevel = (-40 + 100) * 100;
        _body_s32.attackT_ms = 10;
        _body_s32.releaseT_ms = 100;
        _body_s32.holdT_ms = 100;
        _body_s32.detectT_ms = 10;
        _body_s32.depth = (-40 + 100) * 100;
        _body_s32.noiseGain = (-30 + 100) * 100;
        for (int i = 0; i < 32; ++i)
        {
            _body_s32.inCtrl[i].inMute = 0;
            _body_s32.inCtrl[i].priorGain = (0 + 100) * 100;
        }
        _body_s32.mixOutMute = 0;
        _body_s32.mixOutGain = (0 + 100) * 100;

        _maxChannel = 32;
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        memset(&_body_gat4, 0, sizeof(IG_PARACTL_AUTOMIXNGAT4));
        _body_gat4.mixOutMute = 0;
        _body_gat4.mixOutGain = (0 + 100) * 100;
        _body_gat4.lastMicOn    = 1;
        _body_gat4.idGating     = 1;
        _body_gat4.bgpercentage = 0 * 100;
        _body_gat4.depth        = (60 + 100) * 100;
        _body_gat4.gateHoldT_ms = 1000;
        _body_gat4.maxPassChs        = 3;
        _body_gat4.attenuationType   = 1;
        _body_gat4.attenStep         = (3 + 100) * 100;
        _body_gat4.attenMax          = (20 + 100) * 100;
        _body_gat4.attenResponseT_ms = 10;
        for (int i = 0; i < 4; ++i) {
            _body_gat4.inChAttr[i].manual    = 0;
            _body_gat4.inChAttr[i].gateMode  = 0;
            _body_gat4.inChAttr[i].priority  = 1;
            _body_gat4.inChAttr[i].threshold = (-20 + 100) * 100;
        }

        _maxChannel = 4;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        memset(&_body_gat8, 0, sizeof(IG_PARACTL_AUTOMIXNGAT8));
        _body_gat8.mixOutMute = 0;
        _body_gat8.mixOutGain = (0 + 100) * 100;
        _body_gat8.lastMicOn    = 1;
        _body_gat8.idGating     = 1;
        _body_gat8.bgpercentage = 0 * 100;
        _body_gat8.depth        = (60 + 100) * 100;
        _body_gat8.gateHoldT_ms = 1000;
        _body_gat8.maxPassChs        = 3;
        _body_gat8.attenuationType   = 1;
        _body_gat8.attenStep         = (3 + 100) * 100;
        _body_gat8.attenMax          = (20 + 100) * 100;
        _body_gat8.attenResponseT_ms = 10;
        for (int i = 0; i < 8; ++i) {
            _body_gat8.inChAttr[i].manual    = 0;
            _body_gat8.inChAttr[i].gateMode  = 0;
            _body_gat8.inChAttr[i].priority  = 1;
            _body_gat8.inChAttr[i].threshold = (-20 + 100) * 100;
        }

        _maxChannel = 8;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        memset(&_body_gat16, 0, sizeof(IG_PARACTL_AUTOMIXNGAT16));
        _body_gat16.mixOutMute = 0;
        _body_gat16.mixOutGain = (0 + 100) * 100;
        _body_gat16.lastMicOn    = 1;
        _body_gat16.idGating     = 1;
        _body_gat16.bgpercentage = 0 * 100;
        _body_gat16.depth        = (60 + 100) * 100;
        _body_gat16.gateHoldT_ms = 1000;
        _body_gat16.maxPassChs        = 3;
        _body_gat16.attenuationType   = 1;
        _body_gat16.attenStep         = (3 + 100) * 100;
        _body_gat16.attenMax          = (20 + 100) * 100;
        _body_gat16.attenResponseT_ms = 10;
        for (int i = 0; i < 16; ++i) {
            _body_gat16.inChAttr[i].manual    = 0;
            _body_gat16.inChAttr[i].gateMode  = 0;
            _body_gat16.inChAttr[i].priority  = 1;
            _body_gat16.inChAttr[i].threshold = (-20 + 100) * 100;
        }

        _maxChannel = 16;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        memset(&_body_gat32, 0, sizeof(IG_PARACTL_AUTOMIXNGAT32));
        _body_gat32.mixOutMute = 0;
        _body_gat32.mixOutGain = (0 + 100) * 100;
        _body_gat32.lastMicOn    = 1;
        _body_gat32.idGating     = 1;
        _body_gat32.bgpercentage = 0 * 100;
        _body_gat32.depth        = (60 + 100) * 100;
        _body_gat32.gateHoldT_ms = 1000;
        _body_gat32.maxPassChs        = 3;
        _body_gat32.attenuationType   = 1;
        _body_gat32.attenStep         = (3 + 100) * 100;
        _body_gat32.attenMax          = (20 + 100) * 100;
        _body_gat32.attenResponseT_ms = 10;
        for (int i = 0; i < 32; ++i) {
            _body_gat32.inChAttr[i].manual    = 0;
            _body_gat32.inChAttr[i].gateMode  = 0;
            _body_gat32.inChAttr[i].priority  = 1;
            _body_gat32.inChAttr[i].threshold = (-20 + 100) * 100;
        }

        _maxChannel = 32;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        memset(&_body_grt4, 0, sizeof(IG_PARACTL_AUTOMIXNGRT4));
        _body_grt4.noEffectPass   = 1;
        _body_grt4.threAboveNoise = (10 + 100) * 100;
        _body_grt4.depth          = (60 + 100) * 100;
        _body_grt4.gateHoldT_ms   = 1000;
        _body_grt4.autoMaxPassChs        = 3;
        _body_grt4.directPassAttenEnable = 1;
        _body_grt4.attenuationType       = 1;
        _body_grt4.attenResponseT_ms     = 10;
        _body_grt4.attenStep             = (3 + 100) * 100;
        _body_grt4.attenMax              = (20 + 100) * 100;
        _body_grt4.mixOutMute = 0;
        _body_grt4.mixOutGain = (0 + 100) * 100;
        for (int i = 0; i < 4; ++i) {
            _body_grt4.inChAttr[i].manual       = 0;
            _body_grt4.inChAttr[i].postGateMute = 0;
            _body_grt4.inChAttr[i].postGateGain = (0 + 100) * 100;
        }

        _maxChannel = 4;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        memset(&_body_grt8, 0, sizeof(IG_PARACTL_AUTOMIXNGRT8));
        _body_grt8.noEffectPass   = 1;
        _body_grt8.threAboveNoise = (10 + 100) * 100;
        _body_grt8.depth          = (60 + 100) * 100;
        _body_grt8.gateHoldT_ms   = 1000;
        _body_grt8.autoMaxPassChs        = 3;
        _body_grt8.directPassAttenEnable = 1;
        _body_grt8.attenuationType       = 1;
        _body_grt8.attenResponseT_ms     = 10;
        _body_grt8.attenStep             = (3 + 100) * 100;
        _body_grt8.attenMax              = (20 + 100) * 100;
        _body_grt8.mixOutMute = 0;
        _body_grt8.mixOutGain = (0 + 100) * 100;
        for (int i = 0; i < 8; ++i) {
            _body_grt8.inChAttr[i].manual       = 0;
            _body_grt8.inChAttr[i].postGateMute = 0;
            _body_grt8.inChAttr[i].postGateGain = (0 + 100) * 100;
        }

        _maxChannel = 8;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        memset(&_body_grt16, 0, sizeof(IG_PARACTL_AUTOMIXNGRT16));
        _body_grt16.noEffectPass   = 1;
        _body_grt16.threAboveNoise = (10 + 100) * 100;
        _body_grt16.depth          = (60 + 100) * 100;
        _body_grt16.gateHoldT_ms   = 1000;
        _body_grt16.autoMaxPassChs        = 3;
        _body_grt16.directPassAttenEnable = 1;
        _body_grt16.attenuationType       = 1;
        _body_grt16.attenResponseT_ms     = 10;
        _body_grt16.attenStep             = (3 + 100) * 100;
        _body_grt16.attenMax              = (20 + 100) * 100;
        _body_grt16.mixOutMute = 0;
        _body_grt16.mixOutGain = (0 + 100) * 100;
        for (int i = 0; i < 16; ++i) {
            _body_grt16.inChAttr[i].manual       = 0;
            _body_grt16.inChAttr[i].postGateMute = 0;
            _body_grt16.inChAttr[i].postGateGain = (0 + 100) * 100;
        }

        _maxChannel = 16;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        memset(&_body_grt32, 0, sizeof(IG_PARACTL_AUTOMIXNGRT32));
        _body_grt32.noEffectPass   = 1;
        _body_grt32.threAboveNoise = (10 + 100) * 100;
        _body_grt32.depth          = (60 + 100) * 100;
        _body_grt32.gateHoldT_ms   = 1000;
        _body_grt32.autoMaxPassChs        = 3;
        _body_grt32.directPassAttenEnable = 1;
        _body_grt32.attenuationType       = 1;
        _body_grt32.attenResponseT_ms     = 10;
        _body_grt32.attenStep             = (3 + 100) * 100;
        _body_grt32.attenMax              = (20 + 100) * 100;
        _body_grt32.mixOutMute = 0;
        _body_grt32.mixOutGain = (0 + 100) * 100;
        for (int i = 0; i < 32; ++i) {
            _body_grt32.inChAttr[i].manual       = 0;
            _body_grt32.inChAttr[i].postGateMute = 0;
            _body_grt32.inChAttr[i].postGateGain = (0 + 100) * 100;
        }

        _maxChannel = 32;
        break;
    default:
        break;
    }

    QStringList iPortNameList, oPortNameList;
    for (int i = 0; i < _maxChannel; ++i) {
        iPortNameList.append(QString("Input %1").arg((i+1)));
        oPortNameList.append(QString("Output %1").arg((i+1)));
    }
    oPortNameList.append(QString("Mix Output"));
    initPort(_maxChannel, _maxChannel+1, iPortNameList, oPortNameList);
    _setting_gainsharing = NULL;
    _setting_gatingAbsoluteThreshold = NULL;
    _setting_gatingRelativeThreshold = NULL;

    //控制针脚初始化
    QList<CLuaPinParam_PC*> pinList;
    pinList.append(new CLuaPinParam_PC(this, name, "Threshold", 0x000B0001, 0x000B0002));
    pinList.append(new CLuaPinParam_PC(this, name, "AttackTime", 0x000B0003, 0x000B0004));
    pinList.append(new CLuaPinParam_PC(this, name, "ReleaseTime", 0x000B0005, 0x000B0006));
    pinList.append(new CLuaPinParam_PC(this, name, "HoldTime", 0x000B0007, 0x000B0008));
    pinList.append(new CLuaPinParam_PC(this, name, "Depth", 0x000B000B, 0x0005000C));
    pinList.append(new CLuaPinParam_PC(this, name, "NoiseGain", 0x000B000F, 0x000B0010));
    for(int i = 0; i< _inPortList.length(); i++)
    {
        QString groupName = QString("In%1").arg(i+1);
        pinList.append(new CLuaPinParam_PC(this, name, "Mute", 0x000B0011, 0x000B0012, (quint8)i, 0x00, groupName));
        pinList.append(new CLuaPinParam_PC(this, name, "Gain", 0x000B0013, 0x000B0014, (quint8)i, 0x00, groupName));
    }
    for(int i = 0; i< _outPortList.length(); i++)
    {
        QString groupName = QString("Out%1").arg(i+1);
        pinList.append(new CLuaPinParam_PC(this, name, "Mute", 0x000B0015, 0x000B0016, (quint8)i, 0x00, groupName));
        pinList.append(new CLuaPinParam_PC(this, name, "Gain", 0x000B0017, 0x000B0018, (quint8)i, 0x00, groupName));
    }
    pinList.append(new CLuaPinParam_PC(this, name, "Percentage", 0x000B001F, 0x000B0020));
    pinList.append(new CLuaPinParam_PC(this, name, "LastMicOn", 0x000B0023, 0x000B0024));
    pinList.append(new CLuaPinParam_PC(this, name, "DefaultCh", 0x000B0025, 0x000B0026));
    pinList.append(new CLuaPinParam_PC(this, name, "DbAboveNoise", 0x000B0027, 0x000B0028));
    pinList.append(new CLuaPinParam_PC(this, name, "MaxNOM", 0x000B0029, 0x000B002A));
    pinList.append(new CLuaPinParam_PC(this, name, "DirectOutsNOMAtten", 0x000B002B, 0x000B002C));
    pinList.append(new CLuaPinParam_PC(this, name, "NOMAttenuationType", 0x000B002D, 0x000B002E));
    pinList.append(new CLuaPinParam_PC(this, name, "NOMAttenuationStep", 0x000B002F, 0x000B0030));
    pinList.append(new CLuaPinParam_PC(this, name, "MaxNOMAttenuation", 0x000B0031, 0x000B0032));
    pinList.append(new CLuaPinParam_PC(this, name, "ResponseTime", 0x000B0033, 0x000B0034));
    initControlPins(pinList);
}

ConfigItemAutomaticMixer::~ConfigItemAutomaticMixer()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
    case CFG_TYPE_AUTOMIX_GS_8:
    case CFG_TYPE_AUTOMIX_GS_16:
    case CFG_TYPE_AUTOMIX_GS_32:
        if (_setting_gainsharing != NULL) {
            delete _setting_gainsharing;
        }
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
    case CFG_TYPE_AUTOMIXNGAT_8:
    case CFG_TYPE_AUTOMIXNGAT_16:
    case CFG_TYPE_AUTOMIXNGAT_32:
        if (_setting_gatingAbsoluteThreshold != NULL) {
            delete _setting_gatingAbsoluteThreshold;
        }
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
    case CFG_TYPE_AUTOMIXNGRT_8:
    case CFG_TYPE_AUTOMIXNGRT_16:
    case CFG_TYPE_AUTOMIXNGRT_32:
        if (_setting_gatingRelativeThreshold != NULL) {
            delete _setting_gatingRelativeThreshold;
        }
        break;
    default:
        break;
    }
}

QWidget *ConfigItemAutomaticMixer::getSettingForm()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        if (_setting_gainsharing == NULL) {
            _setting_gainsharing = new CfgSettingAutomaticMixerGainSharing(4, this);
        }
        return _setting_gainsharing;
    case CFG_TYPE_AUTOMIX_GS_8:
        if (_setting_gainsharing == NULL) {
            _setting_gainsharing = new CfgSettingAutomaticMixerGainSharing(8, this);
        }
        return _setting_gainsharing;
    case CFG_TYPE_AUTOMIX_GS_16:
        if (_setting_gainsharing == NULL) {
            _setting_gainsharing = new CfgSettingAutomaticMixerGainSharing(16, this);
        }
        return _setting_gainsharing;
    case CFG_TYPE_AUTOMIX_GS_32:
        if (_setting_gainsharing == NULL) {
            _setting_gainsharing = new CfgSettingAutomaticMixerGainSharing(32, this);
        }
        return _setting_gainsharing;
    case CFG_TYPE_AUTOMIXNGAT_4:
        if (_setting_gatingAbsoluteThreshold == NULL) {
            _setting_gatingAbsoluteThreshold = new CfgSettingAutomaticMixerGatingAbsoluteThreshold(4, this);
        }
        return _setting_gatingAbsoluteThreshold;
    case CFG_TYPE_AUTOMIXNGAT_8:
        if (_setting_gatingAbsoluteThreshold == NULL) {
            _setting_gatingAbsoluteThreshold = new CfgSettingAutomaticMixerGatingAbsoluteThreshold(8, this);
        }
        return _setting_gatingAbsoluteThreshold;
    case CFG_TYPE_AUTOMIXNGAT_16:
        if (_setting_gatingAbsoluteThreshold == NULL) {
            _setting_gatingAbsoluteThreshold = new CfgSettingAutomaticMixerGatingAbsoluteThreshold(16, this);
        }
        return _setting_gatingAbsoluteThreshold;
    case CFG_TYPE_AUTOMIXNGAT_32:
        if (_setting_gatingAbsoluteThreshold == NULL) {
            _setting_gatingAbsoluteThreshold = new CfgSettingAutomaticMixerGatingAbsoluteThreshold(32, this);
        }
        return _setting_gatingAbsoluteThreshold;
    case CFG_TYPE_AUTOMIXNGRT_4:
        if (_setting_gatingRelativeThreshold == NULL) {
            _setting_gatingRelativeThreshold = new CfgSettingAutomaticMixerGatingRelativeThreshold(4, this);
        }
        return _setting_gatingRelativeThreshold;
    case CFG_TYPE_AUTOMIXNGRT_8:
        if (_setting_gatingRelativeThreshold == NULL) {
            _setting_gatingRelativeThreshold = new CfgSettingAutomaticMixerGatingRelativeThreshold(8, this);
        }
        return _setting_gatingRelativeThreshold;
    case CFG_TYPE_AUTOMIXNGRT_16:
        if (_setting_gatingRelativeThreshold == NULL) {
            _setting_gatingRelativeThreshold = new CfgSettingAutomaticMixerGatingRelativeThreshold(16, this);
        }
        return _setting_gatingRelativeThreshold;
    case CFG_TYPE_AUTOMIXNGRT_32:
        if (_setting_gatingRelativeThreshold == NULL) {
            _setting_gatingRelativeThreshold = new CfgSettingAutomaticMixerGatingRelativeThreshold(32, this);
        }
        return _setting_gatingRelativeThreshold;
    default:
        return nullptr;
    }
}

QByteArray ConfigItemAutomaticMixer::getDetailData()
{
    QByteArray paraBuff;
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        paraBuff.append((const char *)&_body_s4, sizeof(IG_PARACTL_AUTOMIXGS4));
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        paraBuff.append((const char *)&_body_s8, sizeof(IG_PARACTL_AUTOMIXGS8));
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        paraBuff.append((const char *)&_body_s16, sizeof(IG_PARACTL_AUTOMIXGS16));
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        paraBuff.append((const char *)&_body_s32, sizeof(IG_PARACTL_AUTOMIXGS32));
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        paraBuff.append((const char *)&_body_gat4, sizeof(IG_PARACTL_AUTOMIXNGAT4));
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        paraBuff.append((const char *)&_body_gat8, sizeof(IG_PARACTL_AUTOMIXNGAT8));
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        paraBuff.append((const char *)&_body_gat16, sizeof(IG_PARACTL_AUTOMIXNGAT16));
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        paraBuff.append((const char *)&_body_gat32, sizeof(IG_PARACTL_AUTOMIXNGAT32));
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        paraBuff.append((const char *)&_body_grt4, sizeof(IG_PARACTL_AUTOMIXNGRT4));
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        paraBuff.append((const char *)&_body_grt8, sizeof(IG_PARACTL_AUTOMIXNGRT8));
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        paraBuff.append((const char *)&_body_grt16, sizeof(IG_PARACTL_AUTOMIXNGRT16));
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        paraBuff.append((const char *)&_body_grt32, sizeof(IG_PARACTL_AUTOMIXNGRT32));
        break;
    default:
        break;
    }

    return paraBuff;
}


void ConfigItemAutomaticMixer::setDetailData(unsigned int *d)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        memcpy(&_body_s4, d, sizeof(IG_PARACTL_AUTOMIXGS4));

        if(_setting_gainsharing != NULL) {
            _setting_gainsharing->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        memcpy(&_body_s8, d, sizeof(IG_PARACTL_AUTOMIXGS8));

        if(_setting_gainsharing != NULL) {
            _setting_gainsharing->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        memcpy(&_body_s16, d, sizeof(IG_PARACTL_AUTOMIXGS16));

        if(_setting_gainsharing != NULL) {
            _setting_gainsharing->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        memcpy(&_body_s32, d, sizeof(IG_PARACTL_AUTOMIXGS32));

        if(_setting_gainsharing != NULL) {
            _setting_gainsharing->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        memcpy(&_body_gat4, d, sizeof(IG_PARACTL_AUTOMIXNGAT4));

        if(_setting_gatingAbsoluteThreshold != NULL) {
            _setting_gatingAbsoluteThreshold->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        memcpy(&_body_gat8, d, sizeof(IG_PARACTL_AUTOMIXNGAT8));

        if(_setting_gatingAbsoluteThreshold != NULL) {
            _setting_gatingAbsoluteThreshold->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        memcpy(&_body_gat16, d, sizeof(IG_PARACTL_AUTOMIXNGAT16));

        if(_setting_gatingAbsoluteThreshold != NULL) {
            _setting_gatingAbsoluteThreshold->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        memcpy(&_body_gat32, d, sizeof(IG_PARACTL_AUTOMIXNGAT32));

        if(_setting_gatingAbsoluteThreshold != NULL) {
            _setting_gatingAbsoluteThreshold->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        memcpy(&_body_grt4, d, sizeof(IG_PARACTL_AUTOMIXNGRT4));

        if(_setting_gatingRelativeThreshold != NULL) {
            _setting_gatingRelativeThreshold->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        memcpy(&_body_grt8, d, sizeof(IG_PARACTL_AUTOMIXNGRT8));

        if(_setting_gatingRelativeThreshold != NULL) {
            _setting_gatingRelativeThreshold->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        memcpy(&_body_grt16, d, sizeof(IG_PARACTL_AUTOMIXNGRT16));

        if(_setting_gatingRelativeThreshold != NULL) {
            _setting_gatingRelativeThreshold->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        memcpy(&_body_grt32, d, sizeof(IG_PARACTL_AUTOMIXNGRT32));

        if(_setting_gatingRelativeThreshold != NULL) {
            _setting_gatingRelativeThreshold->updateUiData();
        }
        break;
    default:
        break;
    }
}

void ConfigItemAutomaticMixer::setRtnData(unsigned int *d)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
    case CFG_TYPE_AUTOMIX_GS_8:
    case CFG_TYPE_AUTOMIX_GS_16:
    case CFG_TYPE_AUTOMIX_GS_32:
        if (_setting_gainsharing) {
            _setting_gainsharing->DispRtnData(_cfgType, d);
        }
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
    case CFG_TYPE_AUTOMIXNGAT_8:
    case CFG_TYPE_AUTOMIXNGAT_16:
    case CFG_TYPE_AUTOMIXNGAT_32:
        if (_setting_gatingAbsoluteThreshold) {
            _setting_gatingAbsoluteThreshold->DispRtnData(_cfgType, d);
        }
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
    case CFG_TYPE_AUTOMIXNGRT_8:
    case CFG_TYPE_AUTOMIXNGRT_16:
    case CFG_TYPE_AUTOMIXNGRT_32:
        if (_setting_gatingRelativeThreshold) {
            _setting_gatingRelativeThreshold->DispRtnData(_cfgType, d);
        }
        break;
    default:
        break;
    }
}

void ConfigItemAutomaticMixer::updateCfgSettingLoadedData()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
    case CFG_TYPE_AUTOMIX_GS_8:
    case CFG_TYPE_AUTOMIX_GS_16:
    case CFG_TYPE_AUTOMIX_GS_32:
        if(_setting_gainsharing != NULL) {
            _setting_gainsharing->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
    case CFG_TYPE_AUTOMIXNGAT_8:
    case CFG_TYPE_AUTOMIXNGAT_16:
    case CFG_TYPE_AUTOMIXNGAT_32:
        if (_setting_gatingAbsoluteThreshold != NULL) {
            _setting_gatingAbsoluteThreshold->updateUiData();
        }
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
    case CFG_TYPE_AUTOMIXNGRT_8:
    case CFG_TYPE_AUTOMIXNGRT_16:
    case CFG_TYPE_AUTOMIXNGRT_32:
        if (_setting_gatingRelativeThreshold != NULL) {
            _setting_gatingRelativeThreshold->updateUiData();
        }
        break;
    default:
        break;
    }
}

int8_t ConfigItemAutomaticMixer::GetThreadLevel()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        return _body_s4.threadLevel*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_8:
        return _body_s8.threadLevel*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_16:
        return _body_s16.threadLevel*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_32:
        return _body_s32.threadLevel*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    return 0;
}

uint16_t ConfigItemAutomaticMixer::GetAttackTime()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        return _body_s4.attackT_ms;
    case CFG_TYPE_AUTOMIX_GS_8:
        return _body_s8.attackT_ms;
    case CFG_TYPE_AUTOMIX_GS_16:
        return _body_s16.attackT_ms;
    case CFG_TYPE_AUTOMIX_GS_32:
        return _body_s32.attackT_ms;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    return 0;
}

uint16_t ConfigItemAutomaticMixer::GetReleaseTime()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        return _body_s4.releaseT_ms;
    case CFG_TYPE_AUTOMIX_GS_8:
        return _body_s8.releaseT_ms;
    case CFG_TYPE_AUTOMIX_GS_16:
        return _body_s16.releaseT_ms;
    case CFG_TYPE_AUTOMIX_GS_32:
        return _body_s32.releaseT_ms;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    return 0;
}

uint16_t ConfigItemAutomaticMixer::GetHoldTime()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        return _body_s4.holdT_ms;
    case CFG_TYPE_AUTOMIX_GS_8:
        return _body_s8.holdT_ms;
    case CFG_TYPE_AUTOMIX_GS_16:
        return _body_s16.holdT_ms;
    case CFG_TYPE_AUTOMIX_GS_32:
        return _body_s32.holdT_ms;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.gateHoldT_ms;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.gateHoldT_ms;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.gateHoldT_ms;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.gateHoldT_ms;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.gateHoldT_ms;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.gateHoldT_ms;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.gateHoldT_ms;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.gateHoldT_ms;
    default:
        break;
    }

    return 0;
}

uint16_t ConfigItemAutomaticMixer::GetDetectorTime(void)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        return _body_s4.detectT_ms;
    case CFG_TYPE_AUTOMIX_GS_8:
        return _body_s8.detectT_ms;
    case CFG_TYPE_AUTOMIX_GS_16:
        return _body_s16.detectT_ms;
    case CFG_TYPE_AUTOMIX_GS_32:
        return _body_s32.detectT_ms;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    return 0;
}

int16_t ConfigItemAutomaticMixer::GetDepthValue()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        return _body_s4.depth*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_8:
        return _body_s8.depth*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_16:
        return _body_s16.depth*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_32:
        return _body_s32.depth*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.depth*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.depth*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.depth*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.depth*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.depth*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.depth*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.depth*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.depth*0.01-100;
    default:
        break;
    }

    return 0;
}

int8_t ConfigItemAutomaticMixer::GetNoiseValue()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        return _body_s4.noiseGain*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_8:
        return _body_s8.noiseGain*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_16:
        return _body_s16.noiseGain*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_32:
        return _body_s32.noiseGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    return 0;
}

uint8_t ConfigItemAutomaticMixer::GetInputMute(int ch)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        return _body_s4.inCtrl[ch].inMute;
    case CFG_TYPE_AUTOMIX_GS_8:
        return _body_s8.inCtrl[ch].inMute;
    case CFG_TYPE_AUTOMIX_GS_16:
        return _body_s16.inCtrl[ch].inMute;
    case CFG_TYPE_AUTOMIX_GS_32:
        return _body_s32.inCtrl[ch].inMute;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    return 0;
}

int8_t ConfigItemAutomaticMixer::GetPreInGain(int ch)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        return _body_s4.inCtrl[ch].priorGain*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_8:
        return _body_s8.inCtrl[ch].priorGain*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_16:
        return _body_s16.inCtrl[ch].priorGain*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_32:
        return _body_s32.inCtrl[ch].priorGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    return 0;
}

uint8_t ConfigItemAutomaticMixer::GetMixOutMute()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        return _body_s4.mixOutMute;
    case CFG_TYPE_AUTOMIX_GS_8:
        return _body_s8.mixOutMute;
    case CFG_TYPE_AUTOMIX_GS_16:
        return _body_s16.mixOutMute;
    case CFG_TYPE_AUTOMIX_GS_32:
        return _body_s32.mixOutMute;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.mixOutMute;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.mixOutMute;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.mixOutMute;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.mixOutMute;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.mixOutMute;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.mixOutMute;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.mixOutMute;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.mixOutMute;
    default:
        break;
    }

    return 0;
}

int16_t ConfigItemAutomaticMixer::GetMixOutGain()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        return _body_s4.mixOutGain*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_8:
        return _body_s8.mixOutGain*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_16:
        return _body_s16.mixOutGain*0.01-100;
    case CFG_TYPE_AUTOMIX_GS_32:
        return _body_s32.mixOutGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.mixOutGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.mixOutGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.mixOutGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.mixOutGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.mixOutGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.mixOutGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.mixOutGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.mixOutGain*0.01-100;
    default:
        break;
    }

    return 0;
}

uint8_t ConfigItemAutomaticMixer::GetChManual(int ch)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.inChAttr[ch].manual;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.inChAttr[ch].manual;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.inChAttr[ch].manual;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.inChAttr[ch].manual;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.inChAttr[ch].manual;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.inChAttr[ch].manual;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.inChAttr[ch].manual;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.inChAttr[ch].manual;
    default:
        break;
    }

    return 0;
}

uint8_t ConfigItemAutomaticMixer::GetChMode(int ch)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.inChAttr[ch].gateMode;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.inChAttr[ch].gateMode;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.inChAttr[ch].gateMode;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.inChAttr[ch].gateMode;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    return 0;
}

uint16_t ConfigItemAutomaticMixer::GetChPriority(int ch)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.inChAttr[ch].priority;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.inChAttr[ch].priority;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.inChAttr[ch].priority;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.inChAttr[ch].priority;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    return 0;
}

int16_t ConfigItemAutomaticMixer::GetChThreshold(int ch)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.inChAttr[ch].threshold*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.inChAttr[ch].threshold*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.inChAttr[ch].threshold*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.inChAttr[ch].threshold*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    return 0;
}

uint8_t ConfigItemAutomaticMixer::GetChPostGateMute(int ch)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.inChAttr[ch].postGateMute;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.inChAttr[ch].postGateMute;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.inChAttr[ch].postGateMute;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.inChAttr[ch].postGateMute;
    default:
        break;
    }

    return 0;
}

int16_t ConfigItemAutomaticMixer::GetChPostGateGain(int ch)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.inChAttr[ch].postGateGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.inChAttr[ch].postGateGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.inChAttr[ch].postGateGain*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.inChAttr[ch].postGateGain*0.01-100;
    default:
        break;
    }

    return 0;
}

uint8_t ConfigItemAutomaticMixer::GetLastMicOn()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.lastMicOn;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.lastMicOn;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.lastMicOn;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.lastMicOn;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.noEffectPass;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.noEffectPass;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.noEffectPass;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.noEffectPass;
    default:
        break;
    }

    return 0;
}

uint8_t ConfigItemAutomaticMixer::GetIDGating()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.idGating;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.idGating;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.idGating;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.idGating;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    return 0;
}

float ConfigItemAutomaticMixer::GetBackgroundPercentage()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.bgpercentage * 0.01;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.bgpercentage * 0.01;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.bgpercentage * 0.01;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.bgpercentage * 0.01;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    return 0;
}

uint8_t ConfigItemAutomaticMixer::GetDefaultCh()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.defaultch;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.defaultch;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.defaultch;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.defaultch;
    default:
        break;
    }

    return 0;
}

int16_t ConfigItemAutomaticMixer::GetThresholdLevelAboveNoise()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.threAboveNoise*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.threAboveNoise*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.threAboveNoise*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.threAboveNoise*0.01-100;
    default:
        break;
    }

    return 0;
}

uint8_t ConfigItemAutomaticMixer::GetMaxMON()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.maxPassChs;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.maxPassChs;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.maxPassChs;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.maxPassChs;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.autoMaxPassChs;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.autoMaxPassChs;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.autoMaxPassChs;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.autoMaxPassChs;
    default:
        break;
    }

    return 0;
}

uint8_t ConfigItemAutomaticMixer::GetDirectOutsNOMAtten()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.directPassAttenEnable;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.directPassAttenEnable;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.directPassAttenEnable;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.directPassAttenEnable;
    default:
        break;
    }

    return 0;
}

uint8_t ConfigItemAutomaticMixer::GetNOMAttenuationType()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.attenuationType;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.attenuationType;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.attenuationType;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.attenuationType;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.attenuationType;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.attenuationType;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.attenuationType;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.attenuationType;
    default:
        break;
    }

    return 0;
}

int16_t ConfigItemAutomaticMixer::GetNOMAttenuationStep()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.attenStep*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.attenStep*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.attenStep*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.attenStep*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.attenStep*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.attenStep*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.attenStep*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.attenStep*0.01-100;
    default:
        break;
    }

    return 0;
}

int16_t ConfigItemAutomaticMixer::GetMaxNOMAttenuation()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.attenMax*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.attenMax*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.attenMax*0.01-100;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.attenMax*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.attenMax*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.attenMax*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.attenMax*0.01-100;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.attenMax*0.01-100;
    default:
        break;
    }

    return 0;
}

uint16_t ConfigItemAutomaticMixer::GetResponseTime()
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        return _body_gat4.attenResponseT_ms;
    case CFG_TYPE_AUTOMIXNGAT_8:
        return _body_gat8.attenResponseT_ms;
    case CFG_TYPE_AUTOMIXNGAT_16:
        return _body_gat16.attenResponseT_ms;
    case CFG_TYPE_AUTOMIXNGAT_32:
        return _body_gat32.attenResponseT_ms;
    case CFG_TYPE_AUTOMIXNGRT_4:
        return _body_grt4.attenResponseT_ms;
    case CFG_TYPE_AUTOMIXNGRT_8:
        return _body_grt8.attenResponseT_ms;
    case CFG_TYPE_AUTOMIXNGRT_16:
        return _body_grt16.attenResponseT_ms;
    case CFG_TYPE_AUTOMIXNGRT_32:
        return _body_grt32.attenResponseT_ms;
    default:
        break;
    }

    return 0;
}

void ConfigItemAutomaticMixer::SetThreadLevel(int8_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        _body_s4.threadLevel = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        _body_s8.threadLevel = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        _body_s16.threadLevel = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        _body_s32.threadLevel = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetAttackTime(uint16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        _body_s4.attackT_ms = value;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        _body_s8.attackT_ms = value;
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        _body_s16.attackT_ms = value;
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        _body_s32.attackT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetReleaseTime(uint16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        _body_s4.releaseT_ms = value;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        _body_s8.releaseT_ms = value;
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        _body_s16.releaseT_ms = value;
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        _body_s32.releaseT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetHoldTime(uint16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        _body_s4.holdT_ms = value;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        _body_s8.holdT_ms = value;
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        _body_s16.holdT_ms = value;
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        _body_s32.holdT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.gateHoldT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.gateHoldT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.gateHoldT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.gateHoldT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.gateHoldT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.gateHoldT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.gateHoldT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.gateHoldT_ms = value;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetDetectorTime(uint16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        _body_s4.detectT_ms = value;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        _body_s8.detectT_ms = value;
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        _body_s16.detectT_ms = value;
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        _body_s32.detectT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetDepthValue(int16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        _body_s4.depth = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        _body_s8.depth = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        _body_s16.depth = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        _body_s32.depth = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.depth = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.depth = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.depth = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.depth = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.depth = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.depth = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.depth = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.depth = (value + 100) * 100;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetNoiseValue(int8_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        _body_s4.noiseGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        _body_s8.noiseGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        _body_s16.noiseGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        _body_s32.noiseGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetPreInGain(int ch, int8_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        _body_s4.inCtrl[ch].priorGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        _body_s8.inCtrl[ch].priorGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        _body_s16.inCtrl[ch].priorGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        _body_s32.inCtrl[ch].priorGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetInputMute(int ch, uint8_t enable)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        _body_s4.inCtrl[ch].inMute = enable;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        _body_s8.inCtrl[ch].inMute = enable;
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        _body_s16.inCtrl[ch].inMute = enable;
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        _body_s32.inCtrl[ch].inMute = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetMixOutMute(uint8_t enable)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        _body_s4.mixOutMute = enable;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        _body_s8.mixOutMute = enable;
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        _body_s16.mixOutMute = enable;
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        _body_s32.mixOutMute = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.mixOutMute = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.mixOutMute = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.mixOutMute = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.mixOutMute = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.mixOutMute = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.mixOutMute = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.mixOutMute = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.mixOutMute = enable;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetMixOutGain(int16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        _body_s4.mixOutGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        _body_s8.mixOutGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        _body_s16.mixOutGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        _body_s32.mixOutGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.mixOutGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.mixOutGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.mixOutGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.mixOutGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.mixOutGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.mixOutGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.mixOutGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.mixOutGain = (value + 100) * 100;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetChManual(int ch, uint8_t enable)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.inChAttr[ch].manual = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.inChAttr[ch].manual = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.inChAttr[ch].manual = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.inChAttr[ch].manual = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.inChAttr[ch].manual = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.inChAttr[ch].manual = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.inChAttr[ch].manual = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.inChAttr[ch].manual = enable;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetChMode(int ch, uint8_t enable)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.inChAttr[ch].gateMode = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.inChAttr[ch].gateMode = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.inChAttr[ch].gateMode = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.inChAttr[ch].gateMode = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetChPriority(int ch, uint16_t enable)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.inChAttr[ch].priority = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.inChAttr[ch].priority = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.inChAttr[ch].priority = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.inChAttr[ch].priority = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetChThreshold(int ch, int16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.inChAttr[ch].threshold = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.inChAttr[ch].threshold = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.inChAttr[ch].threshold = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.inChAttr[ch].threshold = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetChPostGateMute(int ch, uint8_t enable)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.inChAttr[ch].postGateMute = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.inChAttr[ch].postGateMute = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.inChAttr[ch].postGateMute = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.inChAttr[ch].postGateMute = enable;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetChPostGateGain(int ch, int16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.inChAttr[ch].postGateGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.inChAttr[ch].postGateGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.inChAttr[ch].postGateGain = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.inChAttr[ch].postGateGain = (value + 100) * 100;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetLastMicOn(uint8_t enable)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.lastMicOn = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.lastMicOn = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.lastMicOn = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.lastMicOn = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.noEffectPass = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.noEffectPass = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.noEffectPass = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.noEffectPass = enable;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetIDGating(uint8_t enable)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.idGating = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.idGating = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.idGating = enable;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.idGating = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetBackgroundPercentage(float value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.bgpercentage = value * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.bgpercentage = value * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.bgpercentage = value * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.bgpercentage = value * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetDefaultCh(uint8_t ch)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.defaultch = ch;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.defaultch = ch;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.defaultch = ch;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.defaultch = ch;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetThresholdLevelAboveNoise(int16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.threAboveNoise = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.threAboveNoise = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.threAboveNoise = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.threAboveNoise = (value + 100) * 100;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetMaxMON(uint8_t max)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.maxPassChs = max;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.maxPassChs = max;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.maxPassChs = max;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.maxPassChs = max;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.autoMaxPassChs = max;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.autoMaxPassChs = max;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.autoMaxPassChs = max;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.autoMaxPassChs = max;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetDirectOutsNOMAtten(uint8_t enable)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.directPassAttenEnable = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.directPassAttenEnable = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.directPassAttenEnable = enable;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.directPassAttenEnable = enable;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetNOMAttenuationType(uint8_t type)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.attenuationType = type;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.attenuationType = type;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.attenuationType = type;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.attenuationType = type;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.attenuationType = type;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.attenuationType = type;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.attenuationType = type;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.attenuationType = type;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetNOMAttenuationStep(int16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.attenStep = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.attenStep = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.attenStep = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.attenStep = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.attenStep = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.attenStep = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.attenStep = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.attenStep = (value + 100) * 100;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetMaxNOMAttenuation(int16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.attenMax = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.attenMax = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.attenMax = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.attenMax = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.attenMax = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.attenMax = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.attenMax = (value + 100) * 100;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.attenMax = (value + 100) * 100;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::SetResponseTime(uint16_t value)
{
    switch (_cfgType) {
    case CFG_TYPE_AUTOMIX_GS_4:
        break;
    case CFG_TYPE_AUTOMIX_GS_8:
        break;
    case CFG_TYPE_AUTOMIX_GS_16:
        break;
    case CFG_TYPE_AUTOMIX_GS_32:
        break;
    case CFG_TYPE_AUTOMIXNGAT_4:
        _body_gat4.attenResponseT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGAT_8:
        _body_gat8.attenResponseT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGAT_16:
        _body_gat16.attenResponseT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGAT_32:
        _body_gat32.attenResponseT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGRT_4:
        _body_grt4.attenResponseT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGRT_8:
        _body_grt8.attenResponseT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGRT_16:
        _body_grt16.attenResponseT_ms = value;
        break;
    case CFG_TYPE_AUTOMIXNGRT_32:
        _body_grt32.attenResponseT_ms = value;
        break;
    default:
        break;
    }

    saveParamToDevice();
}

void ConfigItemAutomaticMixer::loadParaFromSetting(QSettings *ConfigIni)
{
    switch(getCfgType()) {
    case CFG_TYPE_AUTOMIX_GS_4:
    case CFG_TYPE_AUTOMIX_GS_8:
    case CFG_TYPE_AUTOMIX_GS_16:
    case CFG_TYPE_AUTOMIX_GS_32:
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_AUTOMIX_GROUP));
        setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
        SetThreadLevel(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_THREADLEVEL)).toInt());
        SetAttackTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTACKTIME)).toUInt());
        SetReleaseTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_RELEASETIME)).toUInt());
        SetHoldTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME)).toUInt());
        SetDetectorTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_DETECTORTIME)).toUInt());
        SetDepthValue(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE)).toInt());
        SetNoiseValue(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_NOISEVALUE)).toInt());
        SetMixOutMute(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE)).toUInt());
        SetMixOutGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN)).toInt());
        for(int i = 0; i < getMaxChannel(); i++)
        {
            SetInputMute(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INPUTMUTE).arg(i)).toUInt());
            SetPreInGain(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_PREINGAIN).arg(i)).toInt());
        }

        ConfigIni->endGroup();
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_4:
    case CFG_TYPE_AUTOMIXNGAT_8:
    case CFG_TYPE_AUTOMIXNGAT_16:
    case CFG_TYPE_AUTOMIXNGAT_32:
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_AUTOMIX_GROUP));
        setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
        SetMixOutMute(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE)).toUInt());
        SetMixOutGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN)).toInt());
        SetLastMicOn(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_LASTMIXON)).toUInt());
        SetIDGating(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_IDGATING)).toUInt());
        SetBackgroundPercentage(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_BGPERCENTAGE)).toInt());
        SetDepthValue(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE)).toInt());
        SetHoldTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME)).toInt());
        SetMaxMON(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MAXNOM)).toInt());
        SetNOMAttenuationType(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENUATIONTYPE)).toInt());
        SetNOMAttenuationStep(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENSTEP)).toInt());
        SetResponseTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENRESPONSETIME)).toInt());
        SetMaxNOMAttenuation(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENMAX)).toInt());
        for(int i = 0; i < getMaxChannel(); i++)
        {
            SetChManual(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_MANUAL).arg(i)).toInt());
            SetChMode(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_GATEMODE).arg(i)).toInt());
            SetChPriority(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_PRIORITY).arg(i)).toInt());
            SetChThreshold(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_THRESHOLD).arg(i)).toInt());
        }

        ConfigIni->endGroup();
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_4:
    case CFG_TYPE_AUTOMIXNGRT_8:
    case CFG_TYPE_AUTOMIXNGRT_16:
    case CFG_TYPE_AUTOMIXNGRT_32:
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_AUTOMIX_GROUP));
        setModuleNum(ConfigIni->value(QString(CONFIG_DEVICE_DEV_MODULE_NO)).toUInt());
        SetMixOutMute(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE)).toUInt());
        SetMixOutGain(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN)).toInt());
        SetLastMicOn(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_LASTMIXON)).toInt());
        SetDefaultCh(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEFAULTCH)).toInt());
        SetThresholdLevelAboveNoise(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_THREABOCENOISE)).toInt());
        SetDepthValue(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE)).toInt());
        SetHoldTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME)).toInt());
        SetMaxMON(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_MAXNOM)).toInt());
        SetDirectOutsNOMAtten(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_DIRECTOUTSNOMATTEN)).toInt());
        SetNOMAttenuationType(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENUATIONTYPE)).toInt());
        SetResponseTime(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENRESPONSETIME)).toInt());
        SetNOMAttenuationStep(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENSTEP)).toInt());
        SetMaxNOMAttenuation(ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENMAX)).toInt());
        for(int i = 0; i < getMaxChannel(); i++)
        {
            SetChManual(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_MANUAL).arg(i)).toInt());
            SetChPostGateMute(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_POSTGATEMUTE).arg(i)).toInt());
            SetChPostGateGain(i, ConfigIni->value(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_POSTGATEGAIN).arg(i)).toInt());
        }

        ConfigIni->endGroup();
        break;
    }
    default:
        break;
    }
}

void ConfigItemAutomaticMixer::saveParaToSetting(QSettings *ConfigIni)
{
    switch(getCfgType()) {
    case CFG_TYPE_AUTOMIX_GS_4:
    case CFG_TYPE_AUTOMIX_GS_8:
    case CFG_TYPE_AUTOMIX_GS_16:
    case CFG_TYPE_AUTOMIX_GS_32:
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_AUTOMIX_GROUP));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_THREADLEVEL), GetThreadLevel());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTACKTIME), GetAttackTime());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_RELEASETIME), GetReleaseTime());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME), GetHoldTime());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_DETECTORTIME), GetDetectorTime());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE), GetDepthValue());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_NOISEVALUE), GetNoiseValue());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE), GetMixOutMute());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN), GetMixOutGain());
        for(int i = 0; i < getMaxChannel(); i++)
        {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INPUTMUTE).arg(i), GetInputMute(i));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_PREINGAIN).arg(i), GetPreInGain(i));
        }

        ConfigIni->endGroup();
        break;
    }
    case CFG_TYPE_AUTOMIXNGAT_4:
    case CFG_TYPE_AUTOMIXNGAT_8:
    case CFG_TYPE_AUTOMIXNGAT_16:
    case CFG_TYPE_AUTOMIXNGAT_32:
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_AUTOMIX_GROUP));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE), GetMixOutMute());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN), GetMixOutGain());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_LASTMIXON), GetLastMicOn());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_IDGATING), GetIDGating());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_BGPERCENTAGE), GetBackgroundPercentage());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE), GetDepthValue());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME), GetHoldTime());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MAXNOM), GetMaxMON());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENUATIONTYPE), GetNOMAttenuationType());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENRESPONSETIME), GetResponseTime());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENSTEP), GetNOMAttenuationStep());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENMAX), GetMaxNOMAttenuation());
        for(int i = 0; i < getMaxChannel(); i++)
        {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_MANUAL).arg(i), GetChManual(i));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_GATEMODE).arg(i), GetChMode(i));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_PRIORITY).arg(i), GetChPriority(i));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_THRESHOLD).arg(i), GetChThreshold(i));
        }

        ConfigIni->endGroup();
        break;
    }
    case CFG_TYPE_AUTOMIXNGRT_4:
    case CFG_TYPE_AUTOMIXNGRT_8:
    case CFG_TYPE_AUTOMIXNGRT_16:
    case CFG_TYPE_AUTOMIXNGRT_32:
    {
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_AUTOMIX_GROUP));
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_MODULE_NO), getModuleNum());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTMUTE), GetMixOutMute());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MIXOUTGAIN), GetMixOutGain());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_LASTMIXON), GetLastMicOn());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEFAULTCH), GetDefaultCh());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_THREABOCENOISE), GetThresholdLevelAboveNoise());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_DEPTHVALUE), GetDepthValue());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_HOLDTIME), GetHoldTime());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_MAXNOM), GetMaxMON());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_DIRECTOUTSNOMATTEN), GetDirectOutsNOMAtten());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENUATIONTYPE), GetNOMAttenuationType());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENRESPONSETIME), GetResponseTime());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENSTEP), GetNOMAttenuationStep());
        ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_ATTENMAX), GetMaxNOMAttenuation());
        for(int i = 0; i < getMaxChannel(); i++)
        {
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_MANUAL).arg(i), GetChManual(i));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_POSTGATEMUTE).arg(i), GetChPostGateMute(i));
            ConfigIni->setValue(QString(CONFIG_DEVICE_DEV_AUTOMIX_INCHATTR_POSTGATEGAIN).arg(i), GetChPostGateGain(i));
        }
        ConfigIni->endGroup();
        break;
    }
    default:
        break;
    }
}


QVariant ConfigItemAutomaticMixer::setLuaPinValue(CLuaPinParam_PC* param, QVariant val)
{
    assert(false); //wj
    return QVariant();
//    bool setAutoMixerDepth(const QString& itemName, double gain);                       //0x000B000C
//    bool setAutoMixerNoiseGain(const QString& itemName, double gain);                   //0x000B0010
//    bool setAutoMixerInputMute(const QString& itemName, quint8 chNo, bool mute);        //0x000B0012
//    bool setAutoMixerInputGain(const QString& itemName, quint8 chNo, double gain);      //0x000B0014
//    bool setAutoMixerOutputMute(const QString& itemName, quint8 chNo, bool mute);       //0x000B0016
//    bool setAutoMixerOutputGain(const QString& itemName, quint8 chNo, double gain);     //0x000B0018
//    bool getAutoMixerInputManual(const QString& itemName, quint8 chNo, bool& manual);   //0x000B0019
//    bool setAutoMixerInputManual(const QString& itemName, quint8 chNo, bool manual);    //0x000B001A
//    bool getAutoMixerInputMode(const QString& itemName, quint8 chNo, quint8& mode);     //0x000B001B
//    bool setAutoMixerInputMode(const QString& itemName, quint8 chNo, quint8 mode);      //0x000B001C
//    bool getAutoMixerInputPriority(const QString& itemName, quint8 chNo, quint16& prio);//0x000B001D
//    bool setAutoMixerInputPriority(const QString& itemName, quint8 chNo, quint16 prio); //0x000B001E
//    bool getAutoMixerPercentage(const QString& itemName, double& percent);              //0x000B001F
//    bool setAutoMixerPercentage(const QString& itemName, double percent);               //0x000B0020
//    bool getAutoMixerLastMicOn(const QString& itemName, quint8& status);                //0x000B0023
//    bool setAutoMixerLastMicOn(const QString& itemName, quint8 status);                 //0x000B0024


//    void SetDetectorTime(uint16_t value);
////    void SetDepthEnable(uint8_t enable);
//    void SetDepthValue(int16_t value);
////    void SetNoiseEnable(uint8_t enable);
//    void SetNoiseValue(int8_t value);
//    void SetInputMute(int ch, uint8_t enable);
//    void SetPreInGain(int ch, int8_t value);
//    void SetMixOutMute(uint8_t enable);
//    void SetMixOutGain(int16_t value);
//    switch(param->SetCmdKey())
//    {
//    case 0x000B0002:
//        assert(false);
////        void SetThreadLevel(int8_t value);
////        setFreq(val.toDouble());
//        return QVariant(val.toDouble());
//    case 0x000B0004:
//        SetAttackTime(val.toInt());
//        return QVariant(val.toInt());
//    case 0x000B0006:
//        SetReleaseTime(val.toInt());
//        return QVariant(val.toInt());
//    case 0x000B0008:
//        SetHoldTime(val.toInt());
//        return QVariant(val.toInt());

//    case 0x000B000C:
//        SetDepthValue(val.toBool());
//        return QVariant(val.toBool());
//    case 0x000B0010:
//        SetNoiseValue(val.toBool());
//        return QVariant(val.toBool());

//    case 0x000B0006:
//        setMute(val.toBool());
//        return QVariant(val.toBool());
//    case 0x000B0006:
//        setMute(val.toBool());
//        return QVariant(val.toBool());
//    case 0x000B0006:
//        setMute(val.toBool());
//        return QVariant(val.toBool());
//    case 0x000B0006:
//        setMute(val.toBool());
//        return QVariant(val.toBool());
//    case 0x000B0006:
//        setMute(val.toBool());
//        return QVariant(val.toBool());
//    case 0x000B0006:
//        setMute(val.toBool());
//        return QVariant(val.toBool());
//    case 0x000B0006:
//        setMute(val.toBool());
//        return QVariant(val.toBool());
//    case 0x000B0006:
//        setMute(val.toBool());
//        return QVariant(val.toBool());
//    default:
//        return QVariant();
//    }
//    void SetChManual(int ch, uint8_t enable);
//    void SetChMode(int ch, uint8_t enable);
//    void SetChPriority(int ch, uint16_t enable);
//    void SetChThreshold(int ch, int16_t value);
//    void SetChPostGateMute(int ch, uint8_t enable);
//    void SetChPostGateGain(int ch, int16_t value);
//    void SetLastMicOn(uint8_t enable);
//    void SetIDGating(uint8_t enable);
//    void SetBackgroundPercentage(float value);
//    void SetDefaultCh(uint8_t ch);
//    void SetThresholdLevelAboveNoise(int16_t value);
//    void SetMaxMON(uint8_t max);
//    void SetDirectOutsNOMAtten(uint8_t enable);
//    void SetNOMAttenuationType(uint8_t type);
//    void SetNOMAttenuationStep(int16_t value);
//    void SetMaxNOMAttenuation(int16_t value);
//    void SetResponseTime(uint16_t value);
//    bool getAutoMixerDefaultCh(const QString& itemName, quint8& defCn);                 //0x000B0025
//    bool setAutoMixerDefaultCh(const QString& itemName, quint8 defCn);                  //0x000B0026
//    bool getAutoMixerThresholdLevelAboveNoise(const QString& itemName, double& gain);   //0x000B0027
//    bool setAutoMixerThresholdLevelAboveNoise(const QString& itemName, double gain);    //0x000B0028
//    bool getAutoMixerMaxNOM(const QString& itemName, bool& onOff);                      //0x000B0029
//    bool setAutoMixerMaxNOM(const QString& itemName, bool onOff);                       //0x000B002A
//    bool getAutoMixerDirectOutsNOMAtten(const QString& itemName, bool& onOff);          //0x000B002B
//    bool setAutoMixerDirectOutsNOMAtten(const QString& itemName, bool onOff);           //0x000B002C
//    bool getAutoMixerNOMAttenuationType(const QString& itemName, quint8& type);         //0x000B002D
//    bool setAutoMixerNOMAttenuationType(const QString& itemName, quint8 type);          //0x000B002E
//    bool getAutoMixerNOMAttenuationStep(const QString& itemName, double& gain);         //0x000B002F
//    bool setAutoMixerNOMAttenuationStep(const QString& itemName, double gain);          //0x000B0030
//    bool getAutoMixerMaxNOMAttenuation(const QString& itemName, double& gain);          //0x000B0031
//    bool setAutoMixerMaxNOMAttenuation(const QString& itemName, double gain);           //0x000B0032
//    bool getAutoMixerResponseTime(const QString& itemName, quint16& ms);                //0x000B0033
//    bool setAutoMixerResponseTime(const QString& itemName, quint16 ms);                 //0x000B0034

}
