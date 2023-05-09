#include "uciitem_port.h"
#include "customWidget/uciportwidget.h"
#include "usercontrolinterface.h"
#include "Lua/Packages/centerctrl.h"
#include <QTextEdit>

UciItem_port::UciItem_port(/*CfgType cfgType,*/ ConfigItemUCI *uciItem, int page, QWidget *parent)
    : QWidget{parent},/* _cfgType(cfgType),*/ _uciItem(uciItem), _page(page)
{
    initUI();
    //    initData();
}



void UciItem_port::initData(QString ip)
{
    qDebug() << __FUNCTION__ << __LINE__ << _labTitle->text() << _uciItem->getLoadUciFileProcData().s_ip << ip;
    if(ip != _uciItem->getLoadUciFileProcData().s_ip) return;

    foreach(UciPortWidget *item, _portWidgetList) {
        item->setOpacity(false);
    }
    QString title;
    QMapIterator<int, ConfigItemUCI::Uci_data_t *> i(_uciItem->getTabWidgetItem(_page)); //key() 控件位置
    i.toFront();
    while (i.hasNext()) {
        i.next();
//          qDebug() << __FUNCTION__ << __LINE__ << i.key() << ": " << i.value()->s_modelType;
        //拉取设备上数据
        double gain = 0.0; bool isMute = false;
        int idx = i.key() % 16;
        if((CfgType)i.value()->s_cfgType == CFG_TYPE_INPUT_4) {
            disconnect(_portWidgetList[idx], SIGNAL(gainAndMuteChanged(int, bool)), this, SLOT(onGainAndMuteChanged(int, bool)));
            _portWidgetList[idx]->setGainRang(i.value()->s_gainRange.minValue, i.value()->s_gainRange.maxValue);
           CenterCtrl::instance()->getInputGain(i.value()->s_name, i.value()->s_channel%4, gain);   _portWidgetList[idx]->setGain(gain);
           CenterCtrl::instance()->getInputMute(i.value()->s_name, i.value()->s_channel%4, isMute); _portWidgetList[idx]->setMute(isMute);
           connect(_portWidgetList[idx], SIGNAL(gainAndMuteChanged(int, bool)), this, SLOT(onGainAndMuteChanged(int, bool)));
        }else if((CfgType)i.value()->s_cfgType == CFG_TYPE_OUTPUT_4){
            disconnect(_portWidgetList[idx], SIGNAL(gainAndMuteChanged(int, bool)), this, SLOT(onGainAndMuteChanged(int, bool)));
            _portWidgetList[idx]->setGainRang(i.value()->s_gainRange.minValue, i.value()->s_gainRange.maxValue);
            CenterCtrl::instance()->getOutputGain(i.value()->s_name, i.value()->s_channel%4, gain);   _portWidgetList[idx]->setGain(gain);
            CenterCtrl::instance()->getOutputMute(i.value()->s_name, i.value()->s_channel%4, isMute); _portWidgetList[idx]->setMute(isMute);
            connect(_portWidgetList[idx], SIGNAL(gainAndMuteChanged(int, bool)), this, SLOT(onGainAndMuteChanged(int, bool)));
        }else continue;
          _portDataList.insert(i.key(), i.value());
        //拉取每个位置控件的属性
        _portWidgetList[idx]->setCfgType((CfgType)i.value()->s_cfgType);
        _portWidgetList[idx]->setChannel(i.value()->s_channel);
        _portWidgetList[idx]->setDevName(i.value()->s_name);
        _portWidgetList[idx]->setOpacity(i.value()->s_visible);
        _portWidgetList[idx]->setUserLab(i.value()->s_userLab);

        //title bar

    }
    _labTitle->setText(_uciItem->getSceneTitle());
}

void UciItem_port::setInOutLevel(unsigned int *d)
{
    FC_RTU_InOutLevel *level = (FC_RTU_InOutLevel *)d;
    foreach(UciPortWidget *item, _portWidgetList) {
        if((item->getCfgType() & FCT_FAMILY_MASK) == CFG_TYPE_INPUT_1) {
//            qDebug() << __FUNCTION__ << __LINE__ << level->vu_in[0];
//            level->vu_in[item->getChannel()] = 60;
            item->setLevelValue(level->vu_in[item->getChannel()]-100);
        }else if((item->getCfgType() & FCT_FAMILY_MASK) == CFG_TYPE_OUTPUT_1) {
//            qDebug() << __FUNCTION__ << __LINE__ << level->vu_out[0];
            item->setLevelValue(level->vu_out[item->getChannel()]-100);
        }
    }
}

