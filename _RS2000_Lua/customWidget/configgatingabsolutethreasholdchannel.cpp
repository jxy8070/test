#include "configgatingabsolutethreasholdchannel.h"
#include "ui_configgatingabsolutethreasholdchannel.h"
#include "config.h"

ConfigGatingAbsoluteThreasholdChannel::ConfigGatingAbsoluteThreasholdChannel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigGatingAbsoluteThreasholdChannel)
{
    ui->setupUi(this);

     ui->verticalLayout->setContentsMargins(3, 3, 3, 3);
     //    ui->pushButton_Open->setFixedSize(50, 18);
     //    ui->pushButton_Manual->setFixedSize(50, 18);

     ui->widget_GainWdg->setRangeParam(-100, 20, StepMode_Line, ParamType_Gain);
     connect(ui->widget_GainWdg, QOverload<double>::of(&ConfigGainWidget::valueChanged),
                this, &ConfigGatingAbsoluteThreasholdChannel::gainWdg_valueChanged);
}

ConfigGatingAbsoluteThreasholdChannel::~ConfigGatingAbsoluteThreasholdChannel()
{
    delete ui;
}

void ConfigGatingAbsoluteThreasholdChannel::setTitle(QString title)
{
    ui->label_title->setText(title);
}

void ConfigGatingAbsoluteThreasholdChannel::setManual(bool checked)
{
    ui->pushButton_Manual->setChecked(checked);
}

void ConfigGatingAbsoluteThreasholdChannel::setMode(int mode)
{
    ui->comboBox_Mode->setCurrentIndex(mode);
}

void ConfigGatingAbsoluteThreasholdChannel::setPriority(int value)
{
    ui->spinBox_Priority->setValue(value);
}

void ConfigGatingAbsoluteThreasholdChannel::setThreshold(int value)
{
    ui->widget_GainWdg->setValue(value);
}

void ConfigGatingAbsoluteThreasholdChannel::setOpen(bool checked)
{
    ui->pushButton_Open->setChecked(checked);
}


void ConfigGatingAbsoluteThreasholdChannel::on_pushButton_Manual_clicked(bool checked)
{
    emit ManualClicked(checked);
}

void ConfigGatingAbsoluteThreasholdChannel::on_comboBox_Mode_currentIndexChanged(int index)
{
    emit ModeValueChanged(index);
}

void ConfigGatingAbsoluteThreasholdChannel::on_spinBox_Priority_valueChanged(int arg1)
{
    emit PriorityValueChanged(arg1);
}

void ConfigGatingAbsoluteThreasholdChannel::gainWdg_valueChanged(double value)
{
    emit ThresholdValueChanged((int)value);
}
