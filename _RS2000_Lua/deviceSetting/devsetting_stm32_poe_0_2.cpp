#include "stable.h"
#include "devsetting_stm32_poe_0_2.h"
#include "ui_devsetting_stm32_poe_0_2.h"
#include "device/device_stm32_poe_0_2.h"
#include "deviceItem/deviceitem_stm32_poe_0_2.h"
#include "trn/eqview_allband.h"
#include "customWidget/eqviewnormal.h"
#include "spk/spk_alarmrecord.h"
#include "spk/spk_amppf.h"
#include "spk/spk_gainset.h"
#include "amp/amp_router.h"
//#include "trn/trn_dynwidget.h"
#include "deviceSetting/amp/amp_dynlimitwidget.h"
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
DevSetting_Amp::DevSetting_Amp(DeviceItem_Amp *devItem,  QWidget *parent) :
    DevSettingDlg(parent),
    _ui(new Ui::DevSetting_Amp)
{
    _ui->setupUi(this);
    _devItem = devItem;
    Q_ASSERT(devItem->getAES70Device());
    _devAmp = qobject_cast<Device_Amp*>(devItem->getAES70Device());

    displayConfigValue();
    connectSigSlots();

    _ui->groupBox_8->setHidden(true);
    _ui->groupBox_9->setHidden(true);
    _ui->groupBox_10->setHidden(true);
    _ui->groupBox_15->setHidden(true);
    _ui->gain1P->setHidden(true);
    _ui->gain2P->setHidden(true);
    _ui->dyn2->setHidden(true);
    _ui->router->setHidden(true);
}

DevSetting_Amp::~DevSetting_Amp()
{
//    saveAlarmSetting();

    delete _ui;
}

void DevSetting_Amp::connectSigSlots()
{
    /* 第一路音量选择 */
    connect(_ui->gain1Slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->gain1M, SIGNAL(clicked(bool)), this, SLOT(pushbuttonClicked(bool)));
    /* 第二路延时选择 */
    connect(_ui->gain2Slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->gain2M, SIGNAL(clicked(bool)), this, SLOT(pushbuttonClicked(bool)));

    connect(_ui->eq_bypass, SIGNAL(clicked(bool)), SLOT(pushbuttonClicked(bool)));
    connect(_ui->eq1, SIGNAL(clicked(bool)), this, SLOT(pushbuttonClicked(bool)));
    connect(_ui->eq2, SIGNAL(clicked(bool)), this, SLOT(pushbuttonClicked(bool)));
    connect(_ui->dyn_bypass, SIGNAL(clicked(bool)), this, SLOT(pushbuttonClicked(bool)));
    connect(_ui->dyn1, SIGNAL(clicked(bool)), this, SLOT(pushbuttonClicked(bool)));
    connect(_ui->saveBtn, SIGNAL(clicked(bool)), this, SLOT(pushbuttonClicked(bool)));

    /* 从设备初始化界面 */
    connect(_devAmp, SIGNAL(initConfigFromDevice()), this, SLOT(displayConfigValue()));
}

void DevSetting_Amp::displayConfigValue()
{
    ConfigData_STM32_POE_0_2 cfg = _devAmp->getConfigData();

    _ui->gain1M->setChecked(cfg.AD_gain[0].mute);
    _ui->gain2M->setChecked(cfg.AD_gain[1].mute);
    _ui->gain1Slider->setValue(cfg.AD_gain[0].vol);
    _ui->gain2Slider->setValue(cfg.AD_gain[1].vol);
    _ui->gain1Value->setText(tr("%1").arg(cfg.AD_gain[0].vol));
    _ui->gain2Value->setText(tr("%1").arg(cfg.AD_gain[1].vol));
    _ui->dyn_bypass->setChecked(cfg.AD_Dyn.bypass);
    _ui->eq_bypass->setChecked(cfg.AD_Eq[0].bypass);
}

void DevSetting_Amp::sliderValueChanged(int value)
{
    if (sender() == _ui->gain1Slider)
    {
        _ui->gain1Value->setText(tr("%1").arg(value));
        _devAmp->sendVolume(0, value);
    }
    else if (sender() == _ui->gain2Slider)
    {
        _ui->gain2Value->setText(tr("%1").arg(value));
        _devAmp->sendVolume(1, value);
    }
}

void DevSetting_Amp::pushbuttonClicked(bool enable)
{
    if (sender() == _ui->gain1M)
    {
        _devAmp->sendMute(0, enable);
    }
    else if (sender() == _ui->gain2M)
    {
        _devAmp->sendMute(1, enable);
    }
    else if (sender() == _ui->eq1)
    {
        EqData4B& eq4B = _devAmp->getConfigData().AD_Eq[0];
        QDialog dlg;
        EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
        eqView->showWidget(0, QString(), _devAmp->getConfigData().AD_Eq[0].bypass, eq4B.eq);
        dlg.resize(eqView->size());
        connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
        dlg.exec();
    }
    else if (sender() == _ui->eq2)
    {
        EqData4B& eq4B = _devAmp->getConfigData().AD_Eq[1];
        QDialog dlg;
        EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
        eqView->showWidget(1, QString(), _devAmp->getConfigData().AD_Eq[0].bypass, eq4B.eq);
        dlg.resize(eqView->size());
        connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
        dlg.exec();
    }
    else if (sender() == _ui->eq_bypass)
    {
        EqData4B& eq4B = _devAmp->getConfigData().AD_Eq[0];
        eq4B.bypass = enable;
        _devAmp->sendEqPack(0, enable, eq4B.eq);
    }
    else if (sender() == _ui->dyn1)
    {
        DynData& dynData = _devAmp->getConfigData().AD_Dyn;
        QDialog dlg;
        amp_dynLimitWidget *dynWidget = new amp_dynLimitWidget(&dlg);
        dynWidget->showWidget(false, 0, QString(), dynData);
        dlg.resize(dynWidget->size());
        connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
        dlg.exec();
    }
    else if (sender() == _ui->dyn_bypass)
    {
        DynData dynData = _devAmp->getConfigData().AD_Dyn;
        dynData.bypass = enable;
        _devAmp->sendDynPack(0, dynData);
    }
    else if (sender() == _ui->saveBtn)
    {
        _devAmp->sendSave2DevicePack();
    }
}

void DevSetting_Amp::onEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    Q_UNUSED(bypass);
    _devAmp->sendEqPack(ch, _ui->eq_bypass->isChecked(), eqPara);
}
void DevSetting_Amp::onDynChanged(quint8 ch, bool isIn ,DynData dynData)
{
    Q_UNUSED(isIn)
    _devAmp->sendDynPack(0, dynData);
}
