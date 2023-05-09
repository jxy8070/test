#ifndef CFGSETTINGAUTOMATICMIXERGAINSHARING8_H
#define CFGSETTINGAUTOMATICMIXERGAINSHARING8_H

#include <QWidget>
#include "protocal/packet.h"

namespace Ui {
class CfgSettingAutomaticMixerGainSharing8;
}

class ConfigItemAutomaticMixer;
class CfgSettingAutomaticMixerGainSharing8 : public QWidget
{
    Q_OBJECT

public:
    explicit CfgSettingAutomaticMixerGainSharing8(ConfigItemAutomaticMixer *cfgItem, QWidget *parent = 0);
    ~CfgSettingAutomaticMixerGainSharing8();

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

    Ui::CfgSettingAutomaticMixerGainSharing8 *ui;
};

#endif // CFGSETTINGAUTOMATICMIXERGAINSHARING8_H
