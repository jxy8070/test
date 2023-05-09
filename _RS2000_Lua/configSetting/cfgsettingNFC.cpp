#include "stable.h"
#include "cfgsettingNFC.h"
#include "ui_cfgsettingNFC.h"
#include "configItem/configitemNFC.h"
#include "ui_CfgSettingNFC_FangTu.h"

CfgSettingNFC::CfgSettingNFC(ConfigItemNFC *cfgItem,  QWidget *parent)
    : QWidget(parent)
{
    _cfgItem = cfgItem;
    _maxFilter = ((_cfgItem->getCfgType() & 0xFF) + 1) * 8;
#if Logo_FangTu
    _ui_FangTu = new Ui::CfgSettingNFC_FangTu;
    _ui_FangTu->setupUi(this);
    initUi_FangTu();
#else
    _uiNFC = new Ui::CfgSettingNFC;
    _uiNFC->setupUi(this);
    initUi();
#endif


    for (int i = 0; i < 32; ++i)
    {
        _howlFreq.at(i)->setText("");
        _NotchDepth.at(i)->setText("");
    }
}

void CfgSettingNFC::setDisp(int idx, int howFreq, int notchDepth)
{
    _howlFreq.at(idx)->setText(QString::number(howFreq));
    _NotchDepth.at(idx)->setText(QString::number(notchDepth));
}

void CfgSettingNFC::updateUiData()
{
#if Logo_FangTu
    _ui_FangTu->outgain->setValue(_cfgItem->getOutGain());
    _ui_FangTu->outgain_label->setText(QString::number(_cfgItem->getOutGain()));
    _ui_FangTu->outmode->setCurrentIndex(_cfgItem->getOutMode());
    _ui_FangTu->pushButton_Bypass->setChecked(_cfgItem->getOutMode() & 0x01);
    _ui_FangTu->pushButton_Mute->setChecked(_cfgItem->getOutMode() & 0x02);

    _ui_FangTu->fixedfilter->setValue(_cfgItem->getFixfilters());
    _ui_FangTu->dynamicfilter->setValue(_cfgItem->getDynfilters());
    //    _ui->notchbandwidth->getDoubleSpinBox()->setValue(_cfgItem->getBandwidth());
    _ui_FangTu->notchbandwidth->setCurrentText(QString("1/%1").arg(_cfgItem->getBandwidth()));
    _ui_FangTu->maxnotchdepth->setValue(_cfgItem->getMaxNotchDepth());
#else
    _uiNFC->outgain->setValue(_cfgItem->getOutGain());
    _uiNFC->outgain_label->setText(QString::number(_cfgItem->getOutGain()));
    _uiNFC->outmode->setCurrentIndex(_cfgItem->getOutMode());
    _uiNFC->pushButton_Bypass->setChecked(_cfgItem->getOutMode() & 0x01);
    _uiNFC->pushButton_Mute->setChecked(_cfgItem->getOutMode() & 0x02);

    _uiNFC->fixedfilter->setValue(_cfgItem->getFixfilters());
    _uiNFC->dynamicfilter->setValue(_cfgItem->getDynfilters());
    //    _uiNFC->notchbandwidth->getDoubleSpinBox()->setValue(_cfgItem->getBandwidth());
    _uiNFC->notchbandwidth->setCurrentText(QString("1/%1").arg(_cfgItem->getBandwidth()));
    _uiNFC->maxnotchdepth->setValue(_cfgItem->getMaxNotchDepth());
#endif
}



void CfgSettingNFC::on_pushButton_clicked()
{
    _cfgItem->setClearFilter(2);//Clear All
}

void CfgSettingNFC::on_pushButton_2_clicked()
{
    _cfgItem->setClearFilter(1);//Clear Dynamic
}

