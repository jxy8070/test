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
		eNoRxPathAssigned = 0,			//频道违被启用
		eNoRxError,						//路由正常，对应与RxPathAssignedNum = 1，ConnectingTxSourceNum=0,1情形
		eAssignMutipleRxPathError,		//切换路由异常，对应与RxPathAssignedNum>1的情形
		eConnectMutipleTxSourcesError,	//切换路由异常，对应与ConnectingTxSourceNum > 1 的情形
		eTxDeviceOffLine,				//信号源设备下线
		eTxStreamPinMisMatch,			//发送源不存在与接收频道配对的流引脚
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
		UINT16	RxConnectorPinNo;				//PinNo编号从1开始
		UINT64	SessionID;
		UINT16	RxStreamPortNo;
		UINT16		RxConnectorID;
		long		RxPathAssignedNum;			//记录连接器分配的路径的个数，正常时应该为0或是1
		long		ConnectingTxSourceNum;				//当前路径所连接的信号源的个数，0表示无连接，1表示正常连接，大于1表示系统有垃圾路由
		long		TxDeviceNo;							//建立设备的数组索引，用于路径的快速搜索
	};

	typedef enum {
		eNoTxPathAssigned = 0,							//发送频道未被启用，对应与TxPathAssignedNum = 0情形
		eNoTxError,										//信号连接正常
		ePartialRxConnectionMissingError,				//发送路由部分悬空，对应与TxPathAssignedNum>TxPathConnectedNum>1情形
		eNoRxConnection,								//发送信号悬空，未被接收端引用，对应与TxPathConnectedNum>1，TxPathConnectedNum=0
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
				TxConnectorPinNo = 0;				//PinNo编号从1开始
				TxConnectorID = 0;
				TxSessionID = 0;
			};
			unsigned long				ConnectedNumByRxCh;	//记录当前路径的接收端连接数
			long						TxChNo;					//频道编号
			UINT16		TxConnectorPinNo;
			UINT16		TxConnectorID;		//频道所连接的一个连接器ID
			UINT64		TxSessionID;
			std::wstring			RxDevicName;
		};

		long			TxChNo;
		::std::vector<CTxPath>	TxPathList;
		TxRoutingStatus_e		eTxRoutingStatus;
		unsigned long			TotalConnectedNumByRxCh;
		unsigned long			ConnectedPathNum;			//有效的连接数
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

		std::wstring				m_strTxDeviceName;	//发送流的设备
		std::wstring				m_strRxDeviceName;	//接收流的设备
		std::string					m_strTxDeviceNameA;	//发送流的设备
		std::string					m_strRxDeviceNameA;	//接收流的设备
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
		UINT16					m_nPinNo;				//编号从1开始
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
		UINT16					m_nPinNo;				//编号从1开始
		long					m_nRxChNo;
		long					m_nTxChNo;
		bool					m_bIsPinInUsed;
		std::string				m_strRxDeviceName;
		std::string				m_strTxDeviceName;
	};
};