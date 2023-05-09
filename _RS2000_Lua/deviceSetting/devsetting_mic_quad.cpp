#include "stable.h"
#include "devsetting_mic_quad.h"
#include "ui_devsetting_mic_quad.h"
#include "device/device_mic_quad.h"
#include "deviceItem/deviceitem_mic_quad.h"
#include "trn/eqview_allband.h"
#include "customWidget/eqviewnormal.h"
#include "spk/spk_alarmrecord.h"
#include "amp/amp_filter1.h"
#include "spk/spk_gainset.h"
#include "trn/trn_dynwidget.h"
#include "alarmconfig.h"
#include "amp/amp_mic_quad_mixer.h"
#include "amp/amp_mic_quad_xover.h"
/*
 * 注意：
 * ControlValue中的delay在接收的地方需要进行" / 3"的处理
  *
 * */
// 构造函数
DevSetting_Mic_Quad::DevSetting_Mic_Quad(DeviceItem_Mic_Quad *devItem,  QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_Mic_Quad)
{
    ui->setupUi(this);
    _devItem = devItem;
    Q_ASSERT(devItem->getAES70Device());
    _dev = qobject_cast<Device_Mic_Quad*>(devItem->getAES70Device());

    setAttribute(Qt::WA_QuitOnClose, true);
    setFixedSize(size());

    displayConfigValue();
    connectSigSlots();

    connect(_dev, SIGNAL(displaySig(MsgData_Mic_Quad)), this, SLOT(DisplaySlot(MsgData_Mic_Quad)));
    _dev->sendStartMonitor();
}

DevSetting_Mic_Quad::~DevSetting_Mic_Quad()
{
    _dev->sendStopMonitor();
    delete ui;
}

void DevSetting_Mic_Quad::connectSigSlots()
{
    /* 从设备初始化界面 */
    connect(_dev, SIGNAL(initConfigFromDevice()), this, SLOT(displayConfigValue()));

    connect(ui->micgain_1, SIGNAL(currentIndexChanged(int)), this, SLOT(MicGainChanged(int)));
    connect(ui->micgain_2, SIGNAL(currentIndexChanged(int)), this, SLOT(MicGainChanged(int)));

    connect(ui->micphantom_1, SIGNAL(clicked(bool)), this, SLOT(MicPhantomChanged(bool)));
    connect(ui->micphantom_2, SIGNAL(clicked(bool)), this, SLOT(MicPhantomChanged(bool)));
    connect(ui->micphantom_3, SIGNAL(clicked(bool)), this, SLOT(MicPhantomChanged(bool)));
    connect(ui->micphantom_4, SIGNAL(clicked(bool)), this, SLOT(MicPhantomChanged(bool)));

    connect(ui->gain_1, SIGNAL(valueChanged(int)), this, SLOT(GainChanged(int)));
    connect(ui->gain_2, SIGNAL(valueChanged(int)), this, SLOT(GainChanged(int)));
    connect(ui->gain_3, SIGNAL(valueChanged(int)), this, SLOT(GainChanged(int)));
    connect(ui->gain_4, SIGNAL(valueChanged(int)), this, SLOT(GainChanged(int)));
    connect(ui->AIn1_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn2_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn3_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn4_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));

    connect(ui->xover_1, SIGNAL(clicked()), this, SLOT(xoverClicked()));
    connect(ui->xover_2, SIGNAL(clicked()), this, SLOT(xoverClicked()));
    connect(ui->xover_3, SIGNAL(clicked()), this, SLOT(xoverClicked()));
    connect(ui->xover_4, SIGNAL(clicked()), this, SLOT(xoverClicked()));

    connect(ui->dyn_1, SIGNAL(clicked()), this, SLOT(DynClicked()));
    connect(ui->dyn_2, SIGNAL(clicked()), this, SLOT(DynClicked()));
    connect(ui->dyn_3, SIGNAL(clicked()), this, SLOT(DynClicked()));
    connect(ui->dyn_4, SIGNAL(clicked()), this, SLOT(DynClicked()));

    connect(ui->eq_1, SIGNAL(clicked()), this, SLOT(EqClicked()));
    connect(ui->eq_2, SIGNAL(clicked()), this, SLOT(EqClicked()));
    connect(ui->eq_3, SIGNAL(clicked()), this, SLOT(EqClicked()));
    connect(ui->eq_4, SIGNAL(clicked()), this, SLOT(EqClicked()));

    connect(ui->phase_1, SIGNAL(clicked(bool)), this, SLOT(OutPhaseClicked(bool)));
    connect(ui->phase_2, SIGNAL(clicked(bool)), this, SLOT(OutPhaseClicked(bool)));
    connect(ui->phase_3, SIGNAL(clicked(bool)), this, SLOT(OutPhaseClicked(bool)));
    connect(ui->phase_4, SIGNAL(clicked(bool)), this, SLOT(OutPhaseClicked(bool)));

    connect(ui->Mixer, SIGNAL(clicked()), this, SLOT(MixerClicked()));

    connect(ui->outgain_1, SIGNAL(valueChanged(int)), this, SLOT(OutGainChanged(int)));
    connect(ui->outgain_2, SIGNAL(valueChanged(int)), this, SLOT(OutGainChanged(int)));
    connect(ui->outgain_3, SIGNAL(valueChanged(int)), this, SLOT(OutGainChanged(int)));
    connect(ui->outgain_4, SIGNAL(valueChanged(int)), this, SLOT(OutGainChanged(int)));
    connect(ui->NOut1_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->NOut2_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->NOut3_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->NOut4_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
}

