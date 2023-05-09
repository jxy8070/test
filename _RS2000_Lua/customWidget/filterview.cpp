#include "stable.h"
#include "filterview.h"
#include "eqgraph.h"


FilterView::FilterView(QWidget *parent) : QWidget(parent)
{
    EqPara* eqParaHP = new EqPara(0, 100, 111, EQ_BANDPASS_H);
    EqPara* eqParaLP = new EqPara(200, 100, 111, EQ_BANDPASS_L);
    _paraList.append(eqParaHP);
    _paraList.append(eqParaLP);

    setupUi();
    resize(800,460);
}

void FilterView::showWidget(int channel, QString chName, SplitData data)
{
    _channel = channel;

    disconnect(_eqGraph, SIGNAL(valueChanged(int,int,int,int)), this, SLOT(onValueChanged(int,int,int,int)));
    disconnect(_bypassBtnHP, SIGNAL(clicked()), this, SLOT(onSetBypass()));
    disconnect(_bypassBtnLP, SIGNAL(clicked()), this, SLOT(onSetBypass()));
    disconnect(_freqSpinHP, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));
    disconnect(_freqSpinLP, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));

    _channelName->setText(chName);
    _bypassBtnHP->setChecked(data.bypassHP);
    _bypassBtnLP->setChecked(data.bypassLP);
    _eqGraph->setHPLPBypass(data.bypassHP, data.bypassLP);

    EqPara *paraHP = _eqGraph->getEqPara(0);
    EqPara *paraLP = _eqGraph->getEqPara(1);

    _eqGraph->setValue(0, data.freqHP, paraHP->gain, paraHP->q);
    _freqSpinHP->setValue(EqGraph::Scale2Float(data.freqHP, 0));
    _eqGraph->setValue(1, data.freqLP, paraLP->gain, paraLP->q);
    _freqSpinLP->setValue(EqGraph::Scale2Float(data.freqLP, 0));

    show();

    connect(_eqGraph, SIGNAL(valueChanged(int,int,int,int)), this, SLOT(onValueChanged(int,int,int,int)));
    connect(_bypassBtnHP, SIGNAL(clicked()), this, SLOT(onSetBypass()));
    connect(_bypassBtnLP, SIGNAL(clicked()), this, SLOT(onSetBypass()));
    connect(_freqSpinHP, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));
    connect(_freqSpinLP, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));
}

void FilterView::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    _channelName = new QLabel(this);
    _channelName->setText(tr("Filter"));
    _channelName->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    mainLayout->addWidget(_channelName);

    _eqGraph = new EqGraph(this);
    _eqGraph->initEqParaList(_paraList);
    connect(_eqGraph, SIGNAL(valueChanged(int,int,int,int)), this, SLOT(onValueChanged(int,int,int,int)));
    mainLayout->addWidget(_eqGraph);

    QHBoxLayout *splitLayout = new QHBoxLayout();
    splitLayout->setSpacing(10);

    QVBoxLayout *typeLayout = new QVBoxLayout();
    typeLayout->setSpacing(6);
    QLabel *freqLbl = new QLabel(tr("type"), this);
    typeLayout->addWidget(freqLbl);
    QLabel *freqLb2 = new QLabel(tr("High Bypass"), this);
    typeLayout->addWidget(freqLb2);
    QLabel *freqLb3 = new QLabel(tr("Low Bypass"), this);
    typeLayout->addWidget(freqLb3);

    QVBoxLayout *freqLayout = new QVBoxLayout();
    freqLayout->setSpacing(6);
    freqLayout->addWidget(new QLabel(tr("frequency:"), this));
    _freqSpinHP = new QDoubleSpinBox(this);
    _freqSpinHP->setMinimum(20.0);
    _freqSpinHP->setMaximum(20000.0);
    _freqSpinHP->setValue(EqGraph::Scale2Float(_paraList.at(0)->freq, 0));
    freqLayout->addWidget(_freqSpinHP);
    _freqSpinLP = new QDoubleSpinBox(this);
    _freqSpinLP->setMinimum(20.0);
    _freqSpinLP->setMaximum(20000.0);
    _freqSpinLP->setValue(EqGraph::Scale2Float(_paraList.at(1)->freq, 0));
    freqLayout->addWidget(_freqSpinLP);

    QVBoxLayout *bypassLayout = new QVBoxLayout();
    bypassLayout->setSpacing(6);
    bypassLayout->addWidget(new QLabel("  ", this));
    _bypassBtnHP = new QPushButton(this);
    _bypassBtnHP->setCheckable(true);
    _bypassBtnHP->setStyleSheet(QLatin1String(
        "QPushButton{ background-color: rgb(255, 0, 0);}"
        "QPushButton:checked{background-color: rgb(0, 255, 0);}"
        ));
    _bypassBtnHP->setText(QApplication::translate("FilterView", "bypass", 0));
    connect(_bypassBtnHP, SIGNAL(clicked()), this, SLOT(onSetBypass()));
    bypassLayout->addWidget(_bypassBtnHP);
    _bypassBtnLP = new QPushButton(this);
    _bypassBtnLP->setCheckable(true);
    _bypassBtnLP->setStyleSheet(QLatin1String(
        "QPushButton{ background-color: rgb(255, 0, 0);}"
        "QPushButton:checked{background-color: rgb(0, 255, 0);}"
        ));
    _bypassBtnLP->setText(QApplication::translate("FilterView", "bypass", 0));
    connect(_bypassBtnLP, SIGNAL(clicked()), this, SLOT(onSetBypass()));
    bypassLayout->addWidget(_bypassBtnLP);


    _resetBtn = new QPushButton(tr("reset"), this);
    connect(_resetBtn, SIGNAL(clicked()), this, SLOT(onReset()));

    splitLayout->addLayout(typeLayout, 2);
    splitLayout->addLayout(freqLayout, 2);
    splitLayout->addLayout(bypassLayout, 2);
    splitLayout->addWidget(_resetBtn, 2);

    mainLayout->addLayout(splitLayout);

    connectSignal();
}


