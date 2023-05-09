#include "stable.h"
#include "amp_filter1.h"

AmpFilter1::AmpFilter1(quint8 channel, DividerData *dividerData, QWidget *parent) :
    DevSettingDlg(parent),
    _ui(new Ui::AmpFilter1)
{
    _ui->setupUi(this);
    _dividerData = dividerData;
    _channel = channel;

    double hf = (double)(_dividerData->freq);
    _ui->dSBFreq->setValue(hf);
    connect(_ui->dSBFreq, SIGNAL(valueChanged(double)), this, SLOT(onDataChanged()));

    _ui->comboBox->setCurrentIndex(_dividerData->q);
    connect(_ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onDataChanged()));

    quint16 order = _dividerData->gain >> 4;
    quint16 fir_n = _dividerData->gain & 0xF;
    _ui->spinBox->setValue(order);
    _ui->comboBox_2->setCurrentIndex(fir_n / 2 - 1);

    if (_dividerData->q == FIR_BLACKMAN)
    {
        _ui->comboBox_2->setVisible(false);
    } else {
        _ui->spinBox->setVisible(false);
    }

    connect(_ui->spinBox, SIGNAL(editingFinished()), this, SLOT(onDataChanged()));
    connect(_ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onDataChanged()));

    _ui->comboBox->setDisabled(true);
    _ui->comboBox_2->setDisabled(true);
}

void AmpFilter1::onDataChanged()
{
    _dividerData->freq = ((int)_ui->dSBFreq->value()) & 0xFFFF;
    _dividerData->q = _ui->comboBox->currentIndex();
//    if (_dividerData->q == FIR_BLACKMAN)
//    {
//        _ui->spinBox->setVisible(true);
//        _ui->comboBox_2->setVisible(false);
//    } else if (_dividerData->q == LINK_RILEY) {
//        _ui->spinBox->setVisible(false);
//        _ui->comboBox_2->setVisible(true);
//    } else {
//        _ui->spinBox->setVisible(false);
//        _ui->comboBox_2->setVisible(false);
//    }

    quint16 order = _ui->spinBox->value();
    quint16 fir_n = (_ui->comboBox_2->currentIndex() + 1) * 2;
    _dividerData->gain = (order << 4 & 0xFFF0) | (fir_n & 0xF);

    emit pfDataChanged(_channel);
}
