#ifndef DANTEDEVICE_H
#define DANTEDEVICE_H

#include <QObject>

#include "common.h"
#include "config.h"
#include "deviceSetting/alarmconfig.h"

class QTimer;
class PacketBase;
enum DevStatus
{
    Dev_Offline = 0, /* 设备离线 */
    Dev_DanteFound,  /* DanteBrower发现设备名称、Mac地址、IP地址后的状态。 */
    Dev_AES70Found,  /* AES70发现设备名称、Mac地址、IP地址后的状态 */
    Dev_Initialized, /* 广播包回应设备类型后的状态 */
    Dev_Online,      /* 设备响应0x80数据帧后的状态，设备可用 */
};

class AES70Device : public QObject
{
    Q_OBJECT
    enum {MONITOR_PERIOD = 200,};
public:
    enum DevSaveState
    {
        DevSave_None = 0,
        DevSave_Save2Dev,
        DevSave_LocalLoad,
    };

public:
    static AES70Device* createDevice(QString mac, QString devName, DevType devType, QString ip, QString devCreator);
    AES70Device(QString mac, QString devName, DevType type, QString ip, QString devDisp);
    void setUpdateTime();
    void updateAES70Info(QString devName, QString devIp, QString creator);
    bool sendPacket_Nobody(MsgType msgType);
    bool sendPacket(MsgType msgType, QByteArray& msgData);
    bool sendPacketSync_Nobody(MsgType msgType, int waitMs);
    bool sendPacketSync(MsgType msgType, QByteArray& msgData, int waitMs);
    bool switchStatus(DevStatus newStatus);
    void confirmOnline(int confirmSecond);
    QString getMfrsName(MfrsType mfrsType = _mfrsType);

    PlatformType getPlatformType();
    bool is16BitProtocal();
    int maxConfigLayer();
    static QString getStatusName(DevStatus type);

public://virtual
    virtual void setOffline();
    virtual QString getDeviceIp() const;
    virtual void setDeviceIp(QString ipAddress);
    virtual void processPacket(PacketBase* pack);
    virtual void sendInitPack();
    virtual void sendHeartbeatPack();
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);
    virtual void saveAlarmSetting();
    virtual void loadAlarmSetting();
    virtual void clearAlarmSetting();

public://inline
    QString getAES70Mac() const { return _AES70Mac; }
    DevType getDeviceType() const { return _deviceType;}
    QString getDevCreator() const { return _devCreator; }
    QString getDeviceName() const { return _deviceName; }
    void setDeviceName(QString newName, bool setToDev);
    quint32 getMsgIdx() { _msgIdx = (_msgIdx + 1) % 65534; return _msgIdx;} //不使用65535,该值被定义为特殊值
    QDateTime lastUpdateTime() {return _lastUpdateTime;}
    DevStatus getStatus() {return _status;}
    bool isOnline() {return _status == Dev_Online;}
    QString getDanteIP(void) { return _danteIP; }
    void setDanteIP(QString IP) { _danteIP = IP; }
    QString getDanteName(void) { return _danteName; }
    void setDanteName(QString name) { _danteName = name; }
    void setMfrsType(MfrsType mfrs) {_mfrsType = mfrs;}
    bool isAddedToProject() {return _isAddedToProj;}
    void addedToProject(bool isAdded) {_isAddedToProj = isAdded;}
    QString getCurRoomName() { return _curRoomName; }
    void setCurRoomName(QString roomName);

signals:
    void deviceStatusChanged(DevStatus devStatus);
    void sendDSPVersion(QString);
    void deviceNameChanged(const QString& name);

public: //非处理器设备的函数
    AlarmConfig *getAlarmCfg() {return _alarmCfg;}
    bool isFactoryMode() { return _factoryMode; }
    void changeFactoryMode() { _factoryMode = !_factoryMode; }

public: //暂时未用
    QList<ChannelInfoStruct> PhyInputPorts; //物理输入通道信息
    QList<ChannelInfoStruct> PhyOutputPorts;//物理输出通道信息

protected:
    static MfrsType _mfrsType;      //所属产商
    QString _AES70Mac;                      //设备MAC地址
    DevType _deviceType;                    //设备类型
    QString _deviceName;                    //设备名称
    bool _isAddedToProj;                    //是否添加到项目中
    QString _devCreator;                    //设备发现者：AES70 Dante Sqlite
    QString _AES70IP;
    quint32 _msgIdx;
    DevStatus _status;                      //设备的状态
    QDateTime _lastUpdateTime;

    QString _danteIP;
    QString _danteName;

    QString _curRoomName = QString();

protected: //非处理器设备的成员
    AlarmConfig *_alarmCfg = NULL;
    bool _factoryMode;
    DevSaveState _saveState;

};

#endif // DANTEDEVICE_H
