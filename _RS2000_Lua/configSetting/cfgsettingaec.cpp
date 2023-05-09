#include "cfgsettingaec.h"
#include "ui_cfgsettingaec.h"
#include "configItem/configitemaec.h"
#include "ui_CfgSettingAEC_FangTu.h"

CfgSettingAEC::CfgSettingAEC(ConfigItemAEC *cfgItem, QWidget *parent) :
    QWidget(parent)
{
#if Logo_FangTu
    _ui_FangTu = new Ui::CfgSettingAEC_FangTu;
    _ui_FangTu->setupUi(this);
    _cfgItem = cfgItem;
    init(_ui_FangTu);
#else
    _ui = new Ui::CfgSettingAEC;
    _ui->setupUi(this);
    _cfgItem = cfgItem;
    init(_ui);
#endif
}

CfgSettingAEC::~CfgSettingAEC()
{
    if(_ui)
        delete _ui;
    if(_ui_FangTu)
        delete _ui_FangTu;
}

void CfgSettingAEC::updateUiData()
{
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu )
    {
        _ui_FangTu->micInGain->setValue(_cfgItem->getMicInGain());
        _ui_FangTu->lineInGain->setValue(_cfgItem->getLineInGain());

        _ui_FangTu->speakOutGain->setValue(_cfgItem->getSpeakOutGain());
        _ui_FangTu->lineOutGain->setValue(_cfgItem->getLineOutGain());

        _ui_FangTu->speak2lineGain->setValue(_cfgItem->getSpeak2LineGain());
        _ui_FangTu->speak2lineSwitch->setChecked(_cfgItem->getSpeak2LineSwitch());

        _ui_FangTu->speak2lineGain->setEnabled(_cfgItem->getSpeak2LineSwitch());
        _ui_FangTu->NRGainSwitch->setChecked(_cfgItem->getNRGainSwitch());
        _ui_FangTu->comboBox->setEnabled(_cfgItem->getNRGainSwitch());
        _ui_FangTu->comboBox->setCurrentIndex((_cfgItem->getNRGain() / 3) - 2);
    }
    else{
        _ui->micInGain->setValue(_cfgItem->getMicInGain());
        _ui->micInGainLabel->setText(tr("%1 dB").arg(_cfgItem->getMicInGain()));

        _ui->lineInGain->setValue(_cfgItem->getLineInGain());
        _ui->lineInGainLabel->setText(tr("%1 dB").arg(_cfgItem->getLineInGain()));

        _ui->speakOutGain->setValue(_cfgItem->getSpeakOutGain());
        _ui->speakOutGainLabel->setText(tr("%1 dB").arg(_cfgItem->getSpeakOutGain()));

        _ui->lineOutGain->setValue(_cfgItem->getLineOutGain());
        _ui->lineOutGainLabel->setText(tr("%1 dB").arg(_cfgItem->getLineOutGain()));

        _ui->speak2lineGain->setValue(_cfgItem->getSpeak2LineGain());
        _ui->speak2lineGainLabel->setText(tr("%1 dB").arg(_cfgItem->getSpeak2LineGain()));

        _ui->speak2lineSwitch->setChecked(_cfgItem->getSpeak2LineSwitch());
        _ui->speak2lineGain->setEnabled(_cfgItem->getSpeak2LineSwitch());
        if(Config::curMfrsCfg().CurMfrs != Mfrs_BeiJingHuiSheng)
        {
            _ui->NRGainSwitch->setChecked(_cfgItem->getNRGainSwitch());
            _ui->comboBox->setEnabled(_cfgItem->getNRGainSwitch());
            _ui->comboBox->setCurrentIndex((_cfgItem->getNRGain() / 3) - 2);
        }
    }
}

void CfgSettingAEC::pb_clicked(bool value)
{
#if Logo_FangTu
    if (sender() == _ui_FangTu->speak2lineSwitch)
    {
        _cfgItem->setSpeak2LineSwitch(value);
        _ui_FangTu->speak2lineGain->setEnabled(value);
    }
#else
    if (sender() == _ui->speak2lineSwitch)
    {
        _cfgItem->setSpeak2LineSwitch(value);
        _ui->speak2lineGain->setEnabled(value);
    }
#endif
}

void CfgSettingAEC::spin_valueChanged(int value)
{
#if Logo_FangTu
    if (sender() == _ui_FangTu->micInGain){
        _cfgItem->setMicInGain(value);
    } else if (sender() == _ui_FangTu->lineInGain) {
        _cfgItem->setLineInGain(value);
    } else if (sender() == _ui_FangTu->speakOutGain) {
        _cfgItem->setSpeakOutGain(value);
    } else if (sender() == _ui_FangTu->lineOutGain) {
        _cfgItem->setLineOutGain(value);
        _cfgItem->setSpeak2LineGain(value);
    }
#else
    if (sender() == _ui->micInGain)
    {
        _cfgItem->setMicInGain(value);
        _ui->micInGainLabel->setText(tr("%1 dB").arg(value));
    } else if (sender() == _ui->lineInGain) {
        _cfgItem->setLineInGain(value);
        _ui->lineInGainLabel->setText(tr("%1 dB").arg(value));
    } else if (sender() == _ui->speakOutGain) {
        _cfgItem->setSpeakOutGain(value);
        _ui->speakOutGainLabel->setText(tr("%1 dB").arg(value));
    } else if (sender() == _ui->lineOutGain) {
        _cfgItem->setLineOutGain(value);
        _ui->lineOutGainLabel->setText(tr("%1 dB").arg(value));
    } else if (sender() == _ui->speak2lineGain) {
        _cfgItem->setSpeak2LineGain(value);
        _ui->speak2lineGainLabel->setText(tr("%1 dB").arg(value));
    }
#endif
}

