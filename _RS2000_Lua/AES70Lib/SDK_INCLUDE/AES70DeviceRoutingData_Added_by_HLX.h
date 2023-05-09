#pragma once

#include "DeclareExportAES70ExtDLL.h"
#include "AES70ManagerDataType.h"
#include <map>
#include <string>

//added by hlx
class EXPORT_AES70_CLASS AES70DeviceRoutingData
{
public:
	typedef enum {
		eNoRxPathAssigned = 0,			//Ƶ��Υ������
		eNoRxError,						//·����������Ӧ��RxPathAssignedNum = 1��ConnectingTxSourceNum=0,1����
		eAssignMutipleRxPathError,		//�л�·���쳣����Ӧ��RxPathAssignedNum>1������
		eConnectMutipleTxSourcesError,	//�л�·���쳣����Ӧ��ConnectingTxSourceNum > 1 ������
		eTxDeviceOffLine,				//�ź�Դ�豸����
		eTxStreamPinMisMatch,			//����Դ�����������Ƶ����Ե�������
	} RxRoutingStatus_e;

	enum MediaStreamCastMode
	{
		MEDIASTREAMCASTMODE_NONE = 0,
		MEDIASTREAMCASTMODE_UNICAST = 1,
		MEDIASTREAMCASTMODE_MULTICAST = 2,
		MEDIASTREAMCASTMODE_MAXIMUM
	};

	class  CRxChRouting {
	public:
		CRxChRouting()
		{
			bIsMulticast = false;
			eRxRoutingStatus = eNoRxPathAssigned;
			RxPathAssignedNum = 0;
			ConnectingTxSourceNum = 0;
			RxChNo = -1;
			TxChNo = -1;
			RxStreamPortNo = 0;
			RxConnectorID = 0;
			RxConnectorPinNo = 0;
			SessionID = 0;
			bIsSignalling = false;
			TxDeviceNo = -1;
		};
		RxRoutingStatus_e	eRxRoutingStatus;
		bool		bIsMulticast;
		bool		bIsSignalling;
		std::wstring		RxDeviceName;
		long		RxChNo;
		std::wstring		TxDeviceName;
		long		TxChNo;
		UINT16	RxConnectorPinNo;				//PinNo��Ŵ�1��ʼ
		UINT64	SessionID;
		UINT16	RxStreamPortNo;
		UINT16		RxConnectorID;
		long		RxPathAssignedNum;			//��¼�����������·���ĸ���������ʱӦ��Ϊ0����1
		long		ConnectingTxSourceNum;				//��ǰ·�������ӵ��ź�Դ�ĸ�����0��ʾ�����ӣ�1��ʾ�������ӣ�����1��ʾϵͳ������·��
		long		TxDeviceNo;							//�����豸����������������·���Ŀ�������
	};

	typedef enum {
		eNoTxPathAssigned = 0,							//����Ƶ��δ�����ã���Ӧ��TxPathAssignedNum = 0����
		eNoTxError,										//�ź���������
		ePartialRxConnectionMissingError,				//����·�ɲ������գ���Ӧ��TxPathAssignedNum>TxPathConnectedNum>1����
		eNoRxConnection,								//�����ź����գ�δ�����ն����ã���Ӧ��TxPathConnectedNum>1��TxPathConnectedNum=0
	}TxRoutingStatus_e;

	class  CTxChRouting {
	public:
		CTxChRouting()
		{
			eTxRoutingStatus = eNoTxPathAssigned;
			TotalConnectedNumByRxCh = 0;
			TxChNo = 0;
			bIsSignalling = false;
			bHasMulticastTx = false;
			ConnectedPathNum = 0;
		};

		class CTxPath
		{
		public:
			CTxPath()
			{
				ConnectedNumByRxCh = 0;
				TxChNo = 0;
				TxConnectorPinNo = 0;				//PinNo��Ŵ�1��ʼ
				TxConnectorID = 0;
				TxSessionID = 0;
			};
			unsigned long				ConnectedNumByRxCh;	//��¼��ǰ·���Ľ��ն�������
			long						TxChNo;					//Ƶ�����
			UINT16		TxConnectorPinNo;
			UINT16		TxConnectorID;		//Ƶ�������ӵ�һ��������ID
			UINT64		TxSessionID;
			std::wstring			RxDevicName;
		};

