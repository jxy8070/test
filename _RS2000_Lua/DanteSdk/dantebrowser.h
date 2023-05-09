#ifndef DANTEBROWSER_H
#define DANTEBROWSER_H

//===================================================================================================
// 注意：由于Dante API定义了Socket 数组的大小和Windows SDK默认的大小不同，为了使用Dante API 必须在
//       程序的最前端，即stafx.h中预定义FD_SETSIZE的大小，否则会出现内存错误
//#define FD_SETSIZE 1024			//为了满足Dante的platfor_specific.h的定义要求，xiaoxi,2014-04-27//
//====================================================================================================
#include <QThread>
#include <QMap>
#include "audinate/dante_api.h"

class AES70Device;

//==========================================================================================================
// 说明：在DanteBrowser初始化时，
//      Open()可以在最多两个指定的接口上发现设备。网络接口为操作系统接口编号
//		PrimaryNetIndex    ：指定一个用来发现设备的网络接口, 0 表示在系统所有可用的Dante接口上发现设备。
//      slaverNeteIndex  ：指定另外一个用来发现设备的网络接口, 0 表示在系统所有可用的Dante接口上发现设备。
//     rediscoverDevice()函数，该函数使网络上的所有设备重新声明，从而重新扫描网络上丢失的设备。
class  DanteBrowser: public QThread
{
    Q_OBJECT

    class TempDevice
    {
    public:
        QString _name;
        QString _mac;
        QString _ip;
        QString _deviceDesp;
    };
    enum{
        MAX_DEVICE_HANDLERS =8,
        MAX_RESOLVES	= 512,
        MAX_SOCKETS		= (MAX_RESOLVES+2) // need space for device and channel browsing
    };

    enum{
        RESTART_BROWSER_PERIOD = 0,	//自动重启动浏览器周期（秒），0表示不自动
        CONFIRM_DEVICE_PERIOD = 3,  //确认设备在线的时间间隔
        REDISCOVER_DEVICE_PERIOD =60//
    };


public:
    explicit DanteBrowser(QObject *parent = 0);
    ~DanteBrowser(void);


protected:
    void run();

public:
    void open();
    void close();
    void resetBrowser(void);
    bool cacheSockets(void);
    void removeDevice(QString name);

signals:
    void deviceRemoved(QString name);
    void deviceAdded(QString mac, QString ip, QString name, QString deviceDesp);
public slots:


private:  //回调函数 监视浏览器是否有更新设备，当浏览器发现设备状态更新时，立刻同步设备中心的设备
    static void OnSocketsChangedCallback(const db_browse_t * browse);
    static void OnNetworkChangedCallback(const db_browse_t * browse);
    static void OnNodeChangedCallback(db_browse_t * browse, const db_node_t * node, db_node_change_t node_change);
    static void OnDeviceChangedCallback(dr_device_t * device, dr_device_change_flags_t change_flags);

private:
    void handleNodeChanged(const db_node_t *node, db_node_change_t node_change);
    void tempDeviceRemoved(QString name);
    void tempDeviceAdd(QString name, QString mac, QString deviceDesp);
    void tempDeviceUpdate(QString name, QString ip);
    bool resetParams(void);
    void confirmDevice(void);       //确认设备是否离线
    void printAudInfo(const char *errorInfo, aud_error_t result =AUD_SUCCESS );
    QString getChangeTypeDesc(db_node_change_t changeType);

private:
    //暂时用单网卡
//    unsigned long _primaryNetIndex;
//    unsigned long _slaverNetIndex;
    QMap<QString, TempDevice*> _tempDeviceMap;
    aud_env_t *_danteEnv;
    dr_devices_t *_localDevices;
    db_browse_t *_browser;
    db_browse_types_t _browseTypes;
    dante_sockets_t		_cached_sockets;

    aud_utime_t         _nextConfirmTime;
    aud_utime_t         _nextRediscoverTime;
    aud_utime_t         _nestRestartTime;
    aud_utime_t         _selectTimout; //select()超时时间
};
#endif // DANTEBROWSER_H
