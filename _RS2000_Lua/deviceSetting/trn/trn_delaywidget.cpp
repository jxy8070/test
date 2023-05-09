#include "stable.h"
#include "trn_delaywidget.h"
#include "ui_trn_delaywidget.h"


Trn_DelayWidget::Trn_DelayWidget(bool isInput, QWidget *parent) :
    DevSettingWidget(parent),
    _ui(new Ui::Trn_DelayWidget)
{
    _ui->setupUi(this);
    hide();

//    QList<QSpinBox*> spinBoxList = findChildren<QSpinBox*>();
//    foreach(QSpinBox* spinBox,  spinBoxList)
//    {
//    }

/*
 * 实现slider和spinBox联动
 */
    connect(_ui->slider1, SIGNAL(valueChanged(int)), _ui->spinBox1, SLOT(setValue(int)));
    connect(_ui->slider2, SIGNAL(valueChanged(int)), _ui->spinBox2, SLOT(setValue(int)));
    connect(_ui->slider3, SIGNAL(valueChanged(int)), _ui->spinBox3, SLOT(setValue(int)));
    connect(_ui->slider4, SIGNAL(valueChanged(int)), _ui->spinBox4, SLOT(setValue(int)));
    connect(_ui->slider5, SIGNAL(valueChanged(int)), _ui->spinBox5, SLOT(setValue(int)));
    connect(_ui->slider6, SIGNAL(valueChanged(int)), _ui->spinBox6, SLOT(setValue(int)));
    connect(_ui->slider7, SIGNAL(valueChanged(int)), _ui->spinBox7, SLOT(setValue(int)));
    connect(_ui->slider8, SIGNAL(valueChanged(int)), _ui->spinBox8, SLOT(setValue(int)));
    connect(_ui->slider9, SIGNAL(valueChanged(int)), _ui->spinBox9, SLOT(setValue(int)));
    connect(_ui->slider10, SIGNAL(valueChanged(int)), _ui->spinBox10, SLOT(setValue(int)));
    connect(_ui->slider11, SIGNAL(valueChanged(int)), _ui->spinBox11, SLOT(setValue(int)));
    connect(_ui->slider12, SIGNAL(valueChanged(int)), _ui->spinBox12, SLOT(setValue(int)));
    connect(_ui->slider13, SIGNAL(valueChanged(int)), _ui->spinBox13, SLOT(setValue(int)));
    connect(_ui->slider14, SIGNAL(valueChanged(int)), _ui->spinBox14, SLOT(setValue(int)));
    connect(_ui->slider15, SIGNAL(valueChanged(int)), _ui->spinBox15, SLOT(setValue(int)));
    connect(_ui->slider16, SIGNAL(valueChanged(int)), _ui->spinBox16, SLOT(setValue(int)));
    connect(_ui->slider17, SIGNAL(valueChanged(int)), _ui->spinBox17, SLOT(setValue(int)));
    connect(_ui->slider18, SIGNAL(valueChanged(int)), _ui->spinBox18, SLOT(setValue(int)));
    connect(_ui->slider19, SIGNAL(valueChanged(int)), _ui->spinBox19, SLOT(setValue(int)));
    connect(_ui->slider20, SIGNAL(valueChanged(int)), _ui->spinBox20, SLOT(setValue(int)));

    connect(_ui->slider1, SIGNAL(sliderMoved(int)), _ui->spinBox1, SLOT(setValue(int)));
    connect(_ui->slider2, SIGNAL(sliderMoved(int)), _ui->spinBox2, SLOT(setValue(int)));
    connect(_ui->slider3, SIGNAL(sliderMoved(int)), _ui->spinBox3, SLOT(setValue(int)));
    connect(_ui->slider4, SIGNAL(sliderMoved(int)), _ui->spinBox4, SLOT(setValue(int)));
    connect(_ui->slider5, SIGNAL(sliderMoved(int)), _ui->spinBox5, SLOT(setValue(int)));
    connect(_ui->slider6, SIGNAL(sliderMoved(int)), _ui->spinBox6, SLOT(setValue(int)));
    connect(_ui->slider7, SIGNAL(sliderMoved(int)), _ui->spinBox7, SLOT(setValue(int)));
    connect(_ui->slider8, SIGNAL(sliderMoved(int)), _ui->spinBox8, SLOT(setValue(int)));
    connect(_ui->slider9, SIGNAL(sliderMoved(int)), _ui->spinBox9, SLOT(setValue(int)));
    connect(_ui->slider10, SIGNAL(sliderMoved(int)), _ui->spinBox10, SLOT(setValue(int)));
    connect(_ui->slider11, SIGNAL(sliderMoved(int)), _ui->spinBox11, SLOT(setValue(int)));
    connect(_ui->slider12, SIGNAL(sliderMoved(int)), _ui->spinBox12, SLOT(setValue(int)));
    connect(_ui->slider13, SIGNAL(sliderMoved(int)), _ui->spinBox13, SLOT(setValue(int)));
    connect(_ui->slider14, SIGNAL(sliderMoved(int)), _ui->spinBox14, SLOT(setValue(int)));
    connect(_ui->slider15, SIGNAL(sliderMoved(int)), _ui->spinBox15, SLOT(setValue(int)));
    connect(_ui->slider16, SIGNAL(sliderMoved(int)), _ui->spinBox16, SLOT(setValue(int)));
    connect(_ui->slider17, SIGNAL(sliderMoved(int)), _ui->spinBox17, SLOT(setValue(int)));
    connect(_ui->slider18, SIGNAL(sliderMoved(int)), _ui->spinBox18, SLOT(setValue(int)));
    connect(_ui->slider19, SIGNAL(sliderMoved(int)), _ui->spinBox19, SLOT(setValue(int)));
    connect(_ui->slider20, SIGNAL(sliderMoved(int)), _ui->spinBox20, SLOT(setValue(int)));

    connect(_ui->spinBox1, SIGNAL(valueChanged(int)), _ui->slider1, SLOT(setValue(int)));
    connect(_ui->spinBox2, SIGNAL(valueChanged(int)), _ui->slider2, SLOT(setValue(int)));
    connect(_ui->spinBox3, SIGNAL(valueChanged(int)), _ui->slider3, SLOT(setValue(int)));
    connect(_ui->spinBox4, SIGNAL(valueChanged(int)), _ui->slider4, SLOT(setValue(int)));
    connect(_ui->spinBox5, SIGNAL(valueChanged(int)), _ui->slider5, SLOT(setValue(int)));
    connect(_ui->spinBox6, SIGNAL(valueChanged(int)), _ui->slider6, SLOT(setValue(int)));
    connect(_ui->spinBox7, SIGNAL(valueChanged(int)), _ui->slider7, SLOT(setValue(int)));
    connect(_ui->spinBox8, SIGNAL(valueChanged(int)), _ui->slider8, SLOT(setValue(int)));
    connect(_ui->spinBox9, SIGNAL(valueChanged(int)), _ui->slider9, SLOT(setValue(int)));
    connect(_ui->spinBox10, SIGNAL(valueChanged(int)), _ui->slider10, SLOT(setValue(int)));
    connect(_ui->spinBox11, SIGNAL(valueChanged(int)), _ui->slider11, SLOT(setValue(int)));
    connect(_ui->spinBox12, SIGNAL(valueChanged(int)), _ui->slider12, SLOT(setValue(int)));
    connect(_ui->spinBox13, SIGNAL(valueChanged(int)), _ui->slider13, SLOT(setValue(int)));
    connect(_ui->spinBox14, SIGNAL(valueChanged(int)), _ui->slider14, SLOT(setValue(int)));
    connect(_ui->spinBox15, SIGNAL(valueChanged(int)), _ui->slider15, SLOT(setValue(int)));
    connect(_ui->spinBox16, SIGNAL(valueChanged(int)), _ui->slider16, SLOT(setValue(int)));
    connect(_ui->spinBox17, SIGNAL(valueChanged(int)), _ui->slider17, SLOT(setValue(int)));
    connect(_ui->spinBox18, SIGNAL(valueChanged(int)), _ui->slider18, SLOT(setValue(int)));
    connect(_ui->spinBox19, SIGNAL(valueChanged(int)), _ui->slider19, SLOT(setValue(int)));
    connect(_ui->spinBox20, SIGNAL(valueChanged(int)), _ui->slider20, SLOT(setValue(int)));

    if (isInput)
    {
        _ui->Title->setText(tr("In Delay Control(ms)"));
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
        _ui->Title->setText(tr("Out Delay Control(ms)"));
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

Trn_DelayWidget::~Trn_DelayWidget()
{
    delete _ui;
}

void Trn_DelayWidget::connectSig()
{
    connect(_ui->spinBox1, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox2, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox3, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox4, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox5, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox6, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox7, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox8, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox9, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox10, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox11, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox12, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox13, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox14, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox15, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox16, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox17, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox18, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox19, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    connect(_ui->spinBox20, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
}

void Trn_DelayWidget::disconnectSig()
{
    disconnect(_ui->spinBox1, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox2, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox3, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox4, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox5, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox6, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox7, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox8, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox9, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox10, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox11, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox12, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox13, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox14, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox15, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox16, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox17, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox18, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox19, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
    disconnect(_ui->spinBox20, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
}

void Trn_DelayWidget::showWidget(quint16 *delayValueList)
{
    _ui->spinBox1->setValue(delayValueList[0]);
    _ui->spinBox2->setValue(delayValueList[1]);
    _ui->spinBox3->setValue(delayValueList[2]);
    _ui->spinBox4->setValue(delayValueList[3]);
    _ui->spinBox5->setValue(delayValueList[4]);
    _ui->spinBox6->setValue(delayValueList[5]);
    _ui->spinBox7->setValue(delayValueList[6]);
    _ui->spinBox8->setValue(delayValueList[7]);
    _ui->spinBox9->setValue(delayValueList[8]);
    _ui->spinBox10->setValue(delayValueList[9]);
    _ui->spinBox11->setValue(delayValueList[10]);
    _ui->spinBox12->setValue(delayValueList[11]);
    _ui->spinBox13->setValue(delayValueList[12]);
    _ui->spinBox14->setValue(delayValueList[13]);
    _ui->spinBox15->setValue(delayValueList[14]);
    _ui->spinBox16->setValue(delayValueList[15]);
    _ui->spinBox17->setValue(delayValueList[16]);
    _ui->spinBox18->setValue(delayValueList[17]);
    _ui->spinBox19->setValue(delayValueList[18]);
    _ui->spinBox20->setValue(delayValueList[19]);

    connectSig();

    show();
}

//最终处理推子值
void Trn_DelayWidget::spinBoxValueChanged(int value)
{
    QString senderName = sender()->objectName();
    Q_ASSERT(senderName.startsWith(QLatin1String("spinBox")));

    quint8 ch = senderName.mid(7).toInt() - 1;
    emit delayChanged(ch, value);
}
