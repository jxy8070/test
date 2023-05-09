#include "usercontrolinterface.h"
#include <QTabWidget>
#include "uciitem_port.h"
#include "uciitem_mixer.h"
#include "device/AES70device.h"
#include "device/devicemanager.h"
#include "device/device_trn.h"
#include "deviceItem/deviceitemfactory.h"
#include "deviceItem/deviceitem_trn.h"
#include <QDesktopWidget>
#include "uciitemview.h"
#include "Lua/Packages/centerctrl.h"
//#include <QDockWidget>
#include "Frame/toolkittree.h"
#include "devicestartuswindow.h"
#include <QShortcut>

#define WINDOW_H 1080
#define WINDOW_W 720
#define TREEWIDGET_W 200

QMutex UserControlInterface::Mutex;
UserControlInterface *UserControlInterface::_instance;

UserControlInterface *UserControlInterface::instance()
{
    if(!_instance) {
        QMutexLocker locker(&Mutex);
        if(!_instance)
            _instance = new UserControlInterface;
    }
    return _instance;
}

UserControlInterface::UserControlInterface(QWidget *parent)
    : QMainWindow{parent}
{
    _tabWidget = new TabWidget(this);
    _devStatusWin = new DeviceStartusWindow;

    _sideBar = new QTreeWidget(this);
    _sideBar->setHeaderHidden(true);
    _sideBar->setFixedSize(TREEWIDGET_W, WINDOW_W);
    _sideBar->move(-_sideBar->width(), 0);

    _sidePush = new QPushButton(">>", this);
    _sidePush->setFixedSize(25, 40);
    _sidePush->move(-1, (_sideBar->height()-_sidePush->height())/2);

    _pptyAnimation_sBar = new QPropertyAnimation(_sideBar, "geometry");
    _pptyAnimation_sBar->setEasingCurve(QEasingCurve::InOutSine);
    _pptyAnimation_sBar->setDuration(500);

    _pptyAnimation_sBtn = new QPropertyAnimation(_sidePush, "geometry");
    _pptyAnimation_sBtn->setEasingCurve(QEasingCurve::InOutSine);
    _pptyAnimation_sBtn->setDuration(500);

    setCentralWidget(_tabWidget);
    setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint);
    setWindowTitle(tr("User Control Interface"));

//    //快捷键
    QShortcut *shortCut_open = new QShortcut(Qt::CTRL + Qt::Key_O, this);
    QShortcut *shortCut_status = new QShortcut(Qt::CTRL + Qt::Key_W, this);

    connect(_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidgetCurrentChangedSlot(int)));
    connect(_tabWidget, SIGNAL(deleteUciItem(ConfigItemUCI *)), this, SLOT(removeLoadUciItem(ConfigItemUCI *)));
    connect(DeviceManager::instance(), &DeviceManager::deviceStateChanged, this, &UserControlInterface::updateDeviceState);
    connect(shortCut_open, SIGNAL(activated()), this, SLOT(openActionTriggeredSlot()));
    connect(shortCut_status, SIGNAL(activated()), this, SLOT(viewOnlineStatus()));
    connect(_sidePush, SIGNAL(clicked()), this, SLOT(onSidePushClickedSlot()));
    connect(_sideBar, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(onTreeWidgetDoubleClickedSlot(QTreeWidgetItem*,int)));

    QString defPath;
    Utils::loadDefaultWorkingPath(defPath);
    initTreeWidget(defPath/* + "/Untitled/UCI"*/);
//    showMaximized();
    initUi();
    resize(WINDOW_H, WINDOW_W);
}

void UserControlInterface::openProcData(QString fileName)
{
    QSettings *ConfigIni = new QSettings(fileName, QSettings::IniFormat, 0);
    qDebug() << __FUNCTION__ << __LINE__ << ConfigIni->childGroups();
    QStringList MacList = ConfigIni->childGroups().filter("UCI_");

    //外围设备
    _peripheralDeviceList.clear();
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_GROUP));
    int count = ConfigIni->value(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_COUNT)).toInt();
    for(int i = 0; i < count; i++) {
        ProcessorData_t data;
        ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DATA).arg(i));
        data.s_mac  = ConfigIni->value(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DEVMAC)).toString();
        data.s_type = ConfigIni->value(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DEVTYPE)).toInt();
        data.s_ip   = ConfigIni->value(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DEVIP)).toString();
        data.s_customName = ConfigIni->value(QString(CONFIG_DEVICE_DEV_UCI_PERIPHERALDEVICE_DEVNAME)).toString();
        ConfigIni->endGroup();
        _peripheralDeviceList.append(data);
