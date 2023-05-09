#include "stable.h"
#include "cfgsettingmixer.h"
#include "configItem/configitemmixer.h"
#include "customWidget/configmixerpushbutton.h"
#include "customWidget/configgainwidget.h"
#include <QScrollArea>
#include "customWidget/configmixergainwidget.h"
#include "customWidget/configmixernewpushbutton.h"
//#include "customWidget/configdelaynewdly.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

CfgSettingMixer::CfgSettingMixer(ConfigItemMixer *cfgItem, QWidget *parent)
    : QWidget(parent)
{
    _colPosetionHead = 2;
    _rowPosetionHead = 2;
    CfgType cfgType = cfgItem->getCfgType();

    _inLabBtnList.clear();
    _outLabBtnList.clear();

    switch(cfgType) {
    case CFG_TYPE_MIXER_4X1:
        _rowNum = 1,_colNum = 4;
        break;
    case CFG_TYPE_MIXER_4X2:
        _rowNum = 2, _colNum = 4;
        break;
    case CFG_TYPE_MIXER_4X4:
        _rowNum = 4, _colNum = 4;
        break;
    case CFG_TYPE_MIXER_8X1:
        _rowNum = 1, _colNum = 8;
        break;
    case CFG_TYPE_MIXER_8X2:
        _rowNum = 2, _colNum = 8;
        break;
    case CFG_TYPE_MIXER_8X4:
        _rowNum = 4, _colNum = 8;
        break;
    case CFG_TYPE_MIXER_8X8:
        _rowNum = 8, _colNum = 8;
        break;
    case CFG_TYPE_MIXER_16X16:
        _rowNum = 16, _colNum = 16;
        break;
    case CFG_TYPE_MIXER_32X32:
        _rowNum = 32; _colNum = 32;
        break;
    case CFG_TYPE_MIXER_64X64:
        _rowNum = 64; _colNum = 64;
        break;
    default: break;
    }

    _cfgItemMixer = cfgItem;

    //关联 cfgItem的参数，具体化cfgItem
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
        setupUi_FangTu();
    else
        setupUi();
}

void CfgSettingMixer::setNodesSwitch()
{
    foreach(ConfigGainWidget *item, _routerBtnList) {
        item->SetEnbleClicked(_cfgItemMixer->getNodesSwitch());
        item->setValue(-100, true);
        item->setMute(false);
    }
}

