#include "stable.h"
#include "signalgenerator.h"
#include <math.h>
#include "skinstyle.h"

const qreal FREQ_COEFF = 2 * 3.1415926 / 48000;
const QString LABEL_STRING[10] = {"10", "1", "0.1", "0.01", "0", "-0.01", "-0.1", "-1", "-10", "-100"};
SignalGenerator::SignalGenerator(QWidget *parent) : QWidget(parent)
{
    _currentdB = 0;
    _currentFreq = 1000;
    _lineSpacing = 1;

    _sinBufSize = 240;
    _psinBuf = (qreal *)malloc(_sinBufSize*sizeof(qreal));
    calSineWave(_currentdB, _currentFreq);
}

qreal SignalGenerator::calGain(qreal cur_dB)
{
    qreal cur_Gain = 1;

    /* _lineSpacing为标准像素格，即0dB时峰值占_lineSpacing个像素 */
    if (cur_dB > 0)
    {
        cur_Gain = 3 * _lineSpacing + cur_dB;
    }
    else if (cur_dB == 0)
    {
        cur_Gain = 3 * _lineSpacing;
    }
    else if (cur_dB < 0)
    {
        if (cur_dB >= -40)
        {
            cur_Gain = 3 * _lineSpacing + cur_dB * _lineSpacing / 20.0;
        }
        else if (cur_dB >= -60)
        {
            cur_Gain = _lineSpacing + (cur_dB + 40) * _lineSpacing / 40.0;
        }
        else if (cur_dB >= -100)
        {
            cur_Gain = _lineSpacing / 2 + (cur_dB + 60) * _lineSpacing / 80.0;
        }
    }

    return cur_Gain;
}

void SignalGenerator::calSineWave(qreal cur_dB, qreal freq)
{
    qreal gain;

    gain = calGain(cur_dB);
//    qDebug()<<"Gain ="<<gain;

    for (int i = 0; i < _sinBufSize; ++i)
    {
        _psinBuf[i] = gain * sin(FREQ_COEFF*freq*i);
    }
}

void SignalGenerator::dbChanged(qreal dB)
{
    _currentdB = dB;

    calSineWave(_currentdB, _currentFreq);

    update();
}

void SignalGenerator::freqChanged(qreal freq)
{
    _currentFreq = freq;

    calSineWave(_currentdB, _currentFreq);

    update();
}

void SignalGenerator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setBrush(QBrush(QColor(200,200,200), Qt::SolidPattern));
    painter.setPen(QPen(QColor(0,0,0), PEN_WIDTH));
    painter.drawRect(0, 0,
                     width() - PEN_WIDTH,
                     height() - PEN_WIDTH);

    painter.setBrush(QBrush(QColor(0,0,0), Qt::SolidPattern));
    painter.setPen(QPen(QColor(128,255,255), PEN_WIDTH));
    painter.drawRect(FRAME_WIDTH_MATRIX,
                     FRAME_HEIGHT_MATRIX,
                     _GraphViewSize.width() - PEN_WIDTH,
                     _GraphViewSize.height() - PEN_WIDTH);

    painter.setPen(QPen(QColor(150,150,150), PEN_WIDTH));
    painter.drawLines(_line, 8);
//    painter.drawLine(36, 24, 275, 24);
//    painter.drawLine(36, 38, 275, 38);
//    painter.drawLine(36, 52, 275, 52);
//    painter.drawLine(36, 66, 275, 66);
//    painter.drawLine(36, 80, 275, 80);
//    painter.drawLine(36, 94, 275, 94);
//    painter.drawLine(36, 108, 275, 108);
//    painter.drawLine(36, 122, 275, 122);

    painter.setPen(QPen(QColor(0,0,0), PEN_WIDTH));
    painter.setFont(SkinStyle::getFont(7));
    for (int i = 0; i < 10; ++i)
    {
        painter.drawText(_labelRects[0][i], Qt::AlignRight, LABEL_STRING[i]);
        painter.drawText(_labelRects[1][i], Qt::AlignLeft, LABEL_STRING[i]);
    }
