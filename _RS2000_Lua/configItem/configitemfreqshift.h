#ifndef CONFIGITEMFREQSHIFT_H
#define CONFIGITEMFREQSHIFT_H
#include "configitem.h"

class CfgSettingFreqShift;
class ConfigItemFreqShift : public ConfigItem
{
public:
    enum{
      DEV_FREQSHIFTP_WM_BYPASS = 0,        //直通
      DEV_FREQSHIFTP_WM_FIXFREQ,          //固定频移
      DEV_FREQSHIFTP_WM_VARIABLEFREQ_LINE,    //线性变频频移
      DEV_FREQSHIFTP_WM_VARIABLEFREQ_RANDOM,    //随机变频频移
    };

    ConfigItemFreqShift(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemFreqShift();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

public:
    uint8_t GetFreq(void);
    int8_t GetGain(void);
    int16_t GetPhase(void);
    uint32_t GetFreqMode(void);
    uint32_t GetFreqSpan(void);
    uint32_t GetFreqSpeed(void);

    void SetFreq(uint8_t value);
    void SetGain(int8_t value);
    void SetPhase(int16_t value);
    void SetFreqMode(uint32_t value);
    void SetFreqSpan(uint32_t value);
    void SetFreqSpeed(uint32_t value);

private:
    typedef struct {
        __OPACK32_T2(
            OPACKU,shiftfreq_hz,16,				//hz，
            OPACKU,gain,16						//gain_db = gain * 0.01 - 100;
        );
    } IG_PARACTL_FREQSHIFT;

    typedef struct{
      __OPACK32_T2(
        OPACKU,shiftfreq_hz,16,        //hz，
        OPACKU,phase,16            //[0,360]realphase = phase - 180;
      );
      __OPACK32_T1S0(
        OPACKU,gain,16            //gain_db = gain * 0.01 - 100;
      );
      __OPACK32_T3(
          OPACKU,shiftfreqSpan_hz,16,      //hz，[0,2*shiftfreq_hz]
          OPACKU,shiftfreqSpeed_hz,8,      //hz[1,100];
          OPACKU,shiftfreqMode,8        //hz[1,100];
        );
    }IG_PARACTL_FREQSHIFTP;

    IG_PARACTL_FREQSHIFT _body;

    IG_PARACTL_FREQSHIFTP _body_plus;

    CfgSettingFreqShift *_setting;
};

#endif // CONFIGITEMFREQSHIFT_H
