#include "cfgsettingautomaticmixergatingrelativethreshold.h"
#include "ui_cfgsettingautomaticmixergatingrelativethreshold.h"
#include "configItem/configitemautomaticmixer.h"

CfgSettingAutomaticMixerGatingRelativeThreshold::CfgSettingAutomaticMixerGatingRelativeThreshold(int channels, ConfigItemAutomaticMixer *cfgItem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CfgSettingAutomaticMixerGatingRelativeThreshold)
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

    _maxchannel = channels;
    _cfgItem = cfgItem;
    ui->spinBox_MaxNOM->setRange(1, channels);
    initData();
}

CfgSettingAutomaticMixerGatingRelativeThreshold::~CfgSettingAutomaticMixerGatingRelativeThreshold()
{
    delete ui;
}

void CfgSettingAutomaticMixerGatingRelativeThreshold::updateUiData()
{
    uint8_t lastMicOn = _cfgItem->GetLastMicOn();
    uint8_t defaultCh = _cfgItem->GetDefaultCh();
    qDebug() << __FUNCTION__ << __LINE__ << lastMicOn << defaultCh;
    if (lastMicOn == 1) {
        ui->pb_LastMicOn->setChecked(true);
        for(int i = 0; i < _maxchannel; ++i)
            _channel.at(i)->setDefault(false);
    }
    else if (lastMicOn == 2) {
        ui->pb_LastMicOn->setChecked(false);

        for(int i = 0; i < _maxchannel; ++i)
        {
            if(i == defaultCh) { _channel.at(defaultCh)->setDefault(true); }
            else { _channel.at(i)->setDefault(false); }
        }
    }

    ui->dial_ThresholdLevelAboveNoise->setValue(_cfgItem->GetThresholdLevelAboveNoise());
//    ui->label_ThresholdLevelAboveNoise->setText(QString::asprintf("%d dB", _cfgItem->GetThresholdLevelAboveNoise()));
    ui->dial_Depth->setValue(_cfgItem->GetDepthValue());
//    ui->label_Depth->setText(QString::asprintf("%d dB", _cfgItem->GetDepthValue()));
    ui->dial_HoldTime->setValue(_cfgItem->GetHoldTime());
//    ui->label_HoldTime->setText(QString::asprintf("%d ms", _cfgItem->GetHoldTime()));
    ui->spinBox_MaxNOM->setValue(_cfgItem->GetMaxMON());
    ui->pushButton_DirectOutsNOMAtten->setChecked(_cfgItem->GetDirectOutsNOMAtten());
    ui->comboBox_NOMAttenuationType->setCurrentIndex(_cfgItem->GetNOMAttenuationType());
    ui->dial_NOMAttenuationStep->setValue(_cfgItem->GetNOMAttenuationStep());
//    ui->label_NOMAttenuationStep->setText(QString::asprintf("%d dB", _cfgItem->GetNOMAttenuationStep()));
    ui->dial_MAXNOMAttenuation->setValue(_cfgItem->GetMaxNOMAttenuation());
//    ui->label_MAXNOMAttenuation->setText(QString::asprintf("%d dB", _cfgItem->GetMaxNOMAttenuation()));
    ui->dial_ResponseTime->setValue(_cfgItem->GetResponseTime());
//    ui->label_ResponseTime->setText(QString::asprintf("%d ms", _cfgItem->GetResponseTime()));
    ui->pushButton_Mute->setChecked(_cfgItem->GetMixOutMute());
    ui->verticalSlider_Gain->setValue(_cfgItem->GetMixOutGain());
    ui->label_Gain->setText(QString::asprintf("%d dB", _cfgItem->GetMixOutGain()));

    for (int i = 0; i < _maxchannel; ++i) {
        _channel.at(i)->setManual(_cfgItem->GetChManual(i));
        _channel.at(i)->setPostGateMute(_cfgItem->GetChPostGateMute(i));
        _channel.at(i)->setPostGateGain(_cfgItem->GetChPostGateGain(i));
    }
}

