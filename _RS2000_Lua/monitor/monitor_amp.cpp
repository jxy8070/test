#include "Monitor_Amp.h"
#include "ui_Monitor_Amp.h"
#include "protocal/packet.h"
#include "device/device_stm32_poe_0_2.h"
#include "deviceSetting/alarmconfig.h"
#include "deviceItem/deviceItem.h"

Monitor_Amp::Monitor_Amp(Device_Amp *devAmp, DeviceItem *devItem, QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::Monitor_Amp)
{
    ui->setupUi(this);
    _devAmp = devAmp;
    _devItem = devItem;

    _errorState = false;
    initData();

    _syncing = false;

    ui->devicenamelabel->setStyleSheet("border: 1px solid #999999;\
                                       background-color: rgb(218, 218, 218);");

    /* 同步setting界面一些参数 */
//    connect(_devAmp, SIGNAL(volumeChanged()), this, SLOT(syncVolumeChanged()));
//    connect(_devAmp, SIGNAL(muteChanged()), this, SLOT(syncMuteChanged()));
//    connect(_devAmp, SIGNAL(phaseChanged()), this, SLOT(syncPhaseChanged()));
//    connect(_devAmp, SIGNAL(routerChanged()), this, SLOT(syncRouterChanged()));

    /* 设备名称按钮，点击可以进入setting界面 */
//    connect(ui->devicenamelabel, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    /*  电源开关 */
//    connect(ui->pbPower, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    /* ADC数值 */
//    connect(_devAmp, SIGNAL(adcDataChanged(AlarmConfig*)), this, SLOT(onAdcDataChanged(AlarmConfig*)));

    /* 音量推子 */
    ui->ch1volumeValue->setValidator(new QIntValidator(ui->ch1VolumeSlider->minimum(), ui->ch1VolumeSlider->maximum()));
//    connect(ui->ch1VolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
//    connect(ui->ch1volumeValue, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));
    ui->ch2volumeValue->setValidator(new QIntValidator(ui->ch2VolumeSlider->minimum(), ui->ch2VolumeSlider->maximum()));
//    connect(ui->ch2VolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
//    connect(ui->ch2volumeValue, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* mute、phase */
//    connect(ui->ch1mute, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
//    connect(ui->ch1phase, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
//    connect(ui->ch2mute, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
//    connect(ui->ch2phase, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    _livingTimer = new QTimer(this);
    connect(_livingTimer, SIGNAL(timeout()), this, SLOT(unLivingDectect()));
}

Monitor_Amp::~Monitor_Amp()
{
    delete ui;
}

