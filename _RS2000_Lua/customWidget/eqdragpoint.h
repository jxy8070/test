#ifndef EQDRAGPOINT_H
#define EQDRAGPOINT_H
#include <QGraphicsEllipseItem>
#include <QRect>

class EqGraph;
class QPainter;
enum Drag_Type
{
    Drag_Normal,
    Drag_Sub,
    Drag_3Point,
    Drag_2Point_HP_LP,
};

//Eq小圆点
class EqDragPoint : public QGraphicsEllipseItem//可以添加到QGraphicscene的椭圆
{
public:
    enum {
        RADIUS = 5,
        SUB_RADIUS = 4,
        HP_LP_RADIUS = 8,
    };
    EqDragPoint(EqGraph* view, int idx, Drag_Type type, QGraphicsItem *parent =NULL);

    void setRestrictRect(QRect restrictRect);
    int getIndex() {return _idx;}
    void setIsCurrent(bool isCurrent);
    void setPosAndRestrict(int x, int y); //设置坐标点位置，并限制范围
    void setText(QString text) {_text = text;}

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    void restrictDragPoint(QPointF &curPos); //限制拖拽点位置在区域内
private:
    Drag_Type _type;
    EqGraph* _view;
    QString _text;
    int _idx;
    bool _isCurrent;
    QRectF _restrictRect;
};
#endif // EQDRAGPOINT_H
