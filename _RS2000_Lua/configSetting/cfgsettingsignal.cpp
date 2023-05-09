#include "cfgsettingsignal.h"
#include "configItem/configitemsignal.h"
#include "stable.h"
#include "ui_cfgsettingsignal_sine.h"
#include "ui_cfgsettingsignal_noise.h"

CfgSettingSignal::CfgSettingSignal(ConfigItemSignal* cfgItem, QWidget *parent)
    : QWidget(parent)
{
    _cfgItem = cfgItem;

    initUi();
    initData();
    initSignalConnect();
}

CfgSettingSignal::~CfgSettingSignal()
{
    if(_uiSine)
        delete _uiSine;
    if(_uiNoise)
        delete _uiNoise;
}

void CfgSettingSignal::updateUiData()
{
    initData();
}

void CfgSettingSignal::initUi()
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SIG_SINE:
        _uiSine = new Ui::cfgsettingsignal_sine;
        _uiSine->setupUi(this);
        _uiSine->_gainCtrl->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
        _uiSine->_gainCtrl->setDecimalsNum(1);
        _uiSine->_freqCtrl->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
        _uiSine->_freqCtrl->setDecimalsNum(0);
        _uiSine->pb_mute->setProperty("form", "Mute");

        break;
    case CFG_TYPE_SIG_NOISE:
    case CFG_TYPE_SIG_PINKNOISE:
        _uiNoise = new Ui::cfgsettingsignal_noise;
        _uiNoise->setupUi(this);
        _uiNoise->pb_mute->setProperty("form", "Mute");

        _uiNoise->_gainCtrl->setRangeParam(-100, 12, StepMode_Line, ParamType_Gain);
        _uiNoise->_gainCtrl->setDecimalsNum(1);
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSignal::initData()
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SIG_SINE:
        _uiSine->pb_mute->setChecked(_cfgItem->getMute());
        pushButtonSetText(_uiSine->pb_mute, _uiSine->pb_mute->isChecked());

        _uiSine->_gainCtrl->setValue(_cfgItem->getLevel());
        _uiSine->_freqCtrl->setValue(_cfgItem->getFreq());

        _uiSine->widget->dbChanged(_cfgItem->getLevel());
        _uiSine->widget->freqChanged(_cfgItem->getFreq());
        break;
    case CFG_TYPE_SIG_NOISE:
    case CFG_TYPE_SIG_PINKNOISE:
        _uiNoise->pb_mute->setChecked(_cfgItem->getMute());
        pushButtonSetText(_uiNoise->pb_mute, _uiNoise->pb_mute->isChecked());


        _uiNoise->_gainCtrl->setValue(_cfgItem->getLevel());
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSignal::initSignalConnect()
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SIG_SINE:
        connect(_uiSine->_freqCtrl, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
        connect(_uiSine->_gainCtrl, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
        connect(_uiSine->pb_mute, SIGNAL(clicked(bool)), this, SLOT(processPushbuttonSlot(bool)));
        break;
    case CFG_TYPE_SIG_NOISE:
    case CFG_TYPE_SIG_PINKNOISE:
        connect(_uiNoise->_gainCtrl, SIGNAL(valueChanged(double)), this, SLOT(processDoubleSpinBoxSlot(double)));
        connect(_uiNoise->pb_mute, SIGNAL(clicked(bool)), this, SLOT(processPushbuttonSlot(bool)));
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSignal::processPushbuttonSlot(bool flag)
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SIG_SINE:
        if (sender() == _uiSine->pb_mute)
        {
            _cfgItem->setMute(flag);

            pushButtonSetText(qobject_cast<QPushButton *>(sender()), flag);
        }
        break;
    case CFG_TYPE_SIG_NOISE:
    case CFG_TYPE_SIG_PINKNOISE:
        if (sender() == _uiNoise->pb_mute)
        {
            _cfgItem->setMute(flag);

            pushButtonSetText(qobject_cast<QPushButton *>(sender()), flag);
        }
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSignal::processDoubleSpinBoxSlot(double value)
{
    switch(_cfgItem->getCfgType())
    {
    case CFG_TYPE_SIG_SINE:
        if (sender() == _uiSine->_freqCtrl)
        {
            _uiSine->widget->freqChanged(value);
            _cfgItem->setFreq(value);
        }
        else if (sender() == _uiSine->_gainCtrl)
        {
            _uiSine->widget->dbChanged(value);
            _cfgItem->setLevel(value);
        }
        break;
    case CFG_TYPE_SIG_NOISE:
    case CFG_TYPE_SIG_PINKNOISE:
        if (sender() == _uiNoise->_gainCtrl)
        {
            _cfgItem->setLevel(value);
        }
        break;
    default:
        Q_ASSERT(false);
    }
}

void CfgSettingSignal::pushButtonSetText(QPushButton *pb, bool flag)
{
//    if (flag)
//    {
//        pb->setText(tr("On"));
//    }
//    else
//    {
//        pb->setText(tr("Off"));
//    }
}
