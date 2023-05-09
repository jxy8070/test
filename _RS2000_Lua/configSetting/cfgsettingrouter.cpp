#include "stable.h"
#include "cfgsettingrouter.h"
#include "configItem/configitemrouter.h"

CfgSettingRouter::CfgSettingRouter(ConfigItemRouter *cfgItem, QWidget *parent)
    : QWidget(parent)
{
    CfgType cfgType = cfgItem->getCfgType();
    _cfgItemRouter = cfgItem;
    _rowNum = _colNum = cfgItem->getPortNum();

    //关联 cfgItem的参数，具体化cfgItem
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
        setupUi_FangTu();
    }
    else{
        setupUi();
    }
}

void CfgSettingRouter::updateUiData()
{
    for(int col = 0; col < _colNum; col++)
    {
        for(int row = 0; row < _btnGroupList.at(col)->buttons().count(); row++)
        {
            _btnGroupList.at(col)->buttons().at(row)->setChecked(routerChecked(row, col));
        }
        _btnLabelList.at(col)->setText(QString("%1").arg(_cfgItemRouter->getRouterInfo(col)+1));
    }
}

void CfgSettingRouter::setupUi()
{
    int pagePortNum = _pagePortNum;
    int pageCnt = (_colNum + _pagePortNum -1) / _pagePortNum;
    if(pageCnt == 1) pagePortNum = _colNum;
    QSize sizeRouteBtn(32,16);
    QSize sizePageBtn(54,24);
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(8,8,8,8);
    mainLayout->setSpacing(4);

    //行标题Output
    QGroupBox* groupOutput = new QGroupBox(this);
    QGridLayout* groupOutLayout = new QGridLayout(groupOutput);
    groupOutLayout->setContentsMargins(3,3,3,3);
    groupOutLayout->setSpacing(4);

    QLabel *lblGroupOut  = new QLabel(groupOutput);
    lblGroupOut->setText(tr("Output:"));
    lblGroupOut->setFixedSize(sizeRouteBtn.width()*2, sizeRouteBtn.height());
    lblGroupOut->setAlignment(Qt::AlignCenter);
    groupOutLayout->addWidget(lblGroupOut, 0, 0, 1, 2);

    if(pageCnt > 1){ //列范围选择
        QButtonGroup *colSelectBtnGroup = new QButtonGroup(this);
        for(int col = 0; col<pageCnt; col++)
        {
            QString btnText = QString("%1~%2").arg(_pagePortNum*col + 1, 3, 10, QChar('0'))
                    .arg(_pagePortNum*(col + 1), 3, 10, QChar('0'));
            QPushButton *btn = new QPushButton(this);
            btn->setText(btnText);
            btn->setFixedSize(sizePageBtn);
            btn->setCheckable(true);
            btn->setProperty("pageRowIdx", -1);
            btn->setProperty("pageColIdx", col);
            colSelectBtnGroup->addButton(btn);
            if(col == 0){ btn->setChecked(true); }
            connect(btn, SIGNAL(clicked()), this, SLOT(onPageSelected()));
            groupOutLayout->addWidget(btn, 0, 2 + col*2, 1, 2, Qt::AlignCenter);
        }
    }
    for(int col = 0; col <pagePortNum; ++col) {
        QLabel *lblHeader  = new QLabel(this);
        lblHeader->setText(tr("%1").arg(col+1, 2, 10, QChar('0')));
        lblHeader->setAlignment(Qt::AlignCenter);
        lblHeader->setFixedSize(sizeRouteBtn);
        groupOutLayout->addWidget(lblHeader, 1, col, Qt::AlignCenter);
        _outLabelList.append(lblHeader);

        //按钮行
        QLineEdit *btnLabel = new QLineEdit(QString("%1").arg(_cfgItemRouter->getRouterInfo(col)+1));
        btnLabel->setAlignment(Qt::AlignCenter);
        btnLabel->setFixedSize(sizeRouteBtn);
        btnLabel->setDisabled(true);
        groupOutLayout->addWidget(btnLabel, 2, col, Qt::AlignCenter);
        _btnLabelList.append(btnLabel);
    }
    mainLayout->addWidget(groupOutput, 0, 1, 1, 1, Qt::AlignCenter);

    //路由按钮矩阵
    QGroupBox* groupRoute = new QGroupBox(this);
    QGridLayout* groupRouteLayout = new QGridLayout(groupRoute);
    groupRouteLayout->setContentsMargins(3,3,3,3);
    groupRouteLayout->setSpacing(4);
    for(int col = 0; col <pagePortNum; ++col) {
        QButtonGroup *btnGroup = new QButtonGroup(this);
        btnGroup->setExclusive(true);
        for(int row = 0; row < pagePortNum; ++row) {
            QPushButton *btn = new QPushButton(this);
            btn->setCheckable(true);
            btn->setFixedSize(sizeRouteBtn);
            btnGroup->addButton(btn);
            connect(btn, SIGNAL(clicked(bool)), this, SLOT(onRouterChanged(bool)));
            groupRouteLayout->addWidget(btn, row, col, Qt::AlignCenter);
        }
        _btnGroupList.append(btnGroup);
    }
    mainLayout->addWidget(groupRoute, 1, 1, 1, 1, Qt::AlignCenter);

    //列标题Input
    QGroupBox* groupInput = new QGroupBox(this);
    QGridLayout* groupInLayout = new QGridLayout(groupInput);
    groupInLayout->setContentsMargins(3,3,0,3);
    groupInLayout->setSpacing(4);
    int colIdx = 0;
    if(pageCnt > 1){
        QLabel *lblGroupIn  = new QLabel(groupInput);
        lblGroupIn->setFixedSize(54,16);
        lblGroupIn->setText(tr("Input:"));
        lblGroupIn->setAlignment(Qt::AlignCenter);
        groupInLayout->addWidget(lblGroupIn, 0, colIdx, 1, 1, Qt::AlignCenter);

        QButtonGroup *rowSelectBtnGroup = new QButtonGroup(this);
        for(int row = 0; row<pageCnt; row++)
        {
            QString btnText = QString("%1~%2").arg(_pagePortNum*row + 1, 3, 10, QChar('0'))
                    .arg(_pagePortNum*(row + 1), 3, 10, QChar('0'));
            QPushButton *btn = new QPushButton(this);
            btn->setText(btnText);
            btn->setFixedSize(sizePageBtn);
            btn->setCheckable(true);
            btn->setProperty("pageRowIdx", row);
            btn->setProperty("pageColIdx", -1);
            rowSelectBtnGroup->addButton(btn);
            if(row == 0){ btn->setChecked(true); }
            connect(btn, SIGNAL(clicked()), this, SLOT(onPageSelected()));
            groupInLayout->addWidget(btn, 1+row*2, colIdx, 2, 1, Qt::AlignCenter);
        }
        colIdx++;
    }
    for(int row = 0; row < pagePortNum; ++row) {
        QLabel *lblHeader  = new QLabel(this);
        lblHeader->setText(tr("%1").arg(row+1, 2, 10, QChar('0')));
        lblHeader->setFixedSize(26, 16);
        groupInLayout->addWidget(lblHeader, row, colIdx, Qt::AlignRight);
        _inLabelList.append(lblHeader);
    }
    mainLayout->addWidget(groupInput, 1, 0, 1, 1, Qt::AlignCenter);

    updateRoutePage(0, 0);
    adjustSize();
}



