#include "base_2_2_mixer.h"
#include "ui_base_2_2_mixer.h"

base_2_2_mixer::base_2_2_mixer(QStringList in,QStringList out,ConfigData_RT1052_Mixer_2_2 mixer,QWidget *parent) :
    DevSettingDlg(parent),
    ui(new Ui::base_2_2_mixer)
{
    ui->setupUi(this);

    ui->label_a1->setText(in.at(0));
    ui->label_n1->setText(in.at(1));

    ui->label_al->setText(out.at(0));
    ui->label_ar->setText(out.at(1));

    ui->AL_a1->setChecked(mixer.m[0][0]);
    ui->AL_n1->setChecked(mixer.m[0][1]);
    ui->AR_a1->setChecked(mixer.m[1][0]);
    ui->AR_n1->setChecked(mixer.m[1][1]);

    connect(ui->pushButton_close,SIGNAL(clicked(bool)),this,SLOT(close()));

    connect(ui->AL_a1,SIGNAL(clicked(bool)),this,SLOT(mixer_clicked(bool)));
    connect(ui->AL_n1,SIGNAL(clicked(bool)),this,SLOT(mixer_clicked(bool)));
    connect(ui->AR_a1,SIGNAL(clicked(bool)),this,SLOT(mixer_clicked(bool)));
    connect(ui->AR_n1,SIGNAL(clicked(bool)),this,SLOT(mixer_clicked(bool)));

    set_Enabled();
}

base_2_2_mixer::~base_2_2_mixer()
{
    delete ui;
}

void base_2_2_mixer::set_Enabled()
{
    ui->label_ar->setVisible(false);
    ui->AR_a1->setVisible(false);
    ui->AR_n1->setVisible(false);
}

void base_2_2_mixer::mixer_clicked(bool enable)
{
    quint8 in = 0,out = 0;
    if(sender() == ui->AL_a1)
    {
        in = 0;
        out = 0;
    }else if(sender() == ui->AL_n1)
    {
        in = 1;
        out = 0;
    }else if(sender() == ui->AR_a1)
    {
        in = 0;
        out = 1;
    }else if(sender() == ui->AR_n1)
    {
        in = 1;
        out = 1;
    }
    emit mixerchanged(out,in,enable);
}
