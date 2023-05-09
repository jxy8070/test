#include "graphicstextitem.h"

GraphicsTextItem::GraphicsTextItem(QPainterPath path, QGraphicsItem *parent)
    : GraphicsItem(path, GrapItemType_text, parent),
      _textEdit(new QTextEdit),
      _proxWid(new GraphicsProxyWidget(this))
{
    setRect(path.controlPointRect());
//    setPos(path.controlPointRect().topLeft());

    _textEdit->horizontalScrollBar()->setHidden(true);
    _textEdit->verticalScrollBar()->setHidden(true);
    connect(_proxWid, &GraphicsProxyWidget::clearFocusSignal, this, &GraphicsTextItem::onTextChangedSlot);
}

void GraphicsTextItem::onTextChangedSlot()
{
    _text = _textEdit->toPlainText();
}

void GraphicsTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(!_proxWid->widget())
        _proxWid->setWidget(_textEdit);
    _proxWid->setVisible(true);
    _proxWid->setFocus();

    _textEdit->setText(_text);
    _textEdit->selectAll();
    _originalText = _text;

    QGraphicsRectItem::mouseDoubleClickEvent(event);
}

void GraphicsTextItem::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) {
        if(_proxWid->isVisible()) {
            _proxWid->clearFocus();
            _text = _originalText;
        }
    }
    QGraphicsRectItem::keyPressEvent(event);
}



void GraphicsTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    _borderPath.clear();
    _borderPath.moveTo(boundingRect().topLeft());
    _borderPath.addRect(boundingRect());

    painter->setPen(QPen(_borderColor, _borderWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(_backgroundColor);
    QRectF rec = QRectF(boundingRect().topLeft() + QPointF(_borderWidth/2, _borderWidth/2),
                        boundingRect().bottomRight() - QPointF(_borderWidth/2, _borderWidth/2));
    painter->drawRoundedRect(rec, _borderRadius, _borderRadius);

    painter->setFont(_font);
    painter->setPen(_fontColor);
    painter->drawText(boundingRect(), _textAlignment, _text);

    GraphicsItem::paint(painter, option, widget);
}
