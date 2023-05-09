#include "stable.h"
#include "volumewidget.h"
#include "config.h"

VolumeWidget::VolumeWidget(QWidget *parent) : QWidget(parent)
{
    startTimer(50);
    _maxValue = 200;
    _minValue = 0;
    _lCurValue = 0;
    _rCurValue = 0;
    _ppm = _minValue;

    _pos1 = 0;
    _pos2 = 0.9083;
    _pos3 = 1;
    _color1 = QColor(14, 224, 255);
    _color2 = Qt::yellow;
    _color3 = Qt::red;
    _bSingle = false;
    _bScaleLabel  = false;
    _isApplidGain = false;
}

VolumeWidget::~VolumeWidget()
{

}

void VolumeWidget::setMaxValue(double value)
{
    _maxValue = value;
}

double VolumeWidget::getMaxValue()const
{
    return _maxValue;
}

void VolumeWidget::setMinValue(double value)
{
    _minValue = value;
    if(!_isApplidGain){
        _ppm = _lCurValue = _rCurValue = value;
    }
    else{
        _lCurValue = _rCurValue = 0; //压限电平表
    }
}

double VolumeWidget::getMinValue()const
{
    return _minValue;
}

void VolumeWidget::setLCurValue(double value)
{
    if(value > _maxValue) _lCurValue = _maxValue;
    else if(value < _minValue) _lCurValue = _minValue;
    else _lCurValue = value;
    update();
}

double VolumeWidget::getLCurValue() const
{
    return _lCurValue;
}

void VolumeWidget::setRCurValue(double value)
{
    if(value > _maxValue) _rCurValue = _maxValue;
    else if(value < _minValue) _rCurValue = _minValue;
    else _rCurValue = value;
    update();
}

double VolumeWidget::getRCurValue() const
{
    return _rCurValue;
}

double VolumeWidget::getValidValue(double val)
{
    if(val > _maxValue) return _maxValue;
    else if(val < _minValue) return _minValue;
    else return val;
}

void VolumeWidget::setPPM(double l, double d)
{
    l = getValidValue(l);
    d = getValidValue(d);
    _lCurValue = l;

    if( d < l)
        _ppm = l;
    else
        _ppm = d;
    update();
}

void VolumeWidget::setIsApplidGain(bool isApplid)
{
    if(_isApplidGain != isApplid)
    {
        _isApplidGain = isApplid;
        if(isApplid){
            _lCurValue = _rCurValue = 0;
        }
    }
}

void VolumeWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    paintNew(painter);
}


