#ifndef MONITORWINDOWS_H
#define MONITORWINDOWS_H

#include <QScrollArea>


class DeviceItem;
class ProjectScene;
class QHBoxLayout;

namespace Ui {
    class MonitorWindows;
}
class MonitorWindows : public QScrollArea
{
    Q_OBJECT

public:
    MonitorWindows(ProjectScene *projScene, QWidget *parent = 0);

public slots:    
    void flashMonitor();

private slots:
    void onPushbuttonClicked();
private:
    void monitorShow();

private:
    Ui::MonitorWindows *ui;
//    QHBoxLayout *_mainLayout;
    ProjectScene *_projScene;
    QList<QWidget*> _monitorList;
    qint32 _curIndex;
};

#endif // MONITORWINDOWS_H
