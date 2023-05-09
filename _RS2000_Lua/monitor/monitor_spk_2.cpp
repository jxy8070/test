#include "monitor_spk_2.h"
#include "ui_monitor_spk.h"
#include "protocal/packet.h"
#include "device/device_spk_2.h"
#include "deviceSetting/alarmconfig.h"
#include "deviceItem/deviceItem.h"

Monitor_Spk_2::Monitor_Spk_2(Device_Spk_2 *devSpk, DeviceItem *devItem, QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::Monitor_Spk)
{
    ui->setupUi(this);

    _devSpk = devSpk;

    _devItem = devItem;

    _errorState = false;
    initData();

    _syncing = false;

    ui->devicenamelabel->setStyleSheet("border: 1px solid #999999;\
                                       background-color: rgb(218, 218, 218);");

    /* 同步setting界面一些参数 */
//    connect(_devSpk, SIGNAL(volumeChanged()), this, SLOT(syncVolumeChanged()));
//    connect(_devSpk, SIGNAL(muteChanged()), this, SLOT(syncMuteChanged()));
//    connect(_devSpk, SIGNAL(phaseChanged()), this, SLOT(syncPhaseChanged()));
//    connect(_devSpk, SIGNAL(routerChanged()), this, SLOT(syncRouterChanged()));

    /* 设备名称按钮，点击可以进入setting界面 */
//    connect(ui->devicenamelabel, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    /*  电源开关 */
//    connect(ui->pbPower, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    /* ADC数值 */
//    connect(_devSpk, SIGNAL(adcDataChanged(AlarmConfig*)), this, SLOT(onAdcDataChanged(AlarmConfig*)));

    /* 音量推子 */
    ui->volumeValue->setValidator(new QIntValidator(ui->volumeSlider->minimum(), ui->volumeSlider->maximum()));
//    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
//    connect(ui->volumeValue, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* mute、phase */
//    connect(ui->mute, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
//    connect(ui->phase, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    _livingTimer = new QTimer(this);
    connect(_livingTimer, SIGNAL(timeout()), this, SLOT(unLivingDectect()));
}

Monitor_Spk_2::~Monitor_Spk_2()
{
    delete ui;
}

void Monitor_Spk_2::unLivingDectect()
{
    if (_devSpk->getDeviceIp() == "0.0.0.0")
    {
        ui->devicenamelabel->setStyleSheet("border: 1px solid #999999;\
                                           background-color: rgb(218, 218, 218);");
    }
    else
    {
        ui->devicenamelabel->setStyleSheet("border: 1px solid #999999;\
                                           background-color: rgb(135, 206, 235);");
    }
}

