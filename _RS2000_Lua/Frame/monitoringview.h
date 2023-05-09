#ifndef MONITORINGVIEW_H
#define MONITORINGVIEW_H

#include <QGraphicsView>
#include "absgraphview.h"
#include "monitoringscene.h"

class MonitoringScene;
class MonitoringView : public AbsGraphView
{
    Q_OBJECT
public:
    explicit MonitoringView(QString windowTitle, QString sceneFieName, QWidget *parent = 0);
    inline MonitoringScene *scene(){ return _MonScene; }
    inline QString getName() { return _ViewLabelName; }
    inline QRectF getRect() { return _rect; }
    virtual QUndoStack *getUndoStack();
    virtual void saveScene(bool bMsg = true);
    virtual void saveAsScene();
    virtual QString getWndName();
    inline void setFileName(QString fileName) { _fileName = fileName; }
    inline void saveSceneItem() { _MonScene->saveItem(); }
    void openProjectFile(QString fileName);
protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

signals:
    void viewClosed();
public slots:
    void onGetRectsize(QRectF rect);
    void onSavePro(QString fileName);
    inline void onCloseSignal() { this->close(); }
private:
    MonitoringScene *_MonScene;
    QString _ViewLabelName;
    QRectF _rect;
    QString _fileName;
};

#endif // MONITORINGVIEW_H
