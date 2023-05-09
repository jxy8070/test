#include "devsetting_amp_0_2_line_fangtu.h"
#include "ui_devsetting_amp_0_2_line_fangtu.h"

#include "stable.h"
#include "device/device_amp_0_2_line.h"
#include "deviceItem/deviceitem_amp_0_2_line.h"
#include "../trn/eqview_allband.h"
#include "customWidget/eqviewnormal.h"
#include "../spk/spk_alarmrecord.h"
#include "../amp/amp_filter1.h"
#include "../spk/spk_gainset.h"
#include "../trn/trn_dynwidget.h"
#include "../alarmconfig.h"
#include "../amp/amp_0_2_line_mixer.h"
#include "../amp/amp_0_2_line_xover.h"
#include "monitoring_aids/monitorblackstage.h"

#include "customWidget/overwriteitem.h"

// 构造函数
DevSetting_Amp_0_2_Line_FangTu::DevSetting_Amp_0_2_Line_FangTu(DeviceItem_Amp_0_2_Line *devItem,  QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_Amp_0_2_Line_FangTu)
{
    ui->setupUi(this);
    _devItem = devItem;
    Q_ASSERT(devItem->getAES70Device());
    _devAmp = qobject_cast<Device_Amp_0_2_Line*>(devItem->getAES70Device());

    setAttribute(Qt::WA_QuitOnClose, true);
    qDebug() << __FUNCTION__ << __LINE__;
//    setFixedSize(size());
    setFixedSize(102+108+106+36+10, 180+121+50+75);

    ui->AIn1_Delay->setRange(0, MAX_DELAY_TIME);
    ui->AIn2_Delay->setRange(0, MAX_DELAY_TIME);
    ui->label_delay_1->setText(tr("0ms"));
    ui->label_delay_2->setText(tr("0ms"));

    displayConfigValue();
    connectSigSlots();

    connect(_devAmp, SIGNAL(displaySig(struct MsgData_Amp_0_2_Line)), this, SLOT(DisplaySlot(struct MsgData_Amp_0_2_Line)));
    _devAmp->sendStartMonitor();
}

DevSetting_Amp_0_2_Line_FangTu::~DevSetting_Amp_0_2_Line_FangTu()
{
    _devAmp->sendStopMonitor();
    delete ui;
}

void DevSetting_Amp_0_2_Line_FangTu::connectSigSlots()
{
    /* 从设备初始化界面 */
    connect(_devAmp, SIGNAL(initConfigFromDevice()), this, SLOT(displayConfigValue()));

    /* Gain\Mute\Phase\Delay */
    connect(ui->NIn1_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->NIn2_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->NIn1_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->NIn2_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn1_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn2_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn1_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn2_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn1_Phase, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AIn2_Phase, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AIn1_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    connect(ui->AIn2_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));

    /* Eq */
    connect(ui->AIn1_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AIn2_Eq, SIGNAL(clicked()), this, SLOT(setEq()));

    /* Dyn */
    connect(ui->AIn1_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));
    connect(ui->AIn2_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));

    /* XOVER */
    connect(ui->AIn1_xover, SIGNAL(clicked()), this, SLOT(setXOver()));
    connect(ui->AIn2_xover, SIGNAL(clicked()), this, SLOT(setXOver()));

    /* Mixer */
    connect(ui->AOut_Mixter, SIGNAL(clicked()), this, SLOT(setAOutMixer()));
}

void DevSetting_Amp_0_2_Line_FangTu::displayConfigValue()
{
    ConfigData_Amp_0_2_Line& cfgData = _devAmp->getConfigData();

    ui->NIn1_Channel_Gain->setValue(cfgData.channel_gain[0].vol);
    ui->NIn2_Channel_Gain->setValue(cfgData.channel_gain[1].vol);

    ui->NIn1_Channel_Mute->setChecked(cfgData.channel_gain[0].mute);
    ui->NIn2_Channel_Mute->setChecked(cfgData.channel_gain[1].mute);

    ui->AIn1_Gain->setValue(cfgData.Aout_gain[0].vol);
    ui->AIn2_Gain->setValue(cfgData.Aout_gain[1].vol);

    ui->AIn1_Mute->setChecked(cfgData.Aout_gain[0].mute);
    ui->AIn2_Mute->setChecked(cfgData.Aout_gain[1].mute);

    ui->AIn1_Phase->setChecked(cfgData.Aout_gain[0].phase);
    ui->AIn2_Phase->setChecked(cfgData.Aout_gain[1].phase);

    ui->AIn1_Delay->setValue(cfgData.Aout_delay[0].delay);
    ui->AIn2_Delay->setValue(cfgData.Aout_delay[1].delay);
    ui->label_delay_1->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[0].delay / 48.0, 'f', 3)));
    ui->label_delay_2->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[1].delay / 48.0, 'f', 3)));
    quint16 remain = MAX_DELAY_TIME
             - ui->AIn1_Delay->value()
             - ui->AIn2_Delay->value();//最大延时为260ms,调整为一个样值一调整
    ui->AIn1_Delay->setRange(0, ui->AIn1_Delay->value() + remain);
    ui->AIn2_Delay->setRange(0, ui->AIn2_Delay->value() + remain);
}