void UciItem_port::createLab(QWidget *wid)
{
    QVBoxLayout *vlyt = new QVBoxLayout;
    vlyt->setContentsMargins(0,0,0,0);
    QWidget *widLevel = new QWidget;
    QLabel *lab_level   = new QLabel(tr("Level"));   lab_level  ->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    QLabel *lab_dBFS    = new QLabel(tr("(dBFS)"));  lab_dBFS   ->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    QLabel *lab_gain    = new QLabel(tr("Gain"));    lab_gain   ->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    QLabel *lab_Mute    = new QLabel(tr("Mute"));    lab_Mute   ->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    QLabel *lab_userLab = new QLabel(tr("Lab"));     lab_userLab->setAlignment(Qt::AlignRight|Qt::AlignCenter);

    lab_Mute->setFixedHeight(18);
    lab_userLab->setFixedHeight(35);
    //level widget
    widLevel->setFixedSize(40, 252);
    QVBoxLayout *levelWidLayout = new QVBoxLayout;
    levelWidLayout->setSpacing(0);
    levelWidLayout->setContentsMargins(0,widLevel->height()/2-18,0,widLevel->height()/2-18);
    levelWidLayout->addWidget(lab_level);
    levelWidLayout->addWidget(lab_dBFS);
    widLevel->setLayout(levelWidLayout);

    vlyt->addWidget(widLevel);
    vlyt->addWidget(lab_gain);
    vlyt->addWidget(lab_Mute);
    vlyt->addWidget(lab_userLab);

    wid->setLayout(vlyt);
}

void UciItem_port::initUI()
{
    //设备名称
    _labTitle = new QLabel(tr("Page %1").arg(_page+1)); _labTitle->setAlignment(Qt::AlignCenter);

    _labTitle->setProperty("form", "Title");
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(_labTitle, Qt::AlignCenter);


    //Level  Gain  Mute
    QHBoxLayout *portLayout = new QHBoxLayout;
    portLayout->setSpacing(30);
    portLayout->setContentsMargins(20,10, 60, 10);
    QGroupBox *portBox = new QGroupBox;
    portBox->setLayout(portLayout);

    _portWidgetList.clear();

    QHBoxLayout *portLayout_1_4 = new QHBoxLayout;    portLayout_1_4->setSpacing(2);
    //lab
    QWidget *widLab = new QWidget;
    widLab->setFixedWidth(40);
    createLab(widLab);
    portLayout_1_4->addWidget(widLab);

    for(int i = 0; i < 4; i++) {
        UciPortWidget *uciPortWid = new UciPortWidget;
        uciPortWid->setGainRang(-100, 12);
        connect(uciPortWid, SIGNAL(gainAndMuteChanged(int, bool)), this, SLOT(onGainAndMuteChanged(int, bool)));
        _portWidgetList.append(uciPortWid);
        portLayout_1_4->addWidget(uciPortWid);
    }

    QHBoxLayout *portLayout_5_8 = new QHBoxLayout;    portLayout_5_8->setSpacing(2);
    for(int i = 0; i < 4; i++) {
        UciPortWidget *uciPortWid = new UciPortWidget;
        uciPortWid->setGainRang(-100, 12);
        connect(uciPortWid, SIGNAL(gainAndMuteChanged(int, bool)), this, SLOT(onGainAndMuteChanged(int, bool)));
        _portWidgetList.append(uciPortWid);
        portLayout_5_8->addWidget(uciPortWid);
    }
    QHBoxLayout *portLayout_9_12 = new QHBoxLayout;   portLayout_9_12->setSpacing(2);
    for(int i = 0; i < 4; i++) {
        UciPortWidget *uciPortWid = new UciPortWidget;
        uciPortWid->setGainRang(-100, 12);
        connect(uciPortWid, SIGNAL(gainAndMuteChanged(int, bool)), this, SLOT(onGainAndMuteChanged(int, bool)));
        _portWidgetList.append(uciPortWid);
        portLayout_9_12->addWidget(uciPortWid);
    }
    QHBoxLayout *portLayout_13_16 = new QHBoxLayout;  portLayout_13_16->setSpacing(2);
    for(int i = 0; i < 4; i++) {
        UciPortWidget *uciPortWid = new UciPortWidget;
        uciPortWid->setGainRang(-100, 12);
        connect(uciPortWid, SIGNAL(gainAndMuteChanged(int, bool)), this, SLOT(onGainAndMuteChanged(int, bool)));
        _portWidgetList.append(uciPortWid);
        portLayout_13_16->addWidget(uciPortWid);
    }

    portLayout->addLayout(portLayout_1_4);
    portLayout->addLayout(portLayout_5_8);
    portLayout->addLayout(portLayout_9_12);
    portLayout->addLayout(portLayout_13_16);
//    portLayout->addLayout(portLayout_sumOutput);
    mainLayout->addWidget(portBox);

    //快照切换按钮
    QGroupBox *sceneBox = new QGroupBox;           sceneBox->setProperty("form", "Scene");
    QHBoxLayout *sceneLayout = new QHBoxLayout;    sceneLayout->setSpacing(34);
    sceneLayout->setContentsMargins(33, 25, 35, 25);
    sceneBox->setLayout(sceneLayout);
    _sceneBtnList.clear();
    for(int i = 0; i < 4; i++) {
         QGridLayout *Lyt = new QGridLayout;
        Lyt->setContentsMargins(0,0,0,0);
        Lyt->setSpacing(6);
        sceneLayout->addLayout(Lyt);
        for(int j = 0; j < 4; j++) {
            QPushButton *btn = new QPushButton;
            btn->setFixedSize(52, 18);
            btn->setCheckable(true);
            Lyt->addWidget(btn, 0, i*j+j);
            QTextEdit *textEdit = new QTextEdit;
            textEdit->horizontalScrollBar()->setHidden(true);
            textEdit->verticalScrollBar()->setHidden(true);
            textEdit->setPlainText(QString("Snap %1").arg(i*4+j+1));
            textEdit->setFixedSize(52, 27);
            Lyt->addWidget(textEdit, 1, i*j+j);
            _sceneBtnList.append(btn);
        }
    }


    QHBoxLayout *sceneLayout1 = new QHBoxLayout;
    sceneLayout1->setContentsMargins(/*30*/0, 0, 30, 30);
    sceneLayout1->setSpacing(0);
    QLabel *lab = new QLabel(tr("Snap-shot"));
    lab->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    lab->setWordWrap(true);
    lab->setFixedWidth(30);
    sceneLayout1->addWidget(lab);
    sceneLayout1->addWidget(sceneBox);
    QWidget *widScene = new QWidget; widScene->setLayout(sceneLayout1);
    mainLayout->addWidget(widScene);
    setLayout(mainLayout);
    widScene->setHidden(true);
    setStyleSheet(QLatin1String("*{ background-color: #4E505C; }"
                      "QLabel[form=\"Title\"]{ font: 20px; border: 0px solid white; background-color: #4E5C5A;}"
                      "QGroupBox{ border: 0px; }"
                      "QGroupBox[form=\"Scene\"]{ background-color: #3E414A; border-radius: 5px; }"
                      "QPushButton { "
                      "      border: 0px solid #8f8f91; "
                      "      border-radius: 5px; background-color: #C8E8F4; "
                      "      width: 50px; height: 18px; "
                      "}"
                      "QPushButton:checked { background-color: #459C63; }"
                      "QPushButton:focus{ border:2px solid #ff0000; }"
                      "QPushButton:hover{ border:2px solid #00ff00; }"
                      "QTextEdit{ "
                      "     border: 0px solid #8f8f91; "
                      "     border-radius: 5px; "
                      "     background-color: #C8E8F4; "
                      "     selection-background-color:#484848;"
                      "     selection-color:#DCDCDC;"
                      "     color: black;"
                      "}"
                      "QTextEdit:focus{ border:2px solid #ff0000; }"
                      "QTextEdit:hover{ border:2px solid #00ff00; }"
                                ));
}

