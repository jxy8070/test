#include "stable.h"
#include "devsetting_rt1052_AP400.h"
#ifdef Logo_BeiJingHuiSheng
#include "ui_devsetting_rt1052_AP400_Ceiadap.h"
#else
#include "ui_devsetting_rt1052_AP400.h"
#endif
#include "device/device_rt1052_AP400.h"
#include "deviceItem/deviceitem_rt1052_AP400.h"
#include "trn/eqview_allband.h"
#include "customWidget/eqviewnormal.h"
#include "spk/spk_alarmrecord.h"
#include "amp/amp_filter1.h"
#include "spk/spk_gainset.h"
#include "trn/trn_dynwidget.h"
#include "alarmconfig.h"
#include "base/devsettingbase_xover.h"
#include "amp/amp_mic_quad_mixer.h"
#include "base/base_route.h"

// 构造函数
DevSetting_RT1052_AP400::DevSetting_RT1052_AP400(DeviceItem_RT1052_AP400 *devItem,  QWidget *parent) :
    DevSettingDlg(parent),
#ifdef Logo_BeiJingHuiSheng
    ui(new Ui::DevSetting_RT1052_AP400_Ceiadap)
#else
    ui(new Ui::DevSetting_RT1052_AP400)
#endif
{
    ui->setupUi(this);
    _devItem = devItem;
    Q_ASSERT(devItem->getAES70Device());
    _devAmp = qobject_cast<Device_RT1052_AP400*>(devItem->getAES70Device());
    connect(_devAmp, SIGNAL(displaySig(struct MsgData_RT1052_Poe)), this, SLOT(DisplaySlot(struct MsgData_RT1052_Poe)));

    setAttribute(Qt::WA_QuitOnClose, true);

    setFixedSize(size());//设置最小值和最大值

    ui->AOut_Delay_1->setRange(0, MAX_DELAY_TIME);
    ui->AOut_Delay_2->setRange(0, MAX_DELAY_TIME);
    ui->AOut_Delay_3->setRange(0, MAX_DELAY_TIME);
    ui->AOut_Delay_4->setRange(0, MAX_DELAY_TIME);
    ui->AOut_Delay_5->setRange(0, MAX_DELAY_TIME);
    ui->AOut_Delay_6->setRange(0, MAX_DELAY_TIME);
#ifndef Logo_BeiJingHuiSheng
    ui->AOut_Delay_7->setRange(0, MAX_DELAY_TIME);
    ui->AOut_Delay_8->setRange(0, MAX_DELAY_TIME);
#endif
    ui->label_delay_1->setText(tr("0ms"));
    ui->label_delay_2->setText(tr("0ms"));
    ui->label_delay_3->setText(tr("0ms"));
    ui->label_delay_4->setText(tr("0ms"));
    ui->label_delay_5->setText(tr("0ms"));
    ui->label_delay_6->setText(tr("0ms"));
#ifndef Logo_BeiJingHuiSheng
    ui->label_delay_7->setText(tr("0ms"));
    ui->label_delay_8->setText(tr("0ms"));
#endif

    QString angle_sign=u8"°";
    ui->Directivity_Angle->clear();
#if Logo_HuShan
    ui->Directivity_Angle->addItem(QString("20")+angle_sign);
    ui->Directivity_Angle->addItem(QString("15")+angle_sign);
    ui->Directivity_Angle->addItem(QString("10")+angle_sign);
    ui->Directivity_Angle->addItem(QString("5")+angle_sign);
    ui->Directivity_Angle->addItem(QString("0")+angle_sign);
    ui->Directivity_Angle->addItem(QString("-5")+angle_sign);
    ui->Directivity_Angle->addItem(QString("-10")+angle_sign);
    ui->Directivity_Angle->addItem(QString("-15")+angle_sign);
    ui->Directivity_Angle->addItem(QString("-20")+angle_sign);
#else
    ui->Directivity_Angle->addItem(QString("15")+angle_sign);
    ui->Directivity_Angle->addItem(QString("11.25")+angle_sign);
    ui->Directivity_Angle->addItem(QString("7.5")+angle_sign);
    ui->Directivity_Angle->addItem(QString("3.75")+angle_sign);
    ui->Directivity_Angle->addItem(QString("0")+angle_sign);
    ui->Directivity_Angle->addItem(QString("-3.75")+angle_sign);
    ui->Directivity_Angle->addItem(QString("-7.5")+angle_sign);
    ui->Directivity_Angle->addItem(QString("-11.25")+angle_sign);
    ui->Directivity_Angle->addItem(QString("-15")+angle_sign);
#endif

    displayConfigValue();
    connectSigSlots();

    _devAmp->sendStartMonitor();
}

