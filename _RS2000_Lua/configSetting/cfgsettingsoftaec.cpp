#include "cfgsettingsoftaec.h"
#include "ui_cfgsettingsoftaec.h"
#include "configItem/configitemsoftaec.h"

CfgSettingSoftAEC::CfgSettingSoftAEC(ConfigItemSoftAEC *cfgItem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CfgSettingSoftAEC)
{
    ui->setupUi(this);
    _cfgItem = cfgItem;
    initUi();
}

CfgSettingSoftAEC::~CfgSettingSoftAEC()
{
    delete ui;
}

void CfgSettingSoftAEC::initUi()
{
    ui->btnBypass->setProperty("form", "Bypass");
    ui->_gainMicToSpeak->setRangeParam(-40, 12, StepMode_Line, CfgParamType::ParamType_Gain);
    ui->_gainMicToSpeak->setDecimalsNum(2);
    ui->_perscentRES->setRangeParam(0, 100, StepMode_Line, CfgParamType::ParamType_Other);
    ui->_gainNR->setRangeParam(0, 20, StepMode_Line, CfgParamType::ParamType_Gain);
    ui->_gainNR->setDecimalsNum(2);
    ui->_gainCN->setRangeParam(-80, -30, StepMode_Line, CfgParamType::ParamType_Gain);
    ui->_gainCN->setDecimalsNum(2);
    ui->_gainRef->setRangeParam(-40, 0, StepMode_Line, CfgParamType::ParamType_Gain);
    ui->_gainRef->setDecimalsNum(2);
    connect(ui->_gainMicToSpeak, SIGNAL(valueChanged(double)), this, SLOT(spin_valueChanged(double)));
    connect(ui->_perscentRES, SIGNAL(valueChanged(double)), this, SLOT(spin_valueChanged(double)));
    connect(ui->_gainNR, SIGNAL(valueChanged(double)), this, SLOT(spin_valueChanged(double)));
    connect(ui->_gainCN, SIGNAL(valueChanged(double)), this, SLOT(spin_valueChanged(double)));
    connect(ui->_gainRef, SIGNAL(valueChanged(double)), this, SLOT(spin_valueChanged(double)));
    connect(ui->btnBypass, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->btnMicEnable, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->btnRESEnable, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->btnNREnable, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->btnCNEnable, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    ui->vuGainERLE->setMaxValue(20);
    ui->vuGainERLE->setMinValue(0);
    ui->vuGainERLE->setSingleTrack(true);
    ui->vuGainRMLR->setMaxValue(10);
    ui->vuGainRMLR->setMinValue(-10);
    ui->vuGainRMLR->setIsApplidGain(true);

    //初始化数据
    ui->_gainMicToSpeak->setValue(_cfgItem->getMicToSpeakGain());
    ui->_perscentRES->setValue(_cfgItem->getRESStrength());
    ui->_gainRef->setValue(_cfgItem->getRefGain());
    ui->_gainNR->setValue(_cfgItem->getNRGain());
    ui->_gainCN->setValue(_cfgItem->getCNGain());
    ui->btnBypass->setChecked(_cfgItem->getBypass());
    ui->btnMicEnable->setChecked(_cfgItem->getMicToSpeakSwitch());
    ui->btnRESEnable->setChecked(_cfgItem->getRESSwitch());
    ui->btnNREnable->setChecked(_cfgItem->getNRSwitch());
    ui->btnCNEnable->setChecked(_cfgItem->getCNSwitch());
}


void CfgSettingSoftAEC::pb_clicked(bool value)
{
    if (sender() == ui->btnBypass) {
        _cfgItem->setBypass(value);
    }
    else if (sender() == ui->btnMicEnable) {
        _cfgItem->setMicToSpeakSwitch((int)value);
    }
    else if (sender() == ui->btnRESEnable) {
        _cfgItem->setRESSwitch(value);
    }
    else if (sender() == ui->btnNREnable) {
        _cfgItem->setNRSwitch(value);
    }
    else if (sender() == ui->btnCNEnable) {
        _cfgItem->setCNSwitch(value);
    }
}

void CfgSettingSoftAEC::spin_valueChanged(double value)
{
    if (sender() == ui->_gainMicToSpeak) {
        _cfgItem->setMicToSpeakGain(value);
    }
    else if (sender() == ui->_perscentRES) {
        _cfgItem->setRESStrength((int)value);
    }
    else if (sender() == ui->_gainNR) {
        _cfgItem->setNRGain(value);
    }
    else if (sender() == ui->_gainCN) {
        _cfgItem->setCNGain(value);
    }
    else if (sender() == ui->_gainRef) {
        _cfgItem->setRefGain(value);
    }
}

void CfgSettingSoftAEC::updateRtnData(double vuERLE, double vuRMLR)
{
    ui->vuGainERLE->setLCurValue(abs(vuERLE));
    ui->vuLabelERLE->setText(QString::number(vuERLE,'f',1) + "dB");

    ui->vuGainRMLR->setLCurValue(vuRMLR);
    ui->vuLabelRMLR->setText(QString::number(vuRMLR,'f',1) + "dB");
}

void CfgSettingSoftAEC::updateUiData()
{
    ui->btnBypass->setChecked(_cfgItem->getBypass());
    ui->_gainRef->setValue(_cfgItem->getRefGain());

    ui->_gainMicToSpeak->setValue(_cfgItem->getMicToSpeakGain());
    ui->btnMicEnable->setChecked(_cfgItem->getMicToSpeakSwitch());

    ui->_perscentRES->setValue(_cfgItem->getRESStrength());
    ui->btnRESEnable->setChecked(_cfgItem->getRESSwitch());

    ui->_gainNR->setValue(_cfgItem->getNRGain());
    ui->btnNREnable->setChecked(_cfgItem->getNRSwitch());

    ui->_gainCN->setValue(_cfgItem->getCNGain());
    ui->btnCNEnable->setChecked(_cfgItem->getCNSwitch());
}
