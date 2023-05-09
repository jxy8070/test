#include "con_dyn_module.h"
#include "stable.h"
#include "config.h"

const QString LABEL_STRING[13] = {"12", "0", "-12", "-24", "-36", "-48", "-60", "-48", "-36", "-24", "-12", "0", "12"};
Con_Dyn_Module::Con_Dyn_Module(QWidget *parent) : QWidget(parent)
{
    _threshold = 0;
    _ratio = 4;
    _outTargetLevel = 0;
    _gain = 0;
    
    _thresholdScale = 1;
    _ratioScale = 1;
    _outTargetLevelScale = 1;
    _gainScale = 1;
    _updateK1 = false;

    _p1PointIsVisible = true;

    _bCurPointDisp = false;
    _type = CFG_TYPE_DYN_AUTO;
}

void Con_Dyn_Module::setType(CfgType type)
{
    _type = type;

    switch (_type) {
    case CFG_TYPE_DYN_AUTO:
        _threshold = 0;
        _ratio = 4;
        _outTargetLevel = 0;
        break;
    case CFG_TYPE_DYN_COMP:
        _threshold = 0;
        _ratio = 4;
        break;
    case CFG_TYPE_DYN_LIMIT:
        _threshold = 0;
        break;
    case CFG_TYPE_DYN_NOISE:
        _threshold = 0;
        _gain = 0;
        break;
    case CFG_TYPE_DYN_EXPAND:
        _threshold = 0;
        _ratio = 0.25;
        break;
    case CFG_TYPE_DYN_PEAK:
        _threshold = 0;
        break;
    default:
        break;
    }

    _updateK1 = true;
    calNewPos();
}

void Con_Dyn_Module::paintEvent(QPaintEvent *)
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
    painter.drawLines(_lines, 10);

    painter.setPen(QPen(QColor(0,0,0), PEN_WIDTH));
    painter.setFont(SkinStyle::getFont(6));
    for (int i = 0; i < 6; ++i)
    {
        painter.drawText(_labelRects[i], Qt::AlignRight, LABEL_STRING[i]);
        painter.drawText(_labelRects[7+i], Qt::AlignHCenter, LABEL_STRING[7+i]);
    }
    painter.drawText(_labelRects[6], Qt::AlignRight, LABEL_STRING[6]);

    if (_bCurPointDisp) {
        painter.setPen(QPen(QColor(128,128,192), PEN_WIDTH));
        painter.setBrush(QBrush(QColor(255, 0, 0), Qt::SolidPattern));
        painter.drawEllipse(_curPoint - QPoint(1, -1), 4, 4);
    }

    switch (_type) {
    case CFG_TYPE_DYN_AUTO:
        drawLeveler(&painter);
        break;
    case CFG_TYPE_DYN_COMP:
        drawCompressor(&painter);
        break;
    case CFG_TYPE_DYN_LIMIT:
        drawLimiter(&painter);
        break;
    case CFG_TYPE_DYN_NOISE:
        drawNoiseGate(&painter);
        break;
    case CFG_TYPE_DYN_EXPAND:
        drawExpander(&painter);
        break;
    case CFG_TYPE_DYN_PEAK:
        drawClipper(&painter);
        break;
    default:
        break;
    }

    if(_isBypass){
        painter.setPen(QPen(QColor(255,0,0)));
        painter.setFont(SkinStyle::getFont(24));
        painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, "Bypass");
    }
}

void Con_Dyn_Module::drawLeveler(QPainter *painter)
{
    painter->setPen(QPen(QColor(0,255,0), PEN_WIDTH));
    painter->drawLine(_p2Point, _p3Point);
    painter->drawLine(_p1Point, _p2Point);
    painter->drawLine(_p0Point, _p1Point);

    painter->setPen(QPen(QColor(128,128,192), PEN_WIDTH));
    painter->setBrush(QBrush(QColor(128,128,192), Qt::SolidPattern));
    painter->drawEllipse(_p2Point, 2, 2);

    painter->setPen(QPen(QColor(240,240,150), PEN_WIDTH));
    painter->setBrush(QBrush(QColor(128,128,192), Qt::SolidPattern));
    painter->drawEllipse(_p1Point, 2, 2);
    painter->drawEllipse(_p3Point, 2, 2);
}

