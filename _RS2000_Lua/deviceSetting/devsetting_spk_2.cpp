#include "stable.h"
#include "devsetting_spk_2.h"
#include "ui_devsetting_spk.h"
#include "deviceItem/deviceitem_spk.h"
#include "device/device_spk_2.h"
#include "trn/eqview_allband.h"
#include "customWidget/eqviewnormal.h"
#include "customWidget/filterview.h"
#include "spk/spk_alarmrecord.h"
#include "spk/spk_amppf.h"
#include "spk/spk_gainset.h"
#include "spk/spk_router.h"
#include "trn/trn_dynwidget.h"
#include "alarmconfig.h"

//wj
#include "monitor/monitor_spk.h"

// 构造函数
DevSetting_Spk_2::DevSetting_Spk_2(DeviceItem_Spk *devItem, QWidget *parent)
    :QDialog(parent),
    _ui(new Ui::DevSetting_Spk)
{
    _ui->setupUi(this);
    _devItem = devItem;
    Q_ASSERT(devItem->getDanteDevice());
    _devSpk =qobject_cast<Device_Spk_2*>(devItem->getDanteDevice());

//    _devSpk->sendInitPack();
    connect(_devSpk, SIGNAL(adcDataChanged(AlarmConfig*)), this, SLOT(onAdcDataChanged(AlarmConfig*)));
    setAttribute(Qt::WA_QuitOnClose, true);

    _hasData = false;
    _timer = new QTimer;
    connect(_timer, SIGNAL(timeout()), this, SLOT(closeSelf()));
    _timer->start(3000);

    _ui->deviceName->setText(_devItem->customName());
    _ui->IPAddr->setText(_devSpk->getDeviceIp());
    _ui->Mac_addr->setText(_devSpk->getMac());

    _syncing = false;
    setFixedSize(size());
    displayConfigData();

    connectSigSlots();
    resetVoltCurrLimit(_devSpk->getConfigData().volumeCurrentType);

    factoryModeReInit();
    loadAlarmSetting();
}

void DevSetting_Spk_2::closeSelf()
{
    if (!_hasData)
    {
        close();
    }

    _hasData = false;
}

DevSetting_Spk_2::~DevSetting_Spk_2()
{
    saveAlarmSetting();

    delete _ui;
}

void DevSetting_Spk_2::keyPressEvent(QKeyEvent *e)
{
  switch (e->key()) {
  case Qt::Key_Enter:
      break;
  case Qt::Key_F5:
      if (e->modifiers() == (Qt::ControlModifier | Qt::AltModifier | Qt::ShiftModifier))
      {
          _devSpk->changeFactoryMode();
          factoryModeReInit();
      }
  default:
      break;
  }
}

void DevSetting_Spk_2::factoryModeReInit()
{
  if (_devSpk->isFactoryMode())
  {
      _ui->pushButton60v->hide();
      _ui->pushButton100v->hide();
  }
  else {
      _ui->pushButton60v->show();
      _ui->pushButton100v->show();
  }

  _ui->delaySlider->setDisabled(_devSpk->isFactoryMode());
  _ui->delayValue->setDisabled(_devSpk->isFactoryMode());
  _ui->HFGainSlider->setDisabled(_devSpk->isFactoryMode());
  _ui->HFGainValue->setDisabled(_devSpk->isFactoryMode());
  _ui->LFGainSlider->setDisabled(_devSpk->isFactoryMode());
  _ui->LFGainValue->setDisabled(_devSpk->isFactoryMode());
}

// 显示设备发来的电压电流温度等数据
void DevSetting_Spk_2::onAdcDataChanged(AlarmConfig *alarmCfg)
{
    if(_devSpk->isPowerOn())
    {
        double volt1Ch = alarmCfg->getParaValue(Volt1Ch);
        double curr1Ch = alarmCfg->getParaValue(Curr1Ch);
        double temperature = alarmCfg->getParaValue(Temperature);
        double volumeL = alarmCfg->getParaValue(VolumeL);
        double volumeR = alarmCfg->getParaValue(VolumeR);

        _ui->voltDisplay->setLCurValue(volt1Ch);
        _ui->currDisplay->setLCurValue(curr1Ch);
        _ui->tempDisplay->setLCurValue(temperature);
        _ui->voltValue->setText(QString::number(volt1Ch, 'f', 1));
        _ui->currValue->setText(QString::number(curr1Ch, 'f', 1));
        _ui->tempValue->setText(QString::number(temperature, 'f', 1));
        _ui->voluDisp->setLCurValue(volumeL);
        _ui->voluDisp->setRCurValue(volumeR);
        _ui->leftVolLabel->setText(QString::number(volumeL, 'f', 1));
        _ui->rightVolLabel->setText(QString::number(volumeR, 'f', 1));
    }
    _hasData = true;
}

