#include "stable.h"
#include "eqviewnormal.h"
#include "eqgraph.h"
#include "Protocal/eqpacket.h"
#include "eqviewnormalseg.h"
#include <QFormLayout>

EqViewNormal::EqViewNormal(bool bypass, QList<EqData *> dataList, QWidget *parent)
    :QWidget(parent)
{
    _channel = 1;
    _layout = NULL;
    _eqGraph = new EqGraph(this);
    _bypassBtn = new QPushButton(this);
    _reset     = new QPushButton(this);

    foreach(EqData *eqData, dataList)
    {
        _eqGraph->initParaList(eqData->freq, eqData->gain, eqData->q, getGraphEqType(eqData->eqType));

        EqviewNormalSeg *_seg = new EqviewNormalSeg(this);
        _seg->Set_Frequency(eqData->freq);
        _seg->Set_Gain(eqData->gain / 100.);
        _seg->Set_Bandwidth(eqData->q / 100.);
        _seg->set_eqType(eqData->eqType);
        _SegList.append(_seg);
    }
    _eqGraph->initParaListEnd();

    _bypassBtn->setCheckable(true);
    _bypassBtn->setChecked(bypass);
    _eqGraph->setBypass(bypass);

    setupUi_FangTu();
}

void EqViewNormal::reinitEqData(bool bypass, QList<EqData *> dataList)
{
    int i = 0;
    disconnectSignal();

    _bypassBtn->setChecked(bypass);
    _eqGraph->setBypass(bypass);

    foreach(EqData *eqData, dataList)
    {
        _eqGraph->setValue(i, eqData->freq, eqData->gain, eqData->q);

        _SegList.at(i)->Set_Frequency(eqData->freq);
        _SegList.at(i)->Set_Gain(eqData->gain / 100.);
        _SegList.at(i)->Set_Bandwidth(eqData->q / 100.);

        i++;
    }

    connectSignal();
}


void EqViewNormal::setupUi_FangTu()
{
    _layout = new QGridLayout(this);
    _layout->setContentsMargins(10, 10, 10, 10);
    _layout->setSpacing(2);

    QLabel *label_response = new QLabel(tr("response"));
    label_response->setAlignment(Qt::AlignRight);
    _layout->addWidget(label_response, 0, 0);
    QGroupBox *eqGroupGBox = new QGroupBox();
    QGridLayout *eqGroupLayOut = new QGridLayout();
    eqGroupLayOut->addWidget(_eqGraph);
    eqGroupGBox->setLayout(eqGroupLayOut);

    _layout->addWidget(eqGroupGBox, 0, 1);

    initEqParaUi();

    QGridLayout *btnLayout = new QGridLayout();
    btnLayout->setContentsMargins(0, 0, 12, 0);
    btnLayout->setHorizontalSpacing(7);
    btnLayout->addWidget(new QWidget);

    _bypassBtn->setProperty("form", "Bypass");
    _bypassBtn->setFixedWidth(50);
    QLabel *label_bypass = new QLabel(tr("All Bypass"));
    btnLayout->addWidget(label_bypass, 0, 0, Qt::AlignCenter);
    btnLayout->addWidget(_bypassBtn, 1, 0, Qt::AlignCenter);

    connect(_bypassBtn, SIGNAL(clicked(bool)), this, SLOT(onSetBypass(bool)));

    QLabel *label_reset = new QLabel(tr("Clear All"));

    btnLayout->addWidget(label_reset, 0, 1, Qt::AlignCenter);
    btnLayout->addWidget(_reset, 1, 1);

    _reset->setFixedWidth(50);
    connect(_reset, SIGNAL(clicked()), this, SLOT(onResetClicked()));

    if(_SegList.count() <= 10) {
        btnLayout->setContentsMargins((11+57*(_SegList.count()-2)), 0, 12, 0);
        _layout->addLayout(btnLayout,2, 1, Qt::AlignLeft);
    }
    if (_SegList.count() > 10){
        btnLayout->setContentsMargins(11+57*(10-_SegList.count()+10), 0, 12, 0);
        _layout->addLayout(btnLayout,3, 1, Qt::AlignLeft);
    }

}

