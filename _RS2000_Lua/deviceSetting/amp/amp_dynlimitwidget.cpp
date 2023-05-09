#include "amp_dynlimitwidget.h"
#include "ui_amp_dynlimitwidget.h"

amp_dynLimitWidget::amp_dynLimitWidget(QWidget *parent) :
    DevSettingWidget(parent),
    ui(new Ui::amp_dynLimitWidget)
{
    ui->setupUi(this);
    ui->widget->setType(CFG_TYPE_DYN_LIMIT);
}

amp_dynLimitWidget::~amp_dynLimitWidget()
{
    delete ui;
}

void amp_dynLimitWidget::showWidget(bool isIn, quint8 ch, QString chName, DynData &dynData)
{
    _dyndata = dynData;
    getDispData();

    ui->slider_Threshold->setValue(_xValue[1] + 120);
    qreal realValue = _xValue[1] / 10.0;
    ui->lineEdit_Threshold->setText(QString::number(realValue, 'f', 1));
    ui->widget->thresholdChanged(realValue + 12);

    connect(ui->slider_Threshold, SIGNAL(valueChanged(int)), this, SLOT(processSilderSlot(int)));//该信号在拖动滑动条的时候触发
    connect(ui->lineEdit_Threshold, SIGNAL(editingFinished()), this, SLOT(processLineEditSlot()));//该信号在编辑框失去焦点或者获得焦点的时候触发
    show();
}

void amp_dynLimitWidget::processSilderSlot(int value)
{
    _xValue[1] = value;
    qreal realValue = value / 10.0 - 12;

    ui->lineEdit_Threshold->setText(QString::number(realValue, 'f', 1));
    ui->widget->thresholdChanged(realValue+12);

    _dyndata.th1 = (_xValue[1] / 10 - 12) * 256;
    emit dynChanged(1, true, _dyndata);
}

void amp_dynLimitWidget::processLineEditSlot()
{
    double dValue = ui->lineEdit_Threshold->text().toDouble();
    int iValue = dValue * 10;

    if (iValue > ui->slider_Threshold->maximum())
    {
        iValue = ui->slider_Threshold->maximum();
        dValue = iValue / 10.0;
    }
    else if (iValue < ui->slider_Threshold->minimum())
    {
        iValue = ui->slider_Threshold->minimum();
        dValue = iValue / 10.0;
    }

    _xValue[1] = iValue;
    ui->slider_Threshold->setValue(iValue);
    ui->lineEdit_Threshold->setText(QString::number(dValue, 'f', 1));
    ui->widget->thresholdChanged(dValue+12);

    _dyndata.th1 = (_xValue[1] / 10 - 12)* 256;
    emit dynChanged(1, true, _dyndata);
}

void amp_dynLimitWidget::getDispData()
{
    _xValue[1] = _dyndata.th1/256.0 * 10;
}
