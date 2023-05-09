#ifndef CONFIGITEMEQ_H
#define CONFIGITEMEQ_H

#include "configitem.h"

struct EqData;
class CfgSettingEq;
class ConfigItemEq : public ConfigItem
{
public:
    enum{
        FCT_PEQ_PEAK = 0,		//通用滤波器
        FCT_PEQ_HPASS = 1,		//低通
        FCT_PEQ_LPASS = 2,		//高通
        FCT_PEQ_HSHELF = 3,		//高架
        FCT_PEQ_LSHELF = 4,		//低架
    };
    ConfigItemEq(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemEq();

    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    void setEqData(quint8 idx, bool seg_bypass, int freq, int gain, int q, int type);
    void setEqData(quint8 idx, int gain);
    void setAllBypass(bool bypass);
    void setSegBypass(quint32 idx, bool seg_bypass);

    QList<EqData *> getEqDataList(){return _eqList;}
    bool getAllBypass(){return _segBypass & 0x80000000;}
    bool getSegBypass(uint32_t idx) { return _segBypass & (0x1 << idx); }
    void reinit(void);

private:
    int _eqCount;
    QList<int> _freqList;
    void initEq();

    typedef struct{
        __OPACK32_T2(
            OPACKU,freq_hz,16,			//hz
            OPACKU,gain,16				//gain_db = gain * 0.01 - 100;
        );
        __OPACK32_T2S0S1(
            OPACKU,q,16,				//q = (float)q * 0.01;
            OPACKU,eq_type,4			//eq类型
        );
    } IG_PARACTL_PEQ;

    typedef struct{
        __OPACK32_T1S0(
            OPACKU,gain,16				//gain_db = gain * 0.01 - 100;
        );
    }IG_PARACTL_GEQ;

    CfgSettingEq *_setting;
    IG_PARACTL_PEQ _body_peq[16];
    IG_PARACTL_GEQ _body_geq[31];
    double _qValue;
    quint32 _smooth;                    //smooth
    quint32 _segBypass;
    QList<EqData *> _eqList;

//    typedef struct{
//        struct{
//            __OPACK32_T2(
//                OPACKU,freq_hz,16,			//hz
//                OPACKU,gain,16				//gain_db = gain * 0.01 - 100;
//            );
//            __OPACK32_T2S0S1(
//                OPACKU,q,16,					//q = (float)q * 0.01;
//                OPACKU,eq_type,4				//eq类型
//            );
//        }eqp[1];
//        unsigned int bypass;				//bypass
//        unsigned int smooth;				//smooth
//    }IG_PARACTL_PEQ1;
//    typedef struct{
//        struct{
//            __OPACK32_T2(
//                OPACKU,freq_hz,16,			//hz
//                OPACKU,gain,16				//gain_db = gain * 0.01 - 100;
//            );
//            __OPACK32_T2S0S1(
//                OPACKU,q,16,					//q = (float)q * 0.01;
//                OPACKU,eq_type,4				//eq类型
//            );
//        }eqp[3];
//        unsigned int bypass;				//bypass
//        unsigned int smooth;				//smooth
//    }IG_PARACTL_PEQ3;
//    typedef struct{
//        struct{
//            __OPACK32_T2(
//                OPACKU,freq_hz,16,			//hz
//                OPACKU,gain,16				//gain_db = gain * 0.01 - 100;
//            );
//            __OPACK32_T2S0S1(
//                OPACKU,q,16,					//q = (float)q * 0.01;
//                OPACKU,eq_type,4				//eq类型
//            );
//        }eqp[5];
//        unsigned int bypass;				//bypass
//        unsigned int smooth;				//smooth
//    }IG_PARACTL_PEQ5;
//    typedef struct{
//        struct{
//            __OPACK32_T2(
//                OPACKU,freq_hz,16,			//hz
//                OPACKU,gain,16				//gain_db = gain * 0.01 - 100;
//            );
//            __OPACK32_T2S0S1(
//                OPACKU,q,16,					//q = (float)q * 0.01;
//                OPACKU,eq_type,4				//eq类型
//            );
//        }eqp[7];
//        unsigned int bypass;				//bypass
//        unsigned int smooth;				//smooth
//    }IG_PARACTL_PEQ7;
//    typedef struct{
//        struct{
//            __OPACK32_T2(
//                OPACKU,freq_hz,16,			//hz
//                OPACKU,gain,16				//gain_db = gain * 0.01 - 100;
//            );
//            __OPACK32_T2S0S1(
//                OPACKU,q,16,					//q = (float)q * 0.01;
//                OPACKU,eq_type,4				//eq类型
//            );
//        }eqp[10];
//        unsigned int bypass;				//bypass
//        unsigned int smooth;				//smooth
//    }IG_PARACTL_PEQ10;
//    typedef struct{
//        struct{
//            __OPACK32_T2(
//                OPACKU,freq_hz,16,			//hz
//                OPACKU,gain,16				//gain_db = gain * 0.01 - 100;
//            );
//            __OPACK32_T2S0S1(
//                OPACKU,q,16,					//q = (float)q * 0.01;
//                OPACKU,eq_type,4				//eq类型
//            );
//        }eqp[16];
//        unsigned int bypass;				//bypass
//        unsigned int smooth;				//smooth
//    }IG_PARACTL_PEQ16;
//    typedef struct{
//        struct{
//            __OPACK32_T1S0(
//                OPACKU,gain,16				//gain_db = gain * 0.01 - 100;
//            );
//        }eqp[31];
//        unsigned int bypass;				//bypass
//        unsigned int smooth;				//smooth
//    }IG_PARACTL_GEQ31_SHG1P3;
//    typedef struct{
//        struct{
//            __OPACK32_T1S0(
//                OPACKU,gain,16				//gain_db = gain * 0.01 - 100;
//            );
//        }eqp[15];
//        unsigned int bypass;				//bypass
//        unsigned int smooth;				//smooth
//    }IG_PARACTL_GEQ15_SHG2P3;
//    typedef struct{
//        struct{
//            __OPACK32_T1S0(
//                OPACKU,gain,16				//gain_db = gain * 0.01 - 100;
//            );
//        }eqp[10];
//        unsigned int bypass;				//bypass
//        unsigned int smooth;				//smooth
//    }IG_PARACTL_GEQ10_SHG1;

//    IG_PARACTL_PEQ1 _body_peq1;
//    IG_PARACTL_PEQ3 _body_peq3;
//    IG_PARACTL_PEQ5 _body_peq5;
//    IG_PARACTL_PEQ7 _body_peq7;
//    IG_PARACTL_PEQ10 _body_peq10;
//    IG_PARACTL_PEQ16 _body_peq16;
//    IG_PARACTL_GEQ31_SHG1P3 _body_geq31;
//    IG_PARACTL_GEQ15_SHG2P3 _body_geq15;
//    IG_PARACTL_GEQ10_SHG1 _body_geq10;

};
#endif // CONFIGITEMEQ_H