void FilterView::onValueChanged(int idx, int fx, int gain, int q)
{
    Q_UNUSED(gain);
    Q_UNUSED(q);
    Q_ASSERT(idx >= 0 && idx < _paraList.count());

    disconnectSignal();
    if (idx == 0)
    {
        float value = EqGraph::Scale2Float(fx, 0);
        _freqSpinHP->setValue(value);
        _freqSpinHP->setSingleStep(value/20);
    }
    else
    {
        float value = EqGraph::Scale2Float(fx, 0);
        _freqSpinLP->setValue(value);
        _freqSpinLP->setSingleStep(value/20);
    }
    connectSignal();
    emitEqDataChanged();
}

void FilterView::onFrequencyChanged()
{
    if (sender() == _freqSpinHP)
    {
        int fs = EqGraph::Float2Scale(_freqSpinHP->value(), 0);
        _freqSpinHP->setSingleStep(_freqSpinHP->value()/20);
        EqPara *para = _eqGraph->getEqPara(0);
        _eqGraph->setValue(0, fs, para->gain, para->q);
    }
    else if (sender() == _freqSpinLP)
    {
        int fs = EqGraph::Float2Scale(_freqSpinLP->value(), 0);
        _freqSpinLP->setSingleStep(_freqSpinLP->value()/20);
        EqPara *para = _eqGraph->getEqPara(1);
        _eqGraph->setValue(1, fs, para->gain, para->q);
    }

    emitEqDataChanged();
}

void FilterView::emitEqDataChanged()
{
    if(_channel == -1) return;
    SplitData data;
    data.bypassHP = _bypassBtnHP->isChecked();
    data.bypassLP = _bypassBtnLP->isChecked();
    data.freqHP = _eqGraph->getEqPara(0)->freq;
    data.freqLP = _eqGraph->getEqPara(1)->freq;

    emit eqDataChanged(_channel, data);
}

void FilterView::connectSignal()
{
    connect(_freqSpinHP, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));
    connect(_freqSpinLP, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));
}

void FilterView::disconnectSignal()
{
    disconnect(_freqSpinHP, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));
    disconnect(_freqSpinLP, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));
}

void FilterView::onReset()
{
    int fx = 0;

    disconnect(_eqGraph, SIGNAL(valueChanged(int,int,int,int)), this, SLOT(onValueChanged(int,int,int,int)));
    disconnect(_bypassBtnHP, SIGNAL(clicked()), this, SLOT(onSetBypass()));
    disconnect(_bypassBtnLP, SIGNAL(clicked()), this, SLOT(onSetBypass()));
    disconnect(_freqSpinHP, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));
    disconnect(_freqSpinLP, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));

    _freqSpinHP->setValue(EqGraph::Scale2Float(fx, 0));
    EqPara *paraHP = _eqGraph->getEqPara(0);
    _eqGraph->setValue(0, fx, paraHP->gain, paraHP->q);
    _bypassBtnHP->setChecked(true);

    fx = 200;
    _freqSpinLP->setValue(EqGraph::Scale2Float(fx, 0));
    EqPara *paraLP = _eqGraph->getEqPara(1);
    _eqGraph->setValue(1, fx, paraLP->gain, paraLP->q);
    _bypassBtnLP->setChecked(true);

    _eqGraph->setHPLPBypass(_bypassBtnHP->isChecked(), _bypassBtnLP->isChecked());

    connect(_eqGraph, SIGNAL(valueChanged(int,int,int,int)), this, SLOT(onValueChanged(int,int,int,int)));
    connect(_bypassBtnHP, SIGNAL(clicked()), this, SLOT(onSetBypass()));
    connect(_bypassBtnLP, SIGNAL(clicked()), this, SLOT(onSetBypass()));
    connect(_freqSpinHP, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));
    connect(_freqSpinLP, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged()));

    emitEqDataChanged();
}

void FilterView::onSetBypass()
{
    _eqGraph->setHPLPBypass(_bypassBtnHP->isChecked(), _bypassBtnLP->isChecked());
    emitEqDataChanged();
}