void CfgSettingRouter::updateRoutePage(int pageRowIdx, int pageColIdx)
{
    int rowStart = _pagePortNum*pageRowIdx;
    int colStart = _pagePortNum*pageColIdx;

    for(int col = 0; col< _btnGroupList.count(); col++)
    {
        int colIdx = colStart + col;
        QButtonGroup* colBtnGroup = _btnGroupList[col];
        colBtnGroup->setExclusive(false);
        QString lblText = QString("%1").arg(_cfgItemRouter->getRouterInfo(colIdx)+1);
        _btnLabelList.at(colIdx%_pagePortNum)->setText(lblText);
        QList<QAbstractButton*> btnList = _btnGroupList[col]->buttons();
        for(int row = 0; row< btnList.count(); row++)
        {
            int rowIdx = rowStart + row;
            QPushButton* btn = qobject_cast<QPushButton*>(btnList[row]);
            if(btn != nullptr){
                btn->setProperty("row", rowIdx);
                btn->setProperty("col", colIdx);
                btn->setChecked(routerChecked(rowIdx, colIdx));
            }
        }
        colBtnGroup->setExclusive(true);
    }

    for(int idx = 0; idx< _outLabelList.count(); idx++)
    {
        _outLabelList[idx]->setText(QString("%1").arg(colStart + idx + 1, 3, 10, QChar('0')));
        _inLabelList[idx]->setText(QString("%1").arg(rowStart + idx + 1, 3, 10, QChar('0')));
    }

}

