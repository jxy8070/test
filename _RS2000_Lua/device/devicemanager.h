#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QMutex>
#include <QList>
#include "common.h"
#include <QWaitCondition>
#include "Protocal/udpsocketserver.h"

struct DevSocketInfo
{
    QString DevName;
    quint16 TxConnectorNum;
    quint16 RxConnectorNum;
    quint16 MaxPinNum;
    quint16 TxChNum;
    quint16 RxChNum;
};

class AES70Device;
class ProjectScene;
class SolutionScene;
class DanteBrowser;
class QTimer;
class DeviceModel;
class DeviceItem;
class DeviceManager : public QObject
{
    Q_OBJECT
    enum {CONFIRM_PERIOD = 15,      //在线设备确认定时器间隔s
          AES70FINDER_PERIOD = 5};   //AES70设备刷新周期
public:
    AES70Device* getDeviceByMac(QString mac);
    DeviceModel* getDeviceModel() {return _deviceModel;}
    void cleanup();
    QList<AES70Device*> getDevListByType(DevType type, bool onlineOnly = false);
    QList<DevSocketInfo> getDevSocketInfo(QStringList devNameList);  //用于路由下载的设备信息
    bool setDeviceName(AES70Device *device, QString NewName);
    unsigned long ipAddress_StringToBinary(QString ip);
public:
    bool sendPacket(AES70Device *device, PacketBase* pack);
    bool sendPacketSync(AES70Device *device, PacketBase* pack, int waitMs,
                         quint16 msgType = 0xFFFF, quint16 msgIndex = 0xFFFF);
    void downloadProjectRoute(ProjectScene *scene);
    void downloadSolutionRoute(SolutionScene *scene);
    void downloadMulticast(DeviceItem *devItem, quint16 num_slots, quint32 *id);
    DanteBrowser *getDanteBrowser() { return _danteBrowser; }
    void removeofflineAES70Device(QString mac);

public://static
    AES70Device* creartDanteDevice();
signals:
    void deviceListChanged();
    void deviceStateChanged(AES70Device *);

public slots:
    void setDeviceOffline(QString mac);
    void onDeviceAdded(QString mac, QString ip, QString devName, QString devDescription);
    void deviceConfirmHandler();
    void flashAES70DeviceList();
    void broadcastPackHandler();
    void devicePackHandler(PacketBase* pack);
private:
    void sendBroadcastPack();

private:
    QTimer *_AES70DevFindTimer;
    QTimer *_confirmTimer;
    QUdpSocket *_udpBroadcast;
    UdpSocketServer *_udpDevice;
    DanteBrowser *_danteBrowser;
    void *_pAES70Browser;
    QMap<QString, QString> _AES70DeviceNameMap; //<MAC,NAME>
    QMutex _MapMutex;
    DeviceModel* _deviceModel;
    bool _pluggingOutCable;
    QMap<QString, QString> _DanteIpNameMap;

//////////////////////////////////////////////////
/// 实现Singleton模式相关代码
public:
    static DeviceManager *instance();
    static void release();
private:
    DeviceManager(QObject *parent = 0);             //禁止构造函数。
    DeviceManager(const DeviceManager&);            //禁止拷贝构造函数。
    DeviceManager & operator=(const DeviceManager&);//禁止赋值拷贝函数。
    static QMutex _Mutex;                           //Singleton及发送数据包共用
    static DeviceManager* _Instance;                //<使用原子指针,默认初始化为0。
    ~DeviceManager();

private slots:
};
#endif // DEVICEMANAGER_H

