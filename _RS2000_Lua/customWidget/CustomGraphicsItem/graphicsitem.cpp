#include "graphicsitem.h"
#include "config.h"

GraphicsItem::GraphicsItem(QPainterPath path, GraphicsItemType type, QGraphicsItem *parent)
    :DesignItem(), _borderPath(path), _grapItemType(type)
{
    Q_UNUSED(parent);
    setFlags(GraphicsItemFlag::ItemIsFocusable | GraphicsItemFlag::ItemIsMovable
             | GraphicsItemFlag::ItemIsSelectable);
    setAcceptHoverEvents(true);
    initParameter();

}

#include "Frame/absgraphscene.h"
void GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    _sceneItemsPoint.clear();
//    foreach(QGraphicsItem *item, scene()->items()) {
//        if((item->type() == DesignItem::Type ||
//           item->type() == GraphicsItem::Type) && item != this) {
//           QPainterPath path = item->shape();
//           QPolygonF poly = path.toFillPolygon(QTransform::fromScale(1, 1));
//           QVector<QPointF>::iterator iter;
//           for(iter = poly.begin(); iter != poly.end(); iter++) {
//               _sceneItemsPoint.append(QPointF(iter->x(), iter->y()));
//           }
//        }
//    }
//    AbsGraphScene* pScene = qobject_cast<AbsGraphScene*>(scene());
//    if(event->button() == Qt::MouseButton::LeftButton)
    {
//        QPointF scenePoint = scenePos();
//        setData(DataKey_OldScenePos, scenePoint);
//        QPointF sPos = pScene->getCursorScenePos();
//        setData(DataKey_PressedScenePos, sPos);
//        QPointF itemPos = mapFromScene(sPos);
//        itemPos = QPointF(itemPos.x() -1, itemPos.y() -1);
//        setData(DataKey_PressedItemPos, itemPos);
//        qDebug() << __FUNCTION__ << __LINE__ << boundingRect() << pos() << scenePos();
    }
    QGraphicsItem::mousePressEvent(event);
}

void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    QPainterPath path = this->shape();
//    QPolygonF poly = path.toFillPolygon(QTransform::fromScale(1, 1));
//    QVector<QPointF>::iterator iter;
//    for(iter = poly.begin(); iter != poly.end(); iter++) {
//        _sceneItemsPoint.insert(*iter);
//    }
//    foreach(QPointF point, _sceneItemsPoint) {

//    }
    QGraphicsItem::mouseMoveEvent(event);
}

void GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    _sceneItemsPoint.clear();
    QGraphicsItem::mouseReleaseEvent(event);
}
#include "graphicsitemfactory.h"
DesignItem *GraphicsItem::clone(DesignItem *toItem)
{
    GraphicsItem *item = NULL;
    if(!toItem) {
        item = GraphicsItemFactory::CreateGraphicsItem(getGrapItemType(), shape());
    }
    if(item) {
        item->setPos(this->pos());

        item->_text         = this->_text;
        item->_originalText = this->_originalText;
        item->_textAlignment= this->_textAlignment;
        item->_font         = this->_font;
        item->_fontColor    = this->_fontColor;
        item->_borderPath   = this->_borderPath;
        item->_borderWidth  = this->_borderWidth;
        item->_borderRadius = this->_borderRadius;
        item->_borderColor  = this->_borderColor;
        item->_backgroundColor = this->_backgroundColor;
    }
    return item;
}

int GraphicsItem::checkZValue()
{
    return 0;
}

int GraphicsItem::type() const
{
    return Type;
}

void GraphicsItem::initParameter()
{
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu) {
        if(_grapItemType == GrapItemType_text) {
            if(Config::instance()->getLanguage() == LANGUAGE_ENGLISH)
                _text = QString("Text Label");
            else
                _text = QString::fromLocal8Bit("文本标签");
//            _text = QString(tr("Text Label"));
        }
        else if(_grapItemType == GrapItemType_groupBox) {
            if(Config::instance()->getLanguage() == LANGUAGE_ENGLISH)
                _text = QString("Group Box");
            else
                _text = QString::fromLocal8Bit("编组框");
//            _text = QString(tr("Group Box"));
        }
        else if(_grapItemType == GrapItemType_polyLine) {
            _text = QString();
        }
        _originalText = _text;
        _font = QFont("Microsoft YaHei", 10);
        _fontColor = QColor(Qt::white);
        _textAlignment = Qt::AlignCenter;
        _borderWidth = 2;
        _borderRadius = 6;
        _borderColor = QColor(136, 189, 149, 255);
        _backgroundColor = QColor(Qt::transparent);
    }else {
        _text = QString();
        _originalText = _text;
        _font = QFont("Microsoft YaHei", 10);
        _fontColor = Qt::black;
        _textAlignment = Qt::AlignCenter;
        _borderWidth = 1;
        _borderRadius = 6;
        _borderColor = QColor(Qt::black);
        _backgroundColor = QColor(Qt::transparent);
    }