void VolumeWidget::paintNew(QPainter& painter)
{
    QSize sz = this->size();
    int font_width  = 20 + sz.width()%2;
    int font_height = 8;
    int vuWidth = sz.width() - font_width;
    double heigh = (double)sz.height();

    if (_bScaleLabel)
    {
        QTextOption option(Qt::AlignRight | Qt::AlignVCenter);
        painter.setFont(SkinStyle::getFont(7));
        painter.setPen(Config::curSkinStyle()->VolumnLableColor);

        int minVal10 = _minValue - _minValue%10;
        int maxVal10 = _maxValue - _maxValue%10;
        int vuStep = 10;
        if(_maxValue - _minValue > 60) vuStep = 20;
        for(int vu = minVal10; vu<= maxVal10; vu += vuStep)
        {
            int txtY = (_maxValue -vu ) *heigh/ (_maxValue - _minValue);
            QRect lblRc;
            if(txtY == 0)  lblRc = QRect(0, 0, font_width-2, font_height);
            else if(txtY == heigh) lblRc = QRect(0, heigh -font_height, font_width-2, font_height);
            else lblRc = QRect(0, txtY-font_height/2, font_width-2, font_height);

             painter.drawText(lblRc, QString::number(vu) , option);
        }
    }else {
        font_width = 0;
        vuWidth = sz.width();
    }

    painter.fillRect(font_width, 0, sz.width(), heigh, Qt::darkGray);


    QPen pen(Qt::darkGray, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
    painter.setPen(pen);

    QBrush brush;
    if(!_isApplidGain){
        QLinearGradient lg;
        lg = QLinearGradient(font_width, sz.height(), 0, 0);
        lg.setColorAt(_pos1, _color1);
        lg.setColorAt(_pos2, _color2);
        lg.setColorAt(_pos3, _color3);
        brush = QBrush(lg);
    }

    int rectH = 4;
    int widthL = vuWidth/2;
    if(_isApplidGain) //压限电平表
    {
        int vuZero = (_maxValue - 0) *heigh/ (_maxValue - _minValue);
        if(_lCurValue >= 0){
            int vuYL = (_lCurValue - 0) *heigh/ (_maxValue - _minValue);
            if(vuYL < 1) vuYL = 1;
            painter.fillRect(QRectF(font_width+1, vuZero - vuYL, vuWidth-2, vuYL), QBrush(QColor(0,255,0)));
        }
        else{
            int vuYL = (0 - _lCurValue) *heigh/ (_maxValue - _minValue);
            if(vuYL > heigh -1) vuYL = heigh -1;
            painter.fillRect(QRectF(font_width+1, vuZero + 1, vuWidth-2, vuYL), QBrush(QColor(255,0,0)));
        }
    }
    else    //普通电平表
    {
        int vuYL = (_maxValue -_lCurValue ) *heigh/ (_maxValue - _minValue);
        int vuYR = (_maxValue -_rCurValue ) *heigh/ (_maxValue - _minValue);
        if(this->isSingleTrack()) {
            painter.fillRect(QRectF(font_width+1, vuYL, vuWidth-2, heigh -1), brush);
            for(int y = heigh; y > vuYL; y -=rectH){
                painter.drawLine(QLineF(font_width, y-0.5, sz.width(), y-0.5));
            }
        }
        else{
            painter.fillRect(QRectF(font_width+1, vuYL , widthL-2, heigh -1), brush);
            painter.fillRect(QRectF(font_width+1 + widthL + 1, vuYR, widthL-2, heigh -1), brush);
            for(int y = heigh; y > vuYL | y > vuYR; y -=rectH){
                painter.drawLine(QLineF(font_width, y-0.5, sz.width(), y-0.5));
            }
        }

        /* ppm */
        if(_ppm - _minValue > 1){
            int ppmY = (_maxValue -_ppm ) *heigh/ (_maxValue - _minValue);
            painter.setPen(QPen(QColor(255, 50, 50), 2));
            painter.drawLine(font_width+1, ppmY - 1, sz.width()-1, ppmY - 1);
        }
    }
}


void VolumeWidget::paintOld(QPainter& painter)
{
    QSize sz = this->size();
    int font_width   = 0;
    int font_height = 8;

    if (_bScaleLabel)
    {
        QTextOption option(Qt::AlignCenter | Qt::AlignVCenter);
        painter.setFont(SkinStyle::getFont(7));
        painter.setPen(Config::curSkinStyle()->VolumnLableColor);
        font_width = 18;
        double step = sz.height() / (_maxValue - _minValue);
        int    label_count = (this->getMinValue() * -1) / 10 - 1;
        if (_maxValue > 0)
        {
            painter.drawText(QRect(0, 0, font_width, font_height), QString::number(_maxValue), option);
            painter.drawText(QRect(0, _maxValue*step, font_width, font_height), QString::number(0), option);
            painter.drawText(QRect(0, sz.height()-font_height-1, font_width, font_height), QString::number(_minValue), option);
        } else {
            painter.drawText(QRect(0, 0, font_width, font_height), "0", option);
            painter.drawText(QRect(0, sz.height()-font_height-1, font_width, font_height), QString::number(_minValue), option);
        }
        for (int i = 1; i < label_count; i=i+2)
        {
            painter.drawText(QRect(0, (_maxValue+(i+1)*10)*step-font_height/2, font_width, font_height), QString::number((i+1)*(-10)), option);
        }
    }

    double height = (double)sz.height()-1;
    double width = (double)sz.width() - font_width;
    QPen pen(Qt::transparent, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawRect(font_width, 0, sz.width(), sz.height());
    painter.fillRect(font_width, 0, sz.width(), sz.height(), Qt::darkGray);
    QLinearGradient lg;
    if(!this->isApplidGain())
    {
        lg = QLinearGradient(font_width, sz.height(), 0, 0);
    }else {
        lg = QLinearGradient(font_width, 0, 0, sz.height());
    }
    lg.setColorAt(_pos1, _color1);
    lg.setColorAt(_pos2, _color2);
    lg.setColorAt(_pos3, _color3);


    QBrush brs(lg);
    painter.setBrush(brs);
    QPen pen2(Qt::darkGray, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen2);

    if(this->isSingleTrack())
    {
        if(!this->isApplidGain())
        {
            for(int i = 0; i < ((_lCurValue - _minValue) / (_maxValue - _minValue))*40; ++i)
            {
                painter.drawRect(QRectF(font_width, height-(i+1)*height/40, width-1, height/40));
            }
        }else {
//            qDebug() << __FUNCTION__ << __LINE__ << _lCurValue;
            for(int i = 0; i < (abs(_lCurValue - _maxValue) / (_maxValue - _minValue))*40; ++i)
            {
                if(_lCurValue > _maxValue) break;
                painter.drawRect(QRectF(font_width, i*height/40, width-1, height/40));
            }
        }
    }
    else
    {
        for(int i = 0; i < ((_lCurValue - _minValue) / (_maxValue - _minValue))*40; ++i)
        {
            painter.drawRect(QRectF(font_width, height-(i+1)*height/40, width/9*4/*-1*/, height/40));
        }
        for(int j = 0; j < ((_rCurValue - _minValue) / (_maxValue - _minValue))*40; ++j)
        {
            painter.drawRect(QRectF(font_width+width/9*5, height-(j+1)*height/40, width/9*4-1, height/40));
        }
    }
    /* ppm */
    if(_ppm - _minValue > 1){
        /* ppm */
        painter.setPen(QPen(QColor(255, 50, 50),2));
        int ppmY = (_maxValue -_ppm ) *height/ (_maxValue - _minValue);
        painter.drawLine(font_width+1, ppmY - 1, sz.width()-1, ppmY - 1);
    }
}
