#include "configmeter.h"
#include "ui_configmeter.h"

ConfigMeter::ConfigMeter(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ConfigMeter)
{
    ui->setupUi(this);
}

ConfigMeter::~ConfigMeter()
{
//    qDebug() << "ConfigMeter::~ConfigMeter";
    delete ui;
}

void ConfigMeter::init(int channel, int holdTime, bool holdEnable, bool infiniteHold, QString name)
{
    _channel = channel;
    this->setTitle(QString("CH%1").arg(_channel + 1));

    ui->holdTime->setRangeParam(1, 1000, StepMode_Log10, ParamType_Time);
    ui->holdTime->setFixedSize(40, 40);
    ui->holdTime->setValue(holdTime);
    ui->holdEnable->setChecked(holdEnable);
    ui->InfiniteHold->setChecked(infiniteHold);
    ui->lineEdit->setText(name);

    connect(ui->holdTime, SIGNAL(valueChanged(int)), this, SLOT(onSpinboxProcess(int)));
    connect(ui->holdEnable, SIGNAL(clicked(bool)), this, SLOT(onPushbuttonProcess(bool)));
    connect(ui->InfiniteHold, SIGNAL(clicked(bool)), this, SLOT(onPushbuttonProcess(bool)));
    connect(ui->lineEdit, SIGNAL(editingFinished()), this, SLOT(onLineEditProcess()));
}

void ConfigMeter::meterUpdate(double vol, double ppm)
{
    double validVal = ui->Volume->getValidValue(vol);
    ui->Volume->setPPM(validVal, ppm);
    ui->volumeLabel->setText(tr("%1 dBFS").arg(QString::number(validVal, 'f', 1)));
}

void ConfigMeter::onSpinboxProcess(int value)
{
    if (sender() == ui->holdTime)
    {
        emit holdTimeChanged(_channel, value);
    }
}

void ConfigMeter::onPushbuttonProcess(bool checked)
{
    if (sender() == ui->holdEnable)
    {
        emit holdEnableChanged(_channel, checked);
    }
    else if (sender() == ui->InfiniteHold)
    {
        emit infiniteHoldChanged(_channel, checked);
    }
}

void ConfigMeter::onLineEditProcess()
{
    if (sender() == ui->lineEdit)
    {
        emit userNameChanged(_channel, ui->lineEdit->text());
    }
}