DevSetting_RT1052_AP400::~DevSetting_RT1052_AP400()
{
    _devAmp->sendStopMonitor();
    delete ui;
}

void DevSetting_RT1052_AP400::connectSigSlots()
{
    /* 从设备初始化界面 */
    connect(_devAmp, SIGNAL(initConfigFromDevice()), this, SLOT(displayConfigValue()));

    /* Gain\Mute\Phase\Delay */
    connect(ui->AIn1_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AIn2_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->NIn1_Channel_Gain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));

    connect(ui->AIn1_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AIn2_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->NIn1_Channel_Mute, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));

    connect(ui->AOut_Gain_1, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AOut_Gain_2, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AOut_Gain_3, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AOut_Gain_4, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AOut_Gain_5, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AOut_Gain_6, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
#ifndef Logo_BeiJingHuiSheng
    connect(ui->AOut_Gain_7, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->AOut_Gain_8, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
#endif

    connect(ui->AOut_Mute_1, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AOut_Mute_2, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AOut_Mute_3, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AOut_Mute_4, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AOut_Mute_5, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AOut_Mute_6, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
#ifndef Logo_BeiJingHuiSheng
    connect(ui->AOut_Mute_7, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
    connect(ui->AOut_Mute_8, SIGNAL(clicked(bool)), this, SLOT(setMute(bool)));
#endif

    connect(ui->AOut_Phase_1, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AOut_Phase_2, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AOut_Phase_3, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AOut_Phase_4, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AOut_Phase_5, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AOut_Phase_6, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
#ifndef Logo_BeiJingHuiSheng
    connect(ui->AOut_Phase_7, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
    connect(ui->AOut_Phase_8, SIGNAL(clicked(bool)), this, SLOT(setPhase(bool)));
#endif

    connect(ui->AOut_Delay_1, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    connect(ui->AOut_Delay_2, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    connect(ui->AOut_Delay_3, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    connect(ui->AOut_Delay_4, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    connect(ui->AOut_Delay_5, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    connect(ui->AOut_Delay_6, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
#ifndef Logo_BeiJingHuiSheng
    connect(ui->AOut_Delay_7, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    connect(ui->AOut_Delay_8, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
#endif

    /* Eq */
    connect(ui->AIn1_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AIn2_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->NIn1_Channel_Eq, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AOut_Eq_1, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AOut_Eq_2, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AOut_Eq_3, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AOut_Eq_4, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AOut_Eq_5, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AOut_Eq_6, SIGNAL(clicked()), this, SLOT(setEq()));
#ifndef Logo_BeiJingHuiSheng
    connect(ui->AOut_Eq_7, SIGNAL(clicked()), this, SLOT(setEq()));
    connect(ui->AOut_Eq_8, SIGNAL(clicked()), this, SLOT(setEq()));
#endif

    /* Dyn */
    connect(ui->AOut_Dyn_1, SIGNAL(clicked()), this, SLOT(setDyn()));
    connect(ui->AOut_Dyn_2, SIGNAL(clicked()), this, SLOT(setDyn()));
    connect(ui->AOut_Dyn_3, SIGNAL(clicked()), this, SLOT(setDyn()));
    connect(ui->AOut_Dyn_4, SIGNAL(clicked()), this, SLOT(setDyn()));
    connect(ui->AOut_Dyn_5, SIGNAL(clicked()), this, SLOT(setDyn()));
    connect(ui->AOut_Dyn_6, SIGNAL(clicked()), this, SLOT(setDyn()));
#ifndef Logo_BeiJingHuiSheng
    connect(ui->AOut_Dyn_7, SIGNAL(clicked()), this, SLOT(setDyn()));
    connect(ui->AOut_Dyn_8, SIGNAL(clicked()), this, SLOT(setDyn()));
#endif

    /* XOVER */
    connect(ui->NOut_xover_1, SIGNAL(clicked()), this, SLOT(setXOver()));
    connect(ui->AOut_xover_1, SIGNAL(clicked()), this, SLOT(setXOver()));
    connect(ui->AOut_xover_2, SIGNAL(clicked()), this, SLOT(setXOver()));
    connect(ui->AOut_xover_3, SIGNAL(clicked()), this, SLOT(setXOver()));
    connect(ui->AOut_xover_4, SIGNAL(clicked()), this, SLOT(setXOver()));
    connect(ui->AOut_xover_5, SIGNAL(clicked()), this, SLOT(setXOver()));
    connect(ui->AOut_xover_6, SIGNAL(clicked()), this, SLOT(setXOver()));
#ifndef Logo_BeiJingHuiSheng
    connect(ui->AOut_xover_7, SIGNAL(clicked()), this, SLOT(setXOver()));
    connect(ui->AOut_xover_8, SIGNAL(clicked()), this, SLOT(setXOver()));
#endif

    /* Route */
#ifdef Logo_BeiJingHuiSheng
    connect(ui->com_routeMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setRouteMode(int)));
#else
    connect(ui->AOut_Route, SIGNAL(clicked()), this, SLOT(setAOutRoute()));
#endif

    /* Directivity Angle */
    connect(ui->Directivity_Angle, SIGNAL(currentIndexChanged(int)), this, SLOT(setDirectivityAngle(int)));
}

void DevSetting_RT1052_AP400::displayConfigValue()
{
    ConfigData_RT1052_AP400& cfgData = _devAmp->getConfigData();

    ui->AIn1_Channel_Gain->setValue(cfgData.channel_gain[0].vol);
    ui->AIn2_Channel_Gain->setValue(cfgData.channel_gain[1].vol);
    ui->NIn1_Channel_Gain->setValue(cfgData.channel_gain[2].vol);

    ui->AIn1_Channel_Mute->setChecked(cfgData.channel_gain[0].mute);
    ui->AIn2_Channel_Mute->setChecked(cfgData.channel_gain[1].mute);
    ui->NIn1_Channel_Mute->setChecked(cfgData.channel_gain[2].mute);

    ui->AOut_Gain_1->setValue(cfgData.Aout_gain[0].vol);
    ui->AOut_Gain_2->setValue(cfgData.Aout_gain[1].vol);
    ui->AOut_Gain_3->setValue(cfgData.Aout_gain[2].vol);
    ui->AOut_Gain_4->setValue(cfgData.Aout_gain[3].vol);
    ui->AOut_Gain_5->setValue(cfgData.Aout_gain[4].vol);
    ui->AOut_Gain_6->setValue(cfgData.Aout_gain[5].vol);
#ifndef Logo_BeiJingHuiSheng
    ui->AOut_Gain_7->setValue(cfgData.Aout_gain[6].vol);
    ui->AOut_Gain_8->setValue(cfgData.Aout_gain[7].vol);
#endif

    ui->AOut_Mute_1->setChecked(cfgData.Aout_gain[0].mute);
    ui->AOut_Mute_2->setChecked(cfgData.Aout_gain[1].mute);
    ui->AOut_Mute_3->setChecked(cfgData.Aout_gain[2].mute);
    ui->AOut_Mute_4->setChecked(cfgData.Aout_gain[3].mute);
    ui->AOut_Mute_5->setChecked(cfgData.Aout_gain[4].mute);
    ui->AOut_Mute_6->setChecked(cfgData.Aout_gain[5].mute);
#ifndef Logo_BeiJingHuiSheng
    ui->AOut_Mute_7->setChecked(cfgData.Aout_gain[6].mute);
    ui->AOut_Mute_8->setChecked(cfgData.Aout_gain[7].mute);
#endif

    ui->AOut_Phase_1->setChecked(cfgData.Aout_gain[0].phase);
    ui->AOut_Phase_2->setChecked(cfgData.Aout_gain[1].phase);
    ui->AOut_Phase_3->setChecked(cfgData.Aout_gain[2].phase);
    ui->AOut_Phase_4->setChecked(cfgData.Aout_gain[3].phase);
    ui->AOut_Phase_5->setChecked(cfgData.Aout_gain[4].phase);
    ui->AOut_Phase_6->setChecked(cfgData.Aout_gain[5].phase);
#ifndef Logo_BeiJingHuiSheng
    ui->AOut_Phase_7->setChecked(cfgData.Aout_gain[6].phase);
    ui->AOut_Phase_8->setChecked(cfgData.Aout_gain[7].phase);
#endif

    ui->AOut_Delay_1->setValue(cfgData.Aout_delay[0].delay);
    ui->AOut_Delay_2->setValue(cfgData.Aout_delay[1].delay);
    ui->AOut_Delay_3->setValue(cfgData.Aout_delay[2].delay);
    ui->AOut_Delay_4->setValue(cfgData.Aout_delay[3].delay);
    ui->AOut_Delay_5->setValue(cfgData.Aout_delay[4].delay);
    ui->AOut_Delay_6->setValue(cfgData.Aout_delay[5].delay);
#ifndef Logo_BeiJingHuiSheng
    ui->AOut_Delay_7->setValue(cfgData.Aout_delay[6].delay);
    ui->AOut_Delay_8->setValue(cfgData.Aout_delay[7].delay);
#endif
    ui->label_delay_1->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[0].delay / 48.0, 'f', 3)));
    ui->label_delay_2->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[1].delay / 48.0, 'f', 3)));
    ui->label_delay_3->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[2].delay / 48.0, 'f', 3)));
    ui->label_delay_4->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[3].delay / 48.0, 'f', 3)));
    ui->label_delay_5->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[4].delay / 48.0, 'f', 3)));
    ui->label_delay_6->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[5].delay / 48.0, 'f', 3)));
