#ifndef GRAPHICSTEXTITEM_H
#define GRAPHICSTEXTITEM_H

#include "graphicsitem.h"
#include <QTextEdit>
#include <QGraphicsProxyWidget>
#include "../overwriteitem.h"

class GraphicsTextItem : public GraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QString Text READ getText WRITE setText)
//    Q_PROPERTY(int FontSize READ getFontSize WRITE setFontSize)
//    Q_PROPERTY(int Alignment MEMBER _Alignment)
    Q_PROPERTY(QColor FontColor MEMBER _fontColor)
    Q_PROPERTY(QFont Font MEMBER _font)
    Q_PROPERTY(int BorderWidth MEMBER _borderWidth)
    Q_PROPERTY(int BorderRadius MEMBER _borderRadius)
    Q_PROPERTY(QColor BorderColor MEMBER _borderColor)
    Q_PROPERTY(QColor BackgoundColor MEMBER _backgroundColor)


public:
    explicit GraphicsTextItem(QPainterPath path, QGraphicsItem *parent = nullptr);
signals:
public slots:
    void onTextChangedSlot();
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QTextEdit *_textEdit;
    GraphicsProxyWidget *_proxWid;
};

#endif // GRAPHICSTEXTITEM_H
