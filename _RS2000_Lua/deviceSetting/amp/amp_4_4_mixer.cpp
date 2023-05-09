#include "amp_4_4_mixer.h"
#include "ui_amp_4_4_mixer.h"
#include "device/device_amp_quad.h"
#include "device/device_amp_module.h"

Amp_4_4_Mixer::Amp_4_4_Mixer(Device_Amp_Quad *devAmp, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::Amp_4_4_Mixer)
{
    ui->setupUi(this);
    _devAmpModule = NULL;
    _devAmp = devAmp;
    ConfigData_RT1052_Mixer_4_8 mix = _devAmp->getConfigData().Aout_mixer;
    ui->AOut1_AIn1->setChecked(mix.m[0][0]);
    ui->AOut1_AIn2->setChecked(mix.m[0][1]);
    ui->AOut1_AIn3->setChecked(mix.m[0][2]);
    ui->AOut1_AIn4->setChecked(mix.m[0][3]);
    ui->AOut1_NIn1->setChecked(mix.m[0][4]);
    ui->AOut1_NIn2->setChecked(mix.m[0][5]);
    ui->AOut1_NIn3->setChecked(mix.m[0][6]);
    ui->AOut1_NIn4->setChecked(mix.m[0][7]);

    ui->AOut2_AIn1->setChecked(mix.m[1][0]);
    ui->AOut2_AIn2->setChecked(mix.m[1][1]);
    ui->AOut2_AIn3->setChecked(mix.m[1][2]);
    ui->AOut2_AIn4->setChecked(mix.m[1][3]);
    ui->AOut2_NIn1->setChecked(mix.m[1][4]);
    ui->AOut2_NIn2->setChecked(mix.m[1][5]);
    ui->AOut2_NIn3->setChecked(mix.m[1][6]);
    ui->AOut2_NIn4->setChecked(mix.m[1][7]);

    ui->AOut3_AIn1->setChecked(mix.m[2][0]);
    ui->AOut3_AIn2->setChecked(mix.m[2][1]);
    ui->AOut3_AIn3->setChecked(mix.m[2][2]);
    ui->AOut3_AIn4->setChecked(mix.m[2][3]);
    ui->AOut3_NIn1->setChecked(mix.m[2][4]);
    ui->AOut3_NIn2->setChecked(mix.m[2][5]);
    ui->AOut3_NIn3->setChecked(mix.m[2][6]);
    ui->AOut3_NIn4->setChecked(mix.m[2][7]);

    ui->AOut4_AIn1->setChecked(mix.m[3][0]);
    ui->AOut4_AIn2->setChecked(mix.m[3][1]);
    ui->AOut4_AIn3->setChecked(mix.m[3][2]);
    ui->AOut4_AIn4->setChecked(mix.m[3][3]);
    ui->AOut4_NIn1->setChecked(mix.m[3][4]);
    ui->AOut4_NIn2->setChecked(mix.m[3][5]);
    ui->AOut4_NIn3->setChecked(mix.m[3][6]);
    ui->AOut4_NIn4->setChecked(mix.m[3][7]);

    connect(ui->Close, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->AOut1_AIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_AIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_AIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_AIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_NIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_NIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_NIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_NIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));

    connect(ui->AOut2_AIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_AIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_AIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_AIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_NIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_NIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_NIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_NIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));

    connect(ui->AOut3_AIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_AIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_AIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_AIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_NIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_NIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_NIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_NIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));

    connect(ui->AOut4_AIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_AIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_AIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_AIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_NIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_NIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_NIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_NIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
}

Amp_4_4_Mixer::Amp_4_4_Mixer(Device_Amp_Module *devAmp, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::Amp_4_4_Mixer)
{
    ui->setupUi(this);
    _devAmp = NULL;
    _devAmpModule = devAmp;
    ConfigData_RT1052_Mixer_4_8 mix = _devAmpModule->getConfigData().Aout_mixer;
    ui->AOut1_AIn1->setChecked(mix.m[0][0]);
    ui->AOut1_AIn2->setChecked(mix.m[0][1]);
    ui->AOut1_AIn3->setChecked(mix.m[0][2]);
    ui->AOut1_AIn4->setChecked(mix.m[0][3]);
    ui->AOut1_NIn1->setChecked(mix.m[0][4]);
    ui->AOut1_NIn2->setChecked(mix.m[0][5]);
    ui->AOut1_NIn3->setChecked(mix.m[0][6]);
    ui->AOut1_NIn4->setChecked(mix.m[0][7]);

    ui->AOut2_AIn1->setChecked(mix.m[1][0]);
    ui->AOut2_AIn2->setChecked(mix.m[1][1]);
    ui->AOut2_AIn3->setChecked(mix.m[1][2]);
    ui->AOut2_AIn4->setChecked(mix.m[1][3]);
    ui->AOut2_NIn1->setChecked(mix.m[1][4]);
    ui->AOut2_NIn2->setChecked(mix.m[1][5]);
    ui->AOut2_NIn3->setChecked(mix.m[1][6]);
    ui->AOut2_NIn4->setChecked(mix.m[1][7]);

    ui->AOut3_AIn1->setChecked(mix.m[2][0]);
    ui->AOut3_AIn2->setChecked(mix.m[2][1]);
    ui->AOut3_AIn3->setChecked(mix.m[2][2]);
    ui->AOut3_AIn4->setChecked(mix.m[2][3]);
    ui->AOut3_NIn1->setChecked(mix.m[2][4]);
    ui->AOut3_NIn2->setChecked(mix.m[2][5]);
    ui->AOut3_NIn3->setChecked(mix.m[2][6]);
    ui->AOut3_NIn4->setChecked(mix.m[2][7]);

    ui->AOut4_AIn1->setChecked(mix.m[3][0]);
    ui->AOut4_AIn2->setChecked(mix.m[3][1]);
    ui->AOut4_AIn3->setChecked(mix.m[3][2]);
    ui->AOut4_AIn4->setChecked(mix.m[3][3]);
    ui->AOut4_NIn1->setChecked(mix.m[3][4]);
    ui->AOut4_NIn2->setChecked(mix.m[3][5]);
    ui->AOut4_NIn3->setChecked(mix.m[3][6]);
    ui->AOut4_NIn4->setChecked(mix.m[3][7]);

    connect(ui->Close, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->AOut1_AIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_AIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_AIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_AIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_NIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_NIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_NIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut1_NIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));

    connect(ui->AOut2_AIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_AIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_AIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_AIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_NIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_NIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_NIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut2_NIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));

    connect(ui->AOut3_AIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_AIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_AIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_AIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_NIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_NIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_NIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut3_NIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));

    connect(ui->AOut4_AIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_AIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_AIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_AIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_NIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_NIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_NIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->AOut4_NIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
}

