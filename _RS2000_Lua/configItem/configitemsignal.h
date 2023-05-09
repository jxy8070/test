#ifndef CONFIGITEMSIGNAL_H
#define CONFIGITEMSIGNAL_H

#include "configitem.h"

class CfgSettingSignal;
class ConfigItemSignal : public ConfigItem
{
    Q_OBJECT

public:
    ConfigItemSignal(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemSignal();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    bool getMute(){return _mute;}
    double getLevel(){return _level;}
    double getFreq(){return _freq;}

    void setMute(bool flag);
    void setLevel(double value);
    void setFreq(double value);

private:
    void initData();

private:
    bool _mute;
    double _level;
    double _freq;

    typedef struct{
        __OPACK32_T2(
            OPACKU,freq_hz,16,				//hz，
            OPACKU,gain,16					//gain_db = gain * 0.01 - 100;
        );
        __OPACK32_T1S0(
            OPACKU,mute,8
        );
    }IG_PARACTL_SIGGEN;

    IG_PARACTL_SIGGEN _body;
    CfgSettingSignal *_setting;
};
#endif // CONFIGITEMSIGNAL_H
