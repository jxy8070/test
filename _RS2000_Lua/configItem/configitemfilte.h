#ifndef CONFIGITEMFILTE_H
#define CONFIGITEMFILTE_H

#include "configitem.h"

enum EqType;
struct EqData;
class CfgSettingFilte;
class ConfigItemFilte : public ConfigItem
{
public:
    ConfigItemFilte(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemFilte();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

public:
    bool getBypass() {return _bypass;}
    quint16 getFilterType() {return _filterType;}
    EqType getEqType() { return _eqType; }
    EqData* getEqData() {return _eqData;}

    void setBypass(bool bypass);
    void setFilterType(quint16 type);
    void setEqPara(int freq, int gain, int q);
    void setMute(bool enable);
    void setInvert(bool enable);
    bool getMute(void) { return _body_AllPass.mute; }
    bool getInvert(void) { return _body_AllPass.antiphase; }

private:
    enum{
        FCT_FILTER_BUTTERWORTH = 0,
        FCT_FILTER_LINKWITZ = 1,
    };
    typedef struct{
        __OPACK32_T2S0S1(
            OPACKU,filterType,8,
            OPACKU,orders,8					//只对lp,hp有效
        );
        __OPACK32_T2(
            OPACKU,freq_hz,16,				//hz，
            OPACKU,gain,16					//gain_db = gain * 0.01 - 100;
        );
        unsigned int bypass;				//bypass
        unsigned int smooth;				//smooth
    }IG_PARACTL_FILTER;

    typedef struct{
        __OPACK32_T2(
            OPACKU,freq_hz,16,				//hz，
            OPACKU,bandWidth,16				//oct = bandWidth/1000
        );
        unsigned int bypass;				//bypass
        unsigned int smooth;				//smooth
    }IG_PARACTL_BPFILTER;

    typedef struct{
        __OPACK32_T2(
            OPACKU,freq_hz,16,				//hz，
            OPACKU,q,16						//(float)qVal = q * 0.01;
        );
        __OPACK32_T2(
            OPACKU,antiphase,16,
            OPACKU,mute,16					//
        );
        unsigned int bypass;				//bypass
        unsigned int smooth;				//smooth
    }IG_PARACTL_APFILTER;

    IG_PARACTL_FILTER _body;
    IG_PARACTL_BPFILTER _body_BandPass;
    IG_PARACTL_APFILTER _body_AllPass;

    CfgSettingFilte *_setting;
    quint32 _smooth;

    bool _bypass;
    quint16 _filterType;//每6db一段
    EqData *_eqData;
    EqType _eqType;
};

#endif // CONFIGITEMFILTE_H
