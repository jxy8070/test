#ifndef CONFIGITEMFIRCUSTOM_H
#define CONFIGITEMFIRCUSTOM_H
#include "configitem.h"

class CfgSettingFIRCustom;
class ConfigItemFIRCustom : public ConfigItem
{
public:
    ConfigItemFIRCustom(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemFIRCustom();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    int getMaxCoeffSize(void) {return _coeff_size;}
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;

public:
    void setCoeff(QStringList s);
    float *getCoeff(void) { return _coeff; }
    bool getBypass(void) { return  _bypass; }
    void setBypass(bool e);

private:
    typedef struct{
        float coeff[256];
        __OPACK32_T2(
            OPACKU,shift,16,//偏移
            OPACKU,len,16//长度
        );
        __OPACK32_T2S0S1(
            OPACKU,effect,8,//所有发完之后置1
            OPACKU,bypass,8
        );
    }IG_PARACTL_SOFTFIR;
    IG_PARACTL_SOFTFIR _body;

    quint32 _coeff_size;
    float _coeff[1024];
    bool _bypass;
    CfgSettingFIRCustom *_setting;
};

#endif // CONFIGITEMFIRCUSTOM_H
