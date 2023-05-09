#pragma once
#include "DeclareExportAES70ExtDLL.h"
#include "AES70DeviceSystemCfg.h"
#include "AES70ManagerDataType.h"
#include "TString.h"

class EXPORT_AES70_CLASS CAES70Device
{
public:
	CAES70Device();
	CAES70Device(const CAES70Device &Device);

	~CAES70Device();
	
	CAES70Device &operator=(const CAES70Device &Device);
	enum{
		MAX_CH_NUM	  = 128,
		ePrimaryInteraceNo = 0,
		eSlaveInterfaceNo =1,
		eDeviceInterfaceNum = AES70_STREAM_CONNECTION_PARAM_T::eMaxInterfaceNum,
	};
	enum class E_ADMIN_TYPE_T :long {
		eAdminTypeOnLineDevice = 0,
		eAdminTypeOffLineDevice,
	};
	typedef enum  {
		eNeedLoading = 0,
		eInProgress,
		eCompleteLoading,
	}E_LoadingStatus;


	enum class E_RouteStatus :WORD{
		eRouteStatus_NoSubscription = 0,	// 未订阅路由		
		eRouteStatus_Success = 1,
		eRouteStatus_Subscribing,			//正在订阅中
		eRouteStatus_TxFail,
		eRouteStatus_Num
	} ;

	enum class E_PTP1588_STATE :unsigned char {
		PTP_STATE_UNKNOWN = 0,
		PTP_STATE_INITIALIZING,
		PTP_STATE_FAULTY,
		PTP_STATE_DISABLED,
		PTP_STATE_LISTENING,
		PTP_STATE_PREMASTER,
		PTP_STATE_MASTER,
		PTP_STATE_PASSIVE,
		PTP_STATE_UNCALIBRATED,
		PTP_STATE_SLAVE,
		PTP_STATE_NUM
	};

	typedef enum class OcaFitcanPtpMode:unsigned char {
		PTP_MODE_BMC = 0,
		PTP_MODE_MASTER1,
		PTP_MODE_MASTER2,
		PTP_MODE_SLAVE_ONLY,
		PTP_MODE_External_SYNC,
		PTP_MODE_External_SYNC_MASTER1,
		PTP_MODE_External_SYNC_MASTER2,
		PTP_MODE_NUM
	}E_PTP1588_MODE;


	class CConnectorList {
	public:
		CConnectorList()
		{
			m_nConnectorUpdateVersion = 0;	//用来记录路由更新的次数
		};

		AES70_TX_CONNECTORS_T	m_TxConnectorVector;
		AES70_RX_CONNECTORS_T	m_RxConnectorVector;
		DWORD					m_nConnectorUpdateVersion;		//路由更新的版本号，用来记录路由更新的次数。在显示路由时可以比对当前显示的版本号是否相同从而决定是否进行刷新，提高效率。
																//0表示路由未更新，为内部保留，正常编号从1开始。2020-08-20增加
	};

	class CRouteTableItem{
	public:
		char	strTxDeviceName[32];	//发送信号的设备名称（即信号源）
		short	TxChNo;					//发送信号的通道编号，编号从0开始，最大值目前是63，-1表时AES70设备没有订阅信号源
										//在AES70中 0 表示没有信号源
		E_RouteStatus	RouteStatus;
	};

	class CRouteTableHeader{
	public:
		WORD	SyncCode;
		long	ChNum;
	};


	class CRouteTable{
	public:
		enum{
			SYNC_CODE = 0xcc55
		};
		CRouteTable();
		bool operator==(CRouteTable &Table);
		bool operator!=(CRouteTable &Table);
		void MarkStringNullEnd(void);	//确保字符串是有NULL结束符号
		CRouteTableHeader TableHeader;
		CRouteTableItem	  TableItem[MAX_CH_NUM];
	};

	//设备的固件信息
	class CFirmwareInfo{
	public:
		CFirmwareInfo();
		std::wstring			m_strDeviceNameU;		//Unicode名称	
		std::string			m_strDeviceNameUTF_8;	//UTF-8格式名称
		long					m_nDevicType;
		DWORD				m_nDeviceID;
		unsigned char			m_MAC_mDNS[2][6];		//设备的主从网络接口的MAC地址
		DWORD				m_nIp_mDNS[2];		//由mDNS解析获得的IP地址
		WORD					m_nPortNo_mDNS;		//由mDNS解析获得的服务端口号

