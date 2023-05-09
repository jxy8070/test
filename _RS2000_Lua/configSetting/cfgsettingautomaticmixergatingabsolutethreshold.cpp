#include "cfgsettingautomaticmixergatingabsolutethreshold.h"
#include "ui_cfgsettingautomaticmixergatingabsolutethreshold.h"
#include "configItem/configitemautomaticmixer.h"
#include "ui_CfgSettingAutomaticMixerGatingAbsoluteThreshold_FangTu.h"

CfgSettingAutomaticMixerGatingAbsoluteThreshold::CfgSettingAutomaticMixerGatingAbsoluteThreshold(int channels, ConfigItemAutomaticMixer *cfgItem, QWidget *parent) :
    QWidget(parent)
{
    _cfgItem = cfgItem;
#if Logo_FangTu
    initUI_FangTu(channels);
    initData<Ui::CfgSettingAutomaticMixerGatingAbsoluteThreshold_FangTu>(_ui_FangTu);
#else
    initUI(channels);
    initData<Ui::CfgSettingAutomaticMixerGatingAbsoluteThreshold>(_ui);
#endif

//    QLatin1String qss("QScrollArea {background-color:transparent;}\
//                        QWidget#scrollAreaWidgetContents {background-color:transparent;}");
//    setStyleSheet(qss);
}

CfgSettingAutomaticMixerGatingAbsoluteThreshold::~CfgSettingAutomaticMixerGatingAbsoluteThreshold()
{
    if(_ui) delete _ui;
    if(_ui_FangTu) delete _ui_FangTu;
}

void CfgSettingAutomaticMixerGatingAbsoluteThreshold::updateUiData()
{
#if Logo_FangTu
    for (int i = 0; i < _maxchannel; ++i) {
        _channel.at(i)->setManual(_cfgItem->GetChManual(i));
        _channel.at(i)->setMode(_cfgItem->GetChMode(i));
        _channel.at(i)->setPriority(_cfgItem->GetChPriority(i));
        _channel.at(i)->setThreshold(_cfgItem->GetChThreshold(i));
    }

    _ui_FangTu->pushButton_Mute->setChecked(_cfgItem->GetMixOutMute());
    _ui_FangTu->verticalSlider_Gain->setValue(_cfgItem->GetMixOutGain());
    _ui_FangTu->pb_LastMicOn->setChecked(_cfgItem->GetLastMicOn());
    _ui_FangTu->pb_IDGating->setChecked(_cfgItem->GetIDGating());
    _ui_FangTu->dial_BackgroundPercentage->setValue(_cfgItem->GetBackgroundPercentage());
    _ui_FangTu->dial_Depth->setValue(_cfgItem->GetDepthValue());
    _ui_FangTu->dial_HoldTime->setValue(_cfgItem->GetHoldTime());

    _ui_FangTu->spinBox_MaxNOM->setValue(_cfgItem->GetMaxMON());
    _ui_FangTu->comboBox_NOMAttenuationType->setCurrentIndex(_cfgItem->GetNOMAttenuationType());
    _ui_FangTu->dial_NOMAttenuationStep->setValue(_cfgItem->GetNOMAttenuationStep());
    _ui_FangTu->dial_MAXNOMAttenuation->setValue(_cfgItem->GetMaxNOMAttenuation());
    _ui_FangTu->dial_ResponseTime->setValue(_cfgItem->GetResponseTime());
#else
    for (int i = 0; i < _maxchannel; ++i) {
        _channel.at(i)->setManual(_cfgItem->GetChManual(i));
        _channel.at(i)->setMode(_cfgItem->GetChMode(i));
        _channel.at(i)->setPriority(_cfgItem->GetChPriority(i));
        _channel.at(i)->setThreshold(_cfgItem->GetChThreshold(i));
    }

    _ui->pushButton_Mute->setChecked(_cfgItem->GetMixOutMute());
    _ui->verticalSlider_Gain->setValue(_cfgItem->GetMixOutGain());
    _ui->label_Gain->setText(QString::asprintf("%d dB", _cfgItem->GetMixOutGain()));
    _ui->pb_LastMicOn->setChecked(_cfgItem->GetLastMicOn());
    _ui->pb_IDGating->setChecked(_cfgItem->GetIDGating());
    _ui->dial_BackgroundPercentage->setValue(_cfgItem->GetBackgroundPercentage());
//    _ui->label_BackgroundPercentage->setText(QString::asprintf("%.1f\%", _cfgItem->GetBackgroundPercentage()));
    _ui->dial_Depth->setValue(_cfgItem->GetDepthValue());
//    _ui->label_Depth->setText(QString::asprintf("%d dB", _cfgItem->GetDepthValue()));
    _ui->dial_HoldTime->setValue(_cfgItem->GetHoldTime());
//    _ui->label_HoldTime->setText(QString::asprintf("%d ms", _cfgItem->GetHoldTime()));

    _ui->spinBox_MaxNOM->setValue(_cfgItem->GetMaxMON());
    _ui->comboBox_NOMAttenuationType->setCurrentIndex(_cfgItem->GetNOMAttenuationType());
    _ui->dial_NOMAttenuationStep->setValue(_cfgItem->GetNOMAttenuationStep());
//    _ui->label_NOMAttenuationStep->setText(QString::asprintf("%d dB", _cfgItem->GetNOMAttenuationStep()));
    _ui->dial_MAXNOMAttenuation->setValue(_cfgItem->GetMaxNOMAttenuation());
//    _ui->label_MAXNOMAttenuation->setText(QString::asprintf("%d dB", _cfgItem->GetMaxNOMAttenuation()));
    _ui->dial_ResponseTime->setValue(_cfgItem->GetResponseTime());
//    _ui->label_ResponseTime->setText(QString::asprintf("%d ms", _cfgItem->GetResponseTime()));
#endif
}

