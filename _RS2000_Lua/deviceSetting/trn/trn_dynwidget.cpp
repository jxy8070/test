#include "stable.h"
#include "trn_dynwidget.h"
#include "ui_trn_dynwidget.h"
#include "ui_Trn_DynWidget_FangTu.h"

Trn_DynWidget::Trn_DynWidget(QWidget *parent) :
    DevSettingWidget(parent),
    #if Logo_FangTu
        _ui(new Ui::Trn_DynWidget_FangTu)
    #else
        _ui(new Ui::Trn_DynWidget)
    #endif
{
    _ui->setupUi(this);
    hide();

    initDynData();
    connectSig(DynType_ALL);
    display(DynType_ALL);
}

Trn_DynWidget::~Trn_DynWidget()
{
    delete _ui;
}

void Trn_DynWidget::initDynData()
{
    _xValue[0] = 50;
    _yValue[0] = 450;
    _xValue[1] = 200;
    _yValue[1] = 300;
    _xValue[2] = 300;
    _yValue[2] = 200;
    _xValue[3] = 450;
    _yValue[3] = 50;

    float f;
    f = transY(_yValue[1]) - transX(_xValue[1]);
    _dyndata.offset1 = ((int)(f * 256)) & 0xFFFF;

    f = transY(_yValue[2]) - transX(_xValue[2]);
    _dyndata.offset2 = ((int)(f * 256)) & 0xFFFF;

    f = (transY(_yValue[1])-transY(_yValue[0]))/(transX(_xValue[1])-transX(_xValue[0]) + 1e-20);
    if(f > 16.0f)
        f = 16.0f;
    _dyndata.nRatio = ((int)(f * 256)) & 0xFFFF;

    f = (transY(_yValue[3])-transY(_yValue[2]))/(transX(_xValue[3])-transX(_xValue[2]) + 1e-20);
    if(f > 16.0f)
        f = 16.0f;
    _dyndata.ratio = ((int)(f * 256)) & 0xFFFF;

    _dyndata.th1 = ((int)(transX(_xValue[1])*256)) & 0xFFFF;

    _dyndata.th2 = ((int)(transX(_xValue[2])*256)) & 0xFFFF;

    _dyndata.atk = 50;
    _dyndata.release = 500;
    _dyndata.bypass = true;
}

