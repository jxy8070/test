#ifndef CONTROLTEMPLATE_H
#define CONTROLTEMPLATE_H

#include <QObject>
#include <QGraphicsItem>
#include "graphicsitem.h"

class ControlTemplate : public QGraphicsRectItem
{
public:
    enum { Type = UserType + 6,
           DataFlag_OldRect = 20,
           DataFlag_SceneOldRect,
           ResizeGrapItem_width = 8,
         };
    enum DragType{
        DragType_Release,
        DragType_LeftTop,
        DragType_Left,
        DragType_LeftBottom,
        DragType_Top,
        DragType_Bottom,
        DragType_RightTop,
        DragType_Right,
        DragType_RightBottom,
    };
    explicit ControlTemplate(QGraphicsItem *parent = nullptr);

    int type() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
public:
    void addIsSelectItem(QGraphicsItem *item);
    void addIsSelectItemList(QList<QGraphicsItem *> itemList);
    void setCustomRect(QPointF topLeft = _topLeft, QPointF bottomRight = _bottomRight);
    void clear();
    QRectF getNewCenterRectF();
signals:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void calcControlTemplateScaling(DragType dragType, QPointF startPoint, QPointF endPoint);
    void setIsSelectItemScale(DragType dragType, qreal scale, qreal zoom);
private:
    class resizeGraphicsItem : public QGraphicsRectItem
    {
    public:
        resizeGraphicsItem(DragType dragType, QGraphicsItem *parent)
            :QGraphicsRectItem(parent), _dragType(dragType)
        {
            this->setParentItem(parent);
            setAcceptHoverEvents(true);
            this->setRect(QRectF(0, 0, ResizeGrapItem_width, ResizeGrapItem_width));
            this->setPen(QPen(Qt::darkCyan));   // 边框颜色
            this->setBrush(QBrush(Qt::green));  // 填充颜色
        }
        inline DragType getDragType() { return _dragType; }
    protected:
        void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override{
            switch (_dragType) {
            case DragType_LeftTop:
            case DragType_RightBottom: this->setCursor(Qt::SizeFDiagCursor); break;
            case DragType_Left:
            case DragType_Right:       this->setCursor(Qt::SizeHorCursor);   break;
            case DragType_LeftBottom:
            case DragType_RightTop:    this->setCursor(Qt::SizeBDiagCursor); break;
            case DragType_Top:
            case DragType_Bottom:      this->setCursor(Qt::SizeVerCursor);   break;
            case DragType_Release:
            default: break;
            }
            QGraphicsRectItem::hoverEnterEvent(event);
        }
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override{
            this->setCursor(Qt::ArrowCursor);
            QGraphicsRectItem::hoverLeaveEvent(event);
        }

    private:
        DragType _dragType;
    };
private:
    QPointF _startPoint;                        //起始点
    DragType _CurrentDragType;                  //当前拖拽点类型
    QList<QGraphicsItem *> _isSelectItemList;   //已框选的item
    static QPointF _topLeft;                           //左上角
    static QPointF _bottomRight;                       //右下角
    //拖拽点
    QSharedPointer<resizeGraphicsItem> _leftTop_dragPoint;
    QSharedPointer<resizeGraphicsItem> _left_dragPoint;
    QSharedPointer<resizeGraphicsItem> _leftBottom_dragPoint;
    QSharedPointer<resizeGraphicsItem> _top_dragPoint;
    QSharedPointer<resizeGraphicsItem> _bottom_dragPoint;
    QSharedPointer<resizeGraphicsItem> _rightTop_DragPoint;
    QSharedPointer<resizeGraphicsItem> _right_DragPoint;
    QSharedPointer<resizeGraphicsItem> _rightBottom_DragPoint;

};

#endif // CONTROLTEMPLATE_H
