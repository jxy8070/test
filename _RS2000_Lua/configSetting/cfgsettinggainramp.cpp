#include "cfgsettinggainramp.h"
#include "ui_cfgsettinggainramp.h"
#include "configItem/configitemgainramp.h"
#include "ui_CfgSettingGainRamp_FangTu.h"

CfgSettingGainRamp::CfgSettingGainRamp(ConfigItemGainRamp *cfgItem, QWidget *parent) :
    QWidget(parent)
//  ,ui(new Ui::CfgSettingGainRamp)
{
    _cfgItem = cfgItem;
#if Logo_FangTu
    _ui_FangTu = new Ui::CfgSettingGainRamp_FangTu;
    _ui_FangTu->setupUi(this);
    initUI_FangTu();
    initData_FangTu();
#else
    _uiGain = new Ui::CfgSettingGainRamp;
    _uiGain->setupUi(this);
    initUI();
    initData();
#endif

}

CfgSettingGainRamp::~CfgSettingGainRamp()
{
    if(_uiGain)
        delete _uiGain;
    if(_ui_FangTu)
        delete _ui_FangTu;
}

void CfgSettingGainRamp::updateUiData()
{
#if Logo_FangTu
    initData_FangTu();
#else
    initData();
#endif
}

void CfgSettingGainRamp::DispRtnData(unsigned int *d)
{
#if Logo_FangTu
    IG_PARAGET_GAINRAMP *p = (IG_PARAGET_GAINRAMP *)d;
    int rampGain = p->rtRampGain*0.01-100;
    _ui_FangTu->label_Gain->setText(QString::asprintf("%d dB", rampGain));
#else
    IG_PARAGET_GAINRAMP *p = (IG_PARAGET_GAINRAMP *)d;
    int rampGain = p->rtRampGain*0.01-100;
    _uiGain->label_Gain->setText(QString::asprintf("%d dB", rampGain));
#endif
}

void CfgSettingGainRamp::on_pushButton_A_clicked(bool checked)
{
#if Logo_FangTu
    if (checked) {
        _ui_FangTu->pushButton_A->setChecked(true);
        _ui_FangTu->pushButton_B->setChecked(false);
    }
    else {
        _ui_FangTu->pushButton_A->setChecked(false);
        _ui_FangTu->pushButton_B->setChecked(true);
    }

    if (_ui_FangTu->pushButton_Mute->isChecked() == false) {
        _cfgItem->set_effectGain(checked ? 1 : 2);
    }
#else
    if (checked) {
        _uiGain->pushButton_A->setChecked(true);
        _uiGain->pushButton_B->setChecked(false);
    }
    else {
        _uiGain->pushButton_A->setChecked(false);
        _uiGain->pushButton_B->setChecked(true);
    }

    if (_uiGain->pushButton_Mute->isChecked() == false) {
        _cfgItem->set_effectGain(checked ? 1 : 2);
    }
#endif
}

void CfgSettingGainRamp::on_pushButton_B_clicked(bool checked)
{
#if Logo_FangTu
    if (checked) {
        _ui_FangTu->pushButton_A->setChecked(false);
        _ui_FangTu->pushButton_B->setChecked(true);
    }
    else {
        _ui_FangTu->pushButton_A->setChecked(true);
        _ui_FangTu->pushButton_B->setChecked(false);
    }

    if (_ui_FangTu->pushButton_Mute->isChecked() == false) {
        _cfgItem->set_effectGain(checked ? 2 : 1);
    }
#else
    if (checked) {
        _uiGain->pushButton_A->setChecked(false);
        _uiGain->pushButton_B->setChecked(true);
    }
    else {
        _uiGain->pushButton_A->setChecked(true);
        _uiGain->pushButton_B->setChecked(false);
    }

    if (_uiGain->pushButton_Mute->isChecked() == false) {
        _cfgItem->set_effectGain(checked ? 2 : 1);
    }
#endif
}

void CfgSettingGainRamp::on_pushButton_Mute_clicked(bool checked)
{
#if Logo_FangTu
    if (checked) {
        _cfgItem->set_effectGain(0);
    }
    else {
        if (_ui_FangTu->pushButton_A->isChecked()) {
            _cfgItem->set_effectGain(1);
        }
        else if (_ui_FangTu->pushButton_B->isChecked()) {
            _cfgItem->set_effectGain(2);
        }
    }
#else
    if (checked) {
        _cfgItem->set_effectGain(0);
    }
    else {
        if (_uiGain->pushButton_A->isChecked()) {
            _cfgItem->set_effectGain(1);
        }
        else if (_uiGain->pushButton_B->isChecked()) {
            _cfgItem->set_effectGain(2);
        }
    }
#endif
}

