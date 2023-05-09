#ifndef CFGSETTINGAUTOMATICMIXERGATINGABSOLUTETHRESHOLD_H
#define CFGSETTINGAUTOMATICMIXERGATINGABSOLUTETHRESHOLD_H


namespace Ui {
class CfgSettingAutomaticMixerGatingAbsoluteThreshold;
class CfgSettingAutomaticMixerGatingAbsoluteThreshold_FangTu;
}

class ConfigItemAutomaticMixer;
class ConfigGatingAbsoluteThreasholdChannel;
class CfgSettingAutomaticMixerGatingAbsoluteThreshold : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingAutomaticMixerGatingAbsoluteThreshold(int channels, ConfigItemAutomaticMixer *cfgItem, QWidget *parent = 0);
    ~CfgSettingAutomaticMixerGatingAbsoluteThreshold();
    void updateUiData();
    void DispRtnData(CfgType type, unsigned int *d);

private slots:
    void slider_valueChanged(int value);
    void pushbutton_clicked(bool checked);
    void Manual_Clicked(bool checked);
    void Mode_ValueChanged(int value);
    void Priority_ValueChanged(int value);
    void Threshold_ValueChanged(int value);

private:
    #define IG_PARAGET_AUTOMIXNGAT_DEF(x32) 	\
    typedef struct{								\
        __OPACK32_T2S0S1(						\
            OPACKU,passNom,8,					\
            OPACKU,attenGain,16					\
        );										\
        unsigned int openstate[x32];			\
    }
    IG_PARAGET_AUTOMIXNGAT_DEF(1)	IG_PARAGET_AUTOMIXNGAT4_32;
//    IG_PARAGET_AUTOMIXNGAT_DEF(1)	IG_PARAGET_AUTOMIXNGAT8;
//    IG_PARAGET_AUTOMIXNGAT_DEF(1)	IG_PARAGET_AUTOMIXNGAT16;
//    IG_PARAGET_AUTOMIXNGAT_DEF(1)	IG_PARAGET_AUTOMIXNGAT32;

    void initUI(int channels);
    void initUI_FangTu(int channels);

    template<typename UI_t>
    void initData(UI_t *ui);
    Ui::CfgSettingAutomaticMixerGatingAbsoluteThreshold_FangTu *_ui_FangTu = NULL;
    Ui::CfgSettingAutomaticMixerGatingAbsoluteThreshold *_ui = NULL;
    ConfigItemAutomaticMixer *_cfgItem;
    int _maxchannel;
    QList<ConfigGatingAbsoluteThreasholdChannel *> _channel;
};

#endif // CFGSETTINGAUTOMATICMIXERGATINGABSOLUTETHRESHOLD_H
