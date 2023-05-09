#include "controltemplate.h"


QPointF ControlTemplate::_topLeft = QPointF(0, 0);
QPointF ControlTemplate::_bottomRight = QPointF(0, 0);

ControlTemplate::ControlTemplate(QGraphicsItem *parent) : QGraphicsRectItem(parent),
    _leftTop_dragPoint(new resizeGraphicsItem(DragType_LeftTop, this)),
    _left_dragPoint(new resizeGraphicsItem(DragType_Left, this)),
    _leftBottom_dragPoint(new resizeGraphicsItem(DragType_LeftBottom, this)),
    _top_dragPoint(new resizeGraphicsItem(DragType_Top, this)),
    _bottom_dragPoint(new resizeGraphicsItem(DragType_Bottom, this)),
    _rightTop_DragPoint(new resizeGraphicsItem(DragType_RightTop, this)),
    _right_DragPoint(new resizeGraphicsItem(DragType_Right, this)),
    _rightBottom_DragPoint(new resizeGraphicsItem(DragType_RightBottom, this))
{
    setFlags(GraphicsItemFlag::ItemIsFocusable | GraphicsItemFlag::ItemIsMovable
             | GraphicsItemFlag::ItemIsSelectable);
    _CurrentDragType = DragType_Release;
}

int ControlTemplate::type() const
{
    return Type;
}

void ControlTemplate::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    qreal width = _left_dragPoint->rect().width();
    qreal height = _left_dragPoint->rect().height();

//    if(isSelected()) {
//        if(hasFocus()){
//            painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//        }else {
//            painter->setPen(QPen(Qt::red, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
//        }
//    }else {
//        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    }

    if(hasFocus())
        painter->setPen(QPen(Qt::darkCyan, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    else
        painter->setPen(QPen(Qt::transparent, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawRect(QRectF(rect().topLeft()+QPointF(ResizeGrapItem_width, ResizeGrapItem_width)
                            , rect().bottomRight()-QPointF(ResizeGrapItem_width, ResizeGrapItem_width)));

    _leftTop_dragPoint->setPos(rect().left(), rect().top());
    _left_dragPoint->setPos(rect().left(), rect().top()+rect().height()/2-height/2);
    _leftBottom_dragPoint->setPos(rect().left(), rect().bottom()-height);

    _top_dragPoint->setPos(rect().left()+rect().width()/2-width/2, rect().top());
    _bottom_dragPoint->setPos(rect().left()+rect().width()/2-width/2, rect().bottom()-height);

    _rightTop_DragPoint->setPos(rect().right()-width, rect().top());
    _right_DragPoint->setPos(rect().right()-width, rect().top()+rect().height()/2-height/2);
    _rightBottom_DragPoint->setPos(rect().right()-width, rect().bottom()-height);
}

void ControlTemplate::addIsSelectItem(QGraphicsItem *item)
{
    _isSelectItemList.append(item);

    qreal tl_x = item->sceneBoundingRect().topLeft().x();
    qreal tl_y = item->sceneBoundingRect().topLeft().y();
    qreal br_x = item->sceneBoundingRect().bottomRight().x();
    qreal br_y = item->sceneBoundingRect().bottomRight().y();

    if(_topLeft.isNull()) {
        _topLeft.setX(tl_x);
        _topLeft.setY(tl_y);
    }else {
        _topLeft.setX(_topLeft.x() < tl_x ? _topLeft.x() : tl_x);
        _topLeft.setY(_topLeft.y() < tl_y ? _topLeft.y() : tl_y);
    }

    _bottomRight.setX(_bottomRight.x() > br_x ? _bottomRight.x() : br_x);
    _bottomRight.setY(_bottomRight.y() > br_y ? _bottomRight.y() : br_y);
//    qDebug() << __FUNCTION__ << __LINE__ << _topLeft << _bottomRight;
}

void ControlTemplate::addIsSelectItemList(QList<QGraphicsItem *> itemList)
{
    _isSelectItemList.append(itemList);
    if(_isSelectItemList.isEmpty()) return;
    int minX, minY, maxX, maxY;
    foreach(QGraphicsItem *item, _isSelectItemList) {
        qreal tl_x = item->sceneBoundingRect().topLeft().x();
        qreal tl_y = item->sceneBoundingRect().topLeft().y();
        qreal br_x = item->sceneBoundingRect().bottomRight().x();
        qreal br_y = item->sceneBoundingRect().bottomRight().y();

        if(_isSelectItemList.at(0) == item) {
            minX = tl_x;
            minY = tl_y;
            maxX = br_x;
            maxY = br_y;
        }else {
            minX = minX < tl_x ? minX : tl_x;
            minY = minY < tl_y ? minY : tl_y;
            maxX = maxX > br_x ? maxX : br_x;
            maxY = maxY > br_y ? maxY : br_y;
        }
//        qDebug() << __FUNCTION__ << __LINE__ << _topLeft << _bottomRight;
    }
    _topLeft     = QPointF(minX, minY);
    _bottomRight = QPointF(maxX, maxY);
}

void ControlTemplate::setCustomRect(QPointF topLeft, QPointF bottomRight)
{
    topLeft -= QPointF(ResizeGrapItem_width, ResizeGrapItem_width);
    bottomRight += QPointF(ResizeGrapItem_width, ResizeGrapItem_width);
    setPos(topLeft);
    setRect(mapRectFromScene(QRectF(topLeft, bottomRight)));
//    qDebug() << __FUNCTION__ << __LINE__ << mapRectFromScene(QRectF(topLeft, bottomRight));

}

void ControlTemplate::clear()
{
    _isSelectItemList.clear();
    _topLeft = QPointF(0, 0);
    _bottomRight = QPointF(0, 0);
}

QRectF ControlTemplate::getNewCenterRectF()
{
    return QRectF(QRectF(rect().topLeft()+QPointF(ResizeGrapItem_width, ResizeGrapItem_width)
                                            , rect().bottomRight()-QPointF(ResizeGrapItem_width, ResizeGrapItem_width)));
}

void ControlTemplate::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(_leftTop_dragPoint->isUnderMouse()) {
        _CurrentDragType = _leftTop_dragPoint->getDragType();
    }else if(_left_dragPoint->isUnderMouse()) {
        _CurrentDragType = _left_dragPoint->getDragType();
    }else if(_leftBottom_dragPoint->isUnderMouse()) {
        _CurrentDragType = _leftBottom_dragPoint->getDragType();
    }else if(_top_dragPoint->isUnderMouse()) {
        _CurrentDragType = _top_dragPoint->getDragType();
    }else if(_bottom_dragPoint->isUnderMouse()) {
        _CurrentDragType = _bottom_dragPoint->getDragType();
    }else if(_rightTop_DragPoint->isUnderMouse()) {
        _CurrentDragType = _rightTop_DragPoint->getDragType();
    }else if(_right_DragPoint->isUnderMouse()) {
        _CurrentDragType = _right_DragPoint->getDragType();
    }else if(_rightBottom_DragPoint->isUnderMouse()) {
        _CurrentDragType = _rightBottom_DragPoint->getDragType();
    }else {
        _CurrentDragType = DragType_Release;
        this->setSelected(false);
        this->clearFocus();
        event->ignore();
        return;
    }
    _startPoint = event->scenePos();
    this->setData(DataFlag_OldRect, QVariant(boundingRect()));
    this->setData(DataFlag_SceneOldRect, QVariant(sceneBoundingRect()));
    foreach(QGraphicsItem *item, _isSelectItemList) {
        item->setData(DataFlag_OldRect, item->boundingRect());
        item->setData(DataFlag_SceneOldRect, item->sceneBoundingRect());
    }

    QGraphicsRectItem::mousePressEvent(event);
}

void ControlTemplate::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    calcControlTemplateScaling(_CurrentDragType, _startPoint, event->scenePos());

}

void ControlTemplate::calcControlTemplateScaling(DragType dragType, QPointF startPoint, QPointF endPoint)
{
    if(this->data(DataFlag_OldRect).isNull()) return ;
    QVariant var = this->data(DataFlag_OldRect);
    QRectF rec = var.value<QRectF>();
    if(rec.isNull()) return ;

    QPointF diffPoint = endPoint - startPoint;

    //获取this左上角坐标、宽高
    qreal w, h;
    QPointF LTPoint = _topLeft - QPointF(ResizeGrapItem_width, ResizeGrapItem_width);
    switch(dragType) {
    case DragType_Left:
        LTPoint.setX(LTPoint.x() + diffPoint.x());
        w = rec.width()-diffPoint.x();
        h = rec.height();
        break;
    case DragType_LeftTop:
        LTPoint += diffPoint;
        w = rec.width()-diffPoint.x();
        h = rec.height()-diffPoint.y();
        break;
    case DragType_LeftBottom:
        LTPoint.setX(LTPoint.x() + diffPoint.x());
        w = rec.width()-diffPoint.x();
        h = rec.height()+diffPoint.y();
        break;
    case DragType_Top:
        LTPoint.setY(LTPoint.y() + diffPoint.y());
        w = rec.width();
        h = rec.height()-diffPoint.y();
        break;
    case DragType_Bottom:
        w = rec.width();
        h = rec.height()+diffPoint.y();
        break;
    case DragType_Right:
        w = rec.width()+diffPoint.x();
        h = rec.height();
        break;
    case DragType_RightTop:
        LTPoint.setY(LTPoint.y() + diffPoint.y());
        w = rec.width()+diffPoint.x();
        h = rec.height()-diffPoint.y();
        break;
    case DragType_RightBottom:
        w = rec.width()+diffPoint.x();
        h = rec.height()+diffPoint.y();
        break;
    default:
        break;
    }

    if(w <= ResizeGrapItem_width*3 || h <= ResizeGrapItem_width*3) {
//        return;
    }else {
        setRect(mapRectFromScene(QRectF(LTPoint, QSizeF(w, h))));
    }

    //calc缩放比例、宽高比例
    QRectF NcenterRec= getNewCenterRectF();
    QRectF OcenterRec= QRectF(QRectF(rec.topLeft()+QPointF(ResizeGrapItem_width, ResizeGrapItem_width)
                                     , rec.bottomRight()-QPointF(ResizeGrapItem_width, ResizeGrapItem_width)));

    qreal horScale = NcenterRec.width()/OcenterRec.width();
//    qreal scale = NcenterRec.width()/NcenterRec.height();
    qreal verscale = NcenterRec.height()/OcenterRec.height();

//    qDebug() << __FUNCTION__ << __LINE__ << scale << horScale << w << h << rec;
    setIsSelectItemScale(_CurrentDragType, verscale, horScale);
}

void ControlTemplate::setIsSelectItemScale(ControlTemplate::DragType dragType, qreal scale, qreal zoom)
{
    if(this->data(DataFlag_SceneOldRect).isNull()) return ;
    QVariant var = this->data(DataFlag_SceneOldRect);
    QRectF rec = var.value<QRectF>();
    if(rec.isNull()) return ;


    foreach(QGraphicsItem *item, _isSelectItemList) {
        if(item->data(DataFlag_SceneOldRect).isNull()
                || item->data(DataFlag_OldRect).isNull()) continue;
        QVariant svar = item->data(DataFlag_SceneOldRect);
        QVariant var  = item->data(DataFlag_OldRect);
        QRectF SceneOldRec = svar.value<QRectF>();
        QRectF oldRec      = var.value<QRectF>();
        if(SceneOldRec.isNull() || oldRec.isNull()) continue;

        qreal w = oldRec.width() * zoom;
//        qreal h = w / scale;
        qreal h = oldRec.height() * scale;

        QPointF topLeft;
        QPointF OSpacing;
        QPointF p;
        switch (dragType) {
        case DragType_LeftTop:
        case DragType_Left:
        case DragType_Top:
            OSpacing = SceneOldRec.bottomRight() - rec.bottomRight() + QPointF(9, 9);
            p.setX(oldRec.bottomRight().x()+OSpacing.x()*(zoom-1));
            p.setY(oldRec.bottomRight().y()+OSpacing.y()*(scale-1));
            topLeft = p - QPointF(w, h);
            break;
        case DragType_LeftBottom:
            OSpacing = SceneOldRec.topRight() - rec.topRight() + QPointF(9, -9);
            p.setX(oldRec.topRight().x()+OSpacing.x()*(zoom-1));
            p.setY(oldRec.topRight().y()+OSpacing.y()*(scale-1));
            topLeft = p - QPointF(w, 0);
            break;
        case DragType_RightTop:
            OSpacing = SceneOldRec.bottomLeft() - rec.bottomLeft() + QPointF(-9, 9);
            p.setX(oldRec.bottomLeft().x()+OSpacing.x()*(zoom-1));
            p.setY(oldRec.bottomLeft().y()+OSpacing.y()*(scale-1));
            topLeft = p - QPointF(0, h);
            break;
        case DragType_Right:
        case DragType_Bottom:
        case DragType_RightBottom:
            topLeft = oldRec.topLeft();
            OSpacing = SceneOldRec.topLeft() - rec.topLeft() - QPointF(9, 9);
            topLeft.setX(topLeft.x()+OSpacing.x()*(zoom-1));
            topLeft.setY(topLeft.y()+OSpacing.y()*(scale-1));
            break;
        default:
            return;
        }

        GraphicsItem *grapItem = qgraphicsitem_cast<GraphicsItem *>(item);
        if(!grapItem) continue;
//        QPainterPath pPath;
//        pPath.addRect(QRectF(topLeft, QSizeF(w, h)));
//        grapItem->setPath(pPath);
        grapItem->setRect(QRectF(topLeft, QSizeF(w, h)));
    }
}

