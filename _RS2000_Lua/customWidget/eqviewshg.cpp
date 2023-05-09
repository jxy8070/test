#include "stable.h"
#include "eqviewshg.h"
#include "eqgraph.h"
#include "eqslider.h"
#include "Protocal/eqpacket.h"
#include "config.h"

EqViewSHG::EqViewSHG(SHG_Type type, bool bypass, QList<EqData *> dataList, QWidget *parent):
    QWidget(parent)
{
    int i = 0;
    _type = type;
    _bypassBtn = new QPushButton(this);
    _reset     = new QPushButton(this);
    _eqGraph = new EqGraph(this);
    foreach (EqData *tmp, dataList) {
        _eqGraph->initParaList(tmp->freq, tmp->gain, tmp->q, EQ_SHG_CONFIG);

        EqSlider *eqSlider = new EqSlider(i, tmp->freq, tmp->gain, false, false);
        _eqSliderList.append(eqSlider);
        i++;
    }
    _eqGraph->initParaListEnd();

    _bypassBtn->setChecked(bypass);
    _eqGraph->setBypass(bypass);

    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
    {
        setupUi_FangTu();
    }else{
        setupUi();
    }
}

void EqViewSHG::setupUi()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->setLayout(layout);
    layout->setContentsMargins(10, 5, 10, 10);
    QGroupBox *responseBox = new QGroupBox(this);
    QVBoxLayout *responseLayout = new QVBoxLayout(responseBox);
    connect(_eqGraph, SIGNAL(valueChanged(int,int,int,int)), this, SLOT(onValueChanged(int,int,int,int)));
    responseLayout->addWidget(_eqGraph);
    layout->addWidget(responseBox);

    QGroupBox *ctrlBox = new QGroupBox(this);
    QHBoxLayout *ctrlLayout = new QHBoxLayout(ctrlBox);
    int groupCnt = 16;
    int sliderCnt = _eqSliderList.count() > groupCnt? groupCnt :_eqSliderList.count();
    for(int i = 0; i < sliderCnt; i++) {
            ctrlLayout->addWidget(_eqSliderList.at(i), Qt::AlignCenter);
    }
    ctrlBox->setFixedHeight(200);
    layout->addWidget(ctrlBox);
    if(_type == SHG_1X3){
        QGroupBox *ctrlBox16 = new QGroupBox(this);
        QHBoxLayout *ctrlLayout16 = new QHBoxLayout(ctrlBox16);
        for(int i = groupCnt; i < _eqSliderList.count(); i++) {
            ctrlLayout16->addWidget(_eqSliderList.at(i), Qt::AlignCenter);
        }
        QWidget* emptySlider = new QWidget;
        emptySlider->setFixedWidth(_eqSliderList[0]->width());
        ctrlLayout16->addWidget(emptySlider);
        ctrlBox16->setFixedHeight(200);
        layout->addWidget(ctrlBox16);
    }
    QHBoxLayout *btnLayout = new QHBoxLayout();
    _bypassBtn->setCheckable(true);
    _bypassBtn->setChecked(true);
    _bypassBtn->setProperty("form", "Bypass");
    connect(_reset, SIGNAL(clicked()), this, SLOT(onResetClicked()));
    QSpacerItem* hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    btnLayout->addSpacerItem(hSpacer);
    btnLayout->addWidget(new QLabel(QApplication::translate("EqViewSHG", "Bypass", 0)));
    btnLayout->addWidget(_bypassBtn);
    btnLayout->addSpacing(10);
    btnLayout->addWidget(new QLabel(QApplication::translate("EqViewSHG", "Reset", 0)));
    btnLayout->addWidget(_reset);
    layout->addLayout(btnLayout);
}

