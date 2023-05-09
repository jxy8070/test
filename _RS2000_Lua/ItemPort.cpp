#include "stable.h"
#include "ItemPort.h"
#include "deviceItem/deviceItem.h"
#include "customWidget/tooltip.h"
#include "linelabel.h"
#include "Frame/mainwindow.h"

const QColor ItemPort::ActiveColor = Qt::red;
const QColor ItemPort::DisableColor = QColor(192, 192, 192);
const qreal ItemPort::TextHeightPixel = 13;
const QColor ItemPort::MuteFillColor = QColor("#D11713");

ItemPort::ItemPort(DevPortType type, QString portName, int portNum, bool isInput,
                               QPointF &pos, int maxTextPixel,  QGraphicsItem *parent):
    QGraphicsPathItem(parent)
{
    if (type == PortType_Inflection)
    {
        setFlags(ItemSendsScenePositionChanges);//启用这个标志来接受场景位置变化的通知
    } else {
        setFlags(ItemSendsScenePositionChanges | ItemIsSelectable | ItemIsFocusable);//通过单击鼠标或者使用橡皮筋来选择项目
        setAcceptHoverEvents(true);//启用悬停事件
    }

    _portType           = type;
    _portName       = portName;
    _portNum        = portNum;
    _isInput        = isInput;
    _maxTextPixel   = maxTextPixel;
    _linelabel = NULL;
    _ctrlPinDef = NULL;
    _isMute = false;
    _EnableMute = false;

    _painterWidth = 1;
    _painterColor = Qt::black;
    DefaultColor = Qt::white;

    _sharpPath.addRect(-1, -1, 12, 12);    //适当扩大点击选中端口的区域
    setPos(pos); //设置对象所在位置
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu){
        initPort_FangTu(type, portName, portNum, isInput);    }
    else{
        initPort(type, portName, portNum, isInput);
    }
    setToolTip(_portName);
}


QPointF ItemPort::centerPos()
{
    return QPointF(scenePos().x() + 5, scenePos().y() + 5);//scenePos()返回物品在场景中的坐标
}



ItemPort::~ItemPort()
{
//    qDebug() << QString("~DevicePortItem %1").arg(_portName);
}


int ItemPort::type() const {
    return Type;
}

ItemPort* ItemPort::clone()
{
    ItemPort* newPort = NULL;
    if(_portType == PortType_Inflection)
    {
        QPointF pos = scenePos();
        newPort = new ItemPort(PortType_Inflection, QString(""), 1, true, pos, 1, this);
    }
    return newPort;
}


void ItemPort::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
//    qDebug() << __FUNCTION__ << __LINE__ << portName() << _isMute << _EnableMute;
    if(_isMute && _EnableMute) {
        if (_portType != PortType_Inflection && _portType != PortType_ChannelBus
                && _portType != PortType_Config && _portType != PortType_CtrlPin) {
            painter->setPen(QPen(ItemPort::MuteFillColor, _painterWidth));
            painter->setBrush(ItemPort::DefaultColor);
        }else {
            painter->setPen(QPen(_painterColor, _painterWidth));
            painter->setBrush(ItemPort::MuteFillColor);
        }
     }else {
        painter->setPen(QPen(_painterColor, _painterWidth));
        painter->setBrush(ItemPort::DefaultColor);
    }
    if(isSelected())
    {
        painter->setPen(QPen(Qt::red, _painterWidth));
    }

    painter->drawPath(_painterPath);//绘制项目的多边形
#ifdef QT_DEBUG
    //调试端口连线数量
//    if(_portType == PortType_Config || _portType == PortType_ChannelBus){
//        painter->setFont(SkinStyle::getFont(6));
//        painter->drawText(boundingRect().adjusted(3, 0, 0, 0), QString("%1").arg(_usedCount));
//    }
//    //调试行列值
//    if(_portType == PortType_Config || _portType == PortType_ChannelBus){
//        ConfigItem* cfgItem = qgraphicsitem_cast<ConfigItem*>(parentItem());
//        if(cfgItem != NULL){
//            int layer, row;
//            if(_isInput) cfgItem->getInputDef(_portNum, &layer, &row);
//            else cfgItem->getOutputDef(_portNum, &layer, &row);
//            if(layer != 65535 || row != 65535){
//                painter->setFont(SkinStyle::getFont(7));
//                painter->drawText(boundingRect().adjusted(-15, 0, 12, 0), QString("%1,%2").arg(layer).arg(row));
//            }
//        }
//    }
#endif
    switch (_portType) {
    case PortType_MONAURAL:
    case PortType_Dante:
    case PortType_AES67:
        painter->setFont(SkinStyle::getFont(7));
        paintText(painter);
        break;
    default:
        break;
    }
}

void ItemPort::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPathItem::mousePressEvent(event);
}



void ItemPort::paintText(QPainter *painter)//绘制端口名称的矩形
{
    painter->setPen(QPen(Qt::black, 1));
    if(_isInput){
        //10 端口宽度 4文本和端口的间距
        QRectF textRect = QRectF(10+4, -2, _maxTextPixel, TextHeightPixel);
        painter->drawText(textRect, Qt::AlignLeft, _portName);
    }
    else{
        QRectF textRect = QRectF(-_maxTextPixel -4, -2, _maxTextPixel, TextHeightPixel);
        painter->drawText(textRect, Qt::AlignRight, _portName);
    }
}