void DevSetting_Mic_Quad::displayConfigValue()
{
    ConfigData_Mic_Quad& temp = _dev->getConfigData();

    ui->micgain_1->setCurrentIndex(temp.gain[0]);
    ui->micgain_2->setCurrentIndex(temp.gain[1]);

    ui->micphantom_1->setChecked(temp.phantom[0]);
    ui->micphantom_2->setChecked(temp.phantom[1]);
    ui->micphantom_3->setChecked(temp.phantom[2]);
    ui->micphantom_4->setChecked(temp.phantom[3]);

    ui->gain_1->setValue(temp.N_gain[0].vol);
    ui->gain_2->setValue(temp.N_gain[1].vol);
    ui->gain_3->setValue(temp.N_gain[2].vol);
    ui->gain_4->setValue(temp.N_gain[3].vol);
    ui->AIn1_Channel_Mute->setChecked(temp.N_gain[0].mute);
    ui->AIn2_Channel_Mute->setChecked(temp.N_gain[1].mute);
    ui->AIn3_Channel_Mute->setChecked(temp.N_gain[2].mute);
    ui->AIn4_Channel_Mute->setChecked(temp.N_gain[3].mute);

    ui->phase_1->setChecked(temp.N_gain[0].phase);
    ui->phase_2->setChecked(temp.N_gain[1].phase);
    ui->phase_3->setChecked(temp.N_gain[2].phase);
    ui->phase_4->setChecked(temp.N_gain[3].phase);

    ui->outgain_1->setValue(temp.NOut_gain[0].vol);
    ui->outgain_2->setValue(temp.NOut_gain[1].vol);
    ui->outgain_3->setValue(temp.NOut_gain[2].vol);
    ui->outgain_4->setValue(temp.NOut_gain[3].vol);
    ui->NOut1_Channel_Mute->setChecked(temp.NOut_gain[0].mute);
    ui->NOut2_Channel_Mute->setChecked(temp.NOut_gain[1].mute);
    ui->NOut3_Channel_Mute->setChecked(temp.NOut_gain[2].mute);
    ui->NOut4_Channel_Mute->setChecked(temp.NOut_gain[3].mute);
}

void DevSetting_Mic_Quad::MicGainChanged(int value)
{
    int ch = 0;
    if (sender() == ui->micgain_1)
    {
        ch = 0;
    } else if (sender() == ui->micgain_2) {
        ch = 1;
    }

    _dev->sendMicGain(ch, value);
}

void DevSetting_Mic_Quad::MicPhantomChanged(bool enable)
{
    int ch = 0;
    if (sender() == ui->micphantom_1)
    {
        ch = 0;
    } else if (sender() == ui->micphantom_2) {
        ch = 1;
    } else if (sender() == ui->micphantom_3) {
        ch = 2;
    } else if (sender() == ui->micphantom_4) {
        ch = 3;
    }

    _dev->sendMicPhantom(ch, enable);
}

void DevSetting_Mic_Quad::GainChanged(int value)
{
    int ch = 0;
    if (sender() == ui->gain_1)
    {
        ch = 0;
    } else if (sender() == ui->gain_2) {
        ch = 1;
    } else if (sender() == ui->gain_3) {
        ch = 2;
    } else if (sender() == ui->gain_4) {
        ch = 3;
    }

    _dev->sendGain(0, ch, value);
}

void DevSetting_Mic_Quad::setMute(bool enable)
{
    if (sender() == ui->AIn1_Channel_Mute)
    {
        _dev->sendMute(0, 0, enable);
    }
    else if (sender() == ui->AIn2_Channel_Mute)
    {
        _dev->sendMute(0, 1, enable);
    }
    else if (sender() == ui->AIn3_Channel_Mute)
    {
        _dev->sendMute(0, 2, enable);
    }
    else if (sender() == ui->AIn4_Channel_Mute)
    {
        _dev->sendMute(0, 3, enable);
    }
    else if (sender() == ui->NOut1_Channel_Mute)
    {
        _dev->sendMute(1, 0, enable);
    }
    else if (sender() == ui->NOut2_Channel_Mute)
    {
        _dev->sendMute(1, 1, enable);
    }
    else if (sender() == ui->NOut3_Channel_Mute)
    {
        _dev->sendMute(1, 2, enable);
    }
    else if (sender() == ui->NOut4_Channel_Mute)
    {
        _dev->sendMute(1, 3, enable);
    }
}

