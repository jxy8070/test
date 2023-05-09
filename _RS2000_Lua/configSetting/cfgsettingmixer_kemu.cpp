#include "stable.h"
#include "cfgsettingmixer_kemu.h"
#include "configItem/configitemmixer.h"
#include "customWidget/configmixerpushbutton.h"
#include "customWidget/configgainwidget.h"
#include <QScrollArea>
#include "customWidget/configmixergainwidget.h"
#include "customWidget/configmixernewpushbutton.h"
#include "customWidget/gainlineedit.h"
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

CfgSettingMixer_Kemu::CfgSettingMixer_Kemu(ConfigItemMixer *cfgItem, QWidget *parent)
    : QWidget(parent)
{
    _colPosetionHead = 2;
    _rowPosetionHead = 2;
    CfgType cfgType = cfgItem->getCfgType();

    _colPageBtnList.clear();
    _rowPageBtnList.clear();

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

    setupUi();
}



void CfgSettingMixer_Kemu::updateUiData()
{
    for(int i = 0; i < _mixGainList.count(); i++)
    {
        disconnect(_mixGainList[i], SIGNAL(muteChanged(bool)), this, SLOT(onRouterMuteChanged()));
        disconnect(_mixGainList[i], SIGNAL(valueChanged(double)), this, SLOT(onRouterGainChanged(double)));

        int row = _mixGainList[i]->property("row").toInt();
        int col = _mixGainList[i]->property("col").toInt();
//        qDebug()<< __FUNCTION__ << __LINE__ << row <<col << _cfgItemMixer->getRouterGain(row, col);
        _mixMuteBtnList[i]->setChecked(_cfgItemMixer->getRouterMute(row, col));
        _mixGainList[i]->setValue(_cfgItemMixer->getRouterGain(row, col));

        connect(_mixGainList[i], SIGNAL(muteChanged(bool)), this, SLOT(onRouterMuteChanged()));
        connect(_mixGainList[i], SIGNAL(valueChanged(double)), this, SLOT(onRouterGainChanged(double)));
    }
    for(int i = 0; i < _outGainList.count(); i++)
    {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
            disconnect(_outMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
            disconnect(_outGainList[i], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
            disconnect(_outLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));

            int row = _outGainList[i]->property("row").toInt();

            _outMuteBtnList[i]->setChecked(!_cfgItemMixer->getOutGainMute(row).mute);
            _outGainList[i]->setValue(_cfgItemMixer->getOutGainMute(row).gain);

            _outLineEdit[i]->setText(_cfgItemMixer->getLabText(OUT, row));

            connect(_outMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
            connect(_outGainList[i], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
            connect(_outLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
        else {
            disconnect(_outMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
            disconnect(_outGainList[i], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
            disconnect(_outLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));

            int row = _outGainList[i]->property("row").toInt();

            _outMuteBtnList[i]->setChecked(!_cfgItemMixer->getOutGainMute(row).mute);
            _outGainList[i]->setValue(_cfgItemMixer->getOutGainMute(row).gain);

            _outLineEdit[i]->setText(_cfgItemMixer->getLabText(OUT, row));

            connect(_outMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
            connect(_outGainList[i], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
            connect(_outLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
    }
    for(int i = 0; i < _inGainList.count(); i++)
    {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
            disconnect(_inMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
            disconnect(_inGainList[i], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
            disconnect(_InLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));

            int col = _inGainList[i]->property("col").toInt();
//            qDebug()<<__FUNCTION__<<__LINE__<<col;
            _inMuteBtnList[i]->setChecked(!_cfgItemMixer->getInGainMute(col).mute);
            _inGainList[i]->setValue(_cfgItemMixer->getInGainMute(col).gain);
            _InLineEdit[i]->setText(_cfgItemMixer->getLabText(IN, col));

            connect(_inMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
            connect(_inGainList[i], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
            connect(_InLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
        else{
            disconnect(_inMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
            disconnect(_inGainList[i], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
            disconnect(_InLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));

            int col = _inGainList[i]->property("col").toInt();
//            qDebug()<<__FUNCTION__<<__LINE__<<col;
            _inMuteBtnList[i]->setChecked(!_cfgItemMixer->getInGainMute(col).mute);
            _inGainList[i]->setValue(_cfgItemMixer->getInGainMute(col).gain);
            _InLineEdit[i]->setText(_cfgItemMixer->getLabText(IN, col));

            connect(_inMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
            connect(_inGainList[i], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
            connect(_InLineEdit[i], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
    }
}

void CfgSettingMixer_Kemu::onOutMuteChanged()
{
    QPushButton *outMuteBtn = qobject_cast<QPushButton *>(sender());
    int row = outMuteBtn->property("row").toInt();
//    qDebug() << __FUNCTION__ << __LINE__ << " row: " << row << outMuteBtn->isChecked();
    _cfgItemMixer->setOutGainMute(row, _cfgItemMixer->getOutGainMute(row).gain, !outMuteBtn->isChecked());
}

void CfgSettingMixer_Kemu::onInMuteChanged()
{
    QPushButton *inMuteBtn = qobject_cast<QPushButton *>(sender());
    int col = inMuteBtn->property("col").toInt();
    _cfgItemMixer->setInGainMute(col, _cfgItemMixer->getInGainMute(col).gain, !inMuteBtn->isChecked());
}

QVBoxLayout* CfgSettingMixer_Kemu::createNode(int row, int col, double gain, bool mute)
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    GainLineEdit *gainEdit = NULL;
    if(row < 0 || col < 0)
        gainEdit = new GainLineEdit(gain, 12, -100);
    else
        gainEdit = new GainLineEdit(gain, 6, -100);
    gainEdit->setDecimalsNum(2);
    gainEdit->setFixedSize(FixW-2, FixH);
    gainEdit->setProperty("row", row);
    gainEdit->setProperty("col", col);
    layout->addWidget(gainEdit);

    QPushButton *muteBtn = new QPushButton();
    muteBtn->setCheckable(true);
    muteBtn->setFixedSize(FixW, FixH);
    muteBtn->setProperty("row", row);
    muteBtn->setProperty("col", col);
    muteBtn->setProperty("form", "Mute");
    muteBtn->setChecked(mute);
    layout->addWidget(muteBtn);

    if(col < 0){
        connect(gainEdit, SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
        connect(muteBtn, SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
        _outGainList.append(gainEdit);
        _outMuteBtnList.append(muteBtn);
    }
    else if(row < 0){
        connect(gainEdit, SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
        connect(muteBtn, SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
        _inGainList.append(gainEdit);
        _inMuteBtnList.append(muteBtn);
    }
    else{
        connect(gainEdit, SIGNAL(valueChanged(double)), this, SLOT(onRouterGainChanged(double)));
        connect(muteBtn, SIGNAL(clicked()), this, SLOT(onRouterMuteChanged()));
        _mixGainList.append(gainEdit);
        _mixMuteBtnList.append(muteBtn);
    }
    return layout;
}

void CfgSettingMixer_Kemu::setupUi()
{
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(11, 6, 11, 11);
    mainLayout->setVerticalSpacing(2);
    mainLayout->setHorizontalSpacing(2);
    addlabPage(mainLayout);
    addLabelAreaIn(mainLayout);
    addLabelAreaOut(mainLayout);

    QGroupBox *mixGroup = new QGroupBox();
    QGroupBox *inGroup = new QGroupBox();
    QGroupBox *outGroup = new QGroupBox();
    inGroup->setProperty("form", "left");
    outGroup->setProperty("form", "left");
    QGridLayout *mixLayout = new QGridLayout();
    mixLayout->setContentsMargins(8,5,8,5);
    QHBoxLayout *inLayout  = new QHBoxLayout();
    inLayout->setContentsMargins(8,5,8,5);
    QVBoxLayout *outLayout = new QVBoxLayout();
    outLayout->setContentsMargins(8,5,8,5);

    int spt_r = 0, spt_c = 0;
    for(int row = 0; row < _rowNum; row++) {
        if(row >= PageRows){ break; }
        spt_c = 0;
        for(int col = 0; col < _colNum; col++) {
            if(col >= PageCols) { break; }

            QVBoxLayout* mixItem = createNode(row, col, _cfgItemMixer->getRouterGain(row, col),
                                              _cfgItemMixer->getRouterMute(row, col));
            mixLayout->addItem(mixItem, row + spt_r, col + spt_c, 1, 1, Qt::AlignCenter);
//            int flag = col + 1;
//            if(!(flag % 4) && flag != _colNum && flag < ColMax) {
//                spt_c++;
//                QLabel *lab = new QLabel(""); lab->setFixedSize(5, 5);
//                mixLayout->addWidget(lab, row + spt_r, col + spt_c, 1, 1, Qt::AlignCenter);
//            }
        }

        GainMute gainMute = _cfgItemMixer->getOutGainMute(row);
        int col = 0;
        if(_colNum >= PageCols){
            col = PageCols;
        }else {
            col = _colNum;
        }

        QVBoxLayout* outItem =createNode(row, -1, gainMute.gain, !gainMute.mute);
        outLayout->addItem(outItem);

//        int flag = row + 1;
//        if(!(flag % 4) && flag != _rowNum && flag < 16) {
//            spt_r++;
//            QLabel *lab = new QLabel(" "); lab->setFixedSize(5, 5);
//            QLabel *lab1 = new QLabel(" "); lab1->setFixedSize(5, 5);
//            mixLayout->addWidget(lab, row + spt_r, col);
//            outLayout->addWidget(lab1);
//        }
    }

    for(int col = 0; col< _colNum; col++)
    {
        if(col >= PageCols) break;
        GainMute gainMute = _cfgItemMixer->getInGainMute(col);
        QVBoxLayout* inItem =createNode(-1, col, gainMute.gain, !gainMute.mute);
        inLayout->addItem(inItem);

//        int flag = col + 1;
//        if(!(flag % 4) && flag != _colNum && flag < ColMax)
//            inLayout->addWidget(new QLabel(""), Qt::AlignCenter);
    }

    mixGroup->setLayout(mixLayout);
    inGroup->setLayout(inLayout);
    outGroup->setLayout(outLayout);

    mainLayout->addWidget(mixGroup, _rowPosetionHead, _colPosetionHead, _rowNum, _colNum/*, Qt::AlignCenter*/);
    mainLayout->addWidget(outGroup, _rowPosetionHead, _colPosetionHead+_colNum, _rowNum, 1/*, Qt::AlignCenter*/);
    mainLayout->addWidget(inGroup, _rowPosetionHead+_rowNum, _colPosetionHead, 1, _colNum/*, Qt::AlignCenter*/);

    QLabel *labIn = new QLabel(tr("Input"));  labIn->setAlignment(Qt::AlignCenter);
    QLabel *labOut = new QLabel(tr("Output")); labOut->setAlignment(Qt::AlignCenter);

    QVBoxLayout *inGainMute = new QVBoxLayout();
    inGainMute->addWidget(labIn, Qt::AlignCenter);

    QHBoxLayout *outGainMute = new QHBoxLayout();
    outGainMute->addWidget(labOut, Qt::AlignCenter);

    mainLayout->addLayout(inGainMute, _rowPosetionHead+_rowNum, _colPosetionHead-1, 1, 1, Qt::AlignBottom);
    mainLayout->addLayout(outGainMute, _rowPosetionHead-1, _colPosetionHead+_colNum, 1, 1, Qt::AlignBottom);

    initDisplayRange();
}


void CfgSettingMixer_Kemu::onRouterMuteChanged()
{
    QPushButton *muteBtn = qobject_cast<QPushButton *>(sender());
    int row = muteBtn->property("row").toInt();
    int col = muteBtn->property("col").toInt();
    _cfgItemMixer->setRouterInfo(row, col, muteBtn->isChecked());
//    qDebug() << __FUNCTION__ << __LINE__  << " row: " << row
//             << " col: " << col << " isChecked: " << muteBtn->isChecked();
}

void CfgSettingMixer_Kemu::onRouterGainChanged(double value)
{
    int row = sender()->property("row").toInt();
    int col = sender()->property("col").toInt();
    //    qDebug() << __FUNCTION__ << __LINE__ << " value: " << value << " row: " << row << " col: " << col;
    _cfgItemMixer->setRouterGain(row, col, value);
}


void CfgSettingMixer_Kemu::onInGainValueChanged(double value)
{
    int col = sender()->property("col").toInt();
    _cfgItemMixer->setInGainMute(col, value, _cfgItemMixer->getInGainMute(col).mute);
    //    qDebug()<<__FUNCTION__<<__LINE__<< " col: " << col << "value: " << value ;
}


void CfgSettingMixer_Kemu::onOutGainValueChanged(double value)
{
    int row = sender()->property("row").toInt();
    //    qDebug() << __FUNCTION__ << __LINE__ << " row: " << row << value;
    _cfgItemMixer->setOutGainMute(row, value, _cfgItemMixer->getOutGainMute(row).mute);
}

void CfgSettingMixer_Kemu::onLabTextChanged()
{
    QTextEdit *lab = qobject_cast<QTextEdit *>(sender());
    int row = lab->property("row").toInt();
    int col = lab->property("col").toInt();
    //     qDebug()<<__FUNCTION__<<__LINE__<< " row: " << row << " col: " << col << lab->document()->toPlainText();
    _cfgItemMixer->setLabText(row, col, lab->document()->toPlainText());
}

void CfgSettingMixer_Kemu::onInLabelPageChanged()
{
    int colPage = sender()->property("page").toInt();
    if(colPage == -1) {//上一页
        if(_curColPage == 0) return;
        _curColPage --;
    }
    else if(colPage == -2) {//下一页
        if(_curColPage == _colNum/PageCols-1) return;
        _curColPage ++;
    }
    else{
        _curColPage = colPage;
    }

    initDisplayRange();
}

void CfgSettingMixer_Kemu::onOutLabelPageChanged()
{
    int rowPage = sender()->property("page").toInt();
    if(rowPage == -1) {//上一页
        if(_curRowPage == 0) return;
        _curRowPage --;
    }
    else if(rowPage == -2) {//下一页
        if(_curRowPage == _colNum/PageCols-1) return;
        _curRowPage ++;
    }
    else{
        if(_curRowPage == rowPage) return;
        else _curRowPage = rowPage;
    }

    initDisplayRange();
}

void CfgSettingMixer_Kemu::routerInfoChanged(int row, int col, bool status)
{
    _cfgItemMixer->setRouterInfo(row, col, status);
}

void CfgSettingMixer_Kemu::addLabelAreaIn(QGridLayout *mainLayout)
{
    int inLabRow = -2;
    QHBoxLayout *inLabLayout = new QHBoxLayout;
    inLabLayout->setContentsMargins(11, 0, 11, 0);
    inLabLayout->setSpacing(4);
    QGroupBox *nameInLabBox = new QGroupBox;
    QHBoxLayout *nameInLabLayout = new QHBoxLayout(nameInLabBox);
    nameInLabLayout->setContentsMargins(8,5,8,5);
    nameInLabBox->setProperty("form", "left");

    QVBoxLayout *inAndOutLab = new QVBoxLayout();
    inAndOutLab->setSpacing(11);
    QLabel *labIn = new QLabel(tr("Input"));  labIn->setAlignment(Qt::AlignRight);
    QLabel *labOut = new QLabel(tr("Output"));labOut->setAlignment(Qt::AlignCenter);
    inAndOutLab->addWidget(labIn);
    inAndOutLab->addWidget(labOut);
    mainLayout->addLayout(inAndOutLab, inLabRow+_rowPosetionHead+1, _colPosetionHead-1, 1, 1, Qt::AlignBottom);
    int colNum = PageCols;
    if(colNum > _colNum) colNum = _colNum;
    for(int col = 0; col < colNum; col++)
    {
        QLabel *inLabel = new QLabel(this);
        inLabel->setAlignment(Qt::AlignCenter);
        inLabel->setText(QString::number(col + 1));
        inLabel->setFixedSize(FixW, FixH*2);
        inLabLayout->addWidget(inLabel, Qt::AlignCenter);
        inLabLayout->setContentsMargins(0,0,0,0);
        inLabel->setText(tr("In%1").arg(col + 1));
        mainLayout->addWidget(inLabel, inLabRow+_rowPosetionHead, col+_colPosetionHead, 1, 1, Qt::AlignCenter);
        inLabel->setHidden(true);
        _InLabelList.append(inLabel);

        QTextEdit *nameInLab = new QTextEdit(this);
        nameInLab->setFixedSize(FixW, FixW);
        nameInLab->setFocusPolicy(Qt::ClickFocus);
        nameInLab->setText(_cfgItemMixer->getLabText(IN, col));
        nameInLab->setAlignment(Qt::AlignCenter);
        nameInLab->setProperty("row", 0);
        nameInLab->setProperty("col", col);
        connect(nameInLab, SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        nameInLabLayout->addWidget(nameInLab, Qt::AlignCenter);

//        int flag = col + 1;
//        if(!(flag % 4) && flag != _colNum && flag < 32) {
//            QLabel *lab = new QLabel(""); lab->setFixedSize(5, 5);
//            inLabLayout->addWidget(new QLabel(""), Qt::AlignCenter);
//            nameInLabLayout->addWidget(lab, Qt::AlignCenter);
//        }
        _InLineEdit.append(nameInLab);
    }
    mainLayout->addLayout(inLabLayout, inLabRow+_rowPosetionHead, _colPosetionHead, 1, _colNum, Qt::AlignCenter);
    mainLayout->addWidget(nameInLabBox, inLabRow+_rowPosetionHead+1, _colPosetionHead, 1, _colNum, Qt::AlignCenter);
}

void CfgSettingMixer_Kemu::addLabelAreaOut(QGridLayout *mainLayout)
{
    int OutLabCol = -2;
    QVBoxLayout *outLabLayout = new QVBoxLayout;
    outLabLayout->setContentsMargins(0, 6, 0, 6);
    outLabLayout->setSpacing(4);

    QGroupBox *nameOutLabBox = new QGroupBox;
    QVBoxLayout *nameOutLabLayout = new QVBoxLayout(nameOutLabBox);
    nameOutLabLayout->setContentsMargins(8,5,8,5);
    nameOutLabBox->setProperty("form", "left");

    int rowNum = PageRows;
    if(rowNum > _rowNum) rowNum = _rowNum;
    for(int row = 0; row < rowNum; row++)
    {
        QLabel *outLabel = new QLabel(this);
        outLabel->setAlignment(Qt::AlignCenter);
        outLabel->setText(QString::number(row+1));
        outLabel->setFixedSize(50, FixH*2 +2);
        outLabLayout->addWidget(outLabel, Qt::AlignCenter);
        outLabel->setText(tr("Out%1").arg(row+1));
        mainLayout->addWidget(outLabel, row+_rowPosetionHead, OutLabCol+_colPosetionHead, 1, 1, Qt::AlignCenter);
        outLabel->setHidden(true);
        _outLabelList.append(outLabel);

        QTextEdit *nameOutLab = new QTextEdit(this);
        nameOutLab->setFixedSize(50, FixH*2 +2);
        nameOutLab->setText(_cfgItemMixer->getLabText(OUT, row));
        nameOutLab->setAlignment(Qt::AlignCenter);
        nameOutLab->setProperty("row", 1);
        nameOutLab->setProperty("col", row);
        connect(nameOutLab, SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        nameOutLabLayout->addWidget(nameOutLab, Qt::AlignCenter);

//        int flag = row + 1;
//        if(!(flag % 4) && flag != _rowNum && flag < 16) {
//            QLabel *lab = new QLabel(" "); lab->setFixedSize(7, 7);
//            QLabel *lab1 = new QLabel(" "); lab->setFixedSize(7, 7);
//            outLabLayout->addWidget(lab1, Qt::AlignCenter);
//            nameOutLabLayout->addWidget(lab, Qt::AlignCenter);
//        }
        _outLineEdit.append(nameOutLab);
    }
    mainLayout->addLayout(outLabLayout, _rowPosetionHead, OutLabCol+_colPosetionHead, _rowNum, 1, Qt::AlignCenter);
    mainLayout->addWidget(nameOutLabBox, _rowPosetionHead, OutLabCol+_colPosetionHead+1, _rowNum, 1/*, Qt::AlignCenter*/);
}

void CfgSettingMixer_Kemu::addlabPage(QGridLayout *mainLayout)//分页标签
{
    if(_colNum / PageCols > 1)
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
        prevBtn->setProperty("page", -1);
        prevBtn->setFocusPolicy(Qt::NoFocus);
        connect(prevBtn, SIGNAL(clicked()), this, SLOT(onInLabelPageChanged()));
        mainLayout->addWidget(prevBtn, 0, 4, 1, 1, Qt::AlignCenter);

        for(int colPage = 0; colPage < _colNum / PageCols; colPage++)
        {
            int label = colPage * PageCols;
            QPushButton *btn = new QPushButton(this);
            btn->setCheckable(true);
            btn->setFixedSize(36, 18);
            btn->setText(QString("%1~%2").arg(label+1).arg(label+PageCols));
            btn->setProperty("page", colPage);
            btn->setFocusPolicy(Qt::NoFocus);
            connect(btn, SIGNAL(clicked()), this, SLOT(onInLabelPageChanged()));
            mainLayout->addWidget(btn, 0, 5+colPage, 1, 1, Qt::AlignCenter);
            _colPageBtnList.append(btn);
        }

        QPushButton *nextBtn = new QPushButton(this);
        nextBtn->setCheckable(false);
        nextBtn->setFixedSize(36, 18);
        nextBtn->setText("Next");
        nextBtn->setProperty("page", -2);
        nextBtn->setFocusPolicy(Qt::NoFocus);
        connect(nextBtn, SIGNAL(clicked()), this, SLOT(onInLabelPageChanged()));
        mainLayout->addWidget(nextBtn, 0, (5+_colNum / PageCols), 1, 1, Qt::AlignCenter);
    }
    if(_rowNum / PageRows > 1)
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
        prevBtn->setProperty("page", -1);
        prevBtn->setFocusPolicy(Qt::NoFocus);
        connect(prevBtn, SIGNAL(clicked()), this, SLOT(onOutLabelPageChanged()));
        mainLayout->addWidget(prevBtn, 4, 0, 1, 1, Qt::AlignCenter);

        for(int rowPage = 0; rowPage < _rowNum / PageRows; rowPage++)
        {
            int label = rowPage * PageRows;
            QPushButton *Btn = new QPushButton(this);
            Btn->setCheckable(true);
            Btn->setFixedSize(36, 18);
            Btn->setText(QString("%1~%2").arg(label+1).arg(label+PageRows));
            Btn->setProperty("page", rowPage);
            Btn->setFocusPolicy(Qt::NoFocus);
            connect(Btn, SIGNAL(clicked()), this, SLOT(onOutLabelPageChanged()));
            mainLayout->addWidget(Btn, 5+rowPage, 0, 1, 1, Qt::AlignCenter);
            _rowPageBtnList.append(Btn);
        }

        QPushButton *nextBtn = new QPushButton(this);
        nextBtn->setCheckable(false);
        nextBtn->setFixedSize(36, 18);
        nextBtn->setText("Next");
        nextBtn->setProperty("page", -2);
        nextBtn->setFocusPolicy(Qt::NoFocus);
        connect(nextBtn, SIGNAL(clicked()), this, SLOT(onOutLabelPageChanged()));
        mainLayout->addWidget(nextBtn, (5+_rowNum / PageRows), 0, 1, 1, Qt::AlignCenter);
    }
}

void CfgSettingMixer_Kemu::updateNodeData()
{
    int i = 0;
    int minRow = _curRowPage* PageRows;
    int maxRow = minRow + PageRows;
    if(maxRow > _rowNum) maxRow = _rowNum;
    int minCol = _curColPage* PageCols;
    int maxCol = minCol + PageCols;
    if(maxCol > _colNum) maxCol = _colNum;
    for(int row = minRow; row < maxRow; row++)
    {
        for(int col = minCol; col < maxCol; col++)
        {
            disconnect(_mixMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onRouterMuteChanged()));
            disconnect(_mixGainList[i], SIGNAL(valueChanged(double)), this, SLOT(onRouterGainChanged(double)));

            _mixGainList[i]->setProperty("row", row);
            _mixGainList[i]->setProperty("col", col);
            _mixGainList[i]->setValue(_cfgItemMixer->getRouterGain(row, col));
            _mixMuteBtnList[i]->setProperty("row", row);
            _mixMuteBtnList[i]->setProperty("col", col);
            _mixMuteBtnList[i]->setChecked(_cfgItemMixer->getRouterMute(row, col));
            connect(_mixMuteBtnList[i], SIGNAL(clicked()), this, SLOT(onRouterMuteChanged()));
            connect(_mixGainList[i], SIGNAL(valueChanged(double)), this, SLOT(onRouterGainChanged(double)));
            i++;
        }
    }

    for(int row = minRow; row < maxRow; row++)
    {
        disconnect(_outMuteBtnList[row-minRow], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
        disconnect(_outGainList[row-minRow], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));

        _outMuteBtnList[row-minRow]->setProperty("row", row);
        _outGainList[row-minRow]->setProperty("row", row);
        _outMuteBtnList[row-minRow]->setChecked(!_cfgItemMixer->getOutGainMute(row).mute);
        _outGainList[row-minRow]->setValue(_cfgItemMixer->getOutGainMute(row).gain);
        connect(_outMuteBtnList[row-minRow], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
        connect(_outGainList[row-minRow], SIGNAL(valueChanged(double)), this, SLOT(onOutGainValueChanged(double)));
}

    for(int col = minCol; col < maxCol; col++)
    {
//        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
        disconnect(_inMuteBtnList[col-minCol], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
        disconnect(_inGainList[col-minCol], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));

        _inMuteBtnList[col-minCol]->setProperty("col", col);
        _inGainList[col-minCol]->setProperty("col", col);
        _inMuteBtnList[col-minCol]->setChecked(!_cfgItemMixer->getInGainMute(col).mute);
        _inGainList[col-minCol]->setValue(_cfgItemMixer->getInGainMute(col).gain);
        connect(_inMuteBtnList[col-minCol], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
        connect(_inGainList[col-minCol], SIGNAL(valueChanged(double)), this, SLOT(onInGainValueChanged(double)));
    }

}

void CfgSettingMixer_Kemu::initDisplayRange()
{
    if(!_colPageBtnList.isEmpty())
    {
        for(int i = 0; i <_colPageBtnList.length(); i++)  {
            _colPageBtnList[i]->setChecked(i == _curColPage);
        }
        for(int c = 0; c < PageCols; c++)
        {
            int col = _curColPage * PageCols + c;
            disconnect(_InLineEdit[c], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            _InLabelList[c]->setText(QString::number(col+1));
            _InLineEdit[c]->setText(_cfgItemMixer->getLabText(IN, col));
            _InLineEdit[c]->setProperty("col", col);
            connect(_InLineEdit[c], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
    }


    if(!_rowPageBtnList.isEmpty())
    {
        for(int i = 0; i <_rowPageBtnList.length(); i++)  {
            _rowPageBtnList[i]->setChecked(i == _curRowPage);
        }
        for(int r = 0; r < PageRows; r++)
        {
            int row = _curRowPage * PageRows + r;
            disconnect(_outLineEdit[r], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            _outLabelList[r]->setText(QString::number(row+1));
            _outLineEdit[r]->setText(_cfgItemMixer->getLabText(OUT, row));
            _outLineEdit[r]->setProperty("col", row);
            connect(_outLineEdit[r], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
    }

    updateNodeData();
}


