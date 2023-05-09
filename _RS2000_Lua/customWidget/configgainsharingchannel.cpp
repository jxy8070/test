#include "configgainsharingchannel.h"
#include "ui_configgainsharingchannel.h"
#include "config.h"

ConfigGainSharingChannel::ConfigGainSharingChannel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigGainSharingChannel)
{
    ui->setupUi(this);
    ui->PB_InputMute_1->setProperty("form", "Mute");
}

ConfigGainSharingChannel::~ConfigGainSharingChannel()
{
    delete ui;
}

void ConfigGainSharingChannel::setTitle(QString title)
{
    ui->title->setText(title);
}

void ConfigGainSharingChannel::setVu(int value)
{
    ui->Disp_Level_1->setLCurValue(value);
    ui->label_level->setText(QString::asprintf("%d dB", value));
}

void ConfigGainSharingChannel::setPreInGain(int value)
{
    ui->dial->setValue(value);
//    ui->label_input1->setText(QString::asprintf("%d dB", value));
}

void ConfigGainSharingChannel::setPreInMute(bool checked)
{
    ui->PB_InputMute_1->setChecked(checked);
}



void ConfigGainSharingChannel::on_dial_valueChanged(int value)
{
//    ui->label_input1->setText(QString::asprintf("%d dB", value));
    emit PreInGainValueChanged(value);
}

void ConfigGainSharingChannel::on_PB_InputMute_1_clicked(bool checked)
{
    emit PreInMuteClicked(checked);
}
