#ifndef RS2000GRAPHICSVIEW_H
#define RS2000GRAPHICSVIEW_H

#include <QGraphicsView>

enum SubWnd_Type;
class QUndoStack;
class DesignItem;

class AbsGraphView: public QGraphicsView
{
    Q_OBJECT
public:
    AbsGraphView(SubWnd_Type type, QString wndTitle, QWidget *parent = NULL);
    virtual SubWnd_Type getType() { return _type; }
    virtual QUndoStack *getUndoStack() =0;
    virtual void saveScene(bool bMsg = true) =0;
    virtual void saveAsScene() =0;
    virtual QString getWndName() =0;
    virtual QString getSheetSize() const;
    void changedScale(int newScalePercent);

    inline int getScaleValue() { return _scalePercent; }
    inline QPoint getMousePressPoint() { return _mousePressedPoint; }
    inline QPointF getMousePressPointToScene() { return mapToScene(_mousePressedPoint); }
    inline QRectF getRubberBandRectToScene() { return _rubberRect; }
public slots:
    void onTitleStateChanged(bool needSave);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
signals:
    void onScaleValueChanged(qreal);
private:
    QString _wndTitle;
    SubWnd_Type _type;
    QPoint _mousePressedPoint;    
    int _scalePercent;
    QRectF _rubberRect;
};

#endif // RS2000GRAPHICSVIEW_H