void DevSetting_Spk_2::connectSigSlots()
{
    /* 第一路延时选择 */
    _ui->delayValue->setValidator(new QIntValidator(_ui->delaySlider->minimum(), _ui->delaySlider->maximum()));
    connect(_ui->delaySlider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(_ui->delayValue, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* 高低分频输出增益 */
//    _ui->HFGainValue->setValidator(new QDoubleValidator(-1.5, 1.5, 1));
//    _ui->LFGainValue->setValidator(new QDoubleValidator(-1.5, 1.5, 1));
    connect(_ui->HFGainSlider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(_ui->HFGainValue, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));
    connect(_ui->LFGainSlider, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(_ui->LFGainValue, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* 第一路音量选择 */
    _ui->volumeValue_2->setValidator(new QIntValidator(-100, 12));
    connect(_ui->volumeSlider_2, SIGNAL(valueChanged(int)), this, SLOT(setIntValueDisplay(int)));
    connect(_ui->volumeValue_2, SIGNAL(editingFinished()), this, SLOT(setSliderValue()));

    /* 报警相关 */
    connect(_ui->enableAlarm, SIGNAL(toggled(bool)), this, SLOT(setAlarmMode(bool)));

    connect(_ui->voltMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableWidgets(int)));
    connect(_ui->currMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableWidgets(int)));
    connect(_ui->tempMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableWidgets(int)));
    connect(_ui->vuMeterMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableWidgets(int)));

    connect(_ui->voltLowerLimit, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->voltUpperLimit, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->currLowerLimit, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->currUpperLimit, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->tempLowerLimit, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->tempUpperLimit, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->vuMeterLowerLimit, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));
    connect(_ui->vuMeterUpperLimit, SIGNAL(editingFinished()), this, SLOT(setLimiterValue()));

    connect(_ui->saveButton, SIGNAL(clicked()), this, SLOT(save2Device()));

    connect(_devSpk, SIGNAL(initConfigFromDevice()), this, SLOT(displayConfigData()));
}

void DevSetting_Spk_2::setAlarmMode(bool b)
{
    _ui->groupBox->setEnabled(b);
    _devSpk->getAlarmCfg()->setAlarmEnable(b);
}

void DevSetting_Spk_2::on_router_clicked()
{
    SpkRouter_2 router(_devSpk, this);
    router.exec();
}

void DevSetting_Spk_2::on_alarmButton_clicked()
{
    AlarmRecord alarmFrm(_devSpk->getMac());
    alarmFrm.exec();
}

void DevSetting_Spk_2::on_eq_clicked()
{
    EqData4B& eq4B = _devSpk->getConfigData().ch1_Eq;
    QDialog dlg;
    EqView_AllBand *eqView = new EqView_AllBand(4, &dlg);
    eqView->setDisabled(_devSpk->isFactoryMode());
    eqView->showWidget(1, QString(), eq4B.bypass, eq4B.eq);
    eqView->setFixedSize(600,350);
    dlg.setStyleSheet(QLatin1String("QDialog{background-image: url(:/Image/back.png);}"));
    dlg.resize(eqView->size());
    connect(eqView, SIGNAL(eqDataChanged(quint8, bool, EqData*)), this, SLOT(onEqChanged(quint8, bool, EqData*)));
    dlg.exec();
}

