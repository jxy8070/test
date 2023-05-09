#include "amp_mic_quad_xover.h"
#include "ui_amp_mic_quad_xover.h"
#include "device/device_mic_quad.h"

Amp_Mic_Quad_Xover::Amp_Mic_Quad_Xover(Device_Mic_Quad *dev, int ch, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::Amp_Mic_Quad_Xover)
{
    ui->setupUi(this);
    _dev = dev;
    _ch = ch;
    ConfigData_Xover xover = _dev->getConfigData().N_xover[ch];

    ui->head->setText(tr("Channel %1").arg(_ch+1));
    ui->bypass->setChecked(xover.bypass);
    ui->high_freq->setValue(xover.freq[1]);
    ui->high_stages->setCurrentIndex(xover.stages[1]/2 - 1);

    connect(ui->close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->bypass, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->high_freq, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    connect(ui->high_stages, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
}

Amp_Mic_Quad_Xover::~Amp_Mic_Quad_Xover()
{
    delete ui;
}

void Amp_Mic_Quad_Xover::pb_clicked(bool enable)
{
    if (sender() == ui->bypass)
    {
        _dev->sendXover(_ch, enable, ui->high_freq->value(),
                           (ui->high_stages->currentIndex()+1)*2);
    }
}

void Amp_Mic_Quad_Xover::valueChanged(int value)
{
    if (sender() == ui->high_freq)
    {
        _dev->sendXover(_ch, ui->bypass->isChecked(), value,
                           (ui->high_stages->currentIndex()+1)*2);
    }
}

void Amp_Mic_Quad_Xover::currentIndexChanged(int value)
{
    if (sender() == ui->high_stages)
    {
        _dev->sendXover(_ch, ui->bypass->isChecked(), ui->high_freq->value(),
                           (value+1)*2);
    }
}
