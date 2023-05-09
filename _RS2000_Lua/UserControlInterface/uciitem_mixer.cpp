#include "uciitem_mixer.h"
#include "customWidget/configgainwidget.h"
#include <QTextEdit>
#include "Frame/absgraphscene.h"
#include "Lua/Packages/centerctrl.h"
#include "usercontrolinterface.h"
#include "configItem/configitemuci.h"

UciItem_mixer::UciItem_mixer(QString devName, bool nodesSwitch, CfgType type, ConfigItemUCI *uciItem, QGraphicsView *view)
    : QGraphicsScene(view), _cfgType(type),_uciItem(uciItem), _devName(devName), _nodeSwitch(nodesSwitch)
{
    _colPosetionHead = 2;
    _rowPosetionHead = 2;

    switch(_cfgType) {
    case CFG_TYPE_MIXER_4X1:
        _colNum = 4; _rowNum = 1;
        break;
    case CFG_TYPE_MIXER_4X2:
        _colNum = 4; _rowNum = 2;
        break;
    case CFG_TYPE_MIXER_4X4:
        _colNum = 4; _rowNum = 4;
        break;
    case CFG_TYPE_MIXER_8X1:
        _colNum = 8; _rowNum = 1;
        break;
    case CFG_TYPE_MIXER_8X2:
        _colNum = 8; _rowNum = 2;
        break;
    case CFG_TYPE_MIXER_8X4:
        _colNum = 8; _rowNum = 4;
        break;
    case CFG_TYPE_MIXER_8X8:
        _colNum = 8; _rowNum = 8;
        break;
    case CFG_TYPE_MIXER_16X16:
        _colNum = 16; _rowNum = 16;
        break;
    case CFG_TYPE_MIXER_32X32:
        _colNum = 32; _rowNum = 32;
        break;
    case CFG_TYPE_MIXER_64X64:
        _colNum = 64; _rowNum = 64;
        break;
    default:
        _colNum = 0; _rowNum = 0;
        break;
    }

    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 64; j++)
            _nodeLab[i][j] = QString::number(j+1);
    }
    initUi();
}

UciItem_mixer::~UciItem_mixer()
{
    _routerBtnList.clear();
    _InBtnList.clear();
    _OutBtnList.clear();
    _outMuteBtnList.clear();
    _inMuteBtnList.clear();
    _InLabelList.clear();
    _outLabelList.clear();
    _inLabBtnList.clear();
    _outLabBtnList.clear();
    delete _uciItem;
    delete _wid;
}

void UciItem_mixer::onInLabelPageChanged()
{
    if(sender()->property("text").toString() == "1~32")
    {
        for(int col = 0; col < 32; col++)
        {
            disconnect(_InLineEdit[col], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));

//            _InLabelList[col]->setText(QString::number(col+1));

            _InLineEdit[col]->setText(_nodeLab[IN][col]);
            _InLineEdit[col]->setAlignment(Qt::AlignCenter);
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

//            _InLabelList[col-32]->setText(QString::number(col+1));

            _InLineEdit[col-32]->setText(_nodeLab[IN][col]);
            _InLineEdit[col-32]->setAlignment(Qt::AlignCenter);
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
        initDisplayRange();
    }else if(sender()->property("text").toString() == "Next")
    {
        if(_currentMaxCol == _colNum)
        {
            return;
        }
        _currentMinCol += 32;
        _currentMaxCol += 32;

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
    }
}

