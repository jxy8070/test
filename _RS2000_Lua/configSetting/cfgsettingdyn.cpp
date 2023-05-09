#include "cfgsettingdyn.h"
#include "configItem/configitemdyn.h"
#include "ui_cfgsettingdyn.h"
#include "ui_cfgsettingdyn_Compressor_FangTu.h"
#include "ui_cfgsettingdyn_Leveler_FangTu.h"
#include "ui_cfgsettingdyn_Expander_FangTu.h"
#include "ui_cfgsettingdyn_Limiter_FangTu.h"
#include "ui_cfgsettingdyn_NoiseGate_FangTu.h"


CfgSettingDyn::CfgSettingDyn(ConfigItemDyn *cfgItem, QWidget *parent)
    :QWidget(parent)
{
    _cfgItem = cfgItem;
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
        initUi_FangTu();
        initData_FangTu();
        initSignalConnect_FangTu();
    }
    else{
        initUi();
        initData();
        initSignalConnect();
    }
}

void CfgSettingDyn::setMeterDisp(float i, float o)
{
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
    {
        switch(_cfgItem->getCfgType()) {
        case CFG_TYPE_DYN_AUTO:
            _uiLeveler_FangTu->Disp_Level_i->setLCurValue(i);
            _uiLeveler_FangTu->Disp_Level_o->setLCurValue(o);
            _uiLeveler_FangTu->widget->curPointDisp(i, o);
            break;
        case CFG_TYPE_DYN_COMP:
            _uiCompressor_FangTu->Disp_Level_i->setLCurValue(i);
            _uiCompressor_FangTu->Disp_Level_o->setLCurValue(o);
            _uiCompressor_FangTu->widget->curPointDisp(i, o);
            break;
        case CFG_TYPE_DYN_LIMIT:
            _uiLimiter_FangTu->Disp_Level_i->setLCurValue(i);
            _uiLimiter_FangTu->Disp_Level_o->setLCurValue(o);
            _uiLimiter_FangTu->widget->curPointDisp(i, o);
            break;
        case CFG_TYPE_DYN_NOISE:
            _uiNoiseGate_FangTu->Disp_Level_i->setLCurValue(i);
            _uiNoiseGate_FangTu->Disp_Level_o->setLCurValue(o);
            _uiNoiseGate_FangTu->widget->curPointDisp(i, o);
            break;
        case CFG_TYPE_DYN_EXPAND:
            _uiExpander_FangTu->Disp_Level_i->setLCurValue(i);
            _uiExpander_FangTu->Disp_Level_o->setLCurValue(o);
            _uiExpander_FangTu->widget->curPointDisp(i, o);
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    else
    {
        _uiDyn->appliedGain->setLCurValue(o-i);
        _uiDyn->dynGraph->curPointDisp(i, o);
        _uiDyn->txtGain->setText(QString::number(o-i,'f',1));
    }
}

void CfgSettingDyn::updateUiData()
{
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
    {
        initData_FangTu();
    }
    else{
        initData();
    }
}

void CfgSettingDyn::initUi()
{
    CfgType cfgType = _cfgItem->getCfgType();
    //初始化参数
    _uiDyn = new Ui::cfgsettingdyn;
    _uiDyn->setupUi(this);
    _uiDyn->gridLayout->setAlignment(Qt::AlignHCenter| Qt::AlignBottom);
    _uiDyn->dynGraph->setType(cfgType);
    _uiDyn->dynGraph->setCurPointDisp(true);
    _uiDyn->appliedGain->setScaleLabel(true);//电平表参数
    _uiDyn->appliedGain->setSingleTrack(true);
    _uiDyn->appliedGain->setIsApplidGain(true);
    if(cfgType == CFG_TYPE_DYN_AUTO){
        _uiDyn->appliedGain->setMaxValue(40);
        _uiDyn->appliedGain->setMinValue(-20);
    }
    else{
        _uiDyn->appliedGain->setMaxValue(0);
        _uiDyn->appliedGain->setMinValue(-40);
    }

    _uiDyn->_gainThreshold->setRangeParam(-60, 12, StepMode_Line, ParamType_Gain);
    _uiDyn->_gainThreshold->setDecimalsNum(1);
    _uiDyn->_gainTarget->setDecimalsNum(1);
    if(cfgType == CFG_TYPE_DYN_NOISE){
        _uiDyn->lblGainTarget->setText(tr("Gain\n(dB)"));
        _uiDyn->_gainTarget->setRangeParam(-60, 0, StepMode_Line, ParamType_Gain);
    }
    else{
        _uiDyn->_gainTarget->setRangeParam(-60, 12, StepMode_Line, ParamType_Gain);
    }

    if(cfgType == CFG_TYPE_DYN_EXPAND){
        _uiDyn->_dynRatio->setRangeParam(0.2, 1, StepMode_Line, ParamType_Other);
    }
    else{
        _uiDyn->_dynRatio->setRangeParam(1, 100, StepMode_Log10, ParamType_Other);
    }
    _uiDyn->_dynRatio->setDecimalsNum(2);

    if(cfgType == CFG_TYPE_DYN_AUTO){
        _uiDyn->lblTimeAttack->setText(tr("setResponse\nTime(ms)"));
    }
    _uiDyn->_timeAttack->setRangeParam(5, 2000, StepMode_Log10, ParamType_Time);
    _uiDyn->_timeAttack->setDecimalsNum(2);
    _uiDyn->_timeRelease->setRangeParam(10, 4000, StepMode_Log10, ParamType_Time);
    _uiDyn->_timeRelease->setDecimalsNum(2);
    _uiDyn->pb_bypass->setProperty("form", "Bypass");

    //根据类别隐藏不需要的控件
    if(cfgType == CFG_TYPE_DYN_COMP){
        _uiDyn->lblGainTarget->hide();
        _uiDyn->_gainTarget->hide();
    }
    else if(cfgType == CFG_TYPE_DYN_LIMIT){
        _uiDyn->lblGainTarget->hide();
        _uiDyn->_gainTarget->hide();
        _uiDyn->lblDynRadio->hide();
        _uiDyn->_dynRatio->hide();
    }
    else if(cfgType == CFG_TYPE_DYN_NOISE){
        _uiDyn->lblDynRadio->hide();
        _uiDyn->_dynRatio->hide();
    }
    else if(cfgType == CFG_TYPE_DYN_EXPAND){
        _uiDyn->lblGainTarget->hide();
        _uiDyn->_gainTarget->hide();
    }
}


void CfgSettingDyn::initData()
{
    _uiDyn->_gainThreshold->setValue(_cfgItem->getThreshold() );
    _uiDyn->dynGraph->thresholdChanged(_cfgItem->getThreshold());

    _uiDyn->_gainTarget->setValue(_cfgItem->getGain());
    _uiDyn->dynGraph->outTargetLevelChanged(_cfgItem->getGain());

    _uiDyn->_timeAttack->setValue(_cfgItem->getAttack());
    _uiDyn->_timeRelease->setValue(_cfgItem->getRelease());

    _uiDyn->_dynRatio->setValue(_cfgItem->getRatio());
    _uiDyn->dynGraph->ratioChanged(_cfgItem->getRatio());

    _uiDyn->pb_bypass->setChecked(_cfgItem->getBypass());
    _uiDyn->dynGraph->setBypass(_cfgItem->getBypass());
}

void CfgSettingDyn::initSignalConnect()
{
    connect(_uiDyn->_gainThreshold, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot(double)));
    connect(_uiDyn->_gainTarget, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot(double)));
    connect(_uiDyn->_timeAttack, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot(double)));
    connect(_uiDyn->_timeRelease, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot(double)));
    connect(_uiDyn->_dynRatio, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot(double)));

    connect(_uiDyn->pb_bypass, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
}


