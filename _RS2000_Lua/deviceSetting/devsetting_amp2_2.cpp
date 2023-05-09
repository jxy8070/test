#include "stable.h"
#include "devsetting_amp4_4.h"
#include "ui_devsetting_amp4_4.h"
#include "device/device_amp4_4.h"
#include "deviceItem/deviceitem_amp4_4.h"
#include "trn/eqview_allband.h"
#include "customWidget/eqviewnormal.h"
#include "customWidget/filterview.h"
#include "spk/spk_alarmrecord.h"
#include "spk/spk_amppf.h"
#include "spk/spk_gainset.h"
#include "amp/amp4_4_router.h"
#include "trn/trn_dynwidget.h"
#include "alarmconfig.h"

/*
 * 注意：
 * ControlValue中的delay在接收的地方需要进行" / 3"的处理
 *
 *
 *
 *
 * */
// 构造函数
DevSetting_Amp4_4::DevSetting_Amp4_4(DeviceItem_Amp4_4 *devItem,  QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::DevSetting_Amp4_4)
{
    _ui->setupUi(this);
    _devItem = devItem;
    Q_ASSERT(devItem->getDanteDevice());
    _devAmp = qobject_cast<Device_Amp4_4*>(devItem->getDanteDevice());

//    _devAmp->sendInitPack();
    connect(_devAmp, SIGNAL(adcDataChanged(AlarmConfig*)), this, SLOT(onAdcDataChanged(AlarmConfig*)));
    setAttribute(Qt::WA_QuitOnClose, true);

    _hasData = false;
    _timer = new QTimer;
    connect(_timer, SIGNAL(timeout()), this, SLOT(closeSelf()));
    _timer->start(3000);

    _ui->deviceName->setText(_devItem->customName());
    _ui->IPAddr->setText(_devAmp->getDeviceIp());
    _ui->MacAddrLabel->setText(_devAmp->getMac());

    _syncing = false;
    setFixedSize(size());
//    resetVoltCurrLimit(_devAmp->getConfigData().volumeCurrentType);

    displayConfigValue();
    connectSigSlots();

    factoryModeReInit();
    loadAlarmSetting();
    _ui->tab_2->setHidden(true);
}

void DevSetting_Amp4_4::closeSelf()
{
    if (!_hasData)
    {
//        close();
    }

    _hasData = false;
}

DevSetting_Amp4_4::~DevSetting_Amp4_4()
{
    saveAlarmSetting();

    delete _ui;
}

// 显示设备发来的电压电流温度等数据
void DevSetting_Amp4_4::onAdcDataChanged(AlarmConfig *alarmCfg)
{
//    if(_devAmp->isPowerOn())
    {
        double volt1Ch = alarmCfg->getParaValue(Volt1Ch);
        double volt2Ch = alarmCfg->getParaValue(Volt2Ch);
        double curr1Ch = alarmCfg->getParaValue(Curr1Ch);
        double curr2Ch = alarmCfg->getParaValue(Curr2Ch);
        double temperature = alarmCfg->getParaValue(Temperature);
        double volumeL = alarmCfg->getParaValue(VolumeL);
        double volumeR = alarmCfg->getParaValue(VolumeR);

        _ui->voltDispaly1->setLCurValue(volt1Ch);
        _ui->currDisplay1->setLCurValue(curr1Ch);
        _ui->voltDispaly2->setLCurValue(volt2Ch);
        _ui->currDisplay2->setLCurValue(curr2Ch);
        _ui->tempDisplay1->setLCurValue(temperature);
        _ui->tempDisplay2->setLCurValue(temperature);

        _ui->labelVol1->setText(QString::number(volt1Ch, 'f', 1));
        _ui->labelCurr1->setText(QString::number(curr1Ch, 'f', 1));
        _ui->labelTemp1->setText(QString::number(temperature, 'f', 1));
        _ui->labelVol2->setText(QString::number(volt2Ch, 'f', 1));
        _ui->labelCurr2->setText(QString::number(curr2Ch, 'f', 1));
        _ui->labelTemp2->setText(QString::number(temperature, 'f', 1));

        _ui->voluDisplay1->setLCurValue(volumeL);
        _ui->labelVloume1->setText(QString::number(volumeL, 'f', 1));
        _ui->voluDisplay2->setLCurValue(volumeR);
        _ui->labelVloume2->setText(QString::number(volumeR, 'f', 1));
    }
    _hasData = true;
}