//    _font.setPixelSize(13);
    _isInside = false;
}

void GraphicsItem::loadDataFromSettings(QSettings *ConfigIni)
{
    QPointF pos  = ConfigIni->value(QString(CONFIG_GRAPHICSITEM_POS)).toPointF();
    QString text = ConfigIni->value(QString(CONFIG_GRAPHICSITEM_TEXT)).toString();
    QString oglText = ConfigIni->value(QString(CONFIG_GRAPHICSITEM_ORIGINALTEXT)).toString();
    QColor fontColor = ConfigIni->value(QString(CONFIG_GRAPHICSITEM_FONTCOLOR)).value<QColor>();
    QFont font = ConfigIni->value(QString(CONFIG_GRAPHICSITEM_FONT)).value<QFont>();
    int textAlign = ConfigIni->value(QString(CONFIG_GRAPHICSITEM_TEXTALIGN)).toInt();

    int borderWidth = ConfigIni->value(QString(CONFIG_GRAPHICSITEM_BORDERWIDTH)).toInt();
    int borderRadius = ConfigIni->value(QString(CONFIG_GRAPHICSITEM_BORDERRADIUS)).toInt();
    QColor borderColor = ConfigIni->value(QString(CONFIG_GRAPHICSITEM_BORDERCOLOR)).value<QColor>();
    QColor bgdColor = ConfigIni->value(QString(CONFIG_GRAPHICSITEM_BGDCOLOR)).value<QColor>();

    setPos(pos);
    setText(text);
    setOriginalText(oglText);
    setFont(font);
    setFontColor(fontColor);
    setTextAlign(textAlign);
    setBorderWidth(borderWidth);
    setBorderRadius(borderRadius);
    setBorderColor(borderColor);
    setBackgraoundColor(bgdColor);
}

void GraphicsItem::saveDataToSettings(QSettings *ConfigIni)
{
    ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_POS), this->scenePos());
    ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_TYPE), this->getGrapItemType());
    ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_TEXT), this->getText());
    ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_ORIGINALTEXT), this->getOriginalText());
    ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_FONT), this->getFont());
    ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_FONTCOLOR), this->getFontColor());
    ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_TEXTALIGN), this->getTextAlign());
    ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_BORDERPATHCOUNT), this->getBorderPath().elementCount());
    qDebug() << __FUNCTION__ << __LINE__ << this->getBorderPath().elementCount();
    for(int i = 0; i < this->getBorderPath().elementCount(); i++) {
        QPointF point = this->getBorderPath().elementAt(i);
        ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_BORDERPATH).arg(i), point);
    }
    ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_BORDERWIDTH), this->getBorderWidth());
    ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_BORDERRADIUS), this->getBorderRadius());
    ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_BORDERCOLOR), this->getBorderColor());
    ConfigIni->setValue(QString(CONFIG_GRAPHICSITEM_BGDCOLOR), this->getbackGroundColor());
}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(Qt::transparent);
    if(isSelected()) {
        if(hasFocus()){
            painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }else {
            painter->setPen(QPen(Qt::red, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
        }
        painter->drawRoundedRect(boundingRect(), _borderRadius, _borderRadius);
//        painter->drawPath(_borderPath);
    }

    if(_isInside) {
        painter->setPen(QPen(QColor(0,255,0, 100), 2));
        painter->drawRect(boundingRect());
    }

//    painter->setPen(QPen(QColor(255,0,233), _borderWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    painter->drawRect(boundingRect());
}

void GraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    _isInside = true;
    setCursor(Qt::SizeAllCursor);
    QGraphicsRectItem::hoverEnterEvent(event);
}

void GraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    _isInside = false;
    setCursor(Qt::ArrowCursor);
    QGraphicsRectItem::hoverLeaveEvent(event);
}

QRect GraphicsItem::getTextRect(QFont font, QString text)
{
    QFontMetrics fm(font);
    QString str = text + "jg\0";
    return fm.boundingRect(str);
}

