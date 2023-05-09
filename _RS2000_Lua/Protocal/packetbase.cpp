#include "stable.h"
#include "packetbase.h"
#include "config.h"
//    memset(&m_data, 0, sizeof(SharcConfig));
//    memset(&m_dCommpacket, 0, sizeof(CommPacket));
//    strcpy((char*)&m_dCommpacket.h.sync, "DNTE");
//    m_dCommpacket.h.dev_type = DEV_TYPE_PC;
//    m_dCommpacket.h.msg_id = 0;

/////////////////////////////////////////////////////////////////
/// 静态变量及函数
const QString PacketBase::GROUPCAST_IP = "239.254.50.123";

PacketBase* PacketBase::createPacket_Nobody(MsgType msgType, quint16 msgIndex)
{
    return new PacketBase(Dev_Type_PC, msgType, msgIndex);
}

PacketBase* PacketBase::createPacket_Withbody(MsgType msgType,
                            quint16 msgIndex, QByteArray& body)
{
    return new PacketBase(Dev_Type_PC, msgType, msgIndex, body);
}

PacketBase* PacketBase::createBroadcastPacket(MsgType msgType)
{
    return new PacketBase(Dev_Type_PC, msgType, BROAD_CAST_MSG_INDEX);
}

PacketBase* PacketBase::createPacketByBuff(QByteArray& packBuff)
{
    QString errInfo;
    if(!checkValid(packBuff, errInfo))
    {
        qDebug()<< "error: " <<errInfo;
        return NULL;
    }
    PacketBase* pack = new PacketBase(packBuff);
    return pack;
}


////////////////////////////////////////////////////////////////
///
PacketBase::PacketBase(DevType devType, MsgType msgType, quint32 msgIndex)
{
    memset(&_head, 0, sizeof(MsgHead));
    strcpy((char*)&_head, "FTCN");
    if(Config::curMfrsCfg().CheckDevMfrs) {
        _head.manufacturer = Config::curMfrsCfg().CurMfrs;
    }
    else{
        _head.manufacturer = Mfrs_Fitcan; //设为Mfrs_Fitcan, 设备不检测Mfrs
    }
    _head.device_type = devType;
    _head.msg_type = msgType;
    _head.msg_id = msgIndex;
    _head.msg_length = MSG_HEAD_LEN;
    setCheckSum();
}

PacketBase::PacketBase(DevType devType, MsgType msgType, quint32 msgIndex, QByteArray& byteArray)
{
    memset(&_head, 0, sizeof(MsgHead));
    strcpy((char*)&_head, "FTCN");
    if(Config::curMfrsCfg().CheckDevMfrs) {
        _head.manufacturer = Config::curMfrsCfg().CurMfrs;
    }
    else{
        _head.manufacturer = Mfrs_Fitcan; //设为Mfrs_Fitcan, 设备不检测Mfrs
    }
    _head.device_type = devType;
    _head.msg_type = msgType;
    _head.msg_id = msgIndex;
    _head.msg_length = byteArray.count();
    _bodyBuff = byteArray;
    setCheckSum();
}

PacketBase::PacketBase(QByteArray& buff)
{
    memset(&_head, 0, sizeof(MsgHead));
    memcpy(&_head, buff.constData(), MSG_HEAD_LEN);
    if(buff.length() > MSG_HEAD_LEN)
    {
        _bodyBuff = buff.mid(MSG_HEAD_LEN);//返回一个字节数组，从数据报头开始，
    }
}

void PacketBase::setCheckSum()
{
    int checkSum = 0;
    const char* bodyPtr = _bodyBuff.constData();//返回一个指向存储在字节数组的数据的指针，指针可用于访问组成数组的字节

    for(int i = 0; i <_head.msg_length - MSG_HEAD_LEN; ++i)
    {
        checkSum += (quint8)bodyPtr[i];
    }
    _head.body_checksum = checkSum;
}

QByteArray PacketBase::getBuff()
{
    setCheckSum();

    QByteArray sendBuff;
    sendBuff.append((const char*)&_head, MSG_HEAD_LEN);
    sendBuff.append(_bodyBuff);
    return sendBuff;
}

QString PacketBase::getDeviceIP()
{
    return QString("%1.%2.%3.%4")
            .arg((_head.device_ip >>  0) & 0xFF)
            .arg((_head.device_ip >>  8) & 0xFF)
            .arg((_head.device_ip >> 16) & 0xFF)
            .arg((_head.device_ip >> 24) & 0xFF);
}

QString PacketBase::getDeviceMac()
{
    return QString::asprintf("%02X%02X%02X%02X%02X%02X",//%02X   十六进制的格式输出整数
            _head.device_mac[0], _head.device_mac[1], _head.device_mac[2],
            _head.device_mac[3], _head.device_mac[4], _head.device_mac[5]);
}