void EqViewSHG::setupUi_FangTu()
{
    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(5, 0, 5, 5);
    QGroupBox *responseBox = new QGroupBox();
    QVBoxLayout *responseLayout = new QVBoxLayout(responseBox);

    QGroupBox *ctrlBox = new QGroupBox();
    QHBoxLayout *ctrlLayout = new QHBoxLayout(ctrlBox);

    QGroupBox *ctrlBox16 = new QGroupBox();
    QHBoxLayout *ctrlLayout16 = new QHBoxLayout(ctrlBox16);

    connect(_eqGraph, SIGNAL(valueChanged(int,int,int,int)), this, SLOT(onValueChanged(int,int,int,int)));
    responseLayout->addWidget(_eqGraph);
    QLabel *lab_response = new QLabel(tr("Response")); lab_response->setAlignment(Qt::AlignRight);
    lab_response->setFixedWidth(53);
    layout->addWidget(lab_response, 0, 0, Qt::AlignTop);
    layout->addWidget(responseBox, 0, 1);

    for(int i = 0; i < _eqSliderList.count(); i++) {
        if(i < 16) {
            ctrlLayout->addWidget(_eqSliderList.at(i), Qt::AlignCenter);
        }else {
            ctrlLayout16->addWidget(_eqSliderList.at(i), Qt::AlignCenter);
        }
    }
    QVBoxLayout *labCtrlLayout = new QVBoxLayout();
    labCtrlLayout->setContentsMargins(0, 90, 0, 0);
    QLabel *labCtrl_bypass = new QLabel(tr("Bypass")); labCtrl_bypass->setAlignment(Qt::AlignRight);
    QLabel *labCtrl_Gain = new QLabel(tr("Gain(dB)")); labCtrl_Gain->setAlignment(Qt::AlignRight);
    labCtrl_Gain->setFixedHeight(85);        labCtrlLayout->addWidget(labCtrl_Gain, Qt::AlignRight);
    labCtrl_bypass->setFixedHeight(18);      labCtrlLayout->addWidget(labCtrl_bypass, Qt::AlignRight);
    layout->addLayout(labCtrlLayout, 1, 0);

    ctrlBox->setFixedWidth(ctrlBox->layout()->count()*41);
    layout->addWidget(ctrlBox, 1, 1, Qt::AlignLeft);
    if(_type == SHG_1X3) {
        QVBoxLayout *labCtrl16Layout = new QVBoxLayout();
        labCtrl16Layout->setContentsMargins(0, 90, 0, 0);
        QLabel *labCtrl16_bypass = new QLabel(tr("Bypass")); labCtrl16_bypass->setAlignment(Qt::AlignRight);
        QLabel *labCtrl16_Gain = new QLabel(tr("Gain(dB)")); labCtrl16_Gain->setAlignment(Qt::AlignRight);
        labCtrl16_Gain->setFixedHeight(85);     labCtrl16Layout->addWidget(labCtrl16_Gain, Qt::AlignRight);
        labCtrl16_bypass->setFixedHeight(18);   labCtrl16Layout->addWidget(labCtrl16_bypass, Qt::AlignRight);
        layout->addLayout(labCtrl16Layout, 2, 0);

        ctrlBox16->setFixedWidth(ctrlBox16->layout()->count()*41);
        layout->addWidget(ctrlBox16, 2, 1, Qt::AlignLeft);
    }else {
        delete ctrlBox16; ctrlBox16 = 0;
    }


    QGridLayout *btnLayout = new QGridLayout();

    btnLayout->setContentsMargins((_eqSliderList.count() > 16? (_eqSliderList.count()-16)*41 : _eqSliderList.count()*41)-120, 0, 0, 0);
    btnLayout->setVerticalSpacing(2);
    btnLayout->setHorizontalSpacing(9);
    _bypassBtn->setCheckable(true);
    _bypassBtn->setChecked(true);
    _bypassBtn->setProperty("form", "Bypass");
    btnLayout->addWidget(new QLabel(QApplication::translate("EqViewSHG", "Bypass", 0)), 0, 1, 1, 1, Qt::AlignCenter);
    btnLayout->addWidget(new QLabel(QApplication::translate("EqViewSHG", "Clear All", 0)), 0, 2, 1, 1, Qt::AlignCenter);
    btnLayout->addWidget(_bypassBtn, 1, 1, Qt::AlignCenter);
    btnLayout->addWidget(_reset, 1, 2, Qt::AlignCenter);
    connect(_reset, SIGNAL(clicked()), this, SLOT(onResetClicked()));
    layout->addLayout(btnLayout, 3, 1, 1, 1, Qt::AlignLeft);
}

