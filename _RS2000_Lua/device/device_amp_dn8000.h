#ifndef DEVICE_AMP_DN8000_H
#define DEVICE_AMP_DN8000_H

#include "AES70device.h"
#include "protocal/packet.h"
#include <QTcpSocket>

struct ChState
{
    bool isMute;
    bool fault;
    bool isClip;
    bool hasSignel;
    bool protect;
    float temprate;
    float valtage;
    float electricity;
};

class AlarmConfig;
class Device_Amp_DN8000: public AES70Device
{
    Q_OBJECT
    enum {ChCount = 8};
public:
    struct PTP_State {
        unsigned short State;   /* PTP时钟状态，参考上面的枚举 */
        short Offset;           /* PTP时钟偏移，单位PPM*/
        unsigned char GrandMasterID[8];/* 主时钟的ID号 */
    };

public:
    Device_Amp_DN8000(QString mac, QString devName, DevType devType, QString devIp, QString creator);
    virtual void serialize(QString fileName);
    virtual void deserialize(QString fileName);


public://处理数据包
    virtual void processPacket(PacketBase* pack);

public://inline
    QString getVersion() {return _version;}
    QString getAmpVersion() {return _ampVersion;}
    PTP_State& getPTPState() {return _ptpState;}
    bool isAmpStandby() {return _isAmpStandby;}

public slots:
    void onReceiveTcpPacket();
    void sendBlinkLedCmd();
    void sendGetVersionCmd();
    void sendGetConnectStateCmd();
    void sendGetBoardStateCmd();
    void sendSetBoardStateCmd();
    void sendGetChannelStateCmd();
    //rs232透传包
    void setChannelMuteState(quint8 ch, bool mute);
    void getChannelMuteState(quint8 ch);
    void setStandbyState(bool standby);
    void getStandbyState();
    void setChannelProtect(quint8 ch, bool protect);


signals:
    void channelStateChanged(ChState* pData);

    void amplifierParamChanged();
    void initConfigFromDevice();
private:
    void sendPacket(int command, QByteArray packBuff);
    void rs232PacketHandler(int command, QByteArray& packData);
    void tcpPacketHandler(int command, QByteArray& packData);

private:
    QTcpSocket *_tcpClient = NULL;
    ChState _chState[8];
    QString _version;
    QString _ampVersion;
    bool _isAmpStandby;
    PTP_State _ptpState;

    //================= 数据包定义 =================//
    enum {
        COMMAND_NONE = 0x00,
        COMMAND_GET_VERSION = 0x01,         //获取版本信息
        COMMAND_GET_CONNECT_STATE = 0x02,   //获取AES67与功放卡数据连接状态
        COMMAND_GET_RX_INFO = 0x03,         //获取AES67接收流信息
        COMMAND_ADD_RX = 0x04,              //新建AES67接收流
        COMMAND_DEL_RX = 0x05,              //删除AES67接收流
        COMMAND_GET_MODULE_NAME = 0x06,     //获取模块名称
        COMMAND_SET_MODULE_NAME = 0x07,     //设置模块名称
        COMMAND_GET_BOARD_STATE = 0x08,     //获取AES67卡状态
        COMMAND_SET_BOARD_STATE = 0x09,     //设置AES67卡的IP地址，设置后重启生效
        COMMAND_GET_CHANNEL_STATE = 0x0A,   //获取AES67卡输出信号状态
        COMMAND_TRANSFER_DATA_FROM_PC = 0x0B,   //PC往功放发送串口数据
        COMMAND_LED_BLINK = 0x0C,

        COMMAND_ALIVE = 0x8000,             //心跳包
        //功放往PC转发串口数据
        COMMAND_TRANSFER_DATA_FROM_AMP = COMMAND_ALIVE | COMMAND_TRANSFER_DATA_FROM_PC,
        //上报AES67与功放卡数据连接状态
        COMMAND_REPORT_CONNECT_STATE = COMMAND_ALIVE | COMMAND_GET_CONNECT_STATE,
        //上报AES67卡状态
        COMMAND_REPORT_BOARD_STATE = COMMAND_ALIVE | COMMAND_GET_BOARD_STATE,
        //上报AES67卡输出信号状态
        COMMAND_REPORT_CHANNEL_STATE = COMMAND_ALIVE | COMMAND_GET_CHANNEL_STATE,
    };