void CfgSettingNFC::ValueChangedSlot(int d)
{
    //    qDebug() << __FUNCTION__ << __LINE__ << "_maxFilter = " << _maxFilter << "d = " << d;
#if Logo_FangTu
    if (sender() == _ui_FangTu->outgain)
    {
        _cfgItem->setOutGain(d);
        _ui_FangTu->outgain_label->setText(QString::number(_cfgItem->getOutGain()));
    }
    else if (sender() == _ui_FangTu->outmode)
    {
        _cfgItem->setOutMode(d);
    }
    else if (sender() == _ui_FangTu->fixedfilter)
    {
        _cfgItem->setFixfilters(d);
        _ui_FangTu->dynamicfilter->setRange(0, _maxFilter - d);
    }
    else if (sender() == _ui_FangTu->dynamicfilter)
    {
        _cfgItem->setDynfilters(d);
        _ui_FangTu->fixedfilter->setRange(0, _maxFilter - d);
    }
    else if (sender() == _ui_FangTu->notchbandwidth)
    {
        _cfgItem->setBandwidth(_ui_FangTu->notchbandwidth->itemText(d).split("/")[1].toInt());
    }
    else if (sender() == _ui_FangTu->maxnotchdepth)
    {
        _cfgItem->setMaxNotchDepth(d);
    }
#else
    if (sender() == _uiNFC->outgain)
    {
        _cfgItem->setOutGain(d);
        _uiNFC->outgain_label->setText(QString::number(_cfgItem->getOutGain()));
    }
    else if (sender() == _uiNFC->outmode)
    {
        _cfgItem->setOutMode(d);
    }
    else if (sender() == _uiNFC->fixedfilter)
    {
        _cfgItem->setFixfilters(d);
        _uiNFC->dynamicfilter->setRange(0, _maxFilter - d);
    }
    else if (sender() == _uiNFC->dynamicfilter)
    {
        _cfgItem->setDynfilters(d);
        _uiNFC->fixedfilter->setRange(0, _maxFilter - d);
    }
    else if (sender() == _uiNFC->notchbandwidth)
    {
        _cfgItem->setBandwidth(_uiNFC->notchbandwidth->itemText(d).split("/")[1].toInt());
    }
    else if (sender() == _uiNFC->maxnotchdepth)
    {
        _cfgItem->setMaxNotchDepth(d);
    }
#endif
}


void CfgSettingNFC::onOutModePbClicked()
{
#if Logo_FangTu
    if (_ui_FangTu->pushButton_Mute->isChecked()) {
        _cfgItem->setOutMode(2);
    }
    else if (_ui_FangTu->pushButton_Bypass->isChecked()) {
        _cfgItem->setOutMode(1);
    }
    else {
        _cfgItem->setOutMode(0);
    }
#else
    if (_uiNFC->pushButton_Mute->isChecked()) {
        _cfgItem->setOutMode(2);
    }
    else if (_uiNFC->pushButton_Bypass->isChecked()) {
        _cfgItem->setOutMode(1);
    }
    else {
        _cfgItem->setOutMode(0);
    }
#endif
}