void EqViewSHG::reinitEqData(bool bypass, QList<EqData *> dataList)
{
    int i = 0;
    disconnectSig();

    _bypassBtn->setChecked(bypass);
    _eqGraph->setBypass(bypass);

    foreach(EqData *eqData, dataList)
    {
        _eqGraph->setValue(i, eqData->freq, eqData->gain, eqData->q);

        _eqSliderList.at(i)->setGainValue(eqData->gain);

        i++;
    }

    connectSig();
}

void EqViewSHG::connectSig()
{
    connect(_bypassBtn, SIGNAL(clicked(bool)), this, SLOT(onSetBypass(bool)));

    foreach (EqSlider *eqSlider, _eqSliderList) {
        connect(eqSlider, SIGNAL(gainChanged(int,int)), this, SLOT(onGainChanged(int,int)));
        connect(eqSlider, SIGNAL(bypassClicked(int, bool)), this, SLOT(onSegBypassChanged(int, bool)));
    }
}

void EqViewSHG::disconnectSig()
{
    disconnect(_bypassBtn, SIGNAL(clicked(bool)), this, SLOT(onSetBypass(bool)));

    foreach (EqSlider *eqSlider, _eqSliderList) {
        disconnect(eqSlider, SIGNAL(gainChanged(int,int)), this, SLOT(onGainChanged(int,int)));
        disconnect(eqSlider, SIGNAL(bypassClicked(int, bool)), this, SLOT(onSegBypassChanged(int, bool)));
    }
}


void EqViewSHG::setSetBypass(int idx, bool enable)
{
    _eqGraph->setSegBypass(idx, enable);
    _eqSliderList.at(idx)->setBypass(enable);
}

void EqViewSHG::onGainChanged(int idx, int gain)
{
    _eqGraph->onGainChanged(idx, gain);
    emit eqDataChanged(idx, gain);
}

void EqViewSHG::onSegBypassChanged(int idx, bool enable)
{
    _eqGraph->setSegBypass(idx, enable);
    emit eqDataSegBypassClicked(idx, enable);
}

void EqViewSHG::onValueChanged(int idx, int fs, int gain, int q)
{
    Q_UNUSED(fs)
    Q_UNUSED(q)

    disconnectSig();

    _eqSliderList.at(idx)->setGainValue(gain);

    connectSig();

    emit eqDataChanged(idx, gain);
}

void EqViewSHG::onSetBypass(bool isBypass)
{
    _eqGraph->setBypass(isBypass);
    eqDataBypassClicked(isBypass);
}

void EqViewSHG::onResetClicked()
{
    emit eqDataReset();
//    QList<EqData *> eqList;
//    QList<int> freqList;
//    double _qValue;
//    switch (_eqSliderList.count()) {
//    case 10:
//        freqList<<32<<63<<125<<250<<500<<1000<<2000<<4000<<8000<<16000;
//        _qValue = 1.4;
//        break;
//    case 15:
//        freqList<<25<<40<<63<<100<<160<<250<<400<<635<<1000<<1600<<2500
//                    <<4000<<6400<<10000<<16000;
//        _qValue = 2.1;
//        break;
//    case 31:
//        freqList<<20<<25<<32<<40<<50<<63<<80<<100<<125<<160<<200<<250<<320
//                    <<400<<500<<640<<800<<1000<<1300<<1600<<2000<<2600<<3200
//                   <<4000<<5100<<6500<<8100<<10000<<13000<<16000<<20000;
//        _qValue = 4.3;
//        break;
//    }

//    for(int i = 0; i < _eqSliderList.count(); i++)
//    {
//        EqData *eqData = new EqData;
//        eqData->freq = freqList.at(i);
//        eqData->gain = 0;
//        eqData->q = _qValue * 100;
//        eqList.append(eqData);
////        qDebug() << __FUNCTION__ << __LINE__
////                 << _eqList.last()->freq;
//    }

//    reinitEqData(false, eqList);
}
