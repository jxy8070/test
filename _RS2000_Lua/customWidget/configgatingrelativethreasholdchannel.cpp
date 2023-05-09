#include "configgatingrelativethreasholdchannel.h"
#include "ui_configgatingrelativethreasholdchannel.h"
#include "config.h"

ConfigGatingRelativeThreasholdChannel::ConfigGatingRelativeThreasholdChannel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigGatingRelativeThreasholdChannel)
{
    ui->setupUi(this);
    ui->pushButton_PostGateMute->setProperty("form","Mute");
}

ConfigGatingRelativeThreasholdChannel::~ConfigGatingRelativeThreasholdChannel()
{
    delete ui;
}

void ConfigGatingRelativeThreasholdChannel::setTitle(QString title)
{
    ui->label_title->setText(title);
}

void ConfigGatingRelativeThreasholdChannel::setVu(int value)
{
    int vu = value * 0.01 - 100;
    ui->Disp_Level_1->setLCurValue(vu);
    ui->label_level->setText(QString::asprintf("%d dB", vu));
}

void ConfigGatingRelativeThreasholdChannel::setOpen(bool checked)
{
    ui->pushButton_Open->setChecked(checked);
}

void ConfigGatingRelativeThreasholdChannel::setPostGateGain(int value)
{
    ui->dial_PostGateGain->setValue(value);
//    ui->label_PostGateGainDisp->setText(QString::asprintf("%d dB", value));
}

void ConfigGatingRelativeThreasholdChannel::setPostGateMute(bool checked)
{
    ui->pushButton_PostGateMute->setChecked(checked);
}

void ConfigGatingRelativeThreasholdChannel::setDefault(bool checked)
{
    ui->pushButton_Default->setChecked(checked);
}

void ConfigGatingRelativeThreasholdChannel::setManual(bool checked)
{
    ui->pushButton_Manual->setChecked(checked);
}

void ConfigGatingRelativeThreasholdChannel::on_dial_PostGateGain_valueChanged(int value)
{
//    ui->label_PostGateGainDisp->setText(QString::asprintf("%d dB", value));
    emit PostGateGainValueChanged(value);
}

void ConfigGatingRelativeThreasholdChannel::on_pushButton_Default_clicked(bool checked)
{
    emit DefaultClicked(checked);
}

void ConfigGatingRelativeThreasholdChannel::on_pushButton_Manual_clicked(bool checked)
{
    emit ManualClicked(checked);
}

void ConfigGatingRelativeThreasholdChannel::on_pushButton_PostGateMute_clicked(bool checked)
{
    emit PostGateMuteClicked(checked);
}