void CfgSettingAEC::on_comboBox_currentIndexChanged(int index)
{
    _cfgItem->setNRGain((index + 2) * 3);
}

void CfgSettingAEC::on_NRGainSwitch_clicked(bool checked)
{
#if Logo_FangTu
    _cfgItem->setNRGainSwitch(checked);
    _ui_FangTu->comboBox->setEnabled(checked);
#else
    _cfgItem->setNRGainSwitch(checked);

    _ui->comboBox->setEnabled(checked);
#endif
}



void CfgSettingAEC::init(Ui::CfgSettingAEC *ui)
{
    ui->micInGain->setValue(_cfgItem->getMicInGain());
    ui->micInGainLabel->setText(tr("%1 dB").arg(_cfgItem->getMicInGain()));

    ui->lineInGain->setValue(_cfgItem->getLineInGain());
    ui->lineInGainLabel->setText(tr("%1 dB").arg(_cfgItem->getLineInGain()));

    ui->speakOutGain->setValue(_cfgItem->getSpeakOutGain());
    ui->speakOutGainLabel->setText(tr("%1 dB").arg(_cfgItem->getSpeakOutGain()));

    ui->lineOutGain->setValue(_cfgItem->getLineOutGain());
    ui->lineOutGainLabel->setText(tr("%1 dB").arg(_cfgItem->getLineOutGain()));

    ui->speak2lineGain->setValue(_cfgItem->getSpeak2LineGain());
    ui->speak2lineGainLabel->setText(tr("%1 dB").arg(_cfgItem->getSpeak2LineGain()));

    ui->speak2lineSwitch->setChecked(_cfgItem->getSpeak2LineSwitch());
    ui->speak2lineGain->setEnabled(_cfgItem->getSpeak2LineSwitch());
#ifdef Logo_BeiJingHuiSheng
    ui->NRGainSwitch->setHidden(true);
    ui->comboBox->setHidden(true);
#else
    ui->NRGainSwitch->setChecked(_cfgItem->getNRGainSwitch());
    ui->comboBox->setEnabled(_cfgItem->getNRGainSwitch());
    ui->comboBox->setCurrentIndex((_cfgItem->getNRGain() / 3) - 2);
#endif
    connect(ui->speak2lineSwitch, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->micInGain, SIGNAL(valueChanged(int)), this, SLOT(spin_valueChanged(int)));
    connect(ui->lineInGain, SIGNAL(valueChanged(int)), this, SLOT(spin_valueChanged(int)));
    connect(ui->speakOutGain, SIGNAL(valueChanged(int)), this, SLOT(spin_valueChanged(int)));
    connect(ui->lineOutGain, SIGNAL(valueChanged(int)), this, SLOT(spin_valueChanged(int)));
    connect(ui->speak2lineGain, SIGNAL(valueChanged(int)), this, SLOT(spin_valueChanged(int)));
}

void CfgSettingAEC::init(Ui::CfgSettingAEC_FangTu *ui)
{
    ui->micInGain->setRangeParam(-33, 12, StepMode_Line, ParamType_Gain);
    ui->micInGain->setValue(_cfgItem->getMicInGain());

    ui->lineInGain->setRangeParam(-4, 26, StepMode_Line, ParamType_Gain);
    ui->lineInGain->setValue(_cfgItem->getLineInGain());

    ui->speakOutGain->setRangeParam(-33, 12, StepMode_Line, ParamType_Gain);
    ui->speakOutGain->setValue(_cfgItem->getSpeakOutGain());

    ui->lineOutGain->setRangeParam(-28, 2, StepMode_Line, ParamType_Gain);
    ui->lineOutGain->setValue(_cfgItem->getLineOutGain());

    ui->speak2lineGain->setRangeParam(-60, 0, StepMode_Line, ParamType_Gain);
    ui->speak2lineGain->setValue(_cfgItem->getSpeak2LineGain());

    ui->speak2lineSwitch->setChecked(_cfgItem->getSpeak2LineSwitch());
    ui->speak2lineGain->setEnabled(_cfgItem->getSpeak2LineSwitch());

    ui->NRGainSwitch->setChecked(_cfgItem->getNRGainSwitch());
    ui->comboBox->setEnabled(_cfgItem->getNRGainSwitch());
    ui->comboBox->setCurrentIndex((_cfgItem->getNRGain() / 3) - 2);

//    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
//            this, &CfgSettingAEC::on_comboBox_currentIndexChanged);
//    connect(ui->NRGainSwitch, QOverload<bool>::of(&QPushButton::clicked),
//            this, &CfgSettingAEC::on_NRGainSwitch_clicked);
    connect(ui->speak2lineSwitch, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->micInGain, SIGNAL(valueChanged(int)), this, SLOT(spin_valueChanged(int)));
    connect(ui->lineInGain, SIGNAL(valueChanged(int)), this, SLOT(spin_valueChanged(int)));
    connect(ui->speakOutGain, SIGNAL(valueChanged(int)), this, SLOT(spin_valueChanged(int)));
    connect(ui->lineOutGain, SIGNAL(valueChanged(int)), this, SLOT(spin_valueChanged(int)));
    connect(ui->speak2lineGain, SIGNAL(valueChanged(int)), this, SLOT(spin_valueChanged(int)));
}
