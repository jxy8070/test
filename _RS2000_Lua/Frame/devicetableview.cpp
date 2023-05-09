#include "stable.h"
#include "devicetableview.h"
#include "device/devicemodel.h"
#include "device/devicemanager.h"
#include "device/AES70device.h"
#include "mainwindow.h"
#include "projectview.h"
#include "projectscene.h"

DeviceTableView::DeviceTableView(QWidget *parent)
    : QTableView(parent)
{
    setSortingEnabled(true); //启用对表的排序
    setSelectionBehavior(SelectionBehavior::SelectRows);
    _devicesModel = DeviceManager::instance()->getDeviceModel();
    setModel(_devicesModel);//显示deviceModel的视图显示
//    setColumnHidden(DevCol_Serial,true);
    setColumnHidden(DevCol_PortInfo,true);
    if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
    {
        setColumnHidden(DevCol_Mac,true);
    }else {
        setAlternatingRowColors(true);
    }
    sortByColumn(DevCol_Status, Qt::DescendingOrder);//项目按照降序排列
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//根据标题自动调整水平标题的大小
    _readOnlyDelegate = new ReadOnlyDelegate();
    setItemDelegateForColumn(DevCol_Mac, _readOnlyDelegate);    //设置某列只读
    setItemDelegateForColumn(DevCol_Ip, _readOnlyDelegate);
    setItemDelegateForColumn(DevCol_Status, _readOnlyDelegate);
    setItemDelegateForColumn(DevCol_Type, _readOnlyDelegate);
    setItemDelegateForColumn(DevCol_TypeDesp, _readOnlyDelegate);
    setItemDelegateForColumn(DevCol_DevName, _readOnlyDelegate);
    setItemDelegateForColumn(DevCol_Serial, _readOnlyDelegate);

    //添加上下文菜单
    _delAction = new QAction(tr("delete"), this);
    connect(_delAction, SIGNAL(triggered()), this, SLOT(removeDevice()));
    _contextMenu.addAction(_delAction);
    _showOnlineAction = new QAction(tr("show online only"), this);
    _showOnlineAction->setCheckable(true);
    _showOnlineAction->setChecked(false);
    connect(_showOnlineAction, SIGNAL(triggered(bool)), this, SLOT(showOnlineDevices(bool)));
    _contextMenu.addAction(_showOnlineAction);

    _openDevModulePath = new QAction(tr("Go to the location of the device"), this);
    connect(_openDevModulePath, SIGNAL(triggered()), this, SLOT(openDeviceModulePath()));
    _contextMenu.addAction(_openDevModulePath);

    setDragDropMode(QAbstractItemView::DragOnly);
    verticalHeader()->setMinimumWidth(30);
    verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
}

void DeviceTableView::contextMenuEvent(QContextMenuEvent *event)//右键菜单事件
{
    Q_UNUSED(event)
    _contextMenu.exec(cursor().pos());//在光标的位置显示上下文菜单
}

void DeviceTableView::resizeEvent(QResizeEvent *event)//获取控件动态变化的宽高
{
    Q_UNUSED(event)

    horizontalHeader()->adjustSize();//调整水平标题的大小使其自动适应其内容
}



void DeviceTableView::removeDevice()
{
    AES70Device *dev = getCurDev();
    if (!dev) return;
    if (!dev->isOnline()) {        
        MainWindow::instance()->showNotice(tr("Device is Online, Can't be deleted!"));
    }
    else {
        _devicesModel->removeDevice(currentIndex());
    }
}


void DeviceTableView::showOnlineDevices(bool isOnlineOnly)
{
    QString filter;
    if(isOnlineOnly){
        filter = QString("status = %1").arg(Dev_Online);      
    }

    _devicesModel->setFilter(filter);
    _devicesModel->select();
}

