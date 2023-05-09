#ifndef UDPSOCKETSERVER_H
#define UDPSOCKETSERVER_H

#include <QUdpSocket>
#include <QWaitCondition>

class PacketBase;
class AES70Device;
class UdpSocketServer : public QObject
{
    Q_OBJECT

public:
    UdpSocketServer(const QString &ipAaddres, quint16 port = 0);
    ~UdpSocketServer();

    bool sendPacket(AES70Device *device, PacketBase* pack);
    bool sendPacketSync(AES70Device *device, PacketBase* pack, int waitMs, quint16 msgType = 0xFFFF, quint16 msgIndex = 0xFFFF);

signals:
   void packetReceived(PacketBase* pack);

protected slots:
    void initServer();
    void stopServer();
    void packHandler();

private:
    QMutex _Mutex;
    QUdpSocket* _udpSocket;
    QHostAddress _hostAddr;
    quint16     _bindPort;
    QThread*    _ownerThread;
    quint16     _msgTypeWait = 0;
    quint16     _msgIndexWait = 0;
    QWaitCondition _syncCondition;
};

#endif // UDPSOCKETSERVER_H
