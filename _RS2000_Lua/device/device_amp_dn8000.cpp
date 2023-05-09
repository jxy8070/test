#include "stable.h"
#include "Device_Amp_DN8000.h"
#include "protocal/packetbase.h"
#include "protocal/packet.h"
#include "device/devicemanager.h"
#include "deviceSetting/alarmconfig.h"
#include "Frame/mainwindow.h"

Device_Amp_DN8000::Device_Amp_DN8000(QString mac, QString devName, DevType devType, QString devIp, QString creator)
    :AES70Device(mac, devName, devType, devIp, creator)
{
    _alarmCfg = new AlarmConfig(devName, mac);
    _factoryMode = false;
    _saveState = DevSave_None;
    memset(_chState, 0, sizeof(_chState));
    _tcpClient = new QTcpSocket(this);
    connect(_tcpClient,&QTcpSocket::readyRead, this, &Device_Amp_DN8000::onReceiveTcpPacket);
}

// 另存场景为文件
void Device_Amp_DN8000::serialize(QString fileName)
{
//    QFile f(fileName);
//    f.open(QIODevice::WriteOnly);
//    QDataStream ds(&f);
//    ds << _configData;
//    f.close();
}

void Device_Amp_DN8000::deserialize(QString fileName)
{
//    QFile file(fileName);
//    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
//    QDataStream ds(&file);
//    ConfigData_Amp4_4 configData;
//    ds >> configData;
//    file.close();
}

void setRs232CheckSum(quint8* pData)
{
    quint8 checkSum = 0;
    quint8 dataLen = pData[1];
    for(int i = 1; i <= dataLen; i++) {
        checkSum += pData[i];
    }
    pData[dataLen+1] = checkSum;
}

void Device_Amp_DN8000::sendBlinkLedCmd()
{
    sendPacket(COMMAND_LED_BLINK, QByteArray());
}

void Device_Amp_DN8000::sendGetVersionCmd()
{
    sendPacket(COMMAND_GET_VERSION, QByteArray()); //获取AEC版本
    quint8 rsData[16] = {0x55, 0x05, 0x60, 0x00, 0x02, 0x00, 0x67};
    setRs232CheckSum(rsData);
    sendPacket(COMMAND_TRANSFER_DATA_FROM_PC, QByteArray((const char*)rsData, 16)); //获取透传设备版本
    QThread::msleep(20);
}

void Device_Amp_DN8000::sendGetConnectStateCmd()
{
    sendPacket(COMMAND_GET_CONNECT_STATE, QByteArray());
}

void Device_Amp_DN8000::sendGetBoardStateCmd()
{
    sendPacket(COMMAND_GET_BOARD_STATE, QByteArray());
}

void Device_Amp_DN8000::sendSetBoardStateCmd()
{
    sendPacket(COMMAND_GET_BOARD_STATE, QByteArray());
}

void Device_Amp_DN8000::sendGetChannelStateCmd()
{
    sendPacket(COMMAND_GET_CHANNEL_STATE, QByteArray());
}



void Device_Amp_DN8000::setChannelMuteState(quint8 ch, bool mute)
{
    _chState[ch-1].isMute = mute;
    quint8 rsData[16] = {0x55, 0x05, 0x10, 0x00, 0x03, 0x00, 0x00};
    rsData[3] = ch;
    rsData[5] = mute ? 0x02 : 0x01;
    setRs232CheckSum(rsData);
    sendPacket(COMMAND_TRANSFER_DATA_FROM_PC, QByteArray((const char*)rsData, 16));    
    QThread::msleep(50);
}

void Device_Amp_DN8000::getChannelMuteState(quint8 ch)
{
    quint8 rsData[16] = {0x55, 0x05, 0x10, 0x00, 0x02, 0x00, 0x00};
    rsData[3] = ch;
    setRs232CheckSum(rsData);
    sendPacket(COMMAND_TRANSFER_DATA_FROM_PC, QByteArray((const char*)rsData, 16));
    QThread::msleep(50);
}