#ifndef Logo_BeiJingHuiSheng
    ui->label_delay_7->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[6].delay / 48.0, 'f', 3)));
    ui->label_delay_8->setText(tr("%1ms").arg(QString::number(cfgData.Aout_delay[7].delay / 48.0, 'f', 3)));
#endif

    quint16 remain = MAX_DELAY_TIME
             - ui->AOut_Delay_1->value()
             - ui->AOut_Delay_2->value()
             - ui->AOut_Delay_3->value()
             - ui->AOut_Delay_4->value()
             - ui->AOut_Delay_5->value()
             - ui->AOut_Delay_6->value()
#ifndef Logo_BeiJingHuiSheng
             - ui->AOut_Delay_7->value()
             - ui->AOut_Delay_8->value()
#endif
            ;//最大延时为240ms,调整为一个样值一调整
    ui->AOut_Delay_1->setRange(0, ui->AOut_Delay_1->value() + remain);
    ui->AOut_Delay_2->setRange(0, ui->AOut_Delay_2->value() + remain);
    ui->AOut_Delay_3->setRange(0, ui->AOut_Delay_3->value() + remain);
    ui->AOut_Delay_4->setRange(0, ui->AOut_Delay_4->value() + remain);
    ui->AOut_Delay_5->setRange(0, ui->AOut_Delay_5->value() + remain);
    ui->AOut_Delay_6->setRange(0, ui->AOut_Delay_6->value() + remain);
