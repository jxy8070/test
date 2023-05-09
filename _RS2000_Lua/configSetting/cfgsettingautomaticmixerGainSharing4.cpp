#include "cfgsettingautomaticmixerGainSharing4.h"
#include "ui_cfgsettingautomaticmixerGainSharing4.h"
#include "configItem/configitemautomaticmixer.h"
#include "device/device_trn.h"
#include "customWidget/volumewidget.h"
#include "configItem/configitem.h"
#include "frame/configview.h"

CfgSettingAutomaticMixerGainSharing4::CfgSettingAutomaticMixerGainSharing4(ConfigItemAutomaticMixer *cfgItem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CfgSettingAutomaticMixerGainSharing4)
{
    _cfgItem = cfgItem;
    ui->setupUi(this);

    initData();
}

CfgSettingAutomaticMixerGainSharing4::~CfgSettingAutomaticMixerGainSharing4()
{
    delete ui;
}

void CfgSettingAutomaticMixerGainSharing4::updateUiData()
{
    ui->Slider_ThreadLevel->setValue(_cfgItem->GetThreadLevel());
    ui->label_Threshold->setText(QString::asprintf("%d dB", ui->Slider_ThreadLevel->value()));
    ui->Slider_AttackTime->setValue(_cfgItem->GetAttackTime());
    ui->label_Attack->setText(QString::asprintf("%d ms", ui->Slider_AttackTime->value()));
    ui->Slider_ReleaseTime->setValue(_cfgItem->GetReleaseTime());
    ui->label_Release->setText(QString::asprintf("%d ms", ui->Slider_ReleaseTime->value()));
    ui->Slider_HoldTime->setValue(_cfgItem->GetHoldTime());
    ui->label_Hold->setText(QString::asprintf("%d ms", ui->Slider_HoldTime->value()));

    ui->PB_Depth->setChecked(_cfgItem->GetDepthEnable());
    ui->Slider_Depth->setValue(_cfgItem->GetDepthValue());
    ui->label_Depth->setText(QString::asprintf("%d dB", ui->Slider_Depth->value()));

    ui->PB_Noise->setChecked(_cfgItem->GetNoiseEnable());
    ui->Slider_Noise->setValue(_cfgItem->GetNoiseValue());
    ui->label_Noise->setText(QString::asprintf("%d dB", ui->Slider_Noise->value()));

    UI_SetInputMute(1);
    UI_SetInputMute(2);
    UI_SetInputMute(3);
    UI_SetInputMute(4);

    UI_SetPreInGain(1);
    UI_SetPreInGain(2);
    UI_SetPreInGain(3);
    UI_SetPreInGain(4);

    ui->PB_MixOutMute->setChecked(_cfgItem->GetMixOutMute());
    ui->Slider_MixOutGain->setValue(_cfgItem->GetMixOutGain());
    ui->label_Output->setText(QString::asprintf("%d dB", ui->Slider_MixOutGain->value()));
}

void CfgSettingAutomaticMixerGainSharing4::initData()
{
    ui->Slider_ThreadLevel->setValue(_cfgItem->GetThreadLevel());
    ui->label_Threshold->setText(QString::asprintf("%d dB", ui->Slider_ThreadLevel->value()));
    ui->Slider_AttackTime->setValue(_cfgItem->GetAttackTime());
    ui->label_Attack->setText(QString::asprintf("%d ms", ui->Slider_AttackTime->value()));
    ui->Slider_ReleaseTime->setValue(_cfgItem->GetReleaseTime());
    ui->label_Release->setText(QString::asprintf("%d ms", ui->Slider_ReleaseTime->value()));
    ui->Slider_HoldTime->setValue(_cfgItem->GetHoldTime());
    ui->label_Hold->setText(QString::asprintf("%d ms", ui->Slider_HoldTime->value()));

    ui->PB_Depth->setChecked(_cfgItem->GetDepthEnable());
    ui->Slider_Depth->setValue(_cfgItem->GetDepthValue());
    ui->label_Depth->setText(QString::asprintf("%d dB", ui->Slider_Depth->value()));

    ui->PB_Noise->setChecked(_cfgItem->GetNoiseEnable());
    ui->Slider_Noise->setValue(_cfgItem->GetNoiseValue());
    ui->label_Noise->setText(QString::asprintf("%d dB", ui->Slider_Noise->value()));

    UI_SetInputMute(1);
    UI_SetInputMute(2);
    UI_SetInputMute(3);
    UI_SetInputMute(4);

    UI_SetPreInGain(1);
    UI_SetPreInGain(2);
    UI_SetPreInGain(3);
    UI_SetPreInGain(4);

    ui->PB_MixOutMute->setChecked(_cfgItem->GetMixOutMute());
    ui->Slider_MixOutGain->setValue(_cfgItem->GetMixOutGain());
    ui->label_Output->setText(QString::asprintf("%d dB", ui->Slider_MixOutGain->value()));

    connect(ui->Slider_ThreadLevel, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_AttackTime, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_ReleaseTime, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_HoldTime, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_Depth, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_Noise, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_PreInGain_1, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_PreInGain_2, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_PreInGain_3, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_PreInGain_4, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_MixOutGain, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));

    connect(ui->PB_Depth, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->PB_Noise, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->PB_InputMute_1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->PB_InputMute_2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->PB_InputMute_3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->PB_InputMute_4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->PB_MixOutMute, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));

    connect(qobject_cast<ConfigView *>(_cfgItem->getConfigSceneBase())->scene()->getTrnDev(), SIGNAL(dispAutomaticMixer(FC_RTU_AGS_METER*)), this, SLOT(dispAutomaticMixer(FC_RTU_AGS_METER*)));
}