void EqViewNormal::initEqParaUi()
{
    Q_ASSERT(_eqGraph != NULL);

    QVBoxLayout *Label_Layout = new QVBoxLayout();
    Label_Layout->setContentsMargins(0, 0, 0, 0);
    Label_Layout->setSpacing(0);
    QLabel *label_Empty     = new QLabel(tr(""), this);              Label_Layout->addWidget(label_Empty, Qt::AlignCenter);
    QLabel *label_Bypass    = new QLabel(tr("Bypass"), this);        Label_Layout->addWidget(label_Bypass, Qt::AlignCenter);
    QLabel *label_Freq      = new QLabel(tr("Frequency"), this);     Label_Layout->addWidget(label_Freq, Qt::AlignCenter);
    QLabel *label_Gain      = new QLabel(tr("Gain"), this);          Label_Layout->addWidget(label_Gain, Qt::AlignCenter);
    QLabel *label_Bandwidth = new QLabel(tr("Q"), this);             Label_Layout->addWidget(label_Bandwidth, Qt::AlignCenter);
    QLabel *label_EqType    = new QLabel(tr("Type"), this);          Label_Layout->addWidget(label_EqType, Qt::AlignCenter);
    label_Bypass   ->setFixedHeight(40); label_Bypass   ->setAlignment(Qt::AlignRight);
    label_Freq   ->setFixedSize(55, 50); label_Freq     ->setAlignment(Qt::AlignRight);
    label_Gain     ->setFixedHeight(50); label_Gain     ->setAlignment(Qt::AlignRight);
    label_Bandwidth->setFixedHeight(50); label_Bandwidth->setAlignment(Qt::AlignRight);
    label_EqType   ->setFixedHeight(30); label_EqType   ->setAlignment(Qt::AlignRight);

    _layout->addLayout(Label_Layout, 1, 0);

    QGroupBox *segGBox = new QGroupBox(/*"1~10"*/);
    QGroupBox *segGBox11_16 = new QGroupBox(/*"11~16"*/);
    QHBoxLayout *segLayout = new QHBoxLayout;
    segLayout->setContentsMargins(11, 11, 11, 11);
    segLayout->setSpacing(7);
    QHBoxLayout *segLayout11_16 = new QHBoxLayout;
    segLayout11_16->setContentsMargins(11, 11, 11, 11);
    segLayout11_16->setSpacing(7);

    QHBoxLayout *spacerlayout11_16 = new QHBoxLayout;

    for (int i = 0; i < _SegList.count(); ++i) {
        if (i < 10) {
            _SegList.at(i)->Set_Title(QString::number(i+1));
            segLayout->addWidget(_SegList.at(i));
        }else {
            _SegList.at(i)->Set_Title(QString::number(i+1));
            segLayout11_16->addWidget(_SegList.at(i));
        }
    }
    segGBox->setLayout(segLayout);
    _layout->addWidget(segGBox, 1, 1, Qt::AlignLeft);

    if (_SegList.count() > 10){
        QVBoxLayout *Label_Layout11_16 = new QVBoxLayout();
        Label_Layout11_16->setContentsMargins(0, 0, 0, 0);
        Label_Layout11_16->setSpacing(0);
        QLabel *label_Empty11_16     = new QLabel(tr(""), this);              Label_Layout11_16->addWidget(label_Empty11_16, Qt::AlignCenter);
        QLabel *label_Bypass11_16    = new QLabel(tr("Bypass"), this);        Label_Layout11_16->addWidget(label_Bypass11_16, Qt::AlignCenter);
        QLabel *label_Freq11_16      = new QLabel(tr("Frequency"), this);     Label_Layout11_16->addWidget(label_Freq11_16, Qt::AlignCenter);
        QLabel *label_Gain11_16      = new QLabel(tr("Gain"), this);          Label_Layout11_16->addWidget(label_Gain11_16, Qt::AlignCenter);
        QLabel *label_Bandwidth11_16 = new QLabel(/*tr*/("Q"), this);         Label_Layout11_16->addWidget(label_Bandwidth11_16, Qt::AlignCenter);
        QLabel *label_EqType11_16    = new QLabel(tr("Type"), this);          Label_Layout11_16->addWidget(label_EqType11_16, Qt::AlignCenter);
        label_Bypass11_16   ->setFixedHeight(40); label_Bypass11_16   ->setAlignment(Qt::AlignRight);
        label_Freq11_16     ->setFixedHeight(50); label_Freq11_16     ->setAlignment(Qt::AlignRight);
        label_Gain11_16     ->setFixedHeight(50); label_Gain11_16     ->setAlignment(Qt::AlignRight);
        label_Bandwidth11_16->setFixedHeight(50); label_Bandwidth11_16->setAlignment(Qt::AlignRight);
        label_EqType11_16   ->setFixedHeight(30); label_EqType11_16   ->setAlignment(Qt::AlignRight);
        _layout->addLayout(Label_Layout11_16, 2, 0);

        segGBox11_16->setLayout(segLayout11_16);
        _layout->addWidget(segGBox11_16, 2, 1, Qt::AlignLeft);
    }
}

void EqViewNormal::connectSignal()
{
    for (int i = 0; i < _SegList.count(); ++i) {
        connect(_SegList.at(i), SIGNAL(Bypass_Clicked(bool)), this, SLOT(SegBypass_Clicked(bool)));
        connect(_SegList.at(i), SIGNAL(FreqGainBandwidth_ValueChanged(void)), this, SLOT(Seg_ValueChanged(void)));
    }
}

void EqViewNormal::disconnectSignal()
{
    for (int i = 0; i < _SegList.count(); ++i) {
        disconnect(_SegList.at(i), SIGNAL(Bypass_Clicked(bool)), this, SLOT(SegBypass_Clicked(bool)));
        disconnect(_SegList.at(i), SIGNAL(FreqGainBandwidth_ValueChanged(void)), this, SLOT(Seg_ValueChanged(void)));
    }
}