void Device_Amp_DN8000::setStandbyState(bool standby)
{
    _isAmpStandby = standby;
    quint8 rsData[16] = {0x55, 0x05, 0x11, 0x01, 0x01, 0x00, 0x00};
    rsData[5] = standby ? 0x01 : 0x02;
    setRs232CheckSum(rsData);
    sendPacket(COMMAND_TRANSFER_DATA_FROM_PC, QByteArray((const char*)rsData, 16));
    QThread::msleep(50);
}

void Device_Amp_DN8000::getStandbyState()
{
    quint8 rsData[16] = {0x55, 0x05, 0x11, 0x00, 0x02, 0x00, 0x18};
    setRs232CheckSum(rsData);
    sendPacket(COMMAND_TRANSFER_DATA_FROM_PC, QByteArray((const char*)rsData, 16));
    QThread::msleep(50);
}

void Device_Amp_DN8000::setChannelProtect(quint8 ch, bool protect)
{
    _chState[ch-1].protect = protect;
    quint8 rsData[16] = {0x55, 0x05, 0x40, 0x00, 0x03, 0x00, 0x00};
    rsData[3] = ch;
    rsData[5] = protect ? 0x01 : 0x02;
    setRs232CheckSum(rsData);
    sendPacket(COMMAND_TRANSFER_DATA_FROM_PC, QByteArray((const char*)rsData, 16));
    QThread::msleep(50);
}

void Device_Amp_DN8000::processPacket(PacketBase* pack)
{
    switch(pack->getMsgType()) {
    case Msg_Type_HeartbeatAck:
        _saveState = DevSave_None;
        break;
    case Msg_Type_GetInitAck:
        if(isAddedToProject()) {
            emit initConfigFromDevice();
        }
        switchStatus(Dev_Online);
        break;
    case Msg_Type_iMXRT1052_POE_Save_Ack:
        MainWindow::instance()->showNotice(tr("Save Success!"));
        break;
    default:
        qDebug()<< QString("Unknown packet type%1").arg(pack->getMsgType());
        break;
    }
}


QString getHexString(QByteArray buff)
{
    QString hex;
    for(char ch : buff){
        hex.append(QString("%1 ").arg((quint8)ch, 2, 16, QChar('0')));
    }
    return hex;
}

void Device_Amp_DN8000::onReceiveTcpPacket()
{
    if(_tcpClient->state() != QAbstractSocket::ConnectedState || _tcpClient->bytesAvailable() < 0)
        return;
    if(_tcpClient->bytesAvailable() < 0)
    {
        _tcpClient->disconnectFromHost();
        _tcpClient->waitForDisconnected(3000);
    }
    while (_tcpClient->bytesAvailable() >= sizeof(PackcetHeader))
    {
        PackcetHeader head;
        int cnt = _tcpClient->read((char*)&head, sizeof(PackcetHeader));
        if(cnt != sizeof(PackcetHeader) || head.Sync != 0x3B || head.Version != 0x01)  {
            qDebug()<<"!!Packet Header error, skip data!";
            _tcpClient->readAll();
            return;
        }
        QByteArray packBuff = _tcpClient->read( head.DataLen);
        if(packBuff.length() != head.DataLen){
            qDebug()<<"!!Packet data length error!";
            return;
        }
        unsigned short checkSum = 0;
        for(char ch : packBuff)  checkSum+= (quint8)ch;
        if(head.DataCheckSum != checkSum){
            qDebug()<<"!!Packet data checksum error!";
            return;
        }

        if(head.Command >= COMMAND_ALIVE){
            if((quint8)packBuff[0] == 202 &&  (quint8)packBuff[1] == 1 &&
                    (quint8)packBuff[2] == 168 &&(quint8)packBuff[3] == 192){
                packBuff = packBuff.right(packBuff.length() -4);
            }
            else{
                qDebug() << "!!Packet data error: not start with Ip" <<getHexString(packBuff);
            }
        }

        if(head.Command == 0x800B){
            rs232PacketHandler(head.Command, packBuff);
        }
        else{
            tcpPacketHandler(head.Command, packBuff);
        }
    }
}