void CfgSettingAutomaticMixerGatingRelativeThreshold::DispRtnData(CfgType type, unsigned int *d)
{
    switch (type) {
    case CFG_TYPE_AUTOMIXNGRT_4:
    case CFG_TYPE_AUTOMIXNGRT_8:
    case CFG_TYPE_AUTOMIXNGRT_16:
    case CFG_TYPE_AUTOMIXNGRT_32: {
        IG_PARAGET_AUTOMIXNGRT4_32 *p = (IG_PARAGET_AUTOMIXNGRT4_32 *)d;
        ui->label_NOM->setText(QString::number(p->passNom));
        int vu = p->attenGain*0.01-100;
        ui->VU_Gain->setLCurValue(vu);
        ui->label_VUGain->setText(QString::asprintf("%d dB", vu));
        for (int i = 0; i < _maxchannel; ++i) {
            _channel.at(i)->setOpen(p->chMixState[i].openState);
            _channel.at(i)->setVu(p->chMixState[i].sigAboveNoise);
        }
    }
        break;
    default:
        break;
    }
}

void CfgSettingAutomaticMixerGatingRelativeThreshold::slider_valueChanged(int value)
{
    if (sender() == ui->dial_ThresholdLevelAboveNoise) {
//        ui->label_ThresholdLevelAboveNoise->setText(QString::asprintf("%d dB", value));
        _cfgItem->SetThresholdLevelAboveNoise(value);
    }
    else if (sender() == ui->dial_Depth) {
//        ui->label_Depth->setText(QString::asprintf("%d dB", value));
        _cfgItem->SetDepthValue(value);
    }
    else if (sender() == ui->dial_HoldTime) {
//        ui->label_HoldTime->setText(QString::asprintf("%d ms", value));
        _cfgItem->SetHoldTime(value);
    }
    else if (sender() == ui->dial_NOMAttenuationStep) {
//        ui->label_NOMAttenuationStep->setText(QString::asprintf("%d dB", value));
        _cfgItem->SetNOMAttenuationStep(value);
    }
    else if (sender() == ui->dial_MAXNOMAttenuation) {
//        ui->label_MAXNOMAttenuation->setText(QString::asprintf("%d dB", value));
        _cfgItem->SetMaxNOMAttenuation(value);
    }
    else if (sender() == ui->dial_ResponseTime) {
//        ui->label_ResponseTime->setText(QString::asprintf("%d ms", value));
        _cfgItem->SetResponseTime(value);
    }
    else if (sender() == ui->verticalSlider_Gain) {
        ui->label_Gain->setText(QString::asprintf("%d dB", value));
        _cfgItem->SetMixOutGain(value);
    }
    else if (sender() == ui->spinBox_MaxNOM) {
        _cfgItem->SetMaxMON(value);
    }
    else if (sender() == ui->comboBox_NOMAttenuationType)
    {
        _cfgItem->SetNOMAttenuationType(value);
    }
    else {
        for (int i = 0; i < _maxchannel; ++i) {
            if (sender() == _channel.at(i)) {
                _cfgItem->SetChPostGateGain(i, value);
                break;
            }
        }
    }
}

void CfgSettingAutomaticMixerGatingRelativeThreshold::pushbutton_clicked(bool checked)
{
    if (sender() == ui->pb_LastMicOn) {
        for (int i = 0; i < _maxchannel; ++i) {
            _channel.at(i)->setDefault(false);
        }
        _cfgItem->SetLastMicOn(checked ? 1 : 0);
    }
    else if (sender() == ui->pushButton_DirectOutsNOMAtten) {
        _cfgItem->SetDirectOutsNOMAtten(checked);
    }
    else if (sender() == ui->pushButton_Mute) {
        _cfgItem->SetMixOutMute(checked);
    }
}

void CfgSettingAutomaticMixerGatingRelativeThreshold::Default_clicked(bool checked)
{
    for (int i = 0; i < _maxchannel; ++i) {
        if (sender() == _channel.at(i)) {
            ui->pb_LastMicOn->setChecked(false);
            _cfgItem->SetLastMicOn(checked ? 2 : 0);
            _cfgItem->SetDefaultCh(i);
        }
        else {
            _channel.at(i)->setDefault(false);
        }
    }
}

void CfgSettingAutomaticMixerGatingRelativeThreshold::Manual_clicked(bool checked)
{
    for (int i = 0; i < _maxchannel; ++i) {
        if (sender() == _channel.at(i)) {
            _cfgItem->SetChManual(i, checked);
            break;
        }
    }
}

