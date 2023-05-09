#ifndef CFGSETTINGAUTOMATICMIXERGAINSHARING4_H
#define CFGSETTINGAUTOMATICMIXERGAINSHARING4_H

#include <QWidget>
#include "protocal/packet.h"

namespace Ui {
class CfgSettingAutomaticMixerGainSharing4;
}

class ConfigItemAutomaticMixer;
class CfgSettingAutomaticMixerGainSharing4 : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingAutomaticMixerGainSharing4(ConfigItemAutomaticMixer *cfgItem, QWidget *parent = 0);
    ~CfgSettingAutomaticMixerGainSharing4();

    void updateUiData();
private:
#define UI_SetInputMute(x) ui->PB_InputMute_##x##->setChecked(_cfgItem->GetInputMute(ConfigItemAutomaticMixer::INPUT_CH##x##))
#define UI_SetPreInGain(x) \
    ui->Slider_PreInGain_##x##->setValue(_cfgItem->GetPreInGain(ConfigItemAutomaticMixer::INPUT_CH##x##));\
    ui->label_input##x##->setText(QString::asprintf("%d dB", ui->Slider_PreInGain_##x##->value()));
    void initData(void);

private slots:
    void slider_valueChanged(int value);
    void pb_clicked(bool enable);
    void dispAutomaticMixer(FC_RTU_AGS_METER *level);

private:
    ConfigItemAutomaticMixer *_cfgItem;

    Ui::CfgSettingAutomaticMixerGainSharing4 *ui;
};

#endif // CFGSETTINGAUTOMATICMIXERGAINSHARING4_H