//        qDebug() << __FUNCTION__ << __LINE__ << data.s_mac << data.s_ip << data.s_customName << data.s_type;
    }
    ConfigIni->endGroup();
    delete ConfigIni;

//    _loadUCiItemList.clear();
    QPointF pos(20, 20*(90 + 20));
    foreach(QString mac, MacList) {
        mac.remove(0, 4);
        qDebug() << __FUNCTION__ << __LINE__ << mac;
        Device_Trn *trnDev = qobject_cast<Device_Trn *>(DeviceManager::instance()->getDeviceByMac(mac));
        if(!trnDev) continue;
        ConfigItemUCI *uciItem = qgraphicsitem_cast<ConfigItemUCI *>(
                    ConfigItemFactory::createConfigItem(trnDev, CFG_TYPE_UCI, QString("userControlInterface"), pos));
        open(fileName, mac, uciItem);
        _loadUCiItemList.insert(mac, uciItem);
    }
}

void UserControlInterface::open(QString fileName, QString Mac, ConfigItemUCI *uciItem)
{
    QSettings *ConfigIni = new QSettings(fileName, QSettings::IniFormat, 0);
    ConfigIni->beginGroup(QString(UCI_GROUP).arg(Mac));
    ConfigIni->beginGroup(QString(CONFIG_DEVICE_DEV_UCI_GROUP));
    QString toLoadAES70DevIp    = ConfigIni->value(QString(UCI_DEV_AES70DEV_IP)).toString();
    QString toLoadAES70DevCName = ConfigIni->value(QString(UCI_DEV_AES70DEV_CNAME)).toString().toUtf8();
    DevType toLoadAES70DevType  = (DevType)ConfigIni->value(QString(UCI_DEV_AES70DEV_DEVTYPE)).toInt();
    qDebug() << __FUNCTION__ << __LINE__ << "IP: " << toLoadAES70DevIp << " " << "cName: "
             << toLoadAES70DevCName << "devType: " << toLoadAES70DevType;
    ConfigIni->endGroup();
    uciItem->loadParaFromSetting(ConfigIni);

    ConfigIni->endGroup();
    delete ConfigIni;

    ProcessorData_t devData;
    devData.s_mac  = Mac;
    devData.s_ip   = toLoadAES70DevIp;
    devData.s_type = toLoadAES70DevType;
    devData.s_customName = toLoadAES70DevCName;
    uciItem->setLoadUciFileProcData(devData);
}

bool UserControlInterface::devIsOnline()
{
    QString mac = _tabWidget->currentWidget()->property("Mac").toString();
    ConfigItemUCI *uciItem = _loadUCiItemList.find(mac).value();
    if(!uciItem || !uciItem->getAES70Dev()->isOnline())
        return false;
    else
        return true;
}

void UserControlInterface::resizeEvent(QResizeEvent *event)
{
    _sideBar->setFixedHeight(event->size().height());
    int x = -1;
    if(_sideFlag) x = _sideBar->width();
    _sidePush->move(x, (_sideBar->height()-_sidePush->height())/2);
}

void UserControlInterface::onDeviceStatusChangedSlots()
{
    if(!_tabWidget->count()) return ;
    Device_Trn *trnDev = qobject_cast<Device_Trn *>(sender());
    if(!trnDev) return ;
    for(int idx = 0; idx < _tabWidget->count(); idx++) {
        QString mac = _tabWidget->widget(idx)->property("Mac").toString();
        if(trnDev->getAES70Mac() == mac) {
            ConfigItemUCI *uciItem = _loadUCiItemList.find(mac).value();
            if(!uciItem) continue;
            if(!uciItem->getAES70Dev())
                uciItem->setAES70Device(trnDev);

            QString title, AES70DevStatus;
            DevStatus devState = uciItem->getAES70Dev()->getStatus();
            AES70DevStatus = AES70Device::getStatusName(devState);
            switch(devState) {
            case DevStatus::Dev_Offline:
            {
                _tabWidget->widget(idx)->setEnabled(false);
                break;
            }
            case DevStatus::Dev_Online:
            {
                 CenterCtrl::instance()->setDevAddress(uciItem->getLoadUciFileProcData().s_ip);
                 emit loadDataSignal(uciItem->getLoadUciFileProcData().s_ip);
                 _tabWidget->widget(idx)->setEnabled(true);
                break;
            }
            default: break;
            }
            title += uciItem->getLoadUciFileProcData().s_ip + "\n";
            title += uciItem->getLoadUciFileProcData().s_customName + "\n";
            title += "(" + AES70DevStatus + ")";
            _tabWidget->setTabText(idx, title);
            break;
        }
//        qDebug() << __FUNCTION__ << __LINE__ << title;
    }
}