void CfgSettingNFC::initUi()
{
    _uiNFC->fixedfilter->setRangeParam(0, _maxFilter, StepMode_Line, ParamType_Other);

    _uiNFC->dynamicfilter->setRangeParam(0, _maxFilter, StepMode_Line, ParamType_Other);

    QStringList bandWidthList = {"1/5", "1/10", "1/20", "1/80"};
    _uiNFC->notchbandwidth->addItems(bandWidthList);

    _uiNFC->maxnotchdepth->setRangeParam(-40, 0, StepMode_Line, ParamType_Other);

    _uiNFC->outgain->setValue(_cfgItem->getOutGain());
    _uiNFC->outgain_label->setText(QString::number(_cfgItem->getOutGain()));
    _uiNFC->outmode->setCurrentIndex(_cfgItem->getOutMode());
    _uiNFC->pushButton_Bypass->setChecked(_cfgItem->getOutMode() & 0x01);
    _uiNFC->pushButton_Mute->setChecked(_cfgItem->getOutMode() & 0x02);
    _uiNFC->fixedfilter->setValue(_cfgItem->getFixfilters());
    _uiNFC->dynamicfilter->setValue(_cfgItem->getDynfilters());
    _uiNFC->notchbandwidth->setCurrentText(QString("1/%1").arg(_cfgItem->getBandwidth()));
    _uiNFC->maxnotchdepth->setValue(_cfgItem->getMaxNotchDepth());

    connect(_uiNFC->outgain, SIGNAL(valueChanged(int)), this, SLOT(ValueChangedSlot(int)));
    connect(_uiNFC->outmode, SIGNAL(currentIndexChanged(int)), this, SLOT(ValueChangedSlot(int)));
    connect(_uiNFC->pushButton_Mute, SIGNAL(clicked()), this, SLOT(onOutModePbClicked()));
    connect(_uiNFC->pushButton_Bypass, SIGNAL(clicked()), this, SLOT(onOutModePbClicked()));

    connect(_uiNFC->fixedfilter, SIGNAL(valueChanged(int)), this, SLOT(ValueChangedSlot(int)));
    connect(_uiNFC->dynamicfilter, SIGNAL(valueChanged(int)), this, SLOT(ValueChangedSlot(int)));
    connect(_uiNFC->notchbandwidth, SIGNAL(currentIndexChanged(int)), this, SLOT(ValueChangedSlot(int)));
    connect(_uiNFC->maxnotchdepth, SIGNAL(valueChanged(int)), this, SLOT(ValueChangedSlot(int)));

    _howlFreq.append(_uiNFC->label_How_1);
    _howlFreq.append(_uiNFC->label_How_2);
    _howlFreq.append(_uiNFC->label_How_3);
    _howlFreq.append(_uiNFC->label_How_4);
    _howlFreq.append(_uiNFC->label_How_5);
    _howlFreq.append(_uiNFC->label_How_6);
    _howlFreq.append(_uiNFC->label_How_7);
    _howlFreq.append(_uiNFC->label_How_8);
    _howlFreq.append(_uiNFC->label_How_9);
    _howlFreq.append(_uiNFC->label_How_10);
    _howlFreq.append(_uiNFC->label_How_11);
    _howlFreq.append(_uiNFC->label_How_12);
    _howlFreq.append(_uiNFC->label_How_13);
    _howlFreq.append(_uiNFC->label_How_14);
    _howlFreq.append(_uiNFC->label_How_15);
    _howlFreq.append(_uiNFC->label_How_16);
    _howlFreq.append(_uiNFC->label_How_17);
    _howlFreq.append(_uiNFC->label_How_18);
    _howlFreq.append(_uiNFC->label_How_19);
    _howlFreq.append(_uiNFC->label_How_20);
    _howlFreq.append(_uiNFC->label_How_21);
    _howlFreq.append(_uiNFC->label_How_22);
    _howlFreq.append(_uiNFC->label_How_23);
    _howlFreq.append(_uiNFC->label_How_24);
    _howlFreq.append(_uiNFC->label_How_25);
    _howlFreq.append(_uiNFC->label_How_26);
    _howlFreq.append(_uiNFC->label_How_27);
    _howlFreq.append(_uiNFC->label_How_28);
    _howlFreq.append(_uiNFC->label_How_29);
    _howlFreq.append(_uiNFC->label_How_30);
    _howlFreq.append(_uiNFC->label_How_31);
    _howlFreq.append(_uiNFC->label_How_32);

    _NotchDepth.append(_uiNFC->label_Depth_1);
    _NotchDepth.append(_uiNFC->label_Depth_2);
    _NotchDepth.append(_uiNFC->label_Depth_3);
    _NotchDepth.append(_uiNFC->label_Depth_4);
    _NotchDepth.append(_uiNFC->label_Depth_5);
    _NotchDepth.append(_uiNFC->label_Depth_6);
    _NotchDepth.append(_uiNFC->label_Depth_7);
    _NotchDepth.append(_uiNFC->label_Depth_8);
    _NotchDepth.append(_uiNFC->label_Depth_9);
    _NotchDepth.append(_uiNFC->label_Depth_10);
    _NotchDepth.append(_uiNFC->label_Depth_11);
    _NotchDepth.append(_uiNFC->label_Depth_12);
    _NotchDepth.append(_uiNFC->label_Depth_13);
    _NotchDepth.append(_uiNFC->label_Depth_14);
    _NotchDepth.append(_uiNFC->label_Depth_15);
    _NotchDepth.append(_uiNFC->label_Depth_16);
    _NotchDepth.append(_uiNFC->label_Depth_17);
    _NotchDepth.append(_uiNFC->label_Depth_18);
    _NotchDepth.append(_uiNFC->label_Depth_19);
    _NotchDepth.append(_uiNFC->label_Depth_20);
    _NotchDepth.append(_uiNFC->label_Depth_21);
    _NotchDepth.append(_uiNFC->label_Depth_22);
    _NotchDepth.append(_uiNFC->label_Depth_23);
    _NotchDepth.append(_uiNFC->label_Depth_24);
    _NotchDepth.append(_uiNFC->label_Depth_25);
    _NotchDepth.append(_uiNFC->label_Depth_26);
    _NotchDepth.append(_uiNFC->label_Depth_27);
    _NotchDepth.append(_uiNFC->label_Depth_28);
    _NotchDepth.append(_uiNFC->label_Depth_29);
    _NotchDepth.append(_uiNFC->label_Depth_30);
    _NotchDepth.append(_uiNFC->label_Depth_31);
    _NotchDepth.append(_uiNFC->label_Depth_32);

    _uiNFC->outmode->setVisible(false);
    _uiNFC->outgain_label->setProperty("form", true);
    _uiNFC->pushButton_Mute->setProperty("form", "Mute");
    _uiNFC->pushButton_Bypass->setProperty("form", "Bypass");
}