void CfgSettingAutomaticMixerGatingAbsoluteThreshold::DispRtnData(CfgType type, unsigned int *d)
{
    switch (type) {
    case CFG_TYPE_AUTOMIXNGAT_4:
    case CFG_TYPE_AUTOMIXNGAT_8:
    case CFG_TYPE_AUTOMIXNGAT_16:
    case CFG_TYPE_AUTOMIXNGAT_32:{
            IG_PARAGET_AUTOMIXNGAT4_32 *p = (IG_PARAGET_AUTOMIXNGAT4_32 *)d;
            int vu = p->attenGain*0.01-100;
#if Logo_FangTu
            _ui_FangTu->label_NOM->setText(QString::number(p->passNom));
            _ui_FangTu->VU_Gain->setLCurValue(vu);
            _ui_FangTu->label_VUGain->setText(QString::asprintf("%ddB", vu));
            for (int i = 0; i < _maxchannel; ++i) {
                _channel.at(i)->setOpen((p->openstate[0] >> i) & 0x01);
            }
#else
            _ui->label_NOM->setText(QString::number(p->passNom));
            _ui->VU_Gain->setLCurValue(vu);
            _ui->label_VUGain->setText(QString::asprintf("%ddB", vu));
            for (int i = 0; i < _maxchannel; ++i) {
                _channel.at(i)->setOpen((p->openstate[0] >> i) & 0x01);
            }
#endif
        }
        break;
    default:
        break;
    }
}