void UserControlInterface::updateDeviceState(AES70Device * dev)
{
    QHashIterator<QString, ConfigItemUCI *> i(_loadUCiItemList);
    while(i.hasNext()) {
        i.next();
        if(!i.value()->getAES70Dev()) {
            i.value()->setAES70Device(qobject_cast<Device_Trn *>(DeviceManager::instance()->getDeviceByMac(i.key())));
        }
        if(i.value()->getAES70Dev()){
            qDebug() << __FUNCTION__ << __LINE__ << i.key()
                     << i.value()->getAES70Dev()->getDeviceName()
                     << i.value()->getAES70Dev()->getDeviceIp()
                     << i.value()->getAES70Dev()->isOnline();

            i.value()->getAES70Dev()->disconnect(SIGNAL(deviceStatusChanged(DevStatus)));
            connect(i.value()->getAES70Dev(),
                    SIGNAL(deviceStatusChanged(DevStatus)), this, SLOT(onDeviceStatusChangedSlots()));
            Device_Trn *trn = qobject_cast<Device_Trn *>(i.value()->getAES70Dev());
            if(trn) {
                trn->setDebugMode(true);
                trn->disconnect(SIGNAL(InOutLevelSig(unsigned int *)));
                connect(trn, SIGNAL(InOutLevelSig(unsigned int *)), this, SLOT(InOutLevelSlot(unsigned int *)));
            }
        }
    }
}

void UserControlInterface::InOutLevelSlot(unsigned int *level)
{
//    qDebug() << __FUNCTION__ << __LINE__ << _inAndOutList.count();
    foreach(UciItem_port *item, _inAndOutList) {
        if(!item || !item->getUciItem() || !item->getUciItem()->getAES70Dev()) continue;
        Device_Trn *trn = qobject_cast<Device_Trn *>(item->getUciItem()->getAES70Dev());
        if(!trn) continue;
        if(sender() == trn)
            item->setInOutLevel(level);
    }
}

void UserControlInterface::tabWidgetCurrentChangedSlot(int idx)
{
    if(idx == -1) return ;//空tab页
    QString mac = _tabWidget->widget(idx)->property("Mac").toString();
    ConfigItemUCI *uciItem = _loadUCiItemList.find(mac).value();
    if(!uciItem) return;
    CenterCtrl::instance()->setDevAddress(uciItem->getLoadUciFileProcData().s_ip);
}

void UserControlInterface::initUi()
{
//    qDebug() << __FUNCTION__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
//             << _tabWidget->count();
    _tabWidget->disconnect(SIGNAL(currentChanged(int)));
    for(int idx = 0; idx < _tabWidget->count(); idx++) {
        QWidget *wid = _tabWidget->widget(idx);
        QString mac = wid->property("Mac").toString();
//        DeviceManager::instance()->getDeviceByMac(mac)->disconnect(SIGNAL(deviceStatusChanged(DevStatus)));
//        DeviceManager::instance()->getDeviceByMac(mac)->disconnect(SIGNAL(InOutLevelSig(unsigned int *)));
        delete wid;
    }
    _tabWidget->clear();
    _inAndOutList.clear();

    foreach(ConfigItemUCI *uciItem, _loadUCiItemList){
        QTabWidget *tabWdg = new QTabWidget;
        int page = 0;
        while(true) {
            QMap<int, ConfigItemUCI::Uci_data_t *> list = uciItem->getTabWidgetItem(page++);
            if(list.isEmpty()) break;
            foreach(ConfigItemUCI::Uci_data_t *item, list) {
                if((item->s_cfgType&FCT_FAMILY_MASK) == CFG_TYPE_INPUT_1 ||
                   (item->s_cfgType&FCT_FAMILY_MASK) == CFG_TYPE_OUTPUT_1) {
                    UciItem_port* port = new UciItem_port(uciItem, item->s_page);
                    connect(this, QOverload<QString>::of(&UserControlInterface::loadDataSignal),
                            port, QOverload<QString>::of(&UciItem_port::initData));
                    _inAndOutList.append(port);
                    tabWdg->insertTab(item->s_page, port, QString("Page %1").arg(page));
                    break;
                }
                if((item->s_cfgType&FCT_FAMILY_MASK) == CFG_TYPE_MIXER_4X1) {
                    if(!item->s_visible) continue;
                    UciItemView *view = new UciItemView(item->s_name, item->s_mixerNodesSwitch, (CfgType)item->s_cfgType, uciItem);
                    connect(this, QOverload<QString>::of(&UserControlInterface::loadDataSignal),
                            view, QOverload<QString>::of(&UciItemView::initData));
                    tabWdg->insertTab(item->s_page, view, QString("Page %1").arg(item->s_page+1));
                    break;
                }

            }
        }
//        qulonglong longPtr = (qulonglong)uciItem;
        tabWdg->setProperty("Mac", QVariant(uciItem->getLoadUciFileProcData().s_mac));
        _tabWidget->addTab(tabWdg, uciItem->getLoadUciFileProcData().s_ip + "\n" +
                           uciItem->getLoadUciFileProcData().s_customName + "\n" + tr("Offline"));
        tabWdg->setEnabled(false);
        tabWdg->setStyleSheet(QLatin1String(QTabWidgetSyle));
    }
    connect(_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidgetCurrentChangedSlot(int)));
//    qDebug() << __FUNCTION__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
//                << _tabWidget->count();
//    setStyleSheet(QLatin1String("background: rgba(78,92,90,255)"));
}


void UserControlInterface::openActionTriggeredSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), QDir::currentPath(), tr("file(*.uci)"));
    if(fileName.isEmpty()) return;
    openProcData(fileName);
    afterOpeningTheFile();
}

void UserControlInterface::removeLoadUciItem(ConfigItemUCI *cfgUci)
{
    cfgUci->getAES70Dev()->disconnect(SIGNAL(deviceStatusChanged(DevStatus)));
    cfgUci->getAES70Dev()->disconnect(SIGNAL(InOutLevelSig(unsigned int *)));
    foreach(UciItem_port *item, _inAndOutList) {
        if(item->getUciItem() == cfgUci) {
            _inAndOutList.removeAll(item);
        }
    }
    _loadUCiItemList.remove(cfgUci->getLoadUciFileProcData().s_mac);
}


void UserControlInterface::viewOnlineStatus()
{
    _devStatusWin->updateDeviceStatus(_peripheralDeviceList);
    _devStatusWin->show();
}

void UserControlInterface::onSidePushClickedSlot()
{
    if(_sideFlag) {
        _pptyAnimation_sBar->setStartValue(QRect(0, 0, _sideBar->width(), _sideBar->height()));
        _pptyAnimation_sBar->setEndValue(QRect(-_sideBar->width(), 0, _sideBar->width(), _sideBar->height()));
        _pptyAnimation_sBar->start();

        _pptyAnimation_sBtn->setStartValue(QRect(_sideBar->width()-2, _sideBar->height()/2-_sidePush->height()/2,
                                                _sidePush->width(), _sidePush->height()));
        _pptyAnimation_sBtn->setEndValue(QRect(-1, _sideBar->height()/2-_sidePush->height()/2,
                                                    _sidePush->width(), _sidePush->height()));
        _pptyAnimation_sBtn->start();
        _sidePush->setText(">>");
    }
    else {
        _pptyAnimation_sBar->setStartValue(QRect(-_sideBar->width(), 0, _sideBar->width(), _sideBar->height()));
        _pptyAnimation_sBar->setEndValue(QRect(0,0, _sideBar->width(), _sideBar->height()));
        _pptyAnimation_sBar->start();

        _pptyAnimation_sBtn->setStartValue(QRect(-1, _sideBar->height()/2-_sidePush->height()/2,
                                                 _sidePush->width(), _sidePush->height()));
        _pptyAnimation_sBtn->setEndValue(QRect(_sideBar->width()-2, _sideBar->height()/2-_sidePush->height()/2,
                                               _sidePush->width(), _sidePush->height()));
        _pptyAnimation_sBtn->start();
        _sidePush->setText("<<");
    }
    _sideFlag = !_sideFlag;
}

