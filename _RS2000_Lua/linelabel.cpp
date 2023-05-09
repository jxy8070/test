#include "linelabel.h"
#include "ItemPort.h"
#include "Frame/mainwindow.h"

LineLabel::LineLabel(ItemPort* port):
    QAbstractGraphicsShapeItem(port->parentItem())
{
    _font = SkinStyle::getFont(7);
    _isMatched = false;
    _parent = port->parentItem();
    _itemPort = port;
    setBrush(QBrush(Qt::green));
    setFlags(ItemSendsScenePositionChanges | ItemIsSelectable | ItemIsFocusable);//通过单击鼠标或者使用橡皮筋来选择项目
    QPointF pos1 = port->pos();
    if(port->isInput()) {
        setPos(pos1.x() - 4, pos1.y() - 1);
    }
    else {
        setPos(pos1.x() + 13, pos1.y() - 1);
    }
    setName(QString());
}

int LineLabel::type() const
{
    return Type;
}


QRectF LineLabel::boundingRect() const
{
    return _painterPathS.boundingRect();
}

void LineLabel::addToScene()
{
    if(scene() == NULL)
    {
        setParentItem(_parent);
    }
}

void LineLabel::setName(QString text)
{
    _painterPathS.clear();
    _painterPathM.clear();
    _name = text;
    if(!_name.isEmpty())
    {
        QFontMetrics fm(_font);
        int width = fm.boundingRect(_name).width() + 6;
        QPolygonF polygonS, polygonM;
        if(_itemPort->isInput())
        {
             polygonS << QPointF(0.5, Lbl_Height/2 + 0.5) << QPointF(-4.5, 0.5) << QPointF(-4.5 - width -5, 0.5)
                      << QPointF(-4.5 - width, Lbl_Height/2 + 0.5) << QPointF(-4.5 - width -5, Lbl_Height/2 + 0.5)
                      << QPointF(-4.5 - width, Lbl_Height + 0.5) <<  QPointF(-4.5, Lbl_Height + 0.5)
                      << QPointF(0.5, Lbl_Height/2 + 0.5);
             polygonM << QPointF(0.5, Lbl_Height/2 + 0.5) << QPointF(-4.5, 0.5) << QPointF(-4.5 - width, 0.5)
                      << QPointF(-4.5 - width, Lbl_Height + 0.5) <<  QPointF(-4.5, Lbl_Height + 0.5)
                      << QPointF(0.5, Lbl_Height/2 + 0.5);
        }
        else
        {
            polygonS << QPointF(0.5, Lbl_Height/2 + 0.5) << QPointF(5.5, 0.5) << QPointF(5.5 + width + 5, 0.5)
                     << QPointF(5.5 + width, Lbl_Height/2 + 0.5) << QPointF(5.5 + width + 5, Lbl_Height/2 + 0.5)
                     << QPointF(5.5 + width, Lbl_Height + 0.5) <<  QPointF(5.5, Lbl_Height + 0.5)
                     << QPointF(0.5, Lbl_Height/2 + 0.5);
            polygonM << QPointF(0.5, Lbl_Height/2 + 0.5) << QPointF(5.5, 0.5) << QPointF(5.5 + width, 0.5)
                     << QPointF(5.5 + width, Lbl_Height + 0.5) <<  QPointF(5.5, Lbl_Height + 0.5)
                     << QPointF(0.5, Lbl_Height/2 + 0.5);
        }
        _painterPathS.addPolygon(polygonS);
        _painterPathM.addPolygon(polygonM);
        setVisible(true);
    }
    else
    {
        setVisible(false);
    }
}

bool LineLabel::isInput()
{
    return _itemPort->isInput();
}


DevPortType LineLabel::getPortType()
{
    return _itemPort->getPortType();
}

void LineLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setFont(_font);//设置字体
    QPen pen = QPen(Qt::black);
    if(isSelected()){ pen.setColor(Qt::red);}
    painter->setPen(pen);
    if(name().startsWith(Appoint)) {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
            painter->setBrush(QColor(35,217,110));
        else
            painter->setBrush(QColor(12,195,253));
    }else {
        painter->setBrush(QColor(240,248,255));
    }

    if(_isMatched)
        painter->drawPath(_painterPathM);
    else
        painter->drawPath(_painterPathS);

    painter->setPen(QPen(Qt::black));
    QRectF txtRc = boundingRect().adjusted(0, -1, 0, -1);
    painter->drawText(txtRc, Qt::AlignHCenter | Qt::AlignVCenter, _name);

//    if(!_itemPort->isInput())
//        painter->drawText(QRectF(boundingRect().topRight(), QSize(60, 10)), QString("Z:%1,M:%2").arg(getUserZValue()).arg(_matchOutLabelNum));
//    else
//        painter->drawText(QRectF(boundingRect().topLeft()-QPointF(60, 0), QSize(60, 10)),
//                          QString("Z:%1,M:%2").arg(getUserZValue()).arg(_matchOutLabelNum));
}


