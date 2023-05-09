#include "stable.h"
#include "absgraphscene.h"
#include <QMdiSubWindow>
#include "designcommands.h"
#include "Frame/configview.h"
#include "Frame/configscene.h"
#include "projectscene.h"
#include "projectview.h"
#include "sceneresizebox.h"
#include "deviceItem/deviceitemfactory.h"
#include "device/devicemanager.h"
#include "Frame/mainwindow.h"
#include "configItem/configitemfactory.h"
#include "deviceItem/deviceItem.h"
#include "linelabel.h"
#include "customWidget/labelediter.h"
#include <QGraphicsProxyWidget>
#include "configItem/configitemsnapshot.h"
#include "customWidget/CustomGraphicsItem/controltemplate.h"
#include <QTimer>
#include "../serpentineconnector.h"
#include "device/device_trn.h"

AbsGraphScene::AbsGraphScene(AbsGraphView *pView) :
    QGraphicsScene()
{
    _view = pView;
    _undoStack = new QUndoStack(this);
    _labelEditProxy = NULL;
    connect(this, SIGNAL(sceneRectChanged(QRectF)), MainWindow::instance(), SLOT(setSheetSize(QRectF)));
    connect(this, SIGNAL(focusItemChanged(QGraphicsItem*, QGraphicsItem*, Qt::FocusReason)), this, SLOT(onFocusItemChanged(QGraphicsItem*, QGraphicsItem*, Qt::FocusReason)));
    connect(MainWindow::instance(), SIGNAL(onSkinChanged()), this, SLOT(onSkinChanged()));

    setBackgroundBrush(QBrush(Config::curSkinStyle()->SceneFillColor));
    _needSave = false;
    setSceneRect(0, 0, 1400, 900);
    _tempGroup = new QGraphicsItemGroup();
    _toolkitType = ToolkitType_invalid;

    //对齐线
    _hAlignLine = new QGraphicsLineItem();
    _hAlignLine->hide();
    _hAlignLine->setPen(QPen(QBrush(Qt::green), 1));
    _hAlignLine->setZValue(SELECT_ZVALUE + 1);
    addItem(_hAlignLine);
    _vAlignLine = new QGraphicsLineItem();
    _vAlignLine->hide();
    _vAlignLine->setPen(QPen(QBrush(Qt::green), 1));
    _vAlignLine->setZValue(SELECT_ZVALUE + 1);
    addItem(_vAlignLine);

    //右键菜单
    _delAction = new QAction(tr("delete"), this);

    _selectAllMenu = new QMenu(tr("Select all"));
    _selectAllItem = new QAction(tr("Device"), this);
    _selectAllLink = new QAction(tr("Links"), this);
    _selectAllObject = new QAction(tr("Objects"), this);
    connect(_delAction, SIGNAL(triggered()), this, SLOT(onItemsDelete()));

    //选择菜单
    connect(_selectAllItem, SIGNAL(triggered()), this, SLOT(onBaseActionTriggered()));
    connect(_selectAllLink, SIGNAL(triggered()), this, SLOT(onBaseActionTriggered()));
    connect(_selectAllObject, SIGNAL(triggered()), this, SLOT(onBaseActionTriggered()));
}

AbsGraphScene::~AbsGraphScene()
{
}

//初始化环境菜单和动作
void AbsGraphScene::initCommonMenuActions()
{
    _contextMenu.addSeparator();
    _contextMenu.addMenu(_selectAllMenu);
    _selectAllMenu->addAction(_selectAllItem);
    _selectAllMenu->addAction(_selectAllLink);
    _selectAllMenu->addAction(_selectAllObject);
}

void AbsGraphScene::updateMenuActions()
{
    _delAction->setVisible(false);

    if (focusItem() != NULL)
    {
        int curItemType = focusItem()->type();
        if(curItemType == DesignItem::Type || curItemType == LineItem::Type)
        {
            _delAction->setVisible(true);
        }
    }
}

void AbsGraphScene::setToolkitType(AbsGraphScene::ToolkitType type)
{
    _toolkitType = type;
    if(type != ToolkitType_invalid)
        _view->viewport()->setCursor(Qt::CrossCursor);
    else
        _view->viewport()->setCursor(Qt::ArrowCursor);
}

#include "customWidget/CustomGraphicsItem/controltemplate.h"
#include "customWidget/CustomGraphicsItem/graphicsitem.h"
#include "customWidget/CustomGraphicsItem/graphicstextitem.h"
#include "customWidget/CustomGraphicsItem/graphicsgroupboxitem.h"
#include "customWidget/CustomGraphicsItem/graphicspolylineitem.h"
void AbsGraphScene::createGraphicsItem()
{
    switch(getToolkitType()) {
    case ToolkitType_text:
    {
        QPainterPath path; path.addRect(_view->getRubberBandRectToScene());
        if(path.isEmpty()) {
            onEscapeCreateGraphicsItem(); break;
        }

        GraphicsTextItem *textItem = new GraphicsTextItem(path);
        addItem(textItem);
        textItem->setSelected(true);
        MainWindow::instance()->setModelDockBoxToolkitBtnStatus(false);
        break;
    }
    case ToolkitType_GroupBox:
    {
        QPainterPath path; path.addRect(_view->getRubberBandRectToScene());
        if(path.isEmpty()) {
            onEscapeCreateGraphicsItem(); break;
        }

        GraphicsGroupBoxItem *groupBoxItem = new GraphicsGroupBoxItem(path);
        addItem(groupBoxItem);
        groupBoxItem->setSelected(true);
        MainWindow::instance()->setModelDockBoxToolkitBtnStatus(false);
        break;
    }
    case ToolkitType_Polyline:
    {
        if(!_grapPathItem->scene()) {
            _pPath.moveTo(_view->getMousePressPointToScene());
            _grapPathItem->setPath(_pPath);
            addItem(_grapPathItem);
        }else if(_pPath.elementCount() > 2
                 && qAbs(_pPath.elementAt(0).x-_view->getMousePressPointToScene().x()) <= 5
                 && qAbs(_pPath.elementAt(0).y-_view->getMousePressPointToScene().y()) <= 5) {
            _pPath.lineTo(_pPath.elementAt(0).x, _pPath.elementAt(0).y);
//            if(_pPath.isEmpty()) {
//                onEscapeCreateGraphicsItem(); break;
//        }

            graphicsPolylineItem *polylineItem = new graphicsPolylineItem(_pPath);
            addItem(polylineItem);
            polylineItem->setSelected(true);
            MainWindow::instance()->setModelDockBoxToolkitBtnStatus(false);
            _pPath.clear();
            removeItem(_grapPathItem);
        }else {
            _pPath.lineTo(_view->getMousePressPointToScene());
            return;
        }
        break;
    }
    default: break;
    }


    if(!rectItem) {
        rectItem = new ControlTemplate;
        addItem(rectItem);
    }
    if(rectItem->scene() != this) {
        addItem(rectItem);
    }
    QList<QGraphicsItem *> list;
    foreach(QGraphicsItem *item, selectedItems()) {
        if(item->type() == GraphicsItem::Type) {
            list.append(item);
        }
    }
    rectItem->clear();
    if(list.length()) {
        rectItem->addIsSelectItemList(list);
        rectItem->setCustomRect();
        rectItem->setVisible(true);
        rectItem->setSelected(true);
    }else {
        rectItem->setSelected(false);
        rectItem->setVisible(false);
    }
}

void AbsGraphScene::onBaseActionTriggered()
{
    if (sender() == _selectAllItem)
    {
        for (QGraphicsItem *item : items())
        {
            if (item->type() == DesignItem::Type)
            {
                item->setSelected(true);
            }
        }
    }
    else if (sender() == _selectAllLink)
    {
        for (QGraphicsItem *item : items())
        {
            if (item->type() == LineItem::Type)
            {
                item->setSelected(true);
            }
        }
    }
    else if (sender() == _selectAllObject)
    {
        for (QGraphicsItem *item : items())
        {
            if (item->type() == DesignItem::Type
                    || item->type() == LineItem::Type)
            {
                item->setSelected(true);
            }
        }
    }
}