void CfgSettingAutomaticMixerGatingRelativeThreshold::PostGateMute_clicked(bool checked)
{
    for (int i = 0; i < _maxchannel; ++i) {
        if (sender() == _channel.at(i)) {
            _cfgItem->SetChPostGateMute(i, checked);
            break;
        }
    }
}

void CfgSettingAutomaticMixerGatingRelativeThreshold::initData()
{
    uint8_t lastMicOn = _cfgItem->GetLastMicOn();
    uint8_t defaultCh = _cfgItem->GetDefaultCh();

    if (lastMicOn == 1) {
        ui->pb_LastMicOn->setChecked(true);
    }
    else if (lastMicOn == 2) {
        _channel.at(defaultCh)->setDefault(true);
    }

    ui->dial_ThresholdLevelAboveNoise->setValue(_cfgItem->GetThresholdLevelAboveNoise());
//    ui->label_ThresholdLevelAboveNoise->setText(QString::asprintf("%d dB", _cfgItem->GetThresholdLevelAboveNoise()));
    ui->dial_Depth->setValue(_cfgItem->GetDepthValue());
//    ui->label_Depth->setText(QString::asprintf("%d dB", _cfgItem->GetDepthValue()));
    ui->dial_HoldTime->setValue(_cfgItem->GetHoldTime());
//    ui->label_HoldTime->setText(QString::asprintf("%d ms", _cfgItem->GetHoldTime()));
    ui->spinBox_MaxNOM->setValue(_cfgItem->GetMaxMON());
    ui->pushButton_DirectOutsNOMAtten->setChecked(_cfgItem->GetDirectOutsNOMAtten());
    ui->comboBox_NOMAttenuationType->setCurrentIndex(_cfgItem->GetNOMAttenuationType());
    ui->dial_NOMAttenuationStep->setValue(_cfgItem->GetNOMAttenuationStep());
//    ui->label_NOMAttenuationStep->setText(QString::asprintf("%d dB", _cfgItem->GetNOMAttenuationStep()));
    ui->dial_MAXNOMAttenuation->setValue(_cfgItem->GetMaxNOMAttenuation());
//    ui->label_MAXNOMAttenuation->setText(QString::asprintf("%d dB", _cfgItem->GetMaxNOMAttenuation()));
    ui->dial_ResponseTime->setValue(_cfgItem->GetResponseTime());
//    ui->label_ResponseTime->setText(QString::asprintf("%d ms", _cfgItem->GetResponseTime()));
    ui->pushButton_Mute->setChecked(_cfgItem->GetMixOutMute());
    ui->pushButton_Mute->setProperty("form","Mute");
    ui->verticalSlider_Gain->setValue(_cfgItem->GetMixOutGain());
    ui->label_Gain->setText(QString::asprintf("%d dB", _cfgItem->GetMixOutGain()));

    connect(ui->dial_ThresholdLevelAboveNoise, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->dial_Depth, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->dial_HoldTime, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->spinBox_MaxNOM, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->dial_NOMAttenuationStep, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->dial_MAXNOMAttenuation, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->dial_ResponseTime, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->verticalSlider_Gain, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->comboBox_NOMAttenuationType, SIGNAL(currentIndexChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->pb_LastMicOn, SIGNAL(clicked(bool)), this, SLOT(pushbutton_clicked(bool)));
    connect(ui->pushButton_DirectOutsNOMAtten, SIGNAL(clicked(bool)), this, SLOT(pushbutton_clicked(bool)));
    connect(ui->pushButton_Mute, SIGNAL(clicked(bool)), this, SLOT(pushbutton_clicked(bool)));

    for (int i = 0; i < _maxchannel; ++i) {
        _channel.at(i)->setManual(_cfgItem->GetChManual(i));
        _channel.at(i)->setPostGateMute(_cfgItem->GetChPostGateMute(i));
        _channel.at(i)->setPostGateGain(_cfgItem->GetChPostGateGain(i));

        connect(_channel.at(i), SIGNAL(PostGateGainValueChanged(int)), this, SLOT(slider_valueChanged(int)));
        connect(_channel.at(i), SIGNAL(DefaultClicked(bool)), this, SLOT(Default_clicked(bool)));
        connect(_channel.at(i), SIGNAL(ManualClicked(bool)), this, SLOT(Manual_clicked(bool)));
        connect(_channel.at(i), SIGNAL(PostGateMuteClicked(bool)), this, SLOT(PostGateMute_clicked(bool)));
    }
}

