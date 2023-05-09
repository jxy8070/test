#include "devsetting_amp_2_0_mic_line_fangtu.h"
#include "ui_devsetting_amp_2_0_mic_line_fangtu.h"

#include "stable.h"
#include "ui_devsetting_amp_2_0_mic_line.h"
#include "device/device_amp_2_0_mic_line.h"
#include "deviceItem/deviceitem_amp_2_0_mic_line.h"
#include "../trn/eqview_allband.h"
#include "customWidget/eqviewnormal.h"
#include "../spk/spk_alarmrecord.h"
#include "../amp/amp_filter1.h"
#include "../spk/spk_gainset.h"
#include "../trn/trn_dynwidget.h"
#include "../alarmconfig.h"
#include "../amp/amp_2_0_mic_line_mixer.h"
#include "../amp/amp_2_0_mic_line_xover.h"
#include "customWidget/overwriteitem.h"

// 构造函数
DevSetting_Amp_2_0_Mic_Line_FangTu::DevSetting_Amp_2_0_Mic_Line_FangTu(DeviceItem_Amp_2_0_Mic_Line *devItem,  QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::DevSetting_Amp_2_0_Mic_Line_FangTu)
{
    ui->setupUi(this);
    _devItem = devItem;
    Q_ASSERT(devItem->getAES70Device());
    _dev = qobject_cast<Device_Amp_2_0_Mic_Line*>(devItem->getAES70Device());

    setAttribute(Qt::WA_QuitOnClose, true);
//    setFixedSize(size());
    setFixedSize(102+108+106+36+10, 180+121+160);
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);

    displayConfigValue();
    connectSigSlots();

    connect(_dev, SIGNAL(displaySig(MsgData_Amp_2_0_Mic_Line)), this, SLOT(DisplaySlot(MsgData_Amp_2_0_Mic_Line)));
    _dev->sendStartMonitor();
}

