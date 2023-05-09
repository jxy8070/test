#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H
#include "../DesignItem.h"

class GraphicsItem : public DesignItem
{
public:
    enum { Type = UserType + 5 };
    enum GraphicsItemType {
        GrapItemType_text,
        GrapItemType_groupBox,
        GrapItemType_polyLine,
    };

    DesignItem *clone(DesignItem *toItem) override;
    int checkZValue(void) override;
    int type() const override;
    void initParameter();

    inline bool isInside() { return _isInside; };
    inline QString getText() { return _text; }
    inline void setText(QString text) { _text = text; }
    inline QString getOriginalText() { return _originalText; }
    inline void setOriginalText(QString text) { _originalText = text; }
    inline QFont getFont() { return _font; }
    inline void setFont(QFont font) { _font = font; }
    inline QColor getFontColor() { return _fontColor; }
    inline void setFontColor(QColor color) { _fontColor = color; }
    inline int getTextAlign() { return _textAlignment; }
    inline void setTextAlign(int align) { _textAlignment = align; }
    inline QPainterPath getBorderPath() { return _borderPath; }
    inline void setBorderPath(QPainterPath path) { _borderPath = path; }
    inline int getBorderWidth() { return _borderWidth; }
    inline void setBorderWidth(int w) { _borderWidth = w; }
    inline int getBorderRadius() { return _borderRadius; }
    inline void setBorderRadius(int r) { _borderRadius = r; }
    inline QColor getBorderColor() { return _borderColor; }
    inline void setBorderColor(QColor color) { _borderColor = color; }
    inline QColor getbackGroundColor() { return _backgroundColor; }
    inline void setBackgraoundColor(QColor color) { _backgroundColor = color; }

    inline int getFontSize() {return _font.pixelSize(); }
    inline void setFontSize(int size) { _font.setPixelSize(size); }
    inline QString getFontFamily() { return _font.family(); }
    inline void setFontFanmily(QString fam) { _font.setFamily(fam); }
    inline bool getFontBold() { return _font.bold(); }
    inline void setFontBold(bool b) { _font.setBold(b); }
    inline bool getFontItalic() { return _font.italic(); }
    inline void setFontItalic(bool ita) { _font.setItalic(ita); }
    inline GraphicsItemType getGrapItemType() { return _grapItemType; }

    virtual void loadDataFromSettings(QSettings *);
    virtual void saveDataToSettings(QSettings *);
protected:
    GraphicsItem(QPainterPath path, GraphicsItemType type, QGraphicsItem *parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    static QRect getTextRect(QFont font, QString text);
protected:
    QString _text;              //标题
    QString _originalText;      //原始标题
    QFont _font;                //字体
    QColor _fontColor;          //字体颜色
    int _textAlignment;         //字体对齐方式
    QPainterPath _borderPath;   //边框轨迹
    int _borderWidth;           //边框宽度
    int _borderRadius;          //圆角半径
    QColor _borderColor;        //边框颜色
    QColor _backgroundColor;    //背景色

    bool _isInside;
    GraphicsItemType _grapItemType;

    QList<QPointF> _sceneItemsPoint;
};

#endif // GRAPHICSITEM_H
