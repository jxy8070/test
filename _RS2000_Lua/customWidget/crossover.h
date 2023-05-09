#ifndef CROSSOVER_H
#define CROSSOVER_H

#include <QWidget>

#define PEN_WIDTH 1

#define X_LEN_1 497

#define FRAME_LEFT_MATRIX   28
#define FRAME_RIGHT_MATRIX  28
#define FRAME_TOP_MATRIX    20
#define FRAME_BOTTOM_MATRIX 20

enum CrossOverTypeEnum {
    CrossOverType_2Way = 0,
    CrossOverType_3Way,
    CrossOverType_4Way,

};

struct crossOverPoint {
    float fs;
    int   filterType;
    float result[X_LEN_1];
};

class CrossOver : public QWidget
{
    Q_OBJECT
public:
    explicit CrossOver(QWidget *parent = 0);
    ~CrossOver();
    void initType(CrossOverTypeEnum type, crossOverPoint *pnt);
    void fsChanged(int idx, float fs);
    void filterTypeChanged(int idx, int type);
    void setCurrentIdx(int idx);

signals:
    void pointChange(int);
    void pointMove(int, float);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:

private:
    CrossOverTypeEnum _type;
    int _pointCount;
    QSize _graphViewSize;
    QLineF _linesH[10];
    QLineF _linesV[32];
    QRectF _labelRectsH[12];
    QRectF _labelRectsV[32];
    struct crossOverPoint *_point;
    float m_x_tab[X_LEN_1], m_xFs_tab[X_LEN_1];
    QStringList _fsStr;
    QStringList _gainStr;
    QRectF _pointRectF[8];
    bool _isMove;
    int _moveNum;
    int _idx;
};

#endif // CROSSOVER_H