void DevSetting_Amp4_4::connectSigSlots()
{
    /* 报警相关 */
    connect(_ui->enableAlarm, SIGNAL(toggled(bool)), this, SLOT(setAlarmMode(bool)));
    connect(_ui->alarmButton, SIGNAL(clicked()), this, SLOT(onShowAlarmDlg()));
    connect(_ui->voltMode1, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableWidgets(int)));
    connect(_ui->currMode1, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableWidgets(int)));
    connect(_ui->tempMode1, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableWidgets(int)));
    connect(_ui->vuMeterMode1, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableWidgets(int)));
    connect(_ui->voltLowerLimit1, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->voltUpperLimit1, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->currLowerLimit1, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->currUpperLimit1, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->tempLowerLimit1, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->tempUpperLimit1, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->vuMeterLowerLimit1, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->vuMeterUpperLimit1, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->voltMode2, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableWidgets(int)));
    connect(_ui->currMode2, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableWidgets(int)));
    connect(_ui->tempMode2, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableWidgets(int)));
    connect(_ui->vuMeterMode2, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableWidgets(int)));
    connect(_ui->voltLowerLimit2, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->voltUpperLimit2, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->currLowerLimit2, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->currUpperLimit2, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->tempLowerLimit2, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->tempUpperLimit2, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->vuMeterLowerLimit2, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->vuMeterUpperLimit2, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));

    /* 第一路延时选择 */
    _ui->delay1Value->setValidator(new QIntValidator(_ui->delay1Slider->minimum(), _ui->delay1Slider->maximum()));
    connect(_ui->delay1Slider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(_ui->delay1Value, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* 第一路音量选择 */
    _ui->gain1Value->setValidator(new QIntValidator(_ui->gain1Slider->minimum(), _ui->gain1Slider->maximum()));
    connect(_ui->gain1Slider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(_ui->gain1Value, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* 第二路延时选择 */
    _ui->delay2Value->setValidator(new QIntValidator(_ui->delay2Slider->minimum(), _ui->delay2Slider->maximum()));
    connect(_ui->delay2Slider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(_ui->delay2Value, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* 第二路音量选择 */
    _ui->gain2Value->setValidator(new QIntValidator(_ui->gain2Slider->minimum(), _ui->gain2Slider->maximum()));
    connect(_ui->gain2Slider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(_ui->gain2Value, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* 第三路延时选择 */
    _ui->delay3Value->setValidator(new QIntValidator(_ui->delay3Slider->minimum(), _ui->delay3Slider->maximum()));
    connect(_ui->delay3Slider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(_ui->delay3Value, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* 第三路音量选择 */
    _ui->gain3Value->setValidator(new QIntValidator(_ui->gain3Slider->minimum(), _ui->gain3Slider->maximum()));
    connect(_ui->gain3Slider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(_ui->gain3Value, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* 第四路延时选择 */
    _ui->delay4Value->setValidator(new QIntValidator(_ui->delay4Slider->minimum(), _ui->delay4Slider->maximum()));
    connect(_ui->delay4Slider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(_ui->delay4Value, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* 第四路音量选择 */
    _ui->gain4Value->setValidator(new QIntValidator(_ui->gain4Slider->minimum(), _ui->gain4Slider->maximum()));
    connect(_ui->gain4Slider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(_ui->gain4Value, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* 从设备初始化界面 */
    connect(_devAmp, SIGNAL(initConfigFromDevice()), this, SLOT(displayConfigValue()));
    /* 和monitor界面同步 */
//    connect(_devAmp, SIGNAL(volumeChanged()), this, SLOT(syncVolumeChanged()));
//    connect(_devAmp, SIGNAL(muteChanged()), this, SLOT(syncMuteChanged()));
//    connect(_devAmp, SIGNAL(phaseChanged()), this, SLOT(syncPhaseChanged()));
}

// 报警模式，disable/enable相应窗口
void DevSetting_Amp4_4::setEnableWidgets(int i)
{
    AlarmConfig *alarmCfg = _devAmp->getAlarmCfg();
    if(sender() == _ui->voltMode1)
    {
        alarmCfg->setLimitMode(Volt1Ch, i);
        if(i == 0)
        {
            _ui->voltLowerLimit1->setDisabled(true);
            _ui->voltUpperLimit1->setDisabled(true);
        }
        else if(i == 1)
        {
            _ui->voltLowerLimit1->setDisabled(false);
            _ui->voltUpperLimit1->setDisabled(false);
        }
        else if(i == 2)
        {
            _ui->voltLowerLimit1->setDisabled(false);
            _ui->voltUpperLimit1->setDisabled(true);
        }
        else if(i == 3)
        {
            _ui->voltLowerLimit1->setDisabled(true);
            _ui->voltUpperLimit1->setDisabled(false);
        }
    }
    else if(sender() == _ui->currMode1)
    {
        alarmCfg->setLimitMode(Curr1Ch, i);
        if(i == 0)
        {
            _ui->currLowerLimit1->setDisabled(true);
            _ui->currUpperLimit1->setDisabled(true);
        }
        else if(i == 1)
        {
            _ui->currLowerLimit1->setDisabled(false);
            _ui->currUpperLimit1->setDisabled(false);
        }
        else if(i == 2)
        {
            _ui->currLowerLimit1->setDisabled(false);
            _ui->currUpperLimit1->setDisabled(true);
        }
        else if(i == 3)
        {
            _ui->currLowerLimit1->setDisabled(true);
            _ui->currUpperLimit1->setDisabled(false);
        }
    }
    else if(sender() == _ui->voltMode2)
    {
        alarmCfg->setLimitMode(Volt2Ch, i);
        if(i == 0)
        {
            _ui->voltLowerLimit2->setDisabled(true);
            _ui->voltUpperLimit2->setDisabled(true);
        }
        else if(i == 1)
        {
            _ui->voltLowerLimit2->setDisabled(false);
            _ui->voltUpperLimit2->setDisabled(false);
        }
        else if(i == 2)
        {
            _ui->voltLowerLimit2->setDisabled(false);
            _ui->voltUpperLimit2->setDisabled(true);
        }
        else if(i == 3)
        {
            _ui->voltLowerLimit2->setDisabled(true);
            _ui->voltUpperLimit2->setDisabled(false);
        }
    }
    else if(sender() == _ui->currMode2)
    {
        alarmCfg->setLimitMode(Curr2Ch, i);
        if(i == 0)
        {
            _ui->currLowerLimit2->setDisabled(true);
            _ui->currUpperLimit2->setDisabled(true);
        }
        else if(i == 1)
        {
            _ui->currLowerLimit2->setDisabled(false);
            _ui->currUpperLimit2->setDisabled(false);
        }
        else if(i == 2)
        {
            _ui->currLowerLimit2->setDisabled(false);
            _ui->currUpperLimit2->setDisabled(true);
        }
        else if(i == 3)
        {
            _ui->currLowerLimit2->setDisabled(true);
            _ui->currUpperLimit2->setDisabled(false);
        }
    }
    else if(sender() == _ui->tempMode1 || sender() == _ui->tempMode2)
    {
        alarmCfg->setLimitMode(Temperature, i);
        if(i == 0)
        {
            _ui->tempLowerLimit1->setDisabled(true);
            _ui->tempUpperLimit1->setDisabled(true);
            _ui->tempLowerLimit2->setDisabled(true);
            _ui->tempUpperLimit2->setDisabled(true);
        }
        else if(i == 1)
        {
            _ui->tempLowerLimit1->setDisabled(false);
            _ui->tempUpperLimit1->setDisabled(false);
            _ui->tempLowerLimit2->setDisabled(false);
            _ui->tempUpperLimit2->setDisabled(false);
        }
        else if(i == 2)
        {
            _ui->tempLowerLimit1->setDisabled(false);
            _ui->tempUpperLimit1->setDisabled(true);
            _ui->tempLowerLimit2->setDisabled(false);
            _ui->tempUpperLimit2->setDisabled(true);
        }
        else if(i == 3)
        {
            _ui->tempLowerLimit1->setDisabled(true);
            _ui->tempUpperLimit1->setDisabled(false);
            _ui->tempLowerLimit2->setDisabled(true);
            _ui->tempUpperLimit2->setDisabled(false);
        }
    }
    else if (sender() == _ui->vuMeterMode1)
    {
        alarmCfg->setLimitMode(VolumeL, i);
        if(i == 0)
        {
            _ui->vuMeterLowerLimit1->setDisabled(true);
            _ui->vuMeterUpperLimit1->setDisabled(true);
        }
        else if(i == 1)
        {
            _ui->vuMeterLowerLimit1->setDisabled(false);
            _ui->vuMeterUpperLimit1->setDisabled(false);
        }
        else if(i == 2)
        {
            _ui->vuMeterLowerLimit1->setDisabled(false);
            _ui->vuMeterUpperLimit1->setDisabled(true);
        }
        else if(i == 3)
        {
            _ui->vuMeterLowerLimit1->setDisabled(true);
            _ui->vuMeterUpperLimit1->setDisabled(false);
        }
    }
    else if (sender() == _ui->vuMeterMode2)
    {
        alarmCfg->setLimitMode(VolumeR, i);
        if(i == 0)
        {
            _ui->vuMeterLowerLimit2->setDisabled(true);
            _ui->vuMeterUpperLimit2->setDisabled(true);
        }
        else if(i == 1)
        {
            _ui->vuMeterLowerLimit2->setDisabled(false);
            _ui->vuMeterUpperLimit2->setDisabled(false);
        }
        else if(i == 2)
        {
            _ui->vuMeterLowerLimit2->setDisabled(false);
            _ui->vuMeterUpperLimit2->setDisabled(true);
        }
        else if(i == 3)
        {
            _ui->vuMeterLowerLimit2->setDisabled(true);
            _ui->vuMeterUpperLimit2->setDisabled(false);
        }
    }
}

void DevSetting_Amp4_4::setAlarmMode(bool b)
{
    _ui->groupBox->setEnabled(b);
    _ui->groupBox_7->setEnabled(b);
    _devAmp->getAlarmCfg()->setAlarmEnable(b);
}

void DevSetting_Amp4_4::setLimiterValue()
{
    AlarmConfig *alarmCfg = _devAmp->getAlarmCfg();
    if(sender() == _ui->voltLowerLimit1)
    {
        alarmCfg->setMinLimit(Volt1Ch, _ui->voltLowerLimit1->value());
    }
    else if(sender() == _ui->voltUpperLimit1)
    {
        alarmCfg->setMaxLimit(Volt1Ch, _ui->voltUpperLimit1->value());
    }
    else if(sender() == _ui->currLowerLimit1)
    {
        alarmCfg->setMinLimit(Curr1Ch, _ui->currLowerLimit1->value());
    }
    else if(sender() == _ui->currUpperLimit1)
    {
        alarmCfg->setMaxLimit(Curr1Ch, _ui->currUpperLimit1->value());
    }
    else if(sender() == _ui->tempLowerLimit1)
    {
        alarmCfg->setMinLimit(Temperature, _ui->tempLowerLimit1->value());
        _ui->tempLowerLimit2->setValue(_ui->tempLowerLimit1->value());
    }
    else if(sender() == _ui->tempUpperLimit1)
    {
        alarmCfg->setMaxLimit(Temperature, _ui->tempUpperLimit1->value());
        _ui->tempUpperLimit2->setValue(_ui->tempUpperLimit1->value());
    }
    else if (sender() == _ui->vuMeterLowerLimit1)
    {
        alarmCfg->setMinLimit(VolumeL, _ui->vuMeterLowerLimit1->value());
    }
    else if (sender() == _ui->vuMeterUpperLimit1)
    {
        alarmCfg->setMaxLimit(VolumeL, _ui->vuMeterUpperLimit1->value());
    }

    else if(sender() == _ui->voltLowerLimit2)
    {
        alarmCfg->setMinLimit(Volt2Ch, _ui->voltLowerLimit2->value());
    }
    else if(sender() == _ui->voltUpperLimit2)
    {
        alarmCfg->setMaxLimit(Volt2Ch, _ui->voltUpperLimit2->value());
    }
    else if(sender() == _ui->currLowerLimit2)
    {
        alarmCfg->setMinLimit(Curr2Ch, _ui->currLowerLimit2->value());
    }
    else if(sender() == _ui->currUpperLimit2)
    {
        alarmCfg->setMaxLimit(Curr2Ch, _ui->currUpperLimit2->value());
    }
    else if(sender() == _ui->tempLowerLimit2)
    {
        alarmCfg->setMinLimit(Temperature, _ui->tempLowerLimit2->value());
        _ui->tempLowerLimit1->setValue(_ui->tempLowerLimit2->value());
    }
    else if(sender() == _ui->tempUpperLimit2)
    {
        alarmCfg->setMaxLimit(Temperature, _ui->tempUpperLimit2->value());
        _ui->tempUpperLimit1->setValue(_ui->tempUpperLimit2->value());
    }
    else if (sender() == _ui->vuMeterLowerLimit2)
    {
        alarmCfg->setMinLimit(VolumeR, _ui->vuMeterLowerLimit2->value());
    }
    else if (sender() == _ui->vuMeterUpperLimit2)
    {
        alarmCfg->setMaxLimit(VolumeR, _ui->vuMeterUpperLimit2->value());
    }
}

// 保存到设备
void DevSetting_Amp4_4::on_saveBtn_clicked()
{
    _devAmp->sendSave2DevicePack();
}

void DevSetting_Amp4_4::displayConfigValue()
{
    _syncing = true;
    ConfigData_Amp4_4& cfgData = _devAmp->getConfigData();

    _ui->tab_3->setEnabled(true/*cfgData.Switch*/);

    _ui->delay1Slider->setValue(cfgData.CH[Device_Amp4_4::AMP_CH1].ControlValue.delay / 3);
    _ui->delay1Value->setText(QString::number(cfgData.CH[Device_Amp4_4::AMP_CH1].ControlValue.delay / 3));
    _ui->gain1Slider->setValue(cfgData.CH[Device_Amp4_4::AMP_CH1].ControlValue.volume);
    _ui->gain1Value->setText(QString::number(cfgData.CH[Device_Amp4_4::AMP_CH1].ControlValue.volume));
    _ui->gain1M->setChecked(cfgData.CH[Device_Amp4_4::AMP_CH1].ControlValue.mute);
    _ui->gain1P->setChecked(cfgData.CH[Device_Amp4_4::AMP_CH1].ControlValue.phase);

    _ui->delay2Slider->setValue(cfgData.CH[Device_Amp4_4::AMP_CH2].ControlValue.delay / 3);
    _ui->delay2Value->setText(QString::number(cfgData.CH[Device_Amp4_4::AMP_CH2].ControlValue.delay / 3));
    _ui->gain2Slider->setValue(cfgData.CH[Device_Amp4_4::AMP_CH2].ControlValue.volume);
    _ui->gain2Value->setText(QString::number(cfgData.CH[Device_Amp4_4::AMP_CH2].ControlValue.volume));
    _ui->gain2M->setChecked(cfgData.CH[Device_Amp4_4::AMP_CH2].ControlValue.mute);
    _ui->gain2P->setChecked(cfgData.CH[Device_Amp4_4::AMP_CH2].ControlValue.phase);

    _ui->delay3Slider->setValue(cfgData.CH[Device_Amp4_4::AMP_CH3].ControlValue.delay / 3);
    _ui->delay3Value->setText(QString::number(cfgData.CH[Device_Amp4_4::AMP_CH3].ControlValue.delay / 3));
    _ui->gain3Slider->setValue(cfgData.CH[Device_Amp4_4::AMP_CH3].ControlValue.volume);
    _ui->gain3Value->setText(QString::number(cfgData.CH[Device_Amp4_4::AMP_CH3].ControlValue.volume));
    _ui->gain3M->setChecked(cfgData.CH[Device_Amp4_4::AMP_CH3].ControlValue.mute);
    _ui->gain3P->setChecked(cfgData.CH[Device_Amp4_4::AMP_CH3].ControlValue.phase);

    _ui->delay4Slider->setValue(cfgData.CH[Device_Amp4_4::AMP_CH4].ControlValue.delay / 3);
    _ui->delay4Value->setText(QString::number(cfgData.CH[Device_Amp4_4::AMP_CH4].ControlValue.delay / 3));
    _ui->gain4Slider->setValue(cfgData.CH[Device_Amp4_4::AMP_CH4].ControlValue.volume);
    _ui->gain4Value->setText(QString::number(cfgData.CH[Device_Amp4_4::AMP_CH4].ControlValue.volume));
    _ui->gain4M->setChecked(cfgData.CH[Device_Amp4_4::AMP_CH4].ControlValue.mute);
    _ui->gain4P->setChecked(cfgData.CH[Device_Amp4_4::AMP_CH4].ControlValue.phase);

    if (cfgData.CH[Device_Amp4_4::AMP_CH1].divider.hf.bypass == 0x01 && cfgData.CH[Device_Amp4_4::AMP_CH1].divider.lf.bypass == 0x01)
    {
        _ui->filter1Bypass->setChecked(true);
        emit _ui->filter1Bypass->clicked(true);
    }
    else if (cfgData.CH[Device_Amp4_4::AMP_CH1].divider.hf.bypass == 0x01)
    {
        _ui->filter1LPF->setChecked(true);
        emit _ui->filter1LPF->clicked(true);
    }
    else if (cfgData.CH[Device_Amp4_4::AMP_CH1].divider.lf.bypass == 0x01)
    {
        _ui->filter1HPF->setChecked(true);
        emit _ui->filter1HPF->clicked(true);
    }

    if (cfgData.CH[Device_Amp4_4::AMP_CH2].divider.hf.bypass == 0x01 && cfgData.CH[Device_Amp4_4::AMP_CH1].divider.lf.bypass == 0x01)
    {
        _ui->filter2Bypass->setChecked(true);
        emit _ui->filter2Bypass->clicked(true);
    }
    else if (cfgData.CH[Device_Amp4_4::AMP_CH2].divider.hf.bypass == 0x01)
    {
        _ui->filter2LPF->setChecked(true);
        emit _ui->filter2LPF->clicked(true);
    }
    else if (cfgData.CH[Device_Amp4_4::AMP_CH2].divider.lf.bypass == 0x01)
    {
        _ui->filter2HPF->setChecked(true);
        emit _ui->filter2HPF->clicked(true);
    }

    if (cfgData.CH[Device_Amp4_4::AMP_CH3].divider.hf.bypass == 0x01 && cfgData.CH[Device_Amp4_4::AMP_CH3].divider.lf.bypass == 0x01)
    {
        _ui->filter3Bypass->setChecked(true);
        emit _ui->filter3Bypass->clicked(true);
    }
    else if (cfgData.CH[Device_Amp4_4::AMP_CH3].divider.hf.bypass == 0x01)
    {
        _ui->filter3LPF->setChecked(true);
        emit _ui->filter3LPF->clicked(true);
    }
    else if (cfgData.CH[Device_Amp4_4::AMP_CH3].divider.lf.bypass == 0x01)
    {
        _ui->filter3HPF->setChecked(true);
        emit _ui->filter3HPF->clicked(true);
    }

    if (cfgData.CH[Device_Amp4_4::AMP_CH4].divider.hf.bypass == 0x01 && cfgData.CH[Device_Amp4_4::AMP_CH4].divider.lf.bypass == 0x01)
    {
        _ui->filter4Bypass->setChecked(true);
        emit _ui->filter4Bypass->clicked(true);
    }
    else if (cfgData.CH[Device_Amp4_4::AMP_CH4].divider.hf.bypass == 0x01)
    {
        _ui->filter4LPF->setChecked(true);
        emit _ui->filter4LPF->clicked(true);
    }
    else if (cfgData.CH[Device_Amp4_4::AMP_CH4].divider.lf.bypass == 0x01)
    {
        _ui->filter4HPF->setChecked(true);
        emit _ui->filter4HPF->clicked(true);
    }

    _syncing = false;
}

void DevSetting_Amp4_4::onShowAlarmDlg()
{
    AlarmRecord alarmFrm(_devAmp->getMac());
    alarmFrm.exec();
}

void DevSetting_Amp4_4::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Enter:
        break;
    case Qt::Key_F5:
        if (e->modifiers() == (Qt::ControlModifier | Qt::AltModifier | Qt::ShiftModifier))
        {
            _devAmp->changeFactoryMode();
            factoryModeReInit();
        }
    default:
        break;
    }
}

void DevSetting_Amp4_4::factoryModeReInit()
{
    if (_devAmp->isFactoryMode())
    {
        _ui->pushButton60v->hide();
        _ui->pushButton100v->hide();
    } else {
        _ui->pushButton60v->show();
        _ui->pushButton100v->show();
    }
}

// 另存场景为文件
void DevSetting_Amp4_4::on_saveAsBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("save as..."), QDir::currentPath(), QString("file (*.amp)"));
    if(!fileName.isEmpty()){
        _devAmp->serialize(fileName);
    }
}

// 加载场景文件
void DevSetting_Amp4_4::on_loadBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("file (*.amp)"));
    if(!fileName.isEmpty())
    {
        _devAmp->deserialize(fileName);
        displayConfigValue();
        _devAmp->sendAllConfigPack(true);
    }

//    /* 在线更新功能 */
//    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("file (*.bin)"));
//    QFile file(fileName);
//    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
//    QByteArray buff;
//    buff = file.readAll();
//    file.close();

//    QProgressDialog *progressDlg=new QProgressDialog(this);
//    progressDlg->setWindowModality(Qt::WindowModal);
//    progressDlg->setMinimumDuration(1);
//    progressDlg->setWindowTitle(tr("please wait"));
//    progressDlg->setLabelText(tr("Download......"));
//    progressDlg->setCancelButton(0);
//    quint32 totalCount = buff.count();
//    quint32 offset     = 0;
//    quint32 paraLen    = 128;
//    quint32 blk        = totalCount / paraLen;
//    progressDlg->setRange(0,blk);
//    for (quint32 i = 0; i < blk; ++i)
//    {
//        offset = i * 128;
//        _devAmp->sendUpdatePackage(totalCount, offset, paraLen, buff.mid(offset, paraLen));
//        progressDlg->setValue(i);
//        QThread::msleep(50);
//    }
//    progressDlg->setValue(blk);
//    paraLen = totalCount - blk * 128;
//    if (paraLen)
//    {
//        offset = blk * 128;
//        _devAmp->sendUpdatePackage(totalCount, offset, paraLen, buff.mid(offset, paraLen));
//    }
//    delete progressDlg;
}

void DevSetting_Amp4_4::on_pushButton60v_clicked()
{
    resetVoltCurrLimit(0);
    _devAmp->sendVoltTypePack(0);
}

void DevSetting_Amp4_4::on_pushButton100v_clicked()
{
    resetVoltCurrLimit(1);
    _devAmp->sendVoltTypePack(1);
}

//voltType 0:60V  1:100V
void DevSetting_Amp4_4::resetVoltCurrLimit(quint8 voltType)
{
    quint8 voltMax = (voltType == 0 ? 60 : 100);

    _devAmp->getAlarmCfg()->setMaxVolt(voltMax);
    _ui->voltLowerLimit1->setMaximum(voltMax);
    _ui->voltUpperLimit1->setMaximum(voltMax);
    _ui->voltUpperLimit1->setValue(voltMax);
    _ui->currLowerLimit1->setMaximum(voltMax/4);
    _ui->currUpperLimit1->setMaximum(voltMax/4);
    _ui->currUpperLimit1->setValue(voltMax/4);

    _ui->voltUpper1->setText(QString::asprintf("%d", voltMax));
    _ui->currUpper1->setText(QString::asprintf("%d", voltMax/4));

    _ui->voltDispaly1->setMaxValue(voltMax);
    _ui->currDisplay1->setMaxValue(voltMax/4);

    _ui->voltLowerLimit2->setMaximum(voltMax);
    _ui->voltUpperLimit2->setMaximum(voltMax);
    _ui->voltUpperLimit2->setValue(voltMax);
    _ui->currLowerLimit2->setMaximum(voltMax/4);
    _ui->currUpperLimit2->setMaximum(voltMax/4);
    _ui->currUpperLimit2->setValue(voltMax/4);

    _ui->voltUpper2->setText(QString::asprintf("%d", voltMax));
    _ui->currUpper2->setText(QString::asprintf("%d", voltMax/4));

    _ui->voltDispaly2->setMaxValue(voltMax);
    _ui->currDisplay2->setMaxValue(voltMax/4);
}

/******************************************************************************************
 *
 * 延时、音量、禁音、反向
 *
 *****************************************************************************************/
void DevSetting_Amp4_4::on_gain1P_clicked(bool checked)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    _devAmp->sendPhase(Device_Amp4_4::AMP_CH1, checked);
}

void DevSetting_Amp4_4::on_gain1M_clicked(bool checked)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    _devAmp->sendMute(Device_Amp4_4::AMP_CH1, checked);
}

void DevSetting_Amp4_4::on_gain2P_clicked(bool checked)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    _devAmp->sendPhase(Device_Amp4_4::AMP_CH2, checked);
}

void DevSetting_Amp4_4::on_gain2M_clicked(bool checked)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    _devAmp->sendMute(Device_Amp4_4::AMP_CH2, checked);
}

void DevSetting_Amp4_4::on_gain3P_clicked(bool checked)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    _devAmp->sendPhase(Device_Amp4_4::AMP_CH3, checked);
}

void DevSetting_Amp4_4::on_gain3M_clicked(bool checked)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    _devAmp->sendMute(Device_Amp4_4::AMP_CH3, checked);
}

void DevSetting_Amp4_4::on_gain4P_clicked(bool checked)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    _devAmp->sendPhase(Device_Amp4_4::AMP_CH4, checked);
}

void DevSetting_Amp4_4::on_gain4M_clicked(bool checked)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    _devAmp->sendMute(Device_Amp4_4::AMP_CH4, checked);
}

/* 推子最后处理 */
void DevSetting_Amp4_4::setIntValueDisplay(int i)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    ControlValue *cntvlu = NULL;
    //line1
    if(sender() == _ui->delay1Slider)
    {
        _ui->delay1Value->setText(QString::number(i));
        cntvlu = &_devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH1].ControlValue;
        cntvlu->delay = i * 3;

        _devAmp->sendCommonDataPack(Device_Amp4_4::AMP_CH1, *cntvlu);
    }
    else if (sender() == _ui->gain1Slider)
    {
        _ui->gain1Value->setText(QString::number(i));

        _devAmp->sendVolume(Device_Amp4_4::AMP_CH1, i);
    }

    //line2
    else if(sender() == _ui->delay2Slider)
    {
        _ui->delay2Value->setText(QString::number(i));
        cntvlu = &_devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH2].ControlValue;//.ch2_ControlValue;
        cntvlu->delay = i * 3;

        _devAmp->sendCommonDataPack(Device_Amp4_4::AMP_CH2, *cntvlu);
    }
    else if (sender() == _ui->gain2Slider)
    {
        _ui->gain2Value->setText(QString::number(i));

        _devAmp->sendVolume(Device_Amp4_4::AMP_CH2, i);
    }

    //line3
    else if(sender() == _ui->delay3Slider)
    {
        _ui->delay3Value->setText(QString::number(i));
        cntvlu = &_devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH3].ControlValue;
        cntvlu->delay = i * 3;

        _devAmp->sendCommonDataPack(Device_Amp4_4::AMP_CH3, *cntvlu);
    }
    else if (sender() == _ui->gain3Slider)
    {
        _ui->gain3Value->setText(QString::number(i));

        _devAmp->sendVolume(Device_Amp4_4::AMP_CH3, i);
    }

    //line4
    else if(sender() == _ui->delay4Slider)
    {
        _ui->delay4Value->setText(QString::number(i));
        cntvlu = &_devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH4].ControlValue;
        cntvlu->delay = i * 3;

        _devAmp->sendCommonDataPack(Device_Amp4_4::AMP_CH4, *cntvlu);
    }
    else if (sender() == _ui->gain4Slider)
    {
        _ui->gain4Value->setText(QString::number(i));

        _devAmp->sendVolume(Device_Amp4_4::AMP_CH4, i);
    }
}

