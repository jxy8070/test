#ifndef DEVICELINEITEM_H
#define DEVICELINEITEM_H


#include <QGraphicsLineItem>

class AbsGraphScene;
class ItemPort;
class DesignItem;
class LineItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT    
    Q_PROPERTY(QColor LineColor READ lineColor WRITE setLineColor)
    friend QDataStream& operator<<(QDataStream& ds, LineItem& connect);
//    friend QDataStream& operator>>(QDataStream& ds, ConnectionItem& connect);
public:
    explicit LineItem(AbsGraphScene *scene, ItemPort *startPort, ItemPort *endPort, QGraphicsItem *parent = 0);
    ~LineItem();
    LineItem* clone(ItemPort *sPort, ItemPort *ePort);
    enum { Type = UserType + 3 };
    int type() const override;

    void addpoint(ItemPort *point, ItemPort *before, ItemPort *after);
    void delpoint(ItemPort *point, ItemPort *&before, ItemPort *&after);
    void initAfterAdd();
    void clearBeforeDelete();

    DesignItem *getStartDevice();
    DesignItem *getEndDevice();
    bool isParentOfPort(DesignItem* item);
    void flashVisibleByPort();

    ItemPort *getStartPort() {return _startPort;}
    ItemPort *getEndPort() {return _endPort; }
    void setEndPort(ItemPort * endPort) {_endPort = endPort;}
    void setNormalization(); //标准化，确保startPort为输出端口

    QColor lineColor(void) { return _lineColor; }
    void setLineColor(QColor color) { _lineColor = color; }

    int getUserStartZValue() { return _inOutItemZValue[0]; }
    int getUserEndZValue() { return _inOutItemZValue[1]; }
    void setUserZValue(ItemPort *startPort, ItemPort *endPort);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void paint_Defult(QPainter *painter);
    void paint_FangTu(QPainter *painter);

signals:

private:
    bool pointDetect(ItemPort *start, ItemPort *end, ItemPort *cur);
    QList<ItemPort *> _pointList;
    ItemPort * _startPort;
    ItemPort * _endPort;
    AbsGraphScene *_scene;
    QColor _lineColor;

    int _inOutItemZValue[2] = {0};  //输入输出CfgItem的 UserZValue 值  0:input, 1:Output
};

#endif // DEVICELINEITEM_H
