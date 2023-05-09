#include "stable.h"
#include "devsetting_amp_quad.h"
#include "ui_devsetting_amp_quad.h"
#include "device/device_amp_quad.h"
#include "deviceItem/deviceitem_amp_quad.h"
#include "trn/eqview_allband.h"
#include "customWidget/eqviewnormal.h"
#include "spk/spk_alarmrecord.h"
#include "amp/amp_filter1.h"
#include "spk/spk_gainset.h"
#include "trn/trn_dynwidget.h"
#include "alarmconfig.h"
#include "amp/amp_4_4_mixer.h"
#include "amp/amp_4_4_route.h"
#include "amp/amp_4_4_xover.h"

// 构造函数
DevSetting_Amp_Quad::DevSetting_Amp_Quad(DeviceItem_Amp_Quad *devItem,  QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_Amp_Quad)
{
    ui->setupUi(this);
    _devItem = devItem;
    Q_ASSERT(devItem->getAES70Device());
    _devAmp = qobject_cast<Device_Amp_Quad*>(devItem->getAES70Device());
    connect(_devAmp, SIGNAL(displaySig(struct MsgData_RT1052_Poe)), this, SLOT(DisplaySlot(struct MsgData_RT1052_Poe)));

    setAttribute(Qt::WA_QuitOnClose, true);

    setFixedSize(size());

    ui->AIn1_Delay->setRange(0, MAX_DELAY_TIME);
    ui->AIn2_Delay->setRange(0, MAX_DELAY_TIME);
    ui->AIn3_Delay->setRange(0, MAX_DELAY_TIME);
    ui->AIn4_Delay->setRange(0, MAX_DELAY_TIME);
    ui->label_delay_1->setText(tr("0ms"));
    ui->label_delay_2->setText(tr("0ms"));
    ui->label_delay_3->setText(tr("0ms"));
    ui->label_delay_4->setText(tr("0ms"));

    displayConfigValue();
    connectSigSlots();

    _devAmp->sendStartMonitor();
}

DevSetting_Amp_Quad::~DevSetting_Amp_Quad()
{
    _devAmp->sendStopMonitor();
    delete ui;
}

void DevSetting_Amp_Quad::connectSigSlots()
{
    /* 从设备初始化界面 */
    connect(_devAmp, SIGNAL(initConfigFromDevice()), this, SLOT(displayConfigValue()));

    /* Gain\Mute\Phase\Delay */
    connect(ui->AIn1_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn2_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn3_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn4_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->NIn1_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->NIn2_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->NIn3_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->NIn4_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn1_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn2_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn3_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn4_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->NIn1_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->NIn2_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->NIn3_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->NIn4_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn1_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn2_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn3_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn4_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn1_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn2_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn3_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn4_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn1_Phase, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AIn2_Phase, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AIn3_Phase, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AIn4_Phase, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AIn1_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    connect(ui->AIn2_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    connect(ui->AIn3_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    connect(ui->AIn4_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));

    /* Eq */
    connect(ui->AIn1_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AIn2_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AIn3_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AIn4_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AIn1_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AIn2_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AIn3_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AIn4_Eq, SIGNAL(clicked()), this, SLOT(setEq()));

    /* Dyn */
    connect(ui->AIn1_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));
    connect(ui->AIn2_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));
    connect(ui->AIn3_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));
    connect(ui->AIn4_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));

    /* XOVER */
    connect(ui->AIn1_xover, SIGNAL(clicked()), this, SLOT(setXOver()));
    connect(ui->AIn2_xover, SIGNAL(clicked()), this, SLOT(setXOver()));
    connect(ui->AIn3_xover, SIGNAL(clicked()), this, SLOT(setXOver()));
    connect(ui->AIn4_xover, SIGNAL(clicked()), this, SLOT(setXOver()));

    /* Mixer */
    connect(ui->AOut_Mixter, SIGNAL(clicked()), this, SLOT(setAOutMixer()));

    /* Route */
    connect(ui->NOut_Route, SIGNAL(clicked()), this, SLOT(setNOutRoute()));
}