void CfgSettingAutomaticMixerGainSharing4::slider_valueChanged(int value)
{
//    QSlider *obj = qobject_cast<QSlider *>(sender());

    if (sender() == ui->Slider_ThreadLevel) {
        _cfgItem->SetThreadLevel(value);
        ui->label_Threshold->setText(QString::asprintf("%d dB", ui->Slider_ThreadLevel->value()));
    }
    else if (sender() == ui->Slider_AttackTime) {
        _cfgItem->SetAttackTime(value);
        ui->label_Attack->setText(QString::asprintf("%d ms", ui->Slider_AttackTime->value()));
    }
    else if (sender() == ui->Slider_ReleaseTime) {
        _cfgItem->SetReleaseTime(value);
        ui->label_Release->setText(QString::asprintf("%d ms", ui->Slider_ReleaseTime->value()));
    }
    else if (sender() == ui->Slider_HoldTime) {
        _cfgItem->SetHoldTime(value);
        ui->label_Hold->setText(QString::asprintf("%d ms", ui->Slider_HoldTime->value()));
    }
    else if (sender() == ui->Slider_Depth) {
        _cfgItem->SetDepthValue(value);
        ui->label_Depth->setText(QString::asprintf("%d dB", ui->Slider_Depth->value()));
    }
    else if (sender() == ui->Slider_Noise) {
        _cfgItem->SetNoiseValue(value);
        ui->label_Noise->setText(QString::asprintf("%d dB", ui->Slider_Noise->value()));
    }
    else if (sender() == ui->Slider_PreInGain_1) {
        _cfgItem->SetPreInGain(ConfigItemAutomaticMixer::INPUT_CH1, value);
        ui->label_input1->setText(QString::asprintf("%d dB", ui->Slider_PreInGain_1->value()));
    }
    else if (sender() == ui->Slider_PreInGain_2) {
        _cfgItem->SetPreInGain(ConfigItemAutomaticMixer::INPUT_CH2, value);
        ui->label_input2->setText(QString::asprintf("%d dB", ui->Slider_PreInGain_2->value()));
    }
    else if (sender() == ui->Slider_PreInGain_3) {
        _cfgItem->SetPreInGain(ConfigItemAutomaticMixer::INPUT_CH3, value);
        ui->label_input3->setText(QString::asprintf("%d dB", ui->Slider_PreInGain_3->value()));
    }
    else if (sender() == ui->Slider_PreInGain_4) {
        _cfgItem->SetPreInGain(ConfigItemAutomaticMixer::INPUT_CH4, value);
        ui->label_input4->setText(QString::asprintf("%d dB", ui->Slider_PreInGain_4->value()));
    }
    else if (sender() == ui->Slider_MixOutGain) {
        _cfgItem->SetMixOutGain(value);
        ui->label_Output->setText(QString::asprintf("%d dB", ui->Slider_MixOutGain->value()));
    }
}

void CfgSettingAutomaticMixerGainSharing4::pb_clicked(bool enable)
{
//    QPushButton *obj = qobject_cast<QPushButton *>(sender());
    if (sender() == ui->PB_Depth) {
        _cfgItem->SetDepthEnable(enable);
    }
    else if (sender() == ui->PB_Noise) {
        _cfgItem->SetNoiseEnable(enable);
    }
    else if (sender() == ui->PB_InputMute_1) {
        _cfgItem->SetInputMute(ConfigItemAutomaticMixer::INPUT_CH1, enable);
    }
    else if (sender() == ui->PB_InputMute_2) {
        _cfgItem->SetInputMute(ConfigItemAutomaticMixer::INPUT_CH2, enable);
    }
    else if (sender() == ui->PB_InputMute_3) {
        _cfgItem->SetInputMute(ConfigItemAutomaticMixer::INPUT_CH3, enable);
    }
    else if (sender() == ui->PB_InputMute_4) {
        _cfgItem->SetInputMute(ConfigItemAutomaticMixer::INPUT_CH4, enable);
    }
    else if (sender() == ui->PB_MixOutMute) {
        _cfgItem->SetMixOutMute(enable);
    }
}

void CfgSettingAutomaticMixerGainSharing4::dispAutomaticMixer(FC_RTU_AGS_METER *level)
{
    for (int i = 0; i < AUTOMIX_MAXCHANNEL; ++i)
    {
        if (!level[i].effect) return;
        if (level[i].modeUnitNum == _cfgItem->getModuleNum())
        {
            double vu = level[i].level * 0.01 - 100;

            switch (level[i].channelNum) {
            case 0:
                ui->Disp_Level_1->setLCurValue(vu);
                break;
            case 1:
                ui->Disp_Level_2->setLCurValue(vu);
                break;
            case 2:
                ui->Disp_Level_3->setLCurValue(vu);
                break;
            case 3:
                ui->Disp_Level_4->setLCurValue(vu);
                break;
            default:
                break;
            }
        }
    }
}