//    painter.drawText(QRectF(6, 5, 26, 12), Qt::AlignRight, "10");
//    painter.drawText(QRectF(6, 19, 26, 12), Qt::AlignRight, "1");
//    painter.drawText(QRectF(6, 33, 26, 12), Qt::AlignRight, "0.1");
//    painter.drawText(QRectF(6, 47, 26, 12), Qt::AlignRight, "0.01");
//    painter.drawText(QRectF(6, 61, 26, 12), Qt::AlignRight, "0");
//    painter.drawText(QRectF(6, 75, 26, 12), Qt::AlignRight, "-0.01");
//    painter.drawText(QRectF(6, 89, 26, 12), Qt::AlignRight, "-0.1");
//    painter.drawText(QRectF(6, 103, 26, 12), Qt::AlignRight, "-1");
//    painter.drawText(QRectF(6, 117, 26, 12), Qt::AlignRight, "-10");
//    painter.drawText(QRectF(6, 131, 26, 12), Qt::AlignRight, "-100");

//    painter.drawText(QRectF(279, 5, 26, 12), Qt::AlignLeft, "10");
//    painter.drawText(QRectF(279, 19, 26, 12), Qt::AlignLeft, "1");
//    painter.drawText(QRectF(279, 33, 26, 12), Qt::AlignLeft, "0.1");
//    painter.drawText(QRectF(279, 47, 26, 12), Qt::AlignLeft, "0.01");
//    painter.drawText(QRectF(279, 61, 26, 12), Qt::AlignLeft, "0");
//    painter.drawText(QRectF(279, 75, 26, 12), Qt::AlignLeft, "-0.01");
//    painter.drawText(QRectF(279, 89, 26, 12), Qt::AlignLeft, "-0.1");
//    painter.drawText(QRectF(279, 103, 26, 12), Qt::AlignLeft, "-1");
//    painter.drawText(QRectF(279, 117, 26, 12), Qt::AlignLeft, "-10");
//    painter.drawText(QRectF(279, 131, 26, 12), Qt::AlignLeft, "-100");

    QPainterPath sinWavePath;
    sinWavePath.moveTo(_line[3].x1(), _line[3].y1());
    for (int i = 1; i < _sinBufSize; ++i)
    {
        sinWavePath.lineTo(_line[3].x1()+i, _line[3].y1()+_psinBuf[i]);
    }

    painter.setPen(QPen(QColor(0,255,0), PEN_WIDTH));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(sinWavePath);
//    qDebug()<<"3"<<_gain<<_freq;
}

void SignalGenerator::resizeEvent(QResizeEvent *)
{
    _GraphViewSize.setWidth(width() - 2 * FRAME_WIDTH_MATRIX);
    _GraphViewSize.setHeight(height() - 2 * FRAME_HEIGHT_MATRIX);

    _lineSpacing = _GraphViewSize.height() / 9;

    for (int i = 0; i < 8; ++i)
    {
        _line[i].setP1(QPointF(FRAME_WIDTH_MATRIX + 1, FRAME_HEIGHT_MATRIX + (i + 1) * _lineSpacing));
        _line[i].setP2(QPointF(width() - FRAME_WIDTH_MATRIX - 2 * PEN_WIDTH, FRAME_HEIGHT_MATRIX + (i + 1) * _lineSpacing));
    }

    for (int i = 0; i < 9; ++i)
    {
        _labelRects[0][i].setRect(6, 5 + i * _lineSpacing, 26, 12);
        _labelRects[1][i].setRect(_line[0].p2().x() + 4, 5 + i * _lineSpacing, 26, 12);
    }
    _labelRects[0][9].setRect(6, _GraphViewSize.height() + FRAME_HEIGHT_MATRIX - 1 - 5, 26, 12);
    _labelRects[1][9].setRect(_line[0].p2().x() + 4, _GraphViewSize.height() + FRAME_HEIGHT_MATRIX - 1 - 5, 26, 12);

    _sinBufSize = _GraphViewSize.width() - 2 * PEN_WIDTH;
    free(_psinBuf);
    _psinBuf = (qreal *)malloc(_sinBufSize*sizeof(qreal));
    calSineWave(_currentdB, _currentFreq);
}
