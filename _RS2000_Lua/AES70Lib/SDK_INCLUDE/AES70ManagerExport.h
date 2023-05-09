#pragma once

#ifdef  _AES70_DEVICE_MANAGER_				//ע��:�����ɸ�DLL�ı������ж���
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
	long				nPrimaryOsInterfaceIndex,			//������������������ӿ�
	long				nSecondaryOsInterfaceIndex,			//�����������Ĵ�����ӿ�
	unsigned __int32	nPrimaryInterfaceSubNetwork,		//���ӿ����ε�ַ
	unsigned __int32	nSecondaryInterfaceSubNetwork,		//�ӽӿ����ε�ַ
	unsigned __int32	nPrimaryInterfaceSubNetworkMask,	//���ӿ���������
	unsigned __int32	nSecondaryInterfaceSubNetworkMask,	//�ӽӿ���������
	LPCTSTR			strGroupIp,						//�������鲥��ַ
	WORD				nPortNo,							//�������˿ڵ�ַ
	LPCTSTR			strLogFileName
	);
AES70_MANAGER_DLL bool  AES70_StopManager(HANDLE hManager);
AES70_MANAGER_DLL bool	AES70_SetAutoSubscribeEvent(HANDLE hManager, DWORD EnableBit);	//�����Զ������¼���EnableBit�Ķ����E_DeviceEventBit_T����

AES70_MANAGER_DLL long	AES70_GetDeviceMaxAudioChannelNum(HANDLE hManager);
AES70_MANAGER_DLL long  AES70_GetMaxStreamConnectorNum(HANDLE hManager);
AES70_MANAGER_DLL long  AES70_GetMaxStreamAudioChannelNum(HANDLE hManager);

AES70_MANAGER_DLL bool  AES70_SetManagerContext(HANDLE hManager, void *pAppContext);
AES70_MANAGER_DLL bool  AES70_SetPrintDebugMsgFunction(HANDLE hManager, PF_DISPLAY_MESSAGE DisplayFunction, PF_DISPLAY_MESSAGE_LIST DisplayListFunction = NULL);

AES70_MANAGER_DLL bool  AES70_SetDeviceUpdateCallMsg(HANDLE hManager, HWND hWndCallback, UINT MsgID);

AES70_MANAGER_DLL bool  AES70_LoadNetIfCfgFile(HANDLE hManager, LPCTSTR strCfgFileName);
AES70_MANAGER_DLL bool  AES70_SetAppNetIfCfg(HANDLE hManager, long PrimaryOsInterfaceIndex, long SecondOsInterfaceIndex);
AES70_MANAGER_DLL bool	AES70_CallSetDanteCfgDlg(HANDLE hManager);					//ѡ������ӿڶԻ���
AES70_MANAGER_DLL long  AES70_PrimaryOsInterfaceIndex(HANDLE hManager);
AES70_MANAGER_DLL long  AES70_SecondaryOsInterfaceIndex(HANDLE hManager);

AES70_MANAGER_DLL bool  AES70_AppNetInterafaceCfg(HANDLE hManager, CAppNetIfCfg &AppNetInterafaceCfg);
AES70_MANAGER_DLL DWORD AES70_GetMsgID(HANDLE hManager);



//�豸����
AES70_MANAGER_DLL bool	AES70_SetDeviceName(HANDLE hManager,DWORD msgID, DWORD nDeviceIp, WORD nServicePortNo, LPCTSTR pDeviceName, CM3_ERROR	&ConnectionError, bool bPrintMsg);	//�����豸������
AES70_MANAGER_DLL bool	AES70_GetDeviceName(HANDLE hManager,DWORD msgID, DWORD nDeviceIp, WORD nServicePortNo, LPTSTR pDeviceName, long NameBufferByteSize, CM3_ERROR	&ConnectionError, bool bPrintMsg);	//���豸������

