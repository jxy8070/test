#ifndef DYNWIDGET_H
#define DYNWIDGET_H

#include <QWidget>

class DynWidget : public QWidget
{
    Q_OBJECT
public:
    DynWidget(QWidget *parent = 0);
    void setValue(int *px, int *py);
    int *In();
    int *Out();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);
private:
    float m_wMult, m_hMult;
    int m_px[4], m_py[4], m_fpxreal[4], m_fpyreal[4], m_in[4], m_out[4];
    int m_nselect;
    void PressHandle(int x, int y);
    void MoveHandle(int x, int y);
    void ReleaseHandle();
signals:
    void changeValue(int *px, int *py);
};

#endif