void DevSetting_Amp_Quad::displayConfigValue()
{
    ConfigData_Amp4_4& cfgData = _devAmp->getConfigData();

    ui->AIn1_Channel_Gain->setValue(cfgData.channel_gain[0].vol);
    ui->AIn2_Channel_Gain->setValue(cfgData.channel_gain[1].vol);
    ui->AIn3_Channel_Gain->setValue(cfgData.channel_gain[2].vol);
    ui->AIn4_Channel_Gain->setValue(cfgData.channel_gain[3].vol);
    ui->NIn1_Channel_Gain->setValue(cfgData.channel_gain[4].vol);
    ui->NIn2_Channel_Gain->setValue(cfgData.channel_gain[5].vol);
    ui->NIn3_Channel_Gain->setValue(cfgData.channel_gain[6].vol);
    ui->NIn4_Channel_Gain->setValue(cfgData.channel_gain[7].vol);

    ui->AIn1_Channel_Mute->setChecked(cfgData.channel_gain[0].mute);
    ui->AIn2_Channel_Mute->setChecked(cfgData.channel_gain[1].mute);
    ui->AIn3_Channel_Mute->setChecked(cfgData.channel_gain[2].mute);
    ui->AIn4_Channel_Mute->setChecked(cfgData.channel_gain[3].mute);
    ui->NIn1_Channel_Mute->setChecked(cfgData.channel_gain[4].mute);
    ui->NIn2_Channel_Mute->setChecked(cfgData.channel_gain[5].mute);
    ui->NIn3_Channel_Mute->setChecked(cfgData.channel_gain[6].mute);
    ui->NIn4_Channel_Mute->setChecked(cfgData.channel_gain[7].mute);

    ui->AIn1_Gain->setValue(cfgData.Aout_gain[0].vol);
    ui->AIn2_Gain->setValue(cfgData.Aout_gain[1].vol);
    ui->AIn3_Gain->setValue(cfgData.Aout_gain[2].vol);
    ui->AIn4_Gain->setValue(cfgData.Aout_gain[3].vol);

    ui->AIn1_Mute->setChecked(cfgData.Aout_gain[0].mute);
    ui->AIn2_Mute->setChecked(cfgData.Aout_gain[1].mute);
    ui->AIn3_Mute->setChecked(cfgData.Aout_gain[2].mute);
    ui->AIn4_Mute->setChecked(cfgData.Aout_gain[3].mute);

    ui->AIn1_Phase->setChecked(cfgData.Aout_gain[0].phase);
    ui->AIn2_Phase->setChecked(cfgData.Aout_gain[1].phase);
    ui->AIn3_Phase->setChecked(cfgData.Aout_gain[2].phase);
    ui->AIn4_Phase->setChecked(cfgData.Aout_gain[3].phase);

    ui->AIn1_Delay->setValue(cfgData.Aout_delay[0].delay);
    ui->AIn2_Delay->setValue(cfgData.Aout_delay[1].delay);
    ui->AIn3_Delay->setValue(cfgData.Aout_delay[2].delay);
    ui->AIn4_Delay->setValue(cfgData.Aout_delay[3].delay);
    ui->label_delay_1->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[0].delay / 48.0, 'f', 3)));
    ui->label_delay_2->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[1].delay / 48.0, 'f', 3)));
    ui->label_delay_3->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[2].delay / 48.0, 'f', 3)));
    ui->label_delay_4->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[3].delay / 48.0, 'f', 3)));
    quint16 remain = MAX_DELAY_TIME
             - ui->AIn1_Delay->value()
             - ui->AIn2_Delay->value()
             - ui->AIn3_Delay->value()
             - ui->AIn4_Delay->value();//最大延时为260ms,调整为一个样值一调整
    ui->AIn1_Delay->setRange(0, ui->AIn1_Delay->value() + remain);
    ui->AIn2_Delay->setRange(0, ui->AIn2_Delay->value() + remain);
    ui->AIn3_Delay->setRange(0, ui->AIn3_Delay->value() + remain);
    ui->AIn4_Delay->setRange(0, ui->AIn4_Delay->value() + remain);
}

