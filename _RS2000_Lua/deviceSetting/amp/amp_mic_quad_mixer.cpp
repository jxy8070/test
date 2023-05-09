#include "amp_mic_quad_mixer.h"
#include "ui_amp_mic_quad_mixer.h"

Mixer_4_4::Mixer_4_4(QStringList in, QStringList out, ConfigData_RT1052_Mixer_4_4 mixer, QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::Amp_Mic_Quad_Mixer)
{
    ui->setupUi(this);

    ui->label_in1->setText(in.at(0));
    ui->label_in2->setText(in.at(1));
    ui->label_in3->setText(in.at(2));
    ui->label_in4->setText(in.at(3));

    ui->label_out1->setText(out.at(0));
    ui->label_out2->setText(out.at(1));
    ui->label_out3->setText(out.at(2));
    ui->label_out4->setText(out.at(3));

    ui->NOut1_AIn1->setChecked(mixer.m[0][0]);
    ui->NOut1_AIn2->setChecked(mixer.m[0][1]);
    ui->NOut1_AIn3->setChecked(mixer.m[0][2]);
    ui->NOut1_AIn4->setChecked(mixer.m[0][3]);

    ui->NOut2_AIn1->setChecked(mixer.m[1][0]);
    ui->NOut2_AIn2->setChecked(mixer.m[1][1]);
    ui->NOut2_AIn3->setChecked(mixer.m[1][2]);
    ui->NOut2_AIn4->setChecked(mixer.m[1][3]);

    ui->NOut3_AIn1->setChecked(mixer.m[2][0]);
    ui->NOut3_AIn2->setChecked(mixer.m[2][1]);
    ui->NOut3_AIn3->setChecked(mixer.m[2][2]);
    ui->NOut3_AIn4->setChecked(mixer.m[2][3]);

    ui->NOut4_AIn1->setChecked(mixer.m[3][0]);
    ui->NOut4_AIn2->setChecked(mixer.m[3][1]);
    ui->NOut4_AIn3->setChecked(mixer.m[3][2]);
    ui->NOut4_AIn4->setChecked(mixer.m[3][3]);

    connect(ui->Close, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->NOut1_AIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut1_AIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut1_AIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut1_AIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));

    connect(ui->NOut2_AIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut2_AIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut2_AIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut2_AIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));

    connect(ui->NOut3_AIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut3_AIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut3_AIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut3_AIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));

    connect(ui->NOut4_AIn1, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut4_AIn2, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut4_AIn3, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
    connect(ui->NOut4_AIn4, SIGNAL(clicked(bool)), this, SLOT(pb_clicked(bool)));
}

Mixer_4_4::~Mixer_4_4()
{
    delete ui;
}

void Mixer_4_4::pb_clicked(bool enable)
{
    quint8 in = 0, out = 0;
    if (sender() == ui->NOut1_AIn1)
    {
        out = 0; in = 0;
    }
    else if (sender() == ui->NOut1_AIn2)
    {
        out = 0; in = 1;
    }
    else if (sender() == ui->NOut1_AIn3)
    {
        out = 0; in = 2;
    }
    else if (sender() == ui->NOut1_AIn4)
    {
        out = 0; in = 3;
    }
    else if (sender() == ui->NOut2_AIn1)
    {
        out = 1; in = 0;
    }
    else if (sender() == ui->NOut2_AIn2)
    {
        out = 1; in = 1;
    }
    else if (sender() == ui->NOut2_AIn3)
    {
        out = 1; in = 2;
    }
    else if (sender() == ui->NOut2_AIn4)
    {
        out = 1; in = 3;
    }
    else if (sender() == ui->NOut3_AIn1)
    {
        out = 2; in = 0;
    }
    else if (sender() == ui->NOut3_AIn2)
    {
        out = 2; in = 1;
    }
    else if (sender() == ui->NOut3_AIn3)
    {
        out = 2; in = 2;
    }
    else if (sender() == ui->NOut3_AIn4)
    {
        out = 2; in = 3;
    }
    else if (sender() == ui->NOut4_AIn1)
    {
        out = 3; in = 0;
    }
    else if (sender() == ui->NOut4_AIn2)
    {
        out = 3; in = 1;
    }
    else if (sender() == ui->NOut4_AIn3)
    {
        out = 3; in = 2;
    }
    else if (sender() == ui->NOut4_AIn4)
    {
        out = 3; in = 3;
    }

    emit mixerChanged(out, in, enable);
//    _dev->sendMixer(out, in, enable);
}