		DWORD				m_nIpService[2];		//这是浏览器发现的oca服务的IP地址
		WORD					m_nPortService[2];		//这是浏览器发现的oca服务的端口地址

		CDeviceSystemCfg		m_StreamCfg;
		long					m_nSubChNum;			//每个频道的子通道个数，立体声 = 2，单声道 = 1
	};

	class CPtpClockInfo{
		public:
			//PTP时钟状态
			CPtpClockInfo();
			E_PTP1588_MODE	m_ePTPmode;
			E_PTP1588_STATE	m_ePtpClockStateNo;
			//=================================
			INT32			m_nClockOffset;
			
			std::wstring		GetPTPStateText(E_PTP1588_STATE StateNo) const;
			std::wstring		GetPTPModeText(E_PTP1588_MODE ModeNo) const;

	};

	static std::wstring m_strInterfaceStatus[(unsigned long)E_InterfaceStatus::eInterfaceStatusNum];
	static std::wstring m_strDeviceStatus[(unsigned long)E_DeviceStatus::eDeviceStatusNum];
	static std::wstring m_strRouteStatus[(unsigned long)E_RouteStatus::eRouteStatus_Num];

	static const std::wstring m_strPtpState[(long)E_PTP1588_STATE::PTP_STATE_NUM];
	static const std::wstring m_strPtpMode[(long)E_PTP1588_MODE::PTP_MODE_NUM];

	class CDevInfo{
	public:
		CFirmwareInfo			m_FirmwareInfo;
		E_DeviceStatus		m_eDeviceStatus;						//设备的在线状态
		E_InterfaceStatus		m_eInterfaceStatus[eDeviceInterfaceNum];	//设备接口的在线状态
		DWORD				m_nNeedToUpdateRouteTableCounter;		//设备的路由更新状态计数器。每次触发线程更新路由时，此计数器就会自动加1，表示在过去的时间内有多少次请求更新。
																//每次从设备读取路由后，若m_nNeedToUpdateRouteTableCounter保持不变，则说明是读到了当前最新的有效路由。
																//否则还要继续重新从设备读取最新的路由。
		E_LoadingStatus		m_eLoadingCfgStatus;
		CRouteTable			m_RouteTable;					//存放设备的路由表
		CPtpClockInfo			m_ptpClockInfo;
		DWORD				m_nIpKeepAliveMsg;			//发送心跳包的IP地址
		WORD					m_nPortKeepAliveMsg;			//发送心跳包的IP地址的端口号
		ULONGLONG			m_nTimeReceiveKeepAliveMsg[eDeviceInterfaceNum];			//最近一次收到心跳包的时间
		ULONGLONG			m_nTimeReceivePreviousKeepAliveMsg[eDeviceInterfaceNum];	//上一次收到心跳包的时间，用来跟踪调试心跳包用

		bool					m_bPtpClockInfoIsLoaded;
		bool					m_bChannelSignallingStatusIsLoaded;
		DWORD				m_nSubscriptionStatusBit;				//设备注册订阅事件消息的状态位，定义见E_DeviceEventBit_T，2020-08-16修改增订
		DWORD_PTR			m_nListID;
		DWORD				m_RxAuidoSilenceDetection[8];			//提供32*8 = 256个单声道的静音状态
		DWORD				m_TxAuidoSilenceDetection[8];
//		bool					m_bCfgIsLoaded;
		//=================================================
		//调试用追踪用信息
		long					m_DebugMsgNum;
		long					m_DebugBrowsingNum;
		E_ADMIN_TYPE_T		m_eAdminType;
	};

	CDevInfo					m_Info;
	CConnectorList			m_cConnectorList;	//设备流连接器缓存，2020-08-31修订
	CRITICAL_SECTION			m_csLock;

	DWORD_PTR	GetListID(void) const {	return m_Info.m_nListID; };
	void			SetListID(DWORD_PTR ListID) { m_Info.m_nListID = ListID; };

	ULONGLONG	GetAliveTime(long InterfaceNo);
	void		SetAliveTime(long InterfaceNo, ULONGLONG tAlive,bool bSetFisrtTime);

