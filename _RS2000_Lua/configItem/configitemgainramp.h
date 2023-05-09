#ifndef CONFIGITEMGAINRAMP_H
#define CONFIGITEMGAINRAMP_H
#include "configitem.h"

class CfgSettingGainRamp;
class ConfigItemGainRamp : public ConfigItem
{
public:
    ConfigItemGainRamp(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemGainRamp();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    int get_targetTimeA(void) { return _body.targetAT_ms; }
    int get_targetTimeB(void) { return _body.targetBT_ms; }
    int get_muteTime(void)    { return _body.unmuteT_ms; }
    int get_effectGain(void)  { return _body.effectGain; }
    int get_targetGainA(void) { return _body.targetGainA * 0.01 - 100; }
    int get_targetGainB(void) { return _body.targetGainB * 0.01 - 100; }

    void set_targetTimeA(int value);
    void set_targetTimeB(int value);
    void set_targetGainA(int value);
    void set_targetGainB(int value);
    void set_muteTime(int value);
    void set_effectGain(int value);

private:
    typedef struct{
        __OPACK32_T2(
            OPACKU,targetAT_ms,16,
            OPACKU,targetBT_ms,16
        );
        __OPACK32_T2S0S1(
            OPACKU,unmuteT_ms,16,
            OPACKU,effectGain,8							//0 mute ,1 targetGainA ,2 targetGainB
        );
        __OPACK32_T2(
            OPACKU,targetGainA,16,						//targetGainA_dbfs = targetGainA * 0.01 - 100;
            OPACKU,targetGainB,16						//targetGainA_dbfs = targetGainA * 0.01 - 100;
        );
    }IG_PARACTL_GAINRAMP;

    IG_PARACTL_GAINRAMP   _body;
    CfgSettingGainRamp    *_setting;
};

#endif // CONFIGITEMGAINRAMP_H