void UciItem_port::onGainAndMuteChanged(int gainValue, bool mute)
{
    UciPortWidget *sen = qobject_cast<UciPortWidget *>(sender());
    if(!sen) return;
    if(sen->getCfgType() == CFG_TYPE_INPUT_4) {
//        qDebug() << __FUNCTION__ << __LINE__ << sen->getChannel() << sen->getDevName() << gainValue << mute
       /* << */CenterCtrl::instance()->setInputGain(sen->getDevName(), sen->getChannel()%4, gainValue);
       /* << */CenterCtrl::instance()->setInputMute(sen->getDevName(), sen->getChannel()%4, mute);
    }else if(sen->getCfgType() == CFG_TYPE_OUTPUT_4) {
//        qDebug() << __FUNCTION__ << __LINE__ << sen->getChannel() << sen->getDevName() << gainValue << mute
        /*<< */CenterCtrl::instance()->setOutputGain(sen->getDevName(), sen->getChannel()%4, gainValue);
        /*<< */CenterCtrl::instance()->setOutputMute(sen->getDevName(), sen->getChannel()%4, mute);
    }

}

void UciItem_port::onGainAndMuteChanged_sumOutput(int gainValue, bool mute)
{
    _sumOut_valueMark = gainValue - _sumOut_valueMark;
//    qDebug() << __FUNCTION__ << __LINE__ << _sumOut_valueMark << gainValue;
    foreach(UciPortWidget *item, _portWidgetList) {
        if((item->getCfgType()&FCT_FAMILY_MASK) == CFG_TYPE_OUTPUT_1) {
            disconnect(item, SIGNAL(gainAndMuteChanged(int, bool)), this, SLOT(onGainAndMuteChanged(int, bool)));
            item->setGain(item->getGainValue()+_sumOut_valueMark);
            item->setMute(mute);
            CenterCtrl::instance()->setOutputGain(item->getDevName(), item->getChannel()%4, item->getGainValue()+_sumOut_valueMark);
            CenterCtrl::instance()->setOutputMute(item->getDevName(), item->getChannel()%4, mute);
            connect(item, SIGNAL(gainAndMuteChanged(int, bool)), this, SLOT(onGainAndMuteChanged(int, bool)));
        }
    }

    _sumOut_valueMark = gainValue;
}