	static std::wstring		GetDeviceStatus(E_DeviceStatus eDeviceStatus)	{ return m_strDeviceStatus[(long)eDeviceStatus]; };
	static std::wstring		GetRouteStatus(E_RouteStatus eRouteStatus)		{ return m_strRouteStatus[(long)eRouteStatus]; };
	bool		IsAlive(void);
	void		InitialSubscription(void);
	void		SetRouteTable(CRouteTable &Table);
	void		GetRouteTable(CRouteTable &Table);
	long		GetMaxDeviceChNum(void) const { return MAX_CH_NUM; };

	std::wstring	GetDeviceNameU(void)	const { return m_Info.m_FirmwareInfo.m_strDeviceNameU; }
	std::string	GetDeviceNameA(void)	const { return m_Info.m_FirmwareInfo.m_strDeviceNameUTF_8; }
	TString		GetDeviceNameT(void);
	void			SetDeviceNameT(const TString& strDeviceNameT);

	long			GetSubChNum(void)		const {	return m_Info.m_FirmwareInfo.m_nSubChNum;	}
	//设备状态参数 
	E_DeviceStatus	GetDeviceStatus(void) const { return m_Info.m_eDeviceStatus; };
	void				SetDeviceStatus(E_DeviceStatus eDeviceStatus) { m_Info.m_eDeviceStatus = eDeviceStatus; };


	//网络接口的配置参数
	static std::wstring	GetInterfaceStatus(E_InterfaceStatus eInterfaceStatus);
	E_InterfaceStatus		GetInterfaceStatus(long InterfaceNo) const;
	void					SetInterfaceStatus(long InterfaceNo, E_InterfaceStatus eInterfaceStatus);
	void					ClearInterfaceConnection(long InterfaceNo);

	//频道信号状态
	bool		IsRxChSignalling(long RxChNo) const;
	bool		IsTxChSignalling(long TxChNo) const;
	void		SetTxSignalling(long TxChNo,bool bSignalling);


	unsigned char *GetMacCfg(long InterfaceNo);
	const unsigned char* GetMacCfg(long InterfaceNo) const;

	DWORD	GetIpCfg(long InterfaceNo) const;
	WORD		GetServicePortNoCfg(void) const;

	//约定Ip = 0 表示当前连接不存在
	long		SelectActiveInterfaceNo(E_COM_INTERFACE SelIfMode = E_COM_INTERFACE::eAutoSelectInterface);
	DWORD	GetInterfaceIp(long InterfaceNo) const;
	WORD		GetInterfacePortNo(long InterfaceNo) const;

	DWORD	GetDeviceIp(bool bIsPrimaryIp = true) const;
	WORD		GetDevicePortNo(bool bIsPrimaryIp) const;

	bool GetStreamCfg(CDeviceSystemCfg	&StreamCfg)
	{
		StreamCfg = m_Info.m_FirmwareInfo.m_StreamCfg;
		return m_Info.m_FirmwareInfo.m_StreamCfg.IsStreamCfgSetup();
	};

	void		SetupStreamCfg(const CDeviceSystemCfg &StreamCfg) {
		m_Info.m_FirmwareInfo.m_StreamCfg = StreamCfg;
		m_Info.m_FirmwareInfo.m_StreamCfg.SetStreamCfgFlag(true);
	};

	//读流配置属性
	UINT32		GetMediaTransportNetworkONo(void) const { return  m_Info.m_FirmwareInfo.m_StreamCfg.GetMediaTransportNetworkONo(); };
	UINT32		GetSilenceDetectorONo(void) const { return  m_Info.m_FirmwareInfo.m_StreamCfg.GetSilenceDetectorONo(); };
	UINT32		GetDeviceNetworkONo(void) const { return  m_Info.m_FirmwareInfo.m_StreamCfg.GetDeviceNetworkONo(); };

	UINT16		GetMaxSourceConnectors(void) const { return  m_Info.m_FirmwareInfo.m_StreamCfg.GetMaxSourceConnectors(); };
	UINT16		GetMaxSinkConnectors(void) const { return  m_Info.m_FirmwareInfo.m_StreamCfg.GetMaxSinkConnectors(); };
	UINT16		GetMaxPinsPerConnector(void) const{ return m_Info.m_FirmwareInfo.m_StreamCfg.GetMaxPinsPerConnector(); };
	UINT16		GetMaxPortsPerPin(void) const{ return m_Info.m_FirmwareInfo.m_StreamCfg.GetMaxPortsPerPin(); };

