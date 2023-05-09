#ifndef CMARKPACKET_H
#define CMARKPACKET_H

#include <QByteArray>
#include "packet.h"
#include "mfrscfg.h"


class QUdpSocket;
class PacketBase
{
public:
    MsgData_Trn getBodyTrn();
    MsgData_Spk getBodySpk();
    MsgData_Amp getBodyAmp();
    MsgData_STM32_POE getBody_STM32_POE();
    MsgData_RT1052_Poe getBody_RT1052_Poe();
    MsgData_Amp_0_2_Line getBodyAmp_0_2_Line();
    MsgData_Amp_2_0_Mic_Line getBodyAmp_2_0_Mic_Line();
    MsgData_Mic_Quad getBodyMicQuad();
    MsgData_Moniter_SPK getBodyMonitorSPK();

    //static
    static PacketBase* createPacket_Nobody(MsgType msgType, quint16 msgIndex);
    static PacketBase* createPacket_Withbody(MsgType msgType, quint16 msgIndex, QByteArray& body);
    static PacketBase* createBroadcastPacket(MsgType msgType);
    static PacketBase* createPacketByBuff(QByteArray& packBuff);

public: //inline
    static const quint16 UDP_WAIT_MS = 5000;
    static const quint16 DEVICE_UDP_PORT = 39030;
    static const quint16 DEVICE_UDP_PORT1 = 49530;
    static const quint16 LUAITEM_UDP_PORT = 35031;
    static const QString GROUPCAST_IP;
    static const int GROUPCAST_PORT = 9000;
    QByteArray getBuff();
    MfrsType getMfrsType(void) const { return (MfrsType)_head.manufacturer; }
    DevType getDeviceType() const {return (DevType)_head.device_type;}
    MsgType getMsgType() const {return (MsgType)_head.msg_type;}
    quint16 getMsgIndex() const {return (MsgType)_head.msg_id;}
    QString getDeviceIP(void);
    QString getDeviceMac(void);
    bool isNoBody() { return (_head.msg_length == MSG_HEAD_LEN);}
    int getBodySize() {return _bodyBuff.size();}

private:
    static bool checkValid(QByteArray buff, QString &errInfo);
    void setCheckSum();
    PacketBase(DevType devType, MsgType msgType, quint32 msgIndex);
    PacketBase(DevType devType, MsgType msgType, quint32 msgIndex, QByteArray &byteArray);
    PacketBase(QByteArray& buff);

private:
    struct MsgHead
    {
        char sync[4];//FTCN

        unsigned short manufacturer;
        unsigned short device_type;
        unsigned short msg_type;
        unsigned short msg_id;

        unsigned int device_ip;
        unsigned char device_mac[6];

        short msg_length;
        int body_checksum;
    } _head;
    QByteArray _bodyBuff;
    static const int MSG_HEAD_LEN = 28;
    static const int BROAD_CAST_MSG_INDEX = 0;
};


#endif // CMARKPACKET_H
