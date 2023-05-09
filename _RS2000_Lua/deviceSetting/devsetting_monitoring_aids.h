#ifndef DEVSETTING_MONITORING_AIDS_H
#define DEVSETTING_MONITORING_AIDS_H

#include "customWidget/overwriteitem.h"
#include "deviceItem/deviceitem_monitoring_aids.h"
#include <QUndoGroup>
#include "device/device_monitoring_aids.h"

namespace Ui {
class DevSetting_Monitoring_Aids;
}

class Rs2000SubWindow;
class QMdiArea;
class QMdiSubWindow;
class MonitoringView;
enum SubWnd_Type;
class MonitorWindows;
class DevSetting_Monitoring_Aids : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit DevSetting_Monitoring_Aids(DeviceItem_Monitoring_Aids *devItem,QWidget *parent = 0);
    ~DevSetting_Monitoring_Aids();

    void addProjectView(QString fileName);
    void setInitialize();
//    void setStatusBarText(QString text);
    inline bool isSave() { return _save; }
signals:
    void closeView();
    void savePro(QString);
private slots:
    void onSubWindowActivated(QMdiSubWindow *activeWindow);
    void onPushBuottonClicked();
    void onShowDown(bool b);
    void closeEvent(QCloseEvent *);
    void setStatus(QString statusText);
    void setRectSize(QRectF rect);
    void onSuWindowClosed();
    inline void onSave(bool b) { _save = b; }
private:
    Ui::DevSetting_Monitoring_Aids *ui;
    DeviceItem_Monitoring_Aids *_devItem;
    Device_Monitoring_Aids *_device;
    quint32 _subWindowIndex; //子窗口序号
    MonitoringView *_monitorView;

    MonitorWindows *_monitor;
    bool _save;
    QString _fileName;
    QUndoGroup *_undoGroup;
    QAction *_actionUndo;
    QAction *_actionRedo;
};

#endif // DEVSETTING_MONITORING_AIDS_H
