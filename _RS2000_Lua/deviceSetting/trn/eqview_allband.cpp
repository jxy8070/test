#include "stable.h"
#include "eqview_allband.h"
#include "customWidget/eqgraph.h"
#include "protocal/eqpacket.h"
#include "config.h"
#include <QScrollArea>

#define LabelWidth 60

EqView_AllBand::EqView_AllBand(int paraLen, QWidget *parent)
    :DevSettingWidget(parent)
{
    _eqDataLen = paraLen;
    _channel = -1;
    _layout = NULL;
    _eqArray = new EqData[_eqDataLen];

    setupUi();

    const int FIXEDWIDTH[] = {
        500, 500, 500, 500, 500,
        600, 600, 700, 700, 700,
        800, 800, 800, 800, 900,
        900,
    };
//    setFixedWidth(FIXEDWIDTH[_eqDataLen]);
//    resize(500,460);
    setFixedSize(FIXEDWIDTH[_eqDataLen]+LabelWidth, 600);
}

void EqView_AllBand::setupUi()
{
    _layout = new QVBoxLayout(this);

    _channelName = new QLabel(this);
    _channelName->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);//设置控件水平方向和垂直方向中心对齐
    _layout->addWidget(_channelName);

    _eqGraph = new EqGraph(this);//曲线图
    connect(_eqGraph, SIGNAL(valueChanged(int,int,int,int)), this, SLOT(onValueChanged(int,int,int,int)));

    QLabel *labelResponse = new QLabel(tr("Response")); labelResponse->setAlignment(Qt::AlignRight);
    labelResponse->setWordWrap(true); labelResponse->setFixedWidth(LabelWidth);
    QHBoxLayout *eqHLayout = new QHBoxLayout;
    QGroupBox *eqGroupBox = new QGroupBox;
    QVBoxLayout *eqVLayout = new QVBoxLayout;
    eqVLayout->addWidget(_eqGraph);
    eqGroupBox->setLayout(eqVLayout);
    eqHLayout->addWidget(labelResponse);
    eqHLayout->addWidget(eqGroupBox);
    _layout->addLayout(eqHLayout);
    initEqParaUi();


    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(new QWidget, 1);
    _bypassBtn = new QPushButton(this);
    _bypassBtn->setCheckable(true);//启用的开关的切换
    _bypassBtn->setFocusPolicy(Qt::NoFocus);//不接受的键盘焦点
    _bypassBtn->setText(QApplication::translate("EqView_AllBand", "bypass", 0));
    _bypassBtn->setProperty("form", "Bypass");
    _bypassBtn->setFixedSize(72,23);//设置控件的大小
    btnLayout->addWidget(_bypassBtn);
    connect(_bypassBtn, SIGNAL(clicked(bool)), this, SLOT(onSetBypass(bool)));

    btnLayout->addWidget(new QWidget, 1);
    btnLayout->setSpacing(9);//设置间距
    _layout->addLayout(btnLayout,0);
}


#include "customWidget/configgainwidget.h"
void EqView_AllBand::initEqParaUi()
{
    QHBoxLayout *eqparaHLayout = new QHBoxLayout;
    QGridLayout *eqParaLayout = new QGridLayout;
    _layout->addLayout(eqparaHLayout);

    eqParaLayout->setHorizontalSpacing(6);//并排间距设置为10
    eqParaLayout->setVerticalSpacing(6);//排列在一起的小部件间距为4

    QVBoxLayout *labelLayout = new QVBoxLayout;
    labelLayout->setContentsMargins(0, 60, 0, 0);
    QLabel *freqLbl = new QLabel(tr("CenterFreq (HZ)"), this); freqLbl->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    QLabel *gainLbl = new QLabel(tr("Gain(dB)"), this);     gainLbl->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    QLabel *qLbl = new QLabel(tr("Q-Factor"), this);           qLbl->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    freqLbl->setFixedWidth(LabelWidth); freqLbl->setWordWrap(true); labelLayout->addWidget(freqLbl, 1, 0);
    gainLbl->setFixedWidth(LabelWidth); gainLbl->setWordWrap(true); labelLayout->addWidget(gainLbl, 2, 0);
    qLbl->setFixedWidth(LabelWidth);    qLbl->setWordWrap(true);    labelLayout->addWidget(qLbl, 3, 0);
//    eqParaLayout->addWidget(new QWidget(this), 4, 0);

    QGroupBox *eqparaGroupBox = new QGroupBox;
    eqparaGroupBox->setLayout(eqParaLayout);
    for(int i = 0; i < _eqDataLen; i++)
    {
        BandSpinBox *spinBox = new BandSpinBox();
//        eqParaLayout->addWidget(new QWidget(this), i+1, 0);

        QLabel *bandLbl = new QLabel(tr("Band%1").arg(i+1), this);
        bandLbl->setAlignment(Qt::AlignHCenter);
        eqParaLayout->addWidget(bandLbl, 0, i+1);

        spinBox->freqSpin = new ConfigGainWidget(this);
        spinBox->freqSpin->setRangeParam(20, 20000, StepMode_Log10, ParamType_Freq);
        spinBox->freqSpin->setDecimalsNum(0);//精度
        spinBox->freqSpin->setProperty("band", i);//设置对象的 band属性设置为 i-1
        eqParaLayout->addWidget(spinBox->freqSpin, 1, i+1);

        spinBox->gainSpin = new ConfigGainWidget(this);
        spinBox->gainSpin->setRangeParam(-20, 20, StepMode_Line, ParamType_Gain);
        spinBox->gainSpin->setDecimalsNum(1);
        spinBox->gainSpin->setProperty("band", i);
        eqParaLayout->addWidget(spinBox->gainSpin, 2, i+1);

        spinBox->qSpin = new ConfigGainWidget(this);
        spinBox->qSpin->setRangeParam(0.05, 20.0, StepMode_Log10, ParamType_Q);
        spinBox->qSpin->setDecimalsNum(2);//精度
        spinBox->qSpin->setProperty("band", i);
        eqParaLayout->addWidget(spinBox->qSpin, 3, i+1);

        _spinBoxList.append(spinBox);
    }
    eqparaHLayout->addLayout(labelLayout);
    eqparaHLayout->addWidget(eqparaGroupBox, Qt::AlignLeft);
    QWidget *wid = new QWidget;
    wid->setFixedWidth(500-56*_eqDataLen-6);
    eqparaHLayout->addWidget(wid);
}


