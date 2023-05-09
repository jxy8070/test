#ifndef CFGSETTINGAUTOMATICMIXERGATINGRELATIVETHRESHOLD_H
#define CFGSETTINGAUTOMATICMIXERGATINGRELATIVETHRESHOLD_H


namespace Ui {
class CfgSettingAutomaticMixerGatingRelativeThreshold;
}
class ConfigItemAutomaticMixer;
class ConfigGatingRelativeThreasholdChannel;
class CfgSettingAutomaticMixerGatingRelativeThreshold : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingAutomaticMixerGatingRelativeThreshold(int channels, ConfigItemAutomaticMixer *cfgItem, QWidget *parent = 0);
    ~CfgSettingAutomaticMixerGatingRelativeThreshold();
    void updateUiData();
    void DispRtnData(CfgType type, unsigned int *d);

private slots:
    void slider_valueChanged(int value);
    void pushbutton_clicked(bool checked);
    void Default_clicked(bool checked);
    void Manual_clicked(bool checked);
    void PostGateMute_clicked(bool checked);

private:
    #define IG_PARAGET_AUTOMIXNGRT_DEF(ch) 	\
    typedef struct{								\
        __OPACK32_T2S0S1(						\
            OPACKU,passNom,8,					\
            OPACKU,attenGain,16					\
        );										\
        struct{									\
            __OPACK32_T2S0S1(					\
                OPACKU,openState,8,				\
                OPACKU,sigAboveNoise,16			\
            );									\
        }chMixState[ch];							\
    }
//    IG_PARAGET_AUTOMIXNGRT_DEF(4)	IG_PARAGET_AUTOMIXNGRT4;
//    IG_PARAGET_AUTOMIXNGRT_DEF(8)	IG_PARAGET_AUTOMIXNGRT8;
//    IG_PARAGET_AUTOMIXNGRT_DEF(16)	IG_PARAGET_AUTOMIXNGRT16;
    IG_PARAGET_AUTOMIXNGRT_DEF(32)	IG_PARAGET_AUTOMIXNGRT4_32;
    void initData(void);

    ConfigItemAutomaticMixer *_cfgItem;
    int _maxchannel;
    Ui::CfgSettingAutomaticMixerGatingRelativeThreshold *ui;
    QList<ConfigGatingRelativeThreasholdChannel *> _channel;
};

#endif // CFGSETTINGAUTOMATICMIXERGATINGRELATIVETHRESHOLD_H
