#ifndef GRAPHICSGROUPBOXITEM_H
#define GRAPHICSGROUPBOXITEM_H
#include "graphicsitem.h"

class GraphicsGroupBoxItem : public GraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QString Text MEMBER _text)
    Q_PROPERTY(QColor TextColor MEMBER _fontColor)
    Q_PROPERTY(QFont Font MEMBER _font)
    Q_PROPERTY(int BorderWidth MEMBER _borderWidth)
    Q_PROPERTY(int BorderRadius MEMBER _borderRadius)
    Q_PROPERTY(QColor BorderColor MEMBER _borderColor)
    Q_PROPERTY(QColor BackgoundColor MEMBER _backgroundColor)
public:
    GraphicsGroupBoxItem(QPainterPath path, QGraphicsItem *parent = nullptr);
    QPainterPath shape() const override;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void refreshBorderPath();
private:

};

#endif // GRAPHICSGROUPBOXITEM_H