void Con_Dyn_Module::drawCompressor(QPainter *painter)
{
    painter->setPen(QPen(QColor(0,255,0), PEN_WIDTH));
    painter->drawLine(_p1Point, _p2Point);
    painter->drawLine(_p0Point, _p1Point);

    painter->setPen(QPen(QColor(240,240,150), PEN_WIDTH));
    painter->setBrush(QBrush(QColor(128,128,192), Qt::SolidPattern));
    painter->drawEllipse(_p1Point, 2, 2);
    painter->drawEllipse(_p2Point, 2, 2);
}

void Con_Dyn_Module::drawLimiter(QPainter *painter)
{
    painter->setPen(QPen(QColor(0,255,0), PEN_WIDTH));
    painter->drawLine(_p1Point, _p2Point);
    painter->drawLine(_p0Point, _p1Point);

    painter->setPen(QPen(QColor(240,240,150), PEN_WIDTH));
    painter->setBrush(QBrush(QColor(128,128,192), Qt::SolidPattern));
    painter->drawEllipse(_p1Point, 2, 2);
    painter->drawEllipse(_p2Point, 2, 2);
}

void Con_Dyn_Module::drawNoiseGate(QPainter *painter)
{
    painter->setPen(QPen(QColor(0,255,0), PEN_WIDTH));
    painter->drawLine(_p2Point, _p3Point);
    painter->drawLine(_p1Point, _p2Point);
    painter->drawLine(_p0Point, _p1Point);

    painter->setPen(QPen(QColor(240,240,150), PEN_WIDTH));
    painter->setBrush(QBrush(QColor(128,128,192), Qt::SolidPattern));
    if (_p1PointIsVisible)
    {
        painter->drawEllipse(_p1Point, 2, 2);
    }
    painter->drawEllipse(_p2Point, 2, 2);
    painter->drawEllipse(_p3Point, 2, 2);
}

void Con_Dyn_Module::drawExpander(QPainter *painter)
{
    painter->setPen(QPen(QColor(0,255,0), PEN_WIDTH));
    painter->drawLine(_p1Point, _p2Point);
    painter->drawLine(_p0Point, _p1Point);

    painter->setPen(QPen(QColor(240,240,150), PEN_WIDTH));
    painter->setBrush(QBrush(QColor(128,128,192), Qt::SolidPattern));
    painter->drawEllipse(_p0Point, 2, 2);
    painter->drawEllipse(_p1Point, 2, 2);
}

void Con_Dyn_Module::drawClipper(QPainter *painter)
{
    painter->setPen(QPen(QColor(0,255,0), PEN_WIDTH));
    painter->drawLine(_p1Point, _p2Point);
    painter->drawLine(_p0Point, _p1Point);

    painter->setPen(QPen(QColor(240,240,150), PEN_WIDTH));
    painter->setBrush(QBrush(QColor(128,128,192), Qt::SolidPattern));
    painter->drawEllipse(_p1Point, 2, 2);
    painter->drawEllipse(_p2Point, 2, 2);
}