void CfgSettingAutomaticMixerGatingAbsoluteThreshold::slider_valueChanged(int value)
{
#if Logo_FangTu
    if (sender() == _ui_FangTu->verticalSlider_Gain) {
        _cfgItem->SetMixOutGain(value);
    }
    else if (sender() == _ui_FangTu->dial_BackgroundPercentage) {
        _cfgItem->SetBackgroundPercentage(value);
    }
    else if (sender() == _ui_FangTu->dial_Depth) {
        _cfgItem->SetDepthValue(value);
    }
    else if (sender() == _ui_FangTu->dial_HoldTime) {
        _cfgItem->SetHoldTime(value);
    }
    else if (sender() == _ui_FangTu->spinBox_MaxNOM) {
        _cfgItem->SetMaxMON(value);
    }
    else if (sender() == _ui_FangTu->comboBox_NOMAttenuationType) {
        _cfgItem->SetNOMAttenuationType(value);
    }
    else if (sender() == _ui_FangTu->dial_NOMAttenuationStep) {
        _cfgItem->SetNOMAttenuationStep(value);
    }
    else if (sender() == _ui_FangTu->dial_MAXNOMAttenuation) {
        _cfgItem->SetMaxNOMAttenuation(value);
    }
    else if (sender() == _ui_FangTu->dial_ResponseTime) {
        _cfgItem->SetResponseTime(value);
    }
#else
    if (sender() == _ui->verticalSlider_Gain) {
        _cfgItem->SetMixOutGain(value);
        _ui->label_Gain->setText(QString::asprintf("%d dB", _cfgItem->GetMixOutGain()));
    }
    else if (sender() == _ui->dial_BackgroundPercentage) {
        _cfgItem->SetBackgroundPercentage(value);
//        _ui->label_BackgroundPercentage->setText(QString::asprintf("%.1f\%", _cfgItem->GetBackgroundPercentage()));
    }
    else if (sender() == _ui->dial_Depth) {
        _cfgItem->SetDepthValue(value);
//        _ui->label_Depth->setText(QString::asprintf("%d dB", _cfgItem->GetDepthValue()));
    }
    else if (sender() == _ui->dial_HoldTime) {
        _cfgItem->SetHoldTime(value);
//        _ui->label_HoldTime->setText(QString::asprintf("%d ms", _cfgItem->GetHoldTime()));
    }
    else if (sender() == _ui->spinBox_MaxNOM) {
        _cfgItem->SetMaxMON(value);
    }
    else if (sender() == _ui->comboBox_NOMAttenuationType) {
        _cfgItem->SetNOMAttenuationType(value);
    }
    else if (sender() == _ui->dial_NOMAttenuationStep) {
        _cfgItem->SetNOMAttenuationStep(value);
//        _ui->label_NOMAttenuationStep->setText(QString::asprintf("%d dB", _cfgItem->GetNOMAttenuationStep()));
    }
    else if (sender() == _ui->dial_MAXNOMAttenuation) {
        _cfgItem->SetMaxNOMAttenuation(value);
//        _ui->label_MAXNOMAttenuation->setText(QString::asprintf("%d dB", _cfgItem->GetMaxNOMAttenuation()));
    }
    else if (sender() == _ui->dial_ResponseTime) {
        _cfgItem->SetResponseTime(value);
//        _ui->label_ResponseTime->setText(QString::asprintf("%d ms", _cfgItem->GetResponseTime()));
    }
#endif
}

void CfgSettingAutomaticMixerGatingAbsoluteThreshold::pushbutton_clicked(bool checked)
{
#if Logo_FangTu
    if(sender() == _ui_FangTu->pushButton_Mute) {
        _cfgItem->SetMixOutMute(checked);
    }else if(sender() == _ui_FangTu->pb_LastMicOn) {
        _cfgItem->SetLastMicOn(checked);
    }else if(sender() == _ui_FangTu->pb_IDGating) {
        _cfgItem->SetIDGating(checked);
    }
#else
    if (sender() == _ui->pushButton_Mute) {
        _cfgItem->SetMixOutMute(checked);
    }
    else if (sender() == _ui->pb_LastMicOn) {
        _cfgItem->SetLastMicOn(checked);
    }
    else if (sender() == _ui->pb_IDGating) {
        _cfgItem->SetIDGating(checked);
    }
#endif
}

void CfgSettingAutomaticMixerGatingAbsoluteThreshold::Manual_Clicked(bool checked)
{
    for (int i = 0; i < _maxchannel; ++i) {
        if (sender() == _channel.at(i)) {
            _cfgItem->SetChManual(i, checked);
            break;
        }
    }
}

void CfgSettingAutomaticMixerGatingAbsoluteThreshold::Mode_ValueChanged(int value)
{
    for (int i = 0; i < _maxchannel; ++i) {
        if (sender() == _channel.at(i)) {
            _cfgItem->SetChMode(i, value);
            break;
        }
    }
}

void CfgSettingAutomaticMixerGatingAbsoluteThreshold::Priority_ValueChanged(int value)
{
    for (int i = 0; i < _maxchannel; ++i) {
        if (sender() == _channel.at(i)) {
            _cfgItem->SetChPriority(i, value);
            break;
        }
    }
}

void CfgSettingAutomaticMixerGatingAbsoluteThreshold::Threshold_ValueChanged(int value)
{
    for (int i = 0; i < _maxchannel; ++i) {
        if (sender() == _channel.at(i)) {
            _cfgItem->SetChThreshold(i, value);
            break;
        }
    }
}

