#include "udpsocketserver.h"
#include "packetbase.h"
#include "device/AES70device.h"

#define Debug_Recv_Send 0 //显示输入输出信息
UdpSocketServer::UdpSocketServer(const QString &ipAaddres, quint16 port)
{
    _hostAddr = QHostAddress(ipAaddres);
    _bindPort = port;

    _ownerThread = new QThread;
    this->moveToThread(_ownerThread);
    connect(_ownerThread, SIGNAL(started()), this, SLOT(initServer()));
    connect(_ownerThread, SIGNAL(finished()), this, SLOT(stopServer()));
    _ownerThread->start();
}

UdpSocketServer::~UdpSocketServer()
{
    _ownerThread->exit(0);
    _ownerThread->wait(500);
    delete _ownerThread;
}

void UdpSocketServer::initServer()
{
    _udpSocket = new QUdpSocket();
    _udpSocket->bind(_hostAddr, _bindPort, QUdpSocket::ShareAddress);
    connect(_udpSocket, SIGNAL(readyRead()), this, SLOT(packHandler()));
}

void UdpSocketServer::stopServer()
{
    delete _udpSocket;
}

bool UdpSocketServer::sendPacket(AES70Device *device, PacketBase* pack)
{
    QScopedPointer<PacketBase> packPtr(pack);
    QByteArray packBuff = packPtr->getBuff();
    QString distIp = device->getDeviceIp();
    if (distIp == "0.0.0.0") {
        return false;
    }

#if Debug_Recv_Send
    int msgType = packPtr->getMsgType();//调试:发送数据包
    int msgIdx = packPtr->getMsgIndex();
    if(msgType >= 0x8000){
        QString curTimeStr = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
        qDebug()<< QString("%1 #sendPacket(%2:%3) to %4:").arg(curTimeStr).
                   arg(msgType, 0, 16).arg(msgIdx).arg(device->getDeviceIp())
                << "ThreadId:" << QThread::currentThread(); // <<array.toHex();
    }
#endif
    int cnt = _udpSocket->writeDatagram(packBuff, QHostAddress(distIp), PacketBase::DEVICE_UDP_PORT);
    bool rs = (cnt == packBuff.length());
    return rs;
}

bool UdpSocketServer::sendPacketSync(AES70Device *device, PacketBase* pack, int waitMs, quint16 msgTypeWait, quint16 msgIndexWait)
{
    QScopedPointer<PacketBase> packPtr(pack);
    QByteArray packBuff = packPtr->getBuff();
    QString distIp = device->getDeviceIp();
    if (distIp == "0.0.0.0") {
        return false;
    }
#if Debug_Recv_Send
    int msgType = packPtr->getMsgType();//调试:发送数据包
    int msgIdx = packPtr->getMsgIndex();
    if(msgType >= 0x8000){
        QString curTimeStr = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
        qDebug()<< QString("%1 #sendPacketSync(%2:%3) to %4:").arg(curTimeStr).
                   arg(msgType, 0, 16).arg(msgIdx).arg(device->getDeviceIp())
                << "ThreadId:" << QThread::currentThread(); // <<array.toHex();
    }
#endif
    QMutexLocker locker(&_Mutex);//加互斥锁。
    _msgTypeWait = msgTypeWait;
    _msgIndexWait = msgIndexWait;
    int cnt = _udpSocket->writeDatagram(packBuff,
                                        QHostAddress(distIp), PacketBase::DEVICE_UDP_PORT);
    if(cnt == packBuff.length()){
        bool rs = _syncCondition.wait(&_Mutex, waitMs);
        _msgTypeWait = 0;
        _msgIndexWait = 0;
        return rs;
    }
    else{
        return false;
    }
}

//处理设备Udp包
void UdpSocketServer::packHandler()
{
    while (_udpSocket->hasPendingDatagrams())//如果有数据等待接受
    {
        //接收并解析数据包
        QByteArray packBuff;
        packBuff.resize(_udpSocket->pendingDatagramSize());//用UDP数据报的大小来设置字节数组的大小
        QHostAddress sender;
        quint16 senderPort;
        _udpSocket->readDatagram(packBuff.data(), packBuff.size(), &sender, &senderPort);
        PacketBase* pack = PacketBase::createPacketByBuff(packBuff);
        if(pack == NULL ) continue;
        else if(pack->getDeviceIP() == "0.0.0.0"){
            delete pack;
            continue;
        }

#if Debug_Recv_Send
//        int msgType = pack->getMsgType(); //调试：接收数据包
//        if(msgType >= 0x8000 && msgType != 0x8008 && msgType != 0x8009 && msgType != 0x8016 && msgType != 0x8017)
//        {
//            QString curTimeStr = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
//            qDebug()<< QString("%1 #recvPacket(%2:%3) from %4(len:%5)").arg(curTimeStr).
//                       arg(msgType, 0, 16).arg(pack->getMsgIndex()).arg(pack->getDeviceIP()).arg(packBuff.length())
//                    << "ThreadId:" << QThread::currentThread();
//            qDebug()<< "packBuff: "<<packBuff.toHex();
//            qDebug() <<"Condition: " << _msgTypeWait<< pack->getMsgType() << _msgIndexWait<< pack->getMsgIndex();
//        }
#endif
        if(_msgTypeWait == pack->getMsgType() &&
                (_msgIndexWait == 0xFFFF ||_msgIndexWait == pack->getMsgIndex()))
        {
            _Mutex.lock();
            _syncCondition.wakeAll();
            _Mutex.unlock();
        }
        emit packetReceived(pack);
    }
}