void Con_Dyn_Module::resizeEvent(QResizeEvent *)
{
    int wdth = width() - FRAME_LEFT_MATRIX - FRAME_RIGHT_MATRIX;
    int hght = height() - FRAME_TOP_MATRIX - FRAME_BOTTOM_MATRIX;
    int graphLen = wdth > hght ? hght : wdth;
    _graphViewSize.setWidth(graphLen);
    _graphViewSize.setHeight(graphLen);

    /* 计算线间距 */
    _lineSpacing = graphLen / 6.0;

    _thresholdScale = graphLen / 720.0;
    _ratioScale = graphLen / 990.0;
    _outTargetLevelScale = graphLen / 720.0;
    _gainScale = graphLen / 1000;
    
    for (int i = 0; i < 5; ++i)
    {
        /* 横着的5条坐标线 */
        _lines[i].setP1(QPointF(FRAME_LEFT_MATRIX + 1, FRAME_TOP_MATRIX + (i + 1) * _lineSpacing));
        _lines[i].setP2(QPointF(width() - FRAME_RIGHT_MATRIX - 2 * PEN_WIDTH, FRAME_TOP_MATRIX + (i + 1) * _lineSpacing));

        /* 竖着的5条坐标线 */
        _lines[5+i].setP1(QPointF(FRAME_LEFT_MATRIX + (i + 1) * _lineSpacing, FRAME_TOP_MATRIX + 1));
        _lines[5+i].setP2(QPointF(FRAME_LEFT_MATRIX + (i + 1) * _lineSpacing, height() - FRAME_BOTTOM_MATRIX - 2 * PEN_WIDTH));
    }

    for (int i = 0; i < 6; ++i)
    {
        _labelRects[i].setRect(1, FRAME_TOP_MATRIX + 1 + i * _lineSpacing - 5, 20, 10);
        _labelRects[7+i].setRect(FRAME_LEFT_MATRIX + 1 + (i + 1) * _lineSpacing - 10, height() - FRAME_BOTTOM_MATRIX - PEN_WIDTH + 2, 20, 10);
    }
    _labelRects[6].setRect(1, height() - FRAME_BOTTOM_MATRIX - PEN_WIDTH + 2, 20, 10);

    _updateK1 = true;
    calNewPos();
    calCurpointPos();
}

void Con_Dyn_Module::calNewPos(void)
{
    switch (_type) {
    case CFG_TYPE_DYN_AUTO:
        calLevelerP2Pos(_outTargetLevel);
        calLevelerP3Pos(_ratio);
        calLevelerP1Pos(_threshold);
        calLevelerP0Pos();
        break;
    case CFG_TYPE_DYN_COMP:
        calCompressorP0Pos();
        calCompressorP1Pos(_threshold);
        calCompressorP2Pos(_ratio);
        break;
    case CFG_TYPE_DYN_LIMIT:
        calLimiterP0Pos();
        calLimiterP1Pos(_threshold);
        calLimiterP2Pos();
        break;
    case CFG_TYPE_DYN_NOISE:
        calNoiseGateP2Pos(_threshold);
        calNoiseGateP1Pos(_gain);
        calNoiseGateP3Pos();
        calNoiseGateP0Pos();
        break;
    case CFG_TYPE_DYN_EXPAND:
        calExpanderP2Pos();
        calExpanderP1Pos(_threshold);
        calExpanderP0Pos(_ratio);
        break;
    case CFG_TYPE_DYN_PEAK:
        calClipperP0Pos();
        calClipperP1Pos(_threshold);
        calClipperP2Pos();
        break;
    default:
        break;
    }

    _updateK1 = false;
//    qDebug()<<_p0Point<<_p1Point<<_p2Point<<_p3Point;
    update();
}

void Con_Dyn_Module::calCurpointPos()
{
    _curPoint.setX(FRAME_LEFT_MATRIX + PEN_WIDTH);
    _curPoint.setY(height() - FRAME_BOTTOM_MATRIX - PEN_WIDTH);
}

void Con_Dyn_Module::calLevelerP0Pos()
{
    _p0Point.setX(FRAME_LEFT_MATRIX + PEN_WIDTH);
    _p0Point.setY(_p1Point.y() + (_p1Point.x() - _p0Point.x()));
}

void Con_Dyn_Module::calLevelerP1Pos(qreal threshold)
{
    _p1Point.setX(qRound(FRAME_LEFT_MATRIX + (threshold * 10 + 600) * _thresholdScale) - PEN_WIDTH);
    _p1Point.setY(qRound(_p2Point.y() - _k1 * (_p2Point.x() - _p1Point.x())));
}

void Con_Dyn_Module::calLevelerP2Pos(qreal outTargetLevel)
{
    int curPos = (outTargetLevel * 10 + 600) * _outTargetLevelScale;

    _p2Point.setX(FRAME_LEFT_MATRIX  - PEN_WIDTH + curPos);
    _p2Point.setY(FRAME_TOP_MATRIX + PEN_WIDTH + _graphViewSize.height() - curPos);
}

