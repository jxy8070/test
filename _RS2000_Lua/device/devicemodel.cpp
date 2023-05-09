#include "stable.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include "devicemodel.h"
#include "AES70device.h"
#include "devicemanager.h"
#include "sysdbutility.h"

DeviceModel::DeviceModel(DeviceManager *deviceManager)
{
    _devManager = deviceManager;
    setTable("device");
    clearStatus();
    initDeviceList();
    setHeaderData(DevCol_Mac, Qt::Horizontal, tr("MAC Addr"));
    setHeaderData(DevCol_TypeDesp, Qt::Horizontal, tr("Dev TypeName"));
    setHeaderData(DevCol_Type, Qt::Horizontal, tr("Dev Type"));
    setHeaderData(DevCol_Ip, Qt::Horizontal, tr("IP Addr"));
    setHeaderData(DevCol_DevName, Qt::Horizontal, tr("Dev Name"));
    setHeaderData(DevCol_Status, Qt::Horizontal, tr("Dev Status"));
    setHeaderData(DevCol_Serial, Qt::Horizontal, tr("Current Position"));//设备模块当前所在标签页
}

void DeviceModel::clearStatus()
{
    QSqlQuery qry = QSqlQuery(database());

    bool rs = false;
    rs = qry.exec(SysDBUtility::_deleteUnknowDevice);
    rs = qry.exec(SysDBUtility::_deleteSpecialDevice);
    rs = qry.exec(SysDBUtility::_resetDevice);
    submitAll();
}



void DeviceModel::deleteOfflineDevice()
{
    QSqlQuery qry = query();
    qry.exec(SysDBUtility::_deleteOfflineDevice);
}

//从数据库加载历史设备列表
void DeviceModel::initDeviceList()
{
    select();
    int rowNum = rowCount();
    if(rowNum > 0)
    {
        QMutexLocker locker(&_MapMutex);
        for(int i= 0; i< rowNum; i++)
        {
            QString mac = record(i).value(DevCol_Mac).toString();
            uint32 devType = record(i).value(DevCol_Type).toUInt();
            QString devName = record(i).value(DevCol_DevName).toString();
            QString devIp = record(i).value(DevCol_Ip).toString();
            QString creator = "sqlite";
    //        qDebug() << __FUNCTION__ << __LINE__ << mac << devType << devName << devIp;
            AES70Device* device = AES70Device::createDevice(mac, devName, DevType(devType), devIp, creator);
            if (device)
            {
                device->switchStatus(Dev_Offline);
                _deviceList.append(device);
            }
        }

        for(AES70Device* dev : _deviceList)
        {
            QString mac = dev->getAES70Mac();
            _deviceMap[mac] = dev;
        }
    }
}


QList<AES70Device*> DeviceModel::getDeviceList()
{
    QMutexLocker locker(&_MapMutex);
    return QList<AES70Device*>(_deviceList);
}

QList<AES70Device*> DeviceModel::getDeviceListByType(DevType type, bool onlineOnly)
{
    QList<AES70Device*> devList;
    QList<AES70Device*> allDevList = getDeviceList();
    for(AES70Device* dev : allDevList)
    {
        //Dev_Type_Unknow获取全部设备
        if(type == Dev_Type_Unknow || dev->getDeviceType() == type) {
            if(onlineOnly){
                if(dev->isOnline()) devList.append(dev);
            }
            else{
                if(dev->isOnline()) devList.insert(0,dev);
                else devList.append(dev);
            }
        }
    }
    return devList;
}

AES70Device* DeviceModel::getDeviceByMac(QString mac)
{
    QMutexLocker locker(&_MapMutex);
    if(_deviceMap.contains(mac)) return _deviceMap.value(mac);
    return NULL;
}