void UciItem_mixer::onOutLabelPageChanged()
{
    if(sender()->property("text").toString() == "1~16")
    {
        for(int row = 0; row < 16; row++)
        {
            disconnect(_outLineEdit[row], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
            _outLabelList[row]->setText(QString::number(row+1));

            _outLineEdit[row]->setText(_nodeLab[OUT][row]);
            _outLineEdit[row]->setAlignment(Qt::AlignCenter);
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

            _outLineEdit[row-16]->setText(_nodeLab[OUT][row]);
            _outLineEdit[row-16]->setAlignment(Qt::AlignCenter);
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

            _outLineEdit[row-32]->setText(_nodeLab[OUT][row]);
            _outLineEdit[row-32]->setAlignment(Qt::AlignCenter);
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

            _outLineEdit[row-48]->setText(_nodeLab[OUT][row]);
            _outLineEdit[row-48]->setAlignment(Qt::AlignCenter);
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

        initDisplayRange();
    }else if(sender()->property("text").toString() == "Next")
    {
        if(_currentMaxRow == _rowNum)
        {
            return ;
        }
        _currentMinRow += 16;
        _currentMaxRow += 16;
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
    }
}

void UciItem_mixer::initUi()
{
    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->setContentsMargins(11, 0, 11, 11);
    mainLayout->setSpacing(1);

    _wid = new QWidget;
    _wid->setLayout(mainLayout);
    this->addWidget(_wid);

    addlabPage(mainLayout);
    addLabel(mainLayout, IN);
    addLabel(mainLayout, OUT);

    QGroupBox *btnBox = new QGroupBox();
    QGridLayout *BtnLayout = new QGridLayout();

    int spt_r = 0, spt_c = 0;
    for(int row = 0; row < _rowNum; row++) {
        if(row >= 16){ break; }
        spt_c = 0;
        for(int col = 0; col < _colNum; col++) {
            if(col >= 32) { break; }
//            qDebug() << __FUNCTION__ << __LINE__ << row << col;
            ConfigGainWidget *Btn = new ConfigGainWidget(-100, 6, -100,
                                                         ParamType_MixerGain);
            Btn->setFixedSize(50, 50);
            Btn->setProperty("row", row);
            Btn->setProperty("col", col);
            Btn->setMute(false);

            Btn->SetEnbleClicked(_nodeSwitch);
//            connect(Btn, SIGNAL(clicked()), this, SLOT(onRouterInfoChanged()));

            connect(Btn, SIGNAL(valueChanged(int)), this, SLOT(onRouterGainChanged(int)));
            BtnLayout->addWidget(Btn, row + spt_r, col + spt_c, 1, 1, Qt::AlignCenter);

            _routerBtnList.append(Btn);

            int flag = col + 1;
            if(!(flag % 4) && flag != _colNum && flag < 32) {
                spt_c++;
                QLabel *lab = new QLabel(/*"-"*/); lab->setFixedSize(5, 5);
                BtnLayout->addWidget(lab, row + spt_r, col + spt_c, 1, 1, Qt::AlignCenter);
            }
        }
        int flag = row + 1;
        if(!(flag % 4) && flag != _rowNum && flag < 16) {
            spt_r++;
            QLabel *lab = new QLabel(/*"+"*/); lab->setFixedSize(5, 5);
            BtnLayout->addWidget(lab, row + spt_r, 0);
        }
    }
    btnBox->setLayout(BtnLayout);

    mainLayout->addWidget(btnBox, _rowPosetionHead, _colPosetionHead, _rowNum, _colNum);
    _wid->setProperty("form", "uciMixer");
    _wid->setStyleSheet(SkinStyle::getCfgSettingStyleSheet());
//    initDisplayRange();
}

void UciItem_mixer::addlabPage(QGridLayout *mainLayout)//分页标签
{
    if(_colNum / 32 > 1)
    {
        _rowPosetionHead = 3;

        QLabel *label = new QLabel();
        label->setAlignment(Qt::AlignCenter);
        label->setText(tr("Input"));
        label->setHidden(true);

        mainLayout->addWidget(label, 0, 3, 1, 1, Qt::AlignCenter);

        QPushButton *prevBtn = new QPushButton();
        prevBtn->setCheckable(false);
        prevBtn->setFixedSize(36, 18);
        prevBtn->setText(tr("Prev"));
        prevBtn->setProperty("text", "Prev");
        //prevBtn->setProperty("col", col);
        prevBtn->setFocusPolicy(Qt::NoFocus);

        connect(prevBtn, SIGNAL(clicked()), this, SLOT(onInLabelPageChanged()));
        mainLayout->addWidget(prevBtn, 0, 4, 1, 1, Qt::AlignCenter);
        //_inLabBtnList.append(prevBtn);

        for(int col = 0; col < _colNum / 32; col++)
        {
            int label = col * 32;
            QPushButton *btn = new QPushButton();
            btn->setCheckable(true);
            btn->setFixedSize(36, 18);
            btn->setText(QString("%1~%2").arg(label+1).arg(label+32));
            btn->setProperty("text", QString("%1~%2").arg(label+1).arg(label+32));
            //nextBtn->setProperty("col", col);
            btn->setFocusPolicy(Qt::NoFocus);

            connect(btn, SIGNAL(clicked()), this, SLOT(onInLabelPageChanged()));
            mainLayout->addWidget(btn, 0, 5+col, 1, 1, Qt::AlignCenter);
            _inLabBtnList.append(btn);
        }

        QPushButton *nextBtn = new QPushButton();
        nextBtn->setCheckable(false);
        nextBtn->setFixedSize(36, 18);
        nextBtn->setText(tr("Next"));
        nextBtn->setProperty("text", "Next");
        //nextBtn->setProperty("col", col);
        nextBtn->setFocusPolicy(Qt::NoFocus);

        connect(nextBtn, SIGNAL(clicked()), this, SLOT(onInLabelPageChanged()));
        mainLayout->addWidget(nextBtn, 0, (5+_colNum / 32), 1, 1, Qt::AlignCenter);
        //_inLabBtnList.append(nextBtn);
    }
    if(_rowNum / 16 > 1)
    {
        _colPosetionHead = 3;

        QLabel *label = new QLabel();
        label->setAlignment(Qt::AlignCenter);
        label->setText(tr("Output"));
#if Logo_FangTu
        label->setHidden(true);
#endif
        mainLayout->addWidget(label, 3, 0, 1, 1, Qt::AlignCenter);

        QPushButton *prevBtn = new QPushButton();
        prevBtn->setCheckable(false);
        prevBtn->setFixedSize(36, 18);
        prevBtn->setText(tr("Prev"));
        prevBtn->setProperty("text", "Prev");
        //prevBtn->setProperty("col", col);
        prevBtn->setFocusPolicy(Qt::NoFocus);

        connect(prevBtn, SIGNAL(clicked()), this, SLOT(onOutLabelPageChanged()));
        mainLayout->addWidget(prevBtn, 4, 0, 1, 1, Qt::AlignCenter);
        //_outLabBtnList.append(prevBtn);

        for(int row = 0; row < _rowNum / 16; row++)
        {
            int label = row * 16;
            QPushButton *Btn = new QPushButton();
            Btn->setCheckable(true);
            Btn->setFixedSize(36, 18);
            Btn->setText(QString("%1~%2").arg(label+1).arg(label+16));
            Btn->setProperty("text", QString("%1~%2").arg(label+1).arg(label+16));
            //Btn->setProperty("col", col);
            Btn->setFocusPolicy(Qt::NoFocus);

            connect(Btn, SIGNAL(clicked()), this, SLOT(onOutLabelPageChanged()));
            mainLayout->addWidget(Btn, 5+row, 0, 1, 1, Qt::AlignCenter);
            _outLabBtnList.append(Btn);
        }

        QPushButton *nextBtn = new QPushButton();
        nextBtn->setCheckable(false);
        nextBtn->setFixedSize(36, 18);
        nextBtn->setText(tr("Next"));
        nextBtn->setProperty("text", "Next");
        //nextBtn->setProperty("col", col);
        nextBtn->setFocusPolicy(Qt::NoFocus);

        connect(nextBtn, SIGNAL(clicked()), this, SLOT(onOutLabelPageChanged()));
        mainLayout->addWidget(nextBtn, (5+_rowNum / 16), 0, 1, 1, Qt::AlignCenter);
        //_outLabBtnList.append(nextBtn);
    }
}

void UciItem_mixer::addLabel(QGridLayout *mainLayout, labType type)//输入输出label
{
    int inLabRow = -2, OutLabCol = -2;
    QHBoxLayout *inLabLayout = new QHBoxLayout;
    QVBoxLayout *outLabLayout = new QVBoxLayout;

    inLabLayout->setContentsMargins(11, 0, 11, 0);
    outLabLayout->setContentsMargins(0, 11, 0, 11);
    inLabLayout->setSpacing(6);
    outLabLayout->setSpacing(6);
    QGroupBox *nameInLabBox = new QGroupBox;
    QGroupBox *nameOutLabBox = new QGroupBox;
    QHBoxLayout *nameInLabLayout = new QHBoxLayout;
    QVBoxLayout *nameOutLabLayout = new QVBoxLayout;

//    nameInLabBox->setTitle("Input");
//    nameOutLabBox->setTitle("Output");
    nameInLabBox->setProperty("form", "left");
    nameOutLabBox->setProperty("form", "left");

    QVBoxLayout *inAndOutLab = new QVBoxLayout();
    inAndOutLab->setSpacing(11);
    QLabel *labIn = new QLabel(tr("Input"));  labIn->setAlignment(Qt::AlignRight);
    QLabel *labOut = new QLabel(tr("Output"));labOut->setAlignment(Qt::AlignCenter);
    inAndOutLab->addWidget(labIn);
    inAndOutLab->addWidget(labOut);
    mainLayout->addLayout(inAndOutLab, inLabRow+_rowPosetionHead+1, _colPosetionHead-1, 1, 1, Qt::AlignBottom);

//    nameInLabBox->setFixedHeight(94);
    if(type == IN)
    {
        for(int col = 0; col < _colNum; col++)
        {
            if(col >= 32) { break; }
            QLabel *inLabel = new QLabel();
            inLabel->setAlignment(Qt::AlignCenter);
            inLabel->setText(QString::number(col + 1));
            inLabel->setFixedWidth(50);
            inLabLayout->addWidget(inLabel, Qt::AlignCenter);
//            inLabel->setHidden(true);
            inLabLayout->setContentsMargins(0,0,0,0);
            inLabel->setText(tr("In %1").arg(col + 1));
            mainLayout->addWidget(inLabel, inLabRow+_rowPosetionHead, col+_colPosetionHead, 1, 1, Qt::AlignCenter);

            inLabel->setHidden(true);
            _InLabelList.append(inLabel);

            QTextEdit *nameInLab = new QTextEdit();

            nameInLab->setFixedSize(50, 50);
            nameInLab->setFocusPolicy(Qt::ClickFocus);
            nameInLab->verticalScrollBar()->hide();
            nameInLab->horizontalScrollBar()->hide();

            nameInLab->setText(_nodeLab[IN][col]);
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
    nameInLabBox->setLayout(nameInLabLayout);
    mainLayout->addWidget(nameInLabBox, inLabRow+_rowPosetionHead+1, _colPosetionHead, 1, _colNum, Qt::AlignCenter);
    }else if(type == OUT){
        for(int row = 0; row < _rowNum; row++)
        {
            if(row >= 16) { break; }
            QLabel *outLabel = new QLabel();
            outLabel->setAlignment(Qt::AlignCenter);
            outLabel->setText(QString::number(row+1));
            outLabel->setFixedHeight(50);
            outLabLayout->addWidget(outLabel, Qt::AlignCenter);
//            outLabel->setHidden(true);
            outLabel->setText(tr("Out %1").arg(row+1));
            mainLayout->addWidget(outLabel, row+_rowPosetionHead, OutLabCol+_colPosetionHead, 1, 1, Qt::AlignCenter);
            outLabel->setHidden(true);

            _outLabelList.append(outLabel);

            QTextEdit *nameOutLab = new QTextEdit();
            nameOutLab->setFixedSize(50, 50);
            nameOutLab->verticalScrollBar()->setVisible(false);
            nameOutLab->horizontalScrollBar()->setVisible(false);

//            nameOutLab->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//            nameOutLab->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            nameOutLab->setText(_nodeLab[OUT][row]);
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
        nameOutLabBox->setLayout(nameOutLabLayout);
        mainLayout->addWidget(nameOutLabBox, _rowPosetionHead, OutLabCol+_colPosetionHead+1, _rowNum, 1/*, Qt::AlignCenter*/);
    }
}

void UciItem_mixer::initDisplayRange()
{

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

//            _InLabelList[col-_currentMinCol]->setText(QString::number(col+1));
            _InLineEdit[col-_currentMinCol]->setText(_nodeLab[IN][col]);
            _InLineEdit[col-_currentMinCol]->setAlignment(Qt::AlignCenter);
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

            _outLineEdit[row-_currentMinRow]->setText(_nodeLab[OUT][row]);
            _outLineEdit[row-_currentMinRow]->setAlignment(Qt::AlignCenter);
            _outLineEdit[row-_currentMinRow]->setProperty("col", row);
            connect(_outLineEdit[row-_currentMinRow], SIGNAL(textChanged()), this, SLOT(onLabTextChanged()));
        }
    }

    updateNodeData(_currentMinRow, _currentMaxRow, _currentMinCol, _currentMaxCol);
}

void UciItem_mixer::onRouterInfoChanged()
{
    ConfigGainWidget *btn = qobject_cast<ConfigGainWidget *>(sender());
    if(!btn) return ;
    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();
    CenterCtrl::instance()->setMixerNodeState(_devName, col, row, btn->isMute());
}

void UciItem_mixer::onRouterGainChanged(int value)
{
    ConfigGainWidget *btn = qobject_cast<ConfigGainWidget *>(sender());
    if(!btn) return ;
    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();
    /*qDebug() << __FUNCTION__ << __LINE__ << row << col <<*/ CenterCtrl::instance()->setMixerNodeGain(_devName, col, row, value);
}

void UciItem_mixer::onLabTextChanged()
{
    QTextEdit *edit = qobject_cast<QTextEdit *>(sender());
    if(!edit) return;
    int row = edit->property("row").toInt();
    int col = edit->property("col").toInt();
    _nodeLab[row][col] = edit->document()->toPlainText();
}

void UciItem_mixer::initData()
{
    foreach(ConfigGainWidget *item, _routerBtnList) {
        int row = item->property("row").toInt();
        int col = item->property("col").toInt();
        double gain = 0; /*bool mute = false;*/
        CenterCtrl::instance()->getMixerNodeGain(_devName, col, row, gain);
//        CenterCtrl::instance()->getMixerNodeState(_devName, col, row, mute);
        item->setValue(gain);
//        item->setChecked(mute);
    }
}

void UciItem_mixer::updateNodeData(int minRow, int maxRow, int minCol, int maxCol)
{
//    qDebug() << __FUNCTION__ << __LINE__ << " minRow: " << minRow << " maxRow: " << maxRow << " minCol: " << minCol << " maxCol: " << maxCol;
    int i = 0;
    for(int row = minRow; row < maxRow; row++)
    {
        for(int col = minCol; col < maxCol; col++)
        {
            disconnect(_routerBtnList[i], SIGNAL(clicked()), this, SLOT(onRouterInfoChanged()));
            disconnect(_routerBtnList[i], SIGNAL(valueChanged(int)), this, SLOT(onRouterGainChanged(int)));

            _routerBtnList[i]->setProperty("row", row);
            _routerBtnList[i]->setProperty("col", col);
            bool mute = false; double gain = 0;
            CenterCtrl::instance()->getMixerNodeState(_devName, col, row, mute);
            CenterCtrl::instance()->getMixerNodeGain(_devName, col, row, gain);
            _routerBtnList[i]->setMute(mute);
            _routerBtnList[i]->setValue(gain);

            connect(_routerBtnList[i], SIGNAL(clicked()), this, SLOT(onRouterInfoChanged()));
            connect(_routerBtnList[i], SIGNAL(valueChanged(int)), this, SLOT(onRouterGainChanged(int)));
            i++;
        }
    }

//    for(int row = minRow; row < maxRow; row++)
//    {
//        disconnect(_outMuteBtnList[row-minRow], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
//        disconnect(_OutBtnList[row-minRow], SIGNAL(valueChanged(int)), this, SLOT(onOutGainValueChanged(int)));

//        _outMuteBtnList[row-minRow]->setProperty("row", row);
//        _OutBtnList[row-minRow]->setProperty("row", row);
//        _outMuteBtnList[row-minRow]->setChecked(!_cfgItemMixer->getOutGainMute(row).mute);
//        _OutBtnList[row-minRow]->setValue(_cfgItemMixer->getOutGainMute(row).gain);
//        connect(_outMuteBtnList[row-minRow], SIGNAL(clicked()), this, SLOT(onOutMuteChanged()));
//        connect(_OutBtnList[row-minRow], SIGNAL(valueChanged(int)), this, SLOT(onOutGainValueChanged(int)));
//    }

//    for(int col = minCol; col < maxCol; col++)
//    {
//        disconnect(_inMuteBtnList[col-minCol], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
//        disconnect(_InBtnList[col-minCol], SIGNAL(valueChanged(int)), this, SLOT(onInGainValueChanged(int)));

//        _inMuteBtnList[col-minCol]->setProperty("col", col);
//        _InBtnList[col-minCol]->setProperty("col", col);
//        _inMuteBtnList[col-minCol]->setChecked(!_cfgItemMixer->getInGainMute(col).mute);
//        _InBtnList[col-minCol]->setValue(_cfgItemMixer->getInGainMute(col).gain);
//        connect(_inMuteBtnList[col-minCol], SIGNAL(clicked()), this, SLOT(onInMuteChanged()));
//        connect(_InBtnList[col-minCol], SIGNAL(valueChanged(int)), this, SLOT(onInGainValueChanged(int)));
//    }

    update();
}