void DevSetting_Amp4_4::setSliderValue()
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    //line1
    if(sender() == _ui->delay1Value)
    {
        _ui->delay1Slider->setValue(_ui->delay1Value->text().toInt());
    }
    else if(sender() == _ui->gain1Value)
    {
        _ui->gain1Slider->setValue(_ui->gain1Value->text().toInt());
    }

    //line2
    else if(sender() == _ui->delay2Value)
    {
        _ui->delay2Slider->setValue(_ui->delay2Value->text().toInt());
    }
    else if(sender() == _ui->gain2Value)
    {
        _ui->gain2Slider->setValue(_ui->gain2Value->text().toInt());
    }

    //line3
    if(sender() == _ui->delay3Value)
    {
        _ui->delay3Slider->setValue(_ui->delay3Value->text().toInt());
    }
    else if(sender() == _ui->gain3Value)
    {
        _ui->gain3Slider->setValue(_ui->gain3Value->text().toInt());
    }

    //line4
    if(sender() == _ui->delay4Value)
    {
        _ui->delay4Slider->setValue(_ui->delay4Value->text().toInt());
    }
    else if(sender() == _ui->gain4Value)
    {
        _ui->gain4Slider->setValue(_ui->gain4Value->text().toInt());
    }
}

/******************************************************************************************
 *
 * 路由
 *
 *****************************************************************************************/