/* Gain\Mute\Phase\Delay */
void DevSetting_Amp_0_2_Line_FangTu::setGain(int value)
{
    quint8 type = 0, ch = 0;
    if (sender() == ui->NIn1_Channel_Gain)
    {
        type = 0; ch = 0;
    }
    else if (sender() == ui->NIn2_Channel_Gain)
    {
        type = 0; ch = 1;
    }
    else if (sender() == ui->AIn1_Gain)
    {
        type = 1; ch = 0;
    }
    else if (sender() == ui->AIn2_Gain)
    {
        type = 1; ch = 1;
    }

    _devAmp->sendVolume(type, ch, value);
}

void DevSetting_Amp_0_2_Line_FangTu::setMute(bool enable)
{
    quint8 type = 0, ch = 0;

    if (sender() == ui->NIn1_Channel_Mute)
    {
        type = 0; ch = 0;
    }
    else if (sender() == ui->NIn2_Channel_Mute)
    {
        type = 0; ch = 1;
    }
    else if (sender() == ui->AIn1_Mute)
    {
        type = 1; ch = 0;
    }
    else if (sender() == ui->AIn2_Mute)
    {
        type = 1; ch = 1;
    }

    _devAmp->sendMute(type, ch, enable);
}

void DevSetting_Amp_0_2_Line_FangTu::setPhase(bool enable)
{
    quint8 ch = 0;

    if (sender() == ui->AIn1_Phase)
    {
        ch = 0;
    }
    else if (sender() == ui->AIn2_Phase)
    {
        ch = 1;
    }

    _devAmp->sendPhase(ch, enable);
}

void DevSetting_Amp_0_2_Line_FangTu::setDelay(int value)
{
    quint8 ch = 0;
    quint16 remain = MAX_DELAY_TIME
            - ui->AIn1_Delay->value()
            - ui->AIn2_Delay->value();//最大延时为260ms
    ui->AIn1_Delay->setRange(0, ui->AIn1_Delay->value() + remain);
    ui->AIn2_Delay->setRange(0, ui->AIn2_Delay->value() + remain);

    if (sender() == ui->AIn1_Delay)
    {
        ch = 0;
        ui->label_delay_1->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
    else if (sender() == ui->AIn2_Delay)
    {
        ch = 1;
        ui->label_delay_2->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }

    _devAmp->sendDelay(ch, value);
}

/* Eq */
void DevSetting_Amp_0_2_Line_FangTu::setEq()
{
    int channel;

    if (sender() == ui->AIn1_Eq)
    {
        channel = 0;
    }
    else if (sender() == ui->AIn2_Eq)
    {
        channel = 1;
    }

    EqData4B eq4B = _devAmp->getConfigData().Aout_eq[channel];
    DevSettingDlg dlg/*(this)*/;
    EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
    eqView->showWidget(channel, QString(), eq4B.bypass, eq4B.eq);
    dlg.setFixedSize(eqView->size());
    connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
    dlg.exec();
}

/* 处理函数 */
void DevSetting_Amp_0_2_Line_FangTu::onEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    _devAmp->sendEq(ch, bypass, eqPara);
}

/* Dyn */
void DevSetting_Amp_0_2_Line_FangTu::setDyn()
{
    int channel;

    if (sender() == ui->AIn1_Dyn)
    {
        channel = 0;
    }
    else if (sender() == ui->AIn2_Dyn)
    {
        channel = 1;
    }

    DynData dynData = _devAmp->getConfigData().Aout_dyn[channel];
    DevSettingDlg dlg/*(this)*/;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
    dynWidget->setDisabled(_devAmp->isFactoryMode());
    dynWidget->showWidget(false, channel, QString(), dynData);
//    dlg.setStyleSheet(parentWidget()->styleSheet());//hrp：暂时添加
    dlg.setFixedSize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

/* 处理函数 */
void DevSetting_Amp_0_2_Line_FangTu::onDynChanged(quint8 ch, bool isIn ,DynData dynData)
{
    _devAmp->sendDyn(ch, dynData);
}

/* XOVER */
void DevSetting_Amp_0_2_Line_FangTu::setXOver()
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

    Amp_0_2_Line_xover xover(_devAmp, ch/*, this*/);
    xover.exec();
}

/* AOut Mixer */
void DevSetting_Amp_0_2_Line_FangTu::setAOutMixer()
{
    if (sender() == ui->AOut_Mixter)
    {
        Amp_0_2_Line_Mixer mixer(_devAmp/*, this*/);
        mixer.exec();
    }
}

void DevSetting_Amp_0_2_Line_FangTu::DisplaySlot(MsgData_Amp_0_2_Line b)
{
    ui->Meter_In_1->setLCurValue(b._monitor.meter_In[0]);
    ui->Meter_In_2->setLCurValue(b._monitor.meter_In[1]);
    ui->Meter_Out_1->setLCurValue(b._monitor.meter_Out[0]);
    ui->Meter_Out_2->setLCurValue(b._monitor.meter_Out[1]);
}

//// 另存场景为文件
//void DevSetting_Amp_0_2_Line_FangTu::on_saveAsBtn_clicked()
//{
//    QString fileName = QFileDialog::getSaveFileName(this, tr("save as..."), QDir::currentPath(), QString("file (*.amp)"));
//    if(!fileName.isEmpty()){
//        _devAmp->serialize(fileName);
//    }
//}

//// 加载场景文件
//void DevSetting_Amp_0_2_Line_FangTu::on_loadBtn_clicked()
//{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("file (*.amp)"));
//    if(!fileName.isEmpty())
//    {
//        _devAmp->deserialize(fileName);
//        displayConfigValue();
//        _devAmp->sendAllConfigPack(true);
//    }
//}

void DevSetting_Amp_0_2_Line_FangTu::on_save_clicked()
{
    _devAmp->sendSave();
}