void DevSetting_Spk_2::on_dividerHF_clicked()
{
    AmpPf pf(1, &_devSpk->getConfigData().ch1_divider.hf, AmpPfType_SPK);
    pf.setDisabled(_devSpk->isFactoryMode());
    pf.setWindowTitle(tr("HF"));
    connect(&pf, SIGNAL(pfDataChanged(quint8)), this, SLOT(onPfDataChanged(quint8)));
    pf.exec();

}

void DevSetting_Spk_2::on_dividerLF_clicked()
{
    AmpPf pf(1, &_devSpk->getConfigData().ch1_divider.lf, AmpPfType_SPK);
    pf.setDisabled(_devSpk->isFactoryMode());
    pf.setWindowTitle(tr("LF"));
    connect(&pf, SIGNAL(pfDataChanged(quint8)), this, SLOT(onPfDataChanged(quint8)));
    pf.exec();
}

void DevSetting_Spk_2::onPfDataChanged(quint8 channel)
{
    Q_ASSERT(channel == 1);
    Divider& divider = _devSpk->getConfigData().ch1_divider;
    _devSpk->sendDividerPack(channel, divider.hf, divider.lf );
}

void DevSetting_Spk_2::onPfDataChanged(quint8 ch, bool hfb, bool lfb)
{
    Q_ASSERT(ch == 1);

    Divider *divider = NULL;
    divider = &_devSpk->getConfigData().ch1_divider;
    divider->hf.bypass = hfb;
    divider->lf.bypass = lfb;
    _devSpk->sendDividerPack(ch, divider->hf, divider->lf );
}

void DevSetting_Spk_2::on_dyn_clicked()
{
    DynData& dynData = _devSpk->getConfigData().ch1_Dyn;
    QDialog dlg;
    Trn_DynWidget *dynWidget = new Trn_DynWidget(&dlg);
    dynWidget->setDisabled(_devSpk->isFactoryMode());
    dynWidget->showWidget(false, 1, QString(), dynData);
    dlg.setStyleSheet(QLatin1String("QDialog{background-image: url(:/Image/back.png);}"));
    dlg.resize(dynWidget->size());
    connect(dynWidget, SIGNAL(dynChanged(quint8,bool,DynData)), this, SLOT(onDynChanged(quint8,bool,DynData)));
    dlg.exec();
}

void DevSetting_Spk_2::onEqChanged(quint8 ch, bool bypass, EqData *eqPara)
{
    _devSpk->sendEqPack(ch, bypass, eqPara);
}

void DevSetting_Spk_2::onDynChanged(quint8 ch, bool isIn ,DynData dynData)
{
    Q_UNUSED(isIn)
    _devSpk->sendDynPack(ch, dynData);
}

void DevSetting_Spk_2::setLimiterValue()
{
    AlarmConfig *alarmCfg = _devSpk->getAlarmCfg();
    if(sender() == _ui->voltLowerLimit)
    {
        alarmCfg->setMinLimit(Volt1Ch, _ui->voltLowerLimit->value());
    }
    else if(sender() == _ui->voltUpperLimit)
    {
        alarmCfg->setMaxLimit(Volt1Ch, _ui->voltUpperLimit->value());
    }
    else if(sender() == _ui->currLowerLimit)
    {
        alarmCfg->setMinLimit(Curr1Ch, _ui->currLowerLimit->value());
    }
    else if(sender() == _ui->currUpperLimit)
    {
        alarmCfg->setMaxLimit(Curr1Ch, _ui->currUpperLimit->value());
    }
    else if(sender() == _ui->tempLowerLimit)
    {
        alarmCfg->setMinLimit(Temperature, _ui->tempLowerLimit->value());
    }
    else if(sender() == _ui->tempUpperLimit)
    {
        alarmCfg->setMaxLimit(Temperature, _ui->tempUpperLimit->value());
    }
    else if (sender() == _ui->vuMeterLowerLimit)
    {
        alarmCfg->setMinLimit(VolumeL, _ui->vuMeterLowerLimit->value());
    }
    else if (sender() == _ui->vuMeterUpperLimit)
    {
        alarmCfg->setMaxLimit(VolumeL, _ui->vuMeterUpperLimit->value());
    }
}