void Con_Dyn_Module::calLevelerP3Pos(qreal ratio)
{
    _p3Point.setX(width() - FRAME_RIGHT_MATRIX - PEN_WIDTH);
    _p3Point.setY(qRound(FRAME_TOP_MATRIX + PEN_WIDTH + (_p2Point.y() - (FRAME_TOP_MATRIX + PEN_WIDTH)) * (1 - 1.0 / ratio)));

    if (_updateK1)
    {

        if (_p3Point.x() == _p2Point.x())
        {
            _k1 = 0;
        }
        else
        {
            _k1 = (qreal)(_p3Point.y() - _p2Point.y()) / (_p3Point.x() - _p2Point.x());
        }
    }
}

void Con_Dyn_Module::calCompressorP0Pos()
{
    _p0Point.setX(FRAME_LEFT_MATRIX + PEN_WIDTH);
    _p0Point.setY(height() - FRAME_BOTTOM_MATRIX - PEN_WIDTH);
}

void Con_Dyn_Module::calCompressorP1Pos(qreal threshold)
{
    int curPos = (threshold * 10 + 600) * _outTargetLevelScale;

    _p1Point.setX(FRAME_LEFT_MATRIX + curPos);
    _p1Point.setY(FRAME_TOP_MATRIX + PEN_WIDTH + _graphViewSize.height() - curPos);
}

void Con_Dyn_Module::calCompressorP2Pos(qreal ratio)
{
    _p2Point.setX(width() - FRAME_RIGHT_MATRIX - PEN_WIDTH);
    _p2Point.setY(qRound(FRAME_TOP_MATRIX + PEN_WIDTH + (_p1Point.y() - (FRAME_TOP_MATRIX + PEN_WIDTH)) * (1 - 1.0 / ratio)));
}

void Con_Dyn_Module::calLimiterP0Pos()
{
    _p0Point.setX(FRAME_LEFT_MATRIX + PEN_WIDTH);
    _p0Point.setY(height() - FRAME_BOTTOM_MATRIX - PEN_WIDTH);
}

void Con_Dyn_Module::calLimiterP1Pos(qreal threshold)
{
    int curPos = (threshold * 10 + 600) * _outTargetLevelScale;

    _p1Point.setX(FRAME_LEFT_MATRIX + curPos);
    _p1Point.setY(FRAME_TOP_MATRIX + PEN_WIDTH + _graphViewSize.height() - curPos);
}

void Con_Dyn_Module::calLimiterP2Pos()
{
    _p2Point.setX(width() - FRAME_RIGHT_MATRIX - PEN_WIDTH);
    _p2Point.setY(_p1Point.y());
}

void Con_Dyn_Module::calNoiseGateP0Pos()
{
    _p0Point.setY(height() - FRAME_BOTTOM_MATRIX - PEN_WIDTH);
    _p0Point.setX(_p1Point.x() + _p1Point.y() - _p0Point.y());
}

void Con_Dyn_Module::calNoiseGateP1Pos(qreal gain)
{
    int curPos = gain * 10 * _outTargetLevelScale;

    _p1PointIsVisible = true;
    _p1Point.setX(_p2Point.x());
    _p1Point.setY(_p2Point.y() - curPos);

    QRect visibleRect(FRAME_LEFT_MATRIX, FRAME_TOP_MATRIX, _graphViewSize.width(), _graphViewSize.height());
    if (!visibleRect.contains(_p1Point))
    {
        _p1PointIsVisible = false;
        _p1Point.setY(height() - FRAME_BOTTOM_MATRIX - PEN_WIDTH);
    }
}

void Con_Dyn_Module::calNoiseGateP2Pos(qreal threshold)
{
    int curPos = (threshold * 10 + 600) * _outTargetLevelScale;

    _p2Point.setX(FRAME_LEFT_MATRIX - PEN_WIDTH + curPos);
    _p2Point.setY(/*height() - FRAME_BOTTOM_MATRIX - PEN_WIDTH*/FRAME_TOP_MATRIX + PEN_WIDTH + _graphViewSize.height() - curPos);
}

