#ifndef CFGSETTINGAUTOMATICMIXERGAINSHARING_H
#define CFGSETTINGAUTOMATICMIXERGAINSHARING_H

#include "protocal/packet.h"

namespace Ui {
class CfgSettingAutomaticMixerGainSharing;
}

class ConfigItemAutomaticMixer;
class ConfigGainSharingChannel;
class CfgSettingAutomaticMixerGainSharing : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingAutomaticMixerGainSharing(int channels, ConfigItemAutomaticMixer *cfgItem, QWidget *parent = 0);
    ~CfgSettingAutomaticMixerGainSharing();

    void updateUiData();
    void DispRtnData(CfgType type, unsigned int *d);

private:
//#define UI_SetInputMute(x) ui->PB_InputMute_##x##->setChecked(_cfgItem->GetInputMute(ConfigItemAutomaticMixer::INPUT_CH##x##))
//#define UI_SetPreInGain(x) \
//    ui->Slider_PreInGain_##x##->setValue(_cfgItem->GetPreInGain(ConfigItemAutomaticMixer::INPUT_CH##x##));\
//    ui->label_input##x##->setText(QString::asprintf("%d dB", ui->Slider_PreInGain_##x##->value()));
    void initData(void);

private slots:
    void slider_valueChanged(int value);
    void pb_clicked(bool enable);

private:
    #define IG_PARAGET_AUTOMIXGS_DEF(ch) 	\
    typedef struct{							\
        struct{								\
            __OPACK32_T2(					\
                OPACKU,gain1,16,			\
                OPACKU,gain2,16				\
            );								\
        }appliedGain[ch>>1];				\
    }
//    IG_PARAGET_AUTOMIXGS_DEF(4)		IG_PARAGET_AUTOMIXGS4;
//    IG_PARAGET_AUTOMIXGS_DEF(8)		IG_PARAGET_AUTOMIXGS8;
//    IG_PARAGET_AUTOMIXGS_DEF(16)	IG_PARAGET_AUTOMIXGS16;
    IG_PARAGET_AUTOMIXGS_DEF(32)	IG_PARAGET_AUTOMIXGS4_32;

    Ui::CfgSettingAutomaticMixerGainSharing *ui;
    ConfigItemAutomaticMixer *_cfgItem;
    int _maxchannel;
    QList<ConfigGainSharingChannel *> _channel;
};

#endif // CFGSETTINGAUTOMATICMIXERGAINSHARING_H
