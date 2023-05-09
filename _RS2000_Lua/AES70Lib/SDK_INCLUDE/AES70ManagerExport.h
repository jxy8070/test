#pragma once

#ifdef  _AES70_DEVICE_MANAGER_				//注意:在生成该DLL的编译器中定义
#define AES70_MANAGER_DLL   __declspec(dllexport)
#else
#define AES70_MANAGER_DLL   __declspec(dllimport)
#endif

#include	"AES70ManagerDataType.h"
#include	"AppNetIfCfg.h"
#include	"AES70Device.h"
#include	"AES70DeviceRoutingData_added_by_hlx.h"
#include    <vector>


AES70_MANAGER_DLL HANDLE AES70_OpenDeviceManager(void);
AES70_MANAGER_DLL bool   AES70_CloseDeviceManager(HANDLE hManager);

AES70_MANAGER_DLL bool   AES70_StartManager(HANDLE hManager,
	long				nPrimaryOsInterfaceIndex,			//接收心跳包的主网络接口
	long				nSecondaryOsInterfaceIndex,			//接收心跳包的从网络接口
	unsigned __int32	nPrimaryInterfaceSubNetwork,		//主接口网段地址
	unsigned __int32	nSecondaryInterfaceSubNetwork,		//从接口网段地址
	unsigned __int32	nPrimaryInterfaceSubNetworkMask,	//主接口网段掩码
	unsigned __int32	nSecondaryInterfaceSubNetworkMask,	//从接口网段掩码
	LPCTSTR			strGroupIp,						//心跳包组播地址
	WORD				nPortNo,							//心跳包端口地址
	LPCTSTR			strLogFileName
	);
AES70_MANAGER_DLL bool  AES70_StopManager(HANDLE hManager);
AES70_MANAGER_DLL bool	AES70_SetAutoSubscribeEvent(HANDLE hManager, DWORD EnableBit);	//设置自动订阅事件，EnableBit的定义见E_DeviceEventBit_T定义

AES70_MANAGER_DLL long	AES70_GetDeviceMaxAudioChannelNum(HANDLE hManager);
AES70_MANAGER_DLL long  AES70_GetMaxStreamConnectorNum(HANDLE hManager);
AES70_MANAGER_DLL long  AES70_GetMaxStreamAudioChannelNum(HANDLE hManager);

AES70_MANAGER_DLL bool  AES70_SetManagerContext(HANDLE hManager, void *pAppContext);
AES70_MANAGER_DLL bool  AES70_SetPrintDebugMsgFunction(HANDLE hManager, PF_DISPLAY_MESSAGE DisplayFunction, PF_DISPLAY_MESSAGE_LIST DisplayListFunction = NULL);

AES70_MANAGER_DLL bool  AES70_SetDeviceUpdateCallMsg(HANDLE hManager, HWND hWndCallback, UINT MsgID);

AES70_MANAGER_DLL bool  AES70_LoadNetIfCfgFile(HANDLE hManager, LPCTSTR strCfgFileName);
AES70_MANAGER_DLL bool  AES70_SetAppNetIfCfg(HANDLE hManager, long PrimaryOsInterfaceIndex, long SecondOsInterfaceIndex);
AES70_MANAGER_DLL bool	AES70_CallSetDanteCfgDlg(HANDLE hManager);					//选择网络接口对话框
AES70_MANAGER_DLL long  AES70_PrimaryOsInterfaceIndex(HANDLE hManager);
AES70_MANAGER_DLL long  AES70_SecondaryOsInterfaceIndex(HANDLE hManager);

AES70_MANAGER_DLL bool  AES70_AppNetInterafaceCfg(HANDLE hManager, CAppNetIfCfg &AppNetInterafaceCfg);
AES70_MANAGER_DLL DWORD AES70_GetMsgID(HANDLE hManager);



//设备名称
AES70_MANAGER_DLL bool	AES70_SetDeviceName(HANDLE hManager,DWORD msgID, DWORD nDeviceIp, WORD nServicePortNo, LPCTSTR pDeviceName, CM3_ERROR	&ConnectionError, bool bPrintMsg);	//设置设备的名称
AES70_MANAGER_DLL bool	AES70_GetDeviceName(HANDLE hManager,DWORD msgID, DWORD nDeviceIp, WORD nServicePortNo, LPTSTR pDeviceName, long NameBufferByteSize, CM3_ERROR	&ConnectionError, bool bPrintMsg);	//读设备的名称