bool PacketBase::checkValid(QByteArray buff, QString& errInfo)
{
    struct MsgHead *h = (struct MsgHead *)buff.data();

    if(buff.length() < MSG_HEAD_LEN)//字节数组的长度小于数据包头
    {
        errInfo = QString("Message length < MSG_HEAD_LEN(%1)").arg(MSG_HEAD_LEN);
        return false;
    }
    //    qDebug() << __FUNCTION__ << h->manufacturer << QString("%1.%2.%3.%4") .arg((h->device_ip >>  0) & 0xFF)
    //                .arg((h->device_ip >>  8) & 0xFF) .arg((h->device_ip >> 16) & 0xFF).arg((h->device_ip >> 24) & 0xFF);

    if(Config::curMfrsCfg().CheckDevMfrs){
        if (h->manufacturer != Config::curMfrsCfg().CurMfrs) {
            errInfo = QString("Not this Manufactur: %1").arg(h->manufacturer);
            return false;
        }
    }

    int sum = 0;
    for(unsigned short i = MSG_HEAD_LEN; i < h->msg_length; i++)
    {
        sum += (quint8)buff[i];
    }

    if(h->body_checksum != sum)
    {
        errInfo = QString("CheckSum error[%1 != %2]").arg(h->body_checksum).arg(sum);
        return false;
    }

    return true;
}

MsgData_Trn PacketBase::getBodyTrn()
{
//    Q_ASSERT(_head.device_type == DEV_TYPE_DNTE_TRN);
    MsgData_Trn body;
    size_t cpyLen = _bodyBuff.count();
    if(cpyLen > sizeof(MsgData_Trn)){
        qDebug() << QString("##warn## packLen(msgType:%1, len:%2) > sizeof(MsgData_Trn):%3")
                    .arg(getMsgType(), 0, 16).arg(_bodyBuff.count()).arg(sizeof(MsgData_Trn));
        cpyLen = sizeof(MsgData_Trn);
    }
#ifdef QT_DEBUG
//    if(getMsgType() == Msg_Type_NAP_InOutLevel)
//    {
//        static QDateTime LastTime = QDateTime::currentDateTime();//调试用
//        if(LastTime.msecsTo(QDateTime::currentDateTime()) > 10000)
//        {
//            qDebug()<< "MSG_InOutLevel Len:" << cpyLen;
//            LastTime = QDateTime::currentDateTime();
//        }
//    }
#endif
    memcpy((void *)&body, _bodyBuff.constData(), cpyLen);//.constData()函数返回一个指向存储在字节数组中的字节
    return body;
}

MsgData_Spk PacketBase::getBodySpk()
{
    Q_ASSERT(_head.device_type == Dev_Type_AES67_SPK);
    MsgData_Spk body;
    memcpy(&body, _bodyBuff.constData(), _bodyBuff.count()/*sizeof(MsgData_Spk)*/);
    return body;
}

MsgData_Amp PacketBase::getBodyAmp()
{
    Q_ASSERT(_head.device_type == Dev_Type_STM32_POE_AES67_SPK_0_2);
    MsgData_Amp body;
    memcpy(&body, _bodyBuff.constData(), _bodyBuff.count()/*sizeof(MsgData_Amp)*/);
    return body;
}

MsgData_STM32_POE PacketBase::getBody_STM32_POE()
{
    Q_ASSERT(_head.device_type == Dev_Type_STM32_POE_AES67_SPK_0_1 ||
             _head.device_type == Dev_Type_STM32_POE_AES67_SPK_0_2 ||
             _head.device_type == Dev_Type_STM32_POE_AES67_MIC_2_0);
    MsgData_STM32_POE body;
    memcpy(&body, _bodyBuff.constData(), _bodyBuff.count()/*sizeof(MsgData_Amp)*/);
    return body;
}

MsgData_Mic_Quad PacketBase::getBodyMicQuad()
{
    Q_ASSERT(_head.device_type == Dev_Type_RT1052_POE_AES67_MIC_4_0);
    MsgData_Mic_Quad body;
    memcpy(&body, _bodyBuff.constData(), _bodyBuff.count()/*sizeof(MsgData_Mic_Quad)*/);
    return body;
}

MsgData_RT1052_Poe PacketBase::getBody_RT1052_Poe()
{
//    Q_ASSERT(_head.device_type == Dev_Type_RT1052_POE_AES67_LINE_4_4);
    MsgData_RT1052_Poe body;
    memcpy(&body, _bodyBuff.constData(), _bodyBuff.count()/*sizeof(MsgData_RT1052_Poe)*/);
    return body;
}

MsgData_Amp_0_2_Line PacketBase::getBodyAmp_0_2_Line()
{
    Q_ASSERT(_head.device_type == Dev_Type_RT1052_POE_AES67_LINE_0_2);
    MsgData_Amp_0_2_Line body;
    memcpy(&body, _bodyBuff.constData(), _bodyBuff.count());
    return body;
}

MsgData_Amp_2_0_Mic_Line PacketBase::getBodyAmp_2_0_Mic_Line()
{
    Q_ASSERT(_head.device_type == Dev_Type_RT1052_POE_AES67_MIC_2_0 ||
             _head.device_type == Dev_Type_RT1052_POE_AES67_MIC_2_0_Type1 ||
             _head.device_type == Dev_Type_RT1052_POE_AES67_MIC_2_0_Type2 ||
             _head.device_type == Dev_Type_RT1052_POE_AES67_MIC_2_0_Type3);
    MsgData_Amp_2_0_Mic_Line body;
    memcpy(&body, _bodyBuff.constData(), _bodyBuff.count());
    return body;
}

MsgData_Moniter_SPK PacketBase::getBodyMonitorSPK()
{
    Q_ASSERT(_head.device_type == Dev_Type_AES67_MONITOR_SPK);
    MsgData_Moniter_SPK body;
    memcpy(&body, _bodyBuff.constData(), _bodyBuff.count());
    return body;
}
