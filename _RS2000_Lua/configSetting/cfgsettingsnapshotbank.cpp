#include "cfgsettingsnapshotbank.h"
#include <QVBoxLayout>
#include "customWidget/basewindow.h"

CfgSettingSnapshotBank::CfgSettingSnapshotBank(ConfigItemSnapshot *cfgItem, QWidget *parent)
    : QWidget(parent), _cfgItem(cfgItem),_snapCount(_cfgItem->getSnapCount())
{
    initUI();
    initData();
    connected();
}

void CfgSettingSnapshotBank::onBtnClicked_slots()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
//    qDebug() << __FUNCTION__ << __LINE__ << btn->text();
    if(!btn) return ;
    int serial = btn->property(Serial).toUInt();
    switch (btn->property(BtnType).toUInt()) {
    case ConfigItemSnapshot::BtnType_Prev:
        setBtnPervOrNext(ConfigItemSnapshot::BtnType_Prev);
        break;
    case ConfigItemSnapshot::BtnType_Load: {
        if(!btn->isChecked()) {
            disconnect(btn, &QPushButton::clicked, this, &CfgSettingSnapshotBank::onBtnClicked_slots);
            btn->setChecked(true);
            connect(btn, &QPushButton::clicked, this, &CfgSettingSnapshotBank::onBtnClicked_slots);
            break;
        }
        refreshLoadBtn(btn);
        _cfgItem->uploadData(btn->property(Serial).toUInt());
        break;
    }
    case ConfigItemSnapshot::BtnType_Save: {
//        disconnect(_snapBtnList.at(serial).s_loadBtn, &QPushButton::clicked, this, &CfgSettingSnapshotBank::onBtnClicked_slots);
        _snapBtnList.at(serial).s_loadBtn->setEnabled(true);
        refreshLoadBtn(_snapBtnList.at(serial).s_loadBtn);
        _cfgItem->saveData(btn->property(Serial).toUInt());
//        connect(_snapBtnList.at(serial).s_loadBtn, &QPushButton::clicked, this, &CfgSettingSnapshotBank::onBtnClicked_slots);
        break;
    }
    case ConfigItemSnapshot::BtnType_Next:
        setBtnPervOrNext(ConfigItemSnapshot::BtnType_Next);
        break;
    default:
        break;
    }
}

void CfgSettingSnapshotBank::onLineEditEditingFinished_slots()
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());
    if(!lineEdit) return;
    int serial = lineEdit->property(Serial).toUInt();

    _cfgItem->setLabel(serial - 1, lineEdit->text());
}

void CfgSettingSnapshotBank::setUISize()
{
    int x = _snapCount / 8;
    int y = _snapCount % 8;    
    int btn_h = _snapBtnList.at(1).s_loadBtn->height();
    int btn_w = _snapBtnList.at(1).s_loadBtn->width();
    int Edit_h = _snapBtnList.at(1).s_lineEdit->height();
    int space = 5, border_space = 20;
    int load_h = 2 * (btn_h + space) + Edit_h;
}

void CfgSettingSnapshotBank::removeTheLayout()
{
    if(!_snapBtnList.isEmpty()) {
        foreach(SnpBtnPair_t item, _snapBtnList) {
            this->layout()->removeWidget(item.s_loadBtn);
            if(item.s_saveBtn) {
                this->layout()->removeWidget(item.s_saveBtn);
            }
            delete item.s_loadBtn;
            delete item.s_saveBtn;
            delete item.s_lineEdit;
        }
        _snapBtnList.clear();
        delete this->layout();
    }
}

