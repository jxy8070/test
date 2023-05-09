#include "cfgsettingcrossfader.h"
#include "ui_cfgsettingcrossfader.h"
#include "configItem/configitemcrossfader.h"

CfgSettingCrossfader::CfgSettingCrossfader(ConfigItemCrossfader *cfgItem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CfgSettingCrossfader)
{
    ui->setupUi(this);

    _cfgItem = cfgItem;
    initData();
}

CfgSettingCrossfader::~CfgSettingCrossfader()
{
    delete ui;
}

void CfgSettingCrossfader::updateUiData()
{
    initData();
}

void CfgSettingCrossfader::DispRtnData(unsigned int *d)
{
    IG_PARAGET_CROSSFADER *p = (IG_PARAGET_CROSSFADER *)d;
    int gainA = p->chBandAGain*0.01-100;
    int gainB = p->chBandBGain*0.01-100;
    int percent = p->chBandBGain*0.01;
    ui->label_Gain_A->setText(QString::asprintf("%d dB", gainA));
    ui->label_Gain_B->setText(QString::asprintf("%d dB", gainB));
    ui->label_Position->setText(QString::asprintf("%d \%", percent));
}

void CfgSettingCrossfader::on_pushButton_A_clicked(bool checked)
{
    if (checked) {
        ui->pushButton_A->setChecked(true);
        ui->pushButton_B->setChecked(false);
        _cfgItem->set_ch(0);
    }
    else {
        ui->pushButton_A->setChecked(false);
        ui->pushButton_B->setChecked(true);
        _cfgItem->set_ch(1);
    }
}

void CfgSettingCrossfader::on_pushButton_B_clicked(bool checked)
{
    if (checked) {
        ui->pushButton_A->setChecked(false);
        ui->pushButton_B->setChecked(true);
        _cfgItem->set_ch(1);
    }
    else {
        ui->pushButton_A->setChecked(true);
        ui->pushButton_B->setChecked(false);
        _cfgItem->set_ch(0);
    }
}

void CfgSettingCrossfader::on_dial_Time_valueChanged(int value)
{
    _cfgItem->set_time(value);
}

void CfgSettingCrossfader::on_comboBox_currentIndexChanged(int index)
{
    _cfgItem->set_type(index);
}

void CfgSettingCrossfader::initData()
{
    ui->dial_Time->setRangeParam(10, 10000, StepMode_Log10, ParamType_Time);
    ui->dial_Time->setValue(_cfgItem->get_time());

    switch (_cfgItem->get_ch()) {
    case 0: ui->pushButton_A->setChecked(true); ui->pushButton_B->setChecked(false); break;
    case 1: ui->pushButton_A->setChecked(false); ui->pushButton_B->setChecked(true); break;
    default: break;
    }

    ui->comboBox->setCurrentIndex(_cfgItem->get_type());
    ui->label_Position->setProperty("form", true);
    ui->label_Gain_A->setProperty("form", true);
    ui->label_Gain_B->setProperty("form", true);
}
