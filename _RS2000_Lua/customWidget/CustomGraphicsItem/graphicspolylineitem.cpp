#include "graphicspolylineitem.h"

graphicsPolylineItem::graphicsPolylineItem(QPainterPath pPath, QGraphicsItem *parent)
    : GraphicsItem(pPath, GrapItemType_polyLine, parent)
{
    setRect(pPath.controlPointRect());

    QPolygonF poly = _borderPath.toFillPolygon(QTransform::fromScale(1, 1));
    QRectF rec = boundingRect();
    QVector<QPointF>::iterator iter;
    for(iter = poly.begin(); iter != poly.end(); iter++) {
//        qDebug() << __FUNCTION__ << __LINE__ << *iter;
        xyRatio_t ratio;
        ratio.s_x = (*iter-rec.topLeft()).x() / (rec.bottomRight()-rec.topLeft()).x();
        ratio.s_y = (*iter-rec.topLeft()).y() / (rec.bottomRight()-rec.topLeft()).y();
        _xyRatio.append(ratio);
    }
}

QPainterPath graphicsPolylineItem::shape() const
{
    return _borderPath;
}

void graphicsPolylineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath Path = refreshIsSelectedPath();

    painter->setPen(QPen(_borderColor, _borderWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(_backgroundColor);
    painter->drawPath(Path);

    painter->setFont(_font);
    painter->setPen(_fontColor);
    painter->drawText(boundingRect(), _textAlignment, _text);

    painter->setBrush(Qt::transparent);
    if(isSelected()) {
        if(hasFocus()){
            painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }else {
            painter->setPen(QPen(Qt::red, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
        }
        painter->drawPath(_borderPath);
    }

    if(isInside()) {
        painter->setPen(QPen(QColor(0,255,0, 100), 2));
        painter->drawPath(_borderPath);
    }

//    painter->setPen(QPen(QColor(255,0,233), _borderWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    painter->drawRect(boundingRect());
}

void graphicsPolylineItem::refreshBorderPath()
{
    QPolygonF poly = _borderPath.toFillPolygon(QTransform::fromScale(1, 1));
    _borderPath.clear();
    QRectF rec = boundingRect();
    QVector<QPointF>::iterator iter;
    int i = 0;
    for(iter = poly.begin(); iter != poly.end(); iter++) {
        QPointF point;
        point.setX((rec.bottomRight()-rec.topLeft()).x() * _xyRatio.at(i).s_x + rec.topLeft().x());
        point.setY((rec.bottomRight()-rec.topLeft()).y() * _xyRatio.at(i).s_y + rec.topLeft().y());
        if(!i++) {
            _borderPath.moveTo(point);
        }else {
            _borderPath.lineTo(point);
        }
    }
}

QPainterPath graphicsPolylineItem::refreshIsSelectedPath()
{
    refreshBorderPath();

    QPainterPath path;
    int len = _borderPath.elementCount();
    for(int i = 0; i < len-1; i++) {
        const QPointF p  = _borderPath.elementAt(i);
        const QPointF p1 = _borderPath.elementAt(i == 0 ? len - 2 : i - 1);
        const QPointF p2 = _borderPath.elementAt(i == len - 2 ? 0 : i + 1);

        //归一化处理
        qreal v1x = p1.x() - p.x();
        qreal v1y = p1.y() - p.y();
        qreal n1  = qSqrt(qPow(v1x, 2) + qPow(v1y, 2));
        v1x /= n1;
        v1y /= n1;

        qreal v2x = p2.x() - p.x();
        qreal v2y = p2.y() - p.y();
        qreal n2  = qSqrt(qPow(v2x, 2) + qPow(v2y, 2));
        v2x /= n2;
        v2y /= n2;

        //凹凸性
        qreal mult_cross = (p1.x()-p.x()) * (p2.y()-p1.y()) - (p1.y()-p.y()) * (p2.x()-p.x());
        if(mult_cross < 0) mult_cross = -1;
        else mult_cross = 1;

        qreal v = mult_cross * _borderWidth/2 / qSqrt((1 - (v1x * v2x + v1y * v2y)) / 2);
        //向量绝对值 = borderWidth/2 / sin(Ø/2)
        qreal vx = v1x + v2x;
        qreal vy = v1y + v2y;
        qreal n  = v / qSqrt(qPow(vx, 2) + qPow(vy, 2));
        vx *= n;
        vy *= n;

        if(i == 0) {
            path.moveTo(QPointF(vx+p.x(), vy+p.y()));
        }
        path.lineTo(QPointF(vx+p.x(), vy+p.y()));
        if(i == len - 2) {
            path.lineTo(path.elementAt(0));
        }
    }
//    qDebug() << __FUNCTION__ << __LINE__ << _borderPath << path;

    return path;
}
