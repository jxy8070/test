#include "eqviewnormalseg.h"
#include "ui_eqviewnormalseg.h"

EqviewNormalSeg::EqviewNormalSeg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EqviewNormalSeg)
{
    ui->setupUi(this);

    ui->spinBoxFreq->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
    ui->spinBoxFreq->setFixedSize(50, 50);
    ui->spinBoxFreq->setDecimalsNum(0);
    ui->spinBoxFreq->setFangTuIgnoreType(false);

    ui->spinBoxGain->setRangeParam(-30, 15, StepMode_Line, ParamType_Gain);
    ui->spinBoxGain->setFixedSize(50, 50);
    ui->spinBoxGain->setDecimalsNum(1);
    ui->spinBoxGain->setFangTuIgnoreType(false);

    ui->spinBoxQ->setRangeParam(0.05, 20, StepMode_Log10, ParamType_Phase);
    ui->spinBoxQ->setFixedSize(50, 50);
    ui->spinBoxQ->setDecimalsNum(2);
    ui->spinBoxQ->setFangTuIgnoreType(false);
    ui->pushButton->setProperty("form", "Bypass");
    ui->pushButton->setFixedSize(50, 18);

    connect(ui->spinBoxFreq, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxFreq_valueChanged(int)));
    connect(ui->spinBoxGain, SIGNAL(valueChanged(double)), this, SLOT(onSpinBoxGain_valueChanged(double)));
    connect(ui->spinBoxQ, SIGNAL(valueChanged(double)), this, SLOT(onSpinBoxQ_valueChanged(double)));
}

EqviewNormalSeg::~EqviewNormalSeg()
{
    delete ui;
}

void EqviewNormalSeg::select()
{
    ui->label->setStyleSheet("border: 1px solid #FF2020;border-radius: 4px;");
}

void EqviewNormalSeg::unselect()
{
    ui->label->setStyleSheet("border: 1px solid transparent");
}

void EqviewNormalSeg::Set_Title(QString title)
{
    ui->label->setText(title);
}

void EqviewNormalSeg::Set_Bypass(bool enable)
{
    ui->pushButton->setChecked(/*!*/enable);
}

void EqviewNormalSeg::Set_Frequency(int value)
{
    ui->spinBoxFreq->setValue(value);
}

void EqviewNormalSeg::Set_Gain(double value)
{
//    qDebug() << __FUNCTION__ << __LINE__
//             << value;
    ui->spinBoxGain->setValue(value);
}

void EqviewNormalSeg::Set_Bandwidth(double value)
{
//    qDebug() << __FUNCTION__ << __LINE__
//             << value;
    ui->spinBoxQ->setValue(value);
}

void EqviewNormalSeg::set_eqType(int type)
{
    ui->comboBox_EqType->setCurrentIndex(type);
}

bool EqviewNormalSeg::Get_bypass()
{
    return /*!*/ui->pushButton->isChecked();
}

int EqviewNormalSeg::Get_Frequency()
{
    return ui->spinBoxFreq->value();
}

double EqviewNormalSeg::Get_Gain()
{
    return ui->spinBoxGain->value();
}

double EqviewNormalSeg::Get_Bandwidth()
{
    return ui->spinBoxQ->value();
}

int EqviewNormalSeg::get_eqType()
{
    return ui->comboBox_EqType->currentIndex();
}

void EqviewNormalSeg::on_pushButton_clicked(bool checked)
{
    emit Bypass_Clicked(/*!*/checked);
    emit FreqGainBandwidth_ValueChanged();
}

void EqviewNormalSeg::onSpinBoxFreq_valueChanged(int arg1)
{
    emit FreqGainBandwidth_ValueChanged();
}

void EqviewNormalSeg::onSpinBoxGain_valueChanged(double arg1)
{
    emit FreqGainBandwidth_ValueChanged();
}

void EqviewNormalSeg::onSpinBoxQ_valueChanged(double arg1)
{
    emit FreqGainBandwidth_ValueChanged();
}

void EqviewNormalSeg::on_comboBox_EqType_currentIndexChanged(int index)
{
    emit FreqGainBandwidth_ValueChanged();
}

