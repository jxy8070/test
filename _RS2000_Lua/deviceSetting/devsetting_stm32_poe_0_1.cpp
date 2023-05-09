#include "stable.h"
#include "devsetting_stm32_poe_0_1.h"
#include "ui_DevSetting_STM32_POE_0_1.h"
#include "device/device_stm32_poe_0_1.h"
#include "deviceItem/deviceitem_stm32_poe_0_1.h"
#include "trn/eqview_allband.h"
#include "customWidget/eqviewnormal.h"
#include "spk/spk_alarmrecord.h"
#include "spk/spk_amppf.h"
#include "spk/spk_gainset.h"
#include "amp/amp_router.h"
#include "deviceSetting/amp/amp_dynlimitwidget.h"
#include "alarmconfig.h"

// 构造函数
DevSetting_STM32_POE_0_1::DevSetting_STM32_POE_0_1(DeviceItem_STM32_POE_0_1 *devItem,  QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_STM32_POE_0_1)
{
    ui->setupUi(this);
    _devItem = devItem;
    Q_ASSERT(devItem->getAES70Device());
    _devAmp = qobject_cast<Device_STM32_POE_0_1*>(devItem->getAES70Device());

    displayConfigValue();
    connectSigSlots();
}

DevSetting_STM32_POE_0_1::~DevSetting_STM32_POE_0_1()
{
    delete ui;
}

void DevSetting_STM32_POE_0_1::connectSigSlots()
{
    /* 第一路音量选择 */
    connect(ui->gain1Slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(ui->gain1M, SIGNAL(clicked(bool)), this, SLOT(pushbuttonClicked(bool)));
    connect(ui->eq1, SIGNAL(clicked(bool)), this, SLOT(pushbuttonClicked(bool)));
    connect(ui->dyn1, SIGNAL(clicked(bool)), this, SLOT(pushbuttonClicked(bool)));
    connect(ui->dyn_bypass, SIGNAL(clicked(bool)), this, SLOT(pushbuttonClicked(bool)));
    connect(ui->saveBtn, SIGNAL(clicked(bool)), this, SLOT(pushbuttonClicked(bool)));

    /* 从设备初始化界面 */
    connect(_devAmp, SIGNAL(initConfigFromDevice()), this, SLOT(displayConfigValue()));
}

void DevSetting_STM32_POE_0_1::displayConfigValue()
{
    ConfigData_STM32_POE_0_1 cfg = _devAmp->getConfigData();

    ui->gain1M->setChecked(cfg.AD_gain.mute);
    ui->gain1Slider->setValue(cfg.AD_gain.vol);
    ui->label_vol->setText(tr("%1").arg(cfg.AD_gain.vol));
    ui->dyn_bypass->setChecked(cfg.AD_Dyn.bypass);
}

void DevSetting_STM32_POE_0_1::sliderValueChanged(int value)
{
    if (sender() == ui->gain1Slider)
    {
        ui->label_vol->setText(tr("%1").arg(value));
        _devAmp->sendVol(value);
    }
}

void DevSetting_STM32_POE_0_1::pushbuttonClicked(bool enable)
{
    if (sender() == ui->gain1M)
    {
        _devAmp->sendMute(enable);
    } else if (sender() == ui->eq1) {
        EqData4B& eq4B = _devAmp->getConfigData().AD_Eq;
        QDialog dlg;
        EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
        eqView->showWidget(0, QString(), eq4B.bypass, eq4B.eq);
        dlg.resize(eqView->size());
        connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
        dlg.exec();
    } else if (sender() == ui->dyn1) {
        DynData& dynData = _devAmp->getConfigData().AD_Dyn;
        QDialog dlg;
        amp_dynLimitWidget *dynWidget = new amp_dynLimitWidget(&dlg);
        dynWidget->showWidget(false, 0, QString(), dynData);
         dlg.resize(dynWidget->size());
        connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
        dlg.exec();
    } else if (sender() == ui->dyn_bypass) {
        DynData dynData = _devAmp->getConfigData().AD_Dyn;
        dynData.bypass = enable;
        _devAmp->sendDyn(dynData);
    } else if (sender() == ui->saveBtn) {
        _devAmp->sendSave();
    }
}


/******************************************************************************************
 *
 * EQ处理函数
 *
 *****************************************************************************************/
void DevSetting_STM32_POE_0_1::onEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    Q_UNUSED(bypass);
    _devAmp->sendEq(bypass, eqPara);
}

/******************************************************************************************
 *
 * Dyn处理函数
 *
 *****************************************************************************************/
void DevSetting_STM32_POE_0_1::onDynChanged(quint8 ch, bool isIn ,DynData dynData)
{
    Q_UNUSED(isIn)
    _devAmp->sendDyn(dynData);
}