void CfgSettingGainRamp::on_dial_TimeA_valueChanged(int value)
{
#if Logo_FangTu
    _cfgItem->set_targetTimeA(value);

    float rateA = abs(_ui_FangTu->dial_GainA->value() - _ui_FangTu->dial_GainB->value()) / (_ui_FangTu->dial_TimeA->value() / 1000.0f);
    _ui_FangTu->label_RateA->setText(QString::asprintf("%.2f", rateA));
#else
    _cfgItem->set_targetTimeA(value);

    float rateA = abs(_uiGain->dial_GainA->value() - _uiGain->dial_GainB->value()) / (_uiGain->dial_TimeA->value() / 1000.0f);
    _uiGain->label_RateA->setText(QString::asprintf("%.2f", rateA));
#endif
}

void CfgSettingGainRamp::on_dial_TimeMute_valueChanged(int value)
{    
    _cfgItem->set_muteTime(value);
}

void CfgSettingGainRamp::on_dial_TimeB_valueChanged(int value)
{
#if Logo_FangTu
    _cfgItem->set_targetTimeB(value);

    float rateB = abs(_ui_FangTu->dial_GainA->value() - _ui_FangTu->dial_GainB->value()) / (_ui_FangTu->dial_TimeB->value() / 1000.0f);
    _ui_FangTu->label_RateB->setText(QString::asprintf("%.2f", rateB));
#else
    _cfgItem->set_targetTimeB(value);

    float rateB = abs(_uiGain->dial_GainA->value() - _uiGain->dial_GainB->value()) / (_uiGain->dial_TimeB->value() / 1000.0f);
    _uiGain->label_RateB->setText(QString::asprintf("%.2f", rateB));
#endif
}

void CfgSettingGainRamp::on_dial_GainA_valueChanged(int value)
{
#if Logo_FangTu
    _cfgItem->set_targetGainA(value);
    float rateA = abs(_ui_FangTu->dial_GainA->value() - _ui_FangTu->dial_GainB->value()) / (_ui_FangTu->dial_TimeA->value() / 1000.0f);
    float rateB = abs(_ui_FangTu->dial_GainA->value() - _ui_FangTu->dial_GainB->value()) / (_ui_FangTu->dial_TimeB->value() / 1000.0f);
    _ui_FangTu->label_RateA->setText(QString::asprintf("%.2f", rateA));
    _ui_FangTu->label_RateB->setText(QString::asprintf("%.2f", rateB));
#else
    _cfgItem->set_targetGainA(value);
    float rateA = abs(_uiGain->dial_GainA->value() - _uiGain->dial_GainB->value()) / (_uiGain->dial_TimeA->value() / 1000.0f);
    float rateB = abs(_uiGain->dial_GainA->value() - _uiGain->dial_GainB->value()) / (_uiGain->dial_TimeB->value() / 1000.0f);
    _uiGain->label_RateA->setText(QString::asprintf("%.2f", rateA));
    _uiGain->label_RateB->setText(QString::asprintf("%.2f", rateB));
#endif
}

void CfgSettingGainRamp::on_dial_GainB_valueChanged(int value)
{
#if Logo_FangTu
    _cfgItem->set_targetGainB(value);
    float rateA = abs(_ui_FangTu->dial_GainA->value() - _ui_FangTu->dial_GainB->value()) / (_ui_FangTu->dial_TimeA->value() / 1000.0f);
    float rateB = abs(_ui_FangTu->dial_GainA->value() - _ui_FangTu->dial_GainB->value()) / (_ui_FangTu->dial_TimeB->value() / 1000.0f);
    _ui_FangTu->label_RateA->setText(QString::asprintf("%.2f", rateA));
    _ui_FangTu->label_RateB->setText(QString::asprintf("%.2f", rateB));
#else
    _cfgItem->set_targetGainB(value);
    float rateA = abs(_uiGain->dial_GainA->value() - _uiGain->dial_GainB->value()) / (_uiGain->dial_TimeA->value() / 1000.0f);
    float rateB = abs(_uiGain->dial_GainA->value() - _uiGain->dial_GainB->value()) / (_uiGain->dial_TimeB->value() / 1000.0f);
    _uiGain->label_RateA->setText(QString::asprintf("%.2f", rateA));
    _uiGain->label_RateB->setText(QString::asprintf("%.2f", rateB));
#endif
}

