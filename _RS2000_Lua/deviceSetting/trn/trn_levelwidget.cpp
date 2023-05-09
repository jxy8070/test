#include "stable.h"
#include "trn_levelwidget.h"
#include "ui_trn_levelwidget.h"

Trn_LevelWidget::Trn_LevelWidget(bool isInput, QWidget *parent) :
    DevSettingWidget(parent),
    _ui(new Ui::Trn_LevelWidget)
{
    _ui->setupUi(this);
    hide();
    _isInit = false;
    connect(_ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_2, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_3, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_4, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_5, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_6, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_7, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_8, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_9, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_10, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_11, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_12, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_13, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_14, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_15, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_16, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_17, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_18, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_19, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
    connect(_ui->doubleSpinBox_20, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));

    connect(_ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_4, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_5, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_6, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_7, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_8, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_9, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_10, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_11, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_12, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_13, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_14, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_15, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_16, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_17, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_18, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_19, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_20, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));

    connect(_ui->verticalSlider, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_2, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_3, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_4, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_5, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_6, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_7, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_8, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_9, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_10, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_11, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_12, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_13, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_14, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_15, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_16, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_17, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_18, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_19, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));
    connect(_ui->verticalSlider_20, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));

    if (isInput == true)
    {
        _ui->Title->setText(tr("In Level Control(dB)"));
        _ui->label->setText(QString("A-In 1"));
        _ui->label_2->setText(QString("A-In 2"));
        _ui->label_3->setText(QString("A-In 3"));
        _ui->label_4->setText(QString("A-In 4"));
        _ui->label_5->setText(QString("A-In 5"));
        _ui->label_6->setText(QString("A-In 6"));
        _ui->label_7->setText(QString("A-In 7"));
        _ui->label_8->setText(QString("A-In 8"));
        _ui->label_9->setText(QString("D-In 1L"));
        _ui->label_10->setText(QString("D-In 1R"));
        _ui->label_11->setText(QString("D-In 2L"));
        _ui->label_12->setText(QString("D-In 2R"));
        _ui->label_13->setText(QString("NET-In 1"));
        _ui->label_14->setText(QString("NET-In 2"));
        _ui->label_15->setText(QString("NET-In 3"));
        _ui->label_16->setText(QString("NET-In 4"));
        _ui->label_17->setText(QString("NET-In 5"));
        _ui->label_18->setText(QString("NET-In 6"));
        _ui->label_19->setText(QString("NET-In 7"));
        _ui->label_20->setText(QString("NET-In 8"));
    }
    else
    {
        _ui->Title->setText(tr("Out Level Control(dB)"));
        _ui->label->setText(QString("A-Out 1"));
        _ui->label_2->setText(QString("A-Out 2"));
        _ui->label_3->setText(QString("A-Out 3"));
        _ui->label_4->setText(QString("A-Out 4"));
        _ui->label_5->setText(QString("A-Out 5"));
        _ui->label_6->setText(QString("A-Out 6"));
        _ui->label_7->setText(QString("A-Out 7"));
        _ui->label_8->setText(QString("A-Out 8"));
        _ui->label_9->setText(QString("D-Out 1L"));
        _ui->label_10->setText(QString("D-Out 1R"));
        _ui->label_11->setText(QString("D-Out 2L"));
        _ui->label_12->setText(QString("D-Out 2R"));
        _ui->label_13->setText(QString("NET-Out 1"));
        _ui->label_14->setText(QString("NET-Out 2"));
        _ui->label_15->setText(QString("NET-Out 3"));
        _ui->label_16->setText(QString("NET-Out 4"));
        _ui->label_17->setText(QString("NET-Out 5"));
        _ui->label_18->setText(QString("NET-Out 6"));
        _ui->label_19->setText(QString("NET-Out 7"));
        _ui->label_20->setText(QString("NET-Out 8"));
    }
}

Trn_LevelWidget::~Trn_LevelWidget()
{
    delete _ui;
}

