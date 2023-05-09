#include "amp_4_4_xover.h"
#include "ui_amp_4_4_xover.h"
#include "device/device_amp_quad.h"
#include "device/device_amp_module.h"

Amp_4_4_xover::Amp_4_4_xover(Device_Amp_Quad *devAmp, quint8 ch, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::Amp_4_4_xover)
{
    ui->setupUi(this);
    _devAmpModule = NULL;
    _devAmp = devAmp;
    _ch = ch;
    ConfigData_Xover xover = _devAmp->getConfigData().Aout_xover[ch];

    ui->head->setText(tr("Channel %1").arg(_ch+1));
    ui->bypass->setChecked(xover.bypass);
    ui->low_freq->setValue(xover.freq[0]);
    ui->high_freq->setValue(xover.freq[1]);
    ui->low_stages->setCurrentIndex(xover.stages[0]/2 - 1);
    ui->high_stages->setCurrentIndex(xover.stages[1]/2 - 1);

    connect(ui->close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->bypass, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->low_freq, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    connect(ui->high_freq, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    connect(ui->low_stages, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
    connect(ui->high_stages, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
}

Amp_4_4_xover::Amp_4_4_xover(Device_Amp_Module *devAmp, quint8 ch, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::Amp_4_4_xover)
{
    ui->setupUi(this);
    _devAmp = NULL;
    _devAmpModule = devAmp;
    _ch = ch;
    ConfigData_Xover xover = _devAmpModule->getConfigData().Aout_xover[ch];

    ui->head->setText(tr("Channel %1").arg(_ch+1));
    ui->bypass->setChecked(xover.bypass);
    ui->low_freq->setValue(xover.freq[0]);
    ui->high_freq->setValue(xover.freq[1]);
    ui->low_stages->setCurrentIndex(xover.stages[0]/2 - 1);
    ui->high_stages->setCurrentIndex(xover.stages[1]/2 - 1);

    connect(ui->close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->bypass, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->low_freq, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    connect(ui->high_freq, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    connect(ui->low_stages, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
    connect(ui->high_stages, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
}

Amp_4_4_xover::~Amp_4_4_xover()
{
    delete ui;
}

void Amp_4_4_xover::pb_clicked(bool enable)
{
    if (sender() == ui->bypass)
    {
        if(_devAmp != NULL)
        {
            _devAmp->sendXover(_ch, enable, ui->low_freq->value(), ui->high_freq->value(),
                               (ui->low_stages->currentIndex()+1)*2, (ui->high_stages->currentIndex()+1)*2);
        }else{
            _devAmpModule->sendXover(_ch, enable, ui->low_freq->value(), ui->high_freq->value(),
                                     (ui->low_stages->currentIndex()+1)*2, (ui->high_stages->currentIndex()+1)*2);
        }
    }
}

void Amp_4_4_xover::valueChanged(int value)
{
    if (sender() == ui->low_freq)
    {
        if(_devAmp != NULL)
        {
            _devAmp->sendXover(_ch, ui->bypass->isChecked(), value, ui->high_freq->value(),
                               (ui->low_stages->currentIndex()+1)*2, (ui->high_stages->currentIndex()+1)*2);
        }else
        {
            _devAmpModule->sendXover(_ch, ui->bypass->isChecked(), value, ui->high_freq->value(),
                               (ui->low_stages->currentIndex()+1)*2, (ui->high_stages->currentIndex()+1)*2);
        }
    }
    else if (sender() == ui->high_freq)
    {
        if(_devAmp != NULL)
        {
            _devAmp->sendXover(_ch, ui->bypass->isChecked(), ui->low_freq->value(), value,
                               (ui->low_stages->currentIndex()+1)*2, (ui->high_stages->currentIndex()+1)*2);
        }else{
            _devAmpModule->sendXover(_ch, ui->bypass->isChecked(), ui->low_freq->value(), value,
                               (ui->low_stages->currentIndex()+1)*2, (ui->high_stages->currentIndex()+1)*2);
        }
    }
}

void Amp_4_4_xover::currentIndexChanged(int value)
{
    if (sender() == ui->low_stages)
    {
        if(_devAmp != NULL)
        {
            _devAmp->sendXover(_ch, ui->bypass->isChecked(), ui->low_freq->value(), ui->high_freq->value(),
                               (value+1)*2, (ui->high_stages->currentIndex()+1)*2);
        }else{
            _devAmpModule->sendXover(_ch, ui->bypass->isChecked(), ui->low_freq->value(), ui->high_freq->value(),
                               (value+1)*2, (ui->high_stages->currentIndex()+1)*2);
        }
    }
    else if (sender() == ui->high_stages)
    {
        if(_devAmp != NULL)
        {
            _devAmp->sendXover(_ch, ui->bypass->isChecked(), ui->low_freq->value(), ui->high_freq->value(),
                               (ui->low_stages->currentIndex()+1)*2, (value+1)*2);
        }else{
            _devAmpModule->sendXover(_ch, ui->bypass->isChecked(), ui->low_freq->value(), ui->high_freq->value(),
                               (ui->low_stages->currentIndex()+1)*2, (value+1)*2);
        }
    }
}
