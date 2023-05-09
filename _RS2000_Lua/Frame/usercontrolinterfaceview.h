#ifndef USERCONTROLINTERFACEVIEW_H
#define USERCONTROLINTERFACEVIEW_H

#include <QWidget>
#include "absgraphview.h"


class UserControlInterfaceScene;
class UserControlInterfaceView : public AbsGraphView
{
    Q_OBJECT
public:
    explicit UserControlInterfaceView(QString title, QGraphicsItem *item, QWidget *parent = nullptr);

    virtual QUndoStack *getUndoStack();
    virtual void saveScene(bool bMsg = true);
    virtual void saveAsScene();
    virtual QString getWndName();

    UserControlInterfaceScene *scene() { return _scene; }
    QGraphicsItem *getGrapItem() { return _GrapItem; }
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void viewClosed();

private:
    UserControlInterfaceScene *_scene;
    QGraphicsItem *_GrapItem;
};

#endif // USERCONTROLINTERFACEVIEW_H