void CfgSettingDyn::processSilderSlot(double value)
{
    if (sender() == _uiDyn->_gainThreshold)
    {
        if(_cfgItem->getCfgType() ==  CFG_TYPE_DYN_NOISE){
            _uiDyn->dynGraph->thresholdChanged(value);
            _cfgItem->setThreshold(value);
        }
        else{
            if (value > _uiDyn->_gainTarget->value())
            {
                value = _uiDyn->_gainTarget->value();
                _uiDyn->_gainThreshold->setValue(value);
            }

            _uiDyn->dynGraph->thresholdChanged(value);
            _cfgItem->setThreshold(value);
        }
    }
    else if (sender() == _uiDyn->_gainTarget)
    {
        if(_cfgItem->getCfgType() ==  CFG_TYPE_DYN_NOISE){
            _uiDyn->dynGraph->gainChanged(value);
            _cfgItem->setGain(value);
        }
        else{
            if (value < _uiDyn->_gainThreshold->value()) {
                value = _uiDyn->_gainThreshold->value();
                _uiDyn->_gainTarget->setValue(value);
            }
            _uiDyn->dynGraph->outTargetLevelChanged(value);
            _cfgItem->setGain(value);
        }
    }
    else if (sender() == _uiDyn->_timeAttack)
    {
        _cfgItem->setAttack(value);
    }
    else if (sender() == _uiDyn->_timeRelease)
    {
        _cfgItem->setRelease(value);
    }
    else if (sender() == _uiDyn->_dynRatio)
    {
        _uiDyn->dynGraph->ratioChanged(value);
        _cfgItem->setRatio(value);
    }
}


