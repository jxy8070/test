#include "configmeter_fangtu.h"
#include "ui_configmeter_fangtu.h"

ConfigMeter_FangTu::ConfigMeter_FangTu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigMeter_FangTu)
{
    ui->setupUi(this);
}

ConfigMeter_FangTu::~ConfigMeter_FangTu()
{
    delete ui;
}

void ConfigMeter_FangTu::init(int channel, int holdTime, bool holdEnable, bool infiniteHold, QString name)
{
    _channel = channel;
    ui->label_ch->setText(QString("CH%1").arg(_channel + 1));

    ui->holdTime->setRangeParam(1, 1000, StepMode_Log10, ParamType_Freq);
    ui->holdTime->setValue(holdTime);
    ui->holdEnable->setChecked(holdEnable);
    ui->InfiniteHold->setChecked(infiniteHold);
    ui->lineEdit->setText(name);

    connect(ui->holdTime, SIGNAL(valueChanged(int)), this, SLOT(onSpinboxProcess(int)));
    connect(ui->holdEnable, SIGNAL(clicked(bool)), this, SLOT(onPushbuttonProcess(bool)));
    connect(ui->InfiniteHold, SIGNAL(clicked(bool)), this, SLOT(onPushbuttonProcess(bool)));
    connect(ui->lineEdit, SIGNAL(editingFinished()), this, SLOT(onLineEditProcess()));
}

void ConfigMeter_FangTu::meterUpdate(double vol, double ppm)
{
    double validVal = ui->Volume->getValidValue(vol);
    ui->Volume->setPPM(validVal, ppm);
    ui->volumeLabel->setText(tr("%1").arg(QString::number(validVal, 'f', 1)));
}

void ConfigMeter_FangTu::onSpinboxProcess(int value)
{
    if (sender() == ui->holdTime)
    {
        emit holdTimeChanged(_channel, value);
    }
}

void ConfigMeter_FangTu::onPushbuttonProcess(bool checked)
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

void ConfigMeter_FangTu::onLineEditProcess()
{
    if (sender() == ui->lineEdit)
    {
        emit userNameChanged(_channel, ui->lineEdit->text());
    }
}