void UserControlInterface::onTreeWidgetDoubleClickedSlot(QTreeWidgetItem *treeItem, int col)
{
    Q_UNUSED(col);
    if(treeItem->childCount()) return ;

    QString roomDirPath = treeItem->parent()->data(0, RoomDirPath).toString();
    roomDirPath += "/" + treeItem->text(0);
    QDir dir(roomDirPath);
    QFileInfoList uciFiles = dir.entryInfoList(QStringList() << "*.uci", QDir::NoDotAndDotDot|QDir::Files);
    if(uciFiles.isEmpty()) return ;
    _loadUCiItemList.clear();
    foreach(QFileInfo uciFile, uciFiles)
        openProcData(uciFile.filePath());
    afterOpeningTheFile();
}

void UserControlInterface::afterOpeningTheFile()
{
//    QProgressDialog *progressDlg =  new QProgressDialog;//关于缓慢操作的一个进度反馈
//    progressDlg->setWindowModality(Qt::ApplicationModal);//模态窗口
//    progressDlg->setMinimumDuration(1);//以1秒的延时进入进度条
//    progressDlg->setWindowTitle(tr("please wait"));
//    progressDlg->setLabelText(tr("Loading file......"));
//    progressDlg->setCancelButton(0);//取消按钮不会被显示
//    progressDlg->setRange(0, pageNames.count()+1);
//    int proCount = 1;

    initUi();
    updateDeviceState(NULL);

    for(int idx = 0; idx < _tabWidget->count(); idx++) {
        QWidget *wid = _tabWidget->widget(idx);
        if(!wid) continue;
        QString mac = wid->property("Mac").toString();
        ConfigItemUCI *uciItem = _loadUCiItemList.find(mac).value();
        qDebug() << __FUNCTION__ << __LINE__ << idx << mac << uciItem->getLoadUciFileProcData().s_ip << _tabWidget->tabText(idx);
        if(!uciItem) continue;
        if(uciItem->getAES70Dev()->isOnline()) {
            QString title;
            title += uciItem->getLoadUciFileProcData().s_ip + "\n";
            title += uciItem->getLoadUciFileProcData().s_customName + "\n";
            title += "(" + tr("Online") + ")";
            _tabWidget->setTabText(idx, title);
            //初始化数据之前要验证设备中是否有场景在运行
            emit loadDataSignal(uciItem->getLoadUciFileProcData().s_ip);
            wid->setEnabled(true);
        }
    }
}

void UserControlInterface::initTreeWidget(QString filePath)
{
//    //获取uci文件目录结构
//    QDir dir(filePath);
//    QFileInfoList roomPathList = dir.entryInfoList(QStringList() << "*", QDir::NoDotAndDotDot|QDir::Dirs);
//    foreach(QFileInfo fileInfo, roomPathList) {
//        QDir roomDir(fileInfo.filePath());
//        QStringList uciFiles = roomDir.entryList(QStringList() << "*.uci", QDir::NoDotAndDotDot|QDir::Files);
////        qDebug() << __FUNCTION__ << __LINE__ << fileInfo.filePath() << uciFiles;
//        QTreeWidgetItem *roomItem = new QTreeWidgetItem(QStringList() << fileInfo.fileName());
//        foreach (QString file, uciFiles) {
//            roomItem->addChild(new QTreeWidgetItem(roomItem, QStringList() << file));
//        }
//        _sideBar->addTopLevelItem(roomItem);
//    }

    //获取默认工作路径所有solution
    QDir dir(filePath);
    QFileInfoList slnPathList = dir.entryInfoList(QStringList() << "*", QDir::NoDotAndDotDot|QDir::Dirs);
    foreach(QFileInfo slnFInfo, slnPathList) {
        QDir uciDir(slnFInfo.filePath() + "/UCI");
        QFileInfoList roomDirList = uciDir.entryInfoList(QStringList() << "*", QDir::NoDotAndDotDot|QDir::Dirs);
        if(!uciDir.exists()) break;
//        qDebug() << __FUNCTION__ << __LINE__ << slnFInfo.filePath() << roomDirList;
        QTreeWidgetItem *slnItem = new QTreeWidgetItem(QStringList() << slnFInfo.fileName());
        slnItem->setData(0, RoomDirPath, slnFInfo.filePath() + "/UCI");
        foreach (QFileInfo roomFInfo, roomDirList) {
            slnItem->addChild(new QTreeWidgetItem(slnItem, QStringList() << roomFInfo.fileName()));
        }
        _sideBar->addTopLevelItem(slnItem);
    }
    _sideBar->expandAll();
}
