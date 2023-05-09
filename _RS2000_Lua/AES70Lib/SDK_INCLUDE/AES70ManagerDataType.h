#pragma once

#include <windows.h>
#include <vector>
#include <map>
#include <string>
#include "DeviceInterfaceNumDef.h"
#include "AES70Connectors.h"

//=================================================================================
//����������ʾ������Ϣ�Ļص�������Լ����TargetWndNo = 0 ʱ����ʾ��Ϣ
typedef void(*PF_DISPLAY_MESSAGE)(void *pContext, const TCHAR *strMessage, WORD TargetWndNo);
typedef void(*PF_DISPLAY_MESSAGE_LIST)(void *pContext, const std::vector<const TCHAR *> &strMessageList, WORD TargetWndNo);
typedef void(*PF_DEVLIST_UPDATE)(void *pAppContext, long deviceprestaus, long devicecurstatus);
typedef void(*PF_PROC_FITCAN_DEVICE_MSG)(void *pContext, const char *fitcandevmsg, UINT msglength);

enum class E_DEVICE_TYPE_T :unsigned long {
	eDeviceTypeNormal = 0,					//������AES70�豸
	eDeviceTypeMulticastStreamSrc,			//������鲥Դ�豸
	eDeviceTypeFaultConnection,				//����ӿ����õ��豸
};

enum class E_MEDIA_DEVICE_FLAG :unsigned long {
	eMediaDeviceFlagNormal = 1<<(long)E_DEVICE_TYPE_T::eDeviceTypeNormal,
	eMediaDeviceFlagMulticastStreamSrc = 1 << (long)E_DEVICE_TYPE_T::eDeviceTypeMulticastStreamSrc,
	eMediaDeviceFlagFaultConnection = 1 << (long)E_DEVICE_TYPE_T::eDeviceTypeFaultConnection,
	eMediaDeviceFlagNormal_and_Multicast = eMediaDeviceFlagNormal| eMediaDeviceFlagMulticastStreamSrc,
};

typedef enum class E_COM_INTERFACE_T :long {
	eSelectInterface0 = 0,
	eSelectInterface1,
	eAutoSelectInterface,
} E_COM_INTERFACE;


typedef enum :long {
	eGetConnectorTxStream = 0,
	eGetConnectorRxStream,
	eGetDeviceAllTxStream,
	eGetDeviceAllRxStream,
	eGetDeviceAllStream,
} E_GET_STREAM_MODE;

typedef enum :long {
	eGetTxCh_PinNo = 0,		//ĳ������ͨ�����ӵ�����,����1�Զ�
	eGetRxCh_PinNo,			//ĳ������ͨ�����ӵ�����,���ֻ����һ����������
	eGetAllTxCh_PinNo,		//�豸���з���ͨ�����ӵ�����,ÿ������ͨ������1�Զ������
	eGetAllRxCh_PinNo,		//�豸���н���ͨ�����ӵ�����,ÿ������ͨ����������1����������������
	eGetAllTxRxCh_PinNo,	//�豸�������շ�ͨ�������ӵ�����
} E_GET_CH_PIN_NO_MODE;

typedef enum tagDeviceEventNo :WORD{
	eEventNoConnectorPropetyChanged = 0,
	eEventNoTxConnectorChanged,
	eEventNoRxConnectorChanged,
	eEventNoPTPStatusChanged,
	eEventNoFicanDeviceMsg,
	eEventNoSilenceDetectionChanged,
	eEventNoUpdateRouteTableCommand,	//Ӧ�ó���ÿ�������ú�·������ʱ���������豸����ˢ��·�ɱ�����
										//����AES70�豸�ڽ��յ�ˢ��·�ɱ������󣬻������ж����˴��¼���Ϣ��Ӧ�÷���֪ͨ��Ϣ
	eDeviceEventNum
}  E_DeviceEventNo_T;

typedef enum tagDeviceEventBit :WORD {
	eEventBitConnectorPropetyChanged = 1 << eEventNoConnectorPropetyChanged,
	eEventBitTxConnectorChanged		 = 1 << eEventNoTxConnectorChanged,
	eEventBitRxConnectorChanged		 = 1 << eEventNoRxConnectorChanged,
	eEventBitPTPStatusChanged		 = 1 << eEventNoPTPStatusChanged,
	eEventBitFicanDeviceMsg			 = 1 << eEventNoFicanDeviceMsg,
	eEventBitSilenceDetectionChanged = 1 << eEventNoSilenceDetectionChanged,
	eEventBitUpdateRouteTableCommand = 1 << eEventNoUpdateRouteTableCommand,	//Ӧ�ó���ÿ�������ú�·������ʱ���������豸����ˢ��·�ɱ�����
																			    //����AES70�豸�ڽ��յ�ˢ��·�ɱ������󣬻������ж����˴��¼���Ϣ��Ӧ�÷���֪ͨ��Ϣ
}  E_DeviceEventBit_T;




