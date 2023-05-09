#include "firmwareudpserver.h"
#include "config.h"
#include "Protocal/packetbase.h"

#define Debug_Recv_Send 0 //显示输入输出信息

FirmwareUdpServer::FirmwareUdpServer(const QString &ipAaddres, quint16 port)
{
    _hostAddr = QHostAddress(ipAaddres);
    _bindPort = port;
    _ownerThread = new QThread;
    this->moveToThread(_ownerThread);
    connect(_ownerThread, SIGNAL(started()), this, SLOT(initServer()));
    connect(_ownerThread, SIGNAL(finished()), this, SLOT(stopServer()));
    _ownerThread->start();
    QThread::msleep(50);
}

FirmwareUdpServer::~FirmwareUdpServer()
{
    _ownerThread->exit(0);
    _ownerThread->wait(500);
    delete _ownerThread;
}

void FirmwareUdpServer::initServer()
{
    _udpSocket = new QUdpSocket();

    bool isBind = _udpSocket->bind(_hostAddr, _bindPort, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    _udpSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
    bool isJoinGroup = _udpSocket->joinMulticastGroup(QHostAddress(UPDATE_ONLINE_IP), Config::instance()->getCurItr());
    qDebug() << "udp Bind: " << isBind << "udp joinMulticastGroup: " << isJoinGroup;
    connect(_udpSocket, SIGNAL(readyRead()), this, SLOT(packHandler()));

}

void FirmwareUdpServer::stopServer()
{
    delete _udpSocket;
}

bool FirmwareUdpServer::sendPacket(QString distIp, Update_Online_Udp_t& pack)
{
    QByteArray packBuff((const char*)&pack, sizeof(pack));

#if Debug_Recv_Send
    int msgType = pack.h.type;//调试:发送数据包
    QString curTimeStr = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    qDebug()<< QString("%1 @sendPacket(%2) to %3:").arg(curTimeStr).arg(msgType, 0, 16).arg(distIp)
            << "ThreadId:" << QThread::currentThread(); // <<array.toHex();
#endif
    int cnt = _udpSocket->writeDatagram(packBuff, QHostAddress(distIp), PacketBase::DEVICE_UDP_PORT1);
    bool rs = (cnt == packBuff.length());
    return rs;
}

bool FirmwareUdpServer::sendPacketSync(QString distIp, Update_Online_Udp_t& pack, int waitMs, quint32 msgTypeWait)
{    
    QByteArray packBuff((const char*)&pack, sizeof(pack));

#if Debug_Recv_Send
    int msgType = pack.h.type;//调试:发送数据包
    QString curTimeStr = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    qDebug()<< QString("%1 @sendPacketAync(%2) to %3:").arg(curTimeStr).arg(msgType, 0, 16).arg(distIp)
            << "ThreadId:" << QThread::currentThread() << packBuff.length() << pack.h.checksum;
#endif
    QMutexLocker locker(&_Mutex);//加互斥锁。
    _msgTypeWait = msgTypeWait;
    _distIpWait = distIp;
    int cnt = _udpSocket->writeDatagram(packBuff, QHostAddress(distIp), PacketBase::DEVICE_UDP_PORT1);
    if(cnt == packBuff.length()){
        bool rs = _syncCondition.wait(&_Mutex, waitMs);
        _msgTypeWait = 0;
        _distIpWait = "";
        return rs;
    }
    else{
        return false;
    }
}

//处理设备Udp包
void FirmwareUdpServer::packHandler()
{
    while (_udpSocket->hasPendingDatagrams())
    {
        QByteArray packetArray;
        packetArray.resize(_udpSocket->pendingDatagramSize());
        QHostAddress recvIp;
        quint16 senderPort;
        _udpSocket->readDatagram(packetArray.data(), packetArray.size(), &recvIp, &senderPort);
        memset(&_recvPack, 0, sizeof(_recvPack));
        memcpy(&_recvPack, packetArray.data(), packetArray.size());

        unsigned int checksum = cal_checksum((unsigned char *)packetArray.data(), packetArray.size());
        if (_recvPack.h.dev_type == FirmDev_Type_PC || checksum != _recvPack.h.checksum) {
            qDebug() << "dev_type" << _recvPack.h.dev_type
                     << "recv checksum" << _recvPack.h.checksum << "cal checksum" << checksum;
            continue;
        }

        quint32 msgType = _recvPack.h.type; //调试：接收数据包
#if Debug_Recv_Send

        QString curTimeStr = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
        qDebug()<< QString("%1 @recvPacket(%2) from %3:").arg(curTimeStr). arg(msgType, 0, 16).arg(recvIp.toString())
                << "ThreadId:" << QThread::currentThread();// <<packetBuff.toHex();
        qDebug() <<"Condition: " << _msgTypeWait<< msgType << _distIpWait << recvIp.toString();

#endif

        if(_msgTypeWait == msgType &&
                recvIp == QHostAddress(_distIpWait))
        {
            _Mutex.lock();
            _syncCondition.wakeAll();
            _Mutex.unlock();
        }
    }
}


bool FirmwareUdpServer::getDevInfo(const QString &distIp, Update_Online_Udp_t& recvPack)
{
    QByteArray nonBody;
    Update_Online_Udp_t packGetIp = createDatagram(Type_SearchDev, nonBody);
    bool rs = sendPacketSync(distIp, packGetIp, PacketBase::UDP_WAIT_MS, Type_SearchDev_Ack);
    if(rs){
        recvPack = _recvPack;
    }
    return rs;
}


bool FirmwareUdpServer::setIpInfo(const QString &distIp, setIP_t ipNew)
{
    QByteArray bodyBuff((const char*)&ipNew, sizeof(setIP_t));
    Update_Online_Udp_t packSetIp = createDatagram(Type_SetIP, bodyBuff);

    return sendPacketSync(distIp, packSetIp, PacketBase::UDP_WAIT_MS, Type_SetIP_Ack);
}

bool FirmwareUdpServer::sendFirmUpdateBlock(const QString &distIp, firmSecion_t blkBody)
{
    QByteArray bodyBuff((const char*)&blkBody, sizeof(blkBody));
    Update_Online_Udp_t packSetIp = createDatagram(Type_UpdateOnline, bodyBuff);

    bool rs = sendPacketSync(distIp, packSetIp, PacketBase::UDP_WAIT_MS, Type_UpdateOnline_Ack);
    return rs;
}

bool FirmwareUdpServer::renameDevice(const QString &distIp, const QString& newName)
{
    setDeviceName_t buff;
    memset(&buff, 0, sizeof(setDeviceName_t));
    std::string strName = newName.toStdString();
    strncpy_s(buff.name, strName.c_str(), newName.size());

    QByteArray bodyBuff((const char *)&buff, sizeof(setDeviceName_t));
    Update_Online_Udp_t packSetDevName = createDatagram(Type_Set_DeviceName, bodyBuff);

    bool rs = sendPacketSync(distIp, packSetDevName, PacketBase::UDP_WAIT_MS, Type_Set_DeviceName_Ack);
    return rs;
}

QByteArray FirmwareUdpServer::parserMacStr(QString macStr)
{
    quint8 macHex[6] = {0};
    unsigned int offset2 = 4;
    std::string str = macStr.toStdString();
    const char *pMacChar =  str.c_str();
    for (int i = 0; i < 12; ++i)
    {
        unsigned char c0 = pMacChar[i];
        if (48 <= c0 && c0 < 58) {  //0~9
            c0 -= 0x30;
            macHex[i>>1] |= (c0 << offset2);
        }
        else if (65 <= c0 && c0 < 71) { //A~F
            c0 -= 0x37; //65 -10
            macHex[i>>1] |= (c0 << offset2);
        }
        else if (97 <= c0 && c0 < 103) { //a~f
            c0 -= 0x57; //97 -10
            macHex[i>>1] |= (c0 << offset2);
        }
        if (offset2 == 0) {
            offset2 = 4;
        } else {
            offset2 = 0;
        }
    }
    return QByteArray((const char*)macHex, 6);
}

Update_Online_Udp_t FirmwareUdpServer::createDatagram(FirmMsgType type, QByteArray& bodyBuff)
{
    Update_Online_Udp_t pack;
    int data_len = sizeof(Update_Online_Udp_t);
    memset(&pack, 0, sizeof(pack));
    memcpy(pack.h.sync, "FTCN", 4);
    pack.h.dev_type = FirmDev_Type_PC;
    pack.h.type     = type;
    memcpy(&pack.b, bodyBuff, bodyBuff.length());
    pack.h.checksum = cal_checksum((unsigned char *)&pack, data_len);
    return pack;
}

unsigned int FirmwareUdpServer::cal_checksum(unsigned char *pbuf, unsigned int size)
{
    unsigned int checksum = 0;
    int headSize = sizeof(Update_Online_Udp_Header_t);
    for (int i = 0; i < headSize - 4; i++)
    {
        checksum += pbuf[i];
    }
    //跳过checksum对应的字节
    for(uint i = headSize; i<size; i++ ){
        checksum += pbuf[i];
    }
    return checksum;
}