void CfgSettingSnapshotBank::initUI()
{
    _snapCount = _cfgItem->getSnapCount();
    removeTheLayout();
    int x = _snapCount / 8;
    int y = _snapCount % 8;
    QGridLayout *gLayOut = new QGridLayout;

    QPushButton *loadPrevBtn = new QPushButton(QString("Load\nPrev"));
    loadPrevBtn->setProperty(BtnType, ConfigItemSnapshot::BtnType_Prev);
    loadPrevBtn->setProperty(Serial, 0);
    loadPrevBtn->setFixedSize(40, 40);
    loadPrevBtn->setStyleSheet("QPushButton{background-color:rgba(0,100,0,100%);\
                            color: white;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}" // 按键本色
                            "QPushButton:hover{background-color:white; color: black;}"  //鼠标停放时的色彩
                            "QPushButton:pressed{background-color:rgb(50, 205, 50); border-style: inset; }"   // 鼠标按下的色彩
                            );
    gLayOut->addWidget(loadPrevBtn, 0, 0, Qt::AlignTop);
    _snapBtnList.append(SnpBtnPair_t(loadPrevBtn, NULL, NULL));

    for(int row = 0; row < x; row++) {
        for(int col = 1; col <= 8; col++) {
            QPushButton *loadBtn = new QPushButton(QString("Load\n%1").arg(col + 8*row));
            loadBtn->setProperty(BtnType, ConfigItemSnapshot::BtnType_Load);
            loadBtn->setProperty(Serial, col + 8*row);
            loadBtn->setFixedSize(40, 40);
            loadBtn->setCheckable(true);
//            loadBtn->setEnabled(false);
            loadBtn->setStyleSheet("QPushButton{background-color:rgba(0,100,0,100%);\
                                    color: white;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}"
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:checked{background-color:rgb(50, 205, 50); border-style: inset; }"
                                    "QPushButton:disabled{background-color:rgba(143,188,143, 100%);}"
                                    );

            QPushButton *saveBtn = new QPushButton(QString("Save\n%1").arg(col + 8*row));
            saveBtn->setProperty(BtnType, ConfigItemSnapshot::BtnType_Save);
            saveBtn->setProperty(Serial, col + 8*row);
            saveBtn->setFixedSize(40, 40);
            saveBtn->setStyleSheet("QPushButton{background-color:rgba(255,178,0,100%);\
                                    color: white;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}"
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                                    );
            QLineEdit *lineEdit = new QLineEdit(QString(col + 8*row));
            lineEdit->setProperty(BtnType, ConfigItemSnapshot::BtnType_label);
            lineEdit->setProperty(Serial, col + 8*row);
            lineEdit->setFixedSize(40, 20);
            lineEdit->setAlignment(Qt::AlignCenter);
            lineEdit->setStyleSheet(QLatin1String("QLineEdit{ border:1px solid #242424;\
                                                            border-radius:3px;\
                                                            padding:2px;\
                                                            background:#D8D8D8;\
                                                            selection-background-color:#484848;\
                                                            selection-color:#DCDCDC;}\
                                                   QLineEdit:focus{ border:1px solid #242424;}"));
            QVBoxLayout *vLayOut = new QVBoxLayout();
            vLayOut->addWidget(loadBtn);
            vLayOut->addWidget(saveBtn);
            vLayOut->addWidget(lineEdit);

            gLayOut->addLayout(vLayOut, row, col);
            _snapBtnList.append(SnpBtnPair_t(loadBtn, saveBtn, lineEdit));
        }
    }
    for(int col = 1; col <= y; col++) {
        QPushButton *loadBtn = new QPushButton(QString("Load\n%1").arg(col + 8*x));
        loadBtn->setProperty(BtnType, ConfigItemSnapshot::BtnType_Load);
        loadBtn->setProperty(Serial, col + 8*x);
        loadBtn->setFixedSize(40, 40);
        loadBtn->setCheckable(true);
//        loadBtn->setEnabled(false);
        loadBtn->setStyleSheet("QPushButton{background-color:rgba(0,100,0,100%);\
                                color: white;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}"
                                "QPushButton:hover{background-color:white; color: black;}"
                                "QPushButton:checked{background-color:rgb(50, 205, 50); border-style: inset; }"
                                "QPushButton:disabled{background-color:rgba(143,188,143, 100%);}"
                                );

        QPushButton *saveBtn = new QPushButton(QString("Save\n%1").arg(col + 8*x));
        saveBtn->setProperty(BtnType, ConfigItemSnapshot::BtnType_Save);
        saveBtn->setProperty(Serial, col + 8*x);
        saveBtn->setFixedSize(40, 40);
        saveBtn->setStyleSheet("QPushButton{background-color:rgba(255,178,0,100%);\
                                color: white;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}"
                                "QPushButton:hover{background-color:white; color: black;}"
                                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                                );
        QLineEdit *lineEdit = new QLineEdit(QString(col + 8*x));
        lineEdit->setProperty(BtnType, ConfigItemSnapshot::BtnType_label);
        lineEdit->setProperty(Serial, col + 8*x);
        lineEdit->setFixedSize(40, 20);
        lineEdit->setAlignment(Qt::AlignCenter);
        lineEdit->setStyleSheet(QLatin1String("QLineEdit{ border:1px solid #242424;\
                                                        border-radius:3px;\
                                                        padding:2px;\
                                                        background:#D8D8D8;\
                                                        selection-background-color:#484848;\
                                                        selection-color:#DCDCDC;}\
                                               QLineEdit:focus{ border:1px solid #242424;}"));

        QVBoxLayout *vLayOut = new QVBoxLayout();
        vLayOut->addWidget(loadBtn);
        vLayOut->addWidget(saveBtn);
        vLayOut->addWidget(lineEdit);

        gLayOut->addLayout(vLayOut, x, col);
        _snapBtnList.append(SnpBtnPair_t(loadBtn, saveBtn, lineEdit));
    }
    QPushButton *loadNextBtn = new QPushButton(QString("Load\nNext"));
    loadNextBtn->setProperty(BtnType, ConfigItemSnapshot::BtnType_Next);
    loadNextBtn->setProperty(Serial, _snapCount + 1);
    loadNextBtn->setFixedSize(40, 40);
    loadNextBtn->setStyleSheet("QPushButton{background-color:rgba(0,100,0,100%);\
                                    color: white;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}"
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(50, 205, 50); border-style: inset; }"
                                    );
    if(y == 0) x -= 1;
    gLayOut->addWidget(loadNextBtn, x, 9, Qt::AlignTop);
    _snapBtnList.append(SnpBtnPair_t(loadNextBtn, NULL, NULL));
    this->setLayout(gLayOut);

    setUISize();
}