bool AbsGraphScene::checkDeviceLink(ItemPort *startPort, ItemPort *endPort)
{
    //同一个设备
    if (startPort->parentItem() == endPort->parentItem())
    {
        return false;
    }

    //端口不能使用
    if(!endPort->enable())
    {
        return false;
    }

    //输入输出配对
    if(startPort->isInput() == endPort->isInput())
    {
        return false;
    }

    /* 输入输出端口不匹配 */
    int inType = startPort->getPortType();
    if (inType ==  endPort->getPortType() &&
        (inType == PortType_Dante || inType == PortType_AES67 || inType == PortType_Config))
        return true;
    else
        return false;
}

void AbsGraphScene::autoFitSceneSize() //模块变动后自动调整场景大小
{
//    QRectF itemsRc = itemsBoundingRect();
//    int width = itemsRc.x() + itemsRc.width()+400;
//    int height = itemsRc.y() + itemsRc.height() + 300;
//    if(width < 1400) width= 1400;
//    if(height < 900) height = 900;
//    setSceneRect(0, 0, width, height);
    QRectF pageRec = QRectF(sceneRect().topLeft(), QSizeF(SceneRectLimit_W, SceneRectLimit_H));
    QList<QGraphicsItem *> pageItems = items(pageRec, Qt::ContainsItemBoundingRect);
    if(pageItems.isEmpty()) return ;
    setSceneRect(getItemsBoundingRect(sceneRect().topLeft(), pageItems));
}

void AbsGraphScene::setFitPos(QGraphicsItem *item, QPointF& scenePos)
{
    //考虑到放大缩小，先在view坐标选择合适的位置，然后转化为scene坐标
    QRectF itemTempRc = item->boundingRect();
    itemTempRc.moveTo(scenePos);
    QRectF itemRc = _view->mapFromScene(itemTempRc).boundingRect();
    QRect viewRc = _view->rect();
    //修正滚动条大小，使viewRc为可见范围
    if(_view->horizontalScrollBar()->isVisible()){
        viewRc.adjust(0, 0, -_view->horizontalScrollBar()->height(), 0);
    }
    if(_view->verticalScrollBar()->isVisible()){
        viewRc.adjust(0, 0, 0, -_view->verticalScrollBar()->width());
    }

    //选择合适的显示位置
    if(itemRc.width() > viewRc.width()) {
        itemRc.setLeft(0);
    }
    else if(itemRc.right() > viewRc.right()){
        itemRc.setLeft(itemRc.left() - (itemRc.right() - viewRc.right()));
    }
    if(itemRc.height() > viewRc.height()) {
        itemRc.setTop(0);
    }
    else if(itemRc.bottom() > viewRc.bottom()){
        itemRc.setTop(itemRc.top() - (itemRc.bottom() - viewRc.bottom()));
    }

    //转化到场景坐标并设置新位置
    QPointF newPos = _view->mapToScene(itemRc.left(), itemRc.top());
    item->setPos(newPos);
}


QPointF AbsGraphScene::getCursorScenePos()
{
    return _view->mapToScene(_view->mapFromGlobal(QCursor::pos()));
}


QPointF AbsGraphScene::drawAlignLine(QGraphicsItem* moveItem, bool excludeSelected)
{
    QPointF cPos = getCursorScenePos();
    if(moveItem == NULL) return cPos;

    QRectF rcBound = moveItem->boundingRect().adjusted(0.5, 0.5, -0.5, -0.5);
    qreal dx1, dx2, dy1, dy2;
    QPointF itemPos = QPointF(0,0);
    QVariant var = moveItem->data(DataKey_PressedItemPos);
    if(!var.isNull()) {
        itemPos = var.value<QPointF>();
    }

    dx1 = -itemPos.x();
    dx2 = rcBound.width() - itemPos.x() ;
    dy1 = -itemPos.y();
    dy2 = rcBound.height() - itemPos.y() ;


    QLineF hLine(0, 0, 10000, 10000);
    QLineF vLine(0, 0, 10000, 10000);
    QPointF newPos = cPos;
    QRect viewRect = _view->viewport()->rect();
    QRectF sceneViewRect = _view->mapToScene(viewRect).boundingRect();
    QList<QGraphicsItem *> itemList = items(sceneViewRect);
    for (QGraphicsItem* item : itemList)
    {
        if(item == moveItem || (item->type() != DesignItem::Type && item->type() != GraphicsItem::Type) || item->opacity() < 0.9 ) continue;
        if(excludeSelected && item->isSelected()) continue;
        QRectF rc;
        if(item->type() == DesignItem::Type) {
            DesignItem* dItem = qgraphicsitem_cast<DesignItem*>(item);
            rc = dItem->mapRectToScene(dItem->rect());
        }else if(item->type() == GraphicsItem::Type) {
            GraphicsItem* dItem = qgraphicsitem_cast<GraphicsItem*>(item);
            rc = dItem->mapRectToScene(dItem->boundingRect());
        }

        int ttDist = abs(rc.top() - (cPos.y() + dy1));
        int tbDist = abs(rc.top() - (cPos.y() + dy2));
        int tDist = qMin(ttDist, tbDist);
        int btDist = abs(rc.bottom() - (cPos.y() + dy1));
        int bbDist = abs(rc.bottom() - (cPos.y() + dy2));
        int bDist = qMin(btDist, bbDist);
        if(tDist < ALIGN_DISTENCE)
        {
            QLineF line = getShortLine(true, rc.top(), rc.left(), rc.right(), cPos.x() + dx1,  cPos.x() + dx2);
            if(line.length() < hLine.length()){
                hLine = line;
                if(tDist == ttDist) newPos.setY(rc.top() - dy1);
                else newPos.setY(rc.top() - dy2);
            }
        }
        else if( bDist < ALIGN_DISTENCE)
        {
            QLineF line = getShortLine(true, rc.bottom(), rc.left(), rc.right(), cPos.x() + dx1,  cPos.x() + dx2);
            if(line.length() < hLine.length()){
                hLine = line;
                if(bDist == btDist) newPos.setY(rc.bottom() - dy1);
                else newPos.setY(rc.bottom() - dy2);
            }
        }

        int llDist = abs(rc.left() - (cPos.x() + dx1));
        int lrDist = abs(rc.left() - (cPos.x() + dx2));
        int lDist = qMin(llDist, lrDist);
        int rlDist = abs(rc.right() - (cPos.x() + dx1));
        int rrDist = abs(rc.right() - (cPos.x() + dx2));
        int rDist = qMin(rlDist, rrDist);
        if(lDist < ALIGN_DISTENCE)
        {
            QLineF line = getShortLine(false, rc.left(), rc.top(), rc.bottom(), cPos.y() + dy1,  cPos.y() + dy2);
            if(line.length() < vLine.length()){
                vLine = line;
                if(lDist == llDist) newPos.setX(rc.left() - dx1);
                else newPos.setX(rc.left() - dx2);
            }
        }
        else if(rDist < ALIGN_DISTENCE)
        {
            QLineF line = getShortLine(false, rc.right(), rc.top(), rc.bottom(), cPos.y() + dy1,  cPos.y() + dy2);
            if(line.length() < vLine.length()){
                vLine = line;
                if(rDist == rlDist) newPos.setX(rc.right() - dx1);
                else newPos.setX(rc.right() - dx2);
            }
        }
   }

    if(hLine.length() < 1000)
    {
        _hAlignLine->setLine(hLine);
        _hAlignLine->show();
    }
    else
    {
        _hAlignLine->hide();
    }
    if(vLine.length() < 1000)
    {
        _vAlignLine->setLine(vLine);
        _vAlignLine->show();
    }
    else
    {
        _vAlignLine->hide();
    }
    return newPos;
}