#ifndef Logo_BeiJingHuiSheng
    ui->AOut_Delay_7->setRange(0, ui->AOut_Delay_7->value() + remain);
    ui->AOut_Delay_8->setRange(0, ui->AOut_Delay_8->value() + remain);
#endif

    ui->Directivity_Angle->setCurrentIndex(4-cfgData.directivity);
#ifdef Logo_BeiJingHuiSheng
    ui->com_routeMode->setCurrentIndex(cfgData.route_mode);
    ui->AOut_Route->setVisible(!cfgData.route_mode);
    ui->StopThreshold->setVisible(cfgData.route_mode);
    ui->com_PRISelect->setVisible(cfgData.route_mode);
    ui->ck_autorecovery->setVisible(cfgData.route_mode);
    ui->Reset->setVisible(cfgData.route_mode);
    ui->Reset->setDisabled(cfgData.auto_recovery);
    ui->StopThreshold->setValue(cfgData.threshold);
    ui->com_PRISelect->setCurrentIndex(cfgData.default_route);
    ui->ck_autorecovery->setChecked(cfgData.auto_recovery);

//    qDebug() << __FUNCTION__ << __LINE__ << cfgData.threshold << cfgData.default_route << cfgData.auto_recovery;
    if (cfgData.route_mode)
    {
        disconnect(ui->AOut_Route, SIGNAL(clicked()), this, SLOT(setAOutRoute()));
        connect(ui->com_PRISelect, SIGNAL(currentIndexChanged(int)), this, SLOT(setPriSelect(int)));
        connect(ui->StopThreshold, SIGNAL(valueChanged(int)), this, SLOT(setThreshold(int)));
        connect(ui->ck_autorecovery, SIGNAL(clicked(bool)), this, SLOT(setAutoRecovery(bool)));
        connect(ui->Reset, SIGNAL(clicked()), this, SLOT(setAOutRoute()));
    } else {
        connect(ui->AOut_Route, SIGNAL(clicked()), this, SLOT(setAOutRoute()));
        disconnect(ui->com_PRISelect, SIGNAL(currentIndexChanged(int)), this, SLOT(setPriSelect(int)));
        disconnect(ui->StopThreshold, SIGNAL(valueChanged(int)), this, SLOT(setThreshold(int)));
        disconnect(ui->ck_autorecovery, SIGNAL(clicked(bool)), this, SLOT(setAutoRecovery(bool)));
        disconnect(ui->Reset, SIGNAL(clicked()), this, SLOT(setAOutRoute()));
    }
#endif
}

