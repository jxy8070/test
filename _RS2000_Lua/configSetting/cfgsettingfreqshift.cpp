#include "cfgsettingfreqshift.h"
#include "ui_cfgsettingfreqshift.h"
#include "ui_cfgsettingfreqshift_plus.h"
#include "configItem/configitemfreqshift.h"
#include "ui_CfgSettingFreqShift_Plus_FangTu.h"
CfgSettingFreqShift::CfgSettingFreqShift(ConfigItemFreqShift *cfgItem, QWidget *parent) :
    QWidget(parent)
{
    _cfgItem = cfgItem;

    initData();
}

CfgSettingFreqShift::~CfgSettingFreqShift()
{
    switch (_cfgItem->getCfgType()) {
    case CFG_TYPE_FREQSHIFT:
        delete ui;
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        delete ui_plus;
        break;
    default:
        break;
    }
}

void CfgSettingFreqShift::upDateUiData()
{
    switch (_cfgItem->getCfgType()) {
    case CFG_TYPE_FREQSHIFT:
        ui->SP_Freq->setValue(_cfgItem->GetFreq());
        ui->SP_Gain->setValue(_cfgItem->GetGain());
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
        ui_plus->SP_Freq->setValue(_cfgItem->GetFreq());
        ui_plus->SP_Gain->setValue(_cfgItem->GetGain());
        ui_plus->SP_Phase->setValue(_cfgItem->GetPhase());
        ui_plus->ShiftFreqMode->setCurrentIndex(_cfgItem->GetFreqMode());
        ui_plus->ShiftFreqSpeed->setValue(_cfgItem->GetFreqSpeed());
        ui_plus->ShiftFreqSpan->setRangeParam(0, 2*_cfgItem->GetFreq(), StepMode_Line, ParamType_Freq);
        ui_plus->ShiftFreqSpan->setValue(_cfgItem->GetFreqSpan());
        break;
    default:
        break;
    }
}

void CfgSettingFreqShift::initData()
{
    switch (_cfgItem->getCfgType()) {
    case CFG_TYPE_FREQSHIFT:
        ui = new Ui::CfgSettingFreqShift;
        ui->setupUi(this);

        ui->SP_Freq->setRangeParam(0, 10, StepMode_Line,ParamType_Freq);
        ui->SP_Freq->setValue(_cfgItem->GetFreq());

        ui->SP_Gain->setRangeParam(-100, 0, StepMode_Line,ParamType_Gain);
        ui->SP_Gain->setDecimalsNum(1);
        ui->SP_Gain->setValue(_cfgItem->GetGain());

        ui->pushButton->setProperty("form", "Bypass");
        ui->pushButton->setChecked(true);
//        connect(ui->SP_Freq, SIGNAL(valueChanged(int)), this, SLOT(SP_valueChanged(int)));
//        connect(ui->SP_Gain, SIGNAL(valueChanged(int)), this, SLOT(SP_valueChanged(int)));
        connect(ui->SP_Freq, SIGNAL(valueChanged(int)), this, SLOT(SP_valueChanged(int)));
        connect(ui->SP_Gain, SIGNAL(valueChanged(double)), this, SLOT(SP_valueChanged(double)));

        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
#if Logo_FangTu
        initData_plus_FangTu();
#else
        ui_plus = new Ui::CfgSettingFreqShift_Plus;
        ui_plus->setupUi(this);

        ui_plus->SP_Freq->setRangeParam(0, 10, StepMode_Line, ParamType_Freq);
        ui_plus->SP_Freq->setFixedSize(40, 40);
        ui_plus->SP_Freq->setValue(_cfgItem->GetFreq());

        ui_plus->SP_Gain->setRangeParam(-100, 0, StepMode_Line, ParamType_Gain);
        ui_plus->SP_Gain->setFixedSize(40, 40);
        ui_plus->SP_Gain->setDecimalsNum(1);
        ui_plus->SP_Gain->setValue(_cfgItem->GetGain());

        ui_plus->SP_Phase->setRangeParam(0, 180, StepMode_Line, ParamType_Phase);
        ui_plus->SP_Phase->setFixedSize(40, 40);
        ui_plus->SP_Phase->setValue(_cfgItem->GetPhase());

        ui_plus->ShiftFreqMode->setCurrentIndex(_cfgItem->GetFreqMode());

        ui_plus->ShiftFreqSpeed->setRangeParam(1, 100, StepMode_Line, ParamType_Other);
        ui_plus->ShiftFreqSpeed->setFixedSize(40, 40);
        ui_plus->ShiftFreqSpeed->setValue(_cfgItem->GetFreqSpeed());

        ui_plus->ShiftFreqSpan->setRangeParam(0,2*_cfgItem->GetFreq(), StepMode_Line, ParamType_Freq);
        ui_plus->ShiftFreqSpan->setFixedSize(40, 40);
        ui_plus->ShiftFreqSpan->setValue(_cfgItem->GetFreqSpan());

        connect(ui_plus->SP_Freq, SIGNAL(valueChanged(int)), this, SLOT(SP_valueChanged(int)));
        connect(ui_plus->SP_Gain, SIGNAL(valueChanged(double)), this, SLOT(SP_valueChanged(double)));
        connect(ui_plus->SP_Phase, SIGNAL(valueChanged(int)), this, SLOT(SP_valueChanged(int)));
        connect(ui_plus->ShiftFreqSpan, SIGNAL(valueChanged(int)), this, SLOT(SP_valueChanged(int)));
        connect(ui_plus->ShiftFreqSpeed, SIGNAL(valueChanged(int)), this, SLOT(SP_valueChanged(int)));
        connect(ui_plus->ShiftFreqMode, SIGNAL(currentIndexChanged(int)), this, SLOT(SP_valueChanged(int)));
#endif
        break;
    default:
        break;
    }

}

