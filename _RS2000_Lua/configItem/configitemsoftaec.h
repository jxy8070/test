#ifndef CONFIGITEMSOFTAEC_H
#define CONFIGITEMSOFTAEC_H
#include "configitem.h"

class CfgSettingSoftAEC;
class ConfigItemSoftAEC : public ConfigItem
{
public:
    ConfigItemSoftAEC(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemSoftAEC();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

public:
    bool getBypass(void) { return _body.micToFarBypass; }
    void setBypass(bool enable) {_body.micToFarBypass = enable; saveParamToDevice();}
    double getMicToSpeakGain(void) {return _body.micToSpeakGain * 0.01 - 100;}
    void setMicToSpeakGain(double value) { _body.micToSpeakGain = (value + 100) * 100; saveParamToDevice();}
    bool getMicToSpeakSwitch(void) { return _body.micToSpeakSwitch; }
    void setMicToSpeakSwitch(bool enable) {_body.micToSpeakSwitch = enable; saveParamToDevice();}

    double getRefGain(void) { return _body.refGain * 0.01 - 100; }
    void setRefGain(double value) { _body.refGain = (value + 100) * 100;  saveParamToDevice();}
    int getRESStrength(void) { return _body.RESStrength; }
    void setRESStrength(int value) { _body.RESStrength = value; saveParamToDevice();}
    bool getRESSwitch(void) { return _body.RESSwitch; }
    void setRESSwitch(bool enable) {_body.RESSwitch = enable; saveParamToDevice();}

    double getNRGain(void) { return _body.NRGain * 0.01 - 100; }
    void setNRGain(double value) { _body.NRGain = (value + 100) * 100; saveParamToDevice();}
    bool getNRSwitch(void) { return _body.NRSwitch; }
    void setNRSwitch(bool enable) {_body.NRSwitch = enable; saveParamToDevice();}

    double getCNGain(void) { return _body.CNGain * 0.01 - 100; }
    void setCNGain(double value) { _body.CNGain = (value + 100) * 100; saveParamToDevice();}
    bool getCNSwitch(void) { return _body.CNSwitch; }
    void setCNSwitch(bool enable) {_body.CNSwitch = enable; saveParamToDevice();}


private:
    typedef struct{
        __OPACK32_T3(
            OPACKU,micToSpeakGain,16,
            OPACKU,micToSpeakSwitch,8,
            OPACKU,micToFarBypass,8
        );
        __OPACK32_T3(
            OPACKU,refGain,16,
            OPACKU,RESStrength,8,		//(0~100)
            OPACKU,RESSwitch,8			//residual echo suppress
        );
        __OPACK32_T2S0S1(				//noise reduce
            OPACKU,NRGain,16,
            OPACKU,NRSwitch,8
        );
        __OPACK32_T2S0S1(				//comfort Noise
            OPACKU,CNGain,16,
            OPACKU,CNSwitch,8
        );
        unsigned int remain[2];
    }IG_PARACTL_SOFTAEC;

    typedef struct{
        __OPACK32_T2(
            OPACKU,ERLE,16,				//echo return loss enhancement,				ERLE = ERLE_db * 0.01 - 100;
            OPACKU,RMLR,16				//reference to microphone level ratio		RMLR = ERLE_db * 0.01 - 100;
        );
        unsigned int remain[2];
    }IG_PARAGET_SOFTAEC;
    IG_PARACTL_SOFTAEC _body;
    CfgSettingSoftAEC *_setting = NULL;
};

#endif // CONFIGITEMSOFTAEC_H
