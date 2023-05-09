#include "stable.h"
#include "devsetting_rt1052_poe_AP60_Bluetooth.h"
#include "ui_devsetting_rt1052_poe_AP60_Bluetooth.h"
#include "device/device_rt1052_poe_AP60_Bluetooth.h"
#include "deviceItem/deviceitem_rt1052_poe_AP60_Bluetooth.h"
#include "trn/eqview_allband.h"
#include "customWidget/eqviewnormal.h"
#include "spk/spk_alarmrecord.h"
#include "amp/amp_filter1.h"
#include "spk/spk_gainset.h"
#include "trn/trn_dynwidget.h"
#include "alarmconfig.h"
#include "base/devsettingbase_xover.h"
#include "amp/amp_mic_quad_mixer.h"
#include "base/base_2_2_mixer.h"

// 构造函数
DevSetting_RT1052_Poe_AP60_Bluetooth::DevSetting_RT1052_Poe_AP60_Bluetooth(DeviceItem_RT1052_Poe_AP60_Bluetooth *devItem,  QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_RT1052_Poe_AP60_Bluetooth)
{
    ui->setupUi(this);
    _devItem = devItem;
    Q_ASSERT(devItem->getAES70Device());
    _devAmp = qobject_cast<Device_RT1052_Poe_AP60_Bluetooth*>(devItem->getAES70Device());
    connect(_devAmp, SIGNAL(displaySig(struct MsgData_RT1052_Poe)), this, SLOT(DisplaySlot(struct MsgData_RT1052_Poe)));

    setAttribute(Qt::WA_QuitOnClose, true);

    setFixedSize(size());//设置最小值和最大值
    ui->AIn1_Delay->setRange(0, MAX_DELAY_TIME);
    ui->label_delay_1->setText(tr("0ms"));

    displayConfigValue();
    connectSigSlots();

    _devAmp->sendStartMonitor();
}

DevSetting_RT1052_Poe_AP60_Bluetooth::~DevSetting_RT1052_Poe_AP60_Bluetooth()
{
    _devAmp->sendStopMonitor();
    delete ui;
}

void DevSetting_RT1052_Poe_AP60_Bluetooth::connectSigSlots()
{
    /* 从设备初始化界面 */
    connect(_devAmp, SIGNAL(initConfigFromDevice()), this, SLOT(displayConfigValue()));

    /* Gain\Mute\Phase\Delay */
    connect(ui->AIn1_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->NIn1_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
//    connect(ui->NIn2_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn1_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->NIn1_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
//    connect(ui->NIn2_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn1_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
//    connect(ui->AIn2_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
//    connect(ui->AIn3_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
//    connect(ui->AIn4_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn1_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
//    connect(ui->AIn2_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
//    connect(ui->AIn3_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
//    connect(ui->AIn4_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn1_Phase, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
//    connect(ui->AIn2_Phase, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
//    connect(ui->AIn3_Phase, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
//    connect(ui->AIn4_Phase, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AIn1_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
//    connect(ui->AIn2_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
//    connect(ui->AIn3_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
//    connect(ui->AIn4_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));

    /* Eq */
    connect(ui->AIn1_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->NIn1_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
//    connect(ui->NIn2_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AIn1_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
//    connect(ui->AIn2_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
//    connect(ui->AIn3_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
//    connect(ui->AIn4_Eq, SIGNAL(clicked()), this, SLOT(setEq()));

    /* Dyn */
    connect(ui->AIn1_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));
//    connect(ui->AIn2_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));
//    connect(ui->AIn3_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));
//    connect(ui->AIn4_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));

    /* XOVER */
    connect(ui->AIn1_xover, SIGNAL(clicked()), this, SLOT(setXOver()));
//    connect(ui->AIn3_xover, SIGNAL(clicked()), this, SLOT(setXOver()));
//    connect(ui->AIn4_xover, SIGNAL(clicked()), this, SLOT(setXOver()));

    connect(ui->bt_peidui, SIGNAL(clicked(bool)), this, SLOT(bt_function_clicked(bool)));
    connect(ui->bt_duankai, SIGNAL(clicked(bool)), this, SLOT(bt_function_clicked(bool)));
    connect(ui->bt_chonglian, SIGNAL(clicked(bool)), this, SLOT(bt_function_clicked(bool)));
    connect(ui->bt_shangyishou, SIGNAL(clicked(bool)), this, SLOT(bt_function_clicked(bool)));
    connect(ui->bt_play, SIGNAL(clicked(bool)), this, SLOT(bt_function_clicked(bool)));
    connect(ui->bt_xiayishou, SIGNAL(clicked(bool)), this, SLOT(bt_function_clicked(bool)));
    connect(ui->bt_yinliangjia, SIGNAL(clicked(bool)), this, SLOT(bt_function_clicked(bool)));
    connect(ui->bt_yinliangjian, SIGNAL(clicked(bool)), this, SLOT(bt_function_clicked(bool)));
}

void DevSetting_RT1052_Poe_AP60_Bluetooth::displayConfigValue()
{
    ConfigData_RT1052_Poe_AP60_Bluetooth& cfgData = _devAmp->getConfigData();

    ui->AIn1_Channel_Gain->setValue(cfgData.channel_gain[0].vol);
    ui->NIn1_Channel_Gain->setValue(cfgData.channel_gain[1].vol);

//    ui->NIn2_Channel_Gain->setValue(cfgData.channel_gain[3].vol);

    ui->AIn1_Channel_Mute->setChecked(cfgData.channel_gain[0].mute);
    ui->NIn1_Channel_Mute->setChecked(cfgData.channel_gain[1].mute);

//    ui->NIn2_Channel_Mute->setChecked(cfgData.channel_gain[3].mute);

    ui->AIn1_Gain->setValue(cfgData.Aout_gain[0].vol);
//    ui->AIn2_Gain->setValue(cfgData.Aout_gain[1].vol);
//    ui->AIn3_Gain->setValue(cfgData.Aout_gain[2].vol);
//    ui->AIn4_Gain->setValue(cfgData.Aout_gain[3].vol);

    ui->AIn1_Mute->setChecked(cfgData.Aout_gain[0].mute);
//    ui->AIn2_Mute->setChecked(cfgData.Aout_gain[1].mute);
//    ui->AIn3_Mute->setChecked(cfgData.Aout_gain[2].mute);
//    ui->AIn4_Mute->setChecked(cfgData.Aout_gain[3].mute);

    ui->AIn1_Phase->setChecked(cfgData.Aout_gain[0].phase);
//    ui->AIn2_Phase->setChecked(cfgData.Aout_gain[1].phase);
//    ui->AIn3_Phase->setChecked(cfgData.Aout_gain[2].phase);
//    ui->AIn4_Phase->setChecked(cfgData.Aout_gain[3].phase);

    ui->AIn1_Delay->setValue(cfgData.Aout_delay[0].delay);
//    ui->AIn2_Delay->setValue(cfgData.Aout_delay[1].delay);
//    ui->AIn3_Delay->setValue(cfgData.Aout_delay[2].delay);
//    ui->AIn4_Delay->setValue(cfgData.Aout_delay[3].delay);
    ui->label_delay_1->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[0].delay / 48.0, 'f', 3)));
    quint16 remain = MAX_DELAY_TIME
             - ui->AIn1_Delay->value();//最大延时为260ms,调整为一个样值一调整
    ui->AIn1_Delay->setRange(0, ui->AIn1_Delay->value() + remain);
}

/* Gain\Mute\Phase\Delay */
void DevSetting_RT1052_Poe_AP60_Bluetooth::setGain(int value)
{
    if (sender() == ui->AIn1_Channel_Gain)
    {
        _devAmp->sendVolume(0, 0, value);
    }
    else if (sender() == ui->NIn1_Channel_Gain)
    {
        _devAmp->sendVolume(0, 1, value);
    }
    else if (sender() == ui->AIn1_Gain)
    {
        _devAmp->sendVolume(1, 0, value);
    }
}

void DevSetting_RT1052_Poe_AP60_Bluetooth::setMute(bool enable)
{
    if (sender() == ui->AIn1_Channel_Mute)
    {
        _devAmp->sendMute(0, 0, enable);
    }
    else if (sender() == ui->NIn1_Channel_Mute)
    {
        _devAmp->sendMute(0, 1, enable);
    }
//    else if (sender() == ui->NIn2_Channel_Mute)
//    {
//        _devAmp->sendMute(0, 3, enable);
//    }
    else if (sender() == ui->AIn1_Mute)
    {
        _devAmp->sendMute(1, 0, enable);
    }
//    else if (sender() == ui->AIn2_Mute)
//    {
//        _devAmp->sendMute(1, 1, enable);
//    }
//    else if (sender() == ui->AIn3_Mute)
//    {
//        _devAmp->sendMute(1, 2, enable);
//    }
//    else if (sender() == ui->AIn4_Mute)
//    {
//        _devAmp->sendMute(1, 3, enable);
//    }
}

void DevSetting_RT1052_Poe_AP60_Bluetooth::setPhase(bool enable)
{
    if (sender() == ui->AIn1_Phase)
    {
        _devAmp->sendPhase(0, enable);
    }
//    else if (sender() == ui->AIn2_Phase)
//    {
//        _devAmp->sendPhase(1, enable);
//    }
//    else if (sender() == ui->AIn3_Phase)
//    {
//        _devAmp->sendPhase(2, enable);
//    }
//    else if (sender() == ui->AIn4_Phase)
//    {
//        _devAmp->sendPhase(3, enable);
//    }
}

void DevSetting_RT1052_Poe_AP60_Bluetooth::setDelay(int value)
{
    quint16 remain = MAX_DELAY_TIME
            - ui->AIn1_Delay->value();//最大延时为260ms
    ui->AIn1_Delay->setRange(0, ui->AIn1_Delay->value() + remain);
//    ui->AIn2_Delay->setRange(0, ui->AIn2_Delay->value() + remain);
//    ui->AIn3_Delay->setRange(0, ui->AIn3_Delay->value() + remain);
//    ui->AIn4_Delay->setRange(0, ui->AIn4_Delay->value() + remain);
    if (sender() == ui->AIn1_Delay)
    {
        _devAmp->sendDelay(0, value);
        ui->label_delay_1->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
//    else if (sender() == ui->AIn2_Delay)
//    {
//        _devAmp->sendDelay(1, value);
//    }
//    else if (sender() == ui->AIn3_Delay)
//    {
//        _devAmp->sendDelay(2, value);
//    }
//    else if (sender() == ui->AIn4_Delay)
//    {
//        _devAmp->sendDelay(3, value);
//    }
}

/* Eq */
void DevSetting_RT1052_Poe_AP60_Bluetooth::setEq()
{
    int channel;
    EqData4B eq4B;
    EqData15B eq15B;
    uint8_t type = 0;

    if (sender() == ui->AIn1_Channel_Eq)
    {
        channel = 0;
        eq4B = _devAmp->getConfigData().channel_eq[channel];
        type = 0;
    }
    else if (sender() == ui->NIn1_Channel_Eq)
    {
        channel = 1;
        eq4B = _devAmp->getConfigData().channel_eq[channel];
        type = 0;
    }
//    else if (sender() == ui->NIn2_Channel_Eq)
//    {
//        channel = 3;
//        eq4B = _devAmp->getConfigData().channel_eq[channel];
//        type = 0;
//    }
    else if (sender() == ui->AIn1_Eq)
    {
        channel = 0;
        eq15B = _devAmp->getConfigData().Aout_eq[channel];
        type = 1;
        qDebug() << eq15B.eq[0].freq
                << eq15B.eq[1].freq
                << eq15B.eq[2].freq;
    }
//    else if (sender() == ui->AIn2_Eq)
//    {
//        channel = 1;
//        eq4B = _devAmp->getConfigData().Aout_eq[channel];
//        type = 1;
//    }
//    else if (sender() == ui->AIn3_Eq)
//    {
//        channel = 2;
//        eq4B = _devAmp->getConfigData().Aout_eq[channel];
//        type = 1;
//    }
//    else if (sender() == ui->AIn4_Eq)
//    {
//        channel = 3;
//        eq4B = _devAmp->getConfigData().Aout_eq[channel];
//        type = 1;
//    }

    DevSettingDlg dlg;
    EqView_AllBand *eqView;
    if (type == 0)
    {
        eqView = new EqView_AllBand(4, &dlg);
        eqView->showWidget(channel, QString(), eq4B.bypass, eq4B.eq);
//        eqView->setFixedSize(600,350);
    } else if(type == 1){
        eqView = new EqView_AllBand(15, &dlg);
        eqView->showWidget(channel, QString(), eq15B.bypass, eq15B.eq);
//        eqView->setFixedSize(600,650);
    }

    dlg.resize(eqView->size());
//    dlg.setFixedSize(eqView->width(),eqView->height());
    if (type == 0)
    {
        connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onChannelEqChanged(quint8, bool, EqData*)));
    } else if (type == 1) {
        connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
    }
    dlg.exec();
}

