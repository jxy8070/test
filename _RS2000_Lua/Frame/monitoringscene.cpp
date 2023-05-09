#include "stable.h"
#include "monitoringscene.h"
#include <QMimeData>
#include <QGraphicsItem>
#include "deviceItem/deviceItem.h"
#include "configItem/configItem.h"
#include "configItem/configitemfactory.h"
#include "monitoring_aids/monitorblackstage.h"
#include "monitoringview.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>
#include "monitoring_aids/concretemonitoritem.h"
#include "Frame/mainwindow.h"

MonitoringScene::MonitoringScene(QObject *parent) : QGraphicsScene(parent)
{

    _configIndex = 0;
    _scaleView = 1;
    _labelEditor = false;
    _monitorItem = NULL;
    _view = qobject_cast<MonitoringView *>(parent);
    connect(this, SIGNAL(sceneRectChanged(QRectF)), _view, SLOT(onGetRectsize(QRectF)));
    MonitorBlackStage::instance()->getView(_view);
//     qDebug()<<"----------------------------------_deviceIndex"<<_configIndex;
//    _view = qobject_cast<MonitoringView *>(parent);
#if MONITOR
//     _view = qobject_cast<MonitorItem *>(parent);
#else
     _view = qobject_cast<ConfigView *>(parent);
#endif
    _undoStack = new QUndoStack(this);
    //右键菜单选项实例化

    _selectAllMenu    = new QMenu(tr("Select All"));
    _selectAll        = new QAction("selectAll");
    _selectAllMeter   = new QAction("selectAllMeters");
    _selectAllClipper = new QAction("selectAllClippers");
    _selectAllButton  = new QAction("selectAllButton");
    _selectAllLabel   = new QAction("selectAllLabel");
//需要重构 共同的功能移动到基类
    _scale     = new QMenu(tr("Scale"), _view);
    _scale_25  = new QAction(tr("%25"), _scale);
    _scale_50  = new QAction(tr("%50"), _scale);
    _scale_75  = new QAction(tr("%75"), _scale);
    _scale_100 = new QAction(tr("%100"), _scale);
    _scale_125 = new QAction(tr("%125"), _scale);
    _scale_150 = new QAction(tr("%150"), _scale);
    _scale_175 = new QAction(tr("%175"), _scale);
    _scale_200 = new QAction(tr("%200"), _scale);

    _delAction = new QAction("delete",this);
    _propertyAction = new QAction("Property",this);

    _displayStyle = new QMenu(tr("Display Style"));
    _display1     = new QAction(tr("Display-1"));
    _display2     = new QAction(tr("Display-2"));
    _display3     = new QAction(tr("Display-3"));
    _display4     = new QAction(tr("Display-4"));
    _display5     = new QAction(tr("Display-5"));

    /////////////////////初始化右键上下文菜单//////////////////////
    _displayMenu.addMenu(_displayStyle);
    _displayMenu.addAction(_displayMenu.addSeparator());
    _displayMenu.addAction(_delAction);
    _displayMenu.addAction(_displayMenu.addSeparator());
    _displayMenu.addAction(_propertyAction);
    _displayMenu.addAction(_displayMenu.addSeparator());
    _displayMenu.addMenu(_selectAllMenu);
    _displayMenu.addMenu(_scale);

    ///////////////////////菜单页面选项/////////////////////////////
    //select
    _selectAllMenu->addAction(_selectAll);
    _selectAllMenu->addAction(_selectAllMeter);
    _selectAllMenu->addAction(_selectAllClipper);
    _selectAllMenu->addAction(_selectAllButton);
    _selectAllMenu->addAction(_selectAllLabel);
    //scale
    _scale->addAction(_scale_25);
    _scale->addAction(_scale_50);
    _scale->addAction(_scale_75);
    _scale->addAction(_scale_100);
    _scale->addAction(_scale_125);
    _scale->addAction(_scale_150);
    _scale->addAction(_scale_175);
    _scale->addAction(_scale_200);
    //displayStyle
    _displayStyle->addAction(_display1);
    _displayStyle->addAction(_display2);
    _displayStyle->addAction(_display3);
    _displayStyle->addAction(_display4);
    _displayStyle->addAction(_display5);


    setConnection();     //连接信号与槽

    _sheetMaxSize = new QGraphicsTextItem(" ");
    addItem(_sheetMaxSize);
    set_Size(1920,1080);
    if(!MonitorBlackStage::instance()->getOpenViewList().isEmpty())
        setInitialize();        //打开后台的数据信息
}


