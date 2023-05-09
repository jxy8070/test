#ifndef GRAPHICSPOLYLINEITEM_H
#define GRAPHICSPOLYLINEITEM_H
#include "graphicsitem.h"

class graphicsPolylineItem : public GraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QString Text MEMBER _text)
    Q_PROPERTY(QColor TextColor MEMBER _fontColor)
//    Q_PROPERTY(int FontSize READ getFontSize WRITE setFontSize)
    Q_PROPERTY(QFont Font MEMBER _font)
    Q_PROPERTY(int BorderWidth MEMBER _borderWidth)
//    Q_PROPERTY(int BorderRadius MEMBER _borderRadius)
    Q_PROPERTY(QColor BorderColor MEMBER _borderColor)
    Q_PROPERTY(QColor BackgoundColor MEMBER _backgroundColor)
public:
    graphicsPolylineItem(QPainterPath pPath, QGraphicsItem *parent = nullptr);
    QPainterPath shape() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    void refreshBorderPath();
    QPainterPath refreshIsSelectedPath();
private:
    typedef struct xyRatio{
        qreal s_x;
        qreal s_y;
    }xyRatio_t;
    QList<xyRatio_t> _xyRatio;  //横纵比例
};

#endif // GRAPHICSPOLYLINEITEM_H
