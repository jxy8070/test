#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H

#include <QGraphicsView>
#include "absgraphview.h"

//class DeviceItem_Room;
class ProjectScene;
class DeviceItem_Trn;
class ProjectView : public AbsGraphView
{
    Q_OBJECT
public:
    explicit ProjectView(QString title, QWidget *parent = 0);
    ~ProjectView();
    ProjectScene *scene();
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
    void onDownloadProjectRoute();

private:
    ProjectScene *_proScene;
//    DeviceItem_Room *_room;
};

#endif // PROJECTVIEW_H