void CfgSettingSnapshotBank::connected()
{
    QListIterator<SnpBtnPair_t> i(_snapBtnList);
    while(i.hasNext()) {
        SnpBtnPair_t pair = i.next();
        connect(pair.s_loadBtn, &QPushButton::clicked, this, &CfgSettingSnapshotBank::onBtnClicked_slots);
        if(pair.s_saveBtn)
            connect(pair.s_saveBtn, &QPushButton::clicked, this, &CfgSettingSnapshotBank::onBtnClicked_slots);
        if(pair.s_lineEdit)
            connect(pair.s_lineEdit, &QLineEdit::editingFinished, this, &CfgSettingSnapshotBank::onLineEditEditingFinished_slots);
    }
}

void CfgSettingSnapshotBank::disconnected()
{
    QListIterator<SnpBtnPair_t> i(_snapBtnList);
    while(i.hasNext()) {
        SnpBtnPair_t pair = i.next();
        disconnect(pair.s_loadBtn, &QPushButton::clicked, this, &CfgSettingSnapshotBank::onBtnClicked_slots);
        if(pair.s_saveBtn)
            disconnect(pair.s_saveBtn, &QPushButton::clicked, this, &CfgSettingSnapshotBank::onBtnClicked_slots);
        if(pair.s_lineEdit)
            disconnect(pair.s_lineEdit, &QLineEdit::editingFinished, this, &CfgSettingSnapshotBank::onLineEditEditingFinished_slots);
    }
}