void DevSetting_Mic_Quad::xoverClicked()
{
    int ch = 0;
    if (sender() == ui->xover_1)
    {
        ch = 0;
    }
    else if (sender() == ui->xover_2)
    {
        ch = 1;
    }
    else if (sender() == ui->xover_3)
    {
        ch = 2;
    }
    else if (sender() == ui->xover_4)
    {
        ch = 3;
    }

    Amp_Mic_Quad_Xover xover(_dev, ch, this);
    xover.exec();
}

void DevSetting_Mic_Quad::DynClicked()
{
    int channel;

    if (sender() == ui->dyn_1)
    {
        channel = 0;
    }
    else if (sender() == ui->dyn_2)
    {
        channel = 1;
    }
    else if (sender() == ui->dyn_3)
    {
        channel = 2;
    }
    else if (sender() == ui->dyn_4)
    {
        channel = 3;
    }

    DynData dynData = _dev->getConfigData().N_dyn[channel];
    DevSettingDlg dlg;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
    dynWidget->setDisabled(_dev->isFactoryMode());
    dynWidget->showWidget(false, channel, QString(), dynData);
    dlg.resize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

void DevSetting_Mic_Quad::onDynChanged(quint8 ch, bool isIn ,DynData dynData)
{
    _dev->sendDyn(ch, dynData);
}

void DevSetting_Mic_Quad::EqClicked()
{
    int channel;

    if (sender() == ui->eq_1)
    {
        channel = 0;

    }
    else if (sender() == ui->eq_2)
    {
        channel = 1;
    }
    else if (sender() == ui->eq_3)
    {
        channel = 2;
    }
    else if (sender() == ui->eq_4)
    {
        channel = 3;
    }

    EqData4B eq4B = _dev->getConfigData().N_eq[channel];
    DevSettingDlg dlg;
    EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
    eqView->showWidget(channel, QString(), eq4B.bypass, eq4B.eq);
    dlg.resize(eqView->size());
    connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
    dlg.exec();
}

void DevSetting_Mic_Quad::onEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    _dev->sendEq(ch, bypass, eqPara);
}

void DevSetting_Mic_Quad::OutPhaseClicked(bool enable)
{
    int ch = 0;
    if (sender() == ui->phase_1)
    {
        ch = 0;
    } else if (sender() == ui->phase_2) {
        ch = 1;
    } else if (sender() == ui->phase_3) {
        ch = 2;
    } else if (sender() == ui->phase_4) {
        ch = 3;
    }

    _dev->sendPhase(ch, enable);
}

void DevSetting_Mic_Quad::MixerClicked()
{
    if (sender() == ui->Mixer)
    {
        QStringList in, out;
        in << "A-In1" << "A-In2" << "A-In3" << "A-In4";
        out << "N-Out1" << "N-Out2" << "N-Out3" << "N-Out4";
        Mixer_4_4 mixer(in, out, _dev->getConfigData().N_mixer, this);
        connect(&mixer, SIGNAL(mixerChanged(int,int,bool)), this, SLOT(MixerChangedSlot(int,int,bool)));
        mixer.exec();
    }
}

void DevSetting_Mic_Quad::MixerChangedSlot(int out, int in, bool enable)
{
    _dev->sendMixer(out, in, enable);
}

void DevSetting_Mic_Quad::OutGainChanged(int value)
{
    int ch = 0;
    if (sender() == ui->outgain_1)
    {
        ch = 0;
    } else if (sender() == ui->outgain_2) {
        ch = 1;
    } else if (sender() == ui->outgain_3) {
        ch = 2;
    } else if (sender() == ui->outgain_4) {
        ch = 3;
    }

    _dev->sendGain(1, ch, value);
}

void DevSetting_Mic_Quad::DisplaySlot(MsgData_Mic_Quad b)
{
    ui->Meter_In_1->setLCurValue(b._monitor.meter_In[0]);
    ui->Meter_In_2->setLCurValue(b._monitor.meter_In[1]);
    ui->Meter_In_3->setLCurValue(b._monitor.meter_In[2]);
    ui->Meter_In_4->setLCurValue(b._monitor.meter_In[3]);

    ui->Meter_Out_1->setLCurValue(b._monitor.meter_Out[0]);
    ui->Meter_Out_2->setLCurValue(b._monitor.meter_Out[1]);
    ui->Meter_Out_3->setLCurValue(b._monitor.meter_Out[2]);
    ui->Meter_Out_4->setLCurValue(b._monitor.meter_Out[3]);
}

void DevSetting_Mic_Quad::on_save_clicked()
{
    _dev->sendSave();
}