void CfgSettingGainRamp::initUI()
{
    _uiGain->pushButton_Mute->setProperty("form", "Mute");
    _uiGain->dial_TimeA->setRangeParam(100, 50000, StepMode_Log10, ParamType_Time);
    _uiGain->dial_TimeB->setRangeParam(100, 50000, StepMode_Log10, ParamType_Time);
    _uiGain->dial_GainA->setRangeParam(-100, 20, StepMode_Line, ParamType_Gain);
    _uiGain->dial_GainB->setRangeParam(-100, 20, StepMode_Line, ParamType_Gain);
    _uiGain->dial_TimeMute->setRangeParam(100, 50000, StepMode_Log10, ParamType_Time);

}

void CfgSettingGainRamp::initData()
{
    int effect = _cfgItem->get_effectGain();
    switch (effect) {
    case 0:
        _uiGain->pushButton_Mute->setChecked(true);
        _uiGain->pushButton_A->setChecked(false);
        _uiGain->pushButton_B->setChecked(false);
        break;
    case 1:
        _uiGain->pushButton_Mute->setChecked(false);
        _uiGain->pushButton_A->setChecked(true);
        _uiGain->pushButton_B->setChecked(false);
        break;
    case 2:
        _uiGain->pushButton_Mute->setChecked(false);
        _uiGain->pushButton_A->setChecked(false);
        _uiGain->pushButton_B->setChecked(true);
        break;
    default: break;
    }

    _uiGain->dial_TimeA->setValue(_cfgItem->get_targetTimeA());

    _uiGain->dial_TimeB->setValue(_cfgItem->get_targetTimeB());

    _uiGain->dial_TimeMute->setValue(_cfgItem->get_muteTime());

    _uiGain->dial_GainA->setValue(_cfgItem->get_targetGainA());
    _uiGain->dial_GainB->setValue(_cfgItem->get_targetGainB());

    float rateA = abs(_uiGain->dial_GainA->value() - _uiGain->dial_GainB->value()) / (_uiGain->dial_TimeA->value() / 1000.0f);
    float rateB = abs(_uiGain->dial_GainA->value() - _uiGain->dial_GainB->value()) / (_uiGain->dial_TimeB->value() / 1000.0f);
    _uiGain->label_RateA->setText(QString::asprintf("%.2f", rateA));
    _uiGain->label_RateB->setText(QString::asprintf("%.2f", rateB));
}


void CfgSettingGainRamp::initUI_FangTu()
{
    _ui_FangTu->dial_TimeA->setRangeParam(100, 50000, StepMode_Log10, ParamType_Time);
    _ui_FangTu->dial_TimeB->setRangeParam(100, 50000, StepMode_Log10, ParamType_Time);
    _ui_FangTu->dial_GainA->setRangeParam(-100, 20, StepMode_Line, ParamType_Gain);
    _ui_FangTu->dial_GainB->setRangeParam(-100, 20, StepMode_Line, ParamType_Gain);
    _ui_FangTu->dial_TimeMute->setRangeParam(100, 50000, StepMode_Log10, ParamType_Time);

    _ui_FangTu->pushButton_Mute->setProperty("form", "Mute");
}

void CfgSettingGainRamp::initData_FangTu()
{
    int effect = _cfgItem->get_effectGain();
    switch (effect) {
    case 0:
        _ui_FangTu->pushButton_Mute->setChecked(true);
        _ui_FangTu->pushButton_A->setChecked(false);
        _ui_FangTu->pushButton_B->setChecked(false);
        break;
    case 1:
        _ui_FangTu->pushButton_Mute->setChecked(false);
        _ui_FangTu->pushButton_A->setChecked(true);
        _ui_FangTu->pushButton_B->setChecked(false);
        break;
    case 2:
        _ui_FangTu->pushButton_Mute->setChecked(false);
        _ui_FangTu->pushButton_A->setChecked(false);
        _ui_FangTu->pushButton_B->setChecked(true);
        break;
    default: break;
    }

    _ui_FangTu->dial_TimeA->setValue(_cfgItem->get_targetTimeA());

    _ui_FangTu->dial_TimeB->setValue(_cfgItem->get_targetTimeB());

    _ui_FangTu->dial_TimeMute->setValue(_cfgItem->get_muteTime());

    _ui_FangTu->dial_GainA->setValue(_cfgItem->get_targetGainA());
    _ui_FangTu->dial_GainB->setValue(_cfgItem->get_targetGainB());

    float rateA = abs(_ui_FangTu->dial_GainA->value() - _ui_FangTu->dial_GainB->value()) / (_ui_FangTu->dial_TimeA->value() / 1000.0f);
    float rateB = abs(_ui_FangTu->dial_GainA->value() - _ui_FangTu->dial_GainB->value()) / (_ui_FangTu->dial_TimeB->value() / 1000.0f);
    _ui_FangTu->label_RateA->setText(QString::asprintf("%.2f", rateA));
    _ui_FangTu->label_RateB->setText(QString::asprintf("%.2f", rateB));
}