/* Gain\Mute\Phase\Delay */
void DevSetting_Amp_Quad::setGain(int value)
{
    if (sender() == ui->AIn1_Channel_Gain)
    {
        _devAmp->sendVolume(0, 0, value);
    }
    else if (sender() == ui->AIn2_Channel_Gain)
    {
        _devAmp->sendVolume(0, 1, value);
    }
    else if (sender() == ui->AIn3_Channel_Gain)
    {
        _devAmp->sendVolume(0, 2, value);
    }
    else if (sender() == ui->AIn4_Channel_Gain)
    {
        _devAmp->sendVolume(0, 3, value);
    }
    else if (sender() == ui->NIn1_Channel_Gain)
    {
        _devAmp->sendVolume(0, 4, value);
    }
    else if (sender() == ui->NIn2_Channel_Gain)
    {
        _devAmp->sendVolume(0, 5, value);
    }
    else if (sender() == ui->NIn3_Channel_Gain)
    {
        _devAmp->sendVolume(0, 6, value);
    }
    else if (sender() == ui->NIn4_Channel_Gain)
    {
        _devAmp->sendVolume(0, 7, value);
    }
    else if (sender() == ui->AIn1_Gain)
    {
        _devAmp->sendVolume(1, 0, value);
    }
    else if (sender() == ui->AIn2_Gain)
    {
        _devAmp->sendVolume(1, 1, value);
    }
    else if (sender() == ui->AIn3_Gain)
    {
        _devAmp->sendVolume(1, 2, value);
    }
    else if (sender() == ui->AIn4_Gain)
    {
        _devAmp->sendVolume(1, 3, value);
    }
}

void DevSetting_Amp_Quad::setMute(bool enable)
{
    if (sender() == ui->AIn1_Channel_Mute)
    {
        _devAmp->sendMute(0, 0, enable);
    }
    else if (sender() == ui->AIn2_Channel_Mute)
    {
        _devAmp->sendMute(0, 1, enable);
    }
    else if (sender() == ui->AIn3_Channel_Mute)
    {
        _devAmp->sendMute(0, 2, enable);
    }
    else if (sender() == ui->AIn4_Channel_Mute)
    {
        _devAmp->sendMute(0, 3, enable);
    }
    else if (sender() == ui->NIn1_Channel_Mute)
    {
        _devAmp->sendMute(0, 4, enable);
    }
    else if (sender() == ui->NIn2_Channel_Mute)
    {
        _devAmp->sendMute(0, 5, enable);
    }
    else if (sender() == ui->NIn3_Channel_Mute)
    {
        _devAmp->sendMute(0, 6, enable);
    }
    else if (sender() == ui->NIn4_Channel_Mute)
    {
        _devAmp->sendMute(0, 7, enable);
    }
    else if (sender() == ui->AIn1_Mute)
    {
        _devAmp->sendMute(1, 0, enable);
    }
    else if (sender() == ui->AIn2_Mute)
    {
        _devAmp->sendMute(1, 1, enable);
    }
    else if (sender() == ui->AIn3_Mute)
    {
        _devAmp->sendMute(1, 2, enable);
    }
    else if (sender() == ui->AIn4_Mute)
    {
        _devAmp->sendMute(1, 3, enable);
    }
}