void CfgSettingFreqShift::initData_plus_FangTu()
{
    ui_plus_FangTu = new Ui::CfgSettingFreqShift_Plus_FangTu;
    ui_plus_FangTu->setupUi(this);

    ui_plus_FangTu->SP_Freq->setRangeParam(0, 10, StepMode_Line, ParamType_Freq);
    ui_plus_FangTu->SP_Freq->setFixedSize(40, 40);
    ui_plus_FangTu->SP_Freq->setValue(_cfgItem->GetFreq());

    ui_plus_FangTu->SP_Gain->setRangeParam(-100, 0, StepMode_Line, ParamType_Gain);
    ui_plus_FangTu->SP_Gain->setFixedSize(40, 40);
    ui_plus_FangTu->SP_Gain->setDecimalsNum(1);
    ui_plus_FangTu->SP_Gain->setValue(_cfgItem->GetGain());

    ui_plus_FangTu->SP_Phase->setRangeParam(0, 180, StepMode_Line, ParamType_Phase);
    ui_plus_FangTu->SP_Phase->setFixedSize(40, 40);
    ui_plus_FangTu->SP_Phase->setValue(_cfgItem->GetPhase());

    ui_plus_FangTu->ShiftFreqMode->setCurrentIndex(_cfgItem->GetFreqMode());

    ui_plus_FangTu->ShiftFreqSpeed->setRangeParam(1, 100, StepMode_Line, ParamType_Other);
    ui_plus_FangTu->ShiftFreqSpeed->setFixedSize(40, 40);
    ui_plus_FangTu->ShiftFreqSpeed->setValue(_cfgItem->GetFreqSpeed());

    ui_plus_FangTu->ShiftFreqSpan->setRangeParam(0, 2*_cfgItem->GetFreq(), StepMode_Line, ParamType_Freq);
    ui_plus_FangTu->ShiftFreqSpan->setFixedSize(40, 40);
    ui_plus_FangTu->ShiftFreqSpan->setValue(_cfgItem->GetFreqSpan());

    connect(ui_plus_FangTu->SP_Freq, SIGNAL(valueChanged(int)), this, SLOT(SP_valueChanged(int)));
    connect(ui_plus_FangTu->SP_Gain, SIGNAL(valueChanged(double)), this, SLOT(SP_valueChanged(double)));
    connect(ui_plus_FangTu->SP_Phase, SIGNAL(valueChanged(int)), this, SLOT(SP_valueChanged(int)));
    connect(ui_plus_FangTu->ShiftFreqSpan, SIGNAL(valueChanged(int)), this, SLOT(SP_valueChanged(int)));
    connect(ui_plus_FangTu->ShiftFreqSpeed, SIGNAL(valueChanged(int)), this, SLOT(SP_valueChanged(int)));
    connect(ui_plus_FangTu->ShiftFreqMode, SIGNAL(currentIndexChanged(int)), this, SLOT(SP_valueChanged(int)));
}

