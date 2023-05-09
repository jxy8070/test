#ifndef MONITORITEM_H
#define MONITORITEM_H

#include <QGraphicsRectItem>
#include <QImage>
#include "config.h"
#include <QPainter>
#include <QTimer>
#include <QColor>
#include <QFont>

class MonitorItem : public QGraphicsRectItem
{

public:
    inline CfgType getType() { return _cfgType; }
    inline MonitorItem* getID() { return this; }
    inline QString getTypeName() { return _monitorName; }
    inline void setMonitorName(QString name) { _monitorName = name; }
    inline QPointF getMonitorScenePos() { return _scenePos; }
    inline qreal getMonitorHeight() { return _rectHeight; }
    inline qreal getMonitorWidth() { return _rectWidth; }
    inline void setScenePos(QPointF pos) { _scenePos = pos; }
    inline void setRectSize(QPointF pos) { _rectWidth  = pos.rx();
                                           _rectHeight = pos.ry();
                                           update(); }
    inline int DisplayStyle() { return _displayStyle; }
    ////////////////NetWorkDevType/////////////
    inline void setDevType(DevType type) { _devType = type; }
    inline DevType getDevType() { return _devType; }
    ////////////////NetWorkDevIP///////////////
    inline void setDevIP(QString ip) { _devIP = ip; }
    inline QString getDevIP() { return _devIP; }
    //////////////color//////////////
    inline QColor getFillColor() {return _fillColor;}
    inline QColor getTitilColor() {return _textColor;}
    inline void setFillColor(QColor color) {_fillColor = color; update();}
    inline void setTitleColor(QColor color) {_textColor = color; update();}
    /////////////Font///////////////
    inline QFont textFont() { return _textFont; }
    /////////////Network////////////
    inline QString getNetworkDevType() { return _networkDev; }
    ///////////text//////////////////
    inline QString getText() { return _text; }
    ///////////video ID/////////////
    inline void setVideoID(QString ID) { _videoID.clear(); _videoID.append(ID); }
    inline QString getVideoID() { return _videoID; }

    ////////////image///////////////
    void setDisplayStyle(int i);
    void setDisplayStyle(QString devType);
    QString getDisplayStyle();



    void setTextFout(QFont font);
    void point_button(QPainter *painter);

protected:
    MonitorItem(CfgType cfgtype, QString monitorName, QImage image,
                QPointF scenePos, qreal rectWidth, qreal rectHeight, QGraphicsItem *parent);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString getStringType();

signals:

public slots:
    void setOnLineClicked(bool b);

protected:
    qreal _rectWidth;
    qreal _rectHeight;
    qreal _textLength;

private:
    CfgType _cfgType;
    QImage _image;
    int _displayStyle;    //display Style
    QString _networkDev;  //NetworkDevice
    QString _devIP;       //NetworkDevIP
    DevType _devType;     //NetworkDevType
    QString _monitorName;
    QString _defaultDeviceItemName;
    QPointF _scenePos;

    QTimer *_timer;
    QColor _fillColor;
    QColor _textColor;
    QFont _textFont;
    QString _text;       //button按键的“M”、“Ø”
    QString _videoID;    //视频流地址

};

#endif // MONITORITEM_H