//=============================================================================================
//1) �豸������ָ���豸��Ϣ���õ��豸��IP��ַ����Ϣ
AES70_MANAGER_DLL void	AES70_GetDeviceList(HANDLE hManager, std::vector<CAES70Device> &AES70DeviceList); 
AES70_MANAGER_DLL void	AES70_GetAllDeviceList(HANDLE hManager, std::vector<CAES70Device>& AES70DeviceList, std::vector<CAES70Device>& FaultDeviceList, std::vector<CAES70Device>& VirtualMulticastDeviceList);

AES70_MANAGER_DLL bool	AES70_GetDeviceByName(HANDLE hManager, LPCTSTR strDeviceName, CAES70Device &Device);
AES70_MANAGER_DLL bool	AES70_GetDeviceByIp(HANDLE hManager, DWORD nIp, CAES70Device &Device);
AES70_MANAGER_DLL bool	AES70_GetDeviceByIp(HANDLE hManager, LPCTSTR strIp, CAES70Device &Device);
AES70_MANAGER_DLL bool	AES70_GetDeviceByMac(HANDLE hManager, long InterfaceNo, unsigned char Mac[6], CAES70Device &Device);
AES70_MANAGER_DLL bool	AES70_GetDeviceByAddress(HANDLE hManager,DWORD nIp, WORD nPortNo, WORD InterfaceNo, bool bGetAliveDevice, CAES70Device &Device);

AES70_MANAGER_DLL bool	AES70_GetDevice(HANDLE hManager, long ItemNo, CAES70Device &Device);
AES70_MANAGER_DLL bool	AES70_GetDeviceByListID(HANDLE hManager, DWORD ListID, CAES70Device &Device);

AES70_MANAGER_DLL long	AES70_GetDebugInfo_TotalBrowseNum(HANDLE hManager);	//�������������Ϣ

//==============================================================================================
//2�� ���豸��ϵͳ���ã�
// 1��MediaTransportNetworkONo��SilenceDetectorONo,DeviceNetworkONo;
// 2����Ƶ�������������ͽ�������������������Ƶ������������������䵥����Ƶ������������
//    ÿ�����������������������Ƶ���˿�����
// 3���豸���ն˿ںͷ��Ͷ˿ڵ����������� 
//  bReadFromCacheFirst = true ʱ���������������豸�����һ��������Ѿ���ȡϵͳ������Ϣ����ֱ�ӷ��ػ������е�ϵͳ���ã������Ƿ��������豸�����豸�ж���ϵͳ���á�
AES70_MANAGER_DLL bool AES70_GetDeviceSystemCfg(HANDLE hManager, LPCTSTR strDeviceName, CDeviceSystemCfg &SteamCfg, CM3_ERROR &ConnectionError, E_COM_INTERFACE ComInterface, bool bReadFromCacheFirst = false);
AES70_MANAGER_DLL bool AES70_GetDeviceSystemCfg(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, CDeviceSystemCfg &SteamCfg, CM3_ERROR &ConnectionErrorbool,bool bReadFromCacheFirst = false);

//����豸����������ONo ���
AES70_MANAGER_DLL	bool AES70_Get_MediaTransportNetwork_ONo(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T &MediaTransportNetworkONo, CM3_ERROR	&ConnectionError, bool bPrintMsg);