void DevSetting_Amp4_4::on_router_clicked()
{
    Amp4_4Router router(_devAmp, this);
    router.exec();
}

/******************************************************************************************
 *
 * EQ
 *
 *****************************************************************************************/
/* 第一路 */
void DevSetting_Amp4_4::on_eq1_clicked()
{
    EqData4B& eq4B = _devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH1].Eq;//.ch1_Eq;
    QDialog dlg;
    EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
    eqView->showWidget(Device_Amp4_4::AMP_CH1, QString(), eq4B.bypass, eq4B.eq);
    eqView->setFixedSize(600,350);
    dlg.setStyleSheet(QLatin1String("QDialog{background-image: url(:/Image/back.png);}"));
    dlg.resize(eqView->size());
    connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
    dlg.exec();
}

/* 第二路 */
void DevSetting_Amp4_4::on_eq2_clicked()
{
    EqData4B& eq4B = _devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH2].Eq;//.ch2_Eq;
    QDialog dlg;
    EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
    eqView->showWidget(Device_Amp4_4::AMP_CH2, QString(), eq4B.bypass, eq4B.eq);
    eqView->setFixedSize(600,350);
    dlg.setStyleSheet(QLatin1String("QDialog{background-image: url(:/Image/back.png);}"));
    dlg.resize(eqView->size());
    connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
    dlg.exec();
}

