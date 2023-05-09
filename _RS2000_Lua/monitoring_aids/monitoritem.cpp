#include "monitoritem.h"
#include "monitorblackstage.h"


void MonitorItem::setDisplayStyle(int i)
{
    if(_cfgType == CFG_TYPE_1){
        if(i == 1) { _rectWidth = 60; _rectHeight = 297; }
        else if(i == 5) { _rectWidth =298; _rectHeight = 59; }
        else{ _rectWidth = 85; _rectHeight = 879; }

        _image = QImage(QString("://Image/meter%1").arg(i));
    }
    if(_cfgType == CFG_TYPE_2)
    {
        _image = QImage(QString("://Image/clipper%1").arg(i));
        if(i == 4){ _rectWidth = 68; _rectHeight = 68; }
        else { _rectWidth = 80, _rectHeight = 297; }

    }
    if(_cfgType == CFG_TYPE_3)/*{ _image = QImage(QString("://Image/button%1").arg(i)); }*/
    {
//        _image = QImage(NULL);
        switch(i)
        {
        case 1:
            _rectWidth = 35;
            _rectHeight = 35;
            break;
        case 2:
//            _points[4] = {QPointF(0,25+17), QPointF(25,17), QPointF(50,25+17), QPointF(25,50+17)};
            break;
        case 3:
        case 4:
        case 5:
            _rectWidth = 65;
            _rectHeight = 35;
            break;
        default:
            break;
        }
    }
    if(_cfgType == CFG_TYPE_5) { _image = QImage(QString("://Image/livestreaming%1").arg(i)); }
//    _rectWidth  = _image.rect().width();
//    _rectHeight = _image.rect().height();
    update();
    _displayStyle = i;
}

void MonitorItem::setDisplayStyle(QString devType)
{
    if(devType == "NAP-100")    { _image = QImage("://Image/type/NAP-100.png"); }
    if(devType == "NAP-100i")   { _image = QImage("://Image/type/NAP-100i.png"); }
    if(devType == "SPK-Mono")   { _image = QImage("://Image/type/SPK-Mono.png"); }
    if(devType == "SPK-Stereo") { _image = QImage("://Image/type/SPK-Stereo.png"); }
    if(devType == "Mic-Stereo") { _image = QImage("://Image/type/Mic-Stereo.png"); }
    if(devType == "MIC-Quad")   { _image = QImage("://Image/type/MIC-Quad.png"); }
    if(devType == "AMP-Quad")   { _image = QImage("://Image/type/AMP-Quad.png"); }
    if(devType == "RT-MIC-Stereo") { _image = QImage("://Image/type/RT-MIC-Stereo.png"); }
    if(devType == "RT-Line-Stereo"){ _image = QImage("://Image/type/RT-Line-Stereo.png"); }
    if(devType == "60W Poe")    { _image = QImage("://Image/type/60W Poe.png"); }
    if(devType == "AMP Module") { _image = QImage("://Image/type/AMP Module.png"); }
    update();
    _networkDev.clear();
    _networkDev.append(devType);
}

QString MonitorItem::getDisplayStyle()
{
    switch(_displayStyle)
    {
    case 1: return "display-1";
    case 2: return "display-2";
    case 3: return "display-3";
    case 4: return "display-4";
    case 5: return "display-5";
    default:
        return "No Style";
    }
}


void MonitorItem::setTextFout(QFont font)
{
    _textFont = font;
    update();
}

void MonitorItem::point_button(QPainter *painter)
{

    if(isSelected())//矩形被选中
    {
        painter->setPen(QPen(Qt::red, 3));
        painter->setBrush(Qt::NoBrush);
        if(_displayStyle == 2)
        {
            QPointF points[4] = {QPointF(0-2,25+17), QPointF(25,17-2), QPointF(50+2,25+17), QPointF(25,50+17+2)};
            painter->drawPolygon(points, 4);
        }else
        {
            painter->drawRect(-2, 15, _rectWidth+4,  _rectHeight/* -17-1*/+4);
        }
    }
    //button矩形框
    painter->setPen(QPen(_textColor, 2));
    painter->setBrush(_fillColor);
    if(_displayStyle == 2){
        QPointF points[4] = {QPointF(0,25+17), QPointF(25,17), QPointF(50,25+17), QPointF(25,50+17)};
        painter->drawPolygon(points,4);
    }else{
        painter->drawRoundedRect(0, 17, _rectWidth,  _rectHeight /*-17-1*/, 5, 5);
    }
    //名称
    QString newName = getStringType() + " <" + /*_defaultDeviceItemName*/getTypeName() + ">";
    painter->setPen(QPen(_textColor, 2));
    painter->setFont(/*QFont("Arial", 8)*/_textFont);
    painter->drawText(QRectF(0, _rectHeight+17, _rectWidth, 17), Qt::AlignCenter, newName);
    if(_displayStyle == 4)
    {
       _text = "M";
       painter->drawText(QRectF(0, _rectHeight/3+17, _rectWidth, 17), Qt::AlignCenter, _text);
    }
    if(_displayStyle == 5)
    {
        _text = "Ø";
        painter->drawText(QRectF(0, _rectHeight/3+17, _rectWidth, 17), Qt::AlignCenter, _text);
    }
}

