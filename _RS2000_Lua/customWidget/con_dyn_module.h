#ifndef CON_DYN_MODULE_H
#define CON_DYN_MODULE_H

#include <QWidget>

#define PEN_WIDTH 1
#define FRAME_LEFT_MATRIX 24
#define FRAME_RIGHT_MATRIX 10
#define FRAME_TOP_MATRIX 10
#define FRAME_BOTTOM_MATRIX 18


class Con_Dyn_Module : public QWidget
{
    Q_OBJECT
public:
    explicit Con_Dyn_Module(QWidget *parent = 0);
    void setType(CfgType type);
    void outTargetLevelChanged(qreal value);
    void thresholdChanged(qreal value);
    void ratioChanged(qreal value);
    void gainChanged(qreal value);
    void setCurPointDisp(bool enable);
    void curPointDisp(float iVU, float oVU);
    void setBypass(bool isBypass);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

signals:

public slots:

    
private:
    void calNewPos(void);
//    void calP0Pos();
//    void calThresholdPos(qreal threshold);
//    void calRatioPos(qreal ratio);
//    void calOutTargetLevelPos(qreal outTargetLevel);

    void calCurpointPos();

    void drawLeveler(QPainter *painter);
    void drawCompressor(QPainter *painter);
    void drawLimiter(QPainter *painter);
    void drawNoiseGate(QPainter *painter);
    void drawExpander(QPainter *painter);
    void drawClipper(QPainter *painter);

    void calLevelerP0Pos();
    void calLevelerP1Pos(qreal threshold);
    void calLevelerP2Pos(qreal outTargetLevel);
    void calLevelerP3Pos(qreal ratio);

    void calCompressorP0Pos();
    void calCompressorP1Pos(qreal threshold);
    void calCompressorP2Pos(qreal ratio);

    void calLimiterP0Pos();
    void calLimiterP1Pos(qreal threshold);
    void calLimiterP2Pos();
    
    void calNoiseGateP0Pos();
    void calNoiseGateP1Pos(qreal gain);
    void calNoiseGateP2Pos(qreal threshold);
    void calNoiseGateP3Pos();

    void calExpanderP0Pos(qreal ratio);
    void calExpanderP1Pos(qreal threshold);
    void calExpanderP2Pos();

    void calClipperP0Pos();
    void calClipperP1Pos(qreal threshold);
    void calClipperP2Pos();
private:
    QSize _graphViewSize;
    qreal _lineSpacing;
    QLineF _lines[10];
    QRectF _labelRects[13];

    qreal _ratio;
    qreal _threshold;
    qreal _outTargetLevel;
    qreal _gain;

    qreal _ratioScale;
    qreal _thresholdScale;
    qreal _outTargetLevelScale;
    qreal _gainScale;
    
    qreal _k1;
    bool _updateK1;
    QPoint _p0Point;
    QPoint _p1Point;
    QPoint _p2Point;
    QPoint _p3Point;

    bool _bCurPointDisp;
    QPoint _curPoint;
    bool _isBypass = false;  //默认不显示

    bool _p1PointIsVisible;
    CfgType _type;
};

#endif // CON_DYN_MODULE_H
