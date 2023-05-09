#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <QWidget>

#define PEN_WIDTH 1
#define FRAME_WIDTH_MATRIX 35
#define FRAME_HEIGHT_MATRIX 10

class SignalGenerator : public QWidget
{
    Q_OBJECT
public:
    explicit SignalGenerator(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

signals:

public slots:
    void dbChanged(qreal dB);
    void freqChanged(qreal freq);

private:
    qreal calGain(qreal cur_dB);
    void calSineWave(qreal cur_dB, qreal freq);

private:
    qreal _currentdB;
    qreal _currentFreq;
    qreal *_psinBuf;
    int _sinBufSize;

    int _lineSpacing;/* 线间距 */
    QSize _GraphViewSize;/* 正弦波图像框的大小 */
    QLineF _line[8];/* 图像框中的8条分割线 */
    QRectF _labelRects[2][10];/* 数字标签矩形 */
};

#endif // SIGNALGENERATOR_H