void CfgSettingRouter::setupUi_FangTu()
{
    QGridLayout *mainLayout = new QGridLayout(this);
    addHeader_FangTu(mainLayout);
    int flag_col = 0;
    for(int col = 0; col < _colNum; ++col) {
        if(!(col%4) && col > 0) {
            QLabel *lab_rowInterval = new QLabel(); lab_rowInterval->setFixedSize(5,5);
            mainLayout->addWidget(lab_rowInterval, 2, col+1+flag_col, Qt::AlignCenter);
            flag_col++;
        }
        QLineEdit *btnLabel = new QLineEdit(QString("%1").arg(_cfgItemRouter->getRouterInfo(col)+1));
        btnLabel->setAlignment(Qt::AlignCenter);
        btnLabel->setMaximumSize(50, 18);
        btnLabel->setDisabled(true);
        mainLayout->addWidget(btnLabel, 1, col+1+flag_col, Qt::AlignCenter);
        _btnLabelList.append(btnLabel);

        QButtonGroup *btnGroup = new QButtonGroup(this);
        btnGroup->setExclusive(true);

        int flag_row = 0;
        for(int row = 0; row < _rowNum; ++row) {
            QPushButton *btn = new QPushButton(this);
            btn->setCheckable(true);
            btn->setChecked(routerChecked(row, col));

            btn->setMaximumSize(/*24, 12*/50,18);
            btn->setProperty("row", row);
            btn->setProperty("col", col);
            btnGroup->addButton(btn);
            connect(btn, SIGNAL(clicked(bool)), this, SLOT(onRouterChanged(bool)));

//            qDebug() << __FUNCTION__ << __LINE__ << (row%4);
            if(!(row%4) && row > 0) {
                QLabel *lab_rowInterval = new QLabel(); lab_rowInterval->setFixedSize(5,5);
                mainLayout->addWidget(lab_rowInterval, row+2+flag_row, col+1+flag_col, Qt::AlignCenter);
                flag_row++;
            }
            mainLayout->addWidget(btn, row+2+flag_row, col+1+flag_col, Qt::AlignCenter);
        }

        _btnGroupList.append(btnGroup);
    }
}

bool CfgSettingRouter::routerChecked(int in, int out)
{
//    qDebug() << "open" << out << in << _cfgItemRouter->getRouterInfo(out);
    if (in == _cfgItemRouter->getRouterInfo(out))
    {
        return 1;
    } else {
        return 0;
    }
}

void CfgSettingRouter::onRouterChanged(bool checked)
{
    Q_UNUSED(checked)

    int in = sender()->property("row").toInt();
    int out = sender()->property("col").toInt();
//    qDebug() << "change" << out << in;
    _btnLabelList.at(out%_pagePortNum)->setText(QString("%1").arg(in+1));
    _cfgItemRouter->setRouterInfo(out, in);
}


void CfgSettingRouter::onPageSelected()
{
    int rowIdx = sender()->property("pageRowIdx").toInt();
    if(rowIdx != -1) _pageRowIdx = rowIdx;
    int colIdx = sender()->property("pageColIdx").toInt();
    if(colIdx != -1) _pageColIdx = colIdx;
    updateRoutePage(_pageRowIdx, _pageColIdx);
}


void CfgSettingRouter::addHeader_FangTu(QGridLayout *mainLayout)
{
    int flag = 0;
    for(int i = 1; i <= _colNum; i++) {
        QLabel *lblHeader  = new QLabel(this);
        lblHeader->setText(tr("Out %1").arg(i));
        if(!((i-1)%4) && i > 1) {
            QLabel *lab_rowInterval = new QLabel(); lab_rowInterval->setFixedSize(5,5);
            mainLayout->addWidget(lab_rowInterval, 0, i+flag, Qt::AlignCenter);
            flag++;
        }

        mainLayout->addWidget(lblHeader, 0, i+flag, Qt::AlignCenter);
    }

    flag = 0;
    for(int i = 0; i< _rowNum; i++) {
        QLabel *lblHeader  = new QLabel(this);
        lblHeader->setText(tr("In %1").arg(i+1));

        if(!(i%4) && i > 0) {
            QLabel *lab_rowInterval = new QLabel(); lab_rowInterval->setFixedSize(5,5);
            mainLayout->addWidget(lab_rowInterval, i+2+flag, 0, Qt::AlignCenter);
            flag++;
        }
        mainLayout->addWidget(lblHeader, i+2+flag, 0, Qt::AlignCenter);
    }
}

