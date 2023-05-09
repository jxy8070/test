#include "crossover.h"
#include "stable.h"
#include "curvecalc.h"
#include "eqgraph.h"
#include "config.h"

CrossOver::CrossOver(QWidget *parent) : QWidget(parent)
{
    _type = CrossOverType_2Way;
    _pointCount = 2;
    _idx = 0;
    _point = (crossOverPoint *)malloc(_pointCount * sizeof(crossOverPoint));
}

CrossOver::~CrossOver()
{
    free(_point);
}

void CrossOver::setCurrentIdx(int idx)
{
    _idx = idx;
    update();
}

void CrossOver::initType(CrossOverTypeEnum type, crossOverPoint *pnt)
{
    _type = type;

    switch (_type)
    {
    case CrossOverType_2Way:
        free(_point);
        _pointCount = 2;
        _point = (crossOverPoint *)malloc(_pointCount * sizeof(crossOverPoint));
        break;
    case CrossOverType_3Way:
        free(_point);
        _pointCount = 4;
        _point = (crossOverPoint *)malloc(_pointCount * sizeof(crossOverPoint));
        break;
    case CrossOverType_4Way:
        free(_point);
        _pointCount = 6;
        _point = (crossOverPoint *)malloc(_pointCount * sizeof(crossOverPoint));
        break;
    }

    memcpy(_point, pnt, _pointCount * sizeof(crossOverPoint));
    _isMove = false;
    _moveNum = 0;
    update();
}

void CrossOver::fsChanged(int idx, float fs)
{
    _point[idx].fs = fs;

    update();
}

void CrossOver::filterTypeChanged(int idx, int type)
{
    _point[idx].filterType = type;

    update();
}

void CrossOver::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setBrush(QBrush(QColor(200,200,200), Qt::SolidPattern));
    painter.setPen(QPen(QColor(0,0,0), PEN_WIDTH));
    painter.drawRect(0, 0,
                     width() - PEN_WIDTH,
                     height() - PEN_WIDTH);

    painter.setBrush(QBrush(QColor(0,0,0), Qt::SolidPattern));
    painter.setPen(QPen(QColor(128,255,255), PEN_WIDTH));
    painter.drawRect(FRAME_LEFT_MATRIX,
                     FRAME_TOP_MATRIX,
                     _graphViewSize.width() - PEN_WIDTH,
                     _graphViewSize.height() - PEN_WIDTH);

    painter.setPen(QPen(QColor(150,150,150), PEN_WIDTH));
    painter.drawLines(_linesH, 10);
    painter.drawLines(_linesV, 32);

    painter.setPen(QPen(QColor(0,0,0), PEN_WIDTH));
    painter.setFont(SkinStyle::getFont(8));
    for (int i = 0; i < _fsStr.count(); ++i)
    {
        painter.drawText(_labelRectsV[i], Qt::AlignHCenter, _fsStr[i]);
    }
    for (int i = 0; i < 12; ++i)
    {
        painter.drawText(_labelRectsH[i], Qt::AlignRight, _gainStr[i]);
    }


    for (int i = 0; i < _pointCount; ++i)
    {
        if (i % 2 == 0)
        {
            if (_point[i].filterType > 3)
            {
                CurveCalc::bwLpCalc(_point[i].fs, _point[i].filterType - 3, X_LEN_1, m_xFs_tab, _point[i].result);
            }
            else
            {
                CurveCalc::lrCalc(_point[i].fs, _point[i].filterType + 1, X_LEN_1, m_xFs_tab, _point[i].result, false);
            }
        }
        else
        {
            if (_point[i].filterType > 3)
            {
                CurveCalc::bwHpCalc(_point[i].fs, _point[i].filterType - 3, X_LEN_1, m_xFs_tab, _point[i].result);
            }
            else
            {
                CurveCalc::lrCalc(_point[i].fs, _point[i].filterType + 1, X_LEN_1, m_xFs_tab, _point[i].result, true);
            }
        }
    }

    int t_off = 0;
    for (int t = 0; t < _pointCount; t += t_off)
    {
        int endj = 0;
        if (t == 0 || t == _pointCount - 1)
        {
            endj = t + 1;
            t_off = 1;
        }
        else
        {
            endj = t + 2;
            t_off = 2;
        }
        QPainterPath curves;
        float sum_tab[X_LEN_1];
        memset(sum_tab, 0, X_LEN_1*sizeof(float));

        int y0; //第一个点的y值
        for(int i = 0; i < X_LEN_1; i++)
        {
            for(int j = t; j < endj; j++)
            {
                sum_tab[i] += _point[j].result[i];
            }
            if(sum_tab[i] > 3)
            {
                sum_tab[i] = 3;
            }
            else if(sum_tab[i] < -30)
            {
                sum_tab[i] = -30;
            }

            int yi = 14 + FRAME_TOP_MATRIX - _graphViewSize.height() / 33.0 * sum_tab[i];

            if(i == 0)
            {
                y0 = yi;
                //修正线宽导致曲线画出框外的问题
                curves.moveTo(m_x_tab[0] + 2, yi);
            }
            else if(i == X_LEN_1-1)
            {
                curves.lineTo(m_x_tab[X_LEN_1-1] - 2, yi);
            }
            else
            {
                curves.lineTo(m_x_tab[i], yi);
            }
        }

        if (t == _idx)
        {
            painter.setPen(QPen(QColor(255, 0, 0), 2));
        }
        else
        {
            painter.setPen(QPen(QColor(0, 255, 0), 2));
        }
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(curves);
    }

    painter.setPen(QPen(QColor(255,255,128), PEN_WIDTH));
    painter.setBrush(QBrush(QColor(255,255,128), Qt::SolidPattern));
    for (int i = 0; i < _pointCount; ++i)
    {
        _pointRectF[i].setRect(qRound(log10(_point[i].fs/20.0) * X_LEN_1 /3) + 40, 44, 8, 8);
        painter.drawEllipse(_pointRectF[i]);
    }
}

