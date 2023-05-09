#ifndef DESIGNITEM_H
#define DESIGNITEM_H

#include <QObject>
#include <QGraphicsRectItem>
#include "ItemPort.h"
#include "LineItem.h"


class QtProperty;
class DesignItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    DesignItem();
    virtual DesignItem* clone(DesignItem* toItem) = 0;
    virtual int checkZValue(void) = 0;
    enum { Type = UserType + 1 };
    int type() const override;
    inline QColor getFillColor() {return _fillColor;}
    inline void setFillColor(QColor color) {_fillColor = color; update();}
    static QPainterPath getRoundRectPath(QRectF rect, int tlRadius, int trRadius, int blRadius, int brRadius);
    static QPainterPath getBottomRoundRectPath(QRectF rect, int radius);

public:
    virtual void changePropertyValue(const char* propName, const QVariant& newVal);
    virtual void initAfterAdd(){;}
    virtual void clearBeforeDelete(){;}
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

protected:
    QColor _fillColor;
    QColor _textColor;

private:
    bool _isMovingStarted;
};

#endif // DESIGNITEM_H
