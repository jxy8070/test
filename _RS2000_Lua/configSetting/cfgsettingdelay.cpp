#include "stable.h"
#include "cfgsettingdelay.h"
#include "configItem/configitemdelay.h"
#include "customWidget/configgainwidget.h"

CfgSettingDelay::CfgSettingDelay(ConfigItemDelay *cfgItem, QWidget *parent)
    : QWidget(parent)
{
    _cfgItemDelay = cfgItem;

    setupUi();
}

void CfgSettingDelay::updateUiData()
{
    _btnBypass->setChecked(_cfgItemDelay->getbypass());
    _spinDelay->setValue(_cfgItemDelay->getDelay());
    double m = _cfgItemDelay->getDelay()/1000.0f*340.0f;
    if(_units == "us")
        m = m/10.0f; // ms/m <=> us/cm
    _spinDistance->setValue(m);
    update();
}

void CfgSettingDelay::onDelayChanged(int delay)
{
    double m = delay/1000.0f*340.0f;
    if(_units == "us")
        m = m/10.0f; // ms/m <=> us/cm
//     qDebug() << __FUNCTION__ << __LINE__ << m;
    _spinDistance->setValue(m);

    _cfgItemDelay->setDelay(delay);
}

void CfgSettingDelay::onBypassChanged(bool bypass)
{
    _cfgItemDelay->setBypass(bypass);
}

void CfgSettingDelay::onDistanceChanged(double m)
{
    qDebug() << __FUNCTION__ << __LINE__ << m;
    double delay = m/340.0f*1000.0f;
    if(_units == "us")
        delay = delay*10; // ms/m <=> us/cm
    _spinDelay->setValue(delay);
    _cfgItemDelay->setDelay(delay);
}

void CfgSettingDelay::setupUi()
{    
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(2, 20, 25, 4);
    layout->setSpacing(10);
    QLabel *lblBypass = new QLabel(this);
    lblBypass->setText(tr("Bypass"));
    layout->addWidget(lblBypass, 0, 0, 1, 1, Qt::AlignRight);

    _btnBypass = new QPushButton(this);
    _btnBypass->setCheckable(true);
    _btnBypass->setProperty("form", "Bypass");
    _btnBypass->setFocusPolicy(Qt::NoFocus);
    _btnBypass->setChecked(_cfgItemDelay->getbypass());
    connect(_btnBypass, SIGNAL(clicked(bool)), this, SLOT(onBypassChanged(bool)));
    layout->addWidget(_btnBypass, 0, 1, 1, 1, Qt::AlignCenter);

    QLabel *lblDelay = new QLabel(this);
    layout->addWidget(lblDelay, 1, 0, 1, 1, Qt::AlignRight);

    _spinDelay = new ConfigGainWidget(this);
    _spinDelay->setDecimalsNum(0);
    _spinDelay->setRangeParam(0, _cfgItemDelay->getMaxDelay(), StepMode_Line, ParamType_Time);
    _spinDelay->setValue((double)_cfgItemDelay->getDelay());
    layout->addWidget(_spinDelay, 1, 1, 1, 1, Qt::AlignCenter);
    connect(_spinDelay, SIGNAL(valueChanged(int)), this, SLOT(onDelayChanged(int)));
    if(_cfgItemDelay->getCfgType() <= CFG_TYPE_DELAY_1000)  //毫秒级别延时
    {
        lblDelay->setText(tr("Delay(ms)"));
        _units = "ms";
    }
    else //微妙秒级别延时
    {
        lblDelay->setText(tr("Delay(us)"));
        _units = "us";
        _spinDelay->setSingleStep(1000.0/48); //步长设为48K音频一个采样值的微秒数,
    }

    QLabel *labDistance = new QLabel(tr("Distance(m)"));
    layout->addWidget(labDistance, 2, 0, 1, 1, Qt::AlignRight);
    _spinDistance = new ConfigGainWidget(this);
    _spinDistance->setDecimalsNum(2);
    double maxDistance = _cfgItemDelay->getMaxDelay()/1000.0f*340;
    if(_units == "ms") {
        _spinDistance->setRangeParam(0, maxDistance, StepMode_Line, ParamType_Other);
    }
    else if(_units == "us") {
        maxDistance = maxDistance /10; // ms/m <=> us/cm
        _spinDistance->setRangeParam(0, maxDistance, StepMode_Line, ParamType_Other);
        labDistance->setText(tr("Distance(cm)"));
    }
    onDelayChanged((double)_cfgItemDelay->getDelay());
    layout->addWidget(_spinDistance, 2, 1, 1, 1, Qt::AlignCenter);
    connect(_spinDistance, SIGNAL(valueChanged(double)), this, SLOT(onDistanceChanged(double)));

    resize(180, 150);
}

