#include "stable.h"
#include "spk_amppf.h"
#include "ui_spk_amppf.h"
#include "device/device_spk.h"


AmpPf::AmpPf(quint8 channel, DividerData *dividerData, AmpPfTypeEnum type, QWidget *parent) :
    DevSettingDlg(parent),
    _ui(new Ui::Spk_AmpPf)
{
    _ui->setupUi(this);
    _dividerData = dividerData;
    _channel = channel;
    _type = type;

    double hf = (double)(_dividerData->freq);
    _ui->dSBFreq->setValue(hf);
    connect(_ui->dSBFreq, SIGNAL(valueChanged(double)), this, SLOT(onDataChanged()));

    switch (_type) {
    case AmpPfType_SPK:
        _ui->byPass->setChecked(_dividerData->bypass);
        connect(_ui->byPass, SIGNAL(clicked()), this, SLOT(onDataChanged()));
        break;
    case AmpPfType_AMP:
        _ui->byPass->setVisible(false);
        break;
    default:
        break;
    }

//    connect(_ui->dSBGain, SIGNAL(valueChanged(double)), this, SLOT(onDataChanged()));
//    connect(_ui->dSBQ, SIGNAL(valueChanged(double)), this, SLOT(onDataChanged()));
}

void AmpPf::onDataChanged()
{
    _dividerData->freq = ((int)_ui->dSBFreq->value()) & 0xFFFF;

    quint8 bypass = _ui->byPass->isChecked()? 0x01 : 0x00;
    switch (_type) {
    case AmpPfType_SPK:
        _dividerData->bypass = bypass;
        break;
    case AmpPfType_AMP:
        break;
    default:
        break;
    }

    emit pfDataChanged(_channel);
}


void AmpPf::setViewMode(bool isView)
{
    _ui->dSBFreq->setDisabled(isView);
    _ui->byPass->setDisabled(isView);
}