/////////////////////////////鼠标事件////////////////////////////////////
/// \brief MonitoringScene::dropEvent
/// \param e
///
void MonitoringScene::dropEvent(QGraphicsSceneDragDropEvent *e)
{
    QString typeString(e->mimeData()->data("type"));
#if MONITOR
    MonitorItem *monitorItem = MonitoItemFactory::CreateInstance(CfgType(typeString.toInt()),
                                          QString("monitor_%1").arg(++_configIndex), e->scenePos(), 50, 50/*, _view*/);
    if(monitorItem != NULL)
    {
        addItem(monitorItem);
        emit sendSaveclick(false);
        qDebug()<<"monitorItem != NULL";
    }
#else
    ConfigItem *cfgItem = ConfigItemFactory::createConfigItem(nullptr, CfgType(typeString.toInt()),
                            ++_configIndex, e->scenePos(), _view);

    if(cfgItem != NULL)
    {
        addItem(cfgItem);
        qDebug()<<"cfgItem != NULL";
    }
#endif
}

void MonitoringScene::dragEnterEvent(QGraphicsSceneDragDropEvent *e)
{
    e->acceptProposedAction();
}

void MonitoringScene::dragMoveEvent(QGraphicsSceneDragDropEvent *e)
{
    e->acceptProposedAction();
}

void MonitoringScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QTransform transform;
//    QPointF monitorPos;
//    _monitorItem = NULL;
//    if(selectedItems().count() == 1)
//    {
//        _monitorItem = qgraphicsitem_cast<MonitorItem *>(selectedItems().at(0));
//        if(_monitorItem == NULL){
//            qDebug()<<"_monitorItem is NULL";
//            return ;
//        }
//        qDebug()<<"_monitorItem "<<_monitorItem->getMonitorScenePos();
//        monitorPos = _monitorItem->getMonitorScenePos();
//    }
    if(event->button() == Qt::LeftButton)
    {
        bool select = false;

        QGraphicsItem *selectedItem = itemAt(event->scenePos(),transform);//返回指定位置的最上层的可见项，没有返回0
        if(selectedItem == NULL)
        {
            qDebug()<<"selectedItem == NULL";
            return ;
        }else
        {
            foreach(QGraphicsItem *item, selectedItems())
            {
                if (item->sceneBoundingRect().contains(event->scenePos()) == true)//如果鼠标位置在设备矩形的边界矩形的边上或者内部
                {
//                    item->
                    select = true;
                    break;
                }
            }
            if(!select){
                foreach (QGraphicsItem *item, selectedItems()) {
                   item->setSelected(false);
                }
                                                                                                                                                                                                                                                                                                                                                                                      }

            selectedItem->setSelected(true);
            qDebug()<<"selectedItem != NULL"<<event->scenePos();
            _pressPos = selectedItem->scenePos();//获得按下鼠标光标位置；

            goto ProjectSceneMousePressEvent_End;           
        }

    }
    else if (event->button() == Qt::RightButton)
    {
        if(!items().isEmpty())
        {
            foreach(QGraphicsItem *item, items())
            {
                if(item->sceneBoundingRect().contains(event->scenePos()) == true)
                {
                    item->setSelected(true);
                }
            }
        }

        if(selectedItems().isEmpty())
        {
            qDebug()<<"selectedItems() == NULL";
            return ;
        }
        foreach (QGraphicsItem *item, selectedItems())
        {
            if (item->sceneBoundingRect().contains(event->scenePos()) == false)//如果鼠标位置不在项的边界矩形的边上或者内部
            {
                item->setSelected(false);
            }
        }
    }

ProjectSceneMousePressEvent_End:
    QGraphicsScene::mousePressEvent(event);

}

void MonitoringScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QTransform transForm;
    QGraphicsItem *CurrentItem = itemAt(event->scenePos(), transForm);
//    qDebug()<<__FUNCTION__<<__LINE__;
    QGraphicsItem *item = NULL;
    if(selectedItems().count() == 1)
    {
        item = selectedItems().at(0);
//        qDebug()<<__FUNCTION__<<__LINE__<<item;
    }

    if(item == NULL)
    {
        _view->viewport()->setCursor(Qt::ArrowCursor);
//        qDebug()<<__FUNCTION__<<__LINE__<<"monitorItem is NULL!";
        goto monitorScenemouseMoveEvent_end;
    }
    MonitorItem *monitorItem = qgraphicsitem_cast<MonitorItem *>(item);
    if(monitorItem->getType() == CFG_TYPE_4 && item == CurrentItem && _labelEditor == true
            && CurrentItem->sceneBoundingRect().contains(event->scenePos()))
    {
        _view->viewport()->setCursor(Qt::SizeHorCursor);//横向双箭头
//        qDebug()<<__FUNCTION__<<__LINE__;
    }else {
        _view->viewport()->setCursor(Qt::ArrowCursor);
//        qDebug()<<__FUNCTION__<<__LINE__;
    }

    QPointF curPos = event->scenePos();
    if(event->buttons() != Qt::NoButton)//移动了选择的监控设备
    {
        emit sendSaveclick(false);
//        qDebug()<<"////////////////////////"<<curPos;
        if(!selectedItems().isEmpty() && _labelEditor == true)
        {
            _offsetPos = curPos - _pressPos;
//            qDebug()<<"lllllllllllllllllllllll"<<curPos<<_offsetPos;
            monitorItem->setRectSize(_offsetPos);
            return ;
        }
    }
monitorScenemouseMoveEvent_end:
    QGraphicsScene::mouseMoveEvent(event);//移动Item
}

void MonitoringScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)//鼠标释放事件
{
    qDebug()<<"monseRelease"<<event->scenePos();
    QTransform transForm;
    if(itemAt(event->scenePos(), transForm))
    {
        _releasePos = event->scenePos();
        _offsetPos = _releasePos - _pressPos;
    }
    foreach (QGraphicsItem *item, selectedItems()) {
       MonitorItem * monitorItem = qgraphicsitem_cast<MonitorItem *>(item);
       if(monitorItem == NULL){
           qDebug()<<"monitorItem is NULL!";
           continue;
       }

       if(!_offsetPos.isNull()){
            monitorItem->setScenePos(_offsetPos + monitorItem->getMonitorScenePos());
       }
       _releasePos = monitorItem->rect().topLeft();
       qDebug()<<"--------------"<<_releasePos;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void MonitoringScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Delete:
        on_deleteClicked();
        break;
    case Qt::Key_Alt:
//        qDebug()<<"key_Alt";
        _labelEditor = true;
        break;
    default:
        break;
    }
}

void MonitoringScene::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Alt:
//        qDebug()<<"key_Alt Release";
        _labelEditor = false;
        break;
    default:
        break;
    }
}

void MonitoringScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QList<QGraphicsItem *> items = selectedItems();
    _delAction->setVisible(false);
    _propertyAction->setVisible(false);
    _displayStyle->menuAction()->setVisible(false);//QMenu直接调用QAction的SetVisible不能进行隐藏，需要先于QAction建立联系
    if(!items.isEmpty())
    {
        _delAction->setVisible(true);
        _propertyAction->setVisible(true);
        foreach(QGraphicsItem *item, items){
            MonitorItem *monitorItem = qgraphicsitem_cast<MonitorItem *>(item);
            if(monitorItem->getType() == CFG_TYPE_1 || monitorItem->getType() == CFG_TYPE_2
                    || monitorItem->getType() == CFG_TYPE_3 || monitorItem->getType() == CFG_TYPE_5)
            {
                _displayStyle->menuAction()->setVisible(true);
                _display1->setVisible(true);
                _display2->setVisible(true);
                _display3->setVisible(true);
                _display4->setVisible(true);
                _display5->setVisible(true);

                if(monitorItem->getType() == CFG_TYPE_1)
                {
//                    _display4->setVisible(false);
//                    _display5->setVisible(false);
                }
                if(monitorItem->getType() == CFG_TYPE_2)
                {
//                    _display4->setVisible(false);
                    _display5->setVisible(false);
                }
                if(monitorItem->getType() == CFG_TYPE_5)
                {
                    _display3->setVisible(false);
                    _display4->setVisible(false);
                    _display5->setVisible(false);
                }

            }
        }
    }
    _displayMenu.exec(event->screenPos());
}

void MonitoringScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(selectedItems().count() == 1)
    {
#if MONITOR
        _monitorItem = NULL;
        _monitorItem = qgraphicsitem_cast<MonitorItem *>(selectedItems().at(0));
#else
        _cfgItem = NULL;
        _cfgItem = qgraphicsitem_cast<ConfigItem *>(selectedItems().at(0));
#endif
    }else
    {
        return ;
    }
    on_SetProperty();
//    return ;
    //    QGraphicsScene::mouseDoubleClickEvent(event);
}

void MonitoringScene::setConnection()
{
    connect(_delAction, SIGNAL(triggered()), this, SLOT(on_deleteClicked()));
    connect(_propertyAction, SIGNAL(triggered()), this, SLOT(on_SetProperty()));
    //select
    connect(_selectAll, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_selectAllMeter, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_selectAllClipper, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_selectAllButton, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_selectAllLabel, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    //scale
    connect(_scale_25, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_scale_50, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_scale_75, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_scale_100, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_scale_125, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_scale_150, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_scale_175, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_scale_200, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    //displayStyle
    connect(_display1, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_display2, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_display3, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_display4, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));
    connect(_display5, SIGNAL(triggered(bool)), this, SLOT(on_PushButtonClicked()));

    connect(MonitorBlackStage::instance(), SIGNAL(getItemXY()), this, SLOT(on_getItemXY()));
}

void MonitoringScene::set_Size(int w, int h)
{
    QPointF pos = sceneRect().bottomRight() - _sheetMaxSize->sceneBoundingRect().bottomRight()
            + QPointF(w - sceneRect().width(), h - sceneRect().height());

    _sheetMaxSize->setPos(pos);
}

void MonitoringScene::setInitialize()
{
    qDebug()<<"viewList.count():"<<MonitorBlackStage::instance()->getViewList().count()
           <<"viewNumb:"<<MonitorBlackStage::instance()->getViewNum();

    qDebug()<<"MonitorBlackStage::instance()->getDataList().count():"<<MonitorBlackStage::instance()->getDataList().count()
           <<"  getViewList().count(): "<<MonitorBlackStage::instance()->getViewList().count()
          <<"   getOpenViewList().count():"<<MonitorBlackStage::instance()->getOpenViewList().count();

    for(int i = 0; i < MonitorBlackStage::instance()->getDataList().count(); i++){
//                    qDebug()<<"&(MonitorBlackStage::instance()->getDataList().at(i).view)"<<&(MonitorBlackStage::instance()->getDataList().at(i).view)
//                           <<"   "<<&(MonitorBlackStage::instance()->getViewList().at(0));
//            qDebug()<<"getSameView(i):"<<MonitorBlackStage::instance()->getSameView(i);

//            if(MonitorBlackStage::instance()->getSameView(i))
            {
                CfgType monitorType      = MonitorBlackStage::instance()->getDataList().at(i).cfgType;
                QString monitorName      = MonitorBlackStage::instance()->getDataList().at(i).monitorName;
                QPointF scenePos;
                scenePos.setX(MonitorBlackStage::instance()->getDataList().at(i).x);
                scenePos.setY(MonitorBlackStage::instance()->getDataList().at(i).y);
                qDebug()<<"monitorItem:"/*<<monitorItem*/<<" monitorType:"<<monitorType<<" monitorName:"<<monitorName<<" scenePos:"<<scenePos;
                MonitorItem *monitorItem = MonitoItemFactory::CreateInstance(CfgType(monitorType), monitorName, scenePos, 50, 50);

                if(monitorItem != NULL){
                    addItem(monitorItem);
                    MonitorBlackStage::instance()->setID(MonitorBlackStage::instance()->getDataList().at(i), monitorItem, _view);
                }
            }
        }
        MonitorBlackStage::instance()->updateOpenData();
        MonitorBlackStage::instance()->delViewList(0);
//    }
}

void MonitoringScene::open(QString fileName,int index)
{
    QSettings *monitorIni = new QSettings(fileName, QSettings::IniFormat, 0);

    monitorIni->beginGroup(MONITOR_GROUP);

    monitorIni->beginGroup(QString(MONITOR_INFOMATION_GROUP));
    quint32 groupCount = monitorIni->value(MONITOR_GROUP_COUNT).toUInt();
    QSizeF size = monitorIni->value(MONITOR_SCENESIZE).toSizeF();
    monitorIni->endGroup();

    set_Size(size.width(), size.height());

//    for(quint32 i = 1; i <= groupCount; i++)
    {
        monitorIni->beginGroup(QString(MONITOR_INFOMATION_GROUP));
        quint32 monCount = monitorIni->value(QString(MONITOR_VIEW_DEV_COUNT).arg(index)).toUInt();
        monitorIni->endGroup();
        monitorIni->beginGroup(QString(MONITOR_VIEW_GROUP).arg(index));

        qDebug()<<"quint32 groupCount"<<groupCount<<"  "
               <<"quint32 monCount :"<<monCount<<"   "<<"QSizeF size :"<<size;
        foreach(QGraphicsItem *item, items())
        {
            this->removeItem(item);
        }

        QProgressDialog *progressDlg=new QProgressDialog;//关于缓慢操作的一个进度反馈
        progressDlg->setWindowModality(Qt::ApplicationModal);//模态窗口
        progressDlg->setMinimumDuration(1);//以1秒的延时进入进度条
        progressDlg->setWindowTitle(tr("please wait"));
        progressDlg->setLabelText(tr("Loading file......"));
        progressDlg->setCancelButton(0);//取消按钮不会被显示
        progressDlg->setRange(0,monCount-1);//设置进度对话框的最小值和最大值
        int proCount = 0;

        for(quint32 j = 1; j <= monCount; j++)
        {
            progressDlg->setValue(proCount++);
            monitorIni->beginGroup(QString(MONITOR_DEV_GROUP).arg(j));
            int monitorType     = monitorIni->value(QString(MONITOR_DEV_TYPE)).toInt();
            QPointF scenePos    = monitorIni->value(QString(MONITOR_DEV_POS)).toPointF();
            QString monitorName = monitorIni->value(QString(MONITOR_DEV_NAME)).toString();
            QString monitorIp   = monitorIni->value(QString(MONITOR_DEV_DEVIP)).toString();
            QString monitorPort = monitorIni->value(QString(MONITOR_DEV_PORT)).toString();
            qDebug()<<"monitorType:"<<monitorType<<"  "<<"scenePos:"<<scenePos<<"  "<<"monitorName:"<<monitorName<<" "
                   <<"mopnitorIP:"<<monitorIp<<"  "<<"monitorPort:"<<monitorPort;
            monitorIni->endGroup();
            MonitorItem *monitorItem = MonitoItemFactory::CreateInstance(CfgType(monitorType),
                                                                         QString("monitor_%1").arg(++_configIndex), scenePos, 50, 50);
            qDebug()<<"monitorItem create success"<<monitorItem;
            monitorItem->setMonitorName(monitorName);
            addItem(monitorItem);

            MonitorBlackStage::instance()->getMonitorData(monitorItem, _view, monitorItem->getType(), Dev_Type_Unknow, monitorIp, monitorPort);
        }
        monitorIni->endGroup();
    }
    monitorIni->endGroup();
    monitorIni->endGroup();
    delete monitorIni;
    emit sendSaveclick(true);
}

void MonitoringScene::on_deleteClicked()
{
    MonitorItem *monitorItem = NULL;
    QList<QGraphicsItem *> items = selectedItems();
    if(!items.isEmpty())
    {
        for(int i = items.count()-1; i >= 0; i--)
        {
            removeItem(items.at(i));
            monitorItem = qgraphicsitem_cast<MonitorItem *>(items.at(i));
            qDebug()<<__FUNCTION__<<__LINE__;
            MonitorBlackStage::instance()->removeData(monitorItem);
            if(monitorItem->getType() == CFG_TYPE_NETWORKDEVICE){
                MonitorBlackStage::instance()->removeOnlineDev_IP_Type(monitorItem->getDevIP());
                qDebug()<<__FUNCTION__<<__LINE__<<monitorItem->getNetworkDevType();
            }
//            items.removeLast();
        }
        emit sendSaveclick(false);
    }
}

void MonitoringScene::on_SetProperty()
{
    if(selectedItems().count() == 1)
    {
        _monitorItem = NULL;
        _monitorItem = qgraphicsitem_cast<MonitorItem *>(selectedItems().at(0));
        MonitorProperty pro(_monitorItem, _view);
        pro.exec();
    }
}

void MonitoringScene::on_PushButtonClicked()
{
    QList<QGraphicsItem *>items = this->items();

    if(!items.isEmpty())
    {
        //////////////////select///////////////////////
        if(sender() == _selectAll){
            foreach(QGraphicsItem *item, items){
                item->setSelected(true);
            }
        }
        if(sender() == _selectAllMeter){
            foreach(QGraphicsItem *item, items){
                MonitorItem *mon = qgraphicsitem_cast<MonitorItem *>(item);
                if(mon == NULL)
                {
                    qDebug()<<"mon is NULL!";
                    return ;
                }
                if(mon->getType() == CFG_TYPE_1) { item->setSelected(true); }
            }
        }
        if(sender() == _selectAllClipper){
            foreach(QGraphicsItem *item, items){
                MonitorItem *mon = qgraphicsitem_cast<MonitorItem *>(item);
                if(mon == NULL)
                {
                    qDebug()<<"mon is NULL!";
                    return ;
                }
                if(mon->getType() == CFG_TYPE_2) { item->setSelected(true); }
            }
        }
        if(sender() == _selectAllButton){
            foreach(QGraphicsItem *item, items){
                MonitorItem *mon = qgraphicsitem_cast<MonitorItem *>(item);
                if(mon == NULL)
                {
                    qDebug()<<"mon is NULL!";
                    return ;
                }
                if(mon->getType() == CFG_TYPE_3) { item->setSelected(true); }
            }
        }
        if(sender() == _selectAllLabel){
            foreach(QGraphicsItem *item, items){
                MonitorItem *mon = qgraphicsitem_cast<MonitorItem *>(item);
                if(mon == NULL)
                {
                    qDebug()<<"mon is NULL!";
                    return ;
                }
                if(mon->getType() == CFG_TYPE_4) { item->setSelected(true); }
            }
        }
        ///////////////////////////scale//////////////////////////////

        if(sender() == _scale_25){
            _view->scale(1/_scaleView, 1/_scaleView);
            _scaleView = 0.25;
            _view->scale(_scaleView, _scaleView);
        }
        if(sender() == _scale_50){
            _view->scale(1/_scaleView, 1/_scaleView);
            _scaleView = 0.50;
            _view->scale(_scaleView, _scaleView);
        }
        if(sender() == _scale_75){
            _view->scale(1/_scaleView, 1/_scaleView);
            _scaleView = 0.75;
            _view->scale(_scaleView, _scaleView);
        }
        if(sender() == _scale_100){
            _view->scale(1/_scaleView, 1/_scaleView);
            _scaleView = 1;
            _view->scale(_scaleView, _scaleView);
        }
        if(sender() == _scale_125){
            _view->scale(1/_scaleView, 1/_scaleView);
            _scaleView = 1.25;
            _view->scale(_scaleView, _scaleView);
        }
        if(sender() == _scale_150){
            _view->scale(1/_scaleView, 1/_scaleView);
            _scaleView = 1.50;
            _view->scale(_scaleView, _scaleView);
        }
        if(sender() == _scale_175){
            _view->scale(1/_scaleView, 1/_scaleView);
            _scaleView = 1.75;
            _view->scale(_scaleView, _scaleView);
        }
        if(sender() == _scale_200){
            _view->scale(1/_scaleView, 1/_scaleView);
            _scaleView = 2;
            _view->scale(_scaleView, _scaleView);
        }

    }/*else{
        QMessageBox::information(NULL, tr("Warning"), tr("No monitoring aids!"), QMessageBox::Ok);
    }*/
    ////////////////////DisplayStyle////////////////////
    if(sender() == _display1)
    {
        foreach(QGraphicsItem *item, selectedItems()){
            MonitorItem *monitorItem = qgraphicsitem_cast<MonitorItem *>(item);
            if(monitorItem->getType() == CFG_TYPE_1){ monitorItem->setDisplayStyle(1); }
            if(monitorItem->getType() == CFG_TYPE_2){ monitorItem->setDisplayStyle(1); }
            if(monitorItem->getType() == CFG_TYPE_3){ monitorItem->setDisplayStyle(1); }
            if(monitorItem->getType() == CFG_TYPE_5){ monitorItem->setDisplayStyle(1); }
        }
    }
    if(sender() == _display2)
    {
        foreach(QGraphicsItem *item, selectedItems()){
            MonitorItem *monitorItem = qgraphicsitem_cast<MonitorItem *>(item);
            if(monitorItem->getType() == CFG_TYPE_1){ monitorItem->setDisplayStyle(2); }
            if(monitorItem->getType() == CFG_TYPE_2){ monitorItem->setDisplayStyle(2); }
            if(monitorItem->getType() == CFG_TYPE_3){ monitorItem->setDisplayStyle(2); }
            if(monitorItem->getType() == CFG_TYPE_5){ monitorItem->setDisplayStyle(2); }
        }
    }
    if(sender() == _display3)
    {
        foreach(QGraphicsItem *item, selectedItems()){
            MonitorItem *monitorItem = qgraphicsitem_cast<MonitorItem *>(item);
            if(monitorItem->getType() == CFG_TYPE_1){ monitorItem->setDisplayStyle(3); }
            if(monitorItem->getType() == CFG_TYPE_2){ monitorItem->setDisplayStyle(3); }
            if(monitorItem->getType() == CFG_TYPE_3){ monitorItem->setDisplayStyle(3); }
        }
    }
    if(sender() == _display4)
    {
        foreach(QGraphicsItem *item, selectedItems()){
            MonitorItem *monitorItem = qgraphicsitem_cast<MonitorItem *>(item);
            if(monitorItem->getType() == CFG_TYPE_1){ monitorItem->setDisplayStyle(4); }
            if(monitorItem->getType() == CFG_TYPE_2){ monitorItem->setDisplayStyle(4); }
            if(monitorItem->getType() == CFG_TYPE_3){ monitorItem->setDisplayStyle(4); }
        }
    }
    if(sender() == _display5)
    {
        foreach(QGraphicsItem *item, selectedItems()){
            MonitorItem *monitorItem = qgraphicsitem_cast<MonitorItem *>(item);
            if(monitorItem->getType() == CFG_TYPE_1){ monitorItem->setDisplayStyle(5); }
            if(monitorItem->getType() == CFG_TYPE_2){ continue; }
            if(monitorItem->getType() == CFG_TYPE_3){ monitorItem->setDisplayStyle(5); }
        }
    }
}

void MonitoringScene::on_getItemXY()
{
    QPointF pos;
    foreach(QGraphicsItem *item, items())
    {
        if(item->type() != MonitorItem::Type)
            continue;
        pos = item->scenePos();
        MonitorItem *monitorItem = qgraphicsitem_cast<MonitorItem *>(item);
        monitorItem->setScenePos(pos);
        qDebug()<<__FUNCTION__<<__LINE__<<pos;
    }
}

void MonitoringScene::SavePro(QString fileName)
{
    qDebug()<<"Save project:"<<fileName;
    if(fileName != NULL)
    {
        _view->setWindowTitle(fileName);
        QSettings *monitorIni = new QSettings(fileName, QSettings::IniFormat, 0);
        quint32 monCount = 0;
        quint32 groupCount = MonitorBlackStage::instance()->getViewNum();
//        monitorIni->remove(QString(MONITOR_GROUP));

        monitorIni->beginGroup(QString(MONITOR_GROUP));

        monitorIni->beginGroup(QString(MONITOR_VIEW_GROUP).arg(++groupCount));
        MonitorBlackStage::instance()->setViewNum(groupCount);
        foreach (QGraphicsItem *item, items()) {
            if(item->type() == MonitorItem::Type)
            {
                MonitorItem *monItem = qgraphicsitem_cast<MonitorItem *>(item);
                monitorIni->beginGroup(QString(MONITOR_DEV_GROUP).arg(++monCount));
                monitorIni->setValue(QString(MONITOR_DEV_TYPE), monItem->getType());
                monitorIni->setValue(QString(MONITOR_DEV_POS), monItem->scenePos());
                monitorIni->setValue(QString(MONITOR_DEV_NAME), monItem->getTypeName());

                for(int i = 0; i < MonitorBlackStage::instance()->getDataList().count(); i++)
                {
                    if(monItem == MonitorBlackStage::instance()->getDataList().at(i).ID)
                    {
                        monitorIni->setValue(QString(MONITOR_DEV_DEVIP), MonitorBlackStage::instance()->getDataList().at(i).devIP);
                        monitorIni->setValue(QString(MONITOR_DEV_PORT), MonitorBlackStage::instance()->getDataList().at(i).Port);
                    }
                }
//                if(MonitorGroupType(monItem->getType()>>CFG_TYPE_2_GROUP_TYPE) == MONITOR_GROUP_LABEL)
//                {
//                    MonitorItem_meter *monItemMeter = qgraphicsitem_cast<MonitorItem_meter *>(monItem);
//                }
                monitorIni->endGroup();
            }
        }
        monitorIni->endGroup();

        monitorIni->beginGroup(QString(MONITOR_INFOMATION_GROUP));
        monitorIni->setValue(QString(MONITOR_SCENESIZE), sceneRect().size());
        monitorIni->setValue(QString(MONITOR_VIEW_DEV_COUNT).arg(groupCount),monCount);
        monitorIni->setValue(QString(MONITOR_GROUP_COUNT), groupCount);
        monitorIni->endGroup();
        monitorIni->endGroup();

        delete monitorIni;
        emit sendSaveclick(true);
        MainWindow::instance()->showNotice(tr("Save to %1 successed!").arg(fileName));
    }else{
        MainWindow::instance()->showNotice(tr("fileName is NULL, Save failed"));
        emit sendSaveclick(false);
    }
}

void MonitoringScene::saveItem()
{
    qDebug()<<"items().count():"<<items().count();
    bool save = false;
    int num = 0;
    foreach(QGraphicsItem *item, items()){
        if(item->type() == MonitorItem::Type){
            MonitorItem *monitorItem = qgraphicsitem_cast<MonitorItem *>(item);
            qDebug()<<__FUNCTION__ << __LINE__ <<"  monitorItem:"<<monitorItem<<" _view:"<<&_view<<" monitorItem->getType:"<<monitorItem->getType();

            for(int i = 0; i < MonitorBlackStage::instance()->getDataList().count(); i++)
            {
                save = false;
                if(MonitorBlackStage::instance()->getDataList().at(i).ID == monitorItem){
                    save =true;
                    qDebug()<<"save is true!";
                    break;
                }
            }
            if(!save){
                MonitorBlackStage::instance()->getMonitorData(monitorItem, _view, monitorItem->getType());
            }
        }
    }
    MonitorBlackStage::instance()->setViewNum(MonitorBlackStage::instance()->getViewList().count());
    qDebug()<<__FUNCTION__<<__LINE__<<MonitorBlackStage::instance()->getViewNum()<<"999999999999_viewList:"<<MonitorBlackStage::instance()->getViewList().count();
//    MonitorBlackStage::instance()->clearViewList();
    qDebug()<<"_openviewList.count():"<<MonitorBlackStage::instance()->getOpenViewList().count();
    MonitorBlackStage::instance()->upDateOnlineDev();
}