DevSetting_Amp_2_0_Mic_Line_FangTu::~DevSetting_Amp_2_0_Mic_Line_FangTu()
{
    _dev->sendStopMonitor();
    delete ui;
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::connectSigSlots()
{
    /* 从设备初始化界面 */
    connect(_dev, SIGNAL(initConfigFromDevice()), this, SLOT(displayConfigValue()));
    connect(ui->Mic_open,SIGNAL(clicked(bool)),this,SLOT(micOpenClicked(bool)));

    connect(ui->micgain_1, SIGNAL(currentIndexChanged(int)), this, SLOT(MicGainChanged(int)));
    connect(ui->micphantom_1, SIGNAL(clicked(bool)), this, SLOT(MicPhantomChanged(bool)));

    connect(ui->gain_1, SIGNAL(valueChanged(int)), this, SLOT(GainChanged(int)));
    connect(ui->gain_2, SIGNAL(valueChanged(int)), this, SLOT(GainChanged(int)));
    connect(ui->AIn1_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn2_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));

    connect(ui->xover_1, SIGNAL(clicked()), this, SLOT(xoverClicked()));
    connect(ui->xover_2, SIGNAL(clicked()), this, SLOT(xoverClicked()));

    connect(ui->dyn_1, SIGNAL(clicked()), this, SLOT(DynClicked()));
    connect(ui->dyn_2, SIGNAL(clicked()), this, SLOT(DynClicked()));

    connect(ui->eq_1, SIGNAL(clicked()), this, SLOT(EqClicked()));
    connect(ui->eq_2, SIGNAL(clicked()), this, SLOT(EqClicked()));

    connect(ui->phase_1, SIGNAL(clicked(bool)), this, SLOT(OutPhaseClicked(bool)));
    connect(ui->phase_2, SIGNAL(clicked(bool)), this, SLOT(OutPhaseClicked(bool)));

    connect(ui->Mixer, SIGNAL(clicked()), this, SLOT(MixerClicked()));

    connect(ui->outgain_1, SIGNAL(valueChanged(int)), this, SLOT(OutGainChanged(int)));
    connect(ui->outgain_2, SIGNAL(valueChanged(int)), this, SLOT(OutGainChanged(int)));
    connect(ui->NOut1_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->NOut2_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::displayConfigValue()
{
    ConfigData_Amp_2_0_Mic_Line& temp = _dev->getConfigData();

//    ui->Mic_open->setChecked(true);
    if(temp.mic_open != 0)
    {
        ui->Mic_open->setChecked(true);
        ui->micphantom_1->setEnabled(true);
        ui->micgain_1->setEnabled(true);
    }else
    {
        ui->Mic_open->setChecked(false);
        ui->micphantom_1->setEnabled(false);
        ui->micgain_1->setEnabled(false);
    }
    ui->micgain_1->setCurrentIndex(temp.mic_gain);
    ui->micphantom_1->setChecked(temp.mic_phantom);
    ui->micgain_1->setEnabled(temp.mic_phantom);

    ui->gain_1->setValue(temp.gain[0].vol);
    ui->gain_2->setValue(temp.gain[1].vol);
    ui->AIn1_Channel_Mute->setChecked(temp.gain[0].mute);
    ui->AIn2_Channel_Mute->setChecked(temp.gain[1].mute);

    ui->phase_1->setChecked(temp.gain[0].phase);
    ui->phase_2->setChecked(temp.gain[1].phase);

    ui->outgain_1->setValue(temp.Nout_gain[0].vol);
    ui->outgain_2->setValue(temp.Nout_gain[1].vol);
    ui->NOut1_Channel_Mute->setChecked(temp.Nout_gain[0].mute);
    ui->NOut2_Channel_Mute->setChecked(temp.Nout_gain[1].mute);
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::MicGainChanged(int value)
{
    if (sender() == ui->micgain_1)
    {
        _dev->sendMicGain(value);
    }
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::MicPhantomChanged(bool enable)
{
    if (sender() == ui->micphantom_1)
    {
        _dev->sendMicPhantom(enable);
//        ui->micgain_1->setEnabled(enable);
    }
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::GainChanged(int value)
{
    int ch = 0;
    if (sender() == ui->gain_1)
    {
        ch = 0;
    } else if (sender() == ui->gain_2) {
        ch = 1;
    }

    _dev->sendGain(0, ch, value);
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::setMute(bool enable)
{
    if (sender() == ui->AIn1_Channel_Mute)
    {
        _dev->sendMute(0, 0, enable);
    }
    else if (sender() == ui->AIn2_Channel_Mute)
    {
        _dev->sendMute(0, 1, enable);
    }
    else if (sender() == ui->NOut1_Channel_Mute)
    {
        _dev->sendMute(1, 0, enable);
    }
    else if (sender() == ui->NOut2_Channel_Mute)
    {
        _dev->sendMute(1, 1, enable);
    }
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::xoverClicked()
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

    Amp_2_0_Mic_Line_xover xover(_dev, ch/*, this*/);
    xover.exec();
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::DynClicked()
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

    DynData dynData = _dev->getConfigData().dyn[channel];
    DevSettingDlg dlg/*(this)*/;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
    dynWidget->setDisabled(_dev->isFactoryMode());
    dynWidget->showWidget(false, channel, QString(), dynData);
    dlg.setFixedSize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::onDynChanged(quint8 ch, bool isIn ,DynData dynData)
{
    _dev->sendDyn(ch, dynData);
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::EqClicked()
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

    EqData4B eq4B = _dev->getConfigData().eq[channel];
    DevSettingDlg dlg/*(this)*/;
    EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
    eqView->showWidget(channel, QString(), eq4B.bypass, eq4B.eq);
    dlg.setFixedSize(eqView->size());
    connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
    dlg.exec();
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::onEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    _dev->sendEq(ch, bypass, eqPara);
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::OutPhaseClicked(bool enable)
{
    int ch = 0;
    if (sender() == ui->phase_1)
    {
        ch = 0;
    } else if (sender() == ui->phase_2) {
        ch = 1;
    }

    _dev->sendPhase(ch, enable);
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::MixerClicked()
{
    if (sender() == ui->Mixer)
    {
        Amp_2_0_Mic_Line_Mixer mixer(_dev/*, this*/);
        mixer.exec();
    }
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::OutGainChanged(int value)
{
    int ch = 0;
    if (sender() == ui->outgain_1)
    {
        ch = 0;
    } else if (sender() == ui->outgain_2) {
        ch = 1;
    }

    _dev->sendGain(1, ch, value);
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::DisplaySlot(MsgData_Amp_2_0_Mic_Line b)
{
    ui->Meter_In_1->setLCurValue(b._monitor.meter_In[0]);
    ui->Meter_In_2->setLCurValue(b._monitor.meter_In[1]);

    ui->Meter_Out_1->setLCurValue(b._monitor.meter_Out[0]);
    ui->Meter_Out_2->setLCurValue(b._monitor.meter_Out[1]);

//    qDebug() << __FUNCTION__ << __LINE__
//             << b._monitor.meter_Out[0] << b._monitor.meter_Out[1];
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::micOpenClicked(bool d)
{
    if(ui->Mic_open->isChecked())
    {
        ui->micphantom_1->setEnabled(true);
        ui->micgain_1->setEnabled(true);
    }else
    {
        ui->micphantom_1->setEnabled(false);
        ui->micgain_1->setEnabled(false);
    }
    _dev->sendMic_open((quint8)d);
}

void DevSetting_Amp_2_0_Mic_Line_FangTu::on_save_clicked()
{
    _dev->sendSave();
}