void CfgSettingAutomaticMixerGatingAbsoluteThreshold::initUI(int channels)
{
    _ui = new Ui::CfgSettingAutomaticMixerGatingAbsoluteThreshold;
    _ui->setupUi(this);

    _channel.append(_ui->widget_1);
    _channel.append(_ui->widget_2);
    _channel.append(_ui->widget_3);
    _channel.append(_ui->widget_4);
    _channel.append(_ui->widget_5);
    _channel.append(_ui->widget_6);
    _channel.append(_ui->widget_7);
    _channel.append(_ui->widget_8);
    _channel.append(_ui->widget_9);
    _channel.append(_ui->widget_10);
    _channel.append(_ui->widget_11);
    _channel.append(_ui->widget_12);
    _channel.append(_ui->widget_13);
    _channel.append(_ui->widget_14);
    _channel.append(_ui->widget_15);
    _channel.append(_ui->widget_16);
    _channel.append(_ui->widget_17);
    _channel.append(_ui->widget_18);
    _channel.append(_ui->widget_19);
    _channel.append(_ui->widget_20);
    _channel.append(_ui->widget_21);
    _channel.append(_ui->widget_22);
    _channel.append(_ui->widget_23);
    _channel.append(_ui->widget_24);
    _channel.append(_ui->widget_25);
    _channel.append(_ui->widget_26);
    _channel.append(_ui->widget_27);
    _channel.append(_ui->widget_28);
    _channel.append(_ui->widget_29);
    _channel.append(_ui->widget_30);
    _channel.append(_ui->widget_31);
    _channel.append(_ui->widget_32);

    for (int i = 0; i < 32; ++i) {
        _channel.at(i)->setTitle(QString::number(i+1));
        if (i >= channels) {
            _channel.at(i)->setVisible(false);
        }
    }

    _maxchannel = channels;
    _ui->spinBox_MaxNOM->setRange(1, channels);
    _ui->pushButton_Mute->setProperty("form", "Mute");
    adjustSize();
}

void CfgSettingAutomaticMixerGatingAbsoluteThreshold::initUI_FangTu(int channels)
{
    _ui_FangTu = new Ui::CfgSettingAutomaticMixerGatingAbsoluteThreshold_FangTu;
    _ui_FangTu->setupUi(this);

    _channel.append(_ui_FangTu->widget_1);
    _channel.append(_ui_FangTu->widget_2);
    _channel.append(_ui_FangTu->widget_3);
    _channel.append(_ui_FangTu->widget_4);
    _channel.append(_ui_FangTu->widget_5);
    _channel.append(_ui_FangTu->widget_6);
    _channel.append(_ui_FangTu->widget_7);
    _channel.append(_ui_FangTu->widget_8);
    _channel.append(_ui_FangTu->widget_9);
    _channel.append(_ui_FangTu->widget_10);
    _channel.append(_ui_FangTu->widget_11);
    _channel.append(_ui_FangTu->widget_12);
    _channel.append(_ui_FangTu->widget_13);
    _channel.append(_ui_FangTu->widget_14);
    _channel.append(_ui_FangTu->widget_15);
    _channel.append(_ui_FangTu->widget_16);
    _channel.append(_ui_FangTu->widget_17);
    _channel.append(_ui_FangTu->widget_18);
    _channel.append(_ui_FangTu->widget_19);
    _channel.append(_ui_FangTu->widget_20);
    _channel.append(_ui_FangTu->widget_21);
    _channel.append(_ui_FangTu->widget_22);
    _channel.append(_ui_FangTu->widget_23);
    _channel.append(_ui_FangTu->widget_24);
    _channel.append(_ui_FangTu->widget_25);
    _channel.append(_ui_FangTu->widget_26);
    _channel.append(_ui_FangTu->widget_27);
    _channel.append(_ui_FangTu->widget_28);
    _channel.append(_ui_FangTu->widget_29);
    _channel.append(_ui_FangTu->widget_30);
    _channel.append(_ui_FangTu->widget_31);
    _channel.append(_ui_FangTu->widget_32);

    for (int i = 0; i < 32; ++i) {
        _channel.at(i)->setTitle(QString::number(i+1));
        if (i >= channels) {
            _channel.at(i)->setVisible(false);
        }
    }

    _maxchannel = channels;
    _ui_FangTu->spinBox_MaxNOM->setRange(1, channels);

    _ui_FangTu->dial_BackgroundPercentage->setRangeParam(0, 100, StepMode_Line, ParamType_Other);

    _ui_FangTu->dial_Depth->setRangeParam(0, 60, StepMode_Line, ParamType_Gain);

    _ui_FangTu->dial_HoldTime->setRangeParam(1, 10000, StepMode_Log10, ParamType_Time);

    _ui_FangTu->dial_NOMAttenuationStep->setRangeParam(0, 20, StepMode_Line, ParamType_Gain);

    _ui_FangTu->dial_MAXNOMAttenuation->setRangeParam(0, 40, StepMode_Line, ParamType_Gain);

    _ui_FangTu->dial_ResponseTime->setRangeParam(1, 10000, StepMode_Log10, ParamType_Time);
    _ui_FangTu->pushButton_Mute->setProperty("form", "Mute");
//    qDebug() << __FUNCTION__ << __LINE__ << size();
    setFixedSize(size());
}

