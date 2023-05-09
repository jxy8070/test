#ifndef DEVICESTARTUSWINDOW_H
#define DEVICESTARTUSWINDOW_H

#include "usercontrolinterface.h"
#include <QWidget>

class DeviceModel;
class Device_RT1052_Poe_60W_2_4;
class Device_RT1052_AP400;
class DeviceStartusWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceStartusWindow(QWidget *parent = nullptr);
    void updateDeviceStatus(QList<ProcessorData_t> list);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
signals:

public slots:
    void onAES70DeviceDisplaySlots(struct MsgData_RT1052_Poe);

private:
    typedef struct Arguments{
        float s_voltage;
        float s_temperature;
    }Arguments_t;

    DeviceModel *_devicesModel;
    QTableView *_tableView;
    QList<Device_RT1052_Poe_60W_2_4 *> _ap60List;
    QList<Device_RT1052_AP400 *> _ap400List;
};

#endif // DEVICESTARTUSWINDOW_H