/* 第三路 */
void DevSetting_Amp4_4::on_eq3_clicked()
{
    EqData4B& eq4B = _devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH3].Eq;
    QDialog dlg;
    EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
    eqView->showWidget(Device_Amp4_4::AMP_CH3, QString(), eq4B.bypass, eq4B.eq);
    eqView->setFixedSize(600,350);
    dlg.setStyleSheet(QLatin1String("QDialog{background-image: url(:/Image/back.png);}"));
    dlg.resize(eqView->size());
    connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
    dlg.exec();
}

/* 第四路 */
void DevSetting_Amp4_4::on_eq4_clicked()
{
    EqData4B& eq4B = _devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH4].Eq;
    QDialog dlg;
    EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
    eqView->showWidget(Device_Amp4_4::AMP_CH4, QString(), eq4B.bypass, eq4B.eq);
    eqView->setFixedSize(600,350);
    dlg.setStyleSheet(QLatin1String("QDialog{background-image: url(:/Image/back.png);}"));
    dlg.resize(eqView->size());
    connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
    dlg.exec();
}

/* 处理函数 */
void DevSetting_Amp4_4::onEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    _devAmp->sendEqPack(ch, bypass, eqPara);
}

/******************************************************************************************
 *
 * Filter
 *
 *****************************************************************************************/
