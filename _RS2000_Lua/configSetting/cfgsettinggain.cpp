#include "cfgsettinggain.h"
#include "ui_cfgsettinggain.h"
#include "configItem/configitemgain.h"

CfgSettingGain::CfgSettingGain(ConfigItemGain *cfgItem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CfgSettingGain)
{
    ui->setupUi(this);

    _cfgItem = cfgItem;
    initData();

    connect(ui->pb_bypass, SIGNAL(clicked(bool)), this ,SLOT(pbClickedSlot(bool)));
    connect(ui->pb_invert, SIGNAL(clicked(bool)), this ,SLOT(pbClickedSlot(bool)));
    connect(ui->pb_mute, SIGNAL(clicked(bool)), this ,SLOT(pbClickedSlot(bool)));
    connect(ui->sp_gain, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
}

CfgSettingGain::~CfgSettingGain()
{
    delete ui;
}

void CfgSettingGain::updateUiData()
{
    initData();
}

void CfgSettingGain::DispRtnData(unsigned int *d)
{
//    IG_PARAGET_GAINRAMP *p = (IG_PARAGET_GAINRAMP *)d;
//    int rampGain = p->rtRampGain*0.01-100;
    //    ui->label_Gain->setText(QString::asprintf("%d dB", rampGain));
}

void CfgSettingGain::pbClickedSlot(bool checked)
{
    if (sender() == ui->pb_bypass) {
        if (ui->pb_mute->isChecked()) {
            _cfgItem->set_mode(1);
        }else if(checked) {
            _cfgItem->set_mode(2);
        }else {
            _cfgItem->set_mode(0);
        }
    }
    else if (sender() == ui->pb_mute) {
        if (checked) {
            _cfgItem->set_mode(1);
        }
        else if (ui->pb_bypass->isChecked()) {
            _cfgItem->set_mode(2);
        }
        else {
            _cfgItem->set_mode(0);
        }
    }
    else if (sender() == ui->pb_invert) {
        _cfgItem->set_phase(checked);
    }
}

void CfgSettingGain::processDoubleSpinBoxSlot(double gain)
{
    _cfgItem->set_gain(gain);
}

void CfgSettingGain::initData()
{
    ui->sp_gain->setRangeParam(-100, 20, StepMode_Line, ParamType_Gain);
    ui->sp_gain->setDecimalsNum(2);
    ui->sp_gain->setValue(_cfgItem->get_gain());
    ui->pb_invert->setChecked(_cfgItem->get_phase());
    switch (_cfgItem->get_mode()) {
    case 0://hrp
        ui->pb_bypass->setChecked(false);//
        ui->pb_mute->setChecked(false);//
        break;//
    case 1:
        ui->pb_bypass->setChecked(false);//hrp
        ui->pb_mute->setChecked(true);
        break;
    case 2:
        ui->pb_bypass->setChecked(true);
        ui->pb_mute->setChecked(false);//hrp
        break;
    }
    ui->pb_mute->setProperty("form", "Mute");
    ui->pb_bypass->setProperty("form", "Bypass");
}