void CfgSettingFreqShift::SP_valueChanged(int value)
{
//    qDebug() << __FUNCTION__ << "(int value)" << __LINE__  << value;
    switch (_cfgItem->getCfgType()) {
    case CFG_TYPE_FREQSHIFT:
        if (sender() == ui->SP_Freq) {
            if(ui->pushButton->isChecked()) _cfgItem->SetFreq(0);
            else _cfgItem->SetFreq(value);
        } else if (sender() == ui->SP_Gain) {
            _cfgItem->SetGain(value);
        }
        break;
    case CFG_TYPE_FREQSHIFT_PLUS:
#if Logo_FangTu
        if (sender() == ui_plus_FangTu->SP_Freq) {
            _cfgItem->SetFreq(value);
            ui_plus_FangTu->ShiftFreqSpan->setRange(0, 2*value);
        } else if (sender() == ui_plus_FangTu->SP_Gain) {
            _cfgItem->SetGain(value);
        } else if (sender() == ui_plus_FangTu->SP_Phase) {
            _cfgItem->SetPhase(value);
        } else if (sender() == ui_plus_FangTu->ShiftFreqMode) {
            _cfgItem->SetFreqMode(value);
        } else if (sender() == ui_plus_FangTu->ShiftFreqSpeed) {
            _cfgItem->SetFreqSpeed(value);
        } else if (sender() == ui_plus_FangTu->ShiftFreqSpan) {
            _cfgItem->SetFreqSpan(value);
        }
#else
        if (sender() == ui_plus->SP_Freq) {
            _cfgItem->SetFreq(value);
            ui_plus->ShiftFreqSpan->setRangeParam(0, 2*value, StepMode_Line, ParamType_Freq);
        } else if (sender() == ui_plus->SP_Gain) {
            _cfgItem->SetGain(value);
        } else if (sender() == ui_plus->SP_Phase) {
            _cfgItem->SetPhase(value);
        } else if (sender() == ui_plus->ShiftFreqMode) {
            _cfgItem->SetFreqMode(value);
        } else if (sender() == ui_plus->ShiftFreqSpeed) {
            _cfgItem->SetFreqSpeed(value);
        } else if (sender() == ui_plus->ShiftFreqSpan) {
            _cfgItem->SetFreqSpan(value);
        }
#endif
        break;
    default:
        break;
    }
}

void CfgSettingFreqShift::SP_valueChanged(double value)
{
//    qDebug() << __FUNCTION__ << "(double value)" << __LINE__  << value;
   switch (_cfgItem->getCfgType()) {
   case CFG_TYPE_FREQSHIFT:
       if (sender() == ui->SP_Freq) {
           _cfgItem->SetFreq(value);
       } else if (sender() == ui->SP_Gain) {
           _cfgItem->SetGain(value);
       }
       break;
   case CFG_TYPE_FREQSHIFT_PLUS:
#if Logo_FangTu
       if (sender() == ui_plus_FangTu->SP_Freq) {
           _cfgItem->SetFreq(value);
           ui_plus_FangTu->ShiftFreqSpan->setRange(0, 2*value);
       } else if (sender() == ui_plus_FangTu->SP_Gain) {
           _cfgItem->SetGain(value);
       } else if (sender() == ui_plus_FangTu->SP_Phase) {
      _cfgItem->SetPhase(value);
       } else if (sender() == ui_plus_FangTu->ShiftFreqMode) {
           _cfgItem->SetFreqMode(value);
       } else if (sender() == ui_plus_FangTu->ShiftFreqSpeed) {
           _cfgItem->SetFreqSpeed(value);
       } else if (sender() == ui_plus_FangTu->ShiftFreqSpan) {
           _cfgItem->SetFreqSpan(value);
       }
#else
       if (sender() == ui_plus->SP_Freq) {
           _cfgItem->SetFreq(value);
           ui_plus->ShiftFreqSpan->setRangeParam(0, 2*value, StepMode_Line, ParamType_Freq);
       } else if (sender() == ui_plus->SP_Gain) {
           _cfgItem->SetGain(value);
       } else if (sender() == ui_plus->SP_Phase) {
      _cfgItem->SetPhase(value);
       } else if (sender() == ui_plus->ShiftFreqMode) {
           _cfgItem->SetFreqMode(value);
       } else if (sender() == ui_plus->ShiftFreqSpeed) {
           _cfgItem->SetFreqSpeed(value);
       } else if (sender() == ui_plus->ShiftFreqSpan) {
           _cfgItem->SetFreqSpan(value);
       }
#endif
       break;
   default:
       break;
   }
}


void CfgSettingFreqShift::on_pushButton_clicked(bool checked)
{
    if (checked) {
        _cfgItem->SetFreq(0);
    }
    else {
        _cfgItem->SetFreq(ui->SP_Freq->value());
    }
}