void Device_Amp_DN8000::rs232PacketHandler(int command, QByteArray& pData)
{
    int dataLen = (quint8)pData[1];
    quint8 code = (quint8)pData[2];
    quint8 ch = (quint8)pData[3];
    quint8 checkSum = 0;
    for(int i = 2; i< pData.length()-1; i++)  checkSum+= (quint8)pData[i];
    if((quint8)pData[0] != 0X55 && (quint8)pData[dataLen +1] != checkSum
            && (ch <1 || ch > ChCount) ) {
        qDebug()<<"!!Packet(rs232) head、ch or checkSum error!" << getHexString(pData);
        return;
    }

    switch (code) {
    case 0x10: //通道静音状态
        if((quint8)pData[5] == 0x02){
            _chState[ch-1].isMute = true;
        }
        else if((quint8)pData[5] == 0x01){
            _chState[ch-1].isMute = false;
        }
//        qDebug()<< "&&Channel mute:" << dataLen << getHexString(pData);
        break;
    case 0x11: //待机状态
        if((quint8) pData[5] != 0xaa){
            _isAmpStandby = ((quint8) pData[5] == 0x01);
            emit amplifierParamChanged();
        }
        break;
    case 0x23: //温度Temp
        _chState[0].temprate = (quint8) pData[5];
        _chState[1].temprate = (quint8) pData[5];
        _chState[2].temprate = (quint8) pData[6];
        _chState[3].temprate = (quint8) pData[6];
        _chState[4].temprate = (quint8) pData[7];
        _chState[5].temprate = (quint8) pData[7];
        _chState[6].temprate = (quint8) pData[8];
        _chState[7].temprate = (quint8) pData[8];
        break;
    case 0x26:  //压限Clip
        if(dataLen == 5){
            for(int i = 0; i< ChCount; i++){
                _chState[i].isClip = false;
            }
        }
        else{
            for(int i = 0; i< ChCount; i++){
                _chState[i].isClip = ((quint8)pData[6+i] == 0x01);
            }
        }
        break;
    case 0x2A:  //1~4通道电压
        _chState[0].valtage = (quint8)pData[5]*2.56 + (quint8)pData[6]*0.01;
        _chState[1].valtage = (quint8)pData[7]*2.56 + (quint8)pData[8]*0.01;
        _chState[2].valtage = (quint8)pData[9]*2.56 + (quint8)pData[10]*0.01;
        _chState[3].valtage = (quint8)pData[11]*2.56 + (quint8)pData[12]*0.01;
        _chState[0].hasSignel = _chState[0].valtage > 1.20;
        _chState[1].hasSignel = _chState[1].valtage > 1.20;
        _chState[2].hasSignel = _chState[2].valtage > 1.20;
        _chState[3].hasSignel = _chState[3].valtage > 1.20;
        break;
    case 0x2B:  //5~8通道电压
        _chState[4].valtage = (quint8)pData[5]*2.56 + (quint8)pData[6]*0.01;
        _chState[5].valtage = (quint8)pData[7]*2.56 + (quint8)pData[8]*0.01;
        _chState[6].valtage = (quint8)pData[9]*2.56 + (quint8)pData[10]*0.01;
        _chState[7].valtage = (quint8)pData[11]*2.56 + (quint8)pData[12]*0.01;
        _chState[4].hasSignel = _chState[4].valtage > 1.20;
        _chState[5].hasSignel = _chState[5].valtage > 1.20;
        _chState[6].hasSignel = _chState[6].valtage > 1.20;
        _chState[7].hasSignel = _chState[7].valtage > 1.20;
        break;
    case 0x2C:  //1~4通道电流
        _chState[0].electricity = (quint8)pData[5]*2.56 + (quint8)pData[6]*0.01;
        _chState[1].electricity = (quint8)pData[7]*2.56 + (quint8)pData[8]*0.01;
        _chState[2].electricity = (quint8)pData[9]*2.56 + (quint8)pData[10]*0.01;
        _chState[3].electricity = (quint8)pData[11]*2.56 + (quint8)pData[12]*0.01;
        break;
    case 0x2D:  //5~8通道电流
        _chState[4].electricity = (quint8)pData[5]*2.56 + (quint8)pData[6]*0.01;
        _chState[5].electricity = (quint8)pData[7]*2.56 + (quint8)pData[8]*0.01;
        _chState[6].electricity = (quint8)pData[9]*2.56 + (quint8)pData[10]*0.01;
        _chState[7].electricity = (quint8)pData[11]*2.56 + (quint8)pData[12]*0.01;
        emit channelStateChanged(_chState);
        break;
    case 0x30:  //AES70闪烁回复包，忽略
        if((quint8)pData[5] != 0xAA){
            qDebug()<< "!!Unknown Packet(rs232) Code:" <<QString("%1").arg(code, 2, 16, QChar('0'))
                    << dataLen << getHexString(pData);
        }
        break;
    case 0x40:  //通道保护
        if((quint8)pData[5] == 0x01){
            _chState[ch-1].protect = true;
        }
        else if((quint8)pData[5] == 0x02){
            _chState[ch-1].protect = false;
        }
//        qDebug()<< "&&Channel protect:" << dataLen << getHexString(pData);
        break;
    case 0x60:  //Amp版本
        _ampVersion = QString("%1%2%3%4%4").arg((quint8)pData[5]).arg((quint8)pData[6])
                .arg((quint8)pData[7]).arg((quint8)pData[8]).arg((quint8)pData[9]);
        break;
    default:
        qDebug()<< "!!Unknown Packet(rs232) Code:" <<QString("%1").arg(code, 2, 16, QChar('0'))
                << dataLen << getHexString(pData);
    }
}