/* 第一路 */
void DevSetting_Amp4_4::on_filter1HPF_clicked()
{
    _ui->filter1HPFSet->setEnabled(true);
    _ui->filter1LPFSet->setEnabled(false);

    onPfDataChanged(Device_Amp4_4::AMP_CH1, false, true);
}

void DevSetting_Amp4_4::on_filter1Bypass_clicked()
{
    _ui->filter1HPFSet->setEnabled(false);
    _ui->filter1LPFSet->setEnabled(false);

    onPfDataChanged(Device_Amp4_4::AMP_CH1, true, true);
}

void DevSetting_Amp4_4::on_filter1LPF_clicked()
{
    _ui->filter1HPFSet->setEnabled(false);
    _ui->filter1LPFSet->setEnabled(true);

    onPfDataChanged(Device_Amp4_4::AMP_CH1, true, false);
}

void DevSetting_Amp4_4::on_filter1HPFSet_clicked()
{
    AmpPf pf(Device_Amp4_4::AMP_CH1, &_devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH1].divider.hf/*.ch1_divider.hf*/, AmpPfType_AMP);
    pf.setWindowTitle(tr("Channel1 HF"));
    connect(&pf, SIGNAL(pfDataChanged(quint8)), this, SLOT(onPfDataChanged(quint8)));
    pf.exec();
}

void DevSetting_Amp4_4::on_filter1LPFSet_clicked()
{
    AmpPf pf(Device_Amp4_4::AMP_CH1, &_devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH1].divider.lf/*.ch1_divider.lf*/, AmpPfType_AMP);
    pf.setWindowTitle(tr("Channel1 LF"));
    connect(&pf, SIGNAL(pfDataChanged(quint8)), this, SLOT(onPfDataChanged(quint8)));
    pf.exec();
}

/* 第二路 */
void DevSetting_Amp4_4::on_filter2HPF_clicked()
{
    _ui->filter2HPFSet->setEnabled(true);
    _ui->filter2LPFSet->setEnabled(false);

    onPfDataChanged(Device_Amp4_4::AMP_CH2, false, true);
}

void DevSetting_Amp4_4::on_filter2Bypass_clicked()
{
    _ui->filter2HPFSet->setEnabled(false);
    _ui->filter2LPFSet->setEnabled(false);

    onPfDataChanged(Device_Amp4_4::AMP_CH2, true, true);
}

void DevSetting_Amp4_4::on_filter2LPF_clicked()
{
    _ui->filter2HPFSet->setEnabled(false);
    _ui->filter2LPFSet->setEnabled(true);

    onPfDataChanged(Device_Amp4_4::AMP_CH2, true, false);
}

void DevSetting_Amp4_4::on_filter2HPFSet_clicked()
{
    AmpPf pf(Device_Amp4_4::AMP_CH2, &_devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH2].divider.hf/*.ch2_divider.hf*/, AmpPfType_AMP);
    pf.setWindowTitle(tr("Channel2 HF"));
    connect(&pf, SIGNAL(pfDataChanged(quint8)), this, SLOT(onPfDataChanged(quint8)));
    pf.exec();
}

