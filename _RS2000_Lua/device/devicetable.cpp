//#include "stable.h"
//#include "devicetable.h"
//#include "frame/devicetableview.h"
//#include "devicemodel.h"
//#include "devicemanager.h"
//#include "deviceSetting/adddeviceconfig.h"

//DeviceTable::DeviceTable(bool viewMode, QWidget *parent)
//    : QDialog(parent)
//{
//    _viewMode = viewMode;
//    if(_viewMode) {setWindowTitle(tr("view device"));}
//    else {setWindowTitle(tr("device manager"));}
//    setAttribute(Qt::WA_DeleteOnClose);

//    setupUi();
//}

//void DeviceTable::setupUi()
//{
//    QVBoxLayout *main = new QVBoxLayout(this);
//    _tableView = new DeviceTableView(this);
//    main->addWidget(_tableView);
//    if(!_viewMode)
//    {
//        QHBoxLayout *btnLayout = new QHBoxLayout();
//        main->addLayout(btnLayout);
//        QPushButton *addBtn = new QPushButton(tr("Add"), this);
//        connect(addBtn, SIGNAL(clicked()), this, SLOT(onAddDevice()));
//        btnLayout->addWidget(addBtn);

//        QPushButton *deleteBtn = new QPushButton(tr("Del"), this);
//        connect(deleteBtn, SIGNAL(clicked()), this, SLOT(onDelDevice()));
//        btnLayout->addWidget(deleteBtn);

//        QPushButton *closeBtn = new QPushButton(tr("Close"), this);
//        connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
//        btnLayout->addWidget(closeBtn);
//    }
//}

//void DeviceTable::onAddDevice()
//{
//    AddDeviceConfig *dvc = new AddDeviceConfig(this);
//    dvc->exec();
//}

//void DeviceTable::onDelDevice()
//{
//    DeviceModel *devicesModel = DeviceManager::instance()->getDeviceModel();
//    devicesModel->removeDevice(_tableView->currentIndex());
//}