void CfgSettingMixer::updateUiData()
{
        for(int i = 0; i < _routerBtnList.count(); i++)
        {
            disconnect(_routerBtnList[i], SIGNAL(muteChanged(bool)), this, SLOT(onRouterMuteChanged()));
            disconnect(_routerBtnList[i], SIGNAL(valueChanged(double)), this, SLOT(onRouterGainChanged(double)));

            int row = _routerBtnList[i]->property("row").toInt();
            int col = _routerBtnList[i]->property("col").toInt();
//            qDebug()<< __FUNCTION__ << __LINE__ << row <<col << _cfgItemMixer->getRouterGain(row, col);
            _routerBtnList[i]->setMute(_cfgItemMixer->getRouterMute(row, col));
            _routerBtnList[i]->setValue(_cfgItemMixer->getRouterGain(row, col));

            connect(_routerBtnList[i], SIGNAL(muteChanged(bool)), this, SLOT(onRouterMuteChanged()));
            connect(_routerBtnList[i], SIGNAL(valueChanged(double)), this, SLOT(onRouterGainChanged(double)));
        }
        for(int i = 0; i < _OutBtnList.count(); i++)
        {
            if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
                disconnect(_outMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
                disconnect(_OutBtnList[i], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
                disconnect(_outLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));

                int row = _OutBtnList[i]->property("row").toInt();

                _outMuteBtnList[i]->setChecked(!_cfgItemMixer->getOutGainMute(row).mute);
                _OutBtnList[i]->setValue(_cfgItemMixer->getOutGainMute(row).gain);

                _outLineEdit[i]->setText(_cfgItemMixer->getLabText(OUT, row));

                connect(_outMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
                connect(_OutBtnList[i], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
                connect(_outLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            }
            else {
                disconnect(_outMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
                disconnect(_OutBtnList[i], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
                disconnect(_outLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));

                int row = _OutBtnList[i]->property("row").toInt();

                _outMuteBtnList[i]->setChecked(!_cfgItemMixer->getOutGainMute(row).mute);
                _OutBtnList[i]->setValue(_cfgItemMixer->getOutGainMute(row).gain);

                _outLineEdit[i]->setText(_cfgItemMixer->getLabText(OUT, row));

                connect(_outMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
                connect(_OutBtnList[i], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
                connect(_outLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            }
        }
        for(int i = 0; i < _InBtnList.count(); i++)
        {
            if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
                disconnect(_inMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
                disconnect(_InBtnList[i], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
                disconnect(_InLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));

                int col = _InBtnList[i]->property("col").toInt();
    //            qDebug()<<__FUNCTION__<<__LINE__<<col;
                _inMuteBtnList[i]->setChecked(!_cfgItemMixer->getInGainMute(col).mute);
                _InBtnList[i]->setValue(_cfgItemMixer->getInGainMute(col).gain);
                _InLineEdit[i]->setText(_cfgItemMixer->getLabText(IN, col));

                connect(_inMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
                connect(_InBtnList[i], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
                connect(_InLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            }
            else{
                disconnect(_inMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
                disconnect(_InBtnList[i], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
                disconnect(_InLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));

                int col = _InBtnList[i]->property("col").toInt();
    //            qDebug()<<__FUNCTION__<<__LINE__<<col;
                _inMuteBtnList[i]->setChecked(!_cfgItemMixer->getInGainMute(col).mute);
                _InBtnList[i]->setValue(_cfgItemMixer->getInGainMute(col).gain);
                _InLineEdit[i]->setText(_cfgItemMixer->getLabText(IN, col));

                connect(_inMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
                connect(_InBtnList[i], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
                connect(_InLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            }
        }
}

void CfgSettingMixer::onOutMuteChanged()
{
    QPushButton *outMuteBtn = qobject_cast<QPushButton *>(sender());
    int row = outMuteBtn->property("row").toInt();
//    qDebug() << __FUNCTION__ << __LINE__ << " row: " << row << outMuteBtn->isChecked();
    _cfgItemMixer->setOutGainMute(row, _cfgItemMixer->getOutGainMute(row).gain, !outMuteBtn->isChecked());
}

void CfgSettingMixer::onInMuteChanged()
{
    QPushButton *inMuteBtn = qobject_cast<QPushButton *>(sender());
    int col = inMuteBtn->property("col").toInt();
    _cfgItemMixer->setInGainMute(col, _cfgItemMixer->getInGainMute(col).gain, !inMuteBtn->isChecked());
}

void CfgSettingMixer::setupUi()
{
    int fixedW = ConfigGainWidget::getFixedWidth();
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(11, 0, 11, 11);
    mainLayout->setSpacing(1);

    addlabPage(mainLayout);
    addLabelAreaIn(mainLayout);
    addLabelAreaOut(mainLayout);

    QGroupBox *btnBox = new QGroupBox();
    QGroupBox *inBox = new QGroupBox(/*"In Gain Mute"*/);
    QGroupBox *outBox = new QGroupBox(/*"Out Gain Mute"*/);
    inBox->setProperty("form", "left");
    outBox->setProperty("form", "left");
    QGridLayout *BtnLayout = new QGridLayout();
    QHBoxLayout *inLayout  = new QHBoxLayout();
    QVBoxLayout *outLayout = new QVBoxLayout();

    int spt_r = 0, spt_c = 0;
    for(int row = 0; row < _rowNum; row++) {
        if(row >= 16){ break; }
        spt_c = 0;
        for(int col = 0; col < _colNum; col++) {
            if(col >= 32) { break; }

            ConfigGainWidget *Btn = new ConfigGainWidget(_cfgItemMixer->getRouterGain(row, col), 6, -100,
                                                         ParamType_MixerGain);
            Btn->setDecimalsNum(2);
            Btn->setFixedSize(fixedW, fixedW);
            Btn->setProperty("row", row);
            Btn->setProperty("col", col);
            Btn->setMute(_cfgItemMixer->getRouterMute(row, col));

            Btn->SetEnbleClicked(_cfgItemMixer->getNodesSwitch());
            connect(Btn, SIGNAL(valueChanged(double)), this, SLOT(onRouterGainChanged(double)));
            BtnLayout->addWidget(Btn, row + spt_r, col + spt_c, 1, 1, Qt::AlignCenter);

            _routerBtnList.append(Btn);

            int flag = col + 1;
            if(!(flag % 4) && flag != _colNum && flag < 32) {
                spt_c++;
                QLabel *lab = new QLabel(""); lab->setFixedSize(5, 5);
                BtnLayout->addWidget(lab, row + spt_r, col + spt_c, 1, 1, Qt::AlignCenter);
            }
        }

        GainMute gainMute = _cfgItemMixer->getOutGainMute(row);
        int col = 0;
        if(_colNum >= 32)
        {
            col = 32;
        }else {
            col = _colNum;
        }

        QHBoxLayout *outboxLayout  = new QHBoxLayout; outboxLayout->setContentsMargins(0, 0, 0, 0);
        ConfigGainWidget *OutGain = new ConfigGainWidget(gainMute.gain, 12, -100,
                                                     ParamType_Gain);
        OutGain->setDecimalsNum(2);
        OutGain->setFixedSize(fixedW, fixedW);
        OutGain->setProperty("row", row);
        OutGain->SetEnbleClicked(false);

        QPushButton *outMuteBtn = new QPushButton();
        outMuteBtn->setFixedSize(fixedW, 18);
        outMuteBtn->setCheckable(true);
        outMuteBtn->setProperty("form", "Mute");
        outMuteBtn->setProperty("row", row);
//        qDebug() << __FUNCTION__ << __LINE__ << gainMute.mute << outMuteBtn->isChecked();
        outMuteBtn->setChecked(!gainMute.mute);
        connect(outMuteBtn, SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
        connect(OutGain, SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
        _outMuteBtnList.append(outMuteBtn);
        outboxLayout->addWidget(OutGain, Qt::AlignCenter);
        outboxLayout->addWidget(outMuteBtn/*, Qt::AlignCenter*/);
        outLayout->addLayout(outboxLayout, Qt::AlignCenter);

        _OutBtnList.append(OutGain);
        int flag = row + 1;
        if(!(flag % 4) && flag != _rowNum && flag < 16) {
            spt_r++;
            QLabel *lab = new QLabel(" "); lab->setFixedSize(5, 5);
            QLabel *lab1 = new QLabel(" "); lab1->setFixedSize(5, 5);
            BtnLayout->addWidget(lab, row + spt_r, col);
            outLayout->addWidget(lab1);

//            BtnLayout->setRowMinimumHeight(row + spt_r, 5);
        }
    }

    for(int col = 0; col< _colNum; col++)
    {
        if(col >= 32) break;
        GainMute gainMute = _cfgItemMixer->getInGainMute(col);

        QVBoxLayout *inBoxLayout = new QVBoxLayout;
        ConfigGainWidget *inGain = new ConfigGainWidget(gainMute.gain, 12, -100,
                                                                     ParamType_Gain);
        inGain->setDecimalsNum(2);
        inGain->setFixedSize(fixedW, fixedW);
        inGain->setProperty("col", col);
        inGain->SetEnbleClicked(false);

        QPushButton *InMuteBtn = new QPushButton();
        InMuteBtn->setFixedSize(fixedW, 18);
        InMuteBtn->setCheckable(true);
        InMuteBtn->setProperty("form", "Mute");
        InMuteBtn->setProperty("col", col);
        InMuteBtn->setChecked(!gainMute.mute);
        connect(InMuteBtn, SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
        connect(inGain, SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
        _inMuteBtnList.append(InMuteBtn);
        inBoxLayout->addWidget(inGain, Qt::AlignCenter);
        inBoxLayout->addWidget(InMuteBtn, Qt::AlignCenter);
//        inLayout->addWidget(inGain, Qt::AlignCenter);
        inLayout->addLayout(inBoxLayout, Qt::AlignCenter);

        int flag = col + 1;
        if(!(flag % 4) && flag != _colNum && flag < 32)
            inLayout->addWidget(new QLabel(""), Qt::AlignCenter);
        _InBtnList.append(inGain);
    }

    btnBox->setLayout(BtnLayout);
    inBox->setLayout(inLayout);
    outBox->setLayout(outLayout);

    mainLayout->addWidget(btnBox, _rowPosetionHead, _colPosetionHead, _rowNum, _colNum/*, Qt::AlignCenter*/);
    mainLayout->addWidget(outBox, _rowPosetionHead, _colPosetionHead+_colNum, _rowNum, 1/*, Qt::AlignCenter*/);
    mainLayout->addWidget(inBox, _rowPosetionHead+_rowNum, _colPosetionHead, 1, _colNum/*, Qt::AlignCenter*/);

    QLabel *labIn_Gain = new QLabel(tr("Gain"));  labIn_Gain->setAlignment(Qt::AlignRight);
    QLabel *labIn_Mute = new QLabel(tr("Mute"));  labIn_Mute->setAlignment(Qt::AlignRight);
    QLabel *labOut_Gain = new QLabel(tr("Gain")); labOut_Gain->setAlignment(Qt::AlignCenter);
    QLabel *labOut_Mute = new QLabel(tr("Mute")); labOut_Mute->setAlignment(Qt::AlignCenter);

    QVBoxLayout *inGainMute = new QVBoxLayout();
    inGainMute->setContentsMargins(0, 35, 0, 0);
    inGainMute->addWidget(labIn_Gain, Qt::AlignBottom);
    inGainMute->addWidget(labIn_Mute, Qt::AlignBottom);

    QHBoxLayout *outGainMute = new QHBoxLayout();
    outGainMute->addWidget(labOut_Gain, Qt::AlignBottom);
    outGainMute->addWidget(labOut_Mute, Qt::AlignBottom);

    mainLayout->addLayout(inGainMute, _rowPosetionHead+_rowNum, _colPosetionHead-1, 1, 1, Qt::AlignBottom);
    mainLayout->addLayout(outGainMute, _rowPosetionHead-1, _colPosetionHead+_colNum, 1, 1, Qt::AlignBottom);

    initDisplayRange();
}


void CfgSettingMixer::setupUi_FangTu()
{
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(11, 0, 11, 11);
    mainLayout->setSpacing(1);

    addlabPage(mainLayout);
    addLabelAreaIn(mainLayout);
    addLabelAreaOut(mainLayout);



    QGroupBox *btnBox = new QGroupBox();
    QGroupBox *inBox = new QGroupBox(/*"In Gain Mute"*/);
    QGroupBox *outBox = new QGroupBox(/*"Out Gain Mute"*/);
    inBox->setProperty("form", "left");
    outBox->setProperty("form", "left");
    QGridLayout *BtnLayout = new QGridLayout();
    QHBoxLayout *inLayout  = new QHBoxLayout();
    QVBoxLayout *outLayout = new QVBoxLayout();

    int spt_r = 0, spt_c = 0;
    for(int row = 0; row < _rowNum; row++) {
        if(row >= 16){ break; }
        spt_c = 0;
        for(int col = 0; col < _colNum; col++) {
            if(col >= 32) { break; }

            ConfigGainWidget *Btn = new ConfigGainWidget(_cfgItemMixer->getRouterGain(row, col), 6, -100,
                                                         ParamType_MixerGain);
            Btn->setDecimalsNum(2);
            Btn->setFixedSize(50, 50);
            Btn->setProperty("row", row);
            Btn->setProperty("col", col);
            Btn->setMute(_cfgItemMixer->getRouterMute(row, col));
            Btn->SetEnbleClicked(_cfgItemMixer->getNodesSwitch());
            connect(Btn, SIGNAL(valueChanged(double)), this, SLOT(onRouterGainChanged(double)));
//            BtnLayout->addWidget(Btn, row, col, 1, 1, Qt::AlignCenter);
            BtnLayout->addWidget(Btn, row + spt_r, col + spt_c, 1, 1, Qt::AlignCenter);

            _routerBtnList.append(Btn);

            int flag = col + 1;
            if(!(flag % 4) && flag != _colNum && flag < 32) {
                spt_c++;
                QLabel *lab = new QLabel(""); lab->setFixedSize(5, 5);
                BtnLayout->addWidget(lab, row + spt_r, col + spt_c, 1, 1, Qt::AlignCenter);
            }
        }

        GainMute gainMute = _cfgItemMixer->getOutGainMute(row);
        int col = 0;
        if(_colNum >= 32)
        {
            col = 32;
        }else {
            col = _colNum;
        }

        QHBoxLayout *outboxLayout  = new QHBoxLayout; outboxLayout->setContentsMargins(0, 0, 0, 0);
        ConfigGainWidget *OutGain = new ConfigGainWidget(gainMute.gain, 12, -100,
                                                     ParamType_Gain);
        OutGain->setDecimalsNum(2);
        OutGain->setFixedSize(50, 50);
        OutGain->setProperty("row", row);
        OutGain->SetEnbleClicked(false);
//        OutGain->setChecked(gainMute.mute);

        QPushButton *outMuteBtn = new QPushButton();
        outMuteBtn->setFixedSize(50, 18);
        outMuteBtn->setCheckable(true);
        outMuteBtn->setProperty("form", "Mute");
        outMuteBtn->setProperty("row", row);
//        qDebug() << __FUNCTION__ << __LINE__ << gainMute.mute << outMuteBtn->isChecked();
        outMuteBtn->setChecked(!gainMute.mute);
//        qDebug() << __FUNCTION__ << __LINE__ << gainMute.mute << outMuteBtn->isChecked();
        connect(outMuteBtn, SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
        connect(OutGain, SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
        _outMuteBtnList.append(outMuteBtn);
//        mainLayout->addWidget(OutGain, row+_rowPosetionHead, _colPosetionHead+col+1, 1, 1, Qt::AlignHCenter);
        outboxLayout->addWidget(OutGain, Qt::AlignCenter);
        outboxLayout->addWidget(outMuteBtn/*, Qt::AlignCenter*/);
        outLayout->addLayout(outboxLayout, Qt::AlignCenter);

        _OutBtnList.append(OutGain);
        int flag = row + 1;
        if(!(flag % 4) && flag != _rowNum && flag < 16) {
            spt_r++;
            QLabel *lab = new QLabel(" "); lab->setFixedSize(5, 5);
            QLabel *lab1 = new QLabel(" "); lab1->setFixedSize(5, 5);
            BtnLayout->addWidget(lab, row + spt_r, col);
            outLayout->addWidget(lab1);

//            BtnLayout->setRowMinimumHeight(row + spt_r, 5);
        }
    }

    for(int col = 0; col< _colNum; col++)
    {
        if(col >= 32) break;
        GainMute gainMute = _cfgItemMixer->getInGainMute(col);

        QVBoxLayout *inBoxLayout = new QVBoxLayout;
        ConfigGainWidget *inGain = new ConfigGainWidget(gainMute.gain, 12, -100,
                                                                     ParamType_Gain);
        inGain->setDecimalsNum(2);
        inGain->setFixedSize(50, 50);
        inGain->setProperty("col", col);
//        inGain->setChecked(gainMute.mute);
        inGain->SetEnbleClicked(false);

        QPushButton *InMuteBtn = new QPushButton();
        InMuteBtn->setFixedSize(50, 18);
        InMuteBtn->setCheckable(true);
        InMuteBtn->setProperty("form", "Mute");
        InMuteBtn->setProperty("col", col);
        InMuteBtn->setChecked(!gainMute.mute);
        connect(InMuteBtn, SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
        connect(inGain, SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
        _inMuteBtnList.append(InMuteBtn);
        inBoxLayout->addWidget(inGain, Qt::AlignCenter);
        inBoxLayout->addWidget(InMuteBtn, Qt::AlignCenter);
//        inLayout->addWidget(inGain, Qt::AlignCenter);
        inLayout->addLayout(inBoxLayout, Qt::AlignCenter);

        int flag = col + 1;
        if(!(flag % 4) && flag != _colNum && flag < 32)
            inLayout->addWidget(new QLabel(""), Qt::AlignCenter);
        _InBtnList.append(inGain);
    }

    btnBox->setLayout(BtnLayout);
    inBox->setLayout(inLayout);
    outBox->setLayout(outLayout);

    mainLayout->addWidget(btnBox, _rowPosetionHead, _colPosetionHead, _rowNum, _colNum/*, Qt::AlignCenter*/);
    mainLayout->addWidget(outBox, _rowPosetionHead, _colPosetionHead+_colNum, _rowNum, 1/*, Qt::AlignCenter*/);
    mainLayout->addWidget(inBox, _rowPosetionHead+_rowNum, _colPosetionHead, 1, _colNum/*, Qt::AlignCenter*/);

    QLabel *labIn_Gain = new QLabel(tr("Gain"));  labIn_Gain->setAlignment(Qt::AlignRight);
    QLabel *labIn_Mute = new QLabel(tr("Mute"));  labIn_Mute->setAlignment(Qt::AlignRight);
    QLabel *labOut_Gain = new QLabel(tr("Gain")); labOut_Gain->setAlignment(Qt::AlignCenter);
    QLabel *labOut_Mute = new QLabel(tr("Mute")); labOut_Mute->setAlignment(Qt::AlignCenter);

    QVBoxLayout *inGainMute = new QVBoxLayout();
    inGainMute->setContentsMargins(0, 35, 0, 0);
    inGainMute->addWidget(labIn_Gain, Qt::AlignBottom);
    inGainMute->addWidget(labIn_Mute, Qt::AlignBottom);

    QHBoxLayout *outGainMute = new QHBoxLayout();
    outGainMute->addWidget(labOut_Gain, Qt::AlignBottom);
    outGainMute->addWidget(labOut_Mute, Qt::AlignBottom);

    mainLayout->addLayout(inGainMute, _rowPosetionHead+_rowNum, _colPosetionHead-1, 1, 1, Qt::AlignBottom);
    mainLayout->addLayout(outGainMute, _rowPosetionHead-1, _colPosetionHead+_colNum, 1, 1, Qt::AlignBottom);

    initDisplayRange();
}



void CfgSettingMixer::onRouterMuteChanged()
{
    ConfigGainWidget *Btn = qobject_cast<ConfigGainWidget *>(sender());
    int row = Btn->property("row").toInt();
    int col = Btn->property("col").toInt();
    _cfgItemMixer->setRouterInfo(row, col, Btn->isMute());
//    qDebug() << __FUNCTION__ << __LINE__ << " isChecked: " << Btn->isMute() << " row: " << row << " col: " << col;
}

void CfgSettingMixer::onRouterGainChanged(double value)
{
    int row = sender()->property("row").toInt();
    int col = sender()->property("col").toInt();
//    qDebug() << __FUNCTION__ << __LINE__ << " value: " << value << " row: " << row << " col: " << col;
    _cfgItemMixer->setRouterGain(row, col, value);
}


void CfgSettingMixer::onInGainValueChanged(double value)
{
    ConfigGainWidget *inGain = qobject_cast<ConfigGainWidget *>(sender());

    int col = inGain->property("col").toInt();
    _cfgItemMixer->setInGainMute(col, value, _cfgItemMixer->getInGainMute(col).mute);
//    qDebug()<<__FUNCTION__<<__LINE__<< " col: " << col << "value: " << value <<_cfgItemMixer->getInGainMute(col).mute;
}


void CfgSettingMixer::onOutGainValueChanged(double value)
{
    ConfigGainWidget *outGain = qobject_cast<ConfigGainWidget *>(sender());
    int row = outGain->property("row").toInt();
//    qDebug() << __FUNCTION__ << __LINE__ << " row: " << row << outGain->getValue() << outGain->isChecked();
    _cfgItemMixer->setOutGainMute(row, value, _cfgItemMixer->getOutGainMute(row).mute);
}

void CfgSettingMixer::onLabTextChanged()
{
    QTextEdit *lab = qobject_cast<QTextEdit *>(sender());
    int row = lab->property("row").toInt();
    int col = lab->property("col").toInt();
//     qDebug()<<__FUNCTION__<<__LINE__<< " row: " << row << " col: " << col << lab->document()->toPlainText();
    _cfgItemMixer->setLabText(row, col, lab->document()->toPlainText());
}

void CfgSettingMixer::onInLabelPageChanged()
{
    //qDebug()<<__FUNCTION__<<__LINE__<<sender()->property("text");

    if(sender()->property("text").toString() == "1~32")
    {
        for(int col = 0; col < 32; col++)
        {
            disconnect(_InLineEdit[col], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            _InLabelList[col]->setText(QString::number(col+1));
            _InLineEdit[col]->setText(_cfgItemMixer->getLabText(IN, col));
            _InLineEdit[col]->setProperty("col", col);
            connect(_InLineEdit[col], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
        updateNodeData(_currentMinRow, _currentMaxRow, 0, 32);
        _currentMinCol = 0;
        _currentMaxCol = 32;
    }
    if(sender()->property("text").toString() == "33~64")
    {
        for(int col = 32; col < 64; col++)
        {
            disconnect(_InLineEdit[col-32], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            _InLabelList[col-32]->setText(QString::number(col+1));
            _InLineEdit[col-32]->setText(_cfgItemMixer->getLabText(IN, col));
            _InLineEdit[col-32]->setProperty("col", col);
            connect(_InLineEdit[col-32], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
        updateNodeData(_currentMinRow, _currentMaxRow, 32, 64);
        _currentMinCol = 32;
        _currentMaxCol = 64;
    }

    if(sender()->property("text").toString() == "Prev")
    {
//        qDebug()<<__FUNCTION__<<__LINE__<<_currentMinCol<<_currentMaxCol;
        if(_currentMaxCol - 32 == 0)
        {
            return;
        }
        _currentMinCol -= 32;
        _currentMaxCol -= 32;
        _cfgItemMixer->setDisplayRange(2, _currentMinCol);
        _cfgItemMixer->setDisplayRange(3, _currentMaxCol);

        initDisplayRange();
    }else if(sender()->property("text").toString() == "Next")
    {
        if(_currentMaxCol == _colNum)
        {
            return;
        }
        _currentMinCol += 32;
        _currentMaxCol += 32;
        _cfgItemMixer->setDisplayRange(2, _currentMinCol);
        _cfgItemMixer->setDisplayRange(3, _currentMaxCol);

        initDisplayRange();
    }else{
        foreach (QPushButton *btn, _inLabBtnList) {
           if(btn != qobject_cast<QPushButton *>(sender()))
           {
               btn->setChecked(false);
           }else{
               btn->setChecked(true);
           }
        }
        _cfgItemMixer->setDisplayRange(2, _currentMinCol);
        _cfgItemMixer->setDisplayRange(3, _currentMaxCol);
    }
}

void CfgSettingMixer::onOutLabelPageChanged()
{
    if(sender()->property("text").toString() == "1~16")
    {
        for(int row = 0; row < 16; row++)
        {
            disconnect(_outLineEdit[row], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            _outLabelList[row]->setText(QString::number(row+1));
            _outLineEdit[row]->setText(_cfgItemMixer->getLabText(OUT, row));
            _outLineEdit[row]->setProperty("col", row);
            connect(_outLineEdit[row], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
        updateNodeData(0, 16, _currentMinCol, _currentMaxCol);
        _currentMinRow = 0;
        _currentMaxRow = 16;
    }
    if(sender()->property("text").toString() == "17~32")
    {
        for(int row = 16; row < 32; row++)
        {
            disconnect(_outLineEdit[row-16], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            _outLabelList[row-16]->setText(QString::number(row+1));
            _outLineEdit[row-16]->setText(_cfgItemMixer->getLabText(OUT, row));
            _outLineEdit[row-16]->setProperty("col", row);
            connect(_outLineEdit[row-16], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
        updateNodeData(16, 32, _currentMinCol, _currentMaxCol);
        _currentMinRow = 16;
        _currentMaxRow = 32;
    }
    if(sender()->property("text").toString() == "33~48")
    {
        for(int row = 32; row < 48; row++)
        {
            disconnect(_outLineEdit[row-32], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            _outLabelList[row-32]->setText(QString::number(row+1));
//            _outLabelList[row-32]->setText(tr("Out %1").arg(row+1));
            _outLineEdit[row-32]->setText(_cfgItemMixer->getLabText(OUT, row));
            _outLineEdit[row-32]->setProperty("col", row);
            connect(_outLineEdit[row-32], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
        updateNodeData(32, 48, _currentMinCol, _currentMaxCol);
        _currentMinRow = 32;
        _currentMaxRow = 48;
    }
    if(sender()->property("text").toString() == "49~64")
    {
        for(int row = 48; row < 64; row++)
        {
            disconnect(_outLineEdit[row-48], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            _outLabelList[row-48]->setText(QString::number(row+1));
//            _outLabelList[row-48]->setText(tr("Out %1").arg(row+1));
            _outLineEdit[row-48]->setText(_cfgItemMixer->getLabText(OUT, row));
            _outLineEdit[row-48]->setProperty("col", row);
            connect(_outLineEdit[row-48], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
        updateNodeData(48, 64, _currentMinCol, _currentMaxCol);
        _currentMinRow = 48;
        _currentMaxRow = 64;
    }

    if(sender()->property("text").toString() == "Prev")
    {
        if(_currentMaxRow - 16 == 0)
        {
            return ;
        }
        _currentMinRow -= 16;
        _currentMaxRow -= 16;
        _cfgItemMixer->setDisplayRange(0, _currentMinRow);
        _cfgItemMixer->setDisplayRange(1, _currentMaxRow);

        initDisplayRange();
    }else if(sender()->property("text").toString() == "Next")
    {
        if(_currentMaxRow == _rowNum)
        {
            return ;
        }
        _currentMinRow += 16;
        _currentMaxRow += 16;
        _cfgItemMixer->setDisplayRange(0, _currentMinRow);
        _cfgItemMixer->setDisplayRange(1, _currentMaxRow);
        initDisplayRange();
    }else{
        foreach (QPushButton *btn, _outLabBtnList) {
           if(btn != qobject_cast<QPushButton *>(sender()))
           {
               btn->setChecked(false);
           }else {
               btn->setChecked(true);
           }
        }
        _cfgItemMixer->setDisplayRange(0, _currentMinRow);
        _cfgItemMixer->setDisplayRange(1, _currentMaxRow);
    }
}


void CfgSettingMixer::routerInfoChanged(int row, int col, bool status)
{
    _cfgItemMixer->setRouterInfo(row, col, status);
}

void CfgSettingMixer::addLabelAreaIn(QGridLayout *mainLayout)
{
    int fixedW = ConfigGainWidget::getFixedWidth();
    int inLabRow = -2;
    QHBoxLayout *inLabLayout = new QHBoxLayout;
    inLabLayout->setContentsMargins(11, 0, 11, 0);
    inLabLayout->setSpacing(6);
    QGroupBox *nameInLabBox = new QGroupBox;
    QHBoxLayout *nameInLabLayout = new QHBoxLayout(nameInLabBox);
    nameInLabBox->setProperty("form", "left");

    QVBoxLayout *inAndOutLab = new QVBoxLayout();
    inAndOutLab->setSpacing(11);
    QLabel *labIn = new QLabel(tr("Input"));  labIn->setAlignment(Qt::AlignRight);
    QLabel *labOut = new QLabel(tr("Output"));labOut->setAlignment(Qt::AlignCenter);
    inAndOutLab->addWidget(labIn);
    inAndOutLab->addWidget(labOut);
    mainLayout->addLayout(inAndOutLab, inLabRow+_rowPosetionHead+1, _colPosetionHead-1, 1, 1, Qt::AlignBottom);

    for(int col = 0; col < _colNum; col++)
    {
        if(col >= 32) { break; }
        QLabel *inLabel = new QLabel(this);
        inLabel->setAlignment(Qt::AlignCenter);
        inLabel->setText(QString::number(col + 1));
        inLabel->setFixedWidth(50);
        inLabLayout->addWidget(inLabel, Qt::AlignCenter);
        inLabLayout->setContentsMargins(0,0,0,0);
        inLabel->setText(tr("In %1").arg(col + 1));
        mainLayout->addWidget(inLabel, inLabRow+_rowPosetionHead, col+_colPosetionHead, 1, 1, Qt::AlignCenter);
        inLabel->setHidden(true);
        _InLabelList.append(inLabel);

        QTextEdit *nameInLab = new QTextEdit(this);
        nameInLab->setFixedSize(fixedW, fixedW);
        nameInLab->setFocusPolicy(Qt::ClickFocus);
        nameInLab->setText(_cfgItemMixer->getLabText(IN, col));
        nameInLab->setAlignment(Qt::AlignCenter);
        nameInLab->setProperty("row", 0);
        nameInLab->setProperty("col", col);
        connect(nameInLab, SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        nameInLabLayout->addWidget(nameInLab, Qt::AlignCenter);

        int flag = col + 1;
        if(!(flag % 4) && flag != _colNum && flag < 32) {
            QLabel *lab = new QLabel(""); lab->setFixedSize(5, 5);
            inLabLayout->addWidget(new QLabel(""), Qt::AlignCenter);
            nameInLabLayout->addWidget(lab, Qt::AlignCenter);
        }
        _InLineEdit.append(nameInLab);
    }
    mainLayout->addLayout(inLabLayout, inLabRow+_rowPosetionHead, _colPosetionHead, 1, _colNum, Qt::AlignCenter);
    mainLayout->addWidget(nameInLabBox, inLabRow+_rowPosetionHead+1, _colPosetionHead, 1, _colNum, Qt::AlignCenter);
}

void CfgSettingMixer::addLabelAreaOut(QGridLayout *mainLayout)
{
    int fixedW = ConfigGainWidget::getFixedWidth();
    int OutLabCol = -2;
    QVBoxLayout *outLabLayout = new QVBoxLayout;
    outLabLayout->setContentsMargins(0, 11, 0, 11);
    outLabLayout->setSpacing(6);

    QGroupBox *nameOutLabBox = new QGroupBox;
    QVBoxLayout *nameOutLabLayout = new QVBoxLayout(nameOutLabBox);
    nameOutLabBox->setProperty("form", "left");


    for(int row = 0; row < _rowNum; row++)
    {
        if(row >= 16) { break; }
        QLabel *outLabel = new QLabel(this);
        outLabel->setAlignment(Qt::AlignCenter);
        outLabel->setText(QString::number(row+1));
        outLabel->setFixedHeight(50);
        outLabLayout->addWidget(outLabel, Qt::AlignCenter);
        outLabel->setText(tr("Out %1").arg(row+1));
        mainLayout->addWidget(outLabel, row+_rowPosetionHead, OutLabCol+_colPosetionHead, 1, 1, Qt::AlignCenter);
        outLabel->setHidden(true);
        _outLabelList.append(outLabel);

        QTextEdit *nameOutLab = new QTextEdit(this);
        nameOutLab->setFixedSize(fixedW, fixedW);
        nameOutLab->setText(_cfgItemMixer->getLabText(OUT, row));
        nameOutLab->setAlignment(Qt::AlignCenter);
        nameOutLab->setProperty("row", 1);
        nameOutLab->setProperty("col", row);
        connect(nameOutLab, SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        nameOutLabLayout->addWidget(nameOutLab, Qt::AlignCenter);

        int flag = row + 1;
        if(!(flag % 4) && flag != _rowNum && flag < 16) {
            QLabel *lab = new QLabel(" "); lab->setFixedSize(7, 7);
            QLabel *lab1 = new QLabel(" "); lab->setFixedSize(7, 7);
            outLabLayout->addWidget(lab1, Qt::AlignCenter);
            nameOutLabLayout->addWidget(lab, Qt::AlignCenter);
        }
        _outLineEdit.append(nameOutLab);
    }
    mainLayout->addLayout(outLabLayout, _rowPosetionHead, OutLabCol+_colPosetionHead, _rowNum, 1, Qt::AlignCenter);
    mainLayout->addWidget(nameOutLabBox, _rowPosetionHead, OutLabCol+_colPosetionHead+1, _rowNum, 1/*, Qt::AlignCenter*/);
}

void CfgSettingMixer::addlabPage(QGridLayout *mainLayout)//分页标签
{
    if(_colNum / 32 > 1)
    {
        _rowPosetionHead = 3;

        QLabel *label = new QLabel(this);
        label->setAlignment(Qt::AlignCenter);
        label->setText("Input");
        label->setHidden(true);

        mainLayout->addWidget(label, 0, 3, 1, 1, Qt::AlignCenter);

        QPushButton *prevBtn = new QPushButton(this);
        prevBtn->setCheckable(false);
        prevBtn->setFixedSize(36, 18);
        prevBtn->setText("Prev");
        prevBtn->setProperty("text", "Prev");
        //prevBtn->setProperty("col", col);
        prevBtn->setFocusPolicy(Qt::NoFocus);
        //prevBtn->setChecked(getRouterChecked(row, col));
        connect(prevBtn, SIGNAL(clicked()), this, SLOT(onInLabelPageChanged()));
        mainLayout->addWidget(prevBtn, 0, 4, 1, 1, Qt::AlignCenter);
        //_inLabBtnList.append(prevBtn);

        for(int col = 0; col < _colNum / 32; col++)
        {
            int label = col * 32;
            QPushButton *btn = new QPushButton(this);
            btn->setCheckable(true);
            btn->setFixedSize(36, 18);
            btn->setText(QString("%1~%2").arg(label+1).arg(label+32));
            btn->setProperty("text", QString("%1~%2").arg(label+1).arg(label+32));
            //nextBtn->setProperty("col", col);
            btn->setFocusPolicy(Qt::NoFocus);
            //nextBtn->setChecked(getRouterChecked(row, col));
            connect(btn, SIGNAL(clicked()), this, SLOT(onInLabelPageChanged()));
            mainLayout->addWidget(btn, 0, 5+col, 1, 1, Qt::AlignCenter);
            _inLabBtnList.append(btn);
        }

        QPushButton *nextBtn = new QPushButton(this);
        nextBtn->setCheckable(false);
        nextBtn->setFixedSize(36, 18);
        nextBtn->setText("Next");
        nextBtn->setProperty("text", "Next");
        //nextBtn->setProperty("col", col);
        nextBtn->setFocusPolicy(Qt::NoFocus);
        //nextBtn->setChecked(getRouterChecked(row, col));
        connect(nextBtn, SIGNAL(clicked()), this, SLOT(onInLabelPageChanged()));
        mainLayout->addWidget(nextBtn, 0, (5+_colNum / 32), 1, 1, Qt::AlignCenter);
        //_inLabBtnList.append(nextBtn);
    }
    if(_rowNum / 16 > 1)
    {
        _colPosetionHead = 3;

        QLabel *label = new QLabel(this);
        label->setAlignment(Qt::AlignCenter);
        label->setText("Output");
        label->setHidden(true);

        mainLayout->addWidget(label, 3, 0, 1, 1, Qt::AlignCenter);

        QPushButton *prevBtn = new QPushButton(this);
        prevBtn->setCheckable(false);
        prevBtn->setFixedSize(36, 18);
        prevBtn->setText("Prev");
        prevBtn->setProperty("text", "Prev");
        //prevBtn->setProperty("col", col);
        prevBtn->setFocusPolicy(Qt::NoFocus);
        //prevBtn->setChecked(getRouterChecked(row, col));
        connect(prevBtn, SIGNAL(clicked()), this, SLOT(onOutLabelPageChanged()));
        mainLayout->addWidget(prevBtn, 4, 0, 1, 1, Qt::AlignCenter);
        //_outLabBtnList.append(prevBtn);

        for(int row = 0; row < _rowNum / 16; row++)
        {
            int label = row * 16;
            QPushButton *Btn = new QPushButton(this);
            Btn->setCheckable(true);
            Btn->setFixedSize(36, 18);
            Btn->setText(QString("%1~%2").arg(label+1).arg(label+16));
            Btn->setProperty("text", QString("%1~%2").arg(label+1).arg(label+16));
            //Btn->setProperty("col", col);
            Btn->setFocusPolicy(Qt::NoFocus);
            //Btn->setChecked(getRouterChecked(row, col));
            connect(Btn, SIGNAL(clicked()), this, SLOT(onOutLabelPageChanged()));
            mainLayout->addWidget(Btn, 5+row, 0, 1, 1, Qt::AlignCenter);
            _outLabBtnList.append(Btn);
        }

        QPushButton *nextBtn = new QPushButton(this);
        nextBtn->setCheckable(false);
        nextBtn->setFixedSize(36, 18);
        nextBtn->setText("Next");
        nextBtn->setProperty("text", "Next");
        //nextBtn->setProperty("col", col);
        nextBtn->setFocusPolicy(Qt::NoFocus);
        //nextBtn->setChecked(getRouterChecked(row, col));
        connect(nextBtn, SIGNAL(clicked()), this, SLOT(onOutLabelPageChanged()));
        mainLayout->addWidget(nextBtn, (5+_rowNum / 16), 0, 1, 1, Qt::AlignCenter);
        //_outLabBtnList.append(nextBtn);
    }
}

void CfgSettingMixer::updateNodeData(int minRow, int maxRow, int minCol, int maxCol)
{
//    qDebug() << __FUNCTION__ << __LINE__ << " minRow: " << minRow << " maxRow: " << maxRow << " minCol: " << minCol << " maxCol: " << maxCol;
    int i = 0;
    for(int row = minRow; row < maxRow; row++)
    {
        for(int col = minCol; col < maxCol; col++)
        {
            disconnect(_routerBtnList[i], SIGNAL(muteChanged(bool)), this, SLOT(onRouterMuteChanged()));
            disconnect(_routerBtnList[i], SIGNAL(valueChanged(double)), this, SLOT(onRouterGainChanged(double)));

            _routerBtnList[i]->setProperty("row", row);
            _routerBtnList[i]->setProperty("col", col);
            _routerBtnList[i]->setMute(_cfgItemMixer->getRouterMute(row, col));
            _routerBtnList[i]->setValue(_cfgItemMixer->getRouterGain(row, col));

            connect(_routerBtnList[i], SIGNAL(muteChanged(bool)), this, SLOT(onRouterMuteChanged()));
            connect(_routerBtnList[i], SIGNAL(valueChanged(double)), this, SLOT(onRouterGainChanged(double)));
            i++;
        }
    }


    for(int row = minRow; row < maxRow; row++)
    {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
            disconnect(_outMuteBtnList[row-minRow], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
            disconnect(_OutBtnList[row-minRow], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));

            _outMuteBtnList[row-minRow]->setProperty("row", row);
            _OutBtnList[row-minRow]->setProperty("row", row);
            _outMuteBtnList[row-minRow]->setChecked(!_cfgItemMixer->getOutGainMute(row).mute);
            _OutBtnList[row-minRow]->setValue(_cfgItemMixer->getOutGainMute(row).gain);
            connect(_outMuteBtnList[row-minRow], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
            connect(_OutBtnList[row-minRow], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
        }
        else{
            disconnect(_outMuteBtnList[row-minRow], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
            disconnect(_OutBtnList[row-minRow], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));

            _outMuteBtnList[row-minRow]->setProperty("row", row);
            _OutBtnList[row-minRow]->setProperty("row", row);
            _outMuteBtnList[row-minRow]->setChecked(!_cfgItemMixer->getOutGainMute(row).mute);
            _OutBtnList[row-minRow]->setValue(_cfgItemMixer->getOutGainMute(row).gain);
            connect(_outMuteBtnList[row-minRow], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
            connect(_OutBtnList[row-minRow], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
         }
    }

    for(int col = minCol; col < maxCol; col++)
    {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
            disconnect(_inMuteBtnList[col-minCol], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
            disconnect(_InBtnList[col-minCol], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));

            _inMuteBtnList[col-minCol]->setProperty("col", col);
            _InBtnList[col-minCol]->setProperty("col", col);
            _inMuteBtnList[col-minCol]->setChecked(!_cfgItemMixer->getInGainMute(col).mute);
            _InBtnList[col-minCol]->setValue(_cfgItemMixer->getInGainMute(col).gain);
            connect(_inMuteBtnList[col-minCol], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
            connect(_InBtnList[col-minCol], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
        }
        else{
            disconnect(_inMuteBtnList[col-minCol], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
            disconnect(_InBtnList[col-minCol], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));

            _inMuteBtnList[col-minCol]->setProperty("col", col);
            _InBtnList[col-minCol]->setProperty("col", col);
            _inMuteBtnList[col-minCol]->setChecked(!_cfgItemMixer->getInGainMute(col).mute);
            _InBtnList[col-minCol]->setValue(_cfgItemMixer->getInGainMute(col).gain);
            connect(_inMuteBtnList[col-minCol], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
            connect(_InBtnList[col-minCol], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
        }
    }


    update();
}

void CfgSettingMixer::initDisplayRange()
{
    _currentMinRow = _cfgItemMixer->getDisplayRange(0);
    _currentMaxRow = _cfgItemMixer->getDisplayRange(1);
    _currentMinCol = _cfgItemMixer->getDisplayRange(2);
    _currentMaxCol = _cfgItemMixer->getDisplayRange(3);

//    qDebug()<<__FUNCTION__<<__LINE__<<_currentMinRow<<_currentMaxRow<<_currentMinCol<<_currentMaxCol;
    if(_currentMaxRow == 0)
    {
        _currentMinRow = 0;
        if(_rowNum > 16)
        {
            _currentMaxRow = 16;
        }else {
            _currentMaxRow = _rowNum;
        }

    }
    if(_currentMaxCol == 0)
    {
        _currentMinCol = 0;
        if(_colNum > 32)
        {
            _currentMaxCol = 32;
        }else {
             _currentMaxCol = _colNum;
        }

    }

    if(!_inLabBtnList.isEmpty())
    {
//        qDebug()<<__FUNCTION__<<__LINE__<<_currentMinCol<<_currentMaxCol;
        foreach (QPushButton *btn, _inLabBtnList) {
            if(btn->property("text").toString() == QString("%1~%2").arg(_currentMinCol+1).arg(_currentMaxCol))
            {
                btn->setChecked(true);
            }else{
                btn->setChecked(false);
            }
        }

        for(int col = _currentMinCol; col < _currentMaxCol; col++)
        {
            disconnect(_InLineEdit[col-_currentMinCol], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            _InLabelList[col-_currentMinCol]->setText(QString::number(col+1));
            _InLineEdit[col-_currentMinCol]->setText(_cfgItemMixer->getLabText(IN, col));
            _InLineEdit[col-_currentMinCol]->setProperty("col", col);
            connect(_InLineEdit[col-_currentMinCol], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
    }

    if(!_outLabBtnList.isEmpty())
    {
        foreach (QPushButton *btn, _outLabBtnList) {
            if(btn->property("text").toString() == QString("%1~%2").arg(_currentMinRow+1).arg(_currentMaxRow))
            {
                btn->setChecked(true);
            }else{
                btn->setChecked(false);
            }
        }

        for(int row = _currentMinRow; row < _currentMaxRow; row++)
        {
            disconnect(_outLineEdit[row-_currentMinRow], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            _outLabelList[row-_currentMinRow]->setText(QString::number(row+1));
            _outLineEdit[row-_currentMinRow]->setText(_cfgItemMixer->getLabText(OUT, row));
            _outLineEdit[row-_currentMinRow]->setProperty("col", row);
            connect(_outLineEdit[row-_currentMinRow], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
    }

    updateNodeData(_currentMinRow, _currentMaxRow, _currentMinCol, _currentMaxCol);
}