    struct PackcetHeader {
        unsigned char Sync;/* 同步头 0x3B */
        unsigned char Version;/* 协议版本号，目前为1 */
        unsigned short Command;/* 指令，参考上面的宏定义 */
        unsigned short DataLen;/* 数据区长度 */
        unsigned short DataCheckSum;/* 数据区累加校验和 */
    };

    struct discover_t {
        unsigned int LocalIP;/* 本机IP地址 */
        unsigned short Port;/* 通信端口号 */
        unsigned short Protocol;/* 通信协议， 1：UDP， 2：TCP */
        unsigned char MAC[6];/* 设备的MAC地址 */
        unsigned short Reserve;/* 保留 */
    };

    struct version_t {
        unsigned short Major;/* 主版本 */
        unsigned short Minor;/* 次版本 */
        unsigned short Year;/* 该版本创建的年 */
        unsigned char Month; /* 该版本创建的月 */
        unsigned char Day; /* 该版本创建的日 */
    };

    struct AES67_RX_Get {
        unsigned short StreamID;/* 流编号 */
        unsigned short StreamContainCh;/* 流中包含通道数 */
        unsigned int SrcIP;/* 流的源地址 */
        unsigned int DestIP;/* 流的目标地址 */
        unsigned short DestPort;/* 流的目标端口号 */
        unsigned short Reserve;/* 保留 */
        unsigned char ChannelMap[8];/* 通道映射，00为通道无映射，映射编号最大为通道数 */
    };

    struct AES67_RX_Set {
        unsigned int SrcIP;/* 流的源地址 */
        unsigned int DestIP;/* 流的目标地址 */
        unsigned short DestPort;/* 流的目标端口号 */
        unsigned short StreamContainCh;/* 流中包含通道数 */
        unsigned char ChannelMap[8];/* 通道映射，00为通道无映射，映射编号最大为通道数 */
    };

    enum {
        PTP_INITIALIZING = 0,
        PTP_FAULTY,
        PTP_DISABLED,
        PTP_LISTENING,
        PTP_PRE_MASTER,
        PTP_MASTER,
        PTP_PASSIVE,
        PTP_UNCALIBRATED,
        PTP_SLAVE
    };



    struct AES67_IP {
        unsigned int ip;/* IP地址 */
        unsigned int mask;/* 子网掩码 */
        unsigned int gateway;/* 网关地址 */
        unsigned int isDHCP;/* 是否打开DHCP功能，1：打开，0：关闭 */
    };

    struct Msg {
        struct PackcetHeader header;/* 数据帧头 */
        union {
            unsigned char Data[];/* 用于计算DataCheckSum的指针 */

            struct discover_t Discover;/* 发现设备 */

            struct version_t Version;/* 软件版本号 */

            struct AES67_RX_Get RxInfo[8];/* 查询当前设备接收流信息 */
            struct AES67_RX_Set RxSet;/* 新建接收流 */
            unsigned short StreamID;/* 删除已有流 */

            unsigned char Name[255];/* 模块名称，实际使用长度在DataLen中指定 */

            struct PTP_State PTP;/* AES67卡状态 */
            struct AES67_IP Board_IP;/* 设置AES67卡的IP地址 */

            unsigned char AES67OutStreamInfo[8];/* 输出信号状态，1：有信号输出，0：无信号输出 */
            unsigned char TransferData[256];/* PC往功放转发串口数据，最大256B，实际使用长度填充在DataLen区块 */

            //        struct {
            //            unsigned int LocalIP;
            //            union {
            //                unsigned char TransferData[256];/* 功放往PC转发串口数据，最大256B，实际使用长度填充在DataLen区块 */
            //                struct PTP_State PTP;/* 上报AES67卡状态 */
            //                unsigned char AES67OutStreamInfo[8];/* 上报输出信号状态，1：有信号输出，0：无信号输出 */
            //            };
            //        };
        };
    };
};




#endif // DEVICE_AMP_DN8000_H
