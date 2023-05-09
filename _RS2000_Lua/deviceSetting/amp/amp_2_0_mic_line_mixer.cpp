#include "amp_2_0_mic_line_mixer.h"
#include "ui_amp_2_0_mic_line_mixer.h"
#include "device/device_amp_2_0_mic_line.h"

Amp_2_0_Mic_Line_Mixer::Amp_2_0_Mic_Line_Mixer(Device_Amp_2_0_Mic_Line *devAmp, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::Amp_2_0_Mic_Line_Mixer)
{
    ui->setupUi(this);

    _devAmp = devAmp;
    ConfigData_RT1052_Mixer_2_2 mix = _devAmp->getConfigData().Nout_mixer;

    ui->AOut1_NIn1->setChecked(mix.m[0][0]);
    ui->AOut1_NIn2->setChecked(mix.m[0][1]);

    ui->AOut2_NIn1->setChecked(mix.m[1][0]);
    ui->AOut2_NIn2->setChecked(mix.m[1][1]);

    connect(ui->Close, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->AOut1_NIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_NIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));

    connect(ui->AOut2_NIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_NIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
}

Amp_2_0_Mic_Line_Mixer::~Amp_2_0_Mic_Line_Mixer()
{
    delete ui;
}

void Amp_2_0_Mic_Line_Mixer::pb_clicked(bool enable)
{
    quint8 in = 0, out = 0;
    if (sender() == ui->AOut1_NIn1)
    {
        out = 0; in = 0;
    }
    else if (sender() == ui->AOut1_NIn2)
    {
        out = 0; in = 1;
    }
    else if (sender() == ui->AOut2_NIn1)
    {
        out = 1; in = 0;
    }
    else if (sender() == ui->AOut2_NIn2)
    {
        out = 1; in = 1;
    }

    _devAmp->sendMixer(out, in, enable);
}