void CrossOver::resizeEvent(QResizeEvent *)
{
    int wdth = width() - FRAME_LEFT_MATRIX - FRAME_RIGHT_MATRIX;
    int hght = height() - FRAME_TOP_MATRIX - FRAME_BOTTOM_MATRIX;
    _graphViewSize.setWidth(wdth);
    _graphViewSize.setHeight(hght);

    for (int i = 0; i < 11; ++i)
    {
        _linesH[i].setP1(QPointF(FRAME_LEFT_MATRIX + 1, FRAME_TOP_MATRIX + (i + 1) * 14));
        _linesH[i].setP2(QPointF(width() - FRAME_RIGHT_MATRIX - 2 * PEN_WIDTH, FRAME_TOP_MATRIX + (i + 1) * 14));
    }
    for (int i = 0; i < 32; ++i)
    {
        _linesV[i].setP1(QPointF(FRAME_LEFT_MATRIX + (i + 1) * 16, FRAME_TOP_MATRIX + 1));
        _linesV[i].setP2(QPointF(FRAME_LEFT_MATRIX + (i + 1) * 16, height() - FRAME_TOP_MATRIX - 2 * PEN_WIDTH));
    }

    float stepLG = X_LEN_1 - 1;
    for (int i = 0; i < X_LEN_1; ++i)
    {
        m_x_tab[i] = 44 + i;
        m_xFs_tab[i] = 20 * (pow(10.0, 3.0 * i / stepLG));

        if (i % 16 == 0)
        {
            if (m_xFs_tab[i] < 1000)
            {
                _fsStr << QString::number(m_xFs_tab[i], 'f', 1);
            }
            else
            {
                _fsStr << QString::number(m_xFs_tab[i]/1000.0, 'f', 1) + "k";
            }
        }
    }

    for (int i = 0; i < 32;)
    {
        _labelRectsV[i].setRect(m_x_tab[16*i] - 10, 3, 25, 14);
        ++i;
        _labelRectsV[i].setRect(m_x_tab[16*i] - 10, FRAME_TOP_MATRIX + _graphViewSize.height() + 3, 25, 14);
        ++i;
    }

    for (int i = 0; i < 12; ++i)
    {
        _labelRectsH[i].setRect(1, 13 + 14 * i, 20, 14);
    }
    _gainStr << "3" << "0" << "-3" << "-6" << "-9" << "-12" << "-15" << "-18" << "-21" << "-24" << "-27" << "-30";
}

void CrossOver::mousePressEvent(QMouseEvent *event)
{
    static int lastPointNum = -1;
    for (int i = 0; i < _pointCount; ++i)
    {
//        qDebug() << __FUNCTION__ << __LINE__  << i << _pointRectF[i].contains(event->pos());
        if (_pointRectF[i].contains(event->pos()))
        {
            _moveNum = i;
            _isMove = true;
            if(lastPointNum != i)
            {
                lastPointNum = i;
                emit pointChange(i);
            }
            break;
        }
    }
    QWidget::mousePressEvent(event);
}

void CrossOver::mouseReleaseEvent(QMouseEvent *event)
{
    _isMove = false;

    QWidget::mouseReleaseEvent(event);
}

void CrossOver::mouseMoveEvent(QMouseEvent *event)
{
    if (_isMove)
    {
        float curFs = powf(10.0, (event->pos().x() - 40.0) * 0.00603) * 20.0;
//        qDebug() << __FUNCTION__ << __LINE__ << _moveNum << curFs;
        emit pointMove(_moveNum, curFs);
    }
     QWidget::mouseMoveEvent(event);
}