void DevSetting_Amp4_4::on_filter2LPFSet_clicked()
{
    AmpPf pf(Device_Amp4_4::AMP_CH2, &_devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH2].divider.lf/*.ch2_divider.lf*/, AmpPfType_AMP);
    pf.setWindowTitle(tr("Channel2 LF"));
    connect(&pf, SIGNAL(pfDataChanged(quint8)), this, SLOT(onPfDataChanged(quint8)));
    pf.exec();
}

/* 第三路 */
void DevSetting_Amp4_4::on_filter3HPF_clicked()
{
    _ui->filter3HPFSet->setEnabled(true);
    _ui->filter3LPFSet->setEnabled(false);

    onPfDataChanged(Device_Amp4_4::AMP_CH3, false, true);
}

void DevSetting_Amp4_4::on_filter3Bypass_clicked()
{
    _ui->filter3HPFSet->setEnabled(false);
    _ui->filter3LPFSet->setEnabled(false);

    onPfDataChanged(Device_Amp4_4::AMP_CH3, true, true);
}

void DevSetting_Amp4_4::on_filter3LPF_clicked()
{
    _ui->filter3HPFSet->setEnabled(false);
    _ui->filter3LPFSet->setEnabled(true);

    onPfDataChanged(Device_Amp4_4::AMP_CH3, true, false);
}

void DevSetting_Amp4_4::on_filter3HPFSet_clicked()
{
    AmpPf pf(Device_Amp4_4::AMP_CH3, &_devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH3].divider.hf, AmpPfType_AMP);
    pf.setWindowTitle(tr("Channel3 HF"));
    connect(&pf, SIGNAL(pfDataChanged(quint8)), this, SLOT(onPfDataChanged(quint8)));
    pf.exec();
}

void DevSetting_Amp4_4::on_filter3LPFSet_clicked()
{
    AmpPf pf(Device_Amp4_4::AMP_CH3, &_devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH3].divider.lf, AmpPfType_AMP);
    pf.setWindowTitle(tr("Channel3 LF"));
    connect(&pf, SIGNAL(pfDataChanged(quint8)), this, SLOT(onPfDataChanged(quint8)));
    pf.exec();
}

/* 第四路 */
void DevSetting_Amp4_4::on_filter4HPF_clicked()
{
    _ui->filter4HPFSet->setEnabled(true);
    _ui->filter4LPFSet->setEnabled(false);

    onPfDataChanged(Device_Amp4_4::AMP_CH4, false, true);
}

void DevSetting_Amp4_4::on_filter4Bypass_clicked()
{
    _ui->filter4HPFSet->setEnabled(false);
    _ui->filter4LPFSet->setEnabled(false);

    onPfDataChanged(Device_Amp4_4::AMP_CH4, true, true);
}

void DevSetting_Amp4_4::on_filter4LPF_clicked()
{
    _ui->filter4HPFSet->setEnabled(false);
    _ui->filter4LPFSet->setEnabled(true);

    onPfDataChanged(Device_Amp4_4::AMP_CH4, true, false);
}

void DevSetting_Amp4_4::on_filter4HPFSet_clicked()
{
    AmpPf pf(Device_Amp4_4::AMP_CH4, &_devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH4].divider.hf, AmpPfType_AMP);
    pf.setWindowTitle(tr("Channel4 HF"));
    connect(&pf, SIGNAL(pfDataChanged(quint8)), this, SLOT(onPfDataChanged(quint8)));
    pf.exec();
}

void DevSetting_Amp4_4::on_filter4LPFSet_clicked()
{
    AmpPf pf(Device_Amp4_4::AMP_CH4, &_devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH4].divider.lf, AmpPfType_AMP);
    pf.setWindowTitle(tr("Channel4 LF"));
    connect(&pf, SIGNAL(pfDataChanged(quint8)), this, SLOT(onPfDataChanged(quint8)));
    pf.exec();
}

/* 处理函数 */
void DevSetting_Amp4_4::onPfDataChanged(quint8 channel)
{
    Divider *divider = NULL;
    divider = &_devAmp->getConfigData().CH[channel].divider;
//    if(channel == 1) divider = &_devAmp->getConfigData().ch1_divider;
//    else divider = &_devAmp->getConfigData().ch2_divider;
    _devAmp->sendDividerPack(channel, divider->hf, divider->lf);
}

void DevSetting_Amp4_4::onPfDataChanged(quint8 ch, bool hfb, bool lfb)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    Divider *divider = NULL;
//    if(ch == 1) divider = &_devAmp->getConfigData().ch1_divider;
//    else divider = &_devAmp->getConfigData().ch2_divider;
    divider = &_devAmp->getConfigData().CH[ch].divider;

    divider->hf.bypass = hfb;
    divider->lf.bypass = lfb;
    _devAmp->sendDividerPack(ch, divider->hf, divider->lf);
}

/******************************************************************************************
 *
 * Dyn
 *
 *****************************************************************************************/
/* 第一路 */
void DevSetting_Amp4_4::on_dyn1_clicked()
{
    DynData& dynData = _devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH1].Dyn;//.ch1_Dyn;
    QDialog dlg;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
    dynWidget->setDisabled(_devAmp->isFactoryMode());
    dynWidget->showWidget(false, Device_Amp4_4::AMP_CH1, QString(), dynData);
    dlg.setStyleSheet(QLatin1String("QDialog{background-image: url(:/Image/back.png);}"));
    dlg.resize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

/* 第二路 */
void DevSetting_Amp4_4::on_dyn2_clicked()
{
    DynData& dynData = _devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH2].Dyn;//.ch2_Dyn;
    QDialog dlg;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
    dynWidget->setDisabled(_devAmp->isFactoryMode());
    dynWidget->showWidget(false, Device_Amp4_4::AMP_CH2, QString(), dynData);
    dlg.setStyleSheet(QLatin1String("QDialog{background-image: url(:/Image/back.png);}"));
    dlg.resize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

/* 第三路 */
void DevSetting_Amp4_4::on_dyn3_clicked()
{
    DynData& dynData = _devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH3].Dyn;
    QDialog dlg;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
    dynWidget->setDisabled(_devAmp->isFactoryMode());
    dynWidget->showWidget(false, Device_Amp4_4::AMP_CH3, QString(), dynData);
    dlg.setStyleSheet(QLatin1String("QDialog{background-image: url(:/Image/back.png);}"));
    dlg.resize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