void DevSetting_Spk_2::setIntValueDisplay(int i)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    ControlValue *cntvlu = &_devSpk->getConfigData().ch1_ControlValue;
    Divider *dv = &_devSpk->getConfigData().ch1_divider;
    if (sender() == _ui->volumeSlider_2)
    {
        _ui->volumeValue_2->setText(QString::number(i));

        _devSpk->sendVolume(i);
    }
    else if(sender() == _ui->delaySlider)
    {
        _ui->delayValue->setText(QString::number(i));
        cntvlu->delay = i * 3;

        _devSpk->sendCommonDataPack();
    }
    else if(sender() == _ui->HFGainSlider)
    {
        _ui->HFGainValue->setText(QString::number(i/10.0));
        dv->hf.gain = (i / 10.0) * 256;

        _devSpk->sendDividerPack(1, dv->hf, dv->lf);
    }
    else if(sender() == _ui->LFGainSlider)
    {
        _ui->LFGainValue->setText(QString::number(i/10.0));
        dv->lf.gain = (i / 10.0) * 256;

        _devSpk->sendDividerPack(1, dv->hf, dv->lf);
    }
}

// 报警模式，disable/enable相应窗口
void DevSetting_Spk_2::setEnableWidgets(int i)
{
    AlarmConfig *alarmCfg = _devSpk->getAlarmCfg();
    if(sender() == _ui->voltMode)
    {
        alarmCfg->setLimitMode(Volt1Ch, i);
        if(i == 0)
        {
            _ui->voltLowerLimit->setDisabled(true);
            _ui->voltUpperLimit->setDisabled(true);
        }
        else if(i == 1)
        {
            _ui->voltLowerLimit->setDisabled(false);
            _ui->voltUpperLimit->setDisabled(false);
        }
        else if(i == 2)
        {
            _ui->voltLowerLimit->setDisabled(false);
            _ui->voltUpperLimit->setDisabled(true);
        }
        else if(i == 3)
        {
            _ui->voltLowerLimit->setDisabled(true);
            _ui->voltUpperLimit->setDisabled(false);
        }
    }
    else if(sender() == _ui->currMode)
    {
        alarmCfg->setLimitMode(Curr1Ch, i);
        if(i == 0)
        {
            _ui->currLowerLimit->setDisabled(true);
            _ui->currUpperLimit->setDisabled(true);
        }
        else if(i == 1)
        {
            _ui->currLowerLimit->setDisabled(false);
            _ui->currUpperLimit->setDisabled(false);
        }
        else if(i == 2)
        {
            _ui->currLowerLimit->setDisabled(false);
            _ui->currUpperLimit->setDisabled(true);
        }
        else if(i == 3)
        {
            _ui->currLowerLimit->setDisabled(true);
            _ui->currUpperLimit->setDisabled(false);
        }
    }
    else if(sender() == _ui->tempMode)
    {
        alarmCfg->setLimitMode(Temperature, i);
        if(i == 0)
        {
            _ui->tempLowerLimit->setDisabled(true);
            _ui->tempUpperLimit->setDisabled(true);
        }
        else if(i == 1)
        {
            _ui->tempLowerLimit->setDisabled(false);
            _ui->tempUpperLimit->setDisabled(false);
        }
        else if(i == 2)
        {
            _ui->tempLowerLimit->setDisabled(false);
            _ui->tempUpperLimit->setDisabled(true);
        }
        else if(i == 3)
        {
            _ui->tempLowerLimit->setDisabled(true);
            _ui->tempUpperLimit->setDisabled(false);
        }
    }
    else if (sender() == _ui->vuMeterMode)
    {
        alarmCfg->setLimitMode(VolumeL, i);
        if(i == 0)
        {
            _ui->vuMeterLowerLimit->setDisabled(true);
            _ui->vuMeterUpperLimit->setDisabled(true);
        }
        else if(i == 1)
        {
            _ui->vuMeterLowerLimit->setDisabled(false);
            _ui->vuMeterUpperLimit->setDisabled(false);
        }
        else if(i == 2)
        {
            _ui->vuMeterLowerLimit->setDisabled(false);
            _ui->vuMeterUpperLimit->setDisabled(true);
        }
        else if(i == 3)
        {
            _ui->vuMeterLowerLimit->setDisabled(true);
            _ui->vuMeterUpperLimit->setDisabled(false);
        }
    }
}