//=============================================================================================
//1) 设备管理及读指定设备信息，得到设备的IP地址等信息
AES70_MANAGER_DLL void	AES70_GetDeviceList(HANDLE hManager, std::vector<CAES70Device> &AES70DeviceList); 
AES70_MANAGER_DLL void	AES70_GetAllDeviceList(HANDLE hManager, std::vector<CAES70Device>& AES70DeviceList, std::vector<CAES70Device>& FaultDeviceList, std::vector<CAES70Device>& VirtualMulticastDeviceList);

AES70_MANAGER_DLL bool	AES70_GetDeviceByName(HANDLE hManager, LPCTSTR strDeviceName, CAES70Device &Device);
AES70_MANAGER_DLL bool	AES70_GetDeviceByIp(HANDLE hManager, DWORD nIp, CAES70Device &Device);
AES70_MANAGER_DLL bool	AES70_GetDeviceByIp(HANDLE hManager, LPCTSTR strIp, CAES70Device &Device);
AES70_MANAGER_DLL bool	AES70_GetDeviceByMac(HANDLE hManager, long InterfaceNo, unsigned char Mac[6], CAES70Device &Device);
AES70_MANAGER_DLL bool	AES70_GetDeviceByAddress(HANDLE hManager,DWORD nIp, WORD nPortNo, WORD InterfaceNo, bool bGetAliveDevice, CAES70Device &Device);

AES70_MANAGER_DLL bool	AES70_GetDevice(HANDLE hManager, long ItemNo, CAES70Device &Device);
AES70_MANAGER_DLL bool	AES70_GetDeviceByListID(HANDLE hManager, DWORD ListID, CAES70Device &Device);

AES70_MANAGER_DLL long	AES70_GetDebugInfo_TotalBrowseNum(HANDLE hManager);	//读浏览器调试信息

//==============================================================================================
//2） 读设备的系统配置：
// 1）MediaTransportNetworkONo，SilenceDetectorONo,DeviceNetworkONo;
// 2）音频流发送连接器和接收连接器的数量，音频流的最大引脚数（传输单声道频道的数量），
//    每个接收引脚所能驱动的最大频道端口数。
// 3）设备接收端口和发送端口的数量及配置 
//  bReadFromCacheFirst = true 时，管理器若发现设备在线且缓冲区中已经读取系统配置信息，则直接返回缓冲区中的系统配置，否则是访问在线设备，从设备中读出系统配置。
AES70_MANAGER_DLL bool AES70_GetDeviceSystemCfg(HANDLE hManager, LPCTSTR strDeviceName, CDeviceSystemCfg &SteamCfg, CM3_ERROR &ConnectionError, E_COM_INTERFACE ComInterface, bool bReadFromCacheFirst = false);
AES70_MANAGER_DLL bool AES70_GetDeviceSystemCfg(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, CDeviceSystemCfg &SteamCfg, CM3_ERROR &ConnectionErrorbool,bool bReadFromCacheFirst = false);

//获得设备流连接器的ONo 编号
AES70_MANAGER_DLL	bool AES70_Get_MediaTransportNetwork_ONo(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T &MediaTransportNetworkONo, CM3_ERROR	&ConnectionError, bool bPrintMsg);