/* 第四路 */
void DevSetting_Amp4_4::on_dyn4_clicked()
{
    DynData& dynData = _devAmp->getConfigData().CH[Device_Amp4_4::AMP_CH4].Dyn;
    QDialog dlg;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
    dynWidget->setDisabled(_devAmp->isFactoryMode());
    dynWidget->showWidget(false, Device_Amp4_4::AMP_CH4, QString(), dynData);
    dlg.setStyleSheet(QLatin1String("QDialog{background-image: url(:/Image/back.png);}"));
    dlg.resize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

/* 处理函数 */
void DevSetting_Amp4_4::onDynChanged(quint8 ch, bool isIn ,DynData dynData)
{
    Q_UNUSED(isIn)
    _devAmp->sendDynPack(ch, dynData);
}

/******************************************************************************************
 *
 * 报警
 *
 *****************************************************************************************/
void DevSetting_Amp4_4::loadAlarmSetting()
{
    AlarmConfig *alarmCfg = _devAmp->getAlarmCfg();

    _ui->enableAlarm->setChecked(alarmCfg->getAlarmEnable());
    LimitDef &volt1chLimit = alarmCfg->getLimitDef(Volt1Ch);
    _ui->voltMode1->setCurrentIndex(volt1chLimit.getLimitMode());
    _ui->voltLowerLimit1->setValue(volt1chLimit.minValue);
    emit _ui->voltLowerLimit1->editingFinished();
    _ui->voltUpperLimit1->setValue(volt1chLimit.maxValue);
    emit _ui->voltUpperLimit1->editingFinished();

    LimitDef &volt2chLimit = alarmCfg->getLimitDef(Volt2Ch);
    _ui->voltMode2->setCurrentIndex(volt2chLimit.getLimitMode());
    _ui->voltLowerLimit2->setValue(volt2chLimit.minValue);
    emit _ui->voltLowerLimit2->editingFinished();
    _ui->voltUpperLimit2->setValue(volt2chLimit.maxValue);
    emit _ui->voltUpperLimit2->editingFinished();

    LimitDef &curr1chLimit = alarmCfg->getLimitDef(Curr1Ch);
    _ui->currMode1->setCurrentIndex(curr1chLimit.getLimitMode());
    _ui->currLowerLimit1->setValue(curr1chLimit.minValue);
    emit _ui->currLowerLimit1->editingFinished();
    _ui->currUpperLimit1->setValue(curr1chLimit.maxValue);
    emit _ui->currUpperLimit1->editingFinished();

    LimitDef &curr2chLimit = alarmCfg->getLimitDef(Curr2Ch);
    _ui->currMode2->setCurrentIndex(curr2chLimit.getLimitMode());
    _ui->currLowerLimit2->setValue(curr2chLimit.minValue);
    emit _ui->currLowerLimit2->editingFinished();
    _ui->currUpperLimit2->setValue(curr2chLimit.maxValue);
    emit _ui->currUpperLimit2->editingFinished();

    LimitDef &temp1chLimit = alarmCfg->getLimitDef(Temperature);
    _ui->tempMode1->setCurrentIndex(temp1chLimit.getLimitMode());
    _ui->tempLowerLimit1->setValue(temp1chLimit.minValue);
    emit _ui->tempLowerLimit1->editingFinished();
    _ui->tempUpperLimit1->setValue(temp1chLimit.maxValue);
    emit _ui->tempUpperLimit1->editingFinished();

    LimitDef &temp2chLimit = alarmCfg->getLimitDef(Temperature);
    _ui->tempMode2->setCurrentIndex(temp2chLimit.getLimitMode());
    _ui->tempLowerLimit2->setValue(temp2chLimit.minValue);
    emit _ui->tempLowerLimit2->editingFinished();
    _ui->tempUpperLimit2->setValue(temp2chLimit.maxValue);
    emit _ui->tempUpperLimit2->editingFinished();

    LimitDef &volumeL = alarmCfg->getLimitDef(VolumeL);
    _ui->vuMeterMode1->setCurrentIndex(volumeL.getLimitMode());
    _ui->vuMeterLowerLimit1->setValue(volumeL.minValue);
    emit _ui->vuMeterLowerLimit1->editingFinished();
    _ui->vuMeterUpperLimit1->setValue(volumeL.maxValue);
    emit _ui->vuMeterUpperLimit1->editingFinished();

    LimitDef &volumeR = alarmCfg->getLimitDef(VolumeR);
    _ui->vuMeterMode2->setCurrentIndex(volumeR.getLimitMode());
    _ui->vuMeterLowerLimit2->setValue(volumeR.minValue);
    emit _ui->vuMeterLowerLimit2->editingFinished();
    _ui->vuMeterUpperLimit2->setValue(volumeR.maxValue);
    emit _ui->vuMeterUpperLimit2->editingFinished();
}

void DevSetting_Amp4_4::saveAlarmSetting()
{
   AlarmConfig *alarmCfg = _devAmp->getAlarmCfg();
   alarmCfg->saveAlarmSetting();
}

//void DevSetting_Amp4_4::syncVolumeChanged()
//{
//    _syncing = true;

//    _ui->delay1Slider->setValue(_devAmp->getConfigData().ch1_ControlValue.delay / 3);
//    _ui->delay1Value->setText(QString::number(_devAmp->getConfigData().ch1_ControlValue.delay / 3));
//    _ui->gain1Slider->setValue(_devAmp->getConfigData().ch1_ControlValue.volume);
//    _ui->gain1Value->setText(QString::number(_devAmp->getConfigData().ch1_ControlValue.volume));

//    _ui->delay2Slider->setValue(_devAmp->getConfigData().ch2_ControlValue.delay / 3);
//    _ui->delay2Value->setText(QString::number(_devAmp->getConfigData().ch2_ControlValue.delay / 3));
//    _ui->gain2Slider->setValue(_devAmp->getConfigData().ch2_ControlValue.volume);
//    _ui->gain2Value->setText(QString::number(_devAmp->getConfigData().ch2_ControlValue.volume));

//    _syncing = false;
//}

//void DevSetting_Amp4_4::syncMuteChanged()
//{
//    _syncing = true;

//    _ui->gain1M->setChecked(_devAmp->getConfigData().ch1_ControlValue.mute);
//    _ui->gain2M->setChecked(_devAmp->getConfigData().ch2_ControlValue.mute);

//    _syncing = false;
//}

//void DevSetting_Amp4_4::syncPhaseChanged()
//{
//    _syncing = true;

//    _ui->gain1P->setChecked(_devAmp->getConfigData().ch1_ControlValue.phase);
//    _ui->gain2P->setChecked(_devAmp->getConfigData().ch2_ControlValue.phase);

//    _syncing = false;
//}
