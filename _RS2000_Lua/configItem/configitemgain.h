#ifndef CONFIGITEMGAIN_H
#define CONFIGITEMGAIN_H
#include "configitem.h"

class CfgSettingGain;
class ConfigItemGain : public ConfigItem
{
public:
    ConfigItemGain(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemGain();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    int get_mode(void)   { return _body.tsMode; };
    int get_phase(void)  { return _body.antiPhase; };
    double get_gain(void)   { return _body.gain * 0.01 - 100; }

    void set_mode(int en);
    void set_phase(int en);
    void set_gain(double value);

private:
    typedef struct{
        __OPACK32_T3S0S1S2(
            OPACKU,tsMode,4,			//2bypass,1静音,0正常输出
            OPACKU,antiPhase,4,			//1反相，0正向
            OPACKU,gain,16				//1开，0关,gain_db = gain * 0.01 - 100;
        );
    }IG_PARACTL_GAIN;

    IG_PARACTL_GAIN   _body;
    CfgSettingGain    *_setting;
};

#endif // CONFIGITEMGAIN_H
