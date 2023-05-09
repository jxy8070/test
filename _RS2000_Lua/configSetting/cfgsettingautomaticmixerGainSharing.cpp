#include "cfgsettingautomaticmixerGainSharing.h"
#include "ui_cfgsettingautomaticmixerGainSharing.h"
#include "configItem/configitemautomaticmixer.h"
#include "device/device_trn.h"
#include "customWidget/volumewidget.h"
#include "configItem/configitem.h"
#include "frame/configview.h"
#include "customWidget/configgainsharingchannel.h"

CfgSettingAutomaticMixerGainSharing::CfgSettingAutomaticMixerGainSharing(int channels, ConfigItemAutomaticMixer *cfgItem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CfgSettingAutomaticMixerGainSharing)
{
    ui->setupUi(this);

    _channel.append(ui->widget_1);
    _channel.append(ui->widget_2);
    _channel.append(ui->widget_3);
    _channel.append(ui->widget_4);
    _channel.append(ui->widget_5);
    _channel.append(ui->widget_6);
    _channel.append(ui->widget_7);
    _channel.append(ui->widget_8);
    _channel.append(ui->widget_9);
    _channel.append(ui->widget_10);
    _channel.append(ui->widget_11);
    _channel.append(ui->widget_12);
    _channel.append(ui->widget_13);
    _channel.append(ui->widget_14);
    _channel.append(ui->widget_15);
    _channel.append(ui->widget_16);
    _channel.append(ui->widget_17);
    _channel.append(ui->widget_18);
    _channel.append(ui->widget_19);
    _channel.append(ui->widget_20);
    _channel.append(ui->widget_21);
    _channel.append(ui->widget_22);
    _channel.append(ui->widget_23);
    _channel.append(ui->widget_24);
    _channel.append(ui->widget_25);
    _channel.append(ui->widget_26);
    _channel.append(ui->widget_27);
    _channel.append(ui->widget_28);
    _channel.append(ui->widget_29);
    _channel.append(ui->widget_30);
    _channel.append(ui->widget_31);
    _channel.append(ui->widget_32);

    for (int i = 0; i < 32; ++i) {
        _channel.at(i)->setTitle(QString::number(i+1));
        if (i >= channels) {
            _channel.at(i)->setVisible(false);
        }
    }

    _cfgItem = cfgItem;
    _maxchannel = channels;
    initData();
}

CfgSettingAutomaticMixerGainSharing::~CfgSettingAutomaticMixerGainSharing()
{
    delete ui;
}

void CfgSettingAutomaticMixerGainSharing::updateUiData()
{
    ui->Slider_ThreadLevel->setValue(_cfgItem->GetThreadLevel());
//    ui->label_Threshold->setText(QString::asprintf("%d dB", ui->Slider_ThreadLevel->value()));
    ui->Slider_AttackTime->setValue(_cfgItem->GetAttackTime());
//    ui->label_Attack->setText(QString::asprintf("%d ms", ui->Slider_AttackTime->value()));
    ui->Slider_ReleaseTime->setValue(_cfgItem->GetReleaseTime());
//    ui->label_Release->setText(QString::asprintf("%d ms", ui->Slider_ReleaseTime->value()));
    ui->Slider_HoldTime->setValue(_cfgItem->GetHoldTime());
//    ui->label_Hold->setText(QString::asprintf("%d ms", ui->Slider_HoldTime->value()));
    ui->Slider_DetectorTime->setValue(_cfgItem->GetDetectorTime());
//    ui->label_DetectorTime->setText(QString::asprintf("%d ms", ui->Slider_DetectorTime->value()));
    ui->Slider_Depth->setValue(_cfgItem->GetDepthValue());
//    ui->label_Depth->setText(QString::asprintf("%d dB", ui->Slider_Depth->value()));

    ui->Slider_Noise->setValue(_cfgItem->GetNoiseValue());
//    ui->label_Noise->setText(QString::asprintf("%d dB", ui->Slider_Noise->value()));

    for (int i = 0; i < _maxchannel; ++i) {
        _channel.at(i)->setPreInMute(_cfgItem->GetInputMute(i));
        _channel.at(i)->setPreInGain(_cfgItem->GetPreInGain(i));
    }

    ui->PB_MixOutMute->setChecked(_cfgItem->GetMixOutMute());
    ui->Slider_MixOutGain->setValue(_cfgItem->GetMixOutGain());
//    ui->label_Output->setText(QString::asprintf("%d dB", ui->Slider_MixOutGain->value()));
}

void CfgSettingAutomaticMixerGainSharing::DispRtnData(CfgType type, unsigned int *d)
{
    switch (type) {
    case CFG_TYPE_AUTOMIX_GS_4:
    case CFG_TYPE_AUTOMIX_GS_8:
    case CFG_TYPE_AUTOMIX_GS_16:
    case CFG_TYPE_AUTOMIX_GS_32:{
            IG_PARAGET_AUTOMIXGS4_32 *p = (IG_PARAGET_AUTOMIXGS4_32 *)d;
            for (int i = 0; i < (_maxchannel >> 1); ++i) {
                int vu1 = p->appliedGain[i].gain1*0.01-100;
                int vu2 = p->appliedGain[i].gain2*0.01-100;
                _channel.at(2*i)->setVu(vu1);
                _channel.at(2*i+1)->setVu(vu2);
            }
        }
        break;
    default:
        break;
    }
//    for (int i = 0; i < AUTOMIX_MAXCHANNEL; ++i)
//    {
//        if (!level[i].effect) return;
//        if (level[i].modeUnitNum == _cfgItem->getModuleNum())
//        {
//            double vu = level[i].level * 0.01 - 100;

//            _channel.at(level[i].channelNum)->setVu(vu);
//        }
//    }
}