void CfgSettingNFC::initUi_FangTu()
{
    _ui_FangTu->fixedfilter->setRangeParam(0, _maxFilter, StepMode_Line, ParamType_Other);

    _ui_FangTu->dynamicfilter->setRangeParam(0, _maxFilter, StepMode_Line, ParamType_Other);

    QStringList bandWidthList = {"1/5", "1/10", "1/20", "1/80"};
    _ui_FangTu->notchbandwidth->addItems(bandWidthList);

    _ui_FangTu->maxnotchdepth->setRangeParam(-40, 0, StepMode_Line, ParamType_Other);

    _ui_FangTu->outgain->setValue(_cfgItem->getOutGain());
    _ui_FangTu->outgain_label->setText(QString::number(_cfgItem->getOutGain()));
    _ui_FangTu->outmode->setCurrentIndex(_cfgItem->getOutMode());
    _ui_FangTu->pushButton_Bypass->setChecked(_cfgItem->getOutMode() & 0x01);
    _ui_FangTu->pushButton_Mute->setChecked(_cfgItem->getOutMode() & 0x02);
    _ui_FangTu->fixedfilter->setValue(_cfgItem->getFixfilters());
    _ui_FangTu->dynamicfilter->setValue(_cfgItem->getDynfilters());
    _ui_FangTu->notchbandwidth->setCurrentText(QString("1/%1").arg(_cfgItem->getBandwidth()));
    _ui_FangTu->maxnotchdepth->setValue(_cfgItem->getMaxNotchDepth());

    connect(_ui_FangTu->outgain, SIGNAL(valueChanged(int)), this, SLOT(ValueChangedSlot(int)));
    connect(_ui_FangTu->outmode, SIGNAL(currentIndexChanged(int)), this, SLOT(ValueChangedSlot(int)));
    connect(_ui_FangTu->pushButton_Mute, SIGNAL(clicked()), this, SLOT(onOutModePbClicked()));
    connect(_ui_FangTu->pushButton_Bypass, SIGNAL(clicked()), this, SLOT(onOutModePbClicked()));

    connect(_ui_FangTu->fixedfilter, SIGNAL(valueChanged(int)), this, SLOT(ValueChangedSlot(int)));
    connect(_ui_FangTu->dynamicfilter, SIGNAL(valueChanged(int)), this, SLOT(ValueChangedSlot(int)));
    connect(_ui_FangTu->notchbandwidth, SIGNAL(currentIndexChanged(int)), this, SLOT(ValueChangedSlot(int)));
    connect(_ui_FangTu->maxnotchdepth, SIGNAL(valueChanged(int)), this, SLOT(ValueChangedSlot(int)));

    _howlFreq.append(_ui_FangTu->label_How_1);
    _howlFreq.append(_ui_FangTu->label_How_2);
    _howlFreq.append(_ui_FangTu->label_How_3);
    _howlFreq.append(_ui_FangTu->label_How_4);
    _howlFreq.append(_ui_FangTu->label_How_5);
    _howlFreq.append(_ui_FangTu->label_How_6);
    _howlFreq.append(_ui_FangTu->label_How_7);
    _howlFreq.append(_ui_FangTu->label_How_8);
    _howlFreq.append(_ui_FangTu->label_How_9);
    _howlFreq.append(_ui_FangTu->label_How_10);
    _howlFreq.append(_ui_FangTu->label_How_11);
    _howlFreq.append(_ui_FangTu->label_How_12);
    _howlFreq.append(_ui_FangTu->label_How_13);
    _howlFreq.append(_ui_FangTu->label_How_14);
    _howlFreq.append(_ui_FangTu->label_How_15);
    _howlFreq.append(_ui_FangTu->label_How_16);
    _howlFreq.append(_ui_FangTu->label_How_17);
    _howlFreq.append(_ui_FangTu->label_How_18);
    _howlFreq.append(_ui_FangTu->label_How_19);
    _howlFreq.append(_ui_FangTu->label_How_20);
    _howlFreq.append(_ui_FangTu->label_How_21);
    _howlFreq.append(_ui_FangTu->label_How_22);
    _howlFreq.append(_ui_FangTu->label_How_23);
    _howlFreq.append(_ui_FangTu->label_How_24);
    _howlFreq.append(_ui_FangTu->label_How_25);
    _howlFreq.append(_ui_FangTu->label_How_26);
    _howlFreq.append(_ui_FangTu->label_How_27);
    _howlFreq.append(_ui_FangTu->label_How_28);
    _howlFreq.append(_ui_FangTu->label_How_29);
    _howlFreq.append(_ui_FangTu->label_How_30);
    _howlFreq.append(_ui_FangTu->label_How_31);
    _howlFreq.append(_ui_FangTu->label_How_32);

    _NotchDepth.append(_ui_FangTu->label_Depth_1);
    _NotchDepth.append(_ui_FangTu->label_Depth_2);
    _NotchDepth.append(_ui_FangTu->label_Depth_3);
    _NotchDepth.append(_ui_FangTu->label_Depth_4);
    _NotchDepth.append(_ui_FangTu->label_Depth_5);
    _NotchDepth.append(_ui_FangTu->label_Depth_6);
    _NotchDepth.append(_ui_FangTu->label_Depth_7);
    _NotchDepth.append(_ui_FangTu->label_Depth_8);
    _NotchDepth.append(_ui_FangTu->label_Depth_9);
    _NotchDepth.append(_ui_FangTu->label_Depth_10);
    _NotchDepth.append(_ui_FangTu->label_Depth_11);
    _NotchDepth.append(_ui_FangTu->label_Depth_12);
    _NotchDepth.append(_ui_FangTu->label_Depth_13);
    _NotchDepth.append(_ui_FangTu->label_Depth_14);
    _NotchDepth.append(_ui_FangTu->label_Depth_15);
    _NotchDepth.append(_ui_FangTu->label_Depth_16);
    _NotchDepth.append(_ui_FangTu->label_Depth_17);
    _NotchDepth.append(_ui_FangTu->label_Depth_18);
    _NotchDepth.append(_ui_FangTu->label_Depth_19);
    _NotchDepth.append(_ui_FangTu->label_Depth_20);
    _NotchDepth.append(_ui_FangTu->label_Depth_21);
    _NotchDepth.append(_ui_FangTu->label_Depth_22);
    _NotchDepth.append(_ui_FangTu->label_Depth_23);
    _NotchDepth.append(_ui_FangTu->label_Depth_24);
    _NotchDepth.append(_ui_FangTu->label_Depth_25);
    _NotchDepth.append(_ui_FangTu->label_Depth_26);
    _NotchDepth.append(_ui_FangTu->label_Depth_27);
    _NotchDepth.append(_ui_FangTu->label_Depth_28);
    _NotchDepth.append(_ui_FangTu->label_Depth_29);
    _NotchDepth.append(_ui_FangTu->label_Depth_30);
    _NotchDepth.append(_ui_FangTu->label_Depth_31);
    _NotchDepth.append(_ui_FangTu->label_Depth_32);

    _ui_FangTu->outmode->setVisible(false);
    _ui_FangTu->outgain_label->setProperty("form", true);
    _ui_FangTu->pushButton_Mute->setProperty("form", "Mute");
    _ui_FangTu->pushButton_Bypass->setProperty("form", "Bypass");
}