void CfgSettingDyn::initUi_FangTu()
{
//    qDebug() << __FUNCTION__ << __LINE__ << QString::number(_cfgItem->getCfgType(), 16);
    switch(_cfgItem->getCfgType()) {
    case CFG_TYPE_DYN_AUTO:
        _uiLeveler_FangTu = new Ui::cfgsettingdyn_Leveler_FangTu;
        _uiLeveler_FangTu->setupUi(this);
        _uiLeveler_FangTu->widget->setType(CFG_TYPE_DYN_AUTO);
        _uiLeveler_FangTu->widget->setCurPointDisp(true);

        _uiLeveler_FangTu->slider_Threshold->setRangeParam(-60, 12, StepMode_Line, ParamType_Gain);

        _uiLeveler_FangTu->slider_Ratio->setRangeParam(1, 100, StepMode_Log10, ParamType_Other);
        _uiLeveler_FangTu->slider_Ratio->setDecimalsNum(2);

        _uiLeveler_FangTu->slider_OutTargetLevel->setRangeParam(-60, 12, StepMode_Line, ParamType_Gain);


        _uiLeveler_FangTu->slider_Response->setRangeParam(5, 2000, StepMode_Log10, ParamType_Time);
        _uiLeveler_FangTu->slider_Response->setDecimalsNum(2);

        _uiLeveler_FangTu->slider_Release->setRangeParam(10, 4000, StepMode_Log10, ParamType_Time);
        _uiLeveler_FangTu->slider_Release->setDecimalsNum(2);

        _uiLeveler_FangTu->pb_bypass->setProperty("form", "Bypass");
        break;
    case CFG_TYPE_DYN_COMP:
        _uiCompressor_FangTu = new Ui::cfgsettingdyn_Compressor_FangTu;
        _uiCompressor_FangTu->setupUi(this);
        _uiCompressor_FangTu->widget->setType(CFG_TYPE_DYN_COMP);
        _uiCompressor_FangTu->widget->setCurPointDisp(true);

        _uiCompressor_FangTu->slider_Threshold->setRangeParam(-60, 12, StepMode_Line, ParamType_Gain);
        _uiCompressor_FangTu->slider_Threshold->setDecimalsNum(1);

        _uiCompressor_FangTu->slider_Ratio->setRangeParam(1, 100, StepMode_Log10, ParamType_Other);
        _uiCompressor_FangTu->slider_Ratio->setDecimalsNum(1);

        _uiCompressor_FangTu->slider_Response->setRangeParam(5, 2000, StepMode_Log10, ParamType_Time);
        _uiCompressor_FangTu->slider_Response->setDecimalsNum(2);

        _uiCompressor_FangTu->slider_Release->setRangeParam(10, 4000, StepMode_Log10, ParamType_Time);
        _uiCompressor_FangTu->slider_Release->setDecimalsNum(2);

        _uiCompressor_FangTu->pb_bypass->setProperty("form", "Bypass");
        break;
    case CFG_TYPE_DYN_LIMIT:
        _uiLimiter_FangTu = new Ui::cfgsettingdyn_Limiter_FangTu;
        _uiLimiter_FangTu->setupUi(this);
        _uiLimiter_FangTu->widget->setType(CFG_TYPE_DYN_LIMIT);
        _uiLimiter_FangTu->widget->setCurPointDisp(true);

        _uiLimiter_FangTu->slider_Threshold->setRangeParam(-60, 12, StepMode_Line, ParamType_Gain);
        _uiLimiter_FangTu->slider_Threshold->setDecimalsNum(1);

        _uiLimiter_FangTu->slider_Response->setRangeParam(5, 2000, StepMode_Log10, ParamType_Time);
        _uiLimiter_FangTu->slider_Response->setDecimalsNum(2);

        _uiLimiter_FangTu->slider_Release->setRangeParam(10, 4000, StepMode_Log10, ParamType_Time);
        _uiLimiter_FangTu->slider_Release->setDecimalsNum(2);

        _uiLimiter_FangTu->pb_bypass->setProperty("form", "Bypass");
        break;
    case CFG_TYPE_DYN_NOISE:
        _uiNoiseGate_FangTu = new Ui::cfgsettingdyn_NoiseGate_FangTu;
        _uiNoiseGate_FangTu->setupUi(this);
        _uiNoiseGate_FangTu->widget->setType(CFG_TYPE_DYN_NOISE);
        _uiNoiseGate_FangTu->widget->setCurPointDisp(true);

        _uiNoiseGate_FangTu->slider_Threshold->setRangeParam(-60, 12, StepMode_Line, ParamType_Gain);
        _uiNoiseGate_FangTu->slider_Threshold->setDecimalsNum(1);

        _uiNoiseGate_FangTu->slider_Gain->setRangeParam(-60, 0, StepMode_Line, ParamType_Gain);
        _uiNoiseGate_FangTu->slider_Gain->setDecimalsNum(2);

        _uiNoiseGate_FangTu->slider_Response->setRangeParam(5, 2000, StepMode_Log10, ParamType_Time);
        _uiNoiseGate_FangTu->slider_Response->setDecimalsNum(2);

        _uiNoiseGate_FangTu->slider_Release->setRangeParam(10, 4000, StepMode_Log10, ParamType_Time);
        _uiNoiseGate_FangTu->slider_Release->setDecimalsNum(2);

        _uiNoiseGate_FangTu->pb_bypass->setProperty("form", "Bypass");
        break;
    case CFG_TYPE_DYN_EXPAND:
        _uiExpander_FangTu = new Ui::cfgsettingdyn_Expander_FangTu;
        _uiExpander_FangTu->setupUi(this);
        _uiExpander_FangTu->widget->setType(CFG_TYPE_DYN_EXPAND);
        _uiExpander_FangTu->widget->setCurPointDisp(true);

        _uiExpander_FangTu->slider_Threshold->setRangeParam(-60, 12, StepMode_Line, ParamType_Gain);
        _uiExpander_FangTu->slider_Threshold->setDecimalsNum(1);

        _uiExpander_FangTu->slider_Ratio->setRangeParam(0.2, 1, StepMode_Line, ParamType_Other);
        _uiExpander_FangTu->slider_Ratio->setDecimalsNum(2);

        _uiExpander_FangTu->slider_Response->setRangeParam(5, 2000, StepMode_Log10, ParamType_Time);
        _uiExpander_FangTu->slider_Response->setDecimalsNum(2);

        _uiExpander_FangTu->slider_Release->setRangeParam(10, 4000, StepMode_Log10, ParamType_Time);
        _uiExpander_FangTu->slider_Release->setDecimalsNum(2);

        _uiExpander_FangTu->pb_bypass->setProperty("form", "Bypass");
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    this->setFixedWidth(size().width());
}

void CfgSettingDyn::initData_FangTu()
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_DYN_AUTO:
        _uiLeveler_FangTu->slider_Threshold->setValue(_cfgItem->getThreshold()/* * 10*/);
        _uiLeveler_FangTu->widget->thresholdChanged(_cfgItem->getThreshold());

        _uiLeveler_FangTu->slider_Ratio->setValue(_cfgItem->getRatio()/* * 10*/);
        _uiLeveler_FangTu->widget->ratioChanged(_cfgItem->getRatio());

        _uiLeveler_FangTu->slider_OutTargetLevel->setValue(_cfgItem->getGain()/* * 10*/);
        _uiLeveler_FangTu->widget->outTargetLevelChanged(_cfgItem->getGain());

        _uiLeveler_FangTu->slider_Response->setValue(_cfgItem->getAttack()/* * 100*/);

        _uiLeveler_FangTu->slider_Release->setValue(_cfgItem->getRelease()/* * 100*/);

        _uiLeveler_FangTu->pb_bypass->setChecked(_cfgItem->getBypass());
        break;
    case CFG_TYPE_DYN_COMP:
        _uiCompressor_FangTu->slider_Threshold->setValue(_cfgItem->getThreshold()/* * 10*/);
        _uiCompressor_FangTu->widget->thresholdChanged(_cfgItem->getThreshold());

        _uiCompressor_FangTu->slider_Ratio->setValue(_cfgItem->getRatio()/* * 10*/);
        _uiCompressor_FangTu->widget->ratioChanged(_cfgItem->getRatio());

        _uiCompressor_FangTu->slider_Response->setValue(_cfgItem->getAttack()/* * 100*/);

        _uiCompressor_FangTu->slider_Release->setValue(_cfgItem->getRelease()/* * 100*/);

        _uiCompressor_FangTu->pb_bypass->setChecked(_cfgItem->getBypass());
        break;
    case CFG_TYPE_DYN_LIMIT:
        _uiLimiter_FangTu->slider_Threshold->setValue(_cfgItem->getThreshold()/* * 10*/);
        _uiLimiter_FangTu->widget->thresholdChanged(_cfgItem->getThreshold());

        _uiLimiter_FangTu->slider_Response->setValue(_cfgItem->getAttack()/* * 100*/);

        _uiLimiter_FangTu->slider_Release->setValue(_cfgItem->getRelease()/* * 100*/);

        _uiLimiter_FangTu->pb_bypass->setChecked(_cfgItem->getBypass());
        break;
    case CFG_TYPE_DYN_NOISE:
        _uiNoiseGate_FangTu->slider_Threshold->setValue(_cfgItem->getThreshold()/* * 10*/);
        _uiNoiseGate_FangTu->widget->thresholdChanged(_cfgItem->getThreshold());

        _uiNoiseGate_FangTu->slider_Gain->setValue(_cfgItem->getGain()/* * 10*/);
        _uiNoiseGate_FangTu->widget->gainChanged(_cfgItem->getGain());

        _uiNoiseGate_FangTu->slider_Response->setValue(_cfgItem->getAttack()/* * 100*/);

        _uiNoiseGate_FangTu->slider_Release->setValue(_cfgItem->getRelease()/* * 100*/);

        _uiNoiseGate_FangTu->pb_bypass->setChecked(_cfgItem->getBypass());
        break;
    case CFG_TYPE_DYN_EXPAND:
        _uiExpander_FangTu->slider_Threshold->setValue(_cfgItem->getThreshold()/* * 10*/);
        _uiExpander_FangTu->widget->thresholdChanged(_cfgItem->getThreshold());

        _uiExpander_FangTu->slider_Ratio->setValue(_cfgItem->getRatio()/* * 100*/);
        _uiExpander_FangTu->widget->ratioChanged(_cfgItem->getRatio());

        _uiExpander_FangTu->slider_Response->setValue(_cfgItem->getAttack()/* * 100*/);

        _uiExpander_FangTu->slider_Release->setValue(_cfgItem->getRelease()/* * 100*/);

        _uiExpander_FangTu->pb_bypass->setChecked(_cfgItem->getBypass());
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

void CfgSettingDyn::initSignalConnect_FangTu()
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_DYN_AUTO:
        connect(_uiLeveler_FangTu->slider_Threshold, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiLeveler_FangTu->slider_Ratio, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiLeveler_FangTu->slider_OutTargetLevel, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiLeveler_FangTu->slider_Response, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiLeveler_FangTu->slider_Release, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));

        connect(_uiLeveler_FangTu->pb_bypass, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
        break;
    case CFG_TYPE_DYN_COMP:
        connect(_uiCompressor_FangTu->slider_Ratio, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiCompressor_FangTu->slider_Threshold, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiCompressor_FangTu->slider_Response, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiCompressor_FangTu->slider_Release, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));

        connect(_uiCompressor_FangTu->pb_bypass, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
        break;
    case CFG_TYPE_DYN_LIMIT:
        connect(_uiLimiter_FangTu->slider_Threshold, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiLimiter_FangTu->slider_Response, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiLimiter_FangTu->slider_Release, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));

        connect(_uiLimiter_FangTu->pb_bypass, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
        break;
    case CFG_TYPE_DYN_NOISE:
        connect(_uiNoiseGate_FangTu->slider_Threshold, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiNoiseGate_FangTu->slider_Gain, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiNoiseGate_FangTu->slider_Response, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiNoiseGate_FangTu->slider_Release, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));

        connect(_uiNoiseGate_FangTu->pb_bypass, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
        break;
    case CFG_TYPE_DYN_EXPAND:
        connect(_uiExpander_FangTu->slider_Threshold, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiExpander_FangTu->slider_Ratio, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiExpander_FangTu->slider_Response, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));
        connect(_uiExpander_FangTu->slider_Release, SIGNAL(valueChanged(double)), this, SLOT(processSilderSlot_FangTu(double)));

        connect(_uiExpander_FangTu->pb_bypass, SIGNAL(clicked(bool)), this, SLOT(processPushButtonSlot(bool)));
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