void Device_Amp_DN8000::tcpPacketHandler(int command, QByteArray& packData)
{
    switch (command) {
    case 0x0001:
    {
        version_t verInfo;
        memcpy(&verInfo, packData.data(), sizeof(version_t));
        _version = QString("%1.%2(%3-%4-%5)").arg(verInfo.Major).arg(verInfo.Minor)
                .arg(verInfo.Year).arg(verInfo.Month).arg(verInfo.Day);
        emit amplifierParamChanged();
        break;
    }
    case 0x000B:    //透传的回复包，忽略
        break;
    case 0x000c:    //闪烁AES67指示灯回复包
        break;
    case 0x8002:    //alive包,忽略
        break;
    case 0x8008:
        memcpy(&_ptpState, packData.data(), sizeof(PTP_State));
        emit amplifierParamChanged();
        break;
    case 0x800A:    //有没有AES流，而不是信号
//        for(int i = 0; i< ChCount; i++){
//            _chState[i].hasStream = ((quint8)packData[i] != 0x00);
//        }
//        emit channelStateChanged(_chState);
        break;
    default:
        qDebug()<< "!!Unknown Packet:" <<QString("%1").arg(command, 4, 16, QChar('0'))
                << packData.length() << getHexString(packData);
    }
}

void Device_Amp_DN8000::sendPacket(int command, QByteArray packBuff)
{
    if(!isOnline()) return;
    if( _tcpClient->bytesAvailable() < 0){
        _tcpClient->disconnectFromHost();
        _tcpClient->waitForDisconnected(3000);
    }
    if(_tcpClient->state() != QAbstractSocket::ConnectedState){
        _tcpClient->connectToHost(getDeviceIp(), 49529);
        _tcpClient->waitForConnected(5000);
    }
    if(_tcpClient->state() == QAbstractSocket::ConnectedState)
    {
        PackcetHeader head;
        head.Sync = 0x3B;
        head.Version = 0x01;
        head.Command = (unsigned short)command;
        head.DataLen = (unsigned short)packBuff.length();
        unsigned short checkSum = 0;
        for(char ch : packBuff)  checkSum+= (quint8)ch;
        head.DataCheckSum = checkSum;

        qint64 countSend = _tcpClient->write((const char*)&head, sizeof(PackcetHeader));
        countSend += _tcpClient->write(packBuff.data(), packBuff.length());
        _tcpClient->flush();
    }
}