QRectF ItemPort::boundingRect() const//绘制端口矩形
{
    return _sharpPath.boundingRect();
}


QPainterPath ItemPort::shape() const
{
    return _sharpPath;
}



void ItemPort::initPort(DevPortType &type, QString portName, int portNum, bool isInput)
{
    if(type == PortType_Inflection) {
        return;
    }

    QChar typeChar;
    QPolygonF polygon;
    switch(type)
    {
    case PortType_MONAURAL://单声道端口
        typeChar = 'A';
        if(isInput == true)
            polygon << QPointF(1.5, 0.5) << QPointF(9.5, 5.5) << QPointF(1.5, 9.5) << QPointF(1.5, 0.5); //三角形
        else
            polygon << QPointF(0.5, 5.5) << QPointF(9.5, 0.5) << QPointF(9.5, 9.5) << QPointF(0.5, 5.5);
       _painterPath.addPolygon(polygon);
        break;
    case PortType_Dante://网络端口
        typeChar = 'N';
        _painterPath.addRect(1.5, 1.5, 8, 8);
        break;
    case PortType_AES67://AES67
        typeChar = 'N';
        polygon << QPointF(0.5, 5.5) << QPointF(5.5, 0.5) << QPointF(10.5, 5.5) << QPointF(5.5, 10.5)<< QPointF(0.5, 5.5);//菱形
        _painterPath.addPolygon(polygon);
        break;
    case PortType_Config://算法模块
        _painterPath.addEllipse(0.5, 0.5, 9, 9);
        break;
    case PortType_CtrlPin://控制端口
        _painterPath.addRoundedRect(1, 1, 8, 8, 1, 1);
        break;
    case PortType_RS232://串口
        polygon << QPointF(2.5, 0.5) << QPointF(7.5, 2.5) << QPointF(7.5, 7.5) << QPointF(2.5, 9.5) << QPointF(2.5, 0.5);
        _painterPath.addPolygon(polygon);
        break;
    case PortType_Inflection:
        _painterPath.addEllipse(0.5, 0.5, 10, 10);
        break;
    case PortType_ChannelBus:
        polygon << QPointF(0.5, 0.5) << QPointF(5.5, 3.5) << QPointF(9.5, 0.5)
                << QPointF(9.5, 9.5) << QPointF(5.5, 6.5) << QPointF(0.5, 9.5) << QPointF(0.5, 0.5);
        _painterPath.addPolygon(polygon);
        break;
    }
    setPath(_painterPath);
    if (portName.isNull() || portName.isEmpty()){
        _portName = QString("%1_%2").arg(typeChar).arg(_portNum +1, 2, 10, QChar('0'));
    }
    else { _portName = portName;}
}

void ItemPort::initPort_FangTu(DevPortType &type, QString portName, int portNum, bool isInput)
{
    if(type == PortType_Inflection){
        return;
    }

    QString typeChar_Ft;
    QPolygonF polygon;
    switch(type)
    {
    case PortType_MONAURAL://单声道端口
        if(isInput)
            typeChar_Ft = "A";
        else
            typeChar_Ft = "A";
        _painterPath.addEllipse(2, 2, 8, 8);
        _painterColor = QColor(148,78,64);
        DefaultColor = Qt::transparent;
        _painterWidth = 4;
        break;
    case PortType_Dante://网络端口
        if(isInput)
            typeChar_Ft = "N";
        else
            typeChar_Ft = "N";
        _painterPath.addEllipse(2, 2, 8, 8);
        _painterColor = QColor(36,90,145);
        DefaultColor = Qt::transparent;
        _painterWidth = 4;
        break;
    case PortType_AES67://AES67
        if(isInput)
            typeChar_Ft = "N";
        else
            typeChar_Ft = "N";
        _painterPath.addEllipse(2, 2, 8, 8);
        _painterColor = QColor(74,111,58);
        DefaultColor = Qt::transparent;
        _painterWidth = 4;
        break;
    case PortType_Config://算法模块
        _painterPath.addEllipse(0.5, 0.5, 9, 9);
        _painterColor = QColor(69, 156, 99);
        break;
    case PortType_CtrlPin://控制端口
        _painterPath.addRoundedRect(1.5, 1.5, 8, 8, 1, 1);
        break;
    case PortType_Inflection:
        _painterPath.addEllipse(0.5, 0.5, 10, 10);
        break;
    case PortType_ChannelBus:
        polygon << QPointF(0.5, 0.5) << QPointF(5.5, 3.5) << QPointF(9.5, 0.5)
                << QPointF(9.5, 9.5) << QPointF(5.5, 6.5) << QPointF(0.5, 9.5) << QPointF(0.5, 0.5);
        _painterPath.addPolygon(polygon);
        _painterColor = QColor(69, 156, 99);
        break;
    }
    setPath(_painterPath);
    if (portName.isNull() || portName.isEmpty()){
        _portName = QString("%1%2").arg(typeChar_Ft).arg(_portNum +1, 2, 10, QChar('0'));
    }
    else { _portName = portName;}
}