/* Gain\Mute\Phase\Delay */
void DevSetting_RT1052_AP400::setGain(int value)
{
    if (sender() == ui->AIn1_Channel_Gain)
    {
        _devAmp->sendVolume(0, 0, value);
    }
    else if (sender() == ui->AIn2_Channel_Gain)
    {
        _devAmp->sendVolume(0, 1, value);
    }
    else if (sender() == ui->NIn1_Channel_Gain)
    {
        _devAmp->sendVolume(0, 2, value);
    }
    else if (sender() == ui->AOut_Gain_1)
    {
        _devAmp->sendVolume(1, 0, value);
    }
    else if (sender() == ui->AOut_Gain_2)
    {
        _devAmp->sendVolume(1, 1, value);
    }
    else if (sender() == ui->AOut_Gain_3)
    {
        _devAmp->sendVolume(1, 2, value);
    }
    else if (sender() == ui->AOut_Gain_4)
    {
        _devAmp->sendVolume(1, 3, value);
    }
    else if (sender() == ui->AOut_Gain_5)
    {
        _devAmp->sendVolume(1, 4, value);
    }
    else if (sender() == ui->AOut_Gain_6)
    {
        _devAmp->sendVolume(1, 5, value);
    }
#ifndef Logo_BeiJingHuiSheng
    else if (sender() == ui->AOut_Gain_7)
    {
        _devAmp->sendVolume(1, 6, value);
    }
    else if (sender() == ui->AOut_Gain_8)
    {
        _devAmp->sendVolume(1, 7, value);
    }
#endif
}

void DevSetting_RT1052_AP400::setMute(bool enable)
{
    if (sender() == ui->AIn1_Channel_Mute)
    {
        _devAmp->sendMute(0, 0, enable);
    }
    else if (sender() == ui->AIn2_Channel_Mute)
    {
        _devAmp->sendMute(0, 1, enable);
    }
    else if (sender() == ui->NIn1_Channel_Mute)
    {
        _devAmp->sendMute(0, 2, enable);
    }
    else if (sender() == ui->AOut_Mute_1)
    {
        _devAmp->sendMute(1, 0, enable);
    }
    else if (sender() == ui->AOut_Mute_2)
    {
        _devAmp->sendMute(1, 1, enable);
    }
    else if (sender() == ui->AOut_Mute_3)
    {
        _devAmp->sendMute(1, 2, enable);
    }
    else if (sender() == ui->AOut_Mute_4)
    {
        _devAmp->sendMute(1, 3, enable);
    }
    else if (sender() == ui->AOut_Mute_5)
    {
        _devAmp->sendMute(1, 4, enable);
    }
    else if (sender() == ui->AOut_Mute_6)
    {
        _devAmp->sendMute(1, 5, enable);
    }
#ifndef Logo_BeiJingHuiSheng
    else if (sender() == ui->AOut_Mute_7)
    {
        _devAmp->sendMute(1, 6, enable);
    }
    else if (sender() == ui->AOut_Mute_8)
    {
        _devAmp->sendMute(1, 7, enable);
    }
#endif
}

void DevSetting_RT1052_AP400::setPhase(bool enable)
{
    if (sender() == ui->AOut_Phase_1)
    {
        _devAmp->sendPhase(0, enable);
    }
    else if (sender() == ui->AOut_Phase_2)
    {
        _devAmp->sendPhase(1, enable);
    }
    else if (sender() == ui->AOut_Phase_3)
    {
        _devAmp->sendPhase(2, enable);
    }
    else if (sender() == ui->AOut_Phase_4)
    {
        _devAmp->sendPhase(3, enable);
    }
    else if (sender() == ui->AOut_Phase_5)
    {
        _devAmp->sendPhase(4, enable);
    }
    else if (sender() == ui->AOut_Phase_6)
    {
        _devAmp->sendPhase(5, enable);
    }
#ifndef Logo_BeiJingHuiSheng
    else if (sender() == ui->AOut_Phase_7)
    {
        _devAmp->sendPhase(6, enable);
    }
    else if (sender() == ui->AOut_Phase_8)
    {
        _devAmp->sendPhase(7, enable);
    }
#endif
}