void DevSetting_Amp_Quad::setPhase(bool enable)
{
    if (sender() == ui->AIn1_Phase)
    {
        _devAmp->sendPhase(0, enable);
    }
    else if (sender() == ui->AIn2_Phase)
    {
        _devAmp->sendPhase(1, enable);
    }
    else if (sender() == ui->AIn3_Phase)
    {
        _devAmp->sendPhase(2, enable);
    }
    else if (sender() == ui->AIn4_Phase)
    {
        _devAmp->sendPhase(3, enable);
    }
}

void DevSetting_Amp_Quad::setDelay(int value)
{
    quint16 remain = MAX_DELAY_TIME
            - ui->AIn1_Delay->value()
            - ui->AIn2_Delay->value()
            - ui->AIn3_Delay->value()
            - ui->AIn4_Delay->value();//最大延时为260ms
    ui->AIn1_Delay->setRange(0, ui->AIn1_Delay->value() + remain);
    ui->AIn2_Delay->setRange(0, ui->AIn2_Delay->value() + remain);
    ui->AIn3_Delay->setRange(0, ui->AIn3_Delay->value() + remain);
    ui->AIn4_Delay->setRange(0, ui->AIn4_Delay->value() + remain);
    if (sender() == ui->AIn1_Delay)
    {
        _devAmp->sendDelay(0, value);
        ui->label_delay_1->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
    else if (sender() == ui->AIn2_Delay)
    {
        _devAmp->sendDelay(1, value);
        ui->label_delay_2->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
    else if (sender() == ui->AIn3_Delay)
    {
        _devAmp->sendDelay(2, value);
        ui->label_delay_3->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
    else if (sender() == ui->AIn4_Delay)
    {
        _devAmp->sendDelay(3, value);
        ui->label_delay_4->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
}

/* Eq */
void DevSetting_Amp_Quad::setEq()
{
    int channel;
    EqData4B eq4B;
    uint8_t type = 0;

    if (sender() == ui->AIn1_Channel_Eq)
    {
        channel = 0;
        eq4B = _devAmp->getConfigData().channel_eq[channel];
        type = 0;
    }
    else if (sender() == ui->AIn2_Channel_Eq)
    {
        channel = 1;
        eq4B = _devAmp->getConfigData().channel_eq[channel];
        type = 0;
    }
    else if (sender() == ui->AIn3_Channel_Eq)
    {
        channel = 2;
        eq4B = _devAmp->getConfigData().channel_eq[channel];
        type = 0;
    }
    else if (sender() == ui->AIn4_Channel_Eq)
    {
        channel = 3;
        eq4B = _devAmp->getConfigData().channel_eq[channel];
        type = 0;
    }
    else if (sender() == ui->AIn1_Eq)
    {
        channel = 0;
        eq4B = _devAmp->getConfigData().Aout_eq[channel];
        type = 1;
    }
    else if (sender() == ui->AIn2_Eq)
    {
        channel = 1;
        eq4B = _devAmp->getConfigData().Aout_eq[channel];
        type = 1;
    }
    else if (sender() == ui->AIn3_Eq)
    {
        channel = 2;
        eq4B = _devAmp->getConfigData().Aout_eq[channel];
        type = 1;
    }
    else if (sender() == ui->AIn4_Eq)
    {
        channel = 3;
        eq4B = _devAmp->getConfigData().Aout_eq[channel];
        type = 1;
    }

    DevSettingDlg dlg;
    EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
    eqView->showWidget(channel, QString(), eq4B.bypass, eq4B.eq);
    dlg.resize(eqView->size());
    if (type == 0)
    {
        connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onChannelEqChanged(quint8, bool, EqData*)));
    } else if (type == 1) {
        connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
    }
    dlg.exec();
}

/* 处理函数 */
void DevSetting_Amp_Quad::onChannelEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    _devAmp->sendEq(0, ch, bypass, eqPara);
}

void DevSetting_Amp_Quad::onEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    _devAmp->sendEq(1, ch, bypass, eqPara);
}

