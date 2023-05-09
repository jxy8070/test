#ifndef CONFIGITEMSPLIT_H
#define CONFIGITEMSPLIT_H

#include "configitem.h"

struct EqData;
class CfgSettingSplite;
class ConfigItemSplit : public ConfigItem
{
public:
    struct ItemSplitStr {
        struct {
            bool mute;
            double level;
        }Input;
        struct {
            struct {
                bool setSyncL;
                bool setSyncR;
                int filterTypeL;
                int filterTypeR;
                double freqL;
                double freqR;

                bool setSyncLEnable;
                bool setSyncREnable;
                bool filterTypeLEnable;
                bool filterTypeREnable;
                bool freqLEnable;
                bool freqREnable;
            }Filter;
            struct {
                bool mute;
                bool invert;
                double level;
            }Output;
        } Segment[4];
    };

    ConfigItemSplit(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width = 112);
    ~ConfigItemSplit();
    QWidget *getSettingForm() override;
    QByteArray getDetailData() override;
    void setDetailData(unsigned int *d) override;
    void setRtnData(unsigned int *d) override;
    void updateCfgSettingLoadedData() override;
    void loadParaFromSetting(QSettings *ConfigIni) override;
    void saveParaToSetting(QSettings *ConfigIni) override;
    QVariant setLuaPinValue(CLuaPinParam_PC* param, QVariant val) override;

    bool   getFilterSetSyncLEnable(int idx)    {return _split.Segment[idx].Filter.setSyncLEnable;}
    bool   getFilterSetSyncREnable(int idx)    {return _split.Segment[idx].Filter.setSyncREnable;}
    bool   getFilterFilterTypeLEnable(int idx)    {return _split.Segment[idx].Filter.filterTypeLEnable;}
    bool   getFilterFilterTypeREnable(int idx)    {return _split.Segment[idx].Filter.filterTypeREnable;}
    bool   getFilterFreqLEnable(int idx)    {return _split.Segment[idx].Filter.freqLEnable;}
    bool   getFilterFreqREnable(int idx)    {return _split.Segment[idx].Filter.freqREnable;}

    bool   getInputMute()                {return _split.Input.mute;}
    double getInputLevel()               {return _split.Input.level;}
    bool   getFilterSetSyncL(int idx)    {return _split.Segment[idx].Filter.setSyncL;}
    bool   getFilterSetSyncR(int idx)    {return _split.Segment[idx].Filter.setSyncR;}
    int    getFilterFilterTypeL(int idx) {return _split.Segment[idx].Filter.filterTypeL;}
    int    getFilterFilterTypeR(int idx) {return _split.Segment[idx].Filter.filterTypeR;}
    double getFilterFreqL(int idx)       {return _split.Segment[idx].Filter.freqL;}
    double getFilterFreqR(int idx)       {return _split.Segment[idx].Filter.freqR;}
    bool   getOutputMute(int idx)        {return _split.Segment[idx].Output.mute;}
    bool   getOutputInvert(int idx)      {return _split.Segment[idx].Output.invert;}
    double getOutputLevel(int idx)       {return _split.Segment[idx].Output.level;}

    void setInputMute(bool flag);
    void setInputLevel(double value);
    void setFilterSetSyncL(int idx, bool flag);
    void setFilterSetSyncR(int idx, bool flag);

    void setFilterSetSyncLEnable(int idx, bool flag);
    void setFilterSetSyncREnable(int idx, bool flag);
    void setFilterFilterTypeLEnable(int idx, bool flag);
    void setFilterFilterTypeREnable(int idx, bool flag);
    void setFilterFreqLEnable(int idx, bool flag);
    void setFilterFreqREnable(int idx, bool flag);

    void setFilterFilterTypeL(int idx, int value);
    void setFilterFilterTypeR(int idx, int value);
    void setFilterFilterTypeLL(int idx1, int idx2, double value);
    void setFilterFilterTypeLR(int idx1, int idx2, double value);
    void setFilterFilterTypeRR(int idx1, int idx2, double value);
    void setFilterFreqL(int idx, double value);
    void setFilterFreqR(int idx, double value);
    void setFilterFreqLL(int idx1, int idx2, double value);
    void setFilterFreqLR(int idx1, int idx2, double value);
    void setFilterFreqRR(int idx1, int idx2, double value);
    void setOutputMute(int idx, bool flag);
    void setOutputInvert(int idx, bool flag);
    void setOutputLevel(int idx, double value);
private:
    void initData();
    quint16 calNumType(quint16 num, quint16 type);

private:
    struct ItemSplitStr _split;
    enum{
        FCT_FILTER_BUTTERWORTH = 0,
        FCT_FILTER_LINKWITZ = 1,
    };

    typedef struct{
        struct{
            __OPACK32_T3(
                OPACKU,mute,8,
                OPACKU,gain,16,				//gain_db = gain * 0.01 - 100;
                OPACKU,inverse,8
            );
        }in[1];
        struct{
            __OPACK32_T3(
                OPACKU,filterType,8,
                OPACKU,freq_hz,16,			//hz，
                OPACKU,orders,8				//只对lp,hp有效
            );
        }filter[2];
        struct{
            __OPACK32_T3(
                OPACKU,mute,8,
                OPACKU,gain,16,				//gain_db = gain * 0.01 - 100;
                OPACKU,inverse,8
            );
        }out[2];
        unsigned int smooth;				//smooth,每一位对应不同滤波器
    }IG_PARACTL_SPLIT2;

    typedef struct{
        struct{
            __OPACK32_T3(
                OPACKU,mute,8,
                OPACKU,gain,16,				//gain_db = gain * 0.01 - 100;
                OPACKU,inverse,8
            );
        }in[1];
        struct{
            __OPACK32_T3(
                OPACKU,filterType,8,
                OPACKU,freq_hz,16,			//hz，
                OPACKU,orders,8				//只对lp,hp有效
            );
        }filter[4];
        struct{
            __OPACK32_T3(
                OPACKU,mute,8,
                OPACKU,gain,16,				//gain_db = gain * 0.01 - 100;
                OPACKU,inverse,8
            );
        }out[3];
        unsigned int smooth;				//smooth
    }IG_PARACTL_SPLIT3;

    typedef struct{
        struct{
            __OPACK32_T3(
                OPACKU,mute,8,
                OPACKU,gain,16,				//gain_db = gain * 0.01 - 100;
                OPACKU,inverse,8
            );
        }in[1];
        struct{
            __OPACK32_T3(
                OPACKU,filterType,8,
                OPACKU,freq_hz,16,			//hz，
                OPACKU,orders,8				//只对lp,hp有效
            );

        }filter[6];
        struct{
            __OPACK32_T3(
                OPACKU,mute,8,
                OPACKU,gain,16,				//gain_db = gain * 0.01 - 100;
                OPACKU,inverse,8
            );
        }out[4];
        unsigned int smooth;				//smooth
    }IG_PARACTL_SPLIT4;

    IG_PARACTL_SPLIT2 _body_2;
    IG_PARACTL_SPLIT3 _body_3;
    IG_PARACTL_SPLIT4 _body_4;
    CfgSettingSplite *_setting;
    quint32 _smooth;
};
#endif // CONFIGITEMSPLIT_H
