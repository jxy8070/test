#include "serpentineconnector.h"
#include "configItem/configitem.h"

SerpentineConnector::SerpentineConnector(SerConType conType, int channelCount, DevPortType portType)
    :DesignItem(), _rectWidth(40), /*_borderColor(QColor(34,177,61)),*/ _backgroundColor(QColor(192,203,231)),
      _moduleName(QString("Snake Connector")), _ConType(conType), _channelCount(channelCount), _channelPortType(portType)
{
    setFlags(QGraphicsRectItem::ItemIsSelectable | QGraphicsRectItem::ItemIsFocusable |
             QGraphicsRectItem::ItemIsMovable);

    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        _borderColor = QColor(34,177,61);
        _borderWidth = 2;
    }else if(Config::curMfrsCfg().CurMfrs == Mfrs_KEMU
             || Config::curMfrsCfg().CurMfrs == Mfrs_ControlMax) {
        _borderColor = Qt::black;
        _borderWidth = 1;
    }

    if(_ConType == SerConType_In) {
        initPort(_channelCount, 1);
    }else if(_ConType == SerConType_Out) {
        initPort(1, _channelCount);
    }
    connect(this, SIGNAL(channelCountSignal(int)), this, SLOT(channelCountSlot(int)));

}

int SerpentineConnector::type() const
{
    return Type;
}

QPainterPath SerpentineConnector::shape() const
{
    return _borderPath;
}

#include "Frame/configscene.h"
#include "device/device_trn.h"
DesignItem *SerpentineConnector::clone(DesignItem *toItem)
{
    SerpentineConnector *serItem = NULL;
    if(!toItem) {
        serItem = new SerpentineConnector(this->serConType(), this->getChannelCount(),
                                          this->getChannelPortType());
        serItem->setPos(this->pos());
        serItem->setUserZValue(qobject_cast<ConfigScene *>(this->scene())->getTrnDev()->getItemUserZValueNum(true));
    }
    return serItem;
}

void SerpentineConnector::initPort(int inNum, int outNum)
{
    int maxNum = inNum > outNum? inNum : outNum;
    _rectHeight = 2 + maxNum*portDist;
    refreshBorderPath();
    DevPortType InPortType, OutPortType;
    if(_ConType == SerConType_In) {
        InPortType = _channelPortType;
        OutPortType = PortType_ChannelBus;
    }else if(_ConType == SerConType_Out) {
        InPortType = PortType_ChannelBus;
        OutPortType = _channelPortType;
    }

    if(_inPortList.count()> 0 || _outPortList.count())
    {
        QList<LineItem*> lineList;
        if(this->scene())
            for(QGraphicsItem *item : this->scene()->items(Qt::AscendingOrder)) {
                LineItem * line = qgraphicsitem_cast<LineItem *>(item);
                if(line != NULL) {
                    lineList.append(line);
                }
            }

        for(ItemPort *iItem : _inPortList) {
            for(LineItem *line : lineList) {
                if(line->getEndPort() == iItem) {
                    this->scene()->removeItem(line);
                    delete line;
                }
            }
            _inPortList.removeAll(iItem);
            delete iItem;
        }
        for(ItemPort *oItem : _outPortList) {
            for(LineItem *line : lineList) {
                if(line->getStartPort() == oItem) {
                    this->scene()->removeItem(line);
                    delete line;
                }
            }
            _outPortList.removeAll(oItem);
            delete oItem;
        }
    }

    for(int i =0; i<inNum; i++)//输入端口
    {
        QPointF pos(2, 2 + portWidthAndHeight*i);
        QString portName = QString("ser Input %1").arg(i+1);

        ItemPort* port = new ItemPort(InPortType, portName, i, true, pos, maxPortNameWidth, this);
        _inPortList.append(port);
    }

    for(int i =0; i<outNum; i++)
    {
        QPointF pos(_rectWidth-portWidthAndHeight, 2 + portDist*i);
        QString portName = QString("ser Output %1").arg(i+1);

        ItemPort* port = new ItemPort(OutPortType, portName, i, false, pos, maxPortNameWidth, this);
        _outPortList.append(port);
    }
}

