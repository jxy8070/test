#ifndef DEVICEITEM_ROOM_H
#define DEVICEITEM_ROOM_H
#include "deviceItem.h"

class DeviceItem_Room: public DeviceItem
{
    Q_OBJECT

public:
    DeviceItem_Room(QString projFilePath, QPointF scenePos, QGraphicsItem *parent);

    inline const QString& roomProject() {return _projFilePath;}
    inline const QString& roomName() {return _roomName;}
    void setRoomName(QString name);
    inline void setValid(bool valid) { _isValid = valid; }
//    DesignItem* clone(DesignItem* toItem) override;
    void setInputOutPorts(QMap<int, QList<QString>> Inports, QMap<int, QList<QString>> Outports);
    void loadFromSettings();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QString _projFilePath;
    QString _roomName;
    bool _isValid;
};

#endif // DEVICEITEM_ROOM_H