		long			TxChNo;
		::std::vector<CTxPath>	TxPathList;
		TxRoutingStatus_e		eTxRoutingStatus;
		unsigned long			TotalConnectedNumByRxCh;
		unsigned long			ConnectedPathNum;			//��Ч��������
		bool					bIsSignalling;
		bool					bHasMulticastTx;
	};

	class CDeviceRouting {
	public:
		CDeviceRouting()
		{
			eDeviceStatus = E_DeviceStatus::eDeviceStatusUndefined;
		};

		std::wstring	strDeviceName;
		E_DeviceStatus	eDeviceStatus;
		::std::vector<CRxChRouting>		RxRouting;
		::std::vector<CTxChRouting>		TxRouting;
	};
	typedef ::std::vector<CDeviceRouting>	DeviceRoutingList_t;
	typedef ::std::map<std::wstring,CDeviceRouting>	DeviceRoutingMap_t;

	class CConnectorStreamInfo
	{
	public:
		CConnectorStreamInfo()
		{
			m_nConnectorID = 0;
			m_nPinCount = 0;
			m_PinNumInUsed = 0;
			m_eStreamCastMode = MEDIASTREAMCASTMODE_NONE;
			m_nSessionID = 0;

			m_nPrimaryTxIp = 0;
			m_nSlaveTxIp = 0;
			m_nPrimaryTxPortNo = 0;
			m_nSlaveTxPortNo = 0;

			m_nPrimaryRxIp = 0;
			m_nSlaveRxIp = 0;
			m_nPrimaryRxPortNo = 0;
			m_nSlaveRxPortNo = 0;
			m_bConnectionError = false;
		};
		MediaStreamCastMode	m_eStreamCastMode;
		UINT16		m_nConnectorID;
		UINT16					m_nPinCount;
		UINT16					m_PinNumInUsed;
		UINT64					m_nSessionID;

		DWORD						m_nPrimaryTxIp;
		DWORD						m_nSlaveTxIp;
		WORD						m_nPrimaryTxPortNo;
		WORD						m_nSlaveTxPortNo;


		DWORD						m_nPrimaryRxIp;
		DWORD						m_nSlaveRxIp;
		WORD						m_nPrimaryRxPortNo;
		WORD						m_nSlaveRxPortNo;

		std::wstring				m_strTxDeviceName;	//���������豸
		std::wstring				m_strRxDeviceName;	//���������豸
		std::string					m_strTxDeviceNameA;	//���������豸
		std::string					m_strRxDeviceNameA;	//���������豸
		bool						m_bConnectionError;
	};
	class CTxConnectorPinInfo
	{
	public:
		CTxConnectorPinInfo()
		{
			m_nConnectorID = 0;
			m_nPinCount = 0;
			m_PinNumInUsed = 0;
			m_eStreamCastMode = MEDIASTREAMCASTMODE_NONE;
			m_nPinNo = 0;
			m_nTxChNo = -1;
			m_bIsPinInUsed = false;
		};
		MediaStreamCastMode	m_eStreamCastMode;
		UINT16		m_nConnectorID;
		UINT16					m_nPinCount;
		UINT16					m_PinNumInUsed;
		UINT16					m_nPinNo;				//��Ŵ�1��ʼ
		long					m_nTxChNo;
		bool					m_bIsPinInUsed;
		std::wstring			m_strRxDeviceName;
	};
	class CRxConnectorPinInfo
	{
	public:
		CRxConnectorPinInfo()
		{
			m_nSessionID = 0;
			m_nConnectorID = 0;
			m_nPinCount = 0;
			m_nPinMaxRxChNum = 0;
			m_nPinCurrentConnectedRxChNum = 0;
			m_eStreamCastMode = MEDIASTREAMCASTMODE_NONE;
			m_nPinNo = 0;
			m_nRxChNo = -1;
			m_nTxChNo = -1;
			m_bIsPinInUsed = false;


		};
		MediaStreamCastMode	m_eStreamCastMode;
		UINT64					m_nSessionID;
		UINT16					m_nConnectorID;
		UINT16					m_nPinCount;
		UINT16					m_nPinMaxRxChNum;
		UINT16					m_nPinCurrentConnectedRxChNum;
		UINT16					m_nPinNo;				//��Ŵ�1��ʼ
		long					m_nRxChNo;
		long					m_nTxChNo;
		bool					m_bIsPinInUsed;
		std::string				m_strRxDeviceName;
		std::string				m_strTxDeviceName;
	};
};