#include "devsetting_rt1052_poe_60w_2_4_fangtu.h"
#include "ui_devsetting_rt1052_poe_60w_2_4_fangtu.h"

#include "stable.h"
#ifdef Logo_BeiJingHuiSheng
#include "ui_devsetting_rt1052_poe_60w_2_4_Ceiadap.h"
#else
#include "ui_devsetting_rt1052_poe_60w_2_4.h"
#endif
#include "device/device_rt1052_poe_60w_2_4.h"
#include "deviceItem/deviceitem_rt1052_poe_60w_2_4.h"
#include "../trn/eqview_allband.h"
#include "customWidget/eqviewnormal.h"
#include "../spk/spk_alarmrecord.h"
#include "../amp/amp_filter1.h"
#include "../spk/spk_gainset.h"
#include "../trn/trn_dynwidget.h"
#include "../alarmconfig.h"
#include "../base/devsettingbase_xover.h"
#include "../amp/amp_mic_quad_mixer.h"
#include "../base/base_2_2_mixer.h"
#include "customWidget/overwriteitem.h"


// 构造函数
DevSetting_RT1052_Poe_60W_2_4_FangTu::DevSetting_RT1052_Poe_60W_2_4_FangTu(DeviceItem_RT1052_Poe_60W_2_4 *devItem,  QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_RT1052_Poe_60W_2_4_FangTu)
{
    ui->setupUi(this);
    _devItem = devItem;
    Q_ASSERT(devItem->getAES70Device());
    _devAmp = qobject_cast<Device_RT1052_Poe_60W_2_4*>(devItem->getAES70Device());
    connect(_devAmp, SIGNAL(displaySig(struct MsgData_RT1052_Poe)), this, SLOT(DisplaySlot(struct MsgData_RT1052_Poe)));

    setAttribute(Qt::WA_QuitOnClose, true);

    ui->AIn1_Delay->setRange(0, MAX_DELAY_TIME / 48.0);

    displayConfigValue();
    connectSigSlots();

    _devAmp->sendStartMonitor();

    ui->Meter_AR->setVisible(false);
    ui->label_18->setVisible(false);
    ui->label_6->setVisible(false);
    ui->groupBox_3->setFixedWidth(115);
    ui->AOut_Mixter->setMinimumWidth(160);
    resize(size().width()-ui->label_6->width(), size().height());
    setFixedSize(size());//设置最小值和最大值
}

DevSetting_RT1052_Poe_60W_2_4_FangTu::~DevSetting_RT1052_Poe_60W_2_4_FangTu()
{
    _devAmp->sendStopMonitor();
    delete ui;
}

void DevSetting_RT1052_Poe_60W_2_4_FangTu::connectSigSlots()
{
    /* 从设备初始化界面 */
    connect(_devAmp, SIGNAL(initConfigFromDevice()), this, SLOT(displayConfigValue()));

    /* Gain\Mute\Phase\Delay */
    connect(ui->AIn1_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn2_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->NIn1_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
//    connect(ui->NIn2_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn1_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn2_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
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
    connect(ui->AIn1_Delay, SIGNAL(valueChanged(double)), this, SLOT(setDelay(double)));
//    connect(ui->AIn2_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
//    connect(ui->AIn3_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
//    connect(ui->AIn4_Delay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));

    /* Eq */
    connect(ui->AIn1_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AIn2_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->NIn1_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
//    connect(ui->NIn2_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AIn1_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
//    connect(ui->AIn2_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
//    connect(ui->AIn3_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
//    connect(ui->AIn4_Eq, SIGNAL(clicked()), this, SLOT(setEq()));

    /* Dyn */
    connect(ui->AIn1_Channel_Dyn,SIGNAL(clicked()),this,SLOT(setDyn()));
    connect(ui->AIn1_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));
//    connect(ui->AIn2_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));
//    connect(ui->AIn3_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));
//    connect(ui->AIn4_Dyn, SIGNAL(clicked()), this, SLOT(setDyn()));

    /* XOVER */
    connect(ui->AIn1_xover, SIGNAL(clicked()), this, SLOT(setXOver()));
    connect(ui->AIn2_xover, SIGNAL(clicked()), this, SLOT(setXOver()));
//    connect(ui->AIn3_xover, SIGNAL(clicked()), this, SLOT(setXOver()));
//    connect(ui->AIn4_xover, SIGNAL(clicked()), this, SLOT(setXOver()));

    /* Mixer */
    connect(ui->AOut_Mixter, SIGNAL(clicked()), this, SLOT(setAOutMixer()));
}

void DevSetting_RT1052_Poe_60W_2_4_FangTu::displayConfigValue()
{
    ConfigData_RT1052_Poe_60W_2_4& cfgData = _devAmp->getConfigData();

    ui->AIn1_Channel_Gain->setValue(cfgData.channel_gain[0].vol);
    ui->NIn1_Channel_Gain->setValue(cfgData.channel_gain[1].vol);
    ui->AIn2_Channel_Gain->setValue(cfgData.channel_gain[2].vol);

//    ui->NIn2_Channel_Gain->setValue(cfgData.channel_gain[3].vol);

    ui->AIn1_Channel_Mute->setChecked(cfgData.channel_gain[0].mute);
    ui->NIn1_Channel_Mute->setChecked(cfgData.channel_gain[1].mute);
    ui->AIn2_Channel_Mute->setChecked(cfgData.channel_gain[2].mute);

//    ui->NIn2_Channel_Mute->setChecked(cfgData.channel_gain[3].mute);

    ui->AIn1_Gain->setValue(cfgData.Aout_gain[0].vol); /*ui->AIn1_Gain->SendValueChangedSignal();*/
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

    ui->AIn1_Delay->setValue(cfgData.Aout_delay[0].delay / 48.0);

//    ui->AIn2_Delay->setValue(cfgData.Aout_delay[1].delay);
//    ui->AIn3_Delay->setValue(cfgData.Aout_delay[2].delay);
//    ui->AIn4_Delay->setValue(cfgData.Aout_delay[3].delay);
    quint16 remain = MAX_DELAY_TIME / 48.0
             - ui->AIn1_Delay->value();//最大延时为260ms,调整为一个样值一调整
    ui->AIn1_Delay->setRange(0, ui->AIn1_Delay->value() + remain);
}

/* Gain\Mute\Phase\Delay */
void DevSetting_RT1052_Poe_60W_2_4_FangTu::setGain(int value)
{
    if (sender() == ui->AIn1_Channel_Gain)
    {
        _devAmp->sendVolume(0, 0, value);
    }
    else if (sender() == ui->AIn2_Channel_Gain)
    {
        _devAmp->sendVolume(0, 2, value);
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

void DevSetting_RT1052_Poe_60W_2_4_FangTu::setMute(bool enable)
{qDebug() << __FUNCTION__ << __LINE__ << enable;
    if (sender() == ui->AIn1_Channel_Mute)
    {
        _devAmp->sendMute(0, 0, enable);
    }
    else if (sender() == ui->AIn2_Channel_Mute)
    {
        _devAmp->sendMute(0, 2, enable);
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

void DevSetting_RT1052_Poe_60W_2_4_FangTu::setPhase(bool enable)
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

void DevSetting_RT1052_Poe_60W_2_4_FangTu::setDelay(double value)
{
    quint16 remain = MAX_DELAY_TIME / 48.0
            - ui->AIn1_Delay->value();//最大延时为260ms
    ui->AIn1_Delay->setRange(0, ui->AIn1_Delay->value() + remain);
//    ui->AIn2_Delay->setRange(0, ui->AIn2_Delay->value() + remain);
//    ui->AIn3_Delay->setRange(0, ui->AIn3_Delay->value() + remain);
//    ui->AIn4_Delay->setRange(0, ui->AIn4_Delay->value() + remain);
    if (sender() == ui->AIn1_Delay)
    {
        _devAmp->sendDelay(0, value * 48.0);
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
void DevSetting_RT1052_Poe_60W_2_4_FangTu::setEq()
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
    else if (sender() == ui->AIn2_Channel_Eq)
    {
        channel = 2;
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

    DevSettingDlg dlg/*(this)*/;
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
void DevSetting_RT1052_Poe_60W_2_4_FangTu::onChannelEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
//    qDebug() << "ch:" << ch << "   bypass:"  << bypass;
//    for(int i = 0; i< 4; i++){
//        qDebug() << "   q:" << eqPara[i].q << "   gain:" << eqPara[i].gain << "   freq:" << eqPara[i].freq;
//    }
    _devAmp->sendEq(0, ch, bypass, eqPara);
}

void DevSetting_RT1052_Poe_60W_2_4_FangTu::onEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    _devAmp->sendEq(1, ch, bypass, eqPara);
}

/* Dyn */
void DevSetting_RT1052_Poe_60W_2_4_FangTu::setDyn()
{
    int channel;
    DynData dynData;
    if (sender() == ui->AIn1_Dyn)
    {
        channel = 0;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }else if(sender() == ui->AIn1_Channel_Dyn)
    {
        channel = 1;
        dynData = _devAmp->getConfigData().channel_dyn;//hrp: 修正dyn数据加载位置
//        dynData = _devAmp->getConfigData().Aout_dyn[channel];
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

    DevSettingDlg dlg/*(this)*/;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
    dynWidget->setDisabled(_devAmp->isFactoryMode());//是否禁用小部件
    dynWidget->showWidget(false, channel, QString(), dynData);
    dlg.resize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

/* 处理函数 */
void DevSetting_RT1052_Poe_60W_2_4_FangTu::onDynChanged(quint8 ch, bool isIn ,DynData dynData)
{
    _devAmp->sendDyn(ch, dynData);
}

/* XOVER */
void DevSetting_RT1052_Poe_60W_2_4_FangTu::setXOver()
{
    quint8 ch = 0;
    if (sender() == ui->AIn1_xover)
    {
        ch = 0;
    }
    else if (sender() == ui->AIn2_xover)
    {
        ch = 2;
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

void DevSetting_RT1052_Poe_60W_2_4_FangTu::onXoverChanged(uint8_t ch, ConfigData_Xover xData)
{
    _devAmp->sendXover(ch, xData.bypass, xData.freq[0], xData.freq[1], xData.stages[0], xData.stages[1]);
}


/* AOut Mixer */
void DevSetting_RT1052_Poe_60W_2_4_FangTu::setAOutMixer()
{
    if (sender() == ui->AOut_Mixter)
    {
        QStringList in,out;
        in<<"A-1"<<"N-1";
        out<<"A-L"<<"A-R";

        base_2_2_mixer mixer(in,out,_devAmp->getConfigData().Aout_mixer/*,this*/);
        connect(&mixer,SIGNAL(mixerchanged(int,int,bool)),this,SLOT(MixerChangedSlot(int,int,bool)));
        mixer.exec();
//        QStringList in, out;
//        in << "A-In1" << "A-In2" << "N-In1" << "N-In2";
//        out << "A-Out1" << "A-Out2" << "A-Out3" << "A-Out4";
//        Mixer_4_4 mixer(in, out, _devAmp->getConfigData().Aout_mixer, this);
//        connect(&mixer, SIGNAL(mixerChanged(int,int,bool)), this, SLOT(MixerChangedSlot(int,int,bool)));
//        mixer.exec();
    }
}

void DevSetting_RT1052_Poe_60W_2_4_FangTu::MixerChangedSlot(int out, int in, bool enable)
{
    _devAmp->sendMixer(out, in, enable);
}

void DevSetting_RT1052_Poe_60W_2_4_FangTu::DisplaySlot(struct MsgData_RT1052_Poe b)
{
    ui->Meter_A_1->setLCurValue(b._monitor._line_60w_4_4.meter_In[0]);
    ui->Meter_A_2->setLCurValue(b._monitor._line_60w_4_4.meter_In[2]);
    ui->Meter_N_1->setLCurValue(b._monitor._line_60w_4_4.meter_In[1]);
    ui->Meter_AL->setLCurValue(b._monitor._line_60w_4_4.meter_Out[0]);
    ui->Meter_AR->setLCurValue(b._monitor._line_60w_4_4.meter_Out[1]);
    ui->Meter_N_Out->setLCurValue(b._monitor._line_60w_4_4.meter_Out[2]);

//    ui->Meter_In1->setLCurValue(b._monitor._line_60w_4_4.meter_In[0]);
//    ui->Meter_In2->setLCurValue(b._monitor._line_60w_4_4.meter_In[1]);
//    ui->Meter_Out1->setLCurValue(b._monitor._line_60w_4_4.meter_Out[0]);
//    ui->Meter_Out2->setLCurValue(b._monitor._line_60w_4_4.meter_Out[1]);
//    ui->Meter_Out3->setLCurValue(b._monitor._line_60w_4_4.meter_Out[2]);
//    ui->Meter_Out4->setLCurValue(b._monitor._line_60w_4_4.meter_Out[3]);
    if(b._monitor._line_60w_4_4.AD1_Value > 85.0f
            || b._monitor._line_60w_4_4.temperature > 45.0f)
        return ;
    ui->_24v->setText(tr("%1 v").arg(QString::number(b._monitor._line_60w_4_4.AD1_Value, 'f', 1)));
    ui->_Temperature->setText(tr("%1 C").arg(QString::number(b._monitor._line_60w_4_4.temperature, 'f', 1)));
//    qDebug() << b._display.meter[0][0]
//            << b._display.meter[1][0];
}

//// 另存场景为文件
void DevSetting_RT1052_Poe_60W_2_4_FangTu::on_saveAsBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(NULL, tr("save as..."), QDir::currentPath(), QString("file (*.amp)"));
    if(!fileName.isEmpty()){
        _devAmp->serialize(fileName);
    }
}

//// 加载场景文件
void DevSetting_RT1052_Poe_60W_2_4_FangTu::on_loadBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(NULL, tr("open file"), QDir::currentPath(), tr("file (*.amp)"));
    if(!fileName.isEmpty())
    {
        _devAmp->deserialize(fileName);
        displayConfigValue();
        _devAmp->sendAllConfigPack(true);
    }
}

void DevSetting_RT1052_Poe_60W_2_4_FangTu::on_save_clicked()
{
    _devAmp->sendSave();
}
