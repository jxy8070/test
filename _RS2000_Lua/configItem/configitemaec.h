#ifndef CONFIGITEMAEC_H
#define CONFIGITEMAEC_H
#include "configitem.h"

class CfgSettingAEC;
class ConfigItemAEC : public ConfigItem
{
public:
    ConfigItemAEC(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemAEC();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

public:
    int getMicInGain(void) {return _body.micInGain * 0.01 - 100;}
    int getLineInGain(void) {return _body.lineInGain * 0.01 - 100;}
    int getSpeakOutGain(void) {return _body.speakOutGain * 0.01 - 100;}
    int getLineOutGain(void) {return _body.lineOutGain * 0.01 - 100;}
    int getSpeak2LineGain(void) {return _body.micToSpeakGain * 0.01 - 100;}
    int getSpeak2LineSwitch(void) {return _body.micToSpeakSwitch;}

    int getNRGain(void) {
#ifdef Logo_BeiJingHuiSheng
        return 0;
#else
        return _body.NRGain * 0.01 - 100;
#endif
    }

    bool getNRGainSwitch(void) {
#ifdef Logo_BeiJingHuiSheng
        return 0;
#else
        return _body.NRSwitch;
#endif
    }

    void setMicInGain(int value);
    void setLineInGain(int value);
    void setSpeakOutGain(int value);
    void setLineOutGain(int value);
    void setSpeak2LineGain(int value);
    void setSpeak2LineSwitch(int value);
    void setNRGain(int value);
    void setNRGainSwitch(bool enable);

private:
    typedef struct{
        __OPACK32_T2(
            OPACKU,micInGain,16,						//gain_db = micIngain * 0.01 - 100;
            OPACKU,lineInGain,16
        );
        __OPACK32_T2(
            OPACKU,speakOutGain,16,
            OPACKU,lineOutGain,16
        );
        __OPACK32_T2S0S1(
            OPACKU,micToSpeakGain,16,
            OPACKU,micToSpeakSwitch,8
        );
#ifndef Logo_BeiJingHuiSheng
        __OPACK32_T2S0S1(
          OPACKU,NRGain,16,
          OPACKU,NRSwitch,8
         );
#endif
    }IG_PARACTL_HARDAEC;

    IG_PARACTL_HARDAEC _body;
    CfgSettingAEC *_setting;
};

#endif // CONFIGITEMAEC_H