void CfgSettingSnapshotBank::initData()
{
    QListIterator<SnpBtnPair_t> i(_snapBtnList);
//    int idx = 0;
    i.toFront();
    while(i.hasNext()) {
        SnpBtnPair_t pair = i.next();
        int idx = pair.s_loadBtn->property(Serial).toUInt();
        idx -= 1;
        if(pair.s_loadBtn == _snapBtnList.first().s_loadBtn) continue;
        if(pair.s_loadBtn == _snapBtnList.last().s_loadBtn) return;
        if(idx >= ConfigItemSnapshot::DataCount) return;
        pair.s_loadBtn->setEnabled(_cfgItem->getbuttonPattern(idx));
//        qDebug() << __FUNCTION__ << __LINE__ << _cfgItem->getbuttonPattern(idx)
//                 << idx << pair.s_loadBtn->property(Serial).toUInt() << _cfgItem->getLabel(idx);
        if(pair.s_loadBtn->property(Serial).toUInt() == _cfgItem->getCurrentPattern()) {
            refreshLoadBtn(pair.s_loadBtn);
        }
        if(pair.s_lineEdit) {
            pair.s_lineEdit->setText(_cfgItem->getLabel(idx));
        }
//        idx++;
    }
}

void CfgSettingSnapshotBank::refreshLoadBtn(QPushButton *btn)
{
    QListIterator<SnpBtnPair_t> i(_snapBtnList);
    while(i.hasNext()) {
        SnpBtnPair_t pair = i.next();
//        qDebug() << __FUNCTION__ << __LINE__ << pair.s_loadBtn->isEnabled() << pair.s_loadBtn->isCheckable() << btn->text();
        disconnect(pair.s_loadBtn, &QPushButton::clicked, this, &CfgSettingSnapshotBank::onBtnClicked_slots);
        if(pair.s_loadBtn->isEnabled() && pair.s_loadBtn->isCheckable()) {
              if(pair.s_loadBtn == btn) {
                 pair.s_loadBtn->setChecked(true);
            }else if(pair.s_loadBtn->isChecked()) {
                 pair.s_loadBtn->setChecked(false);
            }
        }
        connect(pair.s_loadBtn, &QPushButton::clicked, this, &CfgSettingSnapshotBank::onBtnClicked_slots);
    }
}

void CfgSettingSnapshotBank::setBtnPervOrNext(ConfigItemSnapshot::btnType type)
{
    QList<QPushButton *> loadBtnList;
    loadBtnList.clear();
    QListIterator<SnpBtnPair_t> i(_snapBtnList);
    while(i.hasNext()) {
        SnpBtnPair_t pair = i.next();
        if(pair.s_loadBtn->property(Serial).toUInt() == 0) continue;
        if(pair.s_loadBtn->property(Serial).toUInt() == _snapCount + 1) continue;
        if(pair.s_loadBtn->isEnabled()) {
           loadBtnList.append(pair.s_loadBtn);
        }
    }
    if(loadBtnList.isEmpty()) return ;
    int curPattern = _cfgItem->getCurrentPattern();
//    qDebug() << __FUNCTION__ << __LINE__ << curPattern;
    for(int idx = 0; idx < loadBtnList.length(); idx++) {
        if(loadBtnList.at(idx) == _snapBtnList.at(curPattern).s_loadBtn) {
            if(type == ConfigItemSnapshot::BtnType_Prev) {
                if(curPattern <=  loadBtnList.first()->property(Serial).toUInt()) return;
                refreshLoadBtn(loadBtnList.at(idx - 1));
                _cfgItem->uploadData(loadBtnList.at(idx - 1)->property(Serial).toUInt());
                _cfgItem->setCurrentPattern(loadBtnList.at(idx - 1)->property(Serial).toUInt());
            }else if(type == ConfigItemSnapshot::BtnType_Next) {
                if(curPattern >=  loadBtnList.last()->property(Serial).toUInt()) return;
                refreshLoadBtn(loadBtnList.at(idx + 1));
                _cfgItem->uploadData(loadBtnList.at(idx + 1)->property(Serial).toUInt());
                _cfgItem->setCurrentPattern(loadBtnList.at(idx + 1)->property(Serial).toUInt());
            }
        }
    }

}