/* 处理函数 */
void DevSetting_RT1052_Poe_AP60_Bluetooth::onChannelEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    _devAmp->sendEq(0, ch, bypass, eqPara);
}

void DevSetting_RT1052_Poe_AP60_Bluetooth::onEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    _devAmp->sendEq(1, ch, bypass, eqPara);
}

/* Dyn */
void DevSetting_RT1052_Poe_AP60_Bluetooth::setDyn()
{
    int channel;
    DynData dynData;
    if (sender() == ui->AIn1_Dyn)
    {
        channel = 0;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }
//    else if (sender() == ui->AIn2_Dyn)
//    {
//        channel = 1;
//        dynData = _devAmp->getConfigData().Aout_dyn[channel];
//    }
//    else if (sender() == ui->AIn3_Dyn)
//    {
//        channel = 2;
//        dynData = _devAmp->getConfigData().Aout_dyn[channel];
//    }
//    else if (sender() == ui->AIn4_Dyn)
//    {
//        channel = 3;
//        dynData = _devAmp->getConfigData().Aout_dyn[channel];
//    }

    DevSettingDlg dlg;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
    dynWidget->setDisabled(_devAmp->isFactoryMode());//是否禁用小部件
    dynWidget->showWidget(false, channel, QString(), dynData);
    dlg.resize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

/* 处理函数 */
void DevSetting_RT1052_Poe_AP60_Bluetooth::onDynChanged(quint8 ch, bool isIn ,DynData dynData)
{
    _devAmp->sendDyn(ch, dynData);
}

/* XOVER */
void DevSetting_RT1052_Poe_AP60_Bluetooth::setXOver()
{
    quint8 ch = 0;
    if (sender() == ui->AIn1_xover)
    {
        ch = 0;
    }
//    else if (sender() == ui->AIn3_xover)
//    {
//        ch = 2;
//    }
//    else if (sender() == ui->AIn4_xover)
//    {
//        ch = 3;
//    }

    if (ch < 2)
    {
        DevSettingBase_Xover xWidget(ch, DevSettingBase_Xover::DevSettingBase_Xover_BandPass, _devAmp->getConfigData().Aout_xover[ch]);
        connect(&xWidget, SIGNAL(valueChanged(uint8_t, struct ConfigData_Xover)), this, SLOT(onXoverChanged(uint8_t,ConfigData_Xover)));
        xWidget.exec();
    } else {
//        qDebug()<<"nout_xover:"<<_devAmp->getConfigData().Nout_xover.bypass;
//        qDebug()<<"nout_xover:"<<_devAmp->getConfigData().Nout_xover.freq;
//        qDebug()<<"nout_xover:"<<_devAmp->getConfigData().Nout_xover.stages;
        DevSettingBase_Xover xWidget(ch, DevSettingBase_Xover::DevSettingBase_Xover_BandPass, _devAmp->getConfigData().Nout_xover);
        connect(&xWidget, SIGNAL(valueChanged(uint8_t, struct ConfigData_Xover)), this, SLOT(onXoverChanged(uint8_t,ConfigData_Xover)));
        xWidget.exec();
    }

}

void DevSetting_RT1052_Poe_AP60_Bluetooth::onXoverChanged(uint8_t ch, ConfigData_Xover xData)
{
    _devAmp->sendXover(ch, xData.bypass, xData.freq[0], xData.freq[1], xData.stages[0], xData.stages[1]);
}

/* AOut Mixer */
void DevSetting_RT1052_Poe_AP60_Bluetooth::setAOutMixer()
{

}

void DevSetting_RT1052_Poe_AP60_Bluetooth::MixerChangedSlot(int out, int in, bool enable)
{
    _devAmp->sendMixer(out, in, enable);
}

void DevSetting_RT1052_Poe_AP60_Bluetooth::bt_function_clicked(bool e)
{
    Q_UNUSED(e);
    if (sender() == ui->bt_peidui)
    {
        _devAmp->sendBluetoothControl("AT#CA\r\n");
    }
    else if (sender() == ui->bt_duankai)
    {
        _devAmp->sendBluetoothControl("AT#CD\r\n");
    }
    else if (sender() == ui->bt_chonglian)
    {
        _devAmp->sendBluetoothControl("AT#CA\r\n");
        QThread::usleep(100*1000);
        _devAmp->sendBluetoothControl("AT#CC\r\n");
    }
    else if (sender() == ui->bt_shangyishou)
    {
        _devAmp->sendBluetoothControl("AT#ME\r\n");
    }
    else if (sender() == ui->bt_play)
    {
        _devAmp->sendBluetoothControl("AT#MA\r\n");
    }
    else if (sender() == ui->bt_xiayishou)
    {
        _devAmp->sendBluetoothControl("AT#MD\r\n");
    }
    else if (sender() == ui->bt_yinliangjia)
    {
        _devAmp->sendBluetoothControl("AT#CK\r\n");
    }
    else if (sender() == ui->bt_yinliangjian)
    {
        _devAmp->sendBluetoothControl("AT#CL\r\n");
    }
}

void DevSetting_RT1052_Poe_AP60_Bluetooth::DisplaySlot(struct MsgData_RT1052_Poe b)
{
    ui->Meter_A_1->setLCurValue(b._monitor._ap60_bluetooth.meter_In[0]);
    ui->Meter_N_1->setLCurValue(b._monitor._ap60_bluetooth.meter_In[1]);
    ui->Meter_AL->setLCurValue(b._monitor._ap60_bluetooth.meter_Out[0]);

    switch (b._monitor._ap60_bluetooth.bluetooth_state) {
    case 0:
        ui->bt_zhuangtai->setText("Wait for connect");
        ui->bt_shangyishou->setEnabled(false);
        ui->bt_play->setEnabled(false);
        ui->bt_play->setText("Play");
        ui->bt_xiayishou->setEnabled(false);
        ui->bt_yinliangjia->setEnabled(false);
        ui->bt_yinliangjian->setEnabled(false);
        break;
    case 1:
        ui->bt_zhuangtai->setText("Pairing");
        ui->bt_shangyishou->setEnabled(false);
        ui->bt_play->setEnabled(false);
        ui->bt_play->setText("Play");
        ui->bt_xiayishou->setEnabled(false);
        ui->bt_yinliangjia->setEnabled(false);
        ui->bt_yinliangjian->setEnabled(false);
        break;
    case 2:
        ui->bt_zhuangtai->setText("Connected");
        ui->bt_shangyishou->setEnabled(true);
        ui->bt_play->setEnabled(true);
        ui->bt_play->setText("Play");
        ui->bt_xiayishou->setEnabled(true);
        ui->bt_yinliangjia->setEnabled(false);
        ui->bt_yinliangjian->setEnabled(false);
        break;
    case 3:
        ui->bt_zhuangtai->setText("Playing");
        ui->bt_shangyishou->setEnabled(true);
        ui->bt_play->setEnabled(true);
        ui->bt_play->setText("Pause");
        ui->bt_xiayishou->setEnabled(true);
        ui->bt_yinliangjia->setEnabled(true);
        ui->bt_yinliangjian->setEnabled(true);
        break;
    default:
        break;
    }

    ui->bt_yinliang->setText(QString::number(b._monitor._ap60_bluetooth.bluetooth_vol));
}

//// 另存场景为文件
//void DevSetting_RT1052_Poe_AP60_Bluetooth::on_saveAsBtn_clicked()
//{
//    QString fileName = QFileDialog::getSaveFileName(this, tr("save as..."), QDir::currentPath(), QString("file (*.amp)"));
//    if(!fileName.isEmpty()){
//        _devAmp->serialize(fileName);
//    }
//}

//// 加载场景文件
//void DevSetting_RT1052_Poe_AP60_Bluetooth::on_loadBtn_clicked()
//{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("file (*.amp)"));
//    if(!fileName.isEmpty())
//    {
//        _devAmp->deserialize(fileName);
//        displayConfigValue();
//        _devAmp->sendAllConfigPack(true);
//    }
//}

void DevSetting_RT1052_Poe_AP60_Bluetooth::on_save_clicked()
{
    _devAmp->sendSave();
}
