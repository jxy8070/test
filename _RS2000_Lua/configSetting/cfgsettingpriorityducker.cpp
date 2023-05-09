#include "cfgsettingpriorityducker.h"
#include "ui_cfgsettingpriorityducker.h"
#include "configItem/configitempriorityducker.h"
#include "ui_CfgSettingPriorityDucker_FangTu.h"

CfgSettingPriorityDucker::CfgSettingPriorityDucker(ConfigItemPriorityDucker *cfgItem, QWidget *parent) :
    QWidget(parent)
{
    _cfgItem = cfgItem;
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
        _ui_FangTu = new Ui::CfgSettingPriorityDucker_FangTu;
        _ui_FangTu->setupUi(this);
        initUi_FangTu();
    }
    else {
        _uiDuck = new Ui::CfgSettingPriorityDucker;
        _uiDuck->setupUi(this);
        initUi();
    }
}

CfgSettingPriorityDucker::~CfgSettingPriorityDucker()
{
    if(_uiDuck)
        delete _uiDuck;
    if(_ui_FangTu)
        delete _ui_FangTu;
}

void CfgSettingPriorityDucker::updateUiData()
{
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
        _ui_FangTu->verticalSlider_InGain->setValue(_cfgItem->GetInGain());
        _ui_FangTu->label_InGain->setText(QString::asprintf("%d dB", _cfgItem->GetInGain()));
        _ui_FangTu->pushButton_Bypass->setChecked(_cfgItem->GetBypass());
        _ui_FangTu->dynGhaph->setBypass(_cfgItem->GetBypass());
        _ui_FangTu->dial_ThresholdLevel->setValue(_cfgItem->GetThresholdLevel());
        _ui_FangTu->dial_Depth->setValue(_cfgItem->GetDepth());
        _ui_FangTu->dial_PriorityGain->setValue(_cfgItem->GetPriorityGain());
        _ui_FangTu->dial_AttackTime->setValue(_cfgItem->GetAttackTime());
        _ui_FangTu->dial_HoldTime->setValue(_cfgItem->GetHoldTime());
        _ui_FangTu->dial_ReleaseTime->setValue(_cfgItem->GetReleaseTime());
        _ui_FangTu->dial_DetectorTime->setValue(_cfgItem->GetDetectorTime());
        _ui_FangTu->verticalSlider_OutGain->setValue(_cfgItem->GetOutGain());
        _ui_FangTu->label_OutGain->setText(QString::asprintf("%d dB", _cfgItem->GetOutGain()));
    }
    else{
        _uiDuck->verticalSlider_InGain->setValue(_cfgItem->GetInGain());
        _uiDuck->label_InGain->setText(QString::asprintf("%d dB", _cfgItem->GetInGain()));
        _uiDuck->pushButton_Bypass->setChecked(_cfgItem->GetBypass());
        _uiDuck->dynGhaph->setBypass(_cfgItem->GetBypass());
        _uiDuck->dial_ThresholdLevel->setValue(_cfgItem->GetThresholdLevel());
        _uiDuck->dial_Depth->setValue(_cfgItem->GetDepth());
        _uiDuck->dial_PriorityGain->setValue(_cfgItem->GetPriorityGain());
        _uiDuck->dial_AttackTime->setValue(_cfgItem->GetAttackTime());
        _uiDuck->dial_HoldTime->setValue(_cfgItem->GetHoldTime());
        _uiDuck->dial_ReleaseTime->setValue(_cfgItem->GetReleaseTime());
        _uiDuck->dial_DetectorTime->setValue(_cfgItem->GetDetectorTime());
        _uiDuck->verticalSlider_OutGain->setValue(_cfgItem->GetOutGain());
        _uiDuck->label_OutGain->setText(QString::asprintf("%d dB", _cfgItem->GetOutGain()));
    }
}

void CfgSettingPriorityDucker::DispRtnData(unsigned int *d)
{
    IG_PARAGET_PDUCKER *p = (IG_PARAGET_PDUCKER *)d;
    double applied = p->mainChDuckerAppliedGain*0.01-100;
    double detect  = p->detectorPriorChInLevel*0.01-100;

    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
        _ui_FangTu->Disp_AppliedGain->setLCurValue(applied);
        _ui_FangTu->label_AppliedGain->setText(QString::asprintf("%.1f dB", applied));
        _ui_FangTu->Disp_Detector->setLCurValue(detect);
        _ui_FangTu->label_Detector->setText(QString::asprintf("%.1f dB", detect));
    }
    else{
        _uiDuck->Disp_AppliedGain->setLCurValue(applied);
        _uiDuck->label_AppliedGain->setText(QString::asprintf("%.1f dB", applied));
        _uiDuck->Disp_Detector->setLCurValue(detect);
        _uiDuck->label_Detector->setText(QString::asprintf("%.1f dB", detect));
    }
}