/* Dyn */
void DevSetting_Amp_Quad::setDyn()
{
    int channel;
    DynData dynData;
    if (sender() == ui->AIn1_Dyn)
    {
        channel = 0;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }
    else if (sender() == ui->AIn2_Dyn)
    {
        channel = 1;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }
    else if (sender() == ui->AIn3_Dyn)
    {
        channel = 2;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }
    else if (sender() == ui->AIn4_Dyn)
    {
        channel = 3;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }

    DevSettingDlg dlg;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
    dynWidget->setDisabled(_devAmp->isFactoryMode());
    dynWidget->showWidget(false, channel, QString(), dynData);
    dlg.resize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

/* 处理函数 */
void DevSetting_Amp_Quad::onDynChanged(quint8 ch, bool isIn ,DynData dynData)
{
    _devAmp->sendDyn(ch, dynData);
}

/* XOVER */
void DevSetting_Amp_Quad::setXOver()
{
    quint8 ch = 0;
    if (sender() == ui->AIn1_xover)
    {
        ch = 0;
    }
    else if (sender() == ui->AIn2_xover)
    {
        ch = 1;
    }
    else if (sender() == ui->AIn3_xover)
    {
        ch = 2;
    }
    else if (sender() == ui->AIn4_xover)
    {
        ch = 3;
    }

    Amp_4_4_xover xover(_devAmp, ch/*, this*/);
    xover.exec();
}

/* AOut Mixer */
void DevSetting_Amp_Quad::setAOutMixer()
{
    if (sender() == ui->AOut_Mixter)
    {
        Amp_4_4_Mixer mixer(_devAmp/*, this*/);
        mixer.exec();
    }
}

/* NOut Route */
void DevSetting_Amp_Quad::setNOutRoute()
{
    if (sender() == ui->NOut_Route)
    {
        Amp_4_4_Route route(_devAmp/*, this*/);
        route.exec();
    }
}

void DevSetting_Amp_Quad::DisplaySlot(struct MsgData_RT1052_Poe b)
{
    ui->Meter_In_1->setLCurValue(b._monitor._line_4_4.meter_In[0]);
    ui->Meter_In_2->setLCurValue(b._monitor._line_4_4.meter_In[1]);
    ui->Meter_In_3->setLCurValue(b._monitor._line_4_4.meter_In[2]);
    ui->Meter_In_4->setLCurValue(b._monitor._line_4_4.meter_In[3]);
    ui->Meter_In_5->setLCurValue(b._monitor._line_4_4.meter_In[4]);
    ui->Meter_In_6->setLCurValue(b._monitor._line_4_4.meter_In[5]);
    ui->Meter_In_7->setLCurValue(b._monitor._line_4_4.meter_In[6]);
    ui->Meter_In_8->setLCurValue(b._monitor._line_4_4.meter_In[7]);

    ui->Meter_Out_1->setLCurValue(b._monitor._line_4_4.meter_Out[0]);
    ui->Meter_Out_2->setLCurValue(b._monitor._line_4_4.meter_Out[1]);
    ui->Meter_Out_3->setLCurValue(b._monitor._line_4_4.meter_Out[2]);
    ui->Meter_Out_4->setLCurValue(b._monitor._line_4_4.meter_Out[3]);
    ui->Meter_Out_5->setLCurValue(b._monitor._line_4_4.meter_Out[4]);
    ui->Meter_Out_6->setLCurValue(b._monitor._line_4_4.meter_Out[5]);
    ui->Meter_Out_7->setLCurValue(b._monitor._line_4_4.meter_Out[6]);
    ui->Meter_Out_8->setLCurValue(b._monitor._line_4_4.meter_Out[7]);

//    qDebug() << b._display.meter[0][0]
//            << b._display.meter[1][0];
}

//// 另存场景为文件
//void DevSetting_Amp_Quad::on_saveAsBtn_clicked()
//{
//    QString fileName = QFileDialog::getSaveFileName(this, tr("save as..."), QDir::currentPath(), QString("file (*.amp)"));
//    if(!fileName.isEmpty()){
//        _devAmp->serialize(fileName);
//    }
//}

//// 加载场景文件
//void DevSetting_Amp_Quad::on_loadBtn_clicked()
//{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("file (*.amp)"));
//    if(!fileName.isEmpty())
//    {
//        _devAmp->deserialize(fileName);
//        displayConfigValue();
//        _devAmp->sendAllConfigPack(true);
//    }
//}

/******************************************************************************************
 *
 * 路由、混音
 *
 *****************************************************************************************/
//void DevSetting_Amp_Quad::on_router_clicked()
//{
//    Amp4_4Router router(_devAmp, this);
//    router.exec();
//}

/******************************************************************************************
 *
 * Filter
 *
 *****************************************************************************************/
///* 第一路 */
//void DevSetting_Amp_Quad::on_filter1HPF_clicked()
//{
//    _ui->filter1HPFSet->setEnabled(true);
//    _ui->filter1LPFSet->setEnabled(false);

//    onPfDataChanged(Device_Amp_Quad::AMP_CH1, false, true);
//}

//void DevSetting_Amp_Quad::on_filter1Bypass_clicked()
//{
//    _ui->filter1HPFSet->setEnabled(false);
//    _ui->filter1LPFSet->setEnabled(false);

//    onPfDataChanged(Device_Amp_Quad::AMP_CH1, true, true);
//}

//void DevSetting_Amp_Quad::on_filter1LPF_clicked()
//{
//    _ui->filter1HPFSet->setEnabled(false);
//    _ui->filter1LPFSet->setEnabled(true);

//    onPfDataChanged(Device_Amp_Quad::AMP_CH1, true, false);
//}

//void DevSetting_Amp_Quad::on_filter1HPFSet_clicked()
//{
//    AmpFilter1 pf(Device_Amp_Quad::AMP_CH1, &_devAmp->getConfigData().CH[Device_Amp_Quad::AMP_CH1].divider.hf);
//    pf.setWindowTitle(tr("Channel1 HF"));
//    connect(&pf, SIGNAL(pfDataChanged(quint8)), this, SLOT(onPfDataChanged(quint8)));
//    pf.exec();
//}

//void DevSetting_Amp_Quad::on_filter1LPFSet_clicked()
//{
//    AmpFilter1 pf(Device_Amp_Quad::AMP_CH1, &_devAmp->getConfigData().CH[Device_Amp_Quad::AMP_CH1].divider.lf);
//    pf.setWindowTitle(tr("Channel1 LF"));
//    connect(&pf, SIGNAL(pfDataChanged(quint8)), this, SLOT(onPfDataChanged(quint8)));
//    pf.exec();
//}

/* 处理函数 */
void DevSetting_Amp_Quad::onPfDataChanged(quint8 channel)
{
//    Divider *divider = NULL;
//    divider = &_devAmp->getConfigData().CH[channel].divider;
////    if(channel == 1) divider = &_devAmp->getConfigData().ch1_divider;
////    else divider = &_devAmp->getConfigData().ch2_divider;
//    _devAmp->sendDividerPack(channel, divider->hf, divider->lf);
}

void DevSetting_Amp_Quad::onPfDataChanged(quint8 ch, bool hfb, bool lfb)
{
//    /* 数据同步时不再次发送 */
//    if (_syncing)
//    {
//        return;
//    }

//    Divider *divider = NULL;
////    if(ch == 1) divider = &_devAmp->getConfigData().ch1_divider;
////    else divider = &_devAmp->getConfigData().ch2_divider;
//    divider = &_devAmp->getConfigData().CH[ch].divider;

//    divider->hf.bypass = hfb;
//    divider->lf.bypass = lfb;
//    _devAmp->sendDividerPack(ch, divider->hf, divider->lf);
}

void DevSetting_Amp_Quad::on_save_clicked()
{
    _devAmp->sendSave();
}
