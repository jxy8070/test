#ifndef CONFIGITEMDELAY_H
#define CONFIGITEMDELAY_H
#include "configitem.h"

class CfgSettingDelay;
class ConfigItemDelay : public ConfigItem
{
    Q_OBJECT

public:
    ConfigItemDelay(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemDelay();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

public:
    bool getbypass(){return !_body.delaySwitch;}
    int getDelay() {return _body.delayT_ms;}
    int getMaxDelay() {return _maxDelay;}


    void setBypass(bool bypass);
    void setDelay(int delay);

private:
    typedef struct{
        __OPACK32_T2S0S1(
            OPACKU,delaySwitch,8,		//1开，0关
            OPACKU,delayT_ms,16			//延时
        );
    }IG_PARACTL_DELAY;
    typedef struct{
        __OPACK32_T2S0S1(
            OPACKU,delaySwitch,8,		//1开，0关
            OPACKU,delayT_us,16			//延时
        );
    }IG_PARACTL_HPDELAY;

    IG_PARACTL_DELAY _body;
    CfgSettingDelay *_setting;
    int _maxDelay;
};

#endif // CONFIGITEMDELAY_H