void CfgSettingDyn::processSilderSlot_FangTu(double value)
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_DYN_AUTO:
        if (sender() == _uiLeveler_FangTu->slider_Threshold)
        {
            if (value > _uiLeveler_FangTu->slider_OutTargetLevel->value())
            {
                value = _uiLeveler_FangTu->slider_OutTargetLevel->value();
                _uiLeveler_FangTu->slider_Threshold->setValue(value);
            }

            _uiLeveler_FangTu->widget->thresholdChanged(value);

            _cfgItem->setThreshold(value);
        }
        else if (sender() == _uiLeveler_FangTu->slider_Ratio)
        {
            _uiLeveler_FangTu->widget->ratioChanged(value);

            _cfgItem->setRatio(value);
        }
        else if (sender() == _uiLeveler_FangTu->slider_OutTargetLevel)
        {
            if (value < _uiLeveler_FangTu->slider_Threshold->value())
            {
                value = _uiLeveler_FangTu->slider_Threshold->value();
                _uiLeveler_FangTu->slider_OutTargetLevel->setValue(value);
            }

            _uiLeveler_FangTu->widget->outTargetLevelChanged(value);

            _cfgItem->setGain(value);
        }
        else if (sender() == _uiLeveler_FangTu->slider_Response)
        {
            _cfgItem->setAttack(value);
        }
        else if (sender() == _uiLeveler_FangTu->slider_Release)
        {
            _cfgItem->setRelease(value);
        }
        break;
    case CFG_TYPE_DYN_COMP:
        if (sender() == _uiCompressor_FangTu->slider_Threshold)
        {
            _uiCompressor_FangTu->widget->thresholdChanged(value);
            _cfgItem->setThreshold(value);
        }
        else if (sender() == _uiCompressor_FangTu->slider_Ratio)
        {
            _uiCompressor_FangTu->widget->ratioChanged(value);
            _cfgItem->setRatio(value);
        }
        else if (sender() == _uiCompressor_FangTu->slider_Response)
        {
            _cfgItem->setAttack(value);
        }
        else if (sender() == _uiCompressor_FangTu->slider_Release)
        {
            _cfgItem->setRelease(value);
        }
        break;
    case CFG_TYPE_DYN_LIMIT:
        if (sender() == _uiLimiter_FangTu->slider_Threshold)
        {
            _uiLimiter_FangTu->widget->thresholdChanged(value);
            _cfgItem->setThreshold(value);
        }
        else if (sender() == _uiLimiter_FangTu->slider_Response)
        {
            _cfgItem->setAttack(value);
        }
        else if (sender() == _uiLimiter_FangTu->slider_Release)
        {
            _cfgItem->setRelease(value);
        }
        break;
    case CFG_TYPE_DYN_NOISE:
        if (sender() == _uiNoiseGate_FangTu->slider_Threshold)
        {
            _uiNoiseGate_FangTu->widget->thresholdChanged(value);

            _cfgItem->setThreshold(value);
        }
        else if (sender() == _uiNoiseGate_FangTu->slider_Gain)
        {
            _uiNoiseGate_FangTu->widget->gainChanged(value);

            _cfgItem->setGain(value);
        }
        else if (sender() == _uiNoiseGate_FangTu->slider_Response)
        {
            _cfgItem->setAttack(value);
        }
        else if (sender() == _uiNoiseGate_FangTu->slider_Release)
        {
            _cfgItem->setRelease(value);
        }
        break;
    case CFG_TYPE_DYN_EXPAND:
        if (sender() == _uiExpander_FangTu->slider_Threshold)
        {
            _uiExpander_FangTu->widget->thresholdChanged(value);

            _cfgItem->setThreshold(value);
        }
        else if (sender() == _uiExpander_FangTu->slider_Ratio)
        {
            _uiExpander_FangTu->widget->ratioChanged(value);

            _cfgItem->setRatio(value);
        }
        else if (sender() == _uiExpander_FangTu->slider_Response)
        {
            _cfgItem->setAttack(value);
        }
        else if (sender() == _uiExpander_FangTu->slider_Release)
        {
            _cfgItem->setRelease(value);
        }
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}


void CfgSettingDyn::processPushButtonSlot(bool flag)
{
    QPushButton *curPb = qobject_cast<QPushButton *>(sender());
    _cfgItem->setBypass(flag);
    _uiDyn->dynGraph->setBypass(flag);
    //    if (flag) {
    //        pb->setText(tr("On"));
    //    }
    //    else {
    //        pb->setText(tr("Off"));
    //    }
}