void Con_Dyn_Module::calNoiseGateP3Pos()
{
    _p3Point.setX(width() - FRAME_RIGHT_MATRIX - PEN_WIDTH);
    _p3Point.setY(FRAME_TOP_MATRIX + PEN_WIDTH);
}

void Con_Dyn_Module::calExpanderP0Pos(qreal ratio)
{
    _p0Point.setY(height() - FRAME_BOTTOM_MATRIX - PEN_WIDTH);
    _p0Point.setX(qRound(_p1Point.x() - ratio * (_graphViewSize.height() - (_p1Point.y() - FRAME_TOP_MATRIX - PEN_WIDTH))));
}

void Con_Dyn_Module::calExpanderP1Pos(qreal threshold)
{
    int curPos = (threshold * 10 + 600) * _outTargetLevelScale;

    _p1Point.setX(FRAME_LEFT_MATRIX - PEN_WIDTH + curPos);
    _p1Point.setY(FRAME_TOP_MATRIX + PEN_WIDTH + _graphViewSize.height() - curPos);
}

void Con_Dyn_Module::calExpanderP2Pos()
{
    _p2Point.setX(width() - FRAME_RIGHT_MATRIX - PEN_WIDTH);
    _p2Point.setY(FRAME_TOP_MATRIX + PEN_WIDTH);
}

void Con_Dyn_Module::calClipperP0Pos()
{
    _p0Point.setX(FRAME_LEFT_MATRIX + PEN_WIDTH);
    _p0Point.setY(height() - FRAME_BOTTOM_MATRIX - PEN_WIDTH);
}

void Con_Dyn_Module::calClipperP1Pos(qreal threshold)
{
    int curPos = (threshold * 10 + 600) * _outTargetLevelScale;

    _p1Point.setX(FRAME_LEFT_MATRIX + curPos);
    _p1Point.setY(FRAME_TOP_MATRIX + PEN_WIDTH + _graphViewSize.height() - curPos);
}

void Con_Dyn_Module::calClipperP2Pos()
{
    _p2Point.setX(width() - FRAME_RIGHT_MATRIX - PEN_WIDTH);
    _p2Point.setY(_p1Point.y());
}

void Con_Dyn_Module::outTargetLevelChanged(qreal value)
{
    _outTargetLevel = value;

    calNewPos();
}

void Con_Dyn_Module::thresholdChanged(qreal value)
{
    _threshold = value;

    calNewPos();
}

void Con_Dyn_Module::ratioChanged(qreal value)
{
    _ratio = value;

    _updateK1 = true;

    calNewPos();
}

void Con_Dyn_Module::gainChanged(qreal value)
{
    _gain = value;
    
    calNewPos();
}

void Con_Dyn_Module::setCurPointDisp(bool enable)
{
    _bCurPointDisp = enable;
}

void Con_Dyn_Module::curPointDisp(float iVU, float oVU)
{
    float realW = width() - FRAME_LEFT_MATRIX - FRAME_RIGHT_MATRIX;
    float realH = height() - FRAME_TOP_MATRIX - FRAME_BOTTOM_MATRIX;
    int startX  = FRAME_LEFT_MATRIX + PEN_WIDTH;
    int startY  = height() - FRAME_BOTTOM_MATRIX - PEN_WIDTH;
    int offsetX = realW / 72.0 * ((iVU < -60) ? 0 : (iVU + 60));
    int offsetY = realH / 72.0 * ((oVU < -60) ? 0 : (oVU + 60));
    int curX    = startX + offsetX;
    int curY    = startY - offsetY;
    _curPoint.setX(curX);
    _curPoint.setY(curY);

//    qDebug() << __FUNCTION__ << __LINE__
//             << input_vu << output_vu
//             << realW << realH
//             << startX << startY
//             << offsetX << offsetY
//             << curX  << curY;
    update();
}


void Con_Dyn_Module::setBypass(bool isBypass)
{
    _isBypass = isBypass;
    update();
}
