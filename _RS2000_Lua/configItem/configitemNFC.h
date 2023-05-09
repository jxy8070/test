#ifndef CONFIGITEMNFC_H
#define CONFIGITEMNFC_H
#include "configitem.h"

class CfgSettingNFC;
class ConfigItemNFC : public ConfigItem
{
public:
    ConfigItemNFC(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemNFC();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;

public:
    int getOutGain(void) { return _body.outGain * 0.01 - 100; }
    unsigned char getOutMode(void) { return _body.outMode; }
    unsigned char getBandwidth(void) { return _body.bandwidth; }
    unsigned char getFixfilters(void) { return _body.fixfilters; }
    unsigned char getDynfilters(void) { return _body.dynfilters; }
    int getMaxNotchDepth(void) { return _body.maxNotchDepth * 0.01 - 100; }

    void setOutGain(int d);
    void setOutMode(unsigned char d);
    void setClearFilter(unsigned char d);
    void setBandwidth(unsigned char d);
    void setFixfilters(unsigned char d);
    void setDynfilters(unsigned char d);
    void setMaxNotchDepth(int d);

private:
    typedef struct{
        __OPACK32_T4(
            OPACKU,outGain,16,							//outGain_dbfs = outGain * 0.01 - 100;
            OPACKU,outMode,4,							//0commen，1bypass，2mute
            OPACKU,clearfilter,4,						//0。filter work;1.clear dymatic filter;2.clear all filter;
            OPACKU,bandwidth,8							//[4,128]
        );
        __OPACK32_T3(
            OPACKU,fixfilters,8,
            OPACKU,dynfilters,8,
            OPACKU,maxNotchDepth,16						//maxNotchDepth_dbfs = maxNotchDepth * 0.01 - 100;
        );
    }IG_PARACTL_NFC;

    typedef struct{
        __OPACK32_T2(
            OPACKU,howlFreqs,16,
            OPACKU,rtNotchDepth,16						//[...,-6]maxNotchDepth_dbfs = maxNotchDepth * 0.01 - 100;
        );
    }IG_PARAGET_NFC_NF;

    typedef struct{
        IG_PARAGET_NFC_NF howl[8];
    }IG_PARAGET_NFC_8NF;

    typedef struct{
        IG_PARAGET_NFC_NF howl[16];
    }IG_PARAGET_NFC_16NF;

    typedef struct{
        IG_PARAGET_NFC_NF howl[24];
    }IG_PARAGET_NFC_24NF;

    typedef struct{
        IG_PARAGET_NFC_NF howl[32];
    }IG_PARAGET_NFC_32NF;

    IG_PARACTL_NFC _body;

    CfgSettingNFC *_setting;
};

#endif // CONFIGITEMNFC_H