QList<ItemPort*> AbsGraphScene::getStartItemPortsForLine()
{
    QList<ItemPort*> portList;
    ItemPort* p0 = qgraphicsitem_cast<ItemPort*>(focusItem());
    if(p0 == NULL) return portList;
    for (QGraphicsItem* item : selectedItems())
    {
        if(item->type() != ItemPort::Type) continue;
        ItemPort* p = qgraphicsitem_cast<ItemPort*>(item);

        if(p->enable() && p->isInput() == p0->isInput()
//                 && p->parentItem() == p0->parentItem() //可用跨控件
                && p->getPortType() == p0->getPortType() )
        {
            int y0 = p->scenePos().y();
            int i = 0;
            for( ; i< portList.count(); i++)
            {
                if( y0 < portList[i]->scenePos().y())
                    break;
            }
            portList.insert(i, p);
        }
    }
    if(portList.count() > 0) {
        setFocusItem(portList[0]);
    }
    return portList;
}

QList<ItemPort*> AbsGraphScene::getStartItemPortsForLabel()
{
    QList<ItemPort*> portList;
    QGraphicsItem* pItem = NULL;
    bool isValid = true;
    bool isInput = false;
    DevPortType portType;
    for (QGraphicsItem* item : selectedItems())
    {
        if(item->type() != ItemPort::Type)
        {
            isValid = false;
            break;
        }
        ItemPort* p = qgraphicsitem_cast<ItemPort*>(item);
        if(pItem == NULL)
        {
            pItem = p->parentItem();
            isInput = p->isInput();
            portType = p->getPortType();
        }
        if(p->enable() == false || p->parentItem() != pItem ||
            p->getPortType() != portType || p->isInput() != isInput)
        {
            isValid = false;
            break;
        }
        else
        {
            int y0 = p->scenePos().y();
            int i = 0;
            for( ; i< portList.count(); i++)
            {
                if( y0 < portList[i]->scenePos().y())
                    break;
            }
            portList.insert(i, p);
        }
    }

    if(isValid && portList.count() > 0) {
        setFocusItem(portList[0]);
        return portList;
    }
    else {
        return QList<ItemPort*>();
    }
}

QList<ItemPort*> AbsGraphScene::getEndItemPorts(QPointF pos, int portType, bool isInput)
{
    QList<ItemPort*> portList;
    ItemPort* port0 = NULL;
    QRectF findRc(pos.x() -6, pos.y(), 12, 2);
    QList<QGraphicsItem *> itemList = items(findRc, Qt::IntersectsItemShape);
    for (QGraphicsItem* item : itemList)
    {
        ItemPort* p = qgraphicsitem_cast<ItemPort*>(item);
        if(p != NULL && p->getPortType() == portType && p->isInput() == isInput &&  p->enable())
        {
            port0 = p;
            break;
        }
    }
    if(port0 != NULL)
    {
        QRectF findRcLager(port0->scenePos().x()-8, port0->scenePos().y(), 26, height());
        QList<QGraphicsItem *> itemList = items(findRcLager, Qt::IntersectsItemShape);
        for (QGraphicsItem* item : itemList)
        {
            ItemPort* p = qgraphicsitem_cast<ItemPort*>(item);
            if(p != NULL && p->getPortType() == portType && p->isInput() == isInput &&  p->enable())
            {
                int y0 = p->scenePos().y();
                int i = 0;
                for( ; i< portList.count(); i++)
                {
                    if(y0 < portList[i]->scenePos().y()) break;

                }
                portList.insert(i, p);
            }
        }
    }

    return portList;
}


///////////////////////////////////////////////////////////////////////////////////////
/// \brief 鼠标事件
void AbsGraphScene::dropEvent(QGraphicsSceneDragDropEvent *e)
{
    if(_tempGroup->scene() != NULL && _tempGroup->childItems().count() > 0 )
    {
        if(_tempGroup->childItems().count() > 0)
        {
            QList<QGraphicsItem*> itemList = getTempGroupChild();
            _undoStack->push(new ItemsAddCommand(this, itemList));
        }
    }
    _hAlignLine->hide();
    _vAlignLine->hide();
}

void AbsGraphScene::dragEnterEvent(QGraphicsSceneDragDropEvent *e)
{
    //需要继承类实现
    assert(false);
}

void AbsGraphScene::dragMoveEvent(QGraphicsSceneDragDropEvent *e)
{
    if(_tempGroup->scene() != NULL)
    {
        QPointF newPos = drawAlignLine(_tempGroup, false);
        _tempGroup->setPos(newPos);
    }

    e->acceptProposedAction();
}

void AbsGraphScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *e)
{
    resetTempGroup();
}

void AbsGraphScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(_labelEditProxy != NULL && _labelEditProxy->isVisible()) {
        QPointF localPos = _labelEditProxy->mapFromScene(event->scenePos());
        if( !_labelEditProxy->contains(localPos))
            _labelEditProxy->hide();
    }
    QGraphicsScene::mousePressEvent(event);
}

void AbsGraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(_isDebuging)
    {
        QGraphicsItem* focItem = focusItem();
        if(focItem != NULL && focItem->type() == QGraphicsProxyWidget::Type) {
            QGraphicsScene::mouseMoveEvent(event);
        }
        return;
    }
    if(_tempGroup->scene() != NULL) //粘贴控件跟随鼠标,
    {
        QPointF newPos = drawAlignLine(_tempGroup, false);
        _tempGroup->setPos(newPos);
        return;
    }
    int itemType = -1;
    QGraphicsItem* focItem = focusItem();
    if(focItem != NULL) itemType = focItem->type();

    if(event->buttons().testFlag(Qt::MouseButton::LeftButton))
    {     
        if(itemType == DesignItem::Type || itemType == GraphicsItem::Type ||
                (itemType == ItemPort::Type && focItem->parentItem() == NULL)) //转折点
        {
            //忽略距离小于ALIGN_DISTENCE的移动
            QVariant var = focItem->data(DataKey_PressedScenePos);
            if(!var.isNull()) {
                QPointF sPos = var.value<QPointF>();
                QPointF moveDiff = event->scenePos() - sPos;

                if(abs(moveDiff.x())<ALIGN_DISTENCE && abs(moveDiff.y())<ALIGN_DISTENCE)
                    return;
                else
                    focItem->setData(DataKey_PressedScenePos, QVariant());
            }

            QPointF newPos = drawAlignLine(focItem, true);
            event->setScenePos(newPos);
            sendEvent(focItem, event);
            return;
        }
        else if(itemType == ItemPort::Type) //画线
        {
            QList<ItemPort*> portList = getStartItemPortsForLine();

            int portType = -1;
            bool isInput = false;
            if(portList.count() > 0)
            {
                portType= portList[0]->getPortType();
                isInput = !portList[0]->isInput();
            }

            QList<ItemPort*> ePortList = getEndItemPorts(event->scenePos(), portType, isInput);
            for(int i = 0; i<portList.count(); i++)
            {
                ItemPort* port = portList[i];
                LineItem *line = NULL;
                QVariant var = port->data(DataKey_TempLineItem);
                if(var.isNull()) {
                    line = new LineItem(this, port, NULL);
                    var.setValue(line);
                    port->setData(DataKey_TempLineItem, var);
                }
                else {
                    line = var.value<LineItem*>();
                }

                if(line->scene() == NULL) addItem(line);

                if(i< ePortList.count()) { line->setEndPort(ePortList[i]); }
                else{
                    if(ePortList.count() > 0) removeItem(line);  //移除多余的线
                    else line->setEndPort(NULL);
                }
                line->update();
            }
        }
    }
    else { //没有鼠标左键的移动
        if(itemType == ItemPort::Type) //画线
        {
            QList<ItemPort*> portList = getStartItemPortsForLine();

            for(int i = 0; i<portList.count(); i++)
            {
                ItemPort* port = portList[i];
                QVariant var = port->data(DataKey_TempLineItem);
                if(!var.isNull()){
                    LineItem* line = var.value<LineItem*>();
                    removeItem(line);
                    port->setData(DataKey_TempLineItem, QVariant());
                }
            }
        }

    }
    //自定义多边形模块 多点绘制辅助线
    if(getToolkitType() == ToolkitType_Polyline) {
        if(!_grapPathItem) {
            _grapPathItem = new QGraphicsPathItem;
            _grapPathItem->setPen(QPen(Qt::green, 1));
        }
        if(_grapPathItem->scene()) {
            QPainterPath path(_pPath);
            path.lineTo(event->scenePos());
//            qDebug() << __FUNCTION__ << __LINE__ << path;
            _grapPathItem->setPath(path);
        }
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void AbsGraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        int itemType = -1;
        QGraphicsItem* focItem = focusItem();
        if(focItem != NULL)
        {
            itemType = focItem->type();
            if(itemType == DesignItem::Type) //控件移动
            {
                QVariant var = focItem->data(DataKey_OldScenePos);
                QPointF moveDiff;
                if(var.isNull()){
                    QPointF pressPoint = event->buttonDownScenePos(Qt::MouseButton::LeftButton);
                    moveDiff = event->scenePos() - pressPoint;
                }
                else{
                    moveDiff = focItem->scenePos() - var.value<QPointF>();
                }

                if(abs(moveDiff.x()) > 0 || abs(moveDiff.y()) > 0)
                {
                    _undoStack->push(new ItemsMoveCommand(this, selectedItems(), moveDiff));
                }

                focItem->setData(DataKey_PressedItemPos, QPointF(0, 0));
            }
            else if(itemType == ItemPort::Type) //添加连线并移除临时线
            {
                QList<ItemPort*> portList = getStartItemPortsForLine();
                QList<QGraphicsItem*> lineList;
                for (ItemPort* port : portList)
                {
                    QVariant var = port->data(DataKey_TempLineItem);
                    if(!var.isNull())
                    {
                        port->setData(DataKey_TempLineItem, QVariant());
                        LineItem *line = var.value<LineItem*>();
                        if(line->getEndPort() == NULL || line->scene() == NULL)
                        {
                            if(line->scene()){
                                removeItem(line);
                            }
                            delete line;
                        }
                        else
                        {
                            line->setNormalization();
                            lineList.append(line);
                        }
                    }
                }
                if(lineList.count()> 0)
                    _undoStack->push(new ItemsAddCommand(this, lineList));
            }
        }

        if(_tempGroup->scene() != NULL) //粘贴拷贝内容
        {
            QList<QGraphicsItem*> itemList = getTempGroupChild();
            _undoStack->push(new ItemsAddCommand(this, itemList));
        }
    }
    else//右键等同esc取消复制
    {
        onEscapePress();
    }
    _hAlignLine->hide();
    _vAlignLine->hide();

    //添加自定义绘制工具
    createGraphicsItem();
    if(focusItem() == NULL){
        MainWindow::instance()->initTreeProperty(NULL);
    }
    QGraphicsScene::mouseReleaseEvent(event);
}


void AbsGraphScene::addLineLabel(bool& skipSpace)
{
    skipSpace = false;
    if(_labelEditProxy != NULL && _labelEditProxy->isVisible())  //以显示跳过
        return;

    QList<ItemPort*> portList = getStartItemPortsForLabel();
    if(portList.isEmpty()) return;
    if(portList[0]->getPortType() == PortType_Inflection ||
       portList[0]->getPortType() == PortType_RS232 ) return;
    if(!portList[0]->data(DataKey_TempLineItem).isNull()) return;//hrp

    QList<LineLabel*> labelList;
    for(ItemPort* port : portList)
    {
        LineLabel* label = port->getLineLabel();
        if(label == NULL)
        {
            label = new LineLabel(port);
            if(port->parentItem()->type() == ConfigItem::Type)
                label->setUserZValue(qgraphicsitem_cast<ConfigItem *>(port->parentItem())->checkZValue());//hrp
            else if(port->parentItem()->type() == SerpentineConnector::Type)
                label->setUserZValue(qgraphicsitem_cast<SerpentineConnector *>(port->parentItem())->checkZValue());
            port->setLineLabel(label);
            _labelList.append(label);
        }
        labelList.append(label);
    }

    if(_labelEditProxy == NULL)
    {
        LabelEditer* widget = new LabelEditer(this);
        _labelEditProxy = addWidget(widget);
        _labelEditProxy->setZValue(LabelEditZVal);
    }

    LabelEditer* editer = qobject_cast<LabelEditer*>(_labelEditProxy->widget());
    QPointF pos0 = portList[0]->scenePos();
    QString oldName;
    if(labelList[0]->isVisible() && labelList[0]->scene() != NULL) {
        oldName = labelList[0]->name();
    }

    editer->init(oldName, pos0, labelList);
    _labelEditProxy->show();
    skipSpace = true;
    setNeedSaveState(true);
}

void AbsGraphScene::addSetLabelCommand(QList<LineLabel*> labelList, QString labelName)
{
    _undoStack->push(new LineLabelSetCommand(this, labelList, labelName));
}

void AbsGraphScene::matchLineLabels()
{
    QList<LineLabel*> lableInList;
    QList<LineLabel*> lableOutList;

    QMap<int, QList<QString>> extInPorts;
    QMap<int, QList<QString>> extOutPorts;
    int inExtPortNum = _extInOutPortNum[0];
    int OutExtPortNum = _extInOutPortNum[1];
    memset(_extInOutPortNum, 0, sizeof(int) * 2);
    for(LineLabel* label : _labelList)
    {
        if(!label) continue;
        for(int i = 0; i < label->getMatchOutLabelNum(); i++)
            label->getLabelPort()->setEnable(true);
        label->setMatchOutLabelNum(0);  //清空匹配个数
        label->setIsMatched(false); //清空匹配状态

        if(label->isVisible() == false || label->scene() == NULL)
            continue;
        if(label->isInput()){
            if(label->name().startsWith(Appoint)) {
                QList<QString> names = extInPorts.value(label->getPortType());
                extInPorts.insert(label->getPortType(), names << label->name());
                _extInOutPortNum[0]++;
            }else
                lableInList.append(label);
        }
        else{
            if(label->name().startsWith(Appoint)) {
                QList<QString> names = extOutPorts.value(label->getPortType());
                extOutPorts.insert(label->getPortType(), names << label->name());
                _extInOutPortNum[1]++;
            }else
                lableOutList.append(label);
        }
    }
    if(inExtPortNum != _extInOutPortNum[0] || OutExtPortNum != _extInOutPortNum[1])
        emit updateSolutionRoomPorts(_view->windowTitle(), extInPorts, extOutPorts);

    for(LineLabel *outLabel : lableOutList) {
        bool isMatched = false;
        int inLabelUsedNum = 0;
        for(LineLabel *inLabel : lableInList) {
            if(inLabel->name() == outLabel->name()) {
                isMatched = true;
                inLabel->setMatchOutLabelNum(1);
                inLabel->setIsMatched(isMatched);
                inLabel->getLabelPort()->setEnable(false);
                outLabel->getLabelPort()->setEnable(false);
                ++inLabelUsedNum;
            }
        }
        outLabel->setIsMatched(isMatched);
        outLabel->setMatchOutLabelNum(inLabelUsedNum);
    }

    update();

//    for(LineLabel* inLabel : lableInList)
//    {
//        bool isMatched = false;
//        QList<LineLabel*> matchedLabels;
//        for(LineLabel* outLabel : lableOutList)
//        {
//            if(inLabel->name() == outLabel->name())
//            {
//                matchedLabels.append(outLabel);
//                isMatched = true;

//            }
//        }
//        inLabel->setIsMatched(isMatched);
//        for(LineLabel* outLabel : matchedLabels)
//        {
//            outLabel->setIsMatched(isMatched);
//        }
//    }

//    for(LineLabel* label : _labelList) {//更新匹配状态
//        label->update();
//    }
}

QList<LineLabel*> AbsGraphScene::getUnmatchedLabels()
{
    QList<LineLabel*> labelList;
    for(LineLabel* label : _labelList)
    {
        if(label->scene() == NULL) continue;
        if(label->isMatched() == false) labelList.append(label);
    }
    return labelList;
}

QList<LineLabel*> AbsGraphScene::filterLabels(DevPortType portType, bool isInput, const QString& text)
{
    QList<LineLabel*> labelList;
    for(LineLabel* label : _labelList)
    {
        if(label->scene() == NULL) continue;
        if(label->isInput() != isInput || label->getPortType() != portType
                || label->name().contains(text) == false)
            continue;
        if(isInput == true && label->isMatched()) continue;
        labelList.append(label);
    }
    return labelList;
}

bool AbsGraphScene::isLabelNameDuplicate(const QString& labelName)
{
    if(labelName.isEmpty())
        return false;
    for(LineLabel* label : _labelList)
    {
        if(label->scene() == NULL || label->isInput()) continue;
        if(labelName == label->name())
            return true;
    }
    return false;
}

QString AbsGraphScene::getNewLabelName(const QString& labelName)
{
    QString perfix = "label";
    int index = 1;
    int i = labelName.length()-1;
    for(; i>= 0; i-- ) {
        if(labelName[i] < '0' || labelName[i] >'9') {
            break;
        }
    }
    perfix = labelName.left(i + 1);
    index = labelName.right(labelName.length()-1 -i).toInt();
    QString newName = labelName;
    while(isLabelNameDuplicate(newName)) {
        newName = QString("%1%2").arg(perfix).arg(index++);
    }

    return newName;
}


void AbsGraphScene::addSnapBankController(QGraphicsItem *parentItem, QList<QGraphicsItem *> itemList)
{
    ConfigItemSnapshot *snapItem = qgraphicsitem_cast<ConfigItemSnapshot *>(parentItem);
    if(!snapItem) return ;
    if(snapItem->getCfgType() == CFG_TYPE_SNAPSHOT_BANK) {
        foreach(QGraphicsItem *item, itemList) {
            ConfigItem *cfgItem = qgraphicsitem_cast<ConfigItem *>(item);
            if(!snapItem->getFromSel_CtrlsList().contains(cfgItem))
                snapItem->insertFromSel_CtrlsList(cfgItem);
        }
    }
    emit designItemChanged(this);
}

void AbsGraphScene::delSnapBankController(QGraphicsItem *parentItem, QList<QGraphicsItem *> itemList)
{
    ConfigItemSnapshot *snapItem = qgraphicsitem_cast<ConfigItemSnapshot *>(parentItem);
    if(!snapItem) return ;
    if(snapItem->getCfgType() == CFG_TYPE_SNAPSHOT_BANK) {
        foreach(QGraphicsItem *item, itemList) {
            ConfigItem *cfgItem = qgraphicsitem_cast<ConfigItem *>(item);
            if(snapItem->getFromSel_CtrlsList().contains(cfgItem))
                snapItem->removeFromSel_CtrlsList(cfgItem);
        }
    }
    emit designItemChanged(this);
}

#include "deviceItem/deviceitem_trn.h"
#include "configscene.h"

void AbsGraphScene::addSerpentineConnector()
{
    QList<ItemPort *> portItemList = getStartItemPortsForLine();
    if(portItemList.isEmpty()) return;
    if(portItemList.at(0)->getPortType() != PortType_Config
            && portItemList[0]->getPortType() != PortType_CtrlPin
            && portItemList[0]->getPortType() != PortType_ChannelBus) return;

    SerConType serConType;
    SerpentineConnector *serCon = NULL;
//    qDebug() << __FUNCTION__ << __LINE__ << portItemList.at(0)->portName();
    if(portItemList[0]->getPortType() == PortType_ChannelBus) {
        DevPortType portType;
        int chCount = 0;
        QGraphicsItem *gItem = portItemList[0]->parentItem();
        if(gItem->type() != SerpentineConnector::Type) return ;
        SerpentineConnector *serConItem = qgraphicsitem_cast<SerpentineConnector *>(gItem);
        if(!serConItem) return ;
        if(portItemList.at(0)->isInput()) {
            serConType = SerConType_In;
            chCount = serConItem->getOutPortList().count();
            portType = serConItem->getOutPortList().at(0)->getPortType();
        }else {
            serConType = SerConType_Out;
            chCount = serConItem->getInPortList().count();
            portType = serConItem->getInPortList().at(0)->getPortType();
        }
        serCon = new SerpentineConnector(serConType, chCount, portType);
    }else {
        if(portItemList.at(0)->isInput()) {
            serConType = SerConType_Out;
        }else {
            serConType = SerConType_In;
        }
        serCon = new SerpentineConnector(
                    serConType, portItemList.count(), portItemList.at(0)->getPortType());
    }

    addItem(serCon);
    serCon->setUserZValue(qobject_cast<ConfigScene *>(serCon->scene())->getTrnDev()->getItemUserZValueNum(true));
    serCon->setPos(getCursorScenePos());
    serCon->conectingLine(portItemList);

    QList<QGraphicsItem *> itemList;
    itemList.append(serCon);

    _undoStack->push(new ItemsAddCommand(this, itemList));
}

bool lessThanX(QGraphicsItem * left, QGraphicsItem * right)
{
    return (left->x() < right->x());
}

bool lessThanY(QGraphicsItem * left, QGraphicsItem * right)
{
    return (left->y() < right->y());
}


void AbsGraphScene::sortByPosition(QList<DesignItem *>& items, bool isHorizontal)
{
    if(isHorizontal){
        std::sort(items.begin(), items.end(), lessThanX);
    }
    else{
        std::sort(items.begin(), items.end(), lessThanY);
    }
}

QString AbsGraphScene::getViewTitle()
{
    return _view->windowTitle();
}

bool AbsGraphScene::checkEditable()
{
    if(_isDebuging){
        MainWindow::instance()->showNotice(tr("Debug mode, can't edit"));
        return false;
    }
    return true;
}

void AbsGraphScene::focusSingleItemInRect(QRectF rect)
{
    QList<QGraphicsItem*> itemsInRect = items(rect, Qt::ItemSelectionMode::IntersectsItemShape);
    QList<QGraphicsItem*> itemList;
    for(QGraphicsItem* item : itemsInRect) {
        if(item->type() == DesignItem::Type || item->type() == GraphicsItem::Type){
            itemList.append(item);
        }
    }
    if(itemList.count() == 1){
        setFocusItem(itemList[0]);
        onFocusItemChanged(itemList[0], NULL, Qt::MouseFocusReason);
    }
}

void AbsGraphScene::deletePage(QString pageName)
{
    int idx = _pageList.indexOf(pageName);
    if(idx == -1) return ;
    //删除items
    QPointF topLeft(0, SceneRectLimit_H*idx);
    QRectF sRec = QRectF(topLeft, QSizeF(SceneRectLimit_W, SceneRectLimit_H));
    deleteGraphicsItems(items(sRec, Qt::ContainsItemBoundingRect));
    //移动items
    int y = _pageList.count()-1 - idx;
    sRec.setTopLeft(QPointF(0, SceneRectLimit_H*(idx+1)));
    sRec.setSize(QSizeF(SceneRectLimit_W, SceneRectLimit_H*y));

    QList<QGraphicsItem *> pageItems = items(sRec, Qt::ContainsItemBoundingRect);
    foreach (QGraphicsItem *item, pageItems) {
        if(item->type() == ItemPort::Type) continue;
        item->moveBy(0, -SceneRectLimit_H);
    }
    _pageList.removeAt(idx);
}

void AbsGraphScene::renamePage(QString oldPageName, QString newPageName)
{
    int idx = _pageList.indexOf(oldPageName);
    if(idx == -1) return ;
    _pageList.insert(idx, newPageName);
    _pageList.removeAt(idx+1);
}

