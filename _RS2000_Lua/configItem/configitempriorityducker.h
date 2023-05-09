#ifndef CONFIGITEMPRIORITYDUCKER_H
#define CONFIGITEMPRIORITYDUCKER_H

#include "configitem.h"

class CfgSettingPriorityDucker;
class ConfigItemPriorityDucker : public ConfigItem
{
public:
    ConfigItemPriorityDucker(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemPriorityDucker();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    uint16_t GetAttackTime(void)     { return _body.attackT_ms; }
    uint16_t GetReleaseTime(void)    { return _body.releaseT_ms; }
    uint16_t GetHoldTime(void)       { return _body.holdT_ms; }
    uint16_t GetDetectorTime(void)   { return _body.DetectT_ms; }
    int      GetThresholdLevel(void) { return _body.threshhold * 0.01 - 100; }
    int      GetDepth(void)          { return _body.depth * 0.01 - 100; }
    int      GetInGain(void)         { return _body.priorChInGain * 0.01 - 100; }
    int      GetPriorityGain(void)   { return _body.priorChOutGain * 0.01 - 100; }
    int      GetOutGain(void)        { return _body.mixOutGain * 0.01 - 100; }
    bool     GetBypass(void)         { return _body.bypass; }

    void SetAttatkTime(uint16_t value);
    void SetReleaseTime(uint16_t value);
    void SetHoldTime(uint16_t value);
    void SetDetectorTime(uint16_t value);
    void SetThresholdLevel(int value);
    void SetDepth(int value);
    void SetInGain(int value);
    void SetPriorityGain(int value);
    void SetOutGain(int value);
    void SetBypass(bool checked);

private:
    typedef struct{
        __OPACK32_T2(
            OPACKU,attackT_ms,16,
            OPACKU,releaseT_ms,16
        );
        __OPACK32_T2(
            OPACKU,holdT_ms,16,
            OPACKU,DetectT_ms,16
        );
        __OPACK32_T2(
            OPACKU,threshhold,16,						//threshhold_dbfs = threshhold * 0.01 - 100;
            OPACKU,depth,16								//depth_dbfs = depth * 0.01 - 100;
        );
        __OPACK32_T2(
            OPACKU,priorChInGain,16,					//priorChInGain_dbfs = priorChInGain * 0.01 - 100;
            OPACKU,priorChOutGain,16					//priorChInGain_dbfs = priorChInGain * 0.01 - 100;
        );
        __OPACK32_T2S0S1(
            OPACKU,mixOutGain,16,
            OPACKU,bypass,8
        );
    }IG_PARACTL_PDUCKER;

    IG_PARACTL_PDUCKER   _body;
    CfgSettingPriorityDucker *_setting;
};

#endif // CONFIGITEMPRIORITYDUCKER_H