void DevSetting_RT1052_AP400::setDelay(int value)
{
    quint16 remain = MAX_DELAY_TIME
             - ui->AOut_Delay_1->value()
             - ui->AOut_Delay_2->value()
             - ui->AOut_Delay_3->value()
             - ui->AOut_Delay_4->value()
             - ui->AOut_Delay_5->value()
             - ui->AOut_Delay_6->value()
#ifndef Logo_BeiJingHuiSheng
             - ui->AOut_Delay_7->value()
             - ui->AOut_Delay_8->value()
#endif
            ;//最大延时为240ms
    ui->AOut_Delay_1->setRange(0, ui->AOut_Delay_1->value() + remain);
    ui->AOut_Delay_2->setRange(0, ui->AOut_Delay_2->value() + remain);
    ui->AOut_Delay_3->setRange(0, ui->AOut_Delay_3->value() + remain);
    ui->AOut_Delay_4->setRange(0, ui->AOut_Delay_4->value() + remain);
    ui->AOut_Delay_5->setRange(0, ui->AOut_Delay_5->value() + remain);
    ui->AOut_Delay_6->setRange(0, ui->AOut_Delay_6->value() + remain);
#ifndef Logo_BeiJingHuiSheng
    ui->AOut_Delay_7->setRange(0, ui->AOut_Delay_7->value() + remain);
    ui->AOut_Delay_8->setRange(0, ui->AOut_Delay_8->value() + remain);
#endif

    if (sender() == ui->AOut_Delay_1)
    {
        _devAmp->sendDelay(0, value);
        ui->label_delay_1->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
    else if (sender() == ui->AOut_Delay_2)
    {
        _devAmp->sendDelay(1, value);
        ui->label_delay_2->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
    else if (sender() == ui->AOut_Delay_3)
    {
        _devAmp->sendDelay(2, value);
        ui->label_delay_3->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
    else if (sender() == ui->AOut_Delay_4)
    {
        _devAmp->sendDelay(3, value);
        ui->label_delay_4->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
    else if (sender() == ui->AOut_Delay_5)
    {
        _devAmp->sendDelay(4, value);
        ui->label_delay_5->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
    else if (sender() == ui->AOut_Delay_6)
    {
        _devAmp->sendDelay(5, value);
        ui->label_delay_6->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
#ifndef Logo_BeiJingHuiSheng
    else if (sender() == ui->AOut_Delay_7)
    {
        _devAmp->sendDelay(6, value);
        ui->label_delay_7->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
    else if (sender() == ui->AOut_Delay_8)
    {
        _devAmp->sendDelay(7, value);
        ui->label_delay_8->setText(tr("%1ms").arg(QString::number(value / 48.0, 'f', 3)));
    }
#endif
}

/* Eq */
void DevSetting_RT1052_AP400::setEq()
{
    int channel;
    EqData4B eq4B;
    EqData15B eq15B;
    uint8_t type = 0;

    if (sender() == ui->AIn1_Channel_Eq)
    {
        type = 0;
        channel = 0;
        eq4B = _devAmp->getConfigData().channel_eq[channel];
    }
    else if (sender() == ui->AIn2_Channel_Eq)
    {
        type = 0;
        channel = 1;
        eq4B = _devAmp->getConfigData().channel_eq[channel];
    }
    else if (sender() == ui->NIn1_Channel_Eq)
    {
        type = 0;
        channel = 2;
        eq4B = _devAmp->getConfigData().channel_eq[channel];
    }
    else if (sender() == ui->AOut_Eq_1)
    {
        type = 1;
        channel = 0;
        eq15B = _devAmp->getConfigData().Aout_eq[channel];
    }
    else if (sender() == ui->AOut_Eq_2)
    {
        type = 1;
        channel = 1;
        eq15B = _devAmp->getConfigData().Aout_eq[channel];
    }
    else if (sender() == ui->AOut_Eq_3)
    {
        type = 1;
        channel = 2;
        eq15B = _devAmp->getConfigData().Aout_eq[channel];
    }
    else if (sender() == ui->AOut_Eq_4)
    {
        type = 1;
        channel = 3;
        eq15B = _devAmp->getConfigData().Aout_eq[channel];
    }
    else if (sender() == ui->AOut_Eq_5)
    {
        type = 1;
        channel = 4;
        eq15B = _devAmp->getConfigData().Aout_eq[channel];
    }
    else if (sender() == ui->AOut_Eq_6)
    {
        type = 1;
        channel = 5;
        eq15B = _devAmp->getConfigData().Aout_eq[channel];
    }
#ifndef Logo_BeiJingHuiSheng
    else if (sender() == ui->AOut_Eq_7)
    {
        type = 1;
        channel = 6;
        eq15B = _devAmp->getConfigData().Aout_eq[channel];
    }
    else if (sender() == ui->AOut_Eq_8)
    {
        type = 1;
        channel = 7;
        eq15B = _devAmp->getConfigData().Aout_eq[channel];
    }
#endif

    DevSettingDlg dlg;
    EqView_AllBand *eqView;
    if (type == 0)
    {
        eqView = new EqView_AllBand(4, &dlg);
        eqView->showWidget(channel, QString(), eq4B.bypass, eq4B.eq);
//        eqView->setFixedSize(600,350);
    } else {
        eqView = new EqView_AllBand(15, &dlg);
        eqView->showWidget(channel, QString(), eq15B.bypass, eq15B.eq);
    }

    dlg.resize(eqView->size());
    dlg.setFixedSize(eqView->width(),eqView->height());
    if (type == 0)
    {
        connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onChannelEqChanged(quint8, bool, EqData*)));
    } else if (type == 1) {
        connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
    }
    dlg.exec();
}

/* 处理函数 */
void DevSetting_RT1052_AP400::onChannelEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    _devAmp->sendEq(0, ch, bypass, eqPara);
}

void DevSetting_RT1052_AP400::onEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    _devAmp->sendEq(1, ch, bypass, eqPara);
}

/* Dyn */
void DevSetting_RT1052_AP400::setDyn()
{
    int channel;
    DynData dynData;
    if (sender() == ui->AOut_Dyn_1)
    {
        channel = 0;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }
    else if(sender() == ui->AOut_Dyn_2)
    {
        channel = 1;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }
    else if(sender() == ui->AOut_Dyn_3)
    {
        channel = 2;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }
    else if(sender() == ui->AOut_Dyn_4)
    {
        channel = 3;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }
    else if(sender() == ui->AOut_Dyn_5)
    {
        channel = 4;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }
    else if(sender() == ui->AOut_Dyn_6)
    {
        channel = 5;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }
#ifndef Logo_BeiJingHuiSheng
    else if(sender() == ui->AOut_Dyn_7)
    {
        channel = 6;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }
    else if(sender() == ui->AOut_Dyn_8)
    {
        channel = 7;
        dynData = _devAmp->getConfigData().Aout_dyn[channel];
    }
#endif

    DevSettingDlg dlg;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
    dynWidget->setDisabled(_devAmp->isFactoryMode());//是否禁用小部件
    dynWidget->showWidget(false, channel, QString(), dynData);
    dlg.resize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

/* 处理函数 */
void DevSetting_RT1052_AP400::onDynChanged(quint8 ch, bool isIn ,DynData dynData)
{
    _devAmp->sendDyn(ch, dynData);
}

/* XOVER */
void DevSetting_RT1052_AP400::setXOver()
{
    quint8 ch = 0;
    if (sender() == ui->AOut_xover_1)
    {
        ch = 0;
    }
    else if (sender() == ui->AOut_xover_2)
    {
        ch = 1;
    }
    else if (sender() == ui->AOut_xover_3)
    {
        ch = 2;
    }
    else if (sender() == ui->AOut_xover_4)
    {
        ch = 3;
    }
    else if (sender() == ui->AOut_xover_5)
    {
        ch = 4;
    }
    else if (sender() == ui->AOut_xover_6)
    {
        ch = 5;
    }
#ifndef Logo_BeiJingHuiSheng
    else if (sender() == ui->AOut_xover_7)
    {
        ch = 6;
    }
    else if (sender() == ui->AOut_xover_8)
    {
        ch = 7;
    }
#endif
    else if (sender() == ui->NOut_xover_1)
    {
        ch = 8;
    }

    if (ch < 8)
    {
        DevSettingBase_Xover xWidget(ch, DevSettingBase_Xover::DevSettingBase_Xover_BandPass, _devAmp->getConfigData().Aout_xover[ch]);
        connect(&xWidget, SIGNAL(valueChanged(uint8_t, struct ConfigData_Xover)), this, SLOT(onXoverChanged(uint8_t,ConfigData_Xover)));
        xWidget.exec();
    } else {
        DevSettingBase_Xover xWidget(ch, DevSettingBase_Xover::DevSettingBase_Xover_BandPass, _devAmp->getConfigData().Nout_xover);
        connect(&xWidget, SIGNAL(valueChanged(uint8_t, struct ConfigData_Xover)), this, SLOT(onXoverChanged(uint8_t,ConfigData_Xover)));
        xWidget.exec();
    }
}

void DevSetting_RT1052_AP400::onXoverChanged(uint8_t ch, ConfigData_Xover xData)
{
    _devAmp->sendXover(ch, xData.bypass, xData.freq[0], xData.freq[1], xData.stages[0], xData.stages[1]);
}

/* AOut Route */
void DevSetting_RT1052_AP400::setAOutRoute()
{
    if (sender() == ui->AOut_Route)
    {
        Base_Route_AP400 route(_devAmp);
        route.exec();
    }
#ifdef Logo_BeiJingHuiSheng
    else if (sender() == ui->Reset)
    {
        _devAmp->sendRoute(0, ui->com_PRISelect->currentIndex());
    }
#endif
}

#ifdef Logo_BeiJingHuiSheng
void DevSetting_RT1052_AP400::setRouteMode(int v)
{
    ui->AOut_Route->setVisible(!v);
    ui->StopThreshold->setVisible(v);
    ui->com_PRISelect->setVisible(v);
    ui->ck_autorecovery->setVisible(v);
    ui->Reset->setVisible(v);

    ConfigData_RT1052_AP400& cfgData = _devAmp->getConfigData();
    ui->StopThreshold->setValue(cfgData.threshold);
    ui->com_PRISelect->setCurrentIndex(cfgData.default_route);
    ui->ck_autorecovery->setChecked(cfgData.auto_recovery);
    ui->Reset->setDisabled(cfgData.auto_recovery);

    _devAmp->sendRouteMode(v);

    if (v)
    {
        disconnect(ui->AOut_Route, SIGNAL(clicked()), this, SLOT(setAOutRoute()));
        connect(ui->com_PRISelect, SIGNAL(currentIndexChanged(int)), this, SLOT(setPriSelect(int)));
        connect(ui->StopThreshold, SIGNAL(valueChanged(int)), this, SLOT(setThreshold(int)));
        connect(ui->ck_autorecovery, SIGNAL(clicked(bool)), this, SLOT(setAutoRecovery(bool)));
        connect(ui->Reset, SIGNAL(clicked()), this, SLOT(setAOutRoute()));
    } else {
        connect(ui->AOut_Route, SIGNAL(clicked()), this, SLOT(setAOutRoute()));
        disconnect(ui->com_PRISelect, SIGNAL(currentIndexChanged(int)), this, SLOT(setPriSelect(int)));
        disconnect(ui->StopThreshold, SIGNAL(valueChanged(int)), this, SLOT(setThreshold(int)));
        disconnect(ui->ck_autorecovery, SIGNAL(clicked(bool)), this, SLOT(setAutoRecovery(bool)));
        disconnect(ui->Reset, SIGNAL(clicked()), this, SLOT(setAOutRoute()));
    }
}

void DevSetting_RT1052_AP400::setPriSelect(int v)
{
    if (sender() == ui->com_PRISelect)
    {
        _devAmp->sendDefaultOutput(v);
    }
}

void DevSetting_RT1052_AP400::setThreshold(int v)
{
    if (sender() == ui->StopThreshold)
    {
        _devAmp->sendThreshold(v);
    }
}

void DevSetting_RT1052_AP400::setAutoRecovery(bool v)
{
    if (sender() == ui->ck_autorecovery)
    {
        _devAmp->sendAutoRecovery(v);
        ui->Reset->setDisabled(v);
    }
}
#endif

void DevSetting_RT1052_AP400::setDirectivityAngle(int cur)
{
    _devAmp->sendDirectivityAngle(4 - cur);
}

void DevSetting_RT1052_AP400::DisplaySlot(struct MsgData_RT1052_Poe b)
{
    ui->Meter_AIn_1->setLCurValue(b._monitor._AP400.meter_In[0]);
    ui->Meter_AIn_2->setLCurValue(b._monitor._AP400.meter_In[2]);
    ui->Meter_AIn_3->setLCurValue(b._monitor._AP400.meter_In[1]);

    ui->Meter_AOut_1->setLCurValue(b._monitor._AP400.meter_Out[0]);
    ui->Meter_AOut_2->setLCurValue(b._monitor._AP400.meter_Out[1]);
    ui->Meter_AOut_3->setLCurValue(b._monitor._AP400.meter_Out[2]);
    ui->Meter_AOut_4->setLCurValue(b._monitor._AP400.meter_Out[3]);
    ui->Meter_AOut_5->setLCurValue(b._monitor._AP400.meter_Out[4]);
    ui->Meter_AOut_6->setLCurValue(b._monitor._AP400.meter_Out[5]);
#ifndef Logo_BeiJingHuiSheng
    ui->Meter_AOut_7->setLCurValue(b._monitor._AP400.meter_Out[6]);
    ui->Meter_AOut_8->setLCurValue(b._monitor._AP400.meter_Out[7]);
#endif
    ui->Meter_AOut_9->setLCurValue(b._monitor._AP400.meter_Out[8]);

    ui->label_Temp->setText(QString::number(b._monitor._AP400.temperature));
    ui->label_Volt->setText(QString::number(b._monitor._AP400.AD1_Value, 'f', 1));

#ifdef Logo_BeiJingHuiSheng
    ui->com_current_src->setCurrentIndex(b._monitor._AP400.current_route_src);
#endif
}

//// 另存场景为文件
//void DevSetting_RT1052_AP400::on_saveAsBtn_clicked()
//{
//    QString fileName = QFileDialog::getSaveFileName(this, tr("save as..."), QDir::currentPath(), QString("file (*.amp)"));
//    if(!fileName.isEmpty()){
//        _devAmp->serialize(fileName);
//    }
//}

//// 加载场景文件
//void DevSetting_RT1052_AP400::on_loadBtn_clicked()
//{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("file (*.amp)"));
//    if(!fileName.isEmpty())
//    {
//        _devAmp->deserialize(fileName);
//        displayConfigValue();
//        _devAmp->sendAllConfigPack(true);
//    }
//}

void DevSetting_RT1052_AP400::on_save_clicked()
{
    _devAmp->sendSave();
}