void EqView_AllBand::connectSig()
{
    foreach (BandSpinBox *spinBox, _spinBoxList) {
        connect(spinBox->freqSpin, SIGNAL(valueChanged(int)), this, SLOT(onEqParaChanged()));
        connect(spinBox->qSpin, SIGNAL(valueChanged(double)), this, SLOT(onEqParaChanged()));
        connect(spinBox->gainSpin, SIGNAL(valueChanged(double)), this, SLOT(onEqParaChanged()));
    }
}

void EqView_AllBand::disconnectSig()
{
    foreach (BandSpinBox *spinBox, _spinBoxList) {
        disconnect(spinBox->freqSpin, SIGNAL(valueChanged(int)), this, SLOT(onEqParaChanged()));
        disconnect(spinBox->qSpin, SIGNAL(valueChanged(double)), this, SLOT(onEqParaChanged()));
        disconnect(spinBox->gainSpin, SIGNAL(valueChanged(double)), this, SLOT(onEqParaChanged()));
    }
}

void EqView_AllBand::showWidget(int channel, QString chName, bool bypass, EqData *eqPara)
{
//    qDebug() << __FUNCTION__ << __LINE__ << channel << chName << bypass <<
//        eqPara->freq << eqPara->gain << eqPara->q << eqPara->eqType;
    _channel = channel;
    if(chName == QString::null) {
        _channelName->hide();//隐藏
    }
    else{
        _channelName->setText(chName);
    }

    for(int i = 0; i < _eqDataLen; i++)
    {
        _eqGraph->initParaList(eqPara[i].freq, eqPara[i].gain*100/256, eqPara[i].q*100/256, EqType::EQ_NORMAL);

        _spinBoxList.at(i)->freqSpin->setValue(eqPara[i].freq);
        _spinBoxList.at(i)->gainSpin->setValue(eqPara[i].gain / 256.0);
        _spinBoxList.at(i)->qSpin->setValue(eqPara[i].q / 256.0);
        qDebug() << __FUNCTION__ << i << eqPara[i].freq << eqPara[i].gain / 256.0 << eqPara[i].q / 256.0;
    }
    _eqGraph->initParaListEnd();

    _bypassBtn->setChecked(bypass);
    _eqGraph->setBypass(bypass);

    show();

    connect(_bypassBtn, SIGNAL(clicked(bool)), this, SLOT(onSetBypass(bool)));
    connectSig();
}

void EqView_AllBand::onEqParaChanged()
{
    int idx = sender()->property("band").toInt();//返回对象的band属性的值

    _eqGraph->setValue(idx, _spinBoxList.at(idx)->freqSpin->value(), _spinBoxList.at(idx)->gainSpin->value()*100, _spinBoxList.at(idx)->qSpin->value()*100);

    emit emitEqDataChanged();
}

void EqView_AllBand::onValueChanged(int idx, int fs, int gain, int q)
{
    disconnectSig();
    _spinBoxList.at(idx)->freqSpin->setValue(fs);
    _spinBoxList.at(idx)->gainSpin->setValue(gain / 100.0);
    _spinBoxList.at(idx)->qSpin->setValue(q / 100.0);
    connectSig();
    update();
    emitEqDataChanged();
}

void EqView_AllBand::emitEqDataChanged()
{
    if(_channel == -1) return;

    for(int i = 0; i< _eqDataLen; i++)
    {
        _eqArray[i].freq = _spinBoxList.at(i)->freqSpin->value();
        _eqArray[i].gain = _spinBoxList.at(i)->gainSpin->value() * 256;
        _eqArray[i].q = _spinBoxList.at(i)->qSpin->value() * 256;
    }

    emit eqDataChanged(_channel, _bypassBtn->isChecked(), _eqArray);
}

void EqView_AllBand::onSetBypass(bool isBypass)
{
    _eqGraph->setBypass(isBypass);

    disconnect(_bypassBtn, SIGNAL(clicked(bool)), this, SLOT(onSetBypass(bool)));
    _bypassBtn->setChecked(isBypass);
    connect(_bypassBtn, SIGNAL(clicked(bool)), this, SLOT(onSetBypass(bool)));

    emitEqDataChanged();
}