void DeviceTableView::deviceCurRoomNameChangeds(const QString& oldFilePath, const QString& newFilePath)
{
    QString oldName = QFileInfo(oldFilePath).fileName();
    QString newName = QFileInfo(newFilePath).fileName();
    foreach(AES70Device *dev, _devicesModel->getDeviceList()) {
//        qDebug() << __FUNCTION__ << __LINE__ << dev->getCurRoomName() << oldName << newName;
        if(dev->getCurRoomName() == oldName)
            dev->setCurRoomName(newName);
    }
}

void DeviceTableView::deleteDeviceCurRoomName(const QString &filePath)
{
    QString roomName = QFileInfo(filePath).fileName();
    foreach(AES70Device *dev, _devicesModel->getDeviceList()) {
        if(dev->getCurRoomName() == roomName)
            dev->setCurRoomName(QString());
    }
}

void DeviceTableView::openDeviceModulePath()
{
    QString fileName = _devicesModel->index(currentIndex().row(), DevCol_Serial).data().toString();
    QString devMac   = _devicesModel->index(currentIndex().row(), DevCol_Mac).data().toString();
    if(fileName.isEmpty() || devMac.isEmpty() ) return;
    MainWindow::instance()->openDevModulePath(devMac, fileName);
}

void DeviceTableView::mousePressEvent(QMouseEvent *e)
{
    QTableView::mousePressEvent(e);
//    _dragDev = NULL;
    if(e->button() == Qt::LeftButton)
    {
        _startPos = e->pos();
//        _dragDev = getCurDev();
    }  
}

void DeviceTableView::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        int distance = (e->pos() - _startPos).manhattanLength();
        if(distance >= QApplication::startDragDistance())   //当拖动距离大于一个推荐抖动距离时，表示同意已经拖动操作了
        {
            performDrag();
        }
    }
}

void DeviceTableView::performDrag()
{
//    //2023-03-22 hrp
//    QByteArray array;
//    QDataStream stream(&array, QIODevice::WriteOnly);
//    QList<QModelIndex> list = selectionModel()->selectedRows(DevCol_Mac);
//    foreach(QModelIndex index, list) {
//        QMap<QString, QByteArray> mimeDataMap;
//        QString mac = index.data().toString();
//        AES70Device *AES70Dev = _devicesModel->getDeviceByMac(mac);
//        QString longPtrStr = QString("%1").arg((qulonglong)AES70Dev);
//        QString devType = QString("%1").arg(AES70Dev->getDeviceType());
//        mimeDataMap.insert("type", devType.toUtf8());
//        mimeDataMap.insert("itemPtr", longPtrStr.toUtf8());
//        stream << index.row() << index.column() << mimeDataMap;
//    }
//    if(array.isEmpty()) return ;

//    QMimeData *mimeData = new QMimeData;
//    mimeData->setData("mimeDataList", array);
//    QDrag *drag = new QDrag(this);
//    drag->setMimeData(mimeData);
//    drag->exec(Qt::MoveAction);

    QStringList typeStrList, longPtrStrList;
    QList<QModelIndex> list = selectionModel()->selectedRows(DevCol_Mac);
    foreach(QModelIndex index, list) {
        QString mac = index.data().toString();
        AES70Device *AES70Dev = _devicesModel->getDeviceByMac(mac);
        if(!AES70Dev) continue;
        QString typeStr = QString("%1").arg(AES70Dev->getDeviceType());
        QString longPtrStr = QString("%1").arg((qulonglong)AES70Dev);
        typeStrList << typeStr;
        longPtrStrList << longPtrStr;
    }
    if(typeStrList.count() < 0) return ;

    QMimeData *mineData = new QMimeData;
    mineData->setText(QString("Count:%1").arg(typeStrList.length()));
    mineData->setData("type", typeStrList.join(",").toUtf8());
    mineData->setData("itemPtr", longPtrStrList.join(",").toUtf8());
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mineData);
    drag->exec(Qt::MoveAction);
}


AES70Device* DeviceTableView::getCurDev()
{
    QString mac = _devicesModel->index(currentIndex().row(), DevCol_Mac).data().toString();
    return _devicesModel->getDeviceByMac(mac);
}