	std::vector<AES70_PORT_CFG_T>& GetTxPort(void) { return m_Info.m_FirmwareInfo.m_StreamCfg.GetTxPort(); };
	const std::vector<AES70_PORT_CFG_T>& GetTxPort(void) const { return m_Info.m_FirmwareInfo.m_StreamCfg.GetTxPort(); };

	std::vector<AES70_PORT_CFG_T>& GetRxPort(void) { return m_Info.m_FirmwareInfo.m_StreamCfg.GetRxPort(); };
	const std::vector<AES70_PORT_CFG_T>& GetRxPort(void) const { return m_Info.m_FirmwareInfo.m_StreamCfg.GetRxPort(); };



	UINT16		GetRxChNum(void) const { return m_Info.m_FirmwareInfo.m_StreamCfg.GetRxChNum(); };
	UINT16		GetTxChNum(void) const { return  m_Info.m_FirmwareInfo.m_StreamCfg.GetTxChNum(); };
	UINT16		GetFirstRxChNo(void) const { return m_Info.m_FirmwareInfo.m_StreamCfg.GetFirstRxChNo(); };
	UINT16		GetFirstTxChNo(void) const { return  m_Info.m_FirmwareInfo.m_StreamCfg.GetFirstTxChNo(); };

	UINT32		GetMediaClock3ONo(void) const;
	void			GetMediaClock3ONoList(std::vector<UINT32>	&MediaClock3List) const {  m_Info.m_FirmwareInfo.m_StreamCfg.GetMediaClock3ONoList(MediaClock3List); };

	CAES70Device::E_PTP1588_MODE	GetPtpMode(void) const { return m_Info.m_ptpClockInfo.m_ePTPmode; };
	E_PTP1588_STATE	GetPtp1588ClockStateNo(void) const { return m_Info.m_ptpClockInfo.m_ePtpClockStateNo; };
	std::wstring		GetPtp1588ClockStateText(E_PTP1588_STATE StateNo) const { return m_Info.m_ptpClockInfo.GetPTPStateText(StateNo); };
	std::wstring		GetPtp1588ClockModeText(E_PTP1588_MODE ModeNo) const { return m_Info.m_ptpClockInfo.GetPTPModeText(ModeNo); };

	INT32	GetPtp1588ClockOffset(void) const { return m_Info.m_ptpClockInfo.m_nClockOffset; };

	void		SetPTP1588ClockInfo(CPtpClockInfo &ClockInfo){ m_Info.m_ptpClockInfo = ClockInfo; };

	bool		IsStreamCfgSetup(void) const	
	{ return  m_Info.m_FirmwareInfo.m_StreamCfg.IsStreamCfgSetup(); };

	bool		IsDeviceMacEqual(unsigned char *mac,long InterfaceNo);
	bool		IsDeviceMacNull(long InterfaceNo);
	static	bool		IsMacNull(unsigned char *mac);


	//读流连接器
	//=========================================================
	// Helper function
	bool	GetRxConnector(UINT16 RxConnectorID, AES70_RX_CONNECTOR_T &RxConnector);
	bool	GetRxConnectorBySessionID(UINT64 SessionID, AES70_RX_CONNECTOR_T &RxConnector);

	bool	GetTxConnector(UINT16 TxConnectorID, AES70_TX_CONNECTOR_T &TxConnector);
	bool	GetTxConnectorBySessionID(UINT64 SessionID, AES70_TX_CONNECTOR_T &TxConnector);



	bool	GetRxConnectors(AES70_RX_CONNECTORS_T &RxConnectors, bool bGetMulticastOnly) const;
	bool	GetTxConnectors(AES70_TX_CONNECTORS_T &TxConnector, bool bGetMulticastOnly)  const;

	bool	GetRxPortConnection(UINT16 PortNo, AES70_RX_CONNECTOR_T &RxConnector, AES70_CONNECTOR_PIN_NO_T &RxPinNo);
	bool	GetTxPortConection(UINT16 TxChNo, AES70_TX_CONNECTORS_T &TxConnectors, std::vector<AES70_CONNECTOR_PIN_NO_T> &TxPins) const;

	bool	IsTxMulticastExist(DWORD nIp, WORD nPortNo, AES70_TX_CONNECTOR_T *pTxConnector = NULL) const;
	//虚拟组播源设备管理
	E_ADMIN_TYPE_T GetAdminType(void) const { return m_Info.m_eAdminType; };
	void SetAdminType(E_ADMIN_TYPE_T eAdminType) { m_Info.m_eAdminType = eAdminType; };
};

