#include "graphicsgroupboxitem.h"


GraphicsGroupBoxItem::GraphicsGroupBoxItem(QPainterPath path, QGraphicsItem *parent)
    :GraphicsItem(path, GrapItemType_groupBox, parent)
{
    setRect(path.controlPointRect());
}

QPainterPath GraphicsGroupBoxItem::shape() const
{
    QPainterPath pPath;
    QPainterPathStroker stroker;
    stroker.setWidth(_borderWidth*2);//50%
    QRect textRec = getTextRect(_font, _text);
    pPath.addRect(boundingRect());
    pPath = stroker.createStroke(pPath);
    pPath.addRect(QRectF(boundingRect().topLeft(), textRec.size()));
    return pPath;
}

void GraphicsGroupBoxItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    refreshBorderPath();
    painter->setPen(QPen(_borderColor, _borderWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(_backgroundColor);
//    QRectF rec = QRectF(boundingRect().topLeft() + QPointF(_borderWidth/2, _borderWidth/2),
//                        boundingRect().bottomRight() - QPointF(_borderWidth/2, _borderWidth/2));
//    painter->drawRoundedRect(rec, _borderRadius, _borderRadius);
    painter->drawPath(_borderPath);

    painter->setFont(_font);
    painter->setPen(_fontColor);

    QRect textRec = getTextRect(_font, _text);
    painter->drawText(QRectF(boundingRect().topLeft(), textRec.size()), _textAlignment, _text);

    GraphicsItem::paint(painter, option, widget);
}

void GraphicsGroupBoxItem::refreshBorderPath()
{
    qreal left = boundingRect().left() + _borderWidth/2;
    qreal top  = boundingRect().top() + _borderWidth/2;
    qreal right = boundingRect().right() - _borderWidth/2;
    qreal bottom = boundingRect().bottom() - _borderWidth/2;
    QRect textRec = getTextRect(_font, _text);

    _borderPath.clear();
    QPointF startP;
    if(_text.isEmpty()) {
        startP = QPointF(left+_borderRadius, top);
        _borderPath.moveTo(startP);
        _borderPath.arcTo(QRectF(left, top, _borderRadius*2, _borderRadius*2), 90, 90);
    }else {
        startP = QPointF(left+_borderRadius+textRec.width(), top);
        _borderPath.moveTo(startP);
        _borderPath.arcTo(QRectF(left+textRec.width(), top, _borderRadius*2, _borderRadius*2), 90, 90);
        _borderPath.lineTo(left+textRec.width(), top+textRec.height()-_borderRadius*2);
        _borderPath.arcTo(QRectF(left+textRec.width()-_borderRadius*2, top+textRec.height()-_borderRadius*2,
                                 _borderRadius*2, _borderRadius*2), 0, -90);
        _borderPath.lineTo(left+_borderRadius, top+textRec.height());
        _borderPath.arcTo(QRectF(left, top+textRec.height(), _borderRadius*2, _borderRadius*2), 90, 90);
    }

    _borderPath.lineTo(left, bottom-_borderRadius*2);
    _borderPath.arcTo(QRectF(left, bottom-_borderRadius*2, _borderRadius*2, _borderRadius*2), 180, 90);
    _borderPath.lineTo(right-_borderRadius, bottom);
    _borderPath.arcTo(QRectF(right-_borderRadius*2, bottom-_borderRadius*2, _borderRadius*2, _borderRadius*2), 270, 90);
    _borderPath.lineTo(right, top+_borderRadius);
    _borderPath.arcTo(QRectF(right-_borderRadius*2, top, _borderRadius*2, _borderRadius*2), 0, 90);
    _borderPath.lineTo(startP);
}
