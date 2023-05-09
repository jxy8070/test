#include "stable.h"
#include "LineItem.h"
#include "ItemPort.h"
#include "DesignItem.h"
#include "Frame/absgraphscene.h"
#include "Frame/mainwindow.h"

QDataStream& operator<<(QDataStream& ds, LineItem& connect)
{
    ds << connect.getStartPort()->scenePos()
       << connect.getEndPort()->scenePos();
    return ds;
}

//QDataStream& operator>>(QDataStream& ds, connectItem& devItem)
//{
//    return ds;
//}

LineItem::LineItem(AbsGraphScene *scene, ItemPort *startPort, ItemPort *endPort, QGraphicsItem *parent) :
    QGraphicsPathItem(parent)
{
//    qDebug() << __FUNCTION__ << __LINE__
//             << staNAP-200rtPort << endPort;
    setFlag(QGraphicsLineItem::ItemIsSelectable, true);
    _scene = scene;
    setPos(startPort->scenePos());
    _startPort = startPort;
    _endPort = endPort;

    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        if(startPort->getPortType() == PortType_MONAURAL) {//模拟
            _lineColor = QColor(146, 195, 223);
        }
        else if(startPort->getPortType() == PortType_AES67) {
            _lineColor = QColor(199, 212, 141);
        }
        else if(startPort->getPortType() == PortType_Config) {
            _lineColor = Qt::black;
        }
        else {
            _lineColor = startPort->getPainterColor();
        }
    }
    else {
//        DevPortType portType = startPort->getPortType();
//        if(portType == PortType_MONAURAL)
//            _lineColor = Qt::white;
//        else if(portType ==PortType_Dante || PortType_AES67)
//            _lineColor = Qt::black;
//        else _lineColor = Qt::black;
        _lineColor = QColor(120,120,120);
    }

    setZValue(LineItemZVal);
    QGraphicsPathItem::update();
}

LineItem::~LineItem()
{

}


LineItem* LineItem::clone(ItemPort *sPort, ItemPort *ePort)
{
    LineItem* line = new LineItem(_scene, sPort, ePort, NULL);
    line->setUserZValue(sPort, ePort);
    return line;
}

int LineItem::type() const
{
    return Type;
}

bool LineItem::pointDetect(ItemPort *start, ItemPort *end, ItemPort *cur)
{
    QPainterPath p;
    QPointF pos_start(mapFromItem(start, 5, 5));
    QPointF pos_end(mapFromItem(end, 5, 5));
    QPointF pos_cur(mapFromItem(cur, 5, 5));

    if (abs(pos_start.x() - pos_end.x()) < 5 || abs(pos_start.y() - pos_end.y()) < 5)
    {
        pos_start += QPointF(0, 0);
        pos_end -= QPointF(5, 5);
    }
    p.moveTo(pos_start);
    p.lineTo(pos_end);

    if (p.boundingRect().contains(pos_cur))
    {
        return true;
    }

    return false;
}

void LineItem::addpoint(ItemPort *point, ItemPort *before, ItemPort *after)
{
//    if (before == NULL && after == NULL)//    {
//        for (int i = 0; i < _pointList.count() - 1; ++i)
//        {
//            if (pointDetect(_pointList.at(i), _pointList.at(i+1), point))
//            {
//                _pointList.insert(i+1, point);
//                break;
//            }
//        }
//    } else {
//        for (int i = 0; i < _pointList.count() - 1; ++i)
//        {
//            if (_pointList.at(i) == before || _pointList.at(i+1) == after)
//            {
//                _pointList.insert(i+1, point);
//                break;
//            }
//        }
//    }
}

void LineItem::delpoint(ItemPort *point, ItemPort *&before, ItemPort *&after)
{
//    qDebug() << "del"
//             << "list:" << _pointList
//             << "point:"<< point;
//    for (int i = 1; i < _pointList.count() - 1; ++i)
//    {
//        if (_pointList.at(i) == point)
//        {
//            before = _pointList.at(i - 1);
//            after = _pointList.at(i + 1);
//            _pointList.removeOne(point);
//            break;
//        }
//    }
}


void LineItem::initAfterAdd()
{
    assert(_startPort != NULL && _endPort != NULL);
    if(_startPort != NULL && _endPort != NULL) {
        _startPort->setEnable(false);
        _endPort->setEnable(false);
    }
}

void LineItem::clearBeforeDelete()
{
    assert(_startPort != NULL && _endPort != NULL);
    _startPort->setEnable(true);
    _endPort->setEnable(true);
}

void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
        paint_FangTu(painter);
    }
    else{
        paint_Defult(painter);
    }
}

void LineItem::paint_Defult(QPainter *painter)
{
    painter->setPen(QPen(Config::curSkinStyle()->LineColor, 1, Qt::SolidLine, Qt::RoundCap));
    painter->setBrush(Qt::transparent);

    if(isSelected())
    {
        painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap));
    }
    QPointF pos_start = mapFromItem(_startPort, 6.5, 5.5);
    QPointF pos_end;
    if( _endPort == NULL) //临时线
    {
        painter->setPen(QPen(QColor(50, 50, 230), 1, Qt::SolidLine, Qt::RoundCap));
        AbsGraphScene* pScene = qobject_cast<AbsGraphScene*>(scene());
        if(pScene != NULL) pos_end = mapFromScene(pScene->getCursorScenePos());
    }
    else
    {
        pos_end = mapFromItem(_endPort, 4.5, 5.5);
    }

    QPainterPath p;
    if(_pointList.count()== 0) //贝塞尔线
    {
        p.moveTo(pos_start);
        qreal dx = qAbs(pos_end.x() - pos_start.x());
        qreal dy = qAbs(pos_end.y() - pos_start.y());
        if(dx > 150) dx = 150;
        if(dy > 50)  dy = 50;

        int iFact = 1;
        if(_startPort->isInput()) iFact = -1;

        QPointF ctr1(pos_start.x() + iFact * dx * 0.3, pos_start.y() + iFact * dy * 0.1);
        QPointF ctr2(pos_end.x() - iFact * dx * 0.3, pos_end.y() - iFact * dy * 0.1);
        p.cubicTo(ctr1, ctr2, pos_end);
    }
    else //折线
    {
        p.moveTo(pos_start);
        for(ItemPort* port : _pointList)
        {
            QPointF portPos(mapFromItem(port, 5, 5));
            p.lineTo(portPos);
        }
        p.lineTo(pos_end);
    }
    setPath(p);
    painter->drawPath(p);

}