EqType EqViewNormal::getGraphEqType(int type)
{
    switch(type) {
    case 0:  return EqType::EQ_NORMAL_CONFIG;
    case 1:  return EqType::EQ_HIGHPASS_CONFIG;
    case 2:  return EqType::EQ_LOWPASS_CONFIG;
    case 3:  return EqType::EQ_HIGHSHELF_CONFIG;
    case 4:  return EqType::EQ_LOWSHELF_CONFIG;

    default: return EqType::EQ_NORMAL_CONFIG;
    }
}

int EqViewNormal::getDataEqType(EqType type)
{
    switch(type) {
    case EQ_NORMAL_CONFIG:    return 0;
    case EQ_HIGHPASS_CONFIG:  return 1;
    case EQ_LOWPASS_CONFIG:   return 2;
    case EQ_HIGHSHELF_CONFIG: return 3;
    case EQ_LOWSHELF_CONFIG:  return 4;
    default: return 0;
    }
}

void EqViewNormal::showWidget(void)
{
    connect(_eqGraph, SIGNAL(valueChanged(int,int,int,int,EqType)), this, SLOT(onValueChanged(int,int,int,int,EqType)));
    connect(_eqGraph, SIGNAL(selectPointChanged(int)), this, SLOT(onSelectSegChanged(int)));

    connectSignal();

    _eqGraph->setCurrentDragPoint(0);
    show();
}

void EqViewNormal::setSegBypass(uint32 idx, bool bypass)
{
    _eqGraph->setSegBypass(idx, bypass);
    _SegList.at(idx)->Set_Bypass(bypass);
}

void EqViewNormal::onValueChanged(int idx, int fs, int gain, int q, EqType type)
{
    disconnectSignal();


    _SegList.at(idx)->Set_Frequency(fs);
    _SegList.at(idx)->Set_Gain(gain / 100.);
    _SegList.at(idx)->Set_Bandwidth(q / 100.);
    _SegList.at(idx)->set_eqType(getDataEqType(type));
    for (int i = 0; i < _SegList.count(); ++i) {
        if (i == idx) {
            _SegList.at(i)->select();
        }
        else {
            _SegList.at(i)->unselect();
        }
    }
    connectSignal();
    emit eqDataChanged(idx, _SegList.at(idx)->Get_bypass(), fs, gain, q, _SegList.at(idx)->get_eqType());
//    qDebug() << __FUNCTION__ << __LINE__ <<
//                idx << _SegList.at(idx)->Get_bypass() << fs << gain << q << _SegList.at(idx)->get_eqType();
}

void EqViewNormal::onSetBypass(bool isBypass)
{
    _eqGraph->setBypass(isBypass);
    emit eqDataBypassClicked(isBypass);
    //    emitEqDataChanged();
}

void EqViewNormal::onResetClicked()
{
    emit eqDataReset();
//    int step = EqGraph::SCALE_CNT / _SegList.count();
//    QList<EqData *> _eqList;
//    for(int i = 0; i < _SegList.count(); i++)
//    {
//        EqData *eqData = new EqData;
//        eqData->freq = 20 * (pow(10.0, 3.0 * (i*step + step/2) / (EqGraph::SCALE_CNT - 1)));
//        eqData->gain = 0;
//        eqData->q = 1.0 * 100;
//        _eqList.append(eqData);
////        qDebug() << __FUNCTION__ << __LINE__
////                 << _eqList.last()->freq;
//    }

//    reinitEqData(true, _eqList);
}

void EqViewNormal::onSelectSegChanged(int idx)
{
    for (int i = 0; i < _SegList.count(); ++i) {
        if (i == idx) {
            _SegList.at(i)->select();
        }
        else {
            _SegList.at(i)->unselect();
        }
    }
}

void EqViewNormal::SegBypass_Clicked(bool enable)
{
    for (int i = 0; i < _SegList.count(); ++i) {
        if (sender() == _SegList.at(i)) {
//            disconnect(_eqGraph, SIGNAL(valueChanged(int,int,int,int)), this, SLOT(onValueChanged(int,int,int,int)));
            _eqGraph->setSegBypass(i, enable);
//            connect(_eqGraph, SIGNAL(valueChanged(int,int,int,int)), this, SLOT(onValueChanged(int,int,int,int)));
            break;
        }
    }
}

void EqViewNormal::Seg_ValueChanged(void)
{
    for (int i = 0; i < _SegList.count(); ++i) {
        if (sender() == _SegList.at(i)) {
            int fs   = _SegList.at(i)->Get_Frequency();
            int gain = _SegList.at(i)->Get_Gain() * 100;
            int q    = _SegList.at(i)->Get_Bandwidth() * 100;
            int type = _SegList.at(i)->get_eqType();
            _eqGraph->setValue(i, fs, gain, q);
            _eqGraph->setType(i, getGraphEqType(type));
            emit eqDataChanged(i, _SegList.at(i)->Get_bypass(), fs, gain, q, type);
            break;
        }
    }
}
