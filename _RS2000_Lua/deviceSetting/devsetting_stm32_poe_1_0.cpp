#include "stable.h"
#include "devsetting_stm32_poe_1_0.h"
#include "ui_devsetting_stm32_poe_1_0.h"
#include "device/device_stm32_poe_1_0.h"
#include "deviceItem/deviceitem_stm32_poe_1_0.h"
#include "trn/eqview_allband.h"
#include "customWidget/eqviewnormal.h"
#include "spk/spk_alarmrecord.h"
#include "spk/spk_amppf.h"
#include "spk/spk_gainset.h"
#include "amp/amp_router.h"
#include "trn/trn_dynwidget.h"
#include "alarmconfig.h"
#include "base/devsettingbase_xover.h"

DevSetting_Mic::DevSetting_Mic(DeviceItem_Mic *devItem,  QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_Mic)
{
    ui->setupUi(this);
    _devItem = devItem;
    Q_ASSERT(devItem->getAES70Device());
    _devAmp = qobject_cast<Device_Mic*>(devItem->getAES70Device());

    connect(_devAmp, SIGNAL(displaySig(struct MsgData_STM32_POE)), this, SLOT(DisplaySlot(struct MsgData_STM32_POE)));

    setAttribute(Qt::WA_QuitOnClose, true);

    setFixedSize(size());

    displayConfigValue();
    connectSigSlots();

    _devAmp->sendStartMonitor();
}

DevSetting_Mic::~DevSetting_Mic()
{
    _devAmp->sendStopMonitor();
    delete ui;
}

void DevSetting_Mic::connectSigSlots()
{
    /* 从设备初始化界面 */
    connect(_devAmp, SIGNAL(initConfigFromDevice()), this, SLOT(displayConfigValue()));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pbClicked()));
//    connect(ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(ui->comboBox_Gain, SIGNAL(currentIndexChanged(int)), this, SLOT(comboxValueChanged(int)));
    connect(ui->Phantom, SIGNAL(clicked(bool)), this, SLOT(phantomClicked(bool)));
    connect(ui->eq, SIGNAL(clicked()), this, SLOT(pbClicked()));
    connect(ui->dyn, SIGNAL(clicked()), this, SLOT(pbClicked()));
    connect(ui->xover, SIGNAL(clicked()), this, SLOT(pbClicked()));
}

void DevSetting_Mic::displayConfigValue()
{
    ConfigData_STM32_POE_1_0 tmp = _devAmp->getConfigData();
    for(int i = 0; i < 11; i++)
    {
        ui->comboBox_Gain->addItem(QString("%1 dB").arg(i*3));
    }
//    ui->verticalSlider->setValue(tmp.Mic_Gain);
    ui->comboBox_Gain->setCurrentIndex(tmp.Mic_Gain);
    ui->label->setText(tr("%1 dB").arg(tmp.Mic_Gain));
    ui->Phantom->setChecked(tmp.phantom);
}

void DevSetting_Mic::pbClicked()
{
    if (sender() == ui->eq)
    {
        EqData4B& eq4B = _devAmp->getConfigData().Eq;
        QDialog dlg;
        EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
        eqView->showWidget(0, QString(), eq4B.bypass, eq4B.eq);
        dlg.resize(eqView->size());
        connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
        dlg.exec();
    }
    else if (sender() == ui->dyn)
    {
        DynData& dynData = _devAmp->getConfigData().Dyn;
        QDialog dlg;
        Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
        dynWidget->showWidget(false, 0, QString(), dynData);
        dlg.resize(dynWidget->size());
        connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
        dlg.exec();
    }
    else if (sender() == ui->xover)
    {
        DevSettingBase_Xover xWidget(0, DevSettingBase_Xover::DevSettingBase_Xover_HP, _devAmp->getConfigData().x);
        connect(&xWidget, SIGNAL(valueChanged(uint8_t, struct ConfigData_Xover)), this, SLOT(onXoverChanged(uint8_t,ConfigData_Xover)));
        xWidget.exec();
    }
    else if (sender() == ui->pushButton)
    {
        //    _devAmp->sendSave2DevicePack();
    }
}

void DevSetting_Mic::phantomClicked(bool enable)
{
    _devAmp->sendPhantom(enable);
}

//void DevSetting_Mic::sliderValueChanged(int value)
//{
//    if (sender() == ui->verticalSlider)
//    {
//        _devAmp->sendMicGain(value);
//        ui->label->setText(tr("%1 dB").arg(value));
//    }
//}

void DevSetting_Mic::comboxValueChanged(int value)
{
    if(sender() == ui->comboBox_Gain)
    {
//        int a = QString(value).toInt();
//        qDebug()<<__FUNCTION__<<__LINE__<<"================="<<a;
        _devAmp->sendMicGain(value);
        ui->label->setText(tr("%1 dB").arg(value*3));
    }
}

/******************************************************************************************
 *
 * EQ处理函数
 *
 *****************************************************************************************/
void DevSetting_Mic::onEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    Q_UNUSED(bypass);
    _devAmp->sendEq(bypass, eqPara);
}

/******************************************************************************************
 *
 * Dyn处理函数
 *
 *****************************************************************************************/
void DevSetting_Mic::onDynChanged(quint8 ch, bool isIn ,DynData dynData)
{
    Q_UNUSED(isIn)
    _devAmp->sendDyn(dynData);
}

void DevSetting_Mic::onXoverChanged(uint8_t ch, ConfigData_Xover xData)
{
    _devAmp->sendXover(xData);
}

void DevSetting_Mic::DisplaySlot(MsgData_STM32_POE b)
{
    ui->Meter_AIn_1->setLCurValue(b._display.A_In);
    ui->Meter_NOut_1->setLCurValue(b._display.N_Out);

    if (b._display.phantom != _devAmp->getConfigData().phantom)
    {
        _devAmp->getConfigData().phantom = b._display.phantom;
        ui->Phantom->setChecked(b._display.phantom);
    }
}

