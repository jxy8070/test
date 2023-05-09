#ifndef FIRMWAREUDPSERVER_H
#define FIRMWAREUDPSERVER_H

#include <QUdpSocket>
#include <QWaitCondition>
#include "firmwarePacket.h"

class AES70Device;
class FirmwareUdpServer : public QObject
{
    Q_OBJECT

public:
    FirmwareUdpServer(const QString &distIp, quint16 port = 0);    
    ~FirmwareUdpServer();
    bool getDevInfo(const QString &distIp, Update_Online_Udp_t& recvPack);
    bool setIpInfo(const QString &distIp, setIP_t ipNew);
    bool sendFirmUpdateBlock(const QString &distIp, firmSecion_t blkBody);
    bool renameDevice(const QString &distIp, const QString& newName);
    static QByteArray parserMacStr(QString macStr);
private:
    bool sendPacket(QString distIp, Update_Online_Udp_t& pack );
    bool sendPacketSync(QString distIp, Update_Online_Udp_t& pack, int waitMs, quint32 msgType);
    Update_Online_Udp_t createDatagram(FirmMsgType type, QByteArray& bodyBuff);
    unsigned int cal_checksum(unsigned char *pbuf, unsigned int size);

signals:

protected slots:
    void initServer();
    void stopServer();
    void packHandler();

private:
    QMutex _Mutex;
    Update_Online_Udp_t _recvPack;
    QUdpSocket* _udpSocket;
    QHostAddress _hostAddr;
    quint16     _bindPort;
    QThread*    _ownerThread;
    quint32     _msgTypeWait = 0;
    QString     _distIpWait;
    QWaitCondition _syncCondition;
};


#endif // FIRMWAREUDPSERVER_H