template<typename UI_t>
void CfgSettingAutomaticMixerGatingAbsoluteThreshold::initData(UI_t *ui)
{
    for (int i = 0; i < _maxchannel; ++i) {
        _channel.at(i)->setManual(_cfgItem->GetChManual(i));
        _channel.at(i)->setMode(_cfgItem->GetChMode(i));
        _channel.at(i)->setPriority(_cfgItem->GetChPriority(i));
        _channel.at(i)->setThreshold(_cfgItem->GetChThreshold(i));

        connect(_channel.at(i), SIGNAL(ManualClicked(bool)), this, SLOT(Manual_Clicked(bool)));
        connect(_channel.at(i), SIGNAL(ModeValueChanged(int)), this, SLOT(Mode_ValueChanged(int)));
        connect(_channel.at(i), SIGNAL(PriorityValueChanged(int)), this, SLOT(Priority_ValueChanged(int)));
        connect(_channel.at(i), SIGNAL(ThresholdValueChanged(int)), this, SLOT(Threshold_ValueChanged(int)));
    }

    ui->pushButton_Mute->setChecked(_cfgItem->GetMixOutMute());
    ui->verticalSlider_Gain->setValue(_cfgItem->GetMixOutGain());

    connect(ui->pushButton_Mute, SIGNAL(clicked(bool)), this, SLOT(pushbutton_clicked(bool)));
    connect(ui->verticalSlider_Gain, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));

    ui->pb_LastMicOn->setChecked(_cfgItem->GetLastMicOn());
    ui->pb_IDGating->setChecked(_cfgItem->GetIDGating());
    ui->dial_BackgroundPercentage->setValue(_cfgItem->GetBackgroundPercentage());


    ui->dial_Depth->setValue(_cfgItem->GetDepthValue());
    ui->dial_HoldTime->setValue(_cfgItem->GetHoldTime());


    connect(ui->pb_LastMicOn, SIGNAL(clicked(bool)), this, SLOT(pushbutton_clicked(bool)));
    connect(ui->pb_IDGating, SIGNAL(clicked(bool)), this, SLOT(pushbutton_clicked(bool)));
    connect(ui->dial_BackgroundPercentage, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->dial_Depth, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->dial_HoldTime, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));

    ui->spinBox_MaxNOM->setValue(_cfgItem->GetMaxMON());
    ui->comboBox_NOMAttenuationType->setCurrentIndex(_cfgItem->GetNOMAttenuationType());
    ui->dial_NOMAttenuationStep->setValue(_cfgItem->GetNOMAttenuationStep());

    ui->dial_MAXNOMAttenuation->setValue(_cfgItem->GetMaxNOMAttenuation());
    ui->dial_ResponseTime->setValue(_cfgItem->GetResponseTime());


    connect(ui->spinBox_MaxNOM, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->comboBox_NOMAttenuationType, SIGNAL(currentIndexChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->dial_NOMAttenuationStep, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->dial_MAXNOMAttenuation, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
    connect(ui->dial_ResponseTime, SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));
}