void SerpentineConnector::refreshBorderPath()
{
    int radius = 3;
    if(_channelCount == 1) {
        _borderPath.clear();
        _borderPath.addRoundedRect(QRectF(0, 0, _rectWidth, _rectHeight), radius, radius);
        return;
    }
    if(_ConType == SerConType_In){
        _borderPath.clear();
        _borderPath.moveTo(radius, 0);
        _borderPath.arcTo(QRect(0, 0, radius*2, radius*2), 90, 90);
        _borderPath.lineTo(0, _rectHeight-radius*2);
        _borderPath.arcTo(QRect(0, _rectHeight-radius*2, radius*2, radius*2), 180, 90);
        _borderPath.lineTo(portDist+2-radius, _rectHeight);
        _borderPath.arcTo(QRect(portDist+2-radius*2, _rectHeight-radius*2, radius*2, radius*2), 270, 90);
        _borderPath.lineTo(portDist+2, portDist+2);
        _borderPath.lineTo(_rectWidth-radius, portDist+2);
        _borderPath.arcTo(QRect(_rectWidth-radius*2, portDist+2-radius*2, radius*2, radius*2), -90, 90);
        _borderPath.lineTo(_rectWidth, radius);
        _borderPath.arcTo(QRect(_rectWidth-radius*2, 0, radius*2, radius*2), 0, 90);
        _borderPath.lineTo(radius, 0);
    }else {
        _borderPath.clear();
        _borderPath.moveTo(radius, 0);
        _borderPath.arcTo(QRect(0, 0, radius*2, radius*2), 90, 90);
        _borderPath.lineTo(0, portDist+2-radius);
        _borderPath.arcTo(QRect(0, portDist+2-radius*2, radius*2, radius*2), 180, 90);
        _borderPath.lineTo(_rectWidth-portDist-2, portDist+2);
        _borderPath.lineTo(_rectWidth-portDist-2, _rectHeight-radius);
        _borderPath.arcTo(QRect(_rectWidth-portDist-2, _rectHeight-radius*2, radius*2, radius*2), 180, 90);
        _borderPath.lineTo(_rectWidth-radius, _rectHeight);
        _borderPath.arcTo(QRect(_rectWidth-radius*2, _rectHeight-radius*2, radius*2, radius*2), -90, 90);
        _borderPath.lineTo(_rectWidth, radius);
        _borderPath.arcTo(QRect(_rectWidth-radius*2, 0, radius*2, radius*2), 0, 90);
        _borderPath.lineTo(radius, 0);
    }
}

void SerpentineConnector::conectingLine(QList<ItemPort *> &startPortList)
{
    QList<ItemPort *> endPortList;
    if(startPortList.isEmpty()) return ;
    if(startPortList.at(0)->getPortType() == PortType_ChannelBus) {
        if(_ConType == SerConType_In) {
            endPortList.append(_outPortList);
        }else if(_ConType == SerConType_Out) {
            endPortList.append(_inPortList);
        }
    }else {
        if(_ConType == SerConType_In) {
            endPortList.append(_inPortList);
        }else if(_ConType == SerConType_Out) {
            endPortList.append(_outPortList);
        }
    }

    int minPortCount = startPortList.count() < endPortList.count()? startPortList.count():endPortList.count();
    for(int i = 0; i < minPortCount; i++) {
       QVariant var = startPortList.at(i)->data(DataKey_TempLineItem);
       if(var.isNull()) break;
       startPortList.at(i)->setData(DataKey_TempLineItem, QVariant());
       LineItem *line = var.value<LineItem *>();
       line->setEndPort(endPortList.at(i));
//       line->initAfterAdd();
       line->update();
       line->setNormalization();
    }

}