MonitorItem::MonitorItem(CfgType cfgtype, QString monitorName, QImage image,
                         QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    setFlag(QGraphicsRectItem::ItemIsSelectable, true);//通过单机项目或者使用框选来选择项目
    setFlag(QGraphicsRectItem::ItemIsMovable, true);//单机项目然后拖动，项目会和光标一起移动
    setAcceptHoverEvents(true);//接受悬停事件

    _rectWidth  = rectWidth;
    _rectHeight = rectHeight;
    setRect(-2, -2,_rectWidth+4, _rectHeight+4);
    setPos(scenePos);//设置项的位置为scenePos

    _devIP.clear();
    _scenePos = scenePos;
    _cfgType = cfgtype;
    _monitorName = monitorName;
    _displayStyle = 1;
    _networkDev.clear();
    _text = "None";
    _videoID = "https://d2zihajmogu5jn.cloudfront.net/bipbop-advanced/bipbop_16x9_variant.m3u8";
    if(_cfgType != CFG_TYPE_4)
    {
        _image = image;
    }else {
//        _selectRectSize = new SelectRectSize();
    }

//    _rectWidth  = _image.rect().width();
//    _rectHeight = _image.rect().height();

    if(_cfgType == CFG_TYPE_3)
    {
        _textColor  = Qt::black;
        _fillColor  = Qt::gray;
        _textFont.setPointSize(12);
    }else
    {
        _textColor  = Qt::white;
        _fillColor  = Qt::darkRed;
        _textFont.setPointSize(8);
    }
}

void MonitorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setRect(-2, -2,_rectWidth+4, _rectHeight+4);//设置选中范围的大小
    if(_cfgType == CFG_TYPE_3)
    {
        point_button(painter);
    }else
    {
        if (isSelected())
        {
            painter->setPen(QPen(Qt::red, 3));
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(-2, -2, _rectWidth+4,  _rectHeight/* -17-1*/+4);

//            if(_cfgType == CFG_TYPE_4)
//            {
////                _selectRectSize->drawSizeRect(painter, _rectWidth, _rectHeight);

//                painter->setPen(QPen(Qt::blue, 3));
//                painter->setBrush(Qt::NoBrush);
//                painter->drawRect(-4 ,-4, 4, 4);
//                painter->drawRect(_rectWidth/2-2,-4, 4, 4);
//                painter->drawRect(_rectWidth, -4, 4, 4);
//                painter->drawRect(_rectWidth, _rectHeight/2-2, 4, 4);
//                painter->drawRect(_rectWidth, _rectHeight, 4, 4);
//                painter->drawRect(_rectWidth/2-2, _rectHeight, 4, 4);
//                painter->drawRect(-4, _rectHeight, 4, 4);
//                painter->drawRect(-4, _rectHeight/2-2, 4, 4);
//            }
        }

        //设备圆角矩形框
        painter->setPen(QPen(_textColor, 2));
        painter->setBrush(_fillColor);
        painter->drawRoundedRect(0, 0, _rectWidth,  _rectHeight /*-17-1*/, 5, 5);


        if (!_image.isNull())//监控设备会用到图片
        {
            QPixmap pixmapToShow = QPixmap::fromImage(_image.scaled(QSize(_rectWidth-2, _rectHeight-2), Qt::KeepAspectRatioByExpanding) );
            painter->drawPixmap(1, 1, pixmapToShow);
        }
        //设备名称
        QString newName = getStringType() + " <" + /*_defaultDeviceItemName*/getTypeName() + ">";
        painter->setPen(QPen(_textColor, 2));
        painter->setFont(/*QFont("Arial", 8)*/_textFont);
        if(_cfgType == CFG_TYPE_4){
            painter->drawText(QRect(0, _rectHeight/3, _rectWidth, 17), Qt::AlignCenter, newName);
        }else{
            painter->drawText(QRectF(0, _rectHeight, _rectWidth, 17), Qt::AlignCenter, newName);
        }
    }

}

QString MonitorItem::getStringType()
{
    switch (_cfgType) {
    case CFG_TYPE_1: return "Meter";
    case CFG_TYPE_2: return "clipper";
    case CFG_TYPE_3: return "button";
    case CFG_TYPE_4: return "label";
    case CFG_TYPE_5: return "live streaming";
    case CFG_TYPE_SCENE: return "Scene";
    case CFG_TYPE_NETWORKDEVICE: return _networkDev/*"Network device"*/;
//    case CFG_TYPE_DEV_NAP_100: return "NAP_100";
//    case CFG_TYPE_DEV_NAP_100i: return "NAP_100i";
//    case CFG_TYPE_DEV_Dante_Sound_Card: return "Dante_Sound_Card";
//    case CFG_TYPE_DEV_AES67_Sound_Card: return "AES67_Sound_Card";
//    case CFG_TYPE_DEV_SPK_Mono: return "SPK_Mono";
//    case CFG_TYPE_DEV_SPK_Stereo: return "SPK_Stereo";
//    case CFG_TYPE_DEV_MIC_Stereo: return "MIC_Stereo";
//    case CFG_TYPE_DEV_MIC_Quad: return "MIC_Quad";
//    case CFG_TYPE_DEV_AMP_Quad: return "AMP_Quad";
//    case CFG_TYPE_DEV_RT_MIC_Stereo: return "RT_MIC_Stereo";
//    case CFG_TYPE_DEV_RT_Line_Stereo: return "TR_Line_Stereo";
//    case CFG_TYPE_DEV_60W_Poe: return "60W_Poe";
//    case CFG_TYPE_DEV_AMP_Module: return "AMP_Mondule";
    default: return "222UnKnow";
    }
}



void MonitorItem::setOnLineClicked(bool b)
{
    QPainter painter;
    painter.setPen(QPen(Qt::red, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(-2, 15, _rectWidth+4, _rectHeight/* -17-1*/+4, 5, 5);
}
