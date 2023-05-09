#ifndef CONFIGVIEW_H
#define CONFIGVIEW_H

#include <QGraphicsView>
#include "absgraphview.h"

class ConfigScene;
class Device_Trn;
//class ControlTemplate;
class ConfigView : public AbsGraphView
{
    Q_OBJECT

public:
    explicit ConfigView(QString title, Device_Trn* trnDev, QWidget *parent = 0);
    ~ConfigView();
    ConfigScene *scene();
    virtual QUndoStack *getUndoStack() override;
    virtual void saveScene(bool bMsg = true) override;
    virtual void saveAsScene() override;
    virtual QString getWndName() override;

protected:
    void showEvent(QShowEvent *) override;
    void closeEvent(QCloseEvent *event) override;
signals:
    void viewClosed();


private:
    ConfigScene *_cfgScene;
//    ControlTemplate *rectItem = NULL;
//    QGraphicsPathItem *_grapPathItem = NULL;
//    QPainterPath _pPath;
};
#endif // CONFIGVIEW_H