void AbsGraphScene::setCurPage(QString pageName)
{
    _curPageName = pageName;
    int idx = _pageList.indexOf(pageName);
    if(idx == -1) return ;
    QPointF topLeft(0, SceneRectLimit_H*idx);
    QRectF sRec = QRectF(topLeft, QSizeF(SceneRectLimit_W, SceneRectLimit_H));
    setSceneRect(getItemsBoundingRect(topLeft, items(sRec, Qt::ContainsItemBoundingRect)));
}

void AbsGraphScene::setItemsAlign(AlignMode alignMode)
{
    if(!checkEditable()) return;
    QList<DesignItem *> itemList;
    QRectF boundRc;
    for(QGraphicsItem* item : selectedItems())
    {
        if(item->type() != DesignItem::Type) continue;
        DesignItem* designItem = qgraphicsitem_cast<DesignItem*>(item);
        itemList.append(designItem);
        QRectF itemRc = designItem->mapRectToScene(designItem->rect());
        if(boundRc.isEmpty()) {
            boundRc =itemRc;
        }
        else {
            boundRc = boundRc.united(itemRc);
        }
    }
    if(itemList.count() < 2) return;

    QList<QPointF> pointList;
    DesignItem* refItem =  qgraphicsitem_cast<DesignItem*>(focusItem());
    if(refItem == NULL) refItem = itemList[0];
    QRectF refRc =  refItem->mapRectToScene(refItem->rect());

    if(alignMode == AlignLeft)
    {
        sortByPosition(itemList, false);
        qreal posStart = boundRc.top();
        for(DesignItem* designItem : itemList)
        {
            QRectF rc = designItem->mapRectToScene(designItem->rect());
            pointList.append(QPointF(refRc.left() -rc.left(), posStart - rc.top()));
            posStart += rc.height();
        }
    }
    else if(alignMode == AlignRight)
    {
        sortByPosition(itemList, false);
        qreal posStart = boundRc.top();
        for(DesignItem* designItem : itemList)
        {
            QRectF rc = designItem->mapRectToScene(designItem->rect());
            pointList.append(QPointF(refRc.right() - rc.right(), posStart - rc.top()));
            posStart += rc.height();
        }
    }
    else if(alignMode == AlignTop)
    {
        sortByPosition(itemList, true);
        qreal posStart = boundRc.left();
        for(DesignItem* designItem : itemList)
        {
            QRectF rc = designItem->mapRectToScene(designItem->rect());
            pointList.append(QPointF(posStart - rc.left(), refRc.top() -rc.top()));
            posStart += rc.width();
        }
    }
    else  if(alignMode == AlignBottom)
    {
        sortByPosition(itemList, true);
        qreal posStart = boundRc.left();
        for(DesignItem* designItem : itemList)
        {
            QRectF rc = designItem->mapRectToScene(designItem->rect());
            pointList.append(QPointF(posStart - rc.left(), refRc.bottom() -rc.bottom()));
            posStart += rc.width();
        }
    }

    _undoStack->push(new ItemsMoveCommand(this, itemList, pointList));
}

void AbsGraphScene::setItemsEqSpace(AlignMode alignMode)
{
    QList<DesignItem *> itemList;
    int totalWidth = 0;
    int totalHeight = 0;
    for(QGraphicsItem* item : selectedItems())
    {
        int idx = 0;
        if(item->type() != DesignItem::Type) continue;
        DesignItem* designItem = qgraphicsitem_cast<DesignItem*>(item);
        if(alignMode == AlignEqHSpace) //按Left升序排序
        {
            for(; idx<itemList.count(); idx++)
            {
                if(designItem->scenePos().x() < itemList[idx]->scenePos().x()) break;
            }
            totalWidth += designItem->rect().width();
            itemList.insert(idx, designItem);
        }
        else if(alignMode == AlignEqVSpace) //按Top升序排序
        {
            for(; idx<itemList.count(); idx++)
            {
                if(designItem->scenePos().y() < itemList[idx]->scenePos().y()) break;
            }
            totalHeight +=  designItem->rect().height();
            itemList.insert(idx, designItem);
        }
    }
    if(itemList.count() < 3) return;

    QList<QPointF> pointList;
    if(alignMode == AlignEqHSpace)
    {
        int leftX = itemList.first()->scenePos().x();
        qreal space = itemList.last()->scenePos().x() + itemList.last()->rect().width() - leftX - totalWidth;
        for(int i = 0; i<itemList.count(); i++)
        {
            DesignItem* dItem = itemList[i];
            int newX = (int)(leftX + space* i/(itemList.count() -1));
            pointList.append(QPointF(newX - dItem->scenePos().x(), 0));
            leftX += dItem->rect().width();
        }
    }
    else if(alignMode == AlignEqVSpace)
    {
        int topY = itemList.first()->scenePos().y();
        qreal space = itemList.last()->scenePos().y() + itemList.last()->rect().height() - topY - totalHeight;
        for(int i = 0; i<itemList.count(); i++)
        {
            DesignItem* dItem = itemList[i];
            int newY = (int)(topY + space* i/(itemList.count() -1));
            pointList.append(QPointF(0, newY - dItem->scenePos().y()));
            topY += dItem->rect().height();
        }
    }
    _undoStack->push(new ItemsMoveCommand(this, itemList, pointList));
}

void AbsGraphScene::addGraphicsItems(QList<QGraphicsItem*> items)
{
    QGraphicsItem* itemToFocus = NULL;
    clearSelection();
    bool labelChanged = false;

    for(int i = 0; i< items.count(); i++)
    {
        QGraphicsItem* curItem = items[i];
        curItem->setOpacity(1);
        if(curItem->type() == DesignItem::Type)
        {
            if(itemToFocus == NULL) itemToFocus = curItem;
            DesignItem* devItem = qgraphicsitem_cast<DesignItem*>(curItem);
            if(devItem->scene() != this) { addItem(devItem); }
            devItem->initAfterAdd();
        }
        else if(curItem->type() == LineItem::Type)
        {
            LineItem* lineItem = qgraphicsitem_cast<LineItem*>(curItem);
            if(lineItem->scene() != this) { addItem(lineItem); }
            lineItem->initAfterAdd();
        }
        else if(curItem->type() == LineLabel::Type)
        {
            LineLabel* labelItem = qgraphicsitem_cast<LineLabel*>(curItem);
            if(labelItem->scene() != this) {labelItem->addToScene();}
            for(int i = 0; i < labelItem->getMatchOutLabelNum(); i++)
                labelItem->getLabelPort()->setEnable(false);
            labelChanged = true;
        }else if(curItem->type() == GraphicsItem::Type) {
            if(itemToFocus == NULL) itemToFocus = curItem;
            GraphicsItem *grapItem = qgraphicsitem_cast<GraphicsItem *>(curItem);
            if(grapItem->scene() != this) {addItem(grapItem);}

        }else if(curItem->type() == SerpentineConnector::Type) {
            SerpentineConnector *serItem = qgraphicsitem_cast<SerpentineConnector *>(curItem);
            if(serItem->scene() != this) {addItem(serItem);}
        }
    }
    emit designItemChanged(this);
    if(labelChanged){
        matchLineLabels();
    }

    for(QGraphicsItem* item : items) {
        item->setSelected(true);
    }
    //批量添加模块和拷贝多个模块时，不设置并刷新焦点Item
    if(items.count() == 1 && itemToFocus != NULL){
        setFocusItem(itemToFocus);
    }
    autoFitSceneSize();
    setNeedSaveState(true);
}