Amp_4_4_Mixer::~Amp_4_4_Mixer()
{
    delete ui;
}

void Amp_4_4_Mixer::pb_clicked(bool enable)
{
    quint8 in = 0, out = 0;
    if (sender() == ui->AOut1_AIn1)
    {
        out = 0; in = 0;
    }
    else if (sender() == ui->AOut1_AIn2)
    {
        out = 0; in = 1;
    }
    else if (sender() == ui->AOut1_AIn3)
    {
        out = 0; in = 2;
    }
    else if (sender() == ui->AOut1_AIn4)
    {
        out = 0; in = 3;
    }
    else if (sender() == ui->AOut1_NIn1)
    {
        out = 0; in = 4;
    }
    else if (sender() == ui->AOut1_NIn2)
    {
        out = 0; in = 5;
    }
    else if (sender() == ui->AOut1_NIn3)
    {
        out = 0; in = 6;
    }
    else if (sender() == ui->AOut1_NIn4)
    {
        out = 0; in = 7;
    }
    else if (sender() == ui->AOut2_AIn1)
    {
        out = 1; in = 0;
    }
    else if (sender() == ui->AOut2_AIn2)
    {
        out = 1; in = 1;
    }
    else if (sender() == ui->AOut2_AIn3)
    {
        out = 1; in = 2;
    }
    else if (sender() == ui->AOut2_AIn4)
    {
        out = 1; in = 3;
    }
    else if (sender() == ui->AOut2_NIn1)
    {
        out = 1; in = 4;
    }
    else if (sender() == ui->AOut2_NIn2)
    {
        out = 1; in = 5;
    }
    else if (sender() == ui->AOut2_NIn3)
    {
        out = 1; in = 6;
    }
    else if (sender() == ui->AOut2_NIn4)
    {
        out = 1; in = 7;
    }
    else if (sender() == ui->AOut3_AIn1)
    {
        out = 2; in = 0;
    }
    else if (sender() == ui->AOut3_AIn2)
    {
        out = 2; in = 1;
    }
    else if (sender() == ui->AOut3_AIn3)
    {
        out = 2; in = 2;
    }
    else if (sender() == ui->AOut3_AIn4)
    {
        out = 2; in = 3;
    }
    else if (sender() == ui->AOut3_NIn1)
    {
        out = 2; in = 4;
    }
    else if (sender() == ui->AOut3_NIn2)
    {
        out = 2; in = 5;
    }
    else if (sender() == ui->AOut3_NIn3)
    {
        out = 2; in = 6;
    }
    else if (sender() == ui->AOut3_NIn4)
    {
        out = 2; in = 7;
    }
    else if (sender() == ui->AOut4_AIn1)
    {
        out = 3; in = 0;
    }
    else if (sender() == ui->AOut4_AIn2)
    {
        out = 3; in = 1;
    }
    else if (sender() == ui->AOut4_AIn3)
    {
        out = 3; in = 2;
    }
    else if (sender() == ui->AOut4_AIn4)
    {
        out = 3; in = 3;
    }
    else if (sender() == ui->AOut4_NIn1)
    {
        out = 3; in = 4;
    }
    else if (sender() == ui->AOut4_NIn2)
    {
        out = 3; in = 5;
    }
    else if (sender() == ui->AOut4_NIn3)
    {
        out = 3; in = 6;
    }
    else if (sender() == ui->AOut4_NIn4)
    {
        out = 3; in = 7;
    }
    if(_devAmp != NULL)
    {
        _devAmp->sendMixer(out, in, enable);
    }else
    {
        _devAmpModule->sendMixer(out, in, enable);
    }
}