//==============================================================================================
//3）读设备的流收发连接器信息-确定路由
AES70_MANAGER_DLL	bool AES70_Get_AES67_Connectors(HANDLE hManager,   LPCTSTR strDeviceName, ::std::vector< ::AES70_TX_CONNECTOR_T> &TxConnectorVector, ::std::vector< ::AES70_RX_CONNECTOR_T> &RxConnectorVector, CM3_ERROR		&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool AES70_Get_AES67_Connectors(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaoTransportNetworkONo, ::std::vector< ::AES70_TX_CONNECTOR_T> &TxConnectorVector, ::std::vector< ::AES70_RX_CONNECTOR_T> &RxConnectorVector, CM3_ERROR		&ConnectionError, bool bPrintMsg);

AES70_MANAGER_DLL	bool AES70_Get_AES67_TxConnectors(HANDLE hManager, LPCTSTR strDeviceName, ::std::vector< ::AES70_TX_CONNECTOR_T> &TxConnectorVector, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool AES70_Get_AES67_TxConnectors(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaoTransportNetworkONo,  ::std::vector< ::AES70_TX_CONNECTOR_T> &TxConnectorVector, CM3_ERROR	&ConnectionError, bool bPrintMsg);

AES70_MANAGER_DLL	bool AES70_Get_AES67_RxConnectors(HANDLE hManager, LPCTSTR strDeviceName, ::std::vector< ::AES70_RX_CONNECTOR_T> &RxConnectorVector, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool AES70_Get_AES67_RxConnectors(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaoTransportNetworkONo, ::std::vector< ::AES70_RX_CONNECTOR_T> &RxConnectorVector, CM3_ERROR	&ConnectionError, bool bPrintMsg);

//删除流连接器
AES70_MANAGER_DLL	bool AES70_Delete_AES67_Connector(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaoTransportNetworkONo, WORD ConnectorID, CM3_ERROR	&ConnectionError, bool bPrintMsg);
AES70_MANAGER_DLL	bool AES70_Delete_AES67_Connector(HANDLE hManager, LPCTSTR strDeviceName, WORD ConnectorID, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);

//创建流连接器
AES70_MANAGER_DLL bool AES70_Add_AES67_SinkConnector(HANDLE hManager,   LPCTSTR strDeviceName,  AES70_CONNECTOR_STATE_T InitialState, AES70_RX_CONNECTOR_T &Connector, AES70_RX_CONNECTOR_T &ReturnConnector, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL bool AES70_Add_AES67_SourceConnector(HANDLE hManager, LPCTSTR strDeviceName,  AES70_CONNECTOR_STATE_T InitialState, AES70_TX_CONNECTOR_T &Connector, AES70_TX_CONNECTOR_T &returnConnector, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL bool AES70_Add_AES67_SinkConnector(  HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaTransportNetworkONo, AES70_CONNECTOR_STATE_T InitialState, AES70_RX_CONNECTOR_T &Connector, AES70_RX_CONNECTOR_T &ReturnConnector, CM3_ERROR	&ConnectionError, bool bPrintMsg);
AES70_MANAGER_DLL bool AES70_Add_AES67_SourceConnector(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaTransportNetworkONo, AES70_CONNECTOR_STATE_T InitialState, AES70_TX_CONNECTOR_T &Connector, AES70_TX_CONNECTOR_T &returnConnector, CM3_ERROR	&ConnectionError, bool bPrintMsg);

//在指定的连接器上设置路由连接
AES70_MANAGER_DLL bool AES70_Set_AES67_SinkConnectorPinMap(HANDLE hManager,   LPCTSTR strDeviceName,  AES70_CONNECTOR_ID_T ConnectorID, const AES70_RX_PIN_MAP_T  &ChannelPinMap, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL bool AES70_Set_AES67_SourceConnectorPinMap(HANDLE hManager, LPCTSTR strDeviceName,  AES70_CONNECTOR_ID_T ConnectorID, const AES70_TX_PIN_MAP_T  &ChannelPinMap, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL bool AES70_Set_AES67_SinkConnectorPinMap(  HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaTransportNetworkONo, AES70_CONNECTOR_ID_T ConnectorID, const AES70_RX_PIN_MAP_T  &ChannelPinMap, CM3_ERROR	&ConnectionError, bool bPrintMsg);
AES70_MANAGER_DLL bool AES70_Set_AES67_SourceConnectorPinMap(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaTransportNetworkONo, AES70_CONNECTOR_ID_T ConnectorID, const AES70_TX_PIN_MAP_T  &ChannelPinMap, CM3_ERROR	&ConnectionError, bool bPrintMsg);

// 向文件打印连接器信息
AES70_MANAGER_DLL	bool AES70_Helper_Print_Connectors(	::std::vector< ::AES70_TX_CONNECTOR_T> &TxConnectorVector,	::std::vector< ::AES70_RX_CONNECTOR_T> &RxConnectorVector,	LPCTSTR	FileName);

//路由管理

AES70_MANAGER_DLL	bool	AES70_GetRouteTable(HANDLE hManager, LPCTSTR strDeviceName, CAES70Device::CRouteTableItem	*pRouteTableItem, long &MaxItemNum);


//设置发送流的发送格式
AES70_MANAGER_DLL	bool	 AES70_SetConnectorCoding(HANDLE hManager, LPCTSTR strDeviceName, unsigned __int16 ConnectorID, unsigned __int16 CodingSchemeID, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);

//AES70_ConnectChannel()：用来建立本地设备和远端设备之间的路由
//strTxDeviceName ： 路由远端发送信号设备的名称。若设备名为空指针，表示这组命令是断开命令
//TxChNo：			 路由远端发送信号设备的发送通道编号，编号从0开始，-1表示断开RxChNo的连接。若TxChNo=NULL空指针表示这组命令是断开命令
//strRxDeviceName ： 路由本地接收信号设备的名称
//RxChNo：			 本地接收设备的接收通道编号，编号从0开始
AES70_MANAGER_DLL	bool	AES70_ConnectChannel(HANDLE hManager, LPCTSTR strTxDeviceName, short TxChNo, LPCTSTR strRxDeviceName, short RxChNo, CM3_ERROR &Error, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_ConnectChannel(HANDLE hManager, LPCTSTR strTxDeviceName, LPCTSTR strRxDeviceName,short *TxChNo, short *RxChNo, unsigned short ConnectionNum, CM3_ERROR &Error, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_ConnectChannelNew(HANDLE hManager, LPCTSTR strTxDeviceName, LPCTSTR strRxDeviceName, short *TxChNo, short *RxChNo, unsigned short ConnectionNum, CM3_ERROR &Error, E_COM_INTERFACE ComInterface);



//AES70_DisConnectChannel()：用来断开路路由
//strDeviceName   ： 路由本地接收信号设备的名称
//RxChNo：			 本地接收设备的接收通道编号，编号从0开始
AES70_MANAGER_DLL	bool	AES70_DisConnectChannel(HANDLE hManager, LPCTSTR strDeviceName, short ChNo, CM3_ERROR &Error, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_DisConnectChannel(HANDLE hManager, LPCTSTR strDeviceName, short *DisConnectChNo,unsigned short DisConnectChNum, CM3_ERROR &Error, E_COM_INTERFACE ComInterface);





AES70_MANAGER_DLL	bool	AES70_TriggerUpdateRouteTableNotification(HANDLE hManager, LPCTSTR strDeviceName, CM3_ERROR &Error,bool bPrintMsg, E_COM_INTERFACE ComInterface);

AES70_MANAGER_DLL	bool	AES70_Lock(HANDLE hManager, LPCTSTR strDeviceName, CM3_ERROR &ConnectionError,  E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_UnLock(HANDLE hManager, LPCTSTR strDeviceName, CM3_ERROR &ConnectionError, E_COM_INTERFACE ComInterface);

AES70_MANAGER_DLL	bool	AES70_IncLock(HANDLE hManager, LPCTSTR strDeviceName ,DWORD &nLockOwnerIp, WORD &nLockOwnerPort, WORD &nLockRefNum, CM3_ERROR &ConnectionError, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_DecLock(HANDLE hManager, LPCTSTR strDeviceName, DWORD &nLockOwnerIp, WORD &nLockOwnerPort, WORD &nLockRefNum, CM3_ERROR &ConnectionError, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_ResetLock(HANDLE hManager, LPCTSTR strDeviceName, CM3_ERROR &ConnectionError, E_COM_INTERFACE ComInterface);

AES70_MANAGER_DLL	bool	AES70_SetPTP_Master(HANDLE hManager, LPCTSTR strDeviceName, unsigned char ClockStatus, CM3_ERROR &ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);


AES70_MANAGER_DLL	bool	AES70_Delete_AES67_ALL_SinkConnectors(HANDLE hManager, LPCTSTR strDeviceName, CM3_ERROR	&ConnectionError,bool bPrintMsg, E_COM_INTERFACE ComInterface);		//删除设备所有的流接收器
AES70_MANAGER_DLL	bool	AES70_Delete_AES67_ALL_SourceConnectors(HANDLE hManager, LPCTSTR strDeviceName, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);	//删除设备所有的流发送器
//AES70_TransportDeviceProtocal() 封装并发送福川公司设备内部命令协议包
//HANDLE hManager				：(输入)管理器句柄
//LPCTSTR strDeviceName			: (输入)设备名称 
//void *pProtocal				: (输入)存放福川私有命令协议的缓冲区
//unsigned short ProtocalByteLen: (输入)缓冲区中私有命令协议的数据长度
//void *pProtocalAck			: (输入)福川设备执行私有命令协议后的应答数据包缓冲区
//unsigned short &AckByteLen	: (输入/输出)，输入时是用来指示pProtocalAck缓冲区的最大字节数，输出时是指示实际的应答字节数。当设备的应答字节大于输入的缓冲区字节数时，输出字节数将被截断。
//CM3_ERROR	&ConnectionError	: (输出)指示通信错误状态
//E_COM_INTERFACE ComInterface	: (输入)指定使用哪个通信接口发送命令
//
AES70_MANAGER_DLL	bool	AES70_TransportDeviceProtocal(HANDLE hManager, LPCTSTR strDeviceName, void *pProtocal, unsigned short ProtocalByteLen, void *pProtocalAck, unsigned short &AckByteLen,CM3_ERROR	&ConnectionError, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_TransportDeviceProtocal(HANDLE hManager, DWORD nIpDevice,WORD nPortNo, void *pProtocal, unsigned short ProtocalByteLen, void *pProtocalAck, unsigned short &AckByteLen, CM3_ERROR	&ConnectionError);
//AES70_GetMsg_TransportDeviceProtocal()将福川设备的私有议命令封协议装成AES70命令消息。
//HANDLE hManager			：(输入)管理器句柄
//void *pProtocal			: (输入)存放福川私有命令协议的缓冲区
//unsigned short ProtocalByteLen		：(输入)缓冲区中私有命令协议的数据长度
//char *pMsgBuf				: (输入)AES70命令消息的缓冲区
//unsigned short &MsgBufLen(输入/输出): 输入时用来指示pMsgBuf缓冲区的最大字节数，输出时存放私有协议数据封装成AES70命令消息后的字节数。
//                            如果输入的字节缓冲区大小小于封装成AES70命令消息所需的字节数，则函数失败返回0字节
//返回值：
//true : 消息封装成功，pMsgBuf缓冲区中返回封装后的命令消息，MsgBufLen返回命令消息的字节数
//false: 消息封装失败。

AES70_MANAGER_DLL	bool	AES70_GetMsg_TransportDeviceProtocal(HANDLE hManager, void *pProtocal, unsigned short ProtocalByteLen, char *pMsgBuf, unsigned short &MsgBufLen);
//在设备启动前用AES70_SetSubscriberAddress()设置订阅通知的地址和连接方式
AES70_MANAGER_DLL	bool	AES70_SetSubscriberAddress(HANDLE hManager, E_DeviceEventNo_T EventNo, DWORD nSubscriberIp, WORD nSubscriberPortNo, bool bIsTcpNotification);
AES70_MANAGER_DLL	bool	AES70_AddSubscription(HANDLE hManager,    LPCTSTR strDeviceName, E_DeviceEventNo_T DeviceEventNo, DWORD nSubscriberIp, WORD nSubscriberPortNo, bool bTcpNotification, CM3_ERROR	&ConnectionError, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_RemoveSubscription(HANDLE hManager, LPCTSTR strDeviceName, E_DeviceEventNo_T EventNo,bool bTcpNotification, CM3_ERROR	&ConnectionError, E_COM_INTERFACE ComInterface);

AES70_MANAGER_DLL	bool	AES70_AddTcpSubscription(HANDLE hManager,    LPCTSTR strDeviceName, E_DeviceEventNo_T DeviceEventNo,  CM3_ERROR	&ConnectionError, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_RemoveTcpSubscription(HANDLE hManager, LPCTSTR strDeviceName, E_DeviceEventNo_T EventNo,        CM3_ERROR	&ConnectionError, E_COM_INTERFACE ComInterface);

AES70_MANAGER_DLL	bool	AES70_GetSilenceSensorState(HANDLE hManager, LPCTSTR strDeviceName, DWORD TxState[8], DWORD RxState[8], CM3_ERROR	&ConnectionError, E_COM_INTERFACE ComInterface);



AES70_MANAGER_DLL	bool	AES70_EnablePrintKeepAliveMsg(HANDLE hManager, bool bEnablePrintKeepAliveMsg);
AES70_MANAGER_DLL	bool	AES70_EnablePrintBrowserMsg(HANDLE hManager, bool bEnablePrintBrowserMsg);
AES70_MANAGER_DLL	bool	AES70_EnablePrintResponseMsg(HANDLE hManager, bool bEnablePrintResponseMsg);
AES70_MANAGER_DLL	bool	AES70_EnablePollingRouteInfoMsg(HANDLE hManager, bool bEnablePollingDeviceMsg);
AES70_MANAGER_DLL	bool	AES70_EnablePrintAllConnection(HANDLE hManager, bool bEnablePrintAllConnection);


AES70_MANAGER_DLL	bool	AES70_SetDisplayCommandMsgWndNo(HANDLE hManager, WORD WndNo);
AES70_MANAGER_DLL	bool	AES70_SetDisplayResposneMsgWndNo(HANDLE hManager, WORD WndNo);
AES70_MANAGER_DLL	bool	AES70_SetDisplayKeepAliveMsgWndNo(HANDLE hManager, WORD WndNo);
AES70_MANAGER_DLL	bool	AES70_SetDisplayDnsBrowserMsgWndNo(HANDLE hManager, WORD WndNo);
AES70_MANAGER_DLL	bool	AES70_SetDisplayPollingDeviceMsgWndNo(HANDLE hManager, WORD WndNo);

AES70_MANAGER_DLL	bool	AES70_Debug_MediaClock3(HANDLE hManager, LPCTSTR strTxDeviceName, CM3_ERROR	&ConnectionError);

AES70_MANAGER_DLL	LPCTSTR	AES70_Helper_GetDeviceEventName(E_DeviceEventNo_T DeviceEventNo);
AES70_MANAGER_DLL	bool	AES70_Helper_GetConnectionSocketAddress(HANDLE hManager, DWORD nDeviceIp, WORD nDevicePortNo, DWORD &nSocketIp, WORD &nSocketPortNo);



//Add by Huliu
AES70_MANAGER_DLL void  AES70_SettingAppNetIf(HANDLE hManager, int PriIdx, int SecIdx);	   //Add by Huliu 20180514
AES70_MANAGER_DLL bool  AES70_SetDeviceUpdateCallBack(HANDLE hManager, PF_DEVLIST_UPDATE pdevlistupdatecallback);	//Add by Huliu 20180803

// Add by Huliu 20180906
AES70_MANAGER_DLL	bool	AES70_GetDeviceVerson(HANDLE hManager, UINT32 nIP, UINT32 &data1, UINT32 &data2, UINT32 &data3);
AES70_MANAGER_DLL	bool	AES70_SetProcFitcanDevMsgCallback(HANDLE hManager, PF_PROC_FITCAN_DEVICE_MSG pdevlistupdatecallback);
AES70_MANAGER_DLL	bool	AES70_UpdateRoutingTable(HANDLE hManager, AES70DeviceRoutingData::DeviceRoutingList_t &DeviceRoutingList);
AES70_MANAGER_DLL	bool	AES70_UpdateRoutingTable(HANDLE hManager, AES70DeviceRoutingData::DeviceRoutingMap_t &DeviceRoutingList);
AES70_MANAGER_DLL	bool	AES70_ClearWasteConnector(HANDLE hManager);


AES70_MANAGER_DLL	bool	AES70_Debug(HANDLE hManager, LPCTSTR strDeviceName);
