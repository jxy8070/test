#ifndef DEVICEPROPERTY_H
#define DEVICEPROPERTY_H

#include "customWidget/overwriteitem.h"
#include "ui_deviceproperty.h"

//class Ui::DeviceProperty;
class DeviceItem;
class QTableWidgetItem;
class DevicePortItem;
class AES70Device;
////////////////////////////////////////////////////////////////
/// \brief 设备属性修改界面
///
class DeviceProperty : public DevSettingDlg
{
    Q_OBJECT

public:
    explicit DeviceProperty(DeviceItem *devItem, QWidget *parent = 0);
    ~DeviceProperty();

public slots:

private slots:
    void onDeviceSelected(int row, int column);
    void onOkCancel(QAbstractButton *button);
    void on_btnBindDev_clicked();

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::DeviceProperty *_ui;
    DeviceItem *_devItem;
    AES70Device* _bindDev;
    QList<AES70Device*> _devList;
    QTableWidget* _devSelectTabel = NULL;
};
#endif // DEVICEPROPERTY_H
