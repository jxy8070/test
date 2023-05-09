#include "stable.h"
#include "eqdragpoint.h"
#include "eqgraph.h"

/////////////////////////////////////////////////////////
///        \brief EqDragPoint类 调EQ的拖拽点            ///
/////////////////////////////////////////////////////////
EqDragPoint::EqDragPoint(EqGraph* view, int idx, Drag_Type type, QGraphicsItem *parent):
                               QGraphicsEllipseItem( parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);//设置此项接受鼠标事件的左键点击
    _view = view;
    _isCurrent = false;
    _idx = idx;
    _text = /*QString::number(idx+1)*/" ";
    _type = type;
    //中心点设置到圆心
    switch(type)
    {
    case Drag_Normal:
        setRect(-RADIUS, -RADIUS, RADIUS*2, RADIUS*2);//设置椭圆的矩形形状
        break;
    case Drag_3Point:
    {
        setRect(-RADIUS, -RADIUS, RADIUS*2, RADIUS*2);
        EqDragPoint *sub1 = new EqDragPoint(view, idx, Drag_Sub, this);
        sub1->setPos(pos().x()-20, pos().y());//设置sub1的坐标位置
        EqDragPoint *sub2 = new EqDragPoint(view, idx, Drag_Sub, this);
        sub2->setPos(pos().x()+20, pos().y());
        break;
    }
    case Drag_Sub:
        setRect(-SUB_RADIUS, -SUB_RADIUS, SUB_RADIUS*2, SUB_RADIUS*2);
        break;
    case Drag_2Point_HP_LP:
        setRect(-HP_LP_RADIUS, -HP_LP_RADIUS, HP_LP_RADIUS*2, HP_LP_RADIUS*2);
        if (idx == 0)
        {
            _text = "H";
        }
        else
        {
            _text = "L";
        }
        break;
    }
}

void EqDragPoint::setRestrictRect(QRect restrictRect)
{
    _restrictRect = restrictRect;
    QPointF curPos = pos();//返回项在场景中的坐标位置
    if(!_restrictRect.contains(curPos))//如果该项位置不包含在矩形范围内
    {
        restrictDragPoint(curPos);
    }
    setPos(curPos);//设置坐标位置
}

void EqDragPoint::setPosAndRestrict(int x, int y)
{
    QPointF pos(x, y);
    restrictDragPoint(pos);
    setPos(pos);
}

void EqDragPoint::restrictDragPoint(QPointF& curPos)//保证坐标在矩形框内部
{
    //如果curPos在场景中的x轴的坐标 < 矩形左边的x轴坐标
    if(curPos.rx() < _restrictRect.left()) curPos.setX(_restrictRect.left());
    if(curPos.rx() > _restrictRect.right()) curPos.setX(_restrictRect.right());
    //如果curPos在场景中的y轴坐标 > 矩形上边缘的y轴坐标
    if(curPos.ry() < _restrictRect.top()) curPos.setY(_restrictRect.top());
    //底部边缘
    if(curPos.ry() > _restrictRect.bottom()) curPos.setY(_restrictRect.bottom());
}

void EqDragPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)//鼠标按下事件
{
    Q_UNUSED(event);
    _isCurrent = true;
    if(_isCurrent) _view->setCurrentDragPoint(this);

    _view->dispPointLabel(true, _idx, this->pos());
}

void EqDragPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF newP = event->pos();
    QPointF oriP = event->lastPos();
    QPointF curPos = this->pos() + newP - oriP;
    restrictDragPoint(curPos);
    setPos(curPos);

    _view->eqParaChanged(_idx, (int)curPos.rx(), (int)curPos.ry());
    _view->setPointLabelPos(_idx, this->pos());
}

void EqDragPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    _view->dispPointLabel(false);
}

void EqDragPoint::setIsCurrent(bool isCurrent)
{
    _isCurrent = isCurrent;
    update();
}

void EqDragPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(_isCurrent)
    {
        painter->setBrush(QBrush(QColor(255, 0, 0, 180), Qt::SolidPattern));
        painter->setPen(QPen(QColor(255, 255, 255), 1));
    }
    else{
        painter->setBrush(QBrush(QColor(150, 150, 150, 180), Qt::SolidPattern));
        painter->setPen(QPen(QColor(255, 255, 255), 1));
    }

    painter->drawEllipse(rect()/*QRect(-RADIUS, -RADIUS, RADIUS*2, RADIUS*2)*/);
    painter->setPen(QPen(QColor(255, 255, 255), 2));
    painter->drawText(this->rect(),  Qt::AlignCenter, _text); //显示序号
}