void DevSetting_Spk_2::setSliderValue()
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    float current;
    if (sender() == _ui->volumeValue_2)
    {
        _ui->volumeSlider_2->setValue(_ui->volumeValue_2->text().toInt());
    }
    else if(sender() == _ui->delayValue)
    {
        _ui->delaySlider->setValue(_ui->delayValue->text().toInt());
    }
    else if(sender() == _ui->HFGainValue)
    {
        current = _ui->HFGainValue->text().toFloat();
        if (current > 1.5)
        {
            current = 1.5;
        }
        else if (current < -1.5)
        {
            current = -1.5;
        }
        _ui->HFGainValue->setText(QString::number(current));
        _ui->HFGainSlider->setValue((int)(current*10));
    }
    else if(sender() == _ui->LFGainValue)
    {
        current = _ui->LFGainValue->text().toFloat();
        if (current > 1.5)
        {
            current = 1.5;
        }
        else if (current < -1.5)
        {
            current = -1.5;
        }
        _ui->LFGainValue->setText(QString::number(current));
        _ui->LFGainSlider->setValue((int)(current*10));
    }
}


// 保存到设备
void DevSetting_Spk_2::save2Device()
{
    _devSpk->sendSave2DevicePack();
}

void DevSetting_Spk_2::displayConfigData()
{
    _syncing = true;

    ConfigData_Spk& cfgData = _devSpk->getConfigData();

    _ui->groupBox_2->setEnabled(cfgData.Switch);

    _ui->volumeSlider_2->setValue(cfgData.ch1_ControlValue.volume);
    _ui->volumeValue_2->setText(QString::number(cfgData.ch1_ControlValue.volume));
    _ui->gain1P->setChecked(cfgData.ch1_ControlValue.phase);
    _ui->gain1M->setChecked(cfgData.ch1_ControlValue.mute);

    _ui->delaySlider->setValue(cfgData.ch1_ControlValue.delay / 3);
    _ui->delayValue->setText(QString::number(cfgData.ch1_ControlValue.delay / 3));

    int hgain = qRound(cfgData.ch1_divider.hf.gain*10/256.0);
    _ui->HFGainSlider->setValue(hgain);
    _ui->HFGainValue->setText(QString::number(hgain / 10.0, 'f', 1));
    int lgain = qRound(cfgData.ch1_divider.lf.gain*10/256.0);
    _ui->LFGainSlider->setValue(lgain);
    _ui->LFGainValue->setText(QString::number(lgain / 10.0, 'f', 1));

    _syncing = false;
}

// 加载场景文件
void DevSetting_Spk_2::on_loadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("file (*.spk)"));
    if(!fileName.isEmpty())
    {
        _devSpk->deserialize(fileName);
        displayConfigData();
        _devSpk->sendAllConfigPack(true);
    }

//    /* 在线更新功能 */
//    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), QDir::currentPath(), tr("file (*.bin)"));
//    QFile file(fileName);
//    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
//    QByteArray buff;
//    buff = file.readAll();
//    file.close();

//    QProgressDialog *progressDlg=new QProgressDialog(this);
//    progressDlg->setWindowModality(Qt::WindowModal);
//    progressDlg->setMinimumDuration(1);
//    progressDlg->setWindowTitle(tr("please wait"));
//    progressDlg->setLabelText(tr("Download......"));
//    progressDlg->setCancelButton(0);
//    quint32 totalCount = buff.count();
//    quint32 offset     = 0;
//    quint32 paraLen    = 128;
//    quint32 blk        = totalCount / paraLen;
//    progressDlg->setRange(0,blk);
//    for (quint32 i = 0; i < blk; ++i)
//    {
//        offset = i * 128;
//        _devSpk->sendUpdatePackage(totalCount, offset, paraLen, buff.mid(offset, paraLen));
//        progressDlg->setValue(i);
//        QThread::msleep(50);
//    }
//    progressDlg->setValue(blk);
//    paraLen = totalCount - blk * 128;
//    if (paraLen)
//    {
//        offset = blk * 128;
//        _devSpk->sendUpdatePackage(totalCount, offset, paraLen, buff.mid(offset, paraLen));
//    }
//    delete progressDlg;
}

