#include "devicestartuswindow.h"
#include "device/devicemodel.h"
#include "device/devicemanager.h"
#include "device/device_rt1052_poe_60w_2_4.h"
#include "device/device_rt1052_AP400.h"

DeviceStartusWindow::DeviceStartusWindow(QWidget *parent) : QWidget(parent),
    _tableView(new QTableView(this))
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(_tableView);
    _devicesModel = DeviceManager::instance()->getDeviceModel();
    _devicesModel->setHeaderData(DevCol_Serial, Qt::Horizontal, tr("Voltage"));
    _devicesModel->setHeaderData(DevCol_PortInfo, Qt::Horizontal, tr("Temperature"));

    _tableView->setModel(_devicesModel);
    _tableView->setColumnHidden(DevCol_Mac,true);
//    _tableView->setColumnHidden(DevCol_Serial,true);
//    _tableView->setColumnHidden(DevCol_PortInfo,true);
    _tableView->sortByColumn(DevCol_Status, Qt::DescendingOrder);//设备状态降序排列
    _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//根据标题自动调整水平标题的大小
    _tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _tableView->setSortingEnabled(true);
    _tableView->horizontalHeader()->setSortIndicatorShown(true);
    _tableView->setMouseTracking(true);
//    connect(_tableView, &QTableView::entered, this, &DeviceStartusWindow::onTabelViewEnteredSlots);
    _tableView->setStyleSheet("QHeaderView::section {"
                              "color: white;"
                              "background: rgb(53,57,66);"
                            "}"
                            "QTableView {\
                                color: white;\
                                gridline-color: rgb(105, 106, 110);\
                                background: rgb(53, 57, 66);\
                                alternate-background-color: rgb(0, 255, 0);\
                                selection-color: white;\
                                selection-background-color: rgb(53, 53, 53);\
                                border: 0px;\
                                border-radius: 0px;\
                            }"
                            "QTableView QTableCornerButton::section {\
                                  width: 30px;\
                                  background: rgb(53, 57, 66);;\
                                  border: 0px outset rgb(53, 57, 66);\
                            }"
                            "* {background-color: rgb(53, 57, 66);}"
                            "QScrollBar { background-color: rgb(53, 57, 66); color: white; }"
                            "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}"
                            "QScrollBar::up-arrow:vertical {"
                            "    border: 0px solid grey;"
                            "    width: 10px;"
                            "    height: 10px;"
                            "    image: url(:/Image/Fangtu/up_arrow.png);"
                            "}"
                            "QScrollBar::down-arrow:vertical {"
                            "    border: 0px solid grey;"
                            "    width: 10px;"
                            "    height: 10px;"
                            "    image: url(:/Image/Fangtu/down_arrow.png);"
                            "}"
                            );

    resize(800, 600);
    setWindowTitle(tr("Device Status"));
}

void DeviceStartusWindow::updateDeviceStatus(QList<ProcessorData_t> list)
{
    _ap60List.clear();
    _ap400List.clear();
    foreach(ProcessorData_t data, list) {
        AES70Device *dev = _devicesModel->getDeviceByMac(data.s_mac);
        if(!dev) continue;
        if(dev->getDeviceType() == Dev_Type_RT1052_AP60) {
            Device_RT1052_Poe_60W_2_4 *ap60 = qobject_cast<Device_RT1052_Poe_60W_2_4 *>(dev);
            if(!ap60) continue;
            /*qDebug() << __FUNCTION__ << __LINE__ << */ap60->disconnect();
            connect(ap60, SIGNAL(displaySig(struct MsgData_RT1052_Poe)), this,
                    SLOT(onAES70DeviceDisplaySlots(struct MsgData_RT1052_Poe)));
            ap60->sendStartMonitor();
            _ap60List.append(ap60);
        }
        if(dev->getDeviceType() == Dev_Type_RT1052_AP400) {
            Device_RT1052_AP400 *ap400 = qobject_cast<Device_RT1052_AP400 *>(dev);
            if(!ap400) continue;
            /*qDebug() << __FUNCTION__ << __LINE__ << */ap400->disconnect();
            connect(ap400, SIGNAL(displaySig(struct MsgData_RT1052_Poe)), this,
                    SLOT(onAES70DeviceDisplaySlots(struct MsgData_RT1052_Poe)));
            ap400->sendStartMonitor();
            _ap400List.append(ap400);
        }
        if(dev->getDeviceIp() == data.s_ip) {
            dev->setDeviceName(data.s_customName, false);
        }
    }
}

void DeviceStartusWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    _tableView->horizontalHeader()->adjustSize();
}

void DeviceStartusWindow::closeEvent(QCloseEvent *event)
{
    foreach(Device_RT1052_Poe_60W_2_4 *ap60, _ap60List) {
        ap60->sendStopMonitor();
    }
    foreach(Device_RT1052_AP400 *ap400, _ap400List) {
        ap400->sendStopMonitor();
    }
}


void DeviceStartusWindow::onAES70DeviceDisplaySlots(struct MsgData_RT1052_Poe msgData)
{
//    qDebug() << __FUNCTION__ << __LINE__ << msgData._monitor._line_60w_4_4.AD1_Value
//             << msgData._monitor._line_60w_4_4.temperature;
    AES70Device *dev = qobject_cast<AES70Device *>(sender());
    if(!dev) return ;

    if(msgData._monitor._line_60w_4_4.AD1_Value > 85.0f
            || msgData._monitor._line_60w_4_4.temperature > 45.0f)
        return ;
    QString strVol = tr("%1 V").arg(QString::number(msgData._monitor._line_60w_4_4.AD1_Value, 'f', 1));
    QString strTemp = tr("%2 C").arg(QString::number(msgData._monitor._line_60w_4_4.temperature, 'f', 1));
     for(int row = 0; row < _devicesModel->rowCount(); row++) {
         QString mac = _devicesModel->index(row, DevCol_Mac).data().toString();
         if(dev->getAES70Mac() == mac) {
             _devicesModel->setData(_devicesModel->index(row, DevCol_Serial), strVol);
             _devicesModel->setData(_devicesModel->index(row, DevCol_PortInfo), strTemp);
         }
     }
}
