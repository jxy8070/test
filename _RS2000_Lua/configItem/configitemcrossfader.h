#ifndef CONFIGITEMCROSSFADER_H
#define CONFIGITEMCROSSFADER_H
#include "configitem.h"

class CfgSettingCrossfader;
class ConfigItemCrossfader : public ConfigItem
{
public:
    ConfigItemCrossfader(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemCrossfader();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    int get_time(void) { return _body.chBandSwT_ms; }
    int get_ch(void)   { return _body.targetChBand; }
    int get_type(void) { return _body.sumType; }

    void set_time(int value);
    void set_ch(int ch);
    void set_type(int type);

private:
    typedef struct{
        __OPACK32_T3(
            OPACKU,chBandSwT_ms,24,				//通道组切换时间
            OPACKU,targetChBand,4,				//0,通道组A;1通道组B
            OPACKU,sumType,4					//0,Gain;1power
        );
    }IG_PARACTL_CROSSFADER;

    IG_PARACTL_CROSSFADER      _body;
    CfgSettingCrossfader       *_setting;
};

#endif // CONFIGITEMCROSSFADER_H