void CfgSettingAutomaticMixerGainSharing::initData()
{
    ui->Slider_ThreadLevel->setValue(_cfgItem->GetThreadLevel());
//    ui->label_Threshold->setText(QString::asprintf("%d dB", ui->Slider_ThreadLevel->value()));
    ui->Slider_AttackTime->setValue(_cfgItem->GetAttackTime());
//    ui->label_Attack->setText(QString::asprintf("%d ms", ui->Slider_AttackTime->value()));
    ui->Slider_ReleaseTime->setValue(_cfgItem->GetReleaseTime());
//    ui->label_Release->setText(QString::asprintf("%d ms", ui->Slider_ReleaseTime->value()));
    ui->Slider_HoldTime->setValue(_cfgItem->GetHoldTime());
//    ui->label_Hold->setText(QString::asprintf("%d ms", ui->Slider_HoldTime->value()));
    ui->Slider_DetectorTime->setValue(_cfgItem->GetDetectorTime());
//    ui->label_DetectorTime->setText(QString::asprintf("%d ms", ui->Slider_DetectorTime->value()));
    ui->Slider_Depth->setValue(_cfgItem->GetDepthValue());
//    ui->label_Depth->setText(QString::asprintf("%d dB", ui->Slider_Depth->value()));
    ui->Slider_Noise->setValue(_cfgItem->GetNoiseValue());
//    ui->label_Noise->setText(QString::asprintf("%d dB", ui->Slider_Noise->value()));
    connect(ui->Slider_ThreadLevel, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_AttackTime, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_ReleaseTime, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_HoldTime, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_DetectorTime, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_Depth, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->Slider_Noise, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));

    for (int i = 0; i < _maxchannel; ++i) {
        _channel.at(i)->setPreInMute(_cfgItem->GetInputMute(i));
        _channel.at(i)->setPreInGain(_cfgItem->GetPreInGain(i));

        connect(_channel.at(i), SIGNAL(PreInGainValueChanged(int)), this, SLOT(slider_valueChanged(int)));
        connect(_channel.at(i), SIGNAL(PreInMuteClicked(bool)), this, SLOT(pb_clicked(bool)));
    }
    ui->PB_MixOutMute->setProperty("form", "Mute");
    ui->PB_MixOutMute->setChecked(_cfgItem->GetMixOutMute());
    ui->Slider_MixOutGain->setValue(_cfgItem->GetMixOutGain());
//    ui->label_Output->setText(QString::asprintf("%d dB", ui->Slider_MixOutGain->value()));

    connect(ui->Slider_MixOutGain, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->PB_MixOutMute, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
}



void CfgSettingAutomaticMixerGainSharing::slider_valueChanged(int value)
{
    if (sender() == ui->Slider_ThreadLevel) {
        _cfgItem->SetThreadLevel(value);
//        ui->label_Threshold->setText(QString::asprintf("%d dB", ui->Slider_ThreadLevel->value()));
    }
    else if (sender() == ui->Slider_AttackTime) {
        _cfgItem->SetAttackTime(value);
//        ui->label_Attack->setText(QString::asprintf("%d ms", ui->Slider_AttackTime->value()));
    }
    else if (sender() == ui->Slider_ReleaseTime) {
        _cfgItem->SetReleaseTime(value);
//        ui->label_Release->setText(QString::asprintf("%d ms", ui->Slider_ReleaseTime->value()));
    }
    else if (sender() == ui->Slider_HoldTime) {
        _cfgItem->SetHoldTime(value);
//        ui->label_Hold->setText(QString::asprintf("%d ms", ui->Slider_HoldTime->value()));
    }
    else if (sender() == ui->Slider_DetectorTime) {
        _cfgItem->SetDetectorTime(value);
//        ui->label_DetectorTime->setText(QString::asprintf("%d ms", ui->Slider_DetectorTime->value()));
    }
    else if (sender() == ui->Slider_Depth) {
        _cfgItem->SetDepthValue(value);
//        ui->label_Depth->setText(QString::asprintf("%d dB", ui->Slider_Depth->value()));
    }
    else if (sender() == ui->Slider_Noise) {
        _cfgItem->SetNoiseValue(value);
//        ui->label_Noise->setText(QString::asprintf("%d dB", ui->Slider_Noise->value()));
    }
    else if (sender() == ui->Slider_MixOutGain) {
        _cfgItem->SetMixOutGain(value);
//        ui->label_Output->setText(QString::asprintf("%d dB", ui->Slider_MixOutGain->value()));
    }
    else {
        for (int i = 0; i < _maxchannel; ++i) {
            if (sender() == _channel.at(i)) {
                _cfgItem->SetPreInGain(i, value);
                break;
            }
        }
    }
}

void CfgSettingAutomaticMixerGainSharing::pb_clicked(bool enable)
{
    if (sender() == ui->PB_MixOutMute) {
        _cfgItem->SetMixOutMute(enable);
    }
    else {
        for (int i = 0; i < _maxchannel; ++i) {
            if (sender() == _channel.at(i)) {
                _cfgItem->SetInputMute(i, enable);
                break;
            }
        }
    }
}