void SerpentineConnector::saveDataToSettings(QSettings *ConfigIni)
{
    ConfigIni->setValue(QString(CONFIG_SERCONITEM_POS), this->pos());
    ConfigIni->setValue(QString(CONFIG_SERCONITEM_HEIGHT), this->_rectHeight);
    int pCount = this->_borderPath.elementCount();
    ConfigIni->setValue(QString(CONFIG_SERCONITEM_PATHCOUNT), pCount);
    for(int i = 0; i < pCount; i++) {
        QPointF pointF = this->_borderPath.elementAt(i);
        ConfigIni->setValue(QString(CONFIG_SERCONITEM_BORDERPATH).arg(i), pointF);
    }
    ConfigIni->setValue(QString(CONFIG_SERCONITEM_BORDERCOLOR), this->_borderColor);
    ConfigIni->setValue(QString(CONFIG_SERCONITEM_BGDCOLOR), this->_backgroundColor);
//    ConfigIni->setValue(QString(CONFIG_SERCONITEM_MODULENAME), this->_moduleName);
    ConfigIni->setValue(QString(CONFIG_SERCONiTEM_TYPE), this->_ConType);
    ConfigIni->setValue(QString(CONFIG_SERCONITEM_CHANNELCOUNT), this->_channelCount);
    ConfigIni->setValue(QString(CONFIG_SERCONITEM_CHAPORTTYPE), this->_channelPortType);
    ConfigIni->setValue(QString(CONFIG_SERCONITEM_ZVALUE), this->checkZValue());


}

void SerpentineConnector::loadDataFromSettings(QSettings *ConfigIni)
{
    QPointF pos = ConfigIni->value(QString(CONFIG_SERCONITEM_POS)).value<QPointF>();
    int height  = ConfigIni->value(QString(CONFIG_SERCONITEM_HEIGHT)).toInt();
    int pathCou = ConfigIni->value(QString(CONFIG_SERCONITEM_PATHCOUNT)).toInt();
    QPainterPath path;
    for(int i = 0; i < pathCou; i++) {
        QPointF point = ConfigIni->value(QString(CONFIG_SERCONITEM_BORDERPATH).arg(i)).value<QPointF>();
        if(!i)
            path.moveTo(point);
        path.lineTo(point);
    }
    QColor borColor = ConfigIni->value(QString(CONFIG_SERCONITEM_BORDERCOLOR)).value<QColor>();
    QColor bgdColor = ConfigIni->value(QString(CONFIG_SERCONITEM_BGDCOLOR)).value<QColor>();
//    QString modName = ConfigIni->value(QString(CONFIG_SERCONITEM_MODULENAME)).toString();

    this->setPos(pos);
    this->_rectHeight  = height;
    this->_borderPath  = path;
    this->_borderColor = borColor;
    this->_backgroundColor = bgdColor;
}

QRectF SerpentineConnector::boundingRect() const
{
    return QRectF(0, 0, _rectWidth, _rectHeight);
}

void SerpentineConnector::channelCountSlot(int count)
{
    Q_UNUSED(count);
    if(_channelCount < 1) _channelCount = 1;
    if(_ConType == SerConType_In) {
        initPort(_channelCount, 1);
    }else if(_ConType == SerConType_Out) {
        initPort(1, _channelCount);
    }
    update();
}

//void SerpentineConnector::backgroundColorSlot(QColor bgColor)
//{
//    Q_UNUSED(bgColor);
//}

void SerpentineConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(_borderColor, _borderWidth));
    painter->setBrush(_backgroundColor);
    if(isSelected()) {
        if(hasFocus()) {
            QPen pen(Qt::red, _borderWidth, Qt::SolidLine, Qt::RoundCap);
            painter->setPen(pen);
        }else {
            QPen pen(Qt::red, _borderWidth, Qt::DashLine, Qt::RoundCap);
            painter->setPen(pen);
        }
    }
    painter->drawPath(_borderPath);

//    painter->drawText(_borderPath.boundingRect(), Qt::AlignCenter, QString::number(_userZValue));
}