void AbsGraphScene::deleteGraphicsItems(QList<QGraphicsItem*> itemList)
{
    clearFocus();
    clearSelection();
    bool labelChanged = false;
    for(int i = 0; i< itemList.count(); i++)
    {
        int itemType = itemList[i]->type();
        if(itemType == DesignItem::Type)
        {
            DesignItem* desItem = qgraphicsitem_cast<DesignItem*>(itemList[i]);

//            ConfigItem* cfgItem = qgraphicsitem_cast<ConfigItem*>(itemList[i]);
            desItem->clearBeforeDelete();
            removeItem(itemList[i]);
        }
        else if(itemType == LineItem::Type)
        {
            LineItem* line = qgraphicsitem_cast<LineItem*>(itemList[i]);
            line->clearBeforeDelete();
            removeItem(itemList[i]);
        }
        else if(itemType == LineLabel::Type)
        {
            LineLabel* label = qgraphicsitem_cast<LineLabel*>(itemList[i]);
            for(int i = 0; i < label->getMatchOutLabelNum(); i++)
                label->getLabelPort()->setEnable(true);
            removeItem(label);
            labelChanged = true;
        }else if(itemType == GraphicsItem::Type) {
            GraphicsItem *grapItem = qgraphicsitem_cast<GraphicsItem *>(itemList[i]);
            removeItem(grapItem);
            foreach(QGraphicsItem *item, items()) {
                if(item->type() == ControlTemplate::Type) {
                    item->setSelected(false);
                    item->setVisible(false);
                }
            }
        }else if(itemList[i]->type() == SerpentineConnector::Type) {
            SerpentineConnector *serItem = qgraphicsitem_cast<SerpentineConnector *>(itemList[i]);
            removeItem(serItem);
        }
    }

    emit designItemChanged(this);
    if(labelChanged){
        matchLineLabels();
    }
    autoFitSceneSize();
    setNeedSaveState(true);
}

void AbsGraphScene::itemPropChanged(DesignItem* dItem, const char *propName, const QVariant& newVal, const QVariant& oldVal)
{
    _undoStack->push(new ItemPropChangedCommand(dItem, propName, newVal, oldVal));
    setNeedSaveState(true);
}

void AbsGraphScene::keyPressEvent(QKeyEvent *event)
{
    bool skipSpace = false;
    switch(event->key()) {
    case Qt::Key_Delete:
        onItemsDelete();
        break;
    case Qt::Key_C:
        if (event->modifiers() == Qt::ControlModifier)
            onItemsCopy();
        break;
    case Qt::Key_V:
        if (event->modifiers() == Qt::ControlModifier)
            onItemsPaste();
        break;
    case Qt::Key_S:
        if (event->modifiers() == Qt::ControlModifier)
            _view->saveScene();
    case Qt::Key_Escape:
        onEscapePress();
        onEscapeCreateGraphicsItem();
        break;
    case Qt::Key_Space:
        addLineLabel(skipSpace);
        addSerpentineConnector();
        break;
    default:
        break;
    }

    if(skipSpace) return; //如果是Space并用于触发Label编辑窗口，则跳过该按键消息
    QGraphicsScene::keyPressEvent(event);
}


QLineF AbsGraphScene::getShortLine(bool isHorizontal , qreal p, qreal p1, qreal p2, qreal p3, qreal p4)
{
    QLineF line1, line2, line3, line4;
    if(isHorizontal)
    {
        line1.setLine(p1-0.5, p-0.5, p3-0.5, p-0.5);
        line2.setLine(p1-0.5, p-0.5, p4-0.5, p-0.5);
        line3.setLine(p2-0.5, p-0.5, p3-0.5, p-0.5);
        line4.setLine(p2-0.5, p-0.5, p4-0.5, p-0.5);
    }
    else
    {
        line1.setLine(p-0.5, p1-0.5, p-0.5, p3-0.5);
        line2.setLine(p-0.5, p1-0.5, p-0.5, p4-0.5);
        line3.setLine(p-0.5, p2-0.5, p-0.5, p3-0.5);
        line4.setLine(p-0.5, p2-0.5, p-0.5, p4-0.5);
    }
    if(line1.length() > line2.length())
        line1 = line2;
    if(line1.length() > line3.length())
        line1 = line3;
    if(line1.length() > line4.length())
        line1 = line4;
    return line1;
}

QList<QGraphicsItem*> AbsGraphScene::getConnectedLinesOrLineLabels(DesignItem* dItem)
{
    QList<QGraphicsItem*> lines;
    for(QGraphicsItem* item : items())
    {
        if(item->type() == LineItem::Type)
        {
            LineItem* line = qgraphicsitem_cast<LineItem*>(item);
            if(line->isParentOfPort(dItem))
            {
                lines.append(line);
            }
        }
        else if(item->type() == LineLabel::Type) {
            LineLabel *lineLab = qgraphicsitem_cast<LineLabel *>(item);
//            qDebug() << __FUNCTION__ << __LINE__ << (lineLab->getLabelPort()->parentItem() == dItem);
            if(lineLab->getLabelPort()->parentItem() == dItem)
                lines.append(lineLab);
        }
    }
    return lines;
}


QList<QGraphicsItem*> AbsGraphScene::prepareItems(QList<QGraphicsItem*> sourceItems)
{
    QSet<QGraphicsItem*> itemSet;
    QMap<int, QGraphicsItem *> desItems;

    for(QGraphicsItem* item : sourceItems)
    {
        int itemType = item->type();
        if(itemType == LineItem::Type)
        {
            itemSet.insert(item);
        }
        else if(itemType == DesignItem::Type)
        {
            DesignItem* dItem = qgraphicsitem_cast<DesignItem*>(item);
            desItems.insert(dItem->checkZValue(), dItem);//hrp  保证在复制粘贴时模块堆叠层次不变
            QList<QGraphicsItem*> lines = getConnectedLinesOrLineLabels(dItem);
//            itemSet.insert(dItem);
            itemSet += QSet<QGraphicsItem *>(lines.begin(), lines.end());

        }
        else if(itemType == LineLabel::Type)
        {
            itemSet.insert(item);
        }
        else if(itemType == ItemPort::Type && item->parentItem() == NULL) //转折点
        {
            itemSet.insert(item);
        }else if(itemType == GraphicsItem::Type) {
            itemSet.insert(item);
        }else if(itemType == SerpentineConnector::Type) {
            SerpentineConnector *serConItem = qgraphicsitem_cast<SerpentineConnector *>(item);
            QList<QGraphicsItem *> lines = getConnectedLinesOrLineLabels(serConItem);
            itemSet.insert(serConItem);
            itemSet += QSet<QGraphicsItem *>(lines.begin(), lines.end());
        }
    }

    QList<QGraphicsItem *> itemList;

    itemList += QList<QGraphicsItem *>(desItems.begin(), desItems.end());
    itemList += QList<QGraphicsItem *>(itemSet.begin(), itemSet.end());//hrp  5.14之后，使用范围构造函数
//    return itemSet.toList();
    return itemList;
}

void AbsGraphScene::onItemsDelete()
{
    if(!checkEditable()) return;
    QList<QGraphicsItem *> items = selectedItems();
    if(items.count() > 0)
    {
        _undoStack->push(new ItemsDeleteCommand(this, items));
    }
}


void AbsGraphScene::onItemsCopy()
{
    if(!checkEditable()) return;
    if(focusItem() != NULL && focusItem()->type() == LineLabel::Type){
        _copiedLabel = qgraphicsitem_cast<LineLabel*>(focusItem());
    }
    else{
        _copiedLabel = NULL;
        _copyItems.clear();
        _copyItems.append(selectedItems());
    }
}


