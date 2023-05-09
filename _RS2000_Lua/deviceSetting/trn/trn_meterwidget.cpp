#include "stable.h"
#include "trn_meterwidget.h"
#include "ui_trn_meterwidget.h"

Trn_MeterWidget::Trn_MeterWidget(bool isInput, QWidget *parent) :
    DevSettingWidget(parent),
    _ui(new Ui::Trn_MeterWidget)
{
    _ui->setupUi(this);
    hide();

    m_dispList.append(_ui->Meter1);
    m_dispList.append(_ui->Meter1_2);
    m_dispList.append(_ui->Meter1_3);
    m_dispList.append(_ui->Meter1_4);
    m_dispList.append(_ui->Meter1_5);
    m_dispList.append(_ui->Meter1_6);
    m_dispList.append(_ui->Meter1_7);
    m_dispList.append(_ui->Meter1_8);
    m_dispList.append(_ui->Meter1_9);
    m_dispList.append(_ui->Meter1_10);
    m_dispList.append(_ui->Meter1_11);
    m_dispList.append(_ui->Meter1_12);
    m_dispList.append(_ui->Meter1_13);
    m_dispList.append(_ui->Meter1_14);
    m_dispList.append(_ui->Meter1_15);
    m_dispList.append(_ui->Meter1_16);
    m_dispList.append(_ui->Meter1_17);
    m_dispList.append(_ui->Meter1_18);
    m_dispList.append(_ui->Meter1_19);
    m_dispList.append(_ui->Meter1_20);

    m_dispLabelList.append(_ui->label1_2);
    m_dispLabelList.append(_ui->label2_2);
    m_dispLabelList.append(_ui->label3_2);
    m_dispLabelList.append(_ui->label4_2);
    m_dispLabelList.append(_ui->label5_2);
    m_dispLabelList.append(_ui->label6_2);
    m_dispLabelList.append(_ui->label7_2);
    m_dispLabelList.append(_ui->label8_2);
    m_dispLabelList.append(_ui->label9_2);
    m_dispLabelList.append(_ui->label10_2);
    m_dispLabelList.append(_ui->label11_2);
    m_dispLabelList.append(_ui->label12_2);
    m_dispLabelList.append(_ui->label13_2);
    m_dispLabelList.append(_ui->label14_2);
    m_dispLabelList.append(_ui->label15_2);
    m_dispLabelList.append(_ui->label16_2);
    m_dispLabelList.append(_ui->label17_2);
    m_dispLabelList.append(_ui->label18_2);
    m_dispLabelList.append(_ui->label19_2);
    m_dispLabelList.append(_ui->label20_2);

    for (int i = 0; i < 20; ++i)
    {
        m_dispList.value(i)->setMaxValue(20);
        m_dispList.value(i)->setMinValue(-42);
    }

    if (isInput)
    {
        _ui->Title->setText(tr("In Meter"));
        _ui->label1->setText(QString("A-In 1"));
        _ui->label2->setText(QString("A-In 2"));
        _ui->label3->setText(QString("A-In 3"));
        _ui->label4->setText(QString("A-In 4"));
        _ui->label5->setText(QString("A-In 5"));
        _ui->label6->setText(QString("A-In 6"));
        _ui->label7->setText(QString("A-In 7"));
        _ui->label8->setText(QString("A-In 8"));
        _ui->label9->setText(QString("D-In 1L"));
        _ui->label10->setText(QString("D-In 1R"));
        _ui->label11->setText(QString("D-In 2L"));
        _ui->label12->setText(QString("D-In 2R"));
        _ui->label13->setText(QString("NET-In 1"));
        _ui->label14->setText(QString("NET-In 2"));
        _ui->label15->setText(QString("NET-In 3"));
        _ui->label16->setText(QString("NET-In 4"));
        _ui->label17->setText(QString("NET-In 5"));
        _ui->label18->setText(QString("NET-In 6"));
        _ui->label19->setText(QString("NET-In 7"));
        _ui->label20->setText(QString("NET-In 8"));
    }
    else
    {
        _ui->Title->setText(tr("Out Meter"));
        _ui->label1->setText(QString("A-Out 1"));
        _ui->label2->setText(QString("A-Out 2"));
        _ui->label3->setText(QString("A-Out 3"));
        _ui->label4->setText(QString("A-Out 4"));
        _ui->label5->setText(QString("A-Out 5"));
        _ui->label6->setText(QString("A-Out 6"));
        _ui->label7->setText(QString("A-Out 7"));
        _ui->label8->setText(QString("A-Out 8"));
        _ui->label9->setText(QString("D-Out 1L"));
        _ui->label10->setText(QString("D-Out 1R"));
        _ui->label11->setText(QString("D-Out 2L"));
        _ui->label12->setText(QString("D-Out 2R"));
        _ui->label13->setText(QString("NET-Out 1"));
        _ui->label14->setText(QString("NET-Out 2"));
        _ui->label15->setText(QString("NET-Out 3"));
        _ui->label16->setText(QString("NET-Out 4"));
        _ui->label17->setText(QString("NET-Out 5"));
        _ui->label18->setText(QString("NET-Out 6"));
        _ui->label19->setText(QString("NET-Out 7"));
        _ui->label20->setText(QString("NET-Out 8"));
    }
}

Trn_MeterWidget::~Trn_MeterWidget()
{
    delete _ui;
}

void Trn_MeterWidget::dispMeter(quint16 *vlu)
{
    for (int i = 0; i < 20; ++i)
    {
        float MeterT = -(vlu[i] / 512.0) + 20.0;
//        qDebug()<<"Meter"<<i<<MeterT;
        m_dispList.value(i)->setLCurValue(MeterT);
        m_dispLabelList.value(i)->setText(QString::asprintf("%.1f", MeterT));
    }
}