QVariant  DeviceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())   return QVariant();

    if (role == Qt::DisplayRole && index.column() == DevCol_Type)
    {   //显示设备型号名称
        int devType = record(index.row()).value(DevCol_Type).toInt();
        return Config::curMfrsCfg().getDevTypeName(DevType(devType));
    }
    else if (role == Qt::DisplayRole && index.column() == DevCol_TypeDesp)
    {   //显示设备类别名称
        int devType = record(index.row()).value(DevCol_Type).toInt();
        return Config::curMfrsCfg().getDevGroupName(DevType(devType));
    }
    else if(role == Qt::DisplayRole && index.column() == DevCol_Status)
    {
        int status = record(index.row()).value(DevCol_Status).toInt();
        return AES70Device::getStatusName(DevStatus(status));
    }
    else if (role == Qt::TextAlignmentRole){
        return (Qt::AlignCenter);
    }
    else if (role == Qt::TextColorRole &&
             record(index.row()).value(DevCol_Status).toInt() == Dev_Online){
        return Config::curSkinStyle()->OnlineTextColor;
    }
    else if (role == Qt::BackgroundRole &&
             !record(index.row()).value(DevCol_Serial).toString().isEmpty()) {
        if(Config::curMfrsCfg().CurMfrs == Mfrs_FangTu)
            return QBrush(Qt::black, Qt::Dense5Pattern);
        else
            return QSqlTableModel::data(index, role);
    }
    else{
        return QSqlTableModel::data(index, role);
    }
}

bool DeviceModel::insertDevice(int position, AES70Device* dev)
{
    beginInsertRows(QModelIndex(), position, position);
    QSqlRecord rowRecord = record();
    rowRecord.setValue(DevCol_Mac,dev->getAES70Mac());
    rowRecord.setValue(DevCol_Ip,dev->getDeviceIp());
    rowRecord.setValue(DevCol_Type,dev->getDeviceType());
    rowRecord.setValue(DevCol_DevName,dev->getDeviceName());
    rowRecord.setValue(DevCol_Status,dev->getStatus());
    rowRecord.setValue(DevCol_Serial, dev->getCurRoomName());
    insertRecord(position,rowRecord);
    submitAll();
    endInsertRows();

    {
        //减少锁定范围
        QMutexLocker locker(&_MapMutex);
        _deviceList.insert(position, dev);
        QString mac = dev->getAES70Mac();
        _deviceMap[mac] =dev;
    }
    return true;
}

bool DeviceModel::updateDevice(AES70Device* danteDev)
{
//    qDebug()<<"updateDevice,type:" << danteDev->getDeviceType() <<"  isOnline:"<< danteDev->isOnline()
//           << " mac:"<< danteDev->getAES70Mac()<<" ip:"<< danteDev->getDeviceIp() << danteDev->getDeviceName() << danteDev->getCurRoomName();

    int rowNum = rowCount();
    for(int i = 0; i<rowNum; i++)
    {
        QString mac = record(i).value(DevCol_Mac).toString();
        if(mac == danteDev->getAES70Mac())
        {
            setData(index(i, DevCol_Ip), danteDev->getDeviceIp());
            setData(index(i, DevCol_Type), danteDev->getDeviceType());
            setData(index(i, DevCol_DevName), danteDev->getDeviceName());
            setData(index(i, DevCol_Status), danteDev->getStatus());
            setData(index(i, DevCol_Serial), danteDev->getCurRoomName());
            submitAll();
            break;
        }
    }
    select();
    return true;
}

bool DeviceModel::replaceDevice(AES70Device* oldDev, AES70Device* newDev)
{
    QMutexLocker locker(&_MapMutex);
    for(int i = 0; i<_deviceList.count(); i++)
    {
        if(_deviceList[i] == oldDev)
        {
            bool rs = updateDevice(newDev);
            _deviceList.replace(i,newDev);
            QString mac = newDev->getAES70Mac();
            _deviceMap[mac] = newDev;
            return rs;
        }
    }

    return false;
}

void DeviceModel::removeDevice(const QModelIndex &index)
{
    QString mac = record(index.row()).value(DevCol_Mac).toString();
    removeRow(index.row()); //删除一行
    submitAll();
    AES70Device* device = getDeviceByMac(mac);
    if(device != NULL)
    {
        QMutexLocker locker(&_MapMutex);
        _deviceList.removeOne(device);
        QString mac = device->getAES70Mac();
        _deviceMap.remove(mac);
    }
    select();
    sort(DevCol_Ip, Qt::DescendingOrder);
}