void AbsGraphScene::onItemsPaste()
{
    if(_copiedLabel != NULL)
    {
        if(focusItem() != NULL && focusItem()->type() == ItemPort::Type)
        {
            ItemPort* focusPort = qgraphicsitem_cast<ItemPort*>(focusItem());
            LineLabel* pasteLabel = new LineLabel(focusPort);
            _labelList.append(pasteLabel);
            QString newName = getNewLabelName(_copiedLabel->name());
            _undoStack->push(new LineLabelSetCommand(this, QList<LineLabel*>{pasteLabel}, newName));
        }
    }
    else
    {
        //判断正在框选
        QMap<QGraphicsItem*, QGraphicsItem*> itemMap;
        QList<QGraphicsItem *> /*devItems,*/ lineItems, grapCustomItems, serConItems;
        QMap<int, QGraphicsItem *> devItems;
        for (QGraphicsItem *srcItem : _copyItems) {
            if(srcItem->type() == DesignItem::Type) {
                DesignItem *dItem = qgraphicsitem_cast<DesignItem *>(srcItem); //wj2
                devItems.insert(dItem->checkZValue(), srcItem);
            }
            else if(srcItem->type() == LineItem::Type) {
                lineItems.append(srcItem);
            }else if(srcItem->type() == GraphicsItem::Type) {
                grapCustomItems.append(srcItem);
            }else if(srcItem->type() == SerpentineConnector::Type) {
                serConItems.append(srcItem);
            }
        }

        QMapIterator<int, QGraphicsItem *> iter(devItems);
        while(iter.hasNext()) {
            iter.next();
            QGraphicsItem *item = CopyGraphicsDesignItem(iter.value());
            if(item != NULL)
            {
                item->setOpacity(0.5);
                _tempGroup->addToGroup(item);
                itemMap.insert(iter.value(), item);
            }
        }

        foreach(QGraphicsItem *srcItem, serConItems) {
            SerpentineConnector *serItem = qgraphicsitem_cast<SerpentineConnector *>(srcItem);
            QGraphicsItem *sItem = serItem->clone(NULL);
            if(sItem) {
                sItem->setOpacity(0.5);
                _tempGroup->addToGroup(sItem);
                itemMap.insert(srcItem, sItem);
            }
        }

        for(QGraphicsItem *srcItem : lineItems) {
            QGraphicsItem *item = CopyGraphicsLineItem(srcItem, itemMap);
            if(item != NULL)
            {
                item->setOpacity(0.5);
                _tempGroup->addToGroup(item);
            }
        }

        foreach(QGraphicsItem *item, grapCustomItems) {
            GraphicsItem *grapItem = qgraphicsitem_cast<GraphicsItem *>(item);
            QGraphicsItem *gItem = grapItem->clone(NULL);
            if(gItem) {
                gItem->setOpacity(0.5);
                _tempGroup->addToGroup(gItem);
            }
        }

        if(_tempGroup->childItems().count() > 0) {
            addTempGroup();

            QPointF pos = getCursorScenePos();
            _tempGroup->setPos(pos);

            autoFitSceneSize();
            setNeedSaveState(true);
        }
        else {
            resetTempGroup();
        }
    }
}


void AbsGraphScene::onEscapePress()
{
    resetTempGroup();
    _hAlignLine->hide();
    _vAlignLine->hide();
}

void AbsGraphScene::onSkinChanged()
{
    setBackgroundBrush(QBrush(Config::curSkinStyle()->SceneFillColor));
}

void AbsGraphScene::onEscapeCreateGraphicsItem()
{
    if(getToolkitType() != ToolkitType_invalid) {
        MainWindow::instance()->setModelDockBoxToolkitBtnStatus(false);
        if(getToolkitType() == ToolkitType_Polyline) {
            _pPath.clear();
            removeItem(_grapPathItem);
        }
    }
}

QGraphicsItem * AbsGraphScene::CopyGraphicsDesignItem(QGraphicsItem *graphItem)
{
    QGraphicsItem* cloneItem = NULL;
    if(graphItem->type() == DesignItem::Type)
    {
        DesignItem* item = qgraphicsitem_cast<DesignItem *>(graphItem);
        cloneItem = item->clone(NULL);
//        cloneItem->setZValue(graphItem->zValue());

//        ConfigItem *fromItem = qgraphicsitem_cast<ConfigItem*>(graphItem);
//        ConfigItem *toItem = qgraphicsitem_cast<ConfigItem*>(cloneItem);
//        qDebug() << __FUNCTION__ << __LINE__
//                 << fromItem->customName() << fromItem->zValue()
//                 << toItem->customName() << toItem->zValue();
    }
    return cloneItem;
}

QGraphicsItem* AbsGraphScene::CopyGraphicsLineItem(QGraphicsItem *graphItem, QMap<QGraphicsItem*, QGraphicsItem*> itemMap)
{
    return NULL;
}

QGraphicsItem *AbsGraphScene::CopyGraphicsPortItem(QGraphicsItem *graphItem)
{
    QGraphicsItem* cloneItem = NULL;
    if(graphItem->type() == ItemPort::Type)
    {
        ItemPort* port = qgraphicsitem_cast<ItemPort *>(graphItem);
        cloneItem = port->clone();
    }
    return cloneItem;
}

void AbsGraphScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)//单机右键菜单栏事件
{
    QList<QGraphicsItem *> itemList = items(event->scenePos());
    for(QGraphicsItem * item : itemList)
    {
        if(item->type() == DesignItem::Type || item->type() == LineItem::Type){
            item->setFocus();
            item->update();
        }
    }
    updateMenuActions();
    _contextMenu.exec(event->screenPos());
}



void AbsGraphScene::addTempGroup()
{
    QRectF rcBound = _tempGroup->boundingRect().adjusted(0.5, 0.5, -0.5, -0.5);
    if(rcBound.x() != 0 ||  rcBound.y() != 0) {
        for(QGraphicsItem *childItem :_tempGroup->childItems()){
            childItem->moveBy(-rcBound.x(), -rcBound.y());
        }
    }
    addItem(_tempGroup);
    _tempGroup->update();
}


QList<QGraphicsItem*> AbsGraphScene::getTempGroupChild()
{
    QList<QGraphicsItem*> childList;
    for(QGraphicsItem* itemChild : _tempGroup->childItems())
    {
        _tempGroup->removeFromGroup(itemChild);
        removeItem(itemChild);
        childList.append(itemChild);
    }
    removeItem(_tempGroup);
    return childList;
}


void AbsGraphScene:: resetTempGroup()
{
    if(_tempGroup->scene() != NULL)
    {
        for(QGraphicsItem* itemChild : _tempGroup->childItems())
        {
            _tempGroup->removeFromGroup(itemChild);
            removeItem(itemChild);
        }
        removeItem(_tempGroup);
    }
}

QRectF AbsGraphScene::getItemsBoundingRect(QPointF topLeft, QList<QGraphicsItem *> items)
{
    if(items.isEmpty()) return QRectF(topLeft, QSize(1400, 900));
    QRectF topLeftRec = QRectF(topLeft, QSize(1,1));
    foreach(QGraphicsItem *item, items) {
        QRectF rect = item->boundingRect();
        rect.setSize(rect.size()+QSizeF(300, 200));
        topLeftRec = topLeftRec.united(item->mapRectToScene(rect));
    }
    return topLeftRec;
}

//焦点变换时刷新属性列表
void AbsGraphScene::onFocusItemChanged(QGraphicsItem* newItem, QGraphicsItem* oldItem, Qt::FocusReason reason)
{
    _oldFocusItem = oldItem;
    if(oldItem != NULL){
        QGraphicsProxyWidget *oldProxy = qgraphicsitem_cast<QGraphicsProxyWidget *>(oldItem);
        if(oldProxy != NULL)
            oldProxy->setZValue(CftSettingZVal);
        else
            oldItem->setZValue(DesignItemMinZVal);
    }


    /*可能1：点在空白处，通过鼠标Release事件直接清除
     *可能2：scene整体失去焦点，此时保持属性列表
     *可能3：删除控件，此事件之后oldItem才删除*/
    if(newItem != NULL){
        QGraphicsProxyWidget *newProxy = qgraphicsitem_cast<QGraphicsProxyWidget *>(newItem);
        if(newProxy != NULL)
            newProxy->setZValue(TopZVal);
        else
            newItem->setZValue(SELECT_ZVALUE);

        MainWindow::instance()->initTreeProperty(newItem);
    }
    else {
        QGraphicsItem* oldFocus = oldItem;
        QTimer::singleShot(50,  this, [oldFocus] {
            if(oldFocus != NULL && oldFocus->scene() == NULL) {
                MainWindow::instance()->initTreeProperty(NULL);
            }
        });
    }
}