// 另存场景为文件
void DevSetting_Spk_2::on_saveAsButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("save as..."),
                                               QDir::currentPath(), tr("file (*.spk)"));
    if(!fileName.isEmpty()){
        _devSpk->serialize(fileName);
    }
}

void DevSetting_Spk_2::on_pushButton60v_clicked()
{
    resetVoltCurrLimit(0);
    _devSpk->sendVoltTypePack(0);
}

void DevSetting_Spk_2::on_pushButton100v_clicked()
{
    resetVoltCurrLimit(1);
    _devSpk->sendVoltTypePack(1);
}

//voltType 0:60V  1:100V
void DevSetting_Spk_2::resetVoltCurrLimit(quint8 voltType)
{
    quint8 voltMax = (voltType == 0 ? 60 : 100);
    _devSpk->getAlarmCfg()->setMaxVolt(voltMax);
    _ui->voltLowerLimit->setMaximum(voltMax);
    _ui->voltUpperLimit->setMaximum(voltMax);
    _ui->voltUpperLimit->setValue(voltMax);
    _ui->currLowerLimit->setMaximum(voltMax/4);
    _ui->currUpperLimit->setMaximum(voltMax/4);
    _ui->currUpperLimit->setValue(voltMax/4);

    _ui->voltUpper->setText(QString::number(voltMax));
    _ui->currUpper->setText(QString::number(voltMax/4));

    _ui->voltDisplay->setMaxValue(voltMax);
    _ui->currDisplay->setMaxValue(voltMax/4);
}


void DevSetting_Spk_2::loadAlarmSetting()
{
    AlarmConfig *alarmCfg = _devSpk->getAlarmCfg();

    _ui->enableAlarm->setChecked(alarmCfg->getAlarmEnable());

    LimitDef &volt1chLimit = alarmCfg->getLimitDef(Volt1Ch);
    _ui->voltMode->setCurrentIndex(volt1chLimit.getLimitMode());
    _ui->voltLowerLimit->setValue(volt1chLimit.minValue);
    emit _ui->voltLowerLimit->editingFinished();
    _ui->voltUpperLimit->setValue(volt1chLimit.maxValue);
    emit _ui->voltUpperLimit->editingFinished();

    LimitDef &curr1chLimit = alarmCfg->getLimitDef(Curr1Ch);
    _ui->currMode->setCurrentIndex(curr1chLimit.getLimitMode());
    _ui->currLowerLimit->setValue(curr1chLimit.minValue);
    emit _ui->currLowerLimit->editingFinished();
    _ui->currUpperLimit->setValue(curr1chLimit.maxValue);
    emit _ui->currUpperLimit->editingFinished();

    LimitDef &tempLimit = alarmCfg->getLimitDef(Temperature);
    _ui->tempMode->setCurrentIndex(tempLimit.getLimitMode());
    _ui->tempLowerLimit->setValue(tempLimit.minValue);
    emit _ui->tempLowerLimit->editingFinished();
    _ui->tempUpperLimit->setValue(tempLimit.maxValue);
    emit _ui->tempUpperLimit->editingFinished();

    LimitDef &volumeL = alarmCfg->getLimitDef(VolumeL);
    _ui->vuMeterMode->setCurrentIndex(volumeL.getLimitMode());
    _ui->vuMeterLowerLimit->setValue(volumeL.minValue);
    emit _ui->vuMeterLowerLimit->editingFinished();
    _ui->vuMeterUpperLimit->setValue(volumeL.maxValue);
    emit _ui->vuMeterUpperLimit->editingFinished();
}

void DevSetting_Spk_2::saveAlarmSetting()
{
   AlarmConfig *alarmCfg = _devSpk->getAlarmCfg();
   alarmCfg->saveAlarmSetting();
}

void DevSetting_Spk_2::on_gain1P_clicked(bool checked)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    _devSpk->sendPhase(checked);
}

void DevSetting_Spk_2::on_gain1M_clicked(bool checked)
{
    /* 数据同步时不再次发送 */
    if (_syncing)
    {
        return;
    }

    _devSpk->sendMute(checked);
}