/*
typedef enum tagUpdateMsgType :DWORD{
	eDnsBrowerFindDevice	= 1L << 0,
	eAddNewDevice			= 1L << 1,	// mDNS�����豸��ͻ������豸����ʱ�豸ͨ����������������������������յ������������������ʽ�϶����������豸
									// ��ʱ�豸��״̬Ϊ CAES70DeviceEx::eDeviceIsNew
	eDeviceIsAlive			= 1L << 2,	
	eDeviceTimeOut			= 1L << 3,
	eDeviceIsDeleted		= 1L << 4,
	eDeviceStatusChange		 = 1L << 5,		//��������⵽�豸��״̬�����˱仯
	eDeviceSocketDeleted	 = 1L << 6,
	eDeviceStreamConfigSetup = 1L << 7,
	eConnectorChanged		 = 1L << 8,
	eNeedToUpdateDisplay = eAddNewDevice | eDeviceIsAlive | eDeviceTimeOut | eDeviceIsDeleted | eConnectorChanged,
}  E_UpdateMsgType;
*/

enum class E_InterfaceStatus :long{
	eInterfaceIsUnknown = 0,
	eInterfaceIsAlive,
	eInterfaceIsTimeout,
	eInterfaceIsBroken,
	eInterfaceStatusNum,
};

enum class E_DeviceStatus :long {
	eDeviceStatusUndefined = 0,
	eDeviceIsTimeout,
	eDeivceIsAlive,
	eDeviceIsNew,
	eDeviceIsWakeUp,
	eDeviceIpUnBound,
	eDeivceIsKickOff,
	eDeviceStreamCfgSetup,
	eDeviceConnectorChange,
	eDeviceDNSBrowse,
	eDeviceChannelSilenceStatusChange,
	eDeviceUpdateRouteTable,
	eDeviceVirtualMulticastSrc,
	eDeviceStatusNum,
};
typedef unsigned __int64 CM3_ERROR;

enum E_ConnectionError :CM3_ERROR {
	eConnection_NoError = 0,
	eConnection_Error_Socket_NULL,
	eConnection_Error_Socket_Not_Connected,
	eConnection_Error_Socket_Send_Data_Blocked,
	eConnection_Error_Socket_Send_Data_Fail,
	eConnection_Error_Command_Response_Timeout,
	eConnection_Error_Command_Response_Missing,
	eConnection_Error_Command_Response_Status_Incorrect,
	eConnection_Error_Unmarshal_Return_Param,
	eConnection_Error_Check_Return_Rx_Connector_ConnectionParam,
	eConnection_Error_Check_Return_Tx_Connector_ConnectionParam,
	eConnection_Error_RxDeviceOffLine,
	eConnection_Error_TxDeviceOffLine,
	eConnection_Error_RxDeviceCfgUnavailable,
	eConnection_Error_TxDeviceCfgUnavailable,

	eConnection_Error_GetRxConnectorList,
	eConnection_Error_GetTxConnectorList,

	eConnection_Error_DeleteRxConnector,
	eConnection_Error_DeleteTxConnector,

	eConnection_Error_SetRxPinMap,
	eConnection_Error_SetTxPinMap,

	eConnection_Error_InvalidRxDeviceName,
	eConnection_Error_InvalidRxChNo,
	eConnection_Error_InvalidTxChNo,

	eConnection_Error_TxConnnectorRunOut,
	eConnection_Error_RxConnnectorRunOut,

	eConnection_Error_ConnnectorPinNumLessThanConncectionChannleNum,
	eConnection_Error_RxPortNoRunOut,

	eConnection_Error_AddNewTxConnector,
	eConnection_Error_AddNewRxConnector,
	eConnection_Error_GetTxConnectorConnnectionStreamParameter67,
	eConnection_Error_GetRxConnectorConnnectionStreamParameter67,
	eConnection_Error_GetActiveInterface,
	eConnection_Error_GetRxDeviceActiveInterface,
	eConnection_Error_GetTxDeviceActiveInterface,
	eConnection_Error_TcpSubscriberNotFound,		//�����߲�����
	eConnection_Error_InvalidEventNo,
	eConnection_Error_DeviceCfgUnavailable,
	eConnection_Error_DeviceOffLine,
	eConnection_Error_NoActiveInterfaceNo,

	eConnection_Error_Num
};


std::wstring GetConnectionErrorText(CM3_ERROR error);