void CfgSettingPriorityDucker::initUi()
{
    _uiDuck->dial_ThresholdLevel->setRangeParam(-60, 20, StepMode_Line, ParamType_Gain);
    _uiDuck->dial_Depth->setRangeParam(0, 100, StepMode_Line, ParamType_Gain);
    _uiDuck->dial_PriorityGain->setRangeParam(-100, 20, StepMode_Line, ParamType_Gain);

    _uiDuck->dial_AttackTime->setRangeParam(5, 10000, StepMode_Log10, ParamType_Time);
    _uiDuck->dial_HoldTime->setRangeParam(1, 30000, StepMode_Log10, ParamType_Time);

    _uiDuck->dial_ReleaseTime->setRangeParam(10, 10000, StepMode_Log10, ParamType_Time);
    _uiDuck->dial_DetectorTime->setRangeParam(0, 300, StepMode_Line, ParamType_Time);


    _uiDuck->verticalSlider_InGain->setValue(_cfgItem->GetInGain());
    _uiDuck->label_InGain->setText(QString::asprintf("%d dB", _cfgItem->GetInGain()));
    _uiDuck->pushButton_Bypass->setChecked(_cfgItem->GetBypass());
    _uiDuck->dial_ThresholdLevel->setValue(_cfgItem->GetThresholdLevel());
    _uiDuck->dial_Depth->setValue(_cfgItem->GetDepth());
    _uiDuck->dial_PriorityGain->setValue(_cfgItem->GetPriorityGain());
    _uiDuck->dial_AttackTime->setValue(_cfgItem->GetAttackTime());
    _uiDuck->dial_HoldTime->setValue(_cfgItem->GetHoldTime());
    _uiDuck->dial_ReleaseTime->setValue(_cfgItem->GetReleaseTime());
    _uiDuck->dial_DetectorTime->setValue(_cfgItem->GetDetectorTime());
    _uiDuck->verticalSlider_OutGain->setValue(_cfgItem->GetOutGain());
    _uiDuck->label_OutGain->setText(QString::asprintf("%d dB", _cfgItem->GetOutGain()));
    _uiDuck->pushButton_Bypass->setProperty("form", "Bypass");
}

void CfgSettingPriorityDucker::initUi_FangTu()
{
    _ui_FangTu->dial_ThresholdLevel->setRangeParam(-60, 20, StepMode_Line, ParamType_Gain);
    _ui_FangTu->dial_Depth->setRangeParam(0, 100, StepMode_Line, ParamType_Gain);
    _ui_FangTu->dial_PriorityGain->setRangeParam(-100, 20, StepMode_Line, ParamType_Gain);

    _ui_FangTu->dial_AttackTime->setRangeParam(5, 10000, StepMode_Log10, ParamType_Time);
    _ui_FangTu->dial_HoldTime->setRangeParam(1, 30000, StepMode_Log10, ParamType_Time);

    _ui_FangTu->dial_ReleaseTime->setRangeParam(10, 10000, StepMode_Log10, ParamType_Time);
    _ui_FangTu->dial_DetectorTime->setRangeParam(0, 300, StepMode_Line, ParamType_Time);


    _ui_FangTu->verticalSlider_InGain->setValue(_cfgItem->GetInGain());
    _ui_FangTu->label_InGain->setText(QString::asprintf("%d dB", _cfgItem->GetInGain()));
    _ui_FangTu->pushButton_Bypass->setChecked(_cfgItem->GetBypass());
    _ui_FangTu->dial_ThresholdLevel->setValue(_cfgItem->GetThresholdLevel());
    _ui_FangTu->dial_Depth->setValue(_cfgItem->GetDepth());
    _ui_FangTu->dial_PriorityGain->setValue(_cfgItem->GetPriorityGain());
    _ui_FangTu->dial_AttackTime->setValue(_cfgItem->GetAttackTime());
    _ui_FangTu->dial_HoldTime->setValue(_cfgItem->GetHoldTime());
    _ui_FangTu->dial_ReleaseTime->setValue(_cfgItem->GetReleaseTime());
    _ui_FangTu->dial_DetectorTime->setValue(_cfgItem->GetDetectorTime());
    _ui_FangTu->verticalSlider_OutGain->setValue(_cfgItem->GetOutGain());
    _ui_FangTu->label_OutGain->setText(QString::asprintf("%d dB", _cfgItem->GetOutGain()));
    _ui_FangTu->pushButton_Bypass->setProperty("form", "Bypass");
    setFixedSize(490, 520);
}



void CfgSettingPriorityDucker::on_verticalSlider_InGain_valueChanged(int value)
{
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
        _ui_FangTu->label_InGain->setText(QString::asprintf("%d dB", value));
    }
    else{
        _uiDuck->label_InGain->setText(QString::asprintf("%d dB", value));
    }
    _cfgItem->SetInGain(value);
}

void CfgSettingPriorityDucker::on_verticalSlider_OutGain_valueChanged(int value)
{
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
        _ui_FangTu->label_OutGain->setText(QString::asprintf("%d dB", value));
    }
    else{
        _uiDuck->label_OutGain->setText(QString::asprintf("%d dB", value));
    }
    _cfgItem->SetOutGain(value);
}


void CfgSettingPriorityDucker::on_pushButton_Bypass_clicked(bool checked)
{
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
        _ui_FangTu->dynGhaph->setBypass(checked);
    }
    else{
        _uiDuck->dynGhaph->setBypass(checked);
    }
    _cfgItem->SetBypass(checked);
}

void CfgSettingPriorityDucker::on_dial_ThresholdLevel_valueChanged(int value)
{
    _cfgItem->SetThresholdLevel(value);
}

void CfgSettingPriorityDucker::on_dial_Depth_valueChanged(int value)
{
    _cfgItem->SetDepth(value);
}

void CfgSettingPriorityDucker::on_dial_PriorityGain_valueChanged(int value)
{
    _cfgItem->SetPriorityGain(value);
}

void CfgSettingPriorityDucker::on_dial_AttackTime_valueChanged(int value)
{
    _cfgItem->SetAttatkTime(value);
}

void CfgSettingPriorityDucker::on_dial_HoldTime_valueChanged(int value)
{
    _cfgItem->SetHoldTime(value);
}

void CfgSettingPriorityDucker::on_dial_ReleaseTime_valueChanged(int value)
{
    _cfgItem->SetReleaseTime(value);
}


void CfgSettingPriorityDucker::on_dial_DetectorTime_valueChanged(int value)
{
    _cfgItem->SetDetectorTime(value);
}