//==============================================================================================
//3�����豸�����շ���������Ϣ-ȷ��·��
AES70_MANAGER_DLL	bool AES70_Get_AES67_Connectors(HANDLE hManager,   LPCTSTR strDeviceName, ::std::vector< ::AES70_TX_CONNECTOR_T> &TxConnectorVector, ::std::vector< ::AES70_RX_CONNECTOR_T> &RxConnectorVector, CM3_ERROR		&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool AES70_Get_AES67_Connectors(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaoTransportNetworkONo, ::std::vector< ::AES70_TX_CONNECTOR_T> &TxConnectorVector, ::std::vector< ::AES70_RX_CONNECTOR_T> &RxConnectorVector, CM3_ERROR		&ConnectionError, bool bPrintMsg);

AES70_MANAGER_DLL	bool AES70_Get_AES67_TxConnectors(HANDLE hManager, LPCTSTR strDeviceName, ::std::vector< ::AES70_TX_CONNECTOR_T> &TxConnectorVector, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool AES70_Get_AES67_TxConnectors(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaoTransportNetworkONo,  ::std::vector< ::AES70_TX_CONNECTOR_T> &TxConnectorVector, CM3_ERROR	&ConnectionError, bool bPrintMsg);

AES70_MANAGER_DLL	bool AES70_Get_AES67_RxConnectors(HANDLE hManager, LPCTSTR strDeviceName, ::std::vector< ::AES70_RX_CONNECTOR_T> &RxConnectorVector, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool AES70_Get_AES67_RxConnectors(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaoTransportNetworkONo, ::std::vector< ::AES70_RX_CONNECTOR_T> &RxConnectorVector, CM3_ERROR	&ConnectionError, bool bPrintMsg);

//ɾ����������
AES70_MANAGER_DLL	bool AES70_Delete_AES67_Connector(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaoTransportNetworkONo, WORD ConnectorID, CM3_ERROR	&ConnectionError, bool bPrintMsg);
AES70_MANAGER_DLL	bool AES70_Delete_AES67_Connector(HANDLE hManager, LPCTSTR strDeviceName, WORD ConnectorID, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);

//������������
AES70_MANAGER_DLL bool AES70_Add_AES67_SinkConnector(HANDLE hManager,   LPCTSTR strDeviceName,  AES70_CONNECTOR_STATE_T InitialState, AES70_RX_CONNECTOR_T &Connector, AES70_RX_CONNECTOR_T &ReturnConnector, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL bool AES70_Add_AES67_SourceConnector(HANDLE hManager, LPCTSTR strDeviceName,  AES70_CONNECTOR_STATE_T InitialState, AES70_TX_CONNECTOR_T &Connector, AES70_TX_CONNECTOR_T &returnConnector, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL bool AES70_Add_AES67_SinkConnector(  HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaTransportNetworkONo, AES70_CONNECTOR_STATE_T InitialState, AES70_RX_CONNECTOR_T &Connector, AES70_RX_CONNECTOR_T &ReturnConnector, CM3_ERROR	&ConnectionError, bool bPrintMsg);
AES70_MANAGER_DLL bool AES70_Add_AES67_SourceConnector(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaTransportNetworkONo, AES70_CONNECTOR_STATE_T InitialState, AES70_TX_CONNECTOR_T &Connector, AES70_TX_CONNECTOR_T &returnConnector, CM3_ERROR	&ConnectionError, bool bPrintMsg);

//��ָ����������������·������
AES70_MANAGER_DLL bool AES70_Set_AES67_SinkConnectorPinMap(HANDLE hManager,   LPCTSTR strDeviceName,  AES70_CONNECTOR_ID_T ConnectorID, const AES70_RX_PIN_MAP_T  &ChannelPinMap, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL bool AES70_Set_AES67_SourceConnectorPinMap(HANDLE hManager, LPCTSTR strDeviceName,  AES70_CONNECTOR_ID_T ConnectorID, const AES70_TX_PIN_MAP_T  &ChannelPinMap, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL bool AES70_Set_AES67_SinkConnectorPinMap(  HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaTransportNetworkONo, AES70_CONNECTOR_ID_T ConnectorID, const AES70_RX_PIN_MAP_T  &ChannelPinMap, CM3_ERROR	&ConnectionError, bool bPrintMsg);
AES70_MANAGER_DLL bool AES70_Set_AES67_SourceConnectorPinMap(HANDLE hManager, DWORD nDeviceIp, WORD  nPortNo, AES70_ONO_T MediaTransportNetworkONo, AES70_CONNECTOR_ID_T ConnectorID, const AES70_TX_PIN_MAP_T  &ChannelPinMap, CM3_ERROR	&ConnectionError, bool bPrintMsg);

// ���ļ���ӡ��������Ϣ
AES70_MANAGER_DLL	bool AES70_Helper_Print_Connectors(	::std::vector< ::AES70_TX_CONNECTOR_T> &TxConnectorVector,	::std::vector< ::AES70_RX_CONNECTOR_T> &RxConnectorVector,	LPCTSTR	FileName);

//·�ɹ���

AES70_MANAGER_DLL	bool	AES70_GetRouteTable(HANDLE hManager, LPCTSTR strDeviceName, CAES70Device::CRouteTableItem	*pRouteTableItem, long &MaxItemNum);


//���÷������ķ��͸�ʽ
AES70_MANAGER_DLL	bool	 AES70_SetConnectorCoding(HANDLE hManager, LPCTSTR strDeviceName, unsigned __int16 ConnectorID, unsigned __int16 CodingSchemeID, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);

//AES70_ConnectChannel()���������������豸��Զ���豸֮���·��
//strTxDeviceName �� ·��Զ�˷����ź��豸�����ơ����豸��Ϊ��ָ�룬��ʾ���������ǶϿ�����
//TxChNo��			 ·��Զ�˷����ź��豸�ķ���ͨ����ţ���Ŵ�0��ʼ��-1��ʾ�Ͽ�RxChNo�����ӡ���TxChNo=NULL��ָ���ʾ���������ǶϿ�����
//strRxDeviceName �� ·�ɱ��ؽ����ź��豸������
//RxChNo��			 ���ؽ����豸�Ľ���ͨ����ţ���Ŵ�0��ʼ
AES70_MANAGER_DLL	bool	AES70_ConnectChannel(HANDLE hManager, LPCTSTR strTxDeviceName, short TxChNo, LPCTSTR strRxDeviceName, short RxChNo, CM3_ERROR &Error, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_ConnectChannel(HANDLE hManager, LPCTSTR strTxDeviceName, LPCTSTR strRxDeviceName,short *TxChNo, short *RxChNo, unsigned short ConnectionNum, CM3_ERROR &Error, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_ConnectChannelNew(HANDLE hManager, LPCTSTR strTxDeviceName, LPCTSTR strRxDeviceName, short *TxChNo, short *RxChNo, unsigned short ConnectionNum, CM3_ERROR &Error, E_COM_INTERFACE ComInterface);



//AES70_DisConnectChannel()�������Ͽ�··��
//strDeviceName   �� ·�ɱ��ؽ����ź��豸������
//RxChNo��			 ���ؽ����豸�Ľ���ͨ����ţ���Ŵ�0��ʼ
AES70_MANAGER_DLL	bool	AES70_DisConnectChannel(HANDLE hManager, LPCTSTR strDeviceName, short ChNo, CM3_ERROR &Error, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_DisConnectChannel(HANDLE hManager, LPCTSTR strDeviceName, short *DisConnectChNo,unsigned short DisConnectChNum, CM3_ERROR &Error, E_COM_INTERFACE ComInterface);





AES70_MANAGER_DLL	bool	AES70_TriggerUpdateRouteTableNotification(HANDLE hManager, LPCTSTR strDeviceName, CM3_ERROR &Error,bool bPrintMsg, E_COM_INTERFACE ComInterface);

AES70_MANAGER_DLL	bool	AES70_Lock(HANDLE hManager, LPCTSTR strDeviceName, CM3_ERROR &ConnectionError,  E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_UnLock(HANDLE hManager, LPCTSTR strDeviceName, CM3_ERROR &ConnectionError, E_COM_INTERFACE ComInterface);

AES70_MANAGER_DLL	bool	AES70_IncLock(HANDLE hManager, LPCTSTR strDeviceName ,DWORD &nLockOwnerIp, WORD &nLockOwnerPort, WORD &nLockRefNum, CM3_ERROR &ConnectionError, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_DecLock(HANDLE hManager, LPCTSTR strDeviceName, DWORD &nLockOwnerIp, WORD &nLockOwnerPort, WORD &nLockRefNum, CM3_ERROR &ConnectionError, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_ResetLock(HANDLE hManager, LPCTSTR strDeviceName, CM3_ERROR &ConnectionError, E_COM_INTERFACE ComInterface);

AES70_MANAGER_DLL	bool	AES70_SetPTP_Master(HANDLE hManager, LPCTSTR strDeviceName, unsigned char ClockStatus, CM3_ERROR &ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);


AES70_MANAGER_DLL	bool	AES70_Delete_AES67_ALL_SinkConnectors(HANDLE hManager, LPCTSTR strDeviceName, CM3_ERROR	&ConnectionError,bool bPrintMsg, E_COM_INTERFACE ComInterface);		//ɾ���豸���е���������
AES70_MANAGER_DLL	bool	AES70_Delete_AES67_ALL_SourceConnectors(HANDLE hManager, LPCTSTR strDeviceName, CM3_ERROR	&ConnectionError, bool bPrintMsg, E_COM_INTERFACE ComInterface);	//ɾ���豸���е���������
//AES70_TransportDeviceProtocal() ��װ�����͸�����˾�豸�ڲ�����Э���
//HANDLE hManager				��(����)���������
//LPCTSTR strDeviceName			: (����)�豸���� 
//void *pProtocal				: (����)��Ÿ���˽������Э��Ļ�����
//unsigned short ProtocalByteLen: (����)��������˽������Э������ݳ���
//void *pProtocalAck			: (����)�����豸ִ��˽������Э����Ӧ�����ݰ�������
//unsigned short &AckByteLen	: (����/���)������ʱ������ָʾpProtocalAck������������ֽ��������ʱ��ָʾʵ�ʵ�Ӧ���ֽ��������豸��Ӧ���ֽڴ�������Ļ������ֽ���ʱ������ֽ��������ضϡ�
//CM3_ERROR	&ConnectionError	: (���)ָʾͨ�Ŵ���״̬
//E_COM_INTERFACE ComInterface	: (����)ָ��ʹ���ĸ�ͨ�Žӿڷ�������
//
AES70_MANAGER_DLL	bool	AES70_TransportDeviceProtocal(HANDLE hManager, LPCTSTR strDeviceName, void *pProtocal, unsigned short ProtocalByteLen, void *pProtocalAck, unsigned short &AckByteLen,CM3_ERROR	&ConnectionError, E_COM_INTERFACE ComInterface);
AES70_MANAGER_DLL	bool	AES70_TransportDeviceProtocal(HANDLE hManager, DWORD nIpDevice,WORD nPortNo, void *pProtocal, unsigned short ProtocalByteLen, void *pProtocalAck, unsigned short &AckByteLen, CM3_ERROR	&ConnectionError);
//AES70_GetMsg_TransportDeviceProtocal()�������豸��˽���������Э��װ��AES70������Ϣ��
//HANDLE hManager			��(����)���������
//void *pProtocal			: (����)��Ÿ���˽������Э��Ļ�����
//unsigned short ProtocalByteLen		��(����)��������˽������Э������ݳ���
//char *pMsgBuf				: (����)AES70������Ϣ�Ļ�����
//unsigned short &MsgBufLen(����/���): ����ʱ����ָʾpMsgBuf������������ֽ��������ʱ���˽��Э�����ݷ�װ��AES70������Ϣ����ֽ�����
//                            ���������ֽڻ�������СС�ڷ�װ��AES70������Ϣ������ֽ���������ʧ�ܷ���0�ֽ�
//����ֵ��
//true : ��Ϣ��װ�ɹ���pMsgBuf�������з��ط�װ���������Ϣ��MsgBufLen����������Ϣ���ֽ���
//false: ��Ϣ��װʧ�ܡ�

AES70_MANAGER_DLL	bool	AES70_GetMsg_TransportDeviceProtocal(HANDLE hManager, void *pProtocal, unsigned short ProtocalByteLen, char *pMsgBuf, unsigned short &MsgBufLen);
//���豸����ǰ��AES70_SetSubscriberAddress()���ö���֪ͨ�ĵ�ַ�����ӷ�ʽ
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