void Monitor_Spk_2::showEvent(QShowEvent *)
{
    _livingTimer->start(3 * 1000);
    initData();

    /* 同步setting界面一些参数 */
    connect(_devSpk, SIGNAL(volumeChanged()), this, SLOT(syncVolumeChanged()));
    connect(_devSpk, SIGNAL(muteChanged()), this, SLOT(syncMuteChanged()));
    connect(_devSpk, SIGNAL(phaseChanged()), this, SLOT(syncPhaseChanged()));
    connect(_devSpk, SIGNAL(routerChanged()), this, SLOT(syncRouterChanged()));

    /* 设备名称按钮，点击可以进入setting界面 */
    connect(ui->devicenamelabel, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    /*  电源开关 */
    connect(ui->pbPower, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    /* ADC数值 */
    connect(_devSpk, SIGNAL(adcDataChanged(AlarmConfig*)), this, SLOT(onAdcDataChanged(AlarmConfig*)));

    /* 音量推子 */
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(ui->volumeValue, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* mute、phase */
    connect(ui->mute, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
    connect(ui->phase, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
}

void Monitor_Spk_2::hideEvent(QHideEvent *)
{
    _livingTimer->stop();

    /* 同步setting界面一些参数 */
    disconnect(_devSpk, SIGNAL(volumeChanged()), this, SLOT(syncVolumeChanged()));
    disconnect(_devSpk, SIGNAL(muteChanged()), this, SLOT(syncMuteChanged()));
    disconnect(_devSpk, SIGNAL(phaseChanged()), this, SLOT(syncPhaseChanged()));
    disconnect(_devSpk, SIGNAL(routerChanged()), this, SLOT(syncRouterChanged()));

    /* 设备名称按钮，点击可以进入setting界面 */
    disconnect(ui->devicenamelabel, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    /*  电源开关 */
    disconnect(ui->pbPower, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    /* ADC数值 */
    disconnect(_devSpk, SIGNAL(adcDataChanged(AlarmConfig*)), this, SLOT(onAdcDataChanged(AlarmConfig*)));

    /* 音量推子 */
    disconnect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    disconnect(ui->volumeValue, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* mute、phase */
    disconnect(ui->mute, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
    disconnect(ui->phase, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
}

void Monitor_Spk_2::initData()
{
    ui->devicenamelabel->setText(_devItem->customName());

    ui->pbPower->setChecked(_devSpk->isPowerOn());
    pushButtonSetText(ui->pbPower, _devSpk->isPowerOn());
    ui->InputSource->setText(_devSpk->getRouterInfo());

    ui->volumeSlider->setValue(_devSpk->getConfigData().ch1_ControlValue.volume);
    ui->volumeValue->setText(QString::number(_devSpk->getConfigData().ch1_ControlValue.volume));
    ui->mute->setChecked(_devSpk->getConfigData().ch1_ControlValue.mute);
    ui->phase->setChecked(_devSpk->getConfigData().ch1_ControlValue.phase);

    for (int i = 0; i < 4; ++i)
    {
        _stateBak[i] = StateNormal;
    }
    _alarmEnable = false;
}

void Monitor_Spk_2::syncVolumeChanged()
{
    _syncing = true;

    ui->volumeSlider->setValue(_devSpk->getConfigData().ch1_ControlValue.volume);
    ui->volumeValue->setText(QString::number(_devSpk->getConfigData().ch1_ControlValue.volume));

    _syncing = false;
}

void Monitor_Spk_2::syncMuteChanged()
{
    _syncing = true;

    ui->mute->setChecked(_devSpk->getConfigData().ch1_ControlValue.mute);

    _syncing = false;
}

void Monitor_Spk_2::syncPhaseChanged()
{
    _syncing = true;

    ui->phase->setChecked(_devSpk->getConfigData().ch1_ControlValue.phase);

    _syncing = false;
}

void Monitor_Spk_2::syncRouterChanged()
{
    ui->InputSource->setText(_devSpk->getRouterInfo());
}

void Monitor_Spk_2::pushButtonSetText(QPushButton *pb, bool flag)
{
    if (flag)
    {
        pb->setText(tr("On"));
    }
    else
    {
        pb->setText(tr("Off"));
    }

    ui->volumeSlider->setEnabled(flag);
    ui->volumeValue->setEnabled(flag);
    ui->mute->setEnabled(flag);
    ui->phase->setEnabled(flag);
}

void Monitor_Spk_2::onAdcDataChanged(AlarmConfig *alarmCfg)
{
    if(_devSpk->isPowerOn())
    {
        double volt1Ch = alarmCfg->getParaValue(Volt1Ch);
        double curr1Ch = alarmCfg->getParaValue(Curr1Ch);
        double temperature = alarmCfg->getParaValue(Temperature);
        double volumeL = alarmCfg->getParaValue(VolumeL);
        double volumeR = alarmCfg->getParaValue(VolumeR);

        ui->labelVolt->setText(QString::number(volt1Ch, 'f', 1));
        ui->labelCurr->setText(QString::number(curr1Ch, 'f', 1));
        ui->labelTemp->setText(QString::number(temperature, 'f', 1));
        ui->vuMeter->setLCurValue(volumeL);
        ui->vuMeterLLabel->setText(QString::number(volumeL, 'f', 1));
        ui->vuMeter->setRCurValue(volumeR);
        ui->vuMeterRLabel->setText(QString::number(volumeR, 'f', 1));
    }

    if (_errorState != _devSpk->getAlarmCfg()->getDevState())
    {
        _errorState = _devSpk->getAlarmCfg()->getDevState();
        if (_errorState)
        {
            ui->labelError->setStyleSheet("border: 1px solid #999999;\
                                                   background-color: rgb(218, 218, 218);\
                                                   image: url(:/Image/red.png);");
        } else {
            ui->labelError->setStyleSheet("border: 1px solid #999999;\
                                                   background-color: rgb(218, 218, 218);\
                                                   image: url(:/Image/green.png);");
        }
    }

    if (_devSpk->getAlarmCfg()->getAlarmEnable())
    {
        if (_devSpk->getAlarmCfg()->getAlarmEnable() != _alarmEnable)
        {
            _alarmEnable = _devSpk->getAlarmCfg()->getAlarmEnable();
        }
        if (_devSpk->getAlarmCfg()->getLimitDef(Volt1Ch).state != _stateBak[0])
        {
            _stateBak[0] = _devSpk->getAlarmCfg()->getLimitDef(Volt1Ch).state;
            if (_stateBak[0] != StateNormal)
            {
                ui->labelVolt->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: red;");
            }
            else
            {
                ui->labelVolt->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: rgb(218, 218, 218);");
            }
        }
        if (_devSpk->getAlarmCfg()->getLimitDef(Curr1Ch).state != _stateBak[1])
        {
            _stateBak[1] = _devSpk->getAlarmCfg()->getLimitDef(Curr1Ch).state;
            if (_stateBak[1] != StateNormal)
            {
                ui->labelCurr->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: red;");
            }
            else
            {
                ui->labelCurr->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: rgb(218, 218, 218);");
            }
        }
        if (_devSpk->getAlarmCfg()->getLimitDef(Temperature).state != _stateBak[2])
        {
            _stateBak[2] = _devSpk->getAlarmCfg()->getLimitDef(Temperature).state;
            if (_stateBak[2] != StateNormal)
            {
                ui->labelTemp->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: red;");
            }
            else
            {
                ui->labelTemp->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: rgb(218, 218, 218);");
            }
        }
        if (_devSpk->getAlarmCfg()->getLimitDef(VolumeL).state != _stateBak[3])
        {
            _stateBak[3] = _devSpk->getAlarmCfg()->getLimitDef(VolumeL).state;
            if (_stateBak[3] != StateNormal)
            {
                ui->vuMeterLLabel->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: red;");
            }
            else
            {
                ui->vuMeterLLabel->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: rgb(218, 218, 218);");
            }
        }
    }
    else
    {
        if (_devSpk->getAlarmCfg()->getAlarmEnable() != _alarmEnable)
        {
            _alarmEnable = _devSpk->getAlarmCfg()->getAlarmEnable();
            for (int i = 0; i < 4; ++i)
            {
                _stateBak[i] = StateNormal;
            }
            ui->labelVolt->setStyleSheet("border: 1px solid #999999;"
                                         "background-color: rgb(218, 218, 218);");
            ui->labelCurr->setStyleSheet("border: 1px solid #999999;"
                                         "background-color: rgb(218, 218, 218);");
            ui->labelTemp->setStyleSheet("border: 1px solid #999999;"
                                         "background-color: rgb(218, 218, 218);");
            ui->vuMeterLLabel->setStyleSheet("border: 1px solid #999999;"
                                         "background-color: rgb(218, 218, 218);");
        }
    }
}

void Monitor_Spk_2::setIntValueDisplay(int i)
{
    /* 同步的时候不再次发送数据 */
    if (_syncing)
    {
        return;
    }

    quint8 ch = 1;
    ControlValue *cntvlu = NULL;
    if(sender() == ui->volumeSlider)
    {
        ui->volumeValue->setText(QString::number(i));
        ch = 1;
        cntvlu = &_devSpk->getConfigData().ch1_ControlValue;
        cntvlu->volume = i;
    }

    _devSpk->sendCommonDataPack();
}

void Monitor_Spk_2::setSliderValue()
{
    /* 同步的时候不再次发送数据 */
    if (_syncing)
    {
        return;
    }

    if(sender() == ui->volumeValue)
    {
        ui->volumeSlider->setValue(ui->volumeValue->text().toInt());
    }
}

void Monitor_Spk_2::processPbClicked(bool flag)
{
    if (sender() == ui->devicenamelabel)
    {
        _devItem->showSettingDialog();
    }
    else if (sender() == ui->pbPower)
    {
        pushButtonSetText(ui->pbPower, flag);
        _devSpk->sendSwitch(flag);
    }
    /* 同步的时候不再次发送数据 */
    else if (_syncing)
    {
        return;
    }
    else if (sender() == ui->mute)
    {
        _devSpk->sendMute(flag);
    }
    else if (sender() == ui->phase)
    {
        _devSpk->sendPhase(flag);
    }
}
