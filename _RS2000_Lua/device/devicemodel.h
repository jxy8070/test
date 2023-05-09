#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QSqlTableModel>
#include <QList>
#include <QReadWriteLock>

class AES70Device;
class DeviceManager;

enum DevColumn{ //和数据库device表列顺序一致
    DevCol_Mac= 0,
    DevCol_TypeDesp = 1,
    DevCol_Type = 2,
    DevCol_Ip = 3,
    DevCol_DevName = 4,
    DevCol_Status = 5,
    DevCol_Serial = 6,
    DevCol_PortInfo = 7,
};

class  DeviceModel : public QSqlTableModel
{
    Q_OBJECT

public:
    DeviceModel(DeviceManager *deviceManager);
    QList<AES70Device*> getDeviceList();
    QList<AES70Device*> getDeviceListByType(DevType type = Dev_Type_Unknow, bool onlineOnly = false);
    AES70Device* getDeviceByMac(QString mac);
    bool insertDevice(int position, AES70Device* device);
    void removeDevice(const QModelIndex &index);
    bool updateDevice(AES70Device* danteDev);
    bool replaceDevice(AES70Device* oldDev, AES70Device* newDev);
    void deleteOfflineDevice(void);
protected:
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

private:
    void clearStatus();
    void initDeviceList();
private:
    DeviceManager *_devManager;
    QList<AES70Device*> _deviceList;
    QMutex _MapMutex;
    QMap<QString, AES70Device*> _deviceMap;
};

#endif // DEVICEMODEL_H
