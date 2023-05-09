#ifndef FIRMWAREPACKET_H
#define FIRMWAREPACKET_H


#define UPDATE_ONLINE_IP            "224.0.5.12"
#define MAX_RETRY                   5
enum FirmDevType{
    FirmDev_Type_PC = 0x00000000,

    FirmDev_Type_MiniPCI_SC582,
    FirmDev_Type_MiniPCI_ZYNQ7020,
    FirmDev_Type_PoeMonoSpeaker_STM32F407,
    FirmDev_Type_PoeStereoSpeaker_STM32F407,
    FirmDev_Type_PoeMicrophone_STM32F407,
    FirmDev_Type_QuadSpeaker_iMXRT1052,
    FirmDev_Type_MonitorSpeaker_iMXRT1052,
    FirmDev_Type_Matrix_ZYNQ7100,
    FirmDev_Type_PhoneticDistinction_ZYNQ7020,
    FirmDev_Type_SoundKing_DM1616_SC584,
    FirmDev_Type_CMark_ADD1010_SC584,
    FirmDev_Type_MiniPCI_ZYNQ7010,
    FirmDev_Type_AES3_ZYNQ7010,
    FirmDev_Type_DongFangJiaLian_iMXRT1052,
    FirmDev_Type_DR3020_iMXRT1052,
    FirmDev_Type_DDS208i_zynq7010,

    FirmDev_Type_Max,
};
enum FirmMsgType{
    Type_SearchDev = 0x00000001,
    Type_SearchDev_Ack,
    Type_SetIP,
    Type_SetIP_Ack,
    Type_SetMAC,
    Type_SetMAC_Ack,
    Type_UpdateOnline,
    Type_UpdateOnline_Ack,
    Type_Reboot,
    Type_Reboot_Ack,
    Type_Get_L3Route,			// ??header,???body
    Type_Get_L3Route_Ack,		// header + body
    Type_Set_L3Route,			// header + body
    Type_Set_L3Route_Ack,     // ???header,???body
    Type_Set_DeviceName,
    Type_Set_DeviceName_Ack,
    Type_Set_StereoMode,
    Type_Set_StereoMode_Ack,
};
typedef struct {
    unsigned char sync[4];
    unsigned int dev_type;
    unsigned int type;
    unsigned char reserve[64];
    unsigned int checksum;
} Update_Online_Udp_Header_t;
typedef struct {
    unsigned char sync[4];
    unsigned int  dev_type;
    unsigned char update_type;
    unsigned char reserve[119];
} Update_Online_File_Header_t;
typedef struct {
    unsigned int current_ip;
    unsigned short dhcp_0;
    unsigned short dhcp_1;
    unsigned int ip_0;
    unsigned int netmask_0;
    unsigned int gateway_0;
    unsigned int ip_1;
    unsigned int netmask_1;
    unsigned int gateway_1;
    unsigned char redundancy;
    unsigned char current_MAC[6];
} setIP_t;
typedef struct {
    unsigned int ip_0;
    unsigned int netmask_0;
    unsigned int gateway_0;
    unsigned int ip_1;
    unsigned int netmask_1;
    unsigned int gateway_1;
    unsigned short dhcp_0;
    unsigned short dhcp_1;
    unsigned char MAC_0[6];
    unsigned char MAC_1[6];
    unsigned char name[255];
    unsigned char redundancy;
    int major;
    int minor;
    int build;
    unsigned char bStereo;
    unsigned char Reserve[3];
} devInfo_t;
typedef struct {
    unsigned int total_blk;
    unsigned int current_blk;
    unsigned char update_type;
    unsigned char reserve1[3];
    unsigned int src_ip;
    unsigned char reserve2[12];
    unsigned char data[1024];
} firmSecion_t;
typedef struct {
    char name[255];
} setDeviceName_t;
typedef struct {
    union {
        devInfo_t searchDev;
        setIP_t setIP;
        firmSecion_t updateData;
        setDeviceName_t setDevName;
    };
} Update_Online_Udp_Body_t;

typedef struct {
    Update_Online_Udp_Header_t h;
    Update_Online_Udp_Body_t b;
} Update_Online_Udp_t;






#endif // FIRMWAREPACKET_H
