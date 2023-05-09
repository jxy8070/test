#include "DesignItem.h"
#include <QMetaProperty>
#include "qtvariantproperty.h"
#include "Frame/mainwindow.h"
#include "Frame/absgraphscene.h"
#include "qdebug.h"


DesignItem::DesignItem()
{
    setZValue(DesignItemMinZVal);
}


int DesignItem::type() const
{
    return Type;
}


void DesignItem::changePropertyValue(const char* propName, const QVariant& newVal)
{
    setProperty(propName, newVal);
    update();
}


//void DesignItem::valueChanged(QtProperty *qtProp, const QVariant &newVal)
//{
//    QByteArray ba = qtProp->propertyName().toLatin1();
//    const char *propName = ba.data();
//    QVariant oldVal = property(propName);
//    AbsGraphScene* pScene = qobject_cast<AbsGraphScene*>(scene());
//    if(pScene != NULL)
//    {
//        pScene->itemPropChanged(this, propName, newVal, oldVal);
//    }
//}


void DesignItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    _isMovingStarted = false;
    AbsGraphScene* pScene = qobject_cast<AbsGraphScene*>(scene());
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        QPointF scenePoint = scenePos();
        setData(DataKey_OldScenePos, scenePoint);
        QPointF sPos = pScene->getCursorScenePos();
        setData(DataKey_PressedScenePos, sPos);
        QPointF itemPos = mapFromScene(sPos);
        itemPos = QPointF(itemPos.x() -1, itemPos.y() -1);
        setData(DataKey_PressedItemPos, itemPos);
    }
    QGraphicsRectItem::mousePressEvent(event);
}

void DesignItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //防止误操作，跳过短距离移动
    if ((event->buttons() & Qt::LeftButton) && (flags() & ItemIsMovable))
    {
        QPointF point = event->buttonDownScenePos(Qt::LeftButton) - event->scenePos();
        qreal dist = point.x()* point.x() +point.y()*point.y();
        if(_isMovingStarted == false && dist < ALIGN_DISTENCE)
        {
            event->ignore();
            return;
        }
        else
        {
           _isMovingStarted = true;
        }
    }

    QGraphicsRectItem::mouseMoveEvent(event);
}

void DesignItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    _isMovingStarted = false;
    setData(DataKey_OldScenePos, QVariant());
    setData(DataKey_PressedScenePos, QVariant());
    setData(DataKey_PressedItemPos, QVariant());

    QGraphicsRectItem::mouseReleaseEvent(event);
}

QVariant DesignItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    //取消限制item移动范围sceneRect，因场景已自适应大小
//    if (change == ItemPositionChange && scene()) {
//        QPointF newPos = value.toPointF();
//        QRectF rect = scene()->sceneRect();
//        if (!rect.contains(newPos)) {
//            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
//            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
//            return newPos;
//        }
//    }
    return QGraphicsItem::itemChange(change, value);
}



QPainterPath DesignItem::getRoundRectPath(QRectF rect, int tlRadius, int trRadius, int blRadius, int brRadius)
{
    QPainterPath path;
    int _width = rect.width();
    int _height = rect.height();
    int _basic = _width > _height ? _height : _width;

    if (tlRadius > (int)(_basic / 2)) tlRadius = (int)(_basic / 2);
    if (trRadius > (int)(_basic / 2)) trRadius = (int)(_basic / 2);
    if (blRadius > (int)(_basic / 2)) blRadius = (int)(_basic / 2);
    if (brRadius > (int)(_basic / 2)) brRadius = (int)(_basic / 2);

    path.moveTo(rect.left() + tlRadius, rect.top());

    QRectF tlRC(rect.topLeft(), QSize(tlRadius * 2, tlRadius * 2));
    path.arcTo(tlRC, 90, 90); // 绘制矩形内切圆，起始角度，逆时针旋转角度后所对应圆弧

    path.lineTo(rect.left(), rect.bottom() - blRadius);

    QRectF blRC(QPoint(rect.left(), rect.bottom() - blRadius * 2), QSize(blRadius * 2, blRadius * 2));
    path.arcTo(blRC, 180, 90);

    path.lineTo(rect.right() - brRadius, rect.bottom());

    QRectF brRc(QPoint(rect.right() - brRadius * 2, rect.bottom() - brRadius * 2), QSize(brRadius * 2, brRadius * 2));
    path.arcTo(brRc, 270, 90);

    path.lineTo(rect.right(), rect.top() + trRadius);

    QRectF trRc(QPoint(rect.right() - trRadius * 2, rect.top()), QSize(trRadius * 2, trRadius * 2));
    path.arcTo(trRc, 0, 90);

    path.lineTo(rect.left() + tlRadius, rect.top());

    return path;
}


QPainterPath DesignItem::getBottomRoundRectPath(QRectF rect, int radius)
{
    QPainterPath path;
    int diameter = radius * 2;

    path.moveTo(rect.right(), rect.bottom() - radius);
    path.lineTo(rect.right(), rect.top());
    path.lineTo(rect.left(), rect.top());
    path.lineTo(rect.left(), rect.bottom() - radius);
    QRectF ltRC(rect.left(), rect.bottom() - diameter, diameter, diameter);
    path.arcTo(ltRC, 180, 90);
    path.lineTo(rect.right() - radius, rect.bottom());
    QRectF rtRC(rect.right()- diameter, rect.bottom() - diameter, diameter, diameter);
    path.arcTo(rtRC, 270, 90);

    return path;
}





