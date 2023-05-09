#ifndef SOLUTIONVIEW_H
#define SOLUTIONVIEW_H

#include <QGraphicsView>
#include "absgraphview.h"

class SolutionScene;
class DeviceItem_Trn;
class SolutionView : public AbsGraphView
{
    Q_OBJECT
public:
    explicit SolutionView(QString title, QWidget *parent = 0);

    SolutionScene *scene();
    virtual QUndoStack *getUndoStack();
    virtual void saveScene(bool bMsg = true);
    virtual void saveAsScene();
    virtual QString getWndName();
protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *event);

signals:
    void viewClosed();
    void isSaved();

public slots:
    void onDownloadSolutionRoute();  
//    void newConfigView(DeviceItem_Trn* trnItem);

private:
    SolutionScene *_slnScene;
};

#endif // SOLUTIONVIEW_H