void Monitor_Amp::unLivingDectect()
{
    if (!_devAmp->isOnline())
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

void Monitor_Amp::showEvent(QShowEvent *)
{
    _livingTimer->start(3 * 1000);

    initData();

    /* 同步setting界面一些参数 */
    connect(_devAmp, SIGNAL(volumeChanged()), this, SLOT(syncVolumeChanged()));
    connect(_devAmp, SIGNAL(muteChanged()), this, SLOT(syncMuteChanged()));
    connect(_devAmp, SIGNAL(phaseChanged()), this, SLOT(syncPhaseChanged()));
    connect(_devAmp, SIGNAL(routerChanged()), this, SLOT(syncRouterChanged()));

    /* 设备名称按钮，点击可以进入setting界面 */
    connect(ui->devicenamelabel, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    /*  电源开关 */
    connect(ui->pbPower, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    /* ADC数值 */
    connect(_devAmp, SIGNAL(adcDataChanged(AlarmConfig*)), this, SLOT(onAdcDataChanged(AlarmConfig*)));

    /* 音量推子 */
    connect(ui->ch1VolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(ui->ch1volumeValue, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));
    connect(ui->ch2VolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(ui->ch2volumeValue, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* mute、phase */
    connect(ui->ch1mute, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
    connect(ui->ch1phase, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
    connect(ui->ch2mute, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
    connect(ui->ch2phase, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
}

void Monitor_Amp::hideEvent(QHideEvent *)
{
    _livingTimer->stop();

    /* 同步setting界面一些参数 */
    disconnect(_devAmp, SIGNAL(volumeChanged()), this, SLOT(syncVolumeChanged()));
    disconnect(_devAmp, SIGNAL(muteChanged()), this, SLOT(syncMuteChanged()));
    disconnect(_devAmp, SIGNAL(phaseChanged()), this, SLOT(syncPhaseChanged()));
    disconnect(_devAmp, SIGNAL(routerChanged()), this, SLOT(syncRouterChanged()));

    /* 设备名称按钮，点击可以进入setting界面 */
    disconnect(ui->devicenamelabel, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    /*  电源开关 */
    disconnect(ui->pbPower, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));

    /* ADC数值 */
    disconnect(_devAmp, SIGNAL(adcDataChanged(AlarmConfig*)), this, SLOT(onAdcDataChanged(AlarmConfig*)));

    /* 音量推子 */
    disconnect(ui->ch1VolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    disconnect(ui->ch1volumeValue, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));
    disconnect(ui->ch2VolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    disconnect(ui->ch2volumeValue, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* mute、phase */
    disconnect(ui->ch1mute, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
    disconnect(ui->ch1phase, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
    disconnect(ui->ch2mute, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
    disconnect(ui->ch2phase, SIGNAL(clicked(bool)), this, SLOT(processPbClicked(bool)));
}

void Monitor_Amp::initData()
{
//    ui->devicenamelabel->setText(_devItem->getAES70Device()->getDeviceName());

//    ui->pbPower->setChecked(_devAmp->isPowerOn());
//    pushButtonSetText(ui->pbPower, _devAmp->isPowerOn());
//    ui->labelLine1->setText(_devAmp->getRouterInfo(_devAmp->getConfigData().ch1_ControlValue.channel));
//    ui->labelLine2->setText(_devAmp->getRouterInfo(_devAmp->getConfigData().ch2_ControlValue.channel));

//    ui->ch1VolumeSlider->setValue(_devAmp->getConfigData().ch1_ControlValue.volume);
//    ui->ch1volumeValue->setText(QString::number(_devAmp->getConfigData().ch1_ControlValue.volume));
//    ui->ch1mute->setChecked(_devAmp->getConfigData().ch1_ControlValue.mute);
//    ui->ch1phase->setChecked(_devAmp->getConfigData().ch1_ControlValue.phase);

//    ui->ch2VolumeSlider->setValue(_devAmp->getConfigData().ch2_ControlValue.volume);
//    ui->ch2volumeValue->setText(QString::number(_devAmp->getConfigData().ch2_ControlValue.volume));
//    ui->ch2mute->setChecked(_devAmp->getConfigData().ch2_ControlValue.mute);
//    ui->ch2phase->setChecked(_devAmp->getConfigData().ch2_ControlValue.phase);

//    for (int i = 0; i < 8; ++i)
//    {
//        _stateBak[i] = StateNormal;
//    }
//    _alarmEnable = false;
}

void Monitor_Amp::syncVolumeChanged()
{
//    _syncing = true;

//    ui->ch1VolumeSlider->setValue(_devAmp->getConfigData().ch1_ControlValue.volume);
//    ui->ch1volumeValue->setText(QString::number(_devAmp->getConfigData().ch1_ControlValue.volume));
//    ui->ch2VolumeSlider->setValue(_devAmp->getConfigData().ch2_ControlValue.volume);
//    ui->ch2volumeValue->setText(QString::number(_devAmp->getConfigData().ch2_ControlValue.volume));

//    _syncing = false;
}

void Monitor_Amp::syncMuteChanged()
{
//    _syncing = true;

//    ui->ch1mute->setChecked(_devAmp->getConfigData().ch1_ControlValue.mute);
//    ui->ch2mute->setChecked(_devAmp->getConfigData().ch2_ControlValue.mute);

//    _syncing = false;
}

void Monitor_Amp::syncPhaseChanged()
{
//    _syncing = true;

//    ui->ch1phase->setChecked(_devAmp->getConfigData().ch1_ControlValue.phase);
//    ui->ch2phase->setChecked(_devAmp->getConfigData().ch2_ControlValue.phase);

//    _syncing = false;
}

void Monitor_Amp::syncRouterChanged()
{
//    ui->labelLine1->setText(_devAmp->getRouterInfo(_devAmp->getConfigData().ch1_ControlValue.channel));
//    ui->labelLine2->setText(_devAmp->getRouterInfo(_devAmp->getConfigData().ch2_ControlValue.channel));
}

void Monitor_Amp::pushButtonSetText(QPushButton *pb, bool flag)
{
    if (flag)
    {
        pb->setText(tr("On"));
    }
    else
    {
        pb->setText(tr("Off"));
    }

    ui->ch1VolumeSlider->setEnabled(flag);
    ui->ch1volumeValue->setEnabled(flag);
    ui->ch1mute->setEnabled(flag);
    ui->ch1phase->setEnabled(flag);

    ui->ch2VolumeSlider->setEnabled(flag);
    ui->ch2volumeValue->setEnabled(flag);
    ui->ch2mute->setEnabled(flag);
    ui->ch2phase->setEnabled(flag);
}

void Monitor_Amp::onAdcDataChanged(AlarmConfig *alarmCfg)
{
    if(_devAmp->isPowerOn())
    {
        double volt1Ch = alarmCfg->getParaValue(Volt1Ch);
        double volt2Ch = alarmCfg->getParaValue(Volt2Ch);
        double curr1Ch = alarmCfg->getParaValue(Curr1Ch);
        double curr2Ch = alarmCfg->getParaValue(Curr2Ch);
        double temperature = alarmCfg->getParaValue(Temperature);
        double volumeL = alarmCfg->getParaValue(VolumeL);
        double volumeR = alarmCfg->getParaValue(VolumeR);

        ui->line1Volt->setText(QString::number(volt1Ch, 'f', 1));
        ui->line1Curr->setText(QString::number(curr1Ch, 'f', 1));
        ui->line1Temp->setText(QString::number(temperature, 'f', 1));
        ui->ch1vuMeter->setLCurValue(volumeL);
        ui->ch1vuMeterLabel->setText(QString::number(volumeL, 'f', 1));


        ui->line2Volt->setText(QString::number(volt2Ch, 'f', 1));
        ui->line2Curr->setText(QString::number(curr2Ch, 'f', 1));
        ui->line2Temp->setText(QString::number(temperature, 'f', 1));
        ui->ch2vuMeter->setLCurValue(volumeR);
        ui->ch2vuMeterLabel->setText(QString::number(volumeR, 'f', 1));
    }

    if (_errorState != _devAmp->getAlarmCfg()->getDevState())
    {
        _errorState = _devAmp->getAlarmCfg()->getDevState();
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

    if (_devAmp->getAlarmCfg()->getAlarmEnable())
    {
        if (_devAmp->getAlarmCfg()->getAlarmEnable() != _alarmEnable)
        {
            _alarmEnable = _devAmp->getAlarmCfg()->getAlarmEnable();
        }
        if (_devAmp->getAlarmCfg()->getLimitDef(Volt1Ch).state != _stateBak[0])
        {
            _stateBak[0] = _devAmp->getAlarmCfg()->getLimitDef(Volt1Ch).state;
            if (_stateBak[0] != StateNormal)
            {
                ui->line1Volt->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: red;");
            }
            else
            {
                ui->line1Volt->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: rgb(218, 218, 218);");
            }
        }
        if (_devAmp->getAlarmCfg()->getLimitDef(Curr1Ch).state != _stateBak[1])
        {
            _stateBak[1] = _devAmp->getAlarmCfg()->getLimitDef(Curr1Ch).state;
            if (_stateBak[1] != StateNormal)
            {
                ui->line1Curr->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: red;");
            }
            else
            {
                ui->line1Curr->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: rgb(218, 218, 218);");
            }
        }
        if (_devAmp->getAlarmCfg()->getLimitDef(Temperature).state != _stateBak[2])
        {
            _stateBak[2] = _devAmp->getAlarmCfg()->getLimitDef(Temperature).state;
            if (_stateBak[2] != StateNormal)
            {
                ui->line1Temp->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: red;");
                ui->line2Temp->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: red;");
            }
            else
            {
                ui->line1Temp->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: rgb(218, 218, 218);");
                ui->line2Temp->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: rgb(218, 218, 218);");
            }
        }
        if (_devAmp->getAlarmCfg()->getLimitDef(VolumeL).state != _stateBak[3])
        {
            _stateBak[3] = _devAmp->getAlarmCfg()->getLimitDef(VolumeL).state;
            if (_stateBak[3] != StateNormal)
            {
                ui->ch1vuMeterLabel->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: red;");
            }
            else
            {
                ui->ch1vuMeterLabel->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: rgb(218, 218, 218);");
            }
        }

        if (_devAmp->getAlarmCfg()->getLimitDef(Volt2Ch).state != _stateBak[4])
        {
            _stateBak[4] = _devAmp->getAlarmCfg()->getLimitDef(Volt2Ch).state;
            if (_stateBak[4] != StateNormal)
            {
                ui->line2Volt->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: red;");
            }
            else
            {
                ui->line2Volt->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: rgb(218, 218, 218);");
            }
        }
        if (_devAmp->getAlarmCfg()->getLimitDef(Curr2Ch).state != _stateBak[5])
        {
            _stateBak[5] = _devAmp->getAlarmCfg()->getLimitDef(Curr2Ch).state;
            if (_stateBak[5] != StateNormal)
            {
                ui->line2Curr->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: red;");
            }
            else
            {
                ui->line2Curr->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: rgb(218, 218, 218);");
            }
        }

        if (_devAmp->getAlarmCfg()->getLimitDef(VolumeR).state != _stateBak[6])
        {
            _stateBak[6] = _devAmp->getAlarmCfg()->getLimitDef(VolumeR).state;
            if (_stateBak[6] != StateNormal)
            {
                ui->ch2vuMeterLabel->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: red;");
            }
            else
            {
                ui->ch2vuMeterLabel->setStyleSheet("border: 1px solid #999999;"
                                             "background-color: rgb(218, 218, 218);");
            }
        }
    }
    else
    {
        if (_devAmp->getAlarmCfg()->getAlarmEnable() != _alarmEnable)
        {
            _alarmEnable = _devAmp->getAlarmCfg()->getAlarmEnable();

            for (int i = 0; i < 8; ++i)
            {
                _stateBak[i] = StateNormal;
            }
            ui->line1Volt->setStyleSheet("border: 1px solid #999999;"
                                         "background-color: rgb(218, 218, 218);");
            ui->line1Curr->setStyleSheet("border: 1px solid #999999;"
                                         "background-color: rgb(218, 218, 218);");
            ui->line1Temp->setStyleSheet("border: 1px solid #999999;"
                                         "background-color: rgb(218, 218, 218);");
            ui->ch1vuMeterLabel->setStyleSheet("border: 1px solid #999999;"
                                         "background-color: rgb(218, 218, 218);");
            ui->line2Volt->setStyleSheet("border: 1px solid #999999;"
                                         "background-color: rgb(218, 218, 218);");
            ui->line2Curr->setStyleSheet("border: 1px solid #999999;"
                                         "background-color: rgb(218, 218, 218);");
            ui->line2Temp->setStyleSheet("border: 1px solid #999999;"
                                         "background-color: rgb(218, 218, 218);");
            ui->ch2vuMeterLabel->setStyleSheet("border: 1px solid #999999;"
                                         "background-color: rgb(218, 218, 218);");
        }
    }
}

void Monitor_Amp::setIntValueDisplay(int i)
{
//    /* 同步的时候不再次发送数据 */
//    if (_syncing)
//    {
//        return;
//    }

//    quint8 ch = 1;
//    ControlValue *cntvlu = NULL;
//    if(sender() == ui->ch1VolumeSlider)
//    {
//        ui->ch1volumeValue->setText(QString::number(i));
//        ch = 1;
//        cntvlu = &_devAmp->getConfigData().ch1_ControlValue;
//        cntvlu->volume = i;
//    }
//    else if (sender() == ui->ch2VolumeSlider)
//    {
//        ui->ch2volumeValue->setText(QString::number(i));
//        ch = 2;
//        cntvlu = &_devAmp->getConfigData().ch2_ControlValue;
//        cntvlu->volume = i;
//    }

//    _devAmp->sendCommonDataPack(ch, *cntvlu);
}

void Monitor_Amp::setSliderValue()
{
    /* 同步的时候不再次发送数据 */
    if (_syncing)
    {
        return;
    }

    if(sender() == ui->ch1volumeValue)
    {
        ui->ch1VolumeSlider->setValue(ui->ch1volumeValue->text().toInt());
    }
    else if(sender() == ui->ch2volumeValue)
    {
        ui->ch2VolumeSlider->setValue(ui->ch2volumeValue->text().toInt());
    }
}

void Monitor_Amp::processPbClicked(bool flag)
{
    if (sender() == ui->devicenamelabel)
    {
        _devItem->showSettingDialog();
    }
    else if (sender() == ui->pbPower)
    {
        pushButtonSetText(ui->pbPower, flag);
        _devAmp->sendSwitch(flag);
    }
    /* 同步的时候不再次发送数据 */
    else if (_syncing)
    {
        return;
    }
    else if (sender() == ui->ch1mute)
    {
        _devAmp->sendMute(Device_Amp::AMP_CH1, flag);
    }
    else if (sender() == ui->ch1phase)
    {
        _devAmp->sendPhase(Device_Amp::AMP_CH1, flag);
    }
    else if (sender() == ui->ch2mute)
    {
        _devAmp->sendMute(Device_Amp::AMP_CH2, flag);
    }
    else if (sender() == ui->ch2phase)
    {
        _devAmp->sendPhase(Device_Amp::AMP_CH2, flag);
    }
}
