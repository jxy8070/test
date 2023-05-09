#include "configmixerrotaryknob.h"
#include <QDebug>
#include "configSetting/cfgsettingmixer.h"

configMixerRotaryKnob::configMixerRotaryKnob(int type, QWidget* parent) :
    QDial(parent)
{
    this->setInvertedControls(false);
    _isSelect = NotSelect;
    _mouseStatus = NotSelect;
    _nodeSwitch = true;
    _prevValue = -101;

    _type = type;
    switch (_type) {
    case InputType:
        setMaximum(12);
        setMinimum(-100);
        break;
    case OutputType:
        setMaximum(12);
        setMinimum(-100);
        break;
    case DefaultType:
        setMaximum(0);
        setMinimum(-100);
    default:
        break;
    }
    //setGainValue(-50);

    _cfgSettingMixer = qobject_cast<CfgSettingMixer *>(parent);
}

void configMixerRotaryKnob::paintEvent(QPaintEvent *e)
{
    //qDebug()<<__FUNCTION__<<__LINE__<<"paintEvent";
    QPainter painter(this);
    painter.setBackground(Qt::white);
    //设置画笔颜色
    painter.setPen(Qt::black);
    //设置填充颜色(背景)
    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    //painter.setPen(pen);
    QRect rect(0, 0, 35, 35);
    int startAngle = 0 * 16;
    int stopAngle = 180 * 16;
    QRect r = QRect(0, 17, 34, 13);
    painter.drawPie(rect, startAngle, stopAngle);
    painter.drawRect(r);
    //扇形dB
    QBrush sectorBrush;
    sectorBrush.setColor(QColor(95 ,158, 160));
    sectorBrush.setStyle(Qt::SolidPattern);
    painter.setBrush(sectorBrush);
    int startsangle = -180 * 16;
    int stopangle = _angle * 16;
    painter.drawPie(rect, startsangle, stopangle);

    //选中重绘边框
    painter.setBrush(brush);
    QPainterPath path;
    QPolygonF polygon;
    polygon<<QPoint(34, 17)<<QPoint(34, 30)<<QPoint(0, 30)<<QPoint(0, 17);
    path.addPolygon(polygon);

    _isSelect = this->isSliderDown();
    switch (_isSelect) {
    case IsSelect:
        painter.setPen(Qt::red);
        painter.drawArc(rect, startAngle, stopAngle);
        painter.drawPath(path);
        painter.setPen(Qt::black);
        painter.drawLine(QPoint(0, 17), QPoint(34, 17));
        break;
    case NotSelect:
        //pen.setColor(Qt::black);
        if(_mouseStatus == InsideItem)
        {
            painter.setPen(Qt::green);
            painter.drawArc(rect, startAngle, stopAngle);
            painter.drawPath(path);
            painter.setPen(Qt::black);
            painter.drawLine(QPoint(0, 17), QPoint(34, 17));
        }
        break;
    default:
        //pen.setColor(Qt::gray);
        break;
    }

    //textDB
    painter.setPen(Qt::black);
    painter.drawText(r, Qt::AlignCenter, _textDB);

}

//void configMixerRotaryKnob::mouseMoveEvent(QMouseEvent *e)
//{
//    qDebug()<<__FUNCTION__<<__LINE__<<"mouseMoveEvent";

//}

void configMixerRotaryKnob::mousePressEvent(QMouseEvent *e)
{
//    qDebug()<<__FUNCTION__<<__LINE__<<"mousePressEvent";
//    if(rect().contains(e->pos()))
//    {
//        this->setSliderDown(true);
////        _isSelect = IsSelect;
//    }else{
//        this->setSliderDown(false);
////        _isSelect = NotSelect;
//    }
//    for(int i = 0; i < _cfgSettingMixer->getRotaryKnobList().count(); i++)
//    {
//        if(_cfgSettingMixer->getRotaryKnobList()[i] != this)
//        {
//            _cfgSettingMixer->getRotaryKnobList()[i]->setSliderDown(false);
//        }
//    }
//    for(int i = 0; i < _cfgSettingMixer->getInKnobList().count(); i++)
//    {
//        if(_cfgSettingMixer->getInKnobList()[i] != this)
//        {
//            _cfgSettingMixer->getInKnobList()[i]->setSliderDown(false);;
//        }
//    }
//    for(int i = 0; i < _cfgSettingMixer->getOutKnobList().count(); i++)
//    {
//        if(_cfgSettingMixer->getOutKnobList()[i] != this)
//        {
//            _cfgSettingMixer->getOutKnobList()[i]->setSliderDown(false);;
//        }
//    }

//    //this->repaint();
//    this->update();
}

void configMixerRotaryKnob::mouseReleaseEvent(QMouseEvent *e)
{

}

void configMixerRotaryKnob::enterEvent(QEvent *event)
{
    _mouseStatus = InsideItem;
    //this->repaint();
    this->update();
}

void configMixerRotaryKnob::leaveEvent(QEvent *event)
{
    _mouseStatus = NotSelect;
    this->update();
}

void configMixerRotaryKnob::setGainValue(int value)
{
    setValue(value);

    if(_prevValue == -101)
    {
        _prevValue = value;
    }else {
        if(value - _prevValue > 50 || _prevValue - value > 50)
        {
            return;
        }
        _prevValue = value;
    }
    _textDB = QString("%1dB").arg(value);
    double num = maximum() - minimum();
    value -= minimum();
    _angle = -180 / num * value;

    //this->update();
}

