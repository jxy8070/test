#include "configmixernewpushbutton.h"
#include <QContextMenuEvent>
#include <QWidgetAction>
#include <QBoxLayout>
#include <QMenu>
#include <QDebug>

configMixerNewPushButton::configMixerNewPushButton(QWidget *parent) :
    QPushButton(parent)
{
    _value = 0;

    QWidget *popup = new QWidget(this);

    slider = new QSlider(/*Qt::Horizontal, */popup);
    slider->setRange(-100, 0);
    slider->setTickPosition(QSlider::TicksAbove);
    slider->setSliderPosition(0);
    slider->setSingleStep(1);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(buttonDispChanged(int)));

    QBoxLayout *popupLayout = new QHBoxLayout(popup);
    popupLayout->setMargin(1);
    popupLayout->addWidget(slider);

    QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(popup);

    menu = new QMenu(this);
    menu->addAction(action);
}

void configMixerNewPushButton::init(int value)
{
    slider->setValue(value);
//    setText(QString("(%1)").arg(QString::number(value)));
    setText(QString("%1").arg(QString::number(value)));
}

void configMixerNewPushButton::contextMenuEvent(QContextMenuEvent *)
{
    QPoint point(0, size().height());
    menu->exec(mapToGlobal(point));
}

void configMixerNewPushButton::paintEvent(QPaintEvent *)
{
    //qDebug()<<__FUNCTION__<<__LINE__<<"paintEvent";
    QPainter painter(this);
    painter.setBackground(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);
    //设置画笔颜色
    QPen pen(QColor(95, 95, 95), 1.3);
    painter.setPen(pen);
    //设置填充颜色(背景)
    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    QRect rect(2, 2, this->width() - 4, this->height() - 4);
    int startAngle = 0 * 16;
    int stopAngle = 180 * 16;
    QRect r = QRect(2, this->width() / 2 , this->width() - 4, 14);
    painter.drawPie(rect, startAngle, stopAngle);
//    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.drawRect(r);
    //扇形dB
    QBrush sectorBrush;
    sectorBrush.setColor(getStatusColor());
    sectorBrush.setStyle(Qt::SolidPattern);
    QPen pen_pie(Qt::transparent, 0.5);
    painter.setBrush(sectorBrush);
    painter.setPen(pen_pie);
    int startsangle = -180 * 16;
    int stopangle = getAngleValue(_value) * 16;
    QRect rect_pie(3, 3, this->width() - 6, this->height() - 6);
    painter.drawPie(rect_pie, startsangle, stopangle);

    //选中重绘边框
    painter.setBrush(brush);
    QPainterPath path;
    QPolygonF polygon;
    polygon<<QPoint(33, 18)<<QPoint(33, 31)<<QPoint(3, 31)<<QPoint(2, 18);
    path.addPolygon(polygon);

//    _isSelect = this->isSliderDown();
//    switch (_isSelect) {
//    case IsSelect:
//        painter.setPen(Qt::red);
//        painter.drawArc(rect, startAngle, stopAngle);
//        painter.drawPath(path);
//        painter.setPen(Qt::black);
//        painter.drawLine(QPoint(0, 17), QPoint(34, 17));
//        break;
//    case NotSelect:
//        //pen.setColor(Qt::black);
        if(_mouseStatus == NewBtn_InsideItem)
        {
            QPen pen(Qt::green, 2);
            painter.setPen(pen);
            painter.drawArc(rect, startAngle, stopAngle);
//            painter.drawPath(path);
//            pen.setColor(Qt::black);
//            painter.drawLine(QPoint(1, 18), QPoint(33, 18));
            painter.drawRect(r);
        }
//        break;
//    default:
//        //pen.setColor(Qt::gray);
//        break;
//    }

    //textDB
    painter.setPen(Qt::black);
    painter.drawText(r, Qt::AlignCenter, this->text());
}

//void configMixerNewPushButton::wheelEvent(QWheelEvent *ev)
//{
//    qDebug() << __FUNCTION__ << __LINE__ ;
//}

void configMixerNewPushButton::buttonDispChanged(int value)
{
//    setText(QString("(%1)").arg(QString::number(value)));
    setText(QString("%1").arg(QString::number(value)));
    _value = value;
    this->update();
    emit gainValueChanged(value);
}

int configMixerNewPushButton::getAngleValue(int value)
{
    double num = slider->maximum() - slider->minimum();
    value -= slider->minimum();
    return (-180 / num * value);
}

QColor configMixerNewPushButton::getStatusColor()
{
    if(this->isChecked())
    {
        return QColor(0, 255, 0);
    }else {
        return QColor(128, 0, 0);
    }
}

void configMixerNewPushButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    _mouseStatus = NewBtn_InsideItem;
    this->update();
}

void configMixerNewPushButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    _mouseStatus = NewBtn_externalItem;
    this->update();
}