void Trn_LevelWidget::showWidget(quint32 *CurrentVluList)
{
    _isInit = true;
    _ui->doubleSpinBox->setValue(20.0 * log10(CurrentVluList[0] / 131072.0));
    _ui->doubleSpinBox_2->setValue(20.0 * log10(CurrentVluList[1] / 131072.0));
    _ui->doubleSpinBox_3->setValue(20.0 * log10(CurrentVluList[2] / 131072.0));
    _ui->doubleSpinBox_4->setValue(20.0 * log10(CurrentVluList[3] / 131072.0));
    _ui->doubleSpinBox_5->setValue(20.0 * log10(CurrentVluList[4] / 131072.0));
    _ui->doubleSpinBox_6->setValue(20.0 * log10(CurrentVluList[5] / 131072.0));
    _ui->doubleSpinBox_7->setValue(20.0 * log10(CurrentVluList[6] / 131072.0));
    _ui->doubleSpinBox_8->setValue(20.0 * log10(CurrentVluList[7] / 131072.0));
    _ui->doubleSpinBox_9->setValue(20.0 * log10(CurrentVluList[8] / 131072.0));
    _ui->doubleSpinBox_10->setValue(20.0 * log10(CurrentVluList[9] / 131072.0));
    _ui->doubleSpinBox_11->setValue(20.0 * log10(CurrentVluList[10] / 131072.0));
    _ui->doubleSpinBox_12->setValue(20.0 * log10(CurrentVluList[11] / 131072.0));
    _ui->doubleSpinBox_13->setValue(20.0 * log10(CurrentVluList[12] / 131072.0));
    _ui->doubleSpinBox_14->setValue(20.0 * log10(CurrentVluList[13] / 131072.0));
    _ui->doubleSpinBox_15->setValue(20.0 * log10(CurrentVluList[14] / 131072.0));
    _ui->doubleSpinBox_16->setValue(20.0 * log10(CurrentVluList[15] / 131072.0));
    _ui->doubleSpinBox_17->setValue(20.0 * log10(CurrentVluList[16] / 131072.0));
    _ui->doubleSpinBox_18->setValue(20.0 * log10(CurrentVluList[17] / 131072.0));
    _ui->doubleSpinBox_19->setValue(20.0 * log10(CurrentVluList[18] / 131072.0));
    _ui->doubleSpinBox_20->setValue(20.0 * log10(CurrentVluList[19] / 131072.0));
    _isInit = false;
    show();
}

//最终处理推子值
void Trn_LevelWidget::spinBoxValueChanged(double vlu)
{
    if (sender() == _ui->doubleSpinBox)
    {
        _ui->verticalSlider->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(0, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_2)
    {
        _ui->verticalSlider_2->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(1, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_3)
    {
        _ui->verticalSlider_3->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(2, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_4)
    {
        _ui->verticalSlider_4->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(3, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_5)
    {
        _ui->verticalSlider_5->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(4, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_6)
    {
        _ui->verticalSlider_6->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(5, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_7)
    {
        _ui->verticalSlider_7->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(6, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_8)
    {
        _ui->verticalSlider_8->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(7, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_9)
    {
        _ui->verticalSlider_9->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(8, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_10)
    {
        _ui->verticalSlider_10->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(9, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_11)
    {
        _ui->verticalSlider_11->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(10, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_12)
    {
        _ui->verticalSlider_12->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(11, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_13)
    {
        _ui->verticalSlider_13->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(12, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_14)
    {
        _ui->verticalSlider_14->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(13, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_15)
    {
        _ui->verticalSlider_15->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(14, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_16)
    {
        _ui->verticalSlider_16->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(15, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_17)
    {
        _ui->verticalSlider_17->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(16, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_18)
    {
        _ui->verticalSlider_18->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(17, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_19)
    {
        _ui->verticalSlider_19->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(18, vlu);
        }
    }
    else if (sender() == _ui->doubleSpinBox_20)
    {
        _ui->verticalSlider_20->setValue(vlu*10);
        if (!_isInit)
        {
            emit levelChanged(19, vlu);
        }
    }
}

/*
 * 实现slider和spinBox联动
 */
void Trn_LevelWidget::sliderValueChanged(int vlu)
{
    if (sender() == _ui->verticalSlider)
    {
        _ui->doubleSpinBox->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_2)
    {
        _ui->doubleSpinBox_2->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_3)
    {
        _ui->doubleSpinBox_3->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_4)
    {
        _ui->doubleSpinBox_4->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_5)
    {
        _ui->doubleSpinBox_5->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_6)
    {
        _ui->doubleSpinBox_6->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_7)
    {
        _ui->doubleSpinBox_7->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_8)
    {
        _ui->doubleSpinBox_8->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_9)
    {
        _ui->doubleSpinBox_9->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_10)
    {
        _ui->doubleSpinBox_10->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_11)
    {
        _ui->doubleSpinBox_11->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_12)
    {
        _ui->doubleSpinBox_12->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_13)
    {
        _ui->doubleSpinBox_13->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_14)
    {
        _ui->doubleSpinBox_14->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_15)
    {
        _ui->doubleSpinBox_15->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_16)
    {
        _ui->doubleSpinBox_16->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_17)
    {
        _ui->doubleSpinBox_17->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_18)
    {
        _ui->doubleSpinBox_18->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_19)
    {
        _ui->doubleSpinBox_19->setValue(vlu/10.0);
    }
    else if (sender() == _ui->verticalSlider_20)
    {
        _ui->doubleSpinBox_20->setValue(vlu/10.0);
    }
}