void LineItem::paint_FangTu(QPainter *painter)
{
    painter->setPen(QPen(/*QColor(34, 177, 61)*/_lineColor, 2, Qt::SolidLine, Qt::RoundCap));
    painter->setBrush(Qt::transparent);

    if(isSelected())
    {
        painter->setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap));
    }
    QPointF pos_start = mapFromItem(_startPort, 5.5, 5.5);
    QPointF pos_end;
    if( _endPort == NULL) //临时线
    {
        painter->setPen(QPen(QColor(34, 177, 61), 3, Qt::SolidLine, Qt::RoundCap));
        AbsGraphScene* pScene = qobject_cast<AbsGraphScene*>(scene());
        if(pScene != NULL) pos_end = mapFromScene(pScene->getCursorScenePos());
    }
    else
    {
        pos_end = mapFromItem(_endPort, 5.5, 5.5);
    }

    QPainterPath p;
    if(_pointList.count()== 0) //贝塞尔线
    {
        p.moveTo(pos_start);
        qreal dx = qAbs(pos_end.x() - pos_start.x());
        qreal dy = qAbs(pos_end.y() - pos_start.y());
        if(dx > 150) dx = 150;
        if(dy > 50)  dy = 50;

        int iFact = 1;
        if(_startPort->isInput()) iFact = -1;

        QPointF ctr1(pos_start.x() + iFact * dx * 0.3, pos_start.y() + iFact * dy * 0.1);
        QPointF ctr2(pos_end.x() - iFact * dx * 0.3, pos_end.y() - iFact * dy * 0.1);
        p.cubicTo(ctr1, ctr2, pos_end);
    }
    else //折线
    {
        p.moveTo(pos_start);
        for(ItemPort* port : _pointList)
        {
            QPointF portPos(mapFromItem(port, 5, 5));
            p.lineTo(portPos);
        }
        p.lineTo(pos_end);
    }
    setPath(p);
    painter->drawPath(p);
}


void LineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setSelected(true);
}


#include "serpentineconnector.h"
DesignItem * LineItem::getStartDevice()
{
    if(_startPort->parentItem()->type() == DesignItem::Type)
        return qgraphicsitem_cast<DesignItem*> (_startPort->parentItem());
    if(_startPort->parentItem()->type() == SerpentineConnector::Type)
        return qgraphicsitem_cast<SerpentineConnector*> (_startPort->parentItem());
//    DesignItem* startDev = qgraphicsitem_cast<DesignItem*> (_startPort->parentItem());
//    return startDev;
}

DesignItem *LineItem::getEndDevice()
{
    if(!_endPort) return NULL;
    if(_endPort->parentItem()->type() == DesignItem::Type)
        return qgraphicsitem_cast<DesignItem*> (_endPort->parentItem());
    if(_endPort->parentItem()->type() == SerpentineConnector::Type)
        return qgraphicsitem_cast<SerpentineConnector*> (_endPort->parentItem());
//    DesignItem* endDev = NULL;
//    if(_endPort != NULL) endDev = qgraphicsitem_cast<DesignItem*> (_endPort->parentItem());
//    return endDev;
}

bool LineItem::isParentOfPort(DesignItem* item)
{
    if(_startPort->parentItem() == item)
        return true;
    else if(_endPort != NULL && _endPort->parentItem() == item)
        return true;
    else
        return false;
}

void LineItem::flashVisibleByPort()
{
    bool isShow = true;
    if(!_startPort->isVisible())
        isShow = false;
    else if (_endPort != NULL && !_endPort->isVisible())
        isShow = false;
    setVisible(isShow);
    update();
}

void LineItem::setNormalization()
{
    //标准化，确保startPort为输出端口
    if(_startPort->isInput())
    {
        ItemPort* port = _startPort;
        _startPort = _endPort;
        _endPort = port;
    }

    setUserZValue(_startPort, _endPort);
}

void LineItem::setUserZValue(ItemPort *startPort, ItemPort *endPort)
{
    if(startPort->parentItem()->type() == DesignItem::Type)
        _inOutItemZValue[0] = qgraphicsitem_cast<DesignItem *>(_startPort->parentItem())->checkZValue();
    else if(startPort->parentItem()->type() == SerpentineConnector::Type)
        _inOutItemZValue[0] = qgraphicsitem_cast<SerpentineConnector *>(_startPort->parentItem())->checkZValue();

    if(endPort->parentItem()->type() == DesignItem::Type)
        _inOutItemZValue[1] = qgraphicsitem_cast<DesignItem *>(_endPort->parentItem())->checkZValue();
    else if(endPort->parentItem()->type() == SerpentineConnector::Type)
        _inOutItemZValue[1] = qgraphicsitem_cast<SerpentineConnector *>(_endPort->parentItem())->checkZValue();
}