void Trn_DynWidget::connectSig(DynTypeEnum type)
{
    switch(type)
    {
    case DynType_ALL:
        connect(_ui->byPass, SIGNAL(clicked()), this, SLOT(byPassFunc()));
        connect(_ui->atkBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxChanged()));
        connect(_ui->rlsBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxChanged()));

        connect(_ui->dynControl, SIGNAL(changeValue(int*,int*)), this, SLOT(getDynValue(int*,int*)));

        connect(_ui->x1_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->x2_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->x3_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->x4_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->y1_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->y2_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->y3_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->y4_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        break;
    case DynType_Grap:
        connect(_ui->dynControl, SIGNAL(changeValue(int*,int*)), this, SLOT(getDynValue(int*,int*)));
        break;
    case DynType_Spin:
        connect(_ui->x1_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->x2_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->x3_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->x4_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->y1_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->y2_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->y3_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        connect(_ui->y4_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        break;
    default:
        break;
    }
}

void Trn_DynWidget::disconnectSig(DynTypeEnum type)
{
    switch(type)
    {
    case DynType_ALL:
        disconnect(_ui->byPass, SIGNAL(clicked()), this, SLOT(byPassFunc()));
        disconnect(_ui->atkBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxChanged()));
        disconnect(_ui->rlsBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxChanged()));

        disconnect(_ui->dynControl, SIGNAL(changeValue(int*,int*)), this, SLOT(getDynValue(int*,int*)));

        disconnect(_ui->x1_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->x2_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->x3_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->x4_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->y1_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->y2_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->y3_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->y4_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        break;
    case DynType_Grap:
        disconnect(_ui->dynControl, SIGNAL(changeValue(int*,int*)), this, SLOT(getDynValue(int*,int*)));
        break;
    case DynType_Spin:
        disconnect(_ui->x1_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->x2_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->x3_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->x4_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->y1_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->y2_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->y3_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        disconnect(_ui->y4_label, SIGNAL(valueChanged(double)), this, SLOT(setDynValue()));
        break;
    default:
        break;
    }
}

void Trn_DynWidget::setSpinBoxLimit()
{
#if Logo_FangTu
    _ui->x1_label->setMaxValue(_ui->x2_label->value());
    _ui->y1_label->setMaxValue(_ui->y2_label->value());

    _ui->x2_label->setMinValue(_ui->x1_label->value());
    _ui->x2_label->setMaxValue(_ui->x3_label->value());
    _ui->y2_label->setMinValue(_ui->y1_label->value());
    _ui->y2_label->setMaxValue(_ui->y3_label->value());

    _ui->x3_label->setMinValue(_ui->x2_label->value());
    _ui->x3_label->setMaxValue(_ui->x4_label->value());
    _ui->y3_label->setMinValue(_ui->y2_label->value());
    _ui->y3_label->setMaxValue(_ui->y4_label->value());

    _ui->x4_label->setMinValue(_ui->x3_label->value());
    _ui->y4_label->setMinValue(_ui->y3_label->value());
#else
    _ui->x1_label->setMaxValue(_ui->x2_label->value());
    _ui->y1_label->setMaxValue(_ui->y2_label->value());

    _ui->x2_label->setMinValue(_ui->x1_label->value());
    _ui->x2_label->setMaxValue(_ui->x3_label->value());
    _ui->y2_label->setMinValue(_ui->y1_label->value());
    _ui->y2_label->setMaxValue(_ui->y3_label->value());

    _ui->x3_label->setMinValue(_ui->x2_label->value());
    _ui->x3_label->setMaxValue(_ui->x4_label->value());
    _ui->y3_label->setMinValue(_ui->y2_label->value());
    _ui->y3_label->setMaxValue(_ui->y4_label->value());

    _ui->x4_label->setMinValue(_ui->x3_label->value());
    _ui->y4_label->setMinValue(_ui->y3_label->value());
#endif
    if(_ui->x1_label->value() == -60.0)
        _ui->x1_label->setDisabled(true);
    else
        _ui->x1_label->setEnabled(true);
    if(_ui->y1_label->value() == -60.0)
        _ui->y1_label->setDisabled(true);
    else
        _ui->y1_label->setEnabled(true);

    if((_ui->x1_label->value() == -60.0) && (_ui->y1_label->value() == -60.0))
    {
        _ui->x1_label->setEnabled(true);
        _ui->y1_label->setEnabled(true);
    }

    if(_ui->x4_label->value() == 20.0)
        _ui->x4_label->setDisabled(true);
    else
        _ui->x4_label->setEnabled(true);
    if(_ui->y4_label->value() == 20.0)
        _ui->y4_label->setDisabled(true);
    else
        _ui->y4_label->setEnabled(true);
    if((_ui->x4_label->value() == 20.0) && (_ui->y4_label->value() == 20.0))
    {
        _ui->x4_label->setEnabled(true);
        _ui->y4_label->setEnabled(true);
    }
}

void Trn_DynWidget::display(DynTypeEnum type)
{
    disconnectSig(type);
    switch (type)
    {
    case DynType_ALL:
        _ui->atkBox->setValue(_dyndata.atk);
        _ui->rlsBox->setValue(_dyndata.release);
        _ui->byPass->setChecked(_dyndata.bypass);

        _ui->dynControl->setValue(_xValue, _yValue);

        _ui->x1_label->setValue(transX(_xValue[0])); // dbfs --> dbu +24
        _ui->x2_label->setValue(transX(_xValue[1]));
        _ui->x3_label->setValue(transX(_xValue[2]));
        _ui->x4_label->setValue(transX(_xValue[3]));
        _ui->y1_label->setValue(transY(_yValue[0]));
        _ui->y2_label->setValue(transY(_yValue[1]));
        _ui->y3_label->setValue(transY(_yValue[2]));
        _ui->y4_label->setValue(transY(_yValue[3]));
        break;
    case DynType_Grap:
        _ui->dynControl->setValue(_xValue, _yValue);
        break;
    case DynType_Spin:
        _ui->x1_label->setValue(transX(_xValue[0])); // dbfs --> dbu +24
        _ui->x2_label->setValue(transX(_xValue[1]));
        _ui->x3_label->setValue(transX(_xValue[2]));
        _ui->x4_label->setValue(transX(_xValue[3]));
        _ui->y1_label->setValue(transY(_yValue[0]));
        _ui->y2_label->setValue(transY(_yValue[1]));
        _ui->y3_label->setValue(transY(_yValue[2]));
        _ui->y4_label->setValue(transY(_yValue[3]));
        update();//Ft: 半圆刷新
        break;
    }
    connectSig(type);

    setSpinBoxLimit();
}

void Trn_DynWidget::getDispData()
{
    _xValue[1] = qRound((_dyndata.th1/256.0+90)*5);
    _xValue[2] = qRound((_dyndata.th2/256.0+90)*5);
    _yValue[1] = qRound(((_dyndata.offset1/256+0.2*_xValue[1]-90-10)/-0.2));
    _yValue[2] = qRound(((_dyndata.offset2/256+0.2*_xValue[2]-90-10)/-0.2));
    _xValue[0] = 50;
    _yValue[0] = qRound((_dyndata.nRatio/256.0)*(_xValue[1]-_xValue[0])+_yValue[1]);
    if(_yValue[0] > 450)
    {
        _yValue[0] = 450;
        _xValue[0] = qRound(_xValue[1] - (_yValue[0]-_yValue[1])/(_dyndata.nRatio/256.0));
    }
    _xValue[3] = 450;
    _yValue[3] = qRound(_dyndata.ratio/256.0*(_xValue[2]-_xValue[3])+_yValue[2]);
    if(_yValue[3] < 50)
    {
        _yValue[3] = 50;
        _xValue[3] = qRound(_xValue[2]-(_yValue[3]-_yValue[2])/(_dyndata.ratio/256.0));
    }
}

DynData &Trn_DynWidget::getDynData()
{
    float f;
    f = transY(_yValue[1]) - transX(_xValue[1]);
    _dyndata.offset1 = ((int)(f * 256)) & 0xFFFF;

    f = transY(_yValue[2]) - transX(_xValue[2]);
    _dyndata.offset2 = ((int)(f * 256)) & 0xFFFF;

    f = (transY(_yValue[1])-transY(_yValue[0]))/(transX(_xValue[1])-transX(_xValue[0]) + 1e-20);
    if(f > 16.0f)
        f = 16.0f;
    _dyndata.nRatio = ((int)(f * 256)) & 0xFFFF;

    f = (transY(_yValue[3])-transY(_yValue[2]))/(transX(_xValue[3])-transX(_xValue[2]) + 1e-20);
    if(f > 16.0f)
        f = 16.0f;
    _dyndata.ratio = ((int)(f * 256)) & 0xFFFF;

    _dyndata.th1 = ((int)(transX(_xValue[1])*256)) & 0xFFFF;

    _dyndata.th2 = ((int)(transX(_xValue[2])*256)) & 0xFFFF;

    _dyndata.atk = _ui->atkBox->value();
    _dyndata.release = _ui->rlsBox->value();
    _dyndata.bypass = _ui->byPass->isChecked();

    return _dyndata;
}

void Trn_DynWidget::showWidget(bool isIn, quint8 ch, QString chName, DynData& dynData)
{
    _ui->Title->setText(chName);
    _ui->Title->setHidden(true);
    _isIn = isIn;
    _ch = ch;
    _dyndata = dynData;

    getDispData();
    display(DynType_ALL);

    show();
}

/* 图像上移动点的处理函数 */
void Trn_DynWidget::getDynValue(int *x, int *y)
{
    for(int i = 0; i < 4; ++i)
        qDebug() << __FUNCTION__ << __LINE__ << i << x[i] << y[i];
    for(int i = 0; i < 4; ++i)
        _xValue[i] = x[i];
    for(int i = 0; i < 4; ++i)
        _yValue[i] = y[i];

    display(DynType_Spin);
    emit dynChanged(_ch, _isIn, getDynData());
}

void Trn_DynWidget::byPassFunc()
{
    emit dynChanged(_ch, _isIn, getDynData());//bypass状态改变
}

void Trn_DynWidget::spinBoxChanged()
{
    emit dynChanged(_ch, _isIn, getDynData());
}

/* spinbox调节动态参数 */
void Trn_DynWidget::setDynValue()
{
    if(sender() == _ui->x1_label)
        _xValue[0] = anti_transX(_ui->x1_label->value());  // dbu --> dbfs -24
    if(sender() == _ui->x2_label)
        _xValue[1] = anti_transX(_ui->x2_label->value());
    if(sender() == _ui->x3_label)
        _xValue[2] = anti_transX(_ui->x3_label->value());
    if(sender() == _ui->x4_label)
        _xValue[3] = anti_transX(_ui->x4_label->value());

    if(sender() == _ui->y1_label)
        _yValue[0] = anti_transY(_ui->y1_label->value());
    if(sender() == _ui->y2_label)
        _yValue[1] = anti_transY(_ui->y2_label->value());
    if(sender() == _ui->y3_label)
        _yValue[2] = anti_transY(_ui->y3_label->value());
    if(sender() == _ui->y4_label)
        _yValue[3] = anti_transY(_ui->y4_label->value());

    display(DynType_Grap);
    emit dynChanged(_ch, _isIn, getDynData());
}

void Trn_DynWidget::on_pushButton_clicked()
{
    initDynData();
    display(DynType_ALL);
    emit dynChanged(_ch, _isIn, getDynData());
}
