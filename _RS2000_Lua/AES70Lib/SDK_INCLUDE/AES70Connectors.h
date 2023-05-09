#pragma once
#include <windows.h>
#include <vector>
#include <map>
#include <string>
#include "DeclareExportAES70ExtDLL.h"
#include "AES70StreamConnectionParam.h"

//================================================
//  定义流连接器内部使用的参数
//================================================
typedef UINT16 AES70_CONNECTOR_ID_T;			//流连接器ID，从1开始编号
typedef UINT16 AES70_CONNECTOR_PIN_NO_T;		//流连接器引脚的编号，从1开始编号。0是无效的引脚编号
typedef UINT16 AES70_CONNECTOR_PORT_NO_T;		//流连接器引脚连接的频道编号，从1开始编号。0是无效的频道编号。
typedef UINT64 AES70_SESSION_ID_T;				//流会话ID号，从1开始编号
typedef UINT32 AES70_ONO_T;						//OcaONo，从1开始编号


/**
 * Enumeration of media connector states
 */
typedef enum:unsigned char
{
	/** Connector has no media connection and no media data is
		being transferred. */
	AES70_CONNECTOR_STATE_STOPPED = 0,
	/** Stream connection is being set up.  Media data is
		not flowing. */
	AES70_CONNECTOR_STATE_SETTING_UP = 1,
	/** Media data is flowing since the connection is
		established. */
	AES70_CONNECTOR_STATE_RUNNING = 2,
	/** Media transfer is stopped. Existing connection
		is intact. */
	AES70_CONNECTOR_STATE_PAUSED = 3,
	/** Data transfer has been halted due to errors.
		Working storage has not been freed. */
	AES70_CONNECTOR_STATE_FAULT = 4,
	/** Used for range checking */
	AES70_CONNECTOR_STATE_MAXIMUM
} AES70_CONNECTOR_STATE_T;

typedef enum {
	eConnectorIsOrigin = 0,
	eConnectorIsModified,
	eConnectorIsDeleted,
	eConnectorIsNew,
	eConnectorIsMulticastNew,

} AES70_CONNECTOR_MODE_T;



typedef enum {
	eAes70PortUndefined = 0,
	eAes70InputPort,
	eAes70OutputPort,
} AES70_PORT_TYPE_T;

typedef struct tagAes70IpAddress {
	DWORD	nIp;
	WORD	nPortNo;
}AES70_IP_ADDRESS;


class AES70_PORT_ID_T {
public:
	AES70_PORT_ID_T()
	{
		m_ePortType = AES70_PORT_TYPE_T::eAes70PortUndefined;
		m_nPortNo = 0;
	};
	AES70_PORT_TYPE_T			m_ePortType;
	AES70_CONNECTOR_PORT_NO_T	m_nPortNo;			//端口编号,0为内部保留使用，编号从1开始
} ;



class AES70_PORT_CFG_T {
public:
	AES70_PORT_ID_T			m_tPortID;
	std::string				m_strName_UTF_8;	//端口名称，使用UTF8编码格式
} ;



typedef std::map<AES70_CONNECTOR_PIN_NO_T, AES70_PORT_ID_T>			AES70_TX_PIN_MAP_T;
typedef std::multimap<AES70_CONNECTOR_PIN_NO_T, AES70_PORT_ID_T>	AES70_RX_PIN_MAP_T;

class AES70_CONNECTOR_CODEC_PARAM_T {
public:
	AES70_CONNECTOR_CODEC_PARAM_T()
	{
		Clear();
	};
	void Clear(void)
	{
		m_Clock3ONo = 0;
		m_CodingSchemeID = 0;
		for (long i = 0; i < sizeof(m_CodecParameters); i++)
		{
			m_CodecParameters[i] = 0;
		}
		m_CodecParameters[0] = 'L';
		m_CodecParameters[1] = '2';
		m_CodecParameters[2] = '4';
	}
	unsigned _int32		m_Clock3ONo;
	unsigned _int16		m_CodingSchemeID;
	char				m_CodecParameters[16];
} ;


//定义发送流连接器的内部参数
class  AES70_TX_CONNECTOR_T {
public:
	AES70_TX_CONNECTOR_T()
	{
		Clear();
	};
	void Clear(void)
	{
		m_eConnnetorMode = AES70_CONNECTOR_MODE_T::eConnectorIsOrigin;
		m_bIsMulticast	= false;
		m_nConnectorID	= 0;
//		m_nPinNumInUsed = 0;
		m_nPinCount		= 0;
	}
	AES70_CONNECTOR_MODE_T	m_eConnnetorMode;			//在进行路由连接命令编辑时，用来记录连接器的编辑状态
	bool					m_bIsMulticast;				//true = 是组播流连接器，false = 是单播流连接器
	AES70_CONNECTOR_ID_T	m_nConnectorID;				//连接器ID编号。0 是系统内部保留ID编号，不可用。
	std::string				m_strIDExternal;			//用作显示的连接器名称
	unsigned __int16		m_nPinCount;				//连接器的流使用的总时隙数
//	unsigned __int16		m_nPinNumInUsed;			//连接器的流非空闲的时隙数
	AES70_STREAM_CONNECTION_PARAM_T	m_cConnectionParam;	//流连接相关的参数
	AES70_CONNECTOR_CODEC_PARAM_T	m_cCodecParam;		//编码器相关的参数
	AES70_TX_PIN_MAP_T				m_cPinMap;			//连接器的引脚和频道的连接关系
	unsigned __int16 GetPinCount() const
	{
		return m_nPinCount;				//注意：这是连接器的当前流的引脚数
	}	
	
	unsigned __int16 GetPinNumInUsed(void) const
	{
		return (unsigned _int16)m_cPinMap.size();
	}

	EXPORT_AES70_CLASS bool IsPinInUsed(AES70_CONNECTOR_PIN_NO_T PinNo, AES70_CONNECTOR_PORT_NO_T *pPortNo = NULL) const;
	EXPORT_AES70_CLASS bool IsPortConnectedToPin(AES70_CONNECTOR_PORT_NO_T PortNo , AES70_CONNECTOR_PIN_NO_T *pPinNo= NULL) const;

	EXPORT_AES70_CLASS bool RemovePinNo(AES70_CONNECTOR_PIN_NO_T PinNo, AES70_CONNECTOR_PORT_NO_T *pPortNo = NULL);
	EXPORT_AES70_CLASS bool AddPortNo(AES70_CONNECTOR_PIN_NO_T PinNo, AES70_CONNECTOR_PORT_NO_T PortNo);
	EXPORT_AES70_CLASS bool ContainsKey(const AES70_CONNECTOR_PIN_NO_T& key) const;
	EXPORT_AES70_CLASS AES70_CONNECTOR_PIN_NO_T GetIdlePinNo(void);
	EXPORT_AES70_CLASS bool GetIdlePinNo(std::vector<AES70_CONNECTOR_PIN_NO_T> &PinNo);

	EXPORT_AES70_CLASS bool GetPinConnectionMap(std::vector<AES70_CONNECTOR_PORT_NO_T> &PinMap);



} ;


//定义接收流连接器的内部参数

class  AES70_RX_CONNECTOR_T {
public:
	AES70_RX_CONNECTOR_T()
	{
		Clear();
	};

	void Clear(void)
	{
		m_eConnnetorMode = AES70_CONNECTOR_MODE_T::eConnectorIsOrigin;
		m_bIsMulticast	= false;
		m_nConnectorID	= 0;
//		m_nPinNumInUsed = 0;
		m_nPinCount = 0;
	}
	AES70_CONNECTOR_MODE_T	m_eConnnetorMode;			//在进行路由连接命令编辑时，用来记录连接器的编辑状态
	bool					m_bIsMulticast;				//true = 是组播流连接器，false = 是单播流连接器
	unsigned __int16		m_nConnectorID;				//连接器ID编号。0 是系统内部保留ID编号，不可用。
	std::string				m_strIDExternal;			//用作显示的连接器名称	
	unsigned __int16		m_nPinCount;				//连接器的流具有的总时隙数
//	unsigned __int16		m_nPinNumInUsed;			//连接器的流非空闲的时隙数

	AES70_STREAM_CONNECTION_PARAM_T	m_cConnectionParam;	//流连接的参数
	AES70_CONNECTOR_CODEC_PARAM_T	m_cCodecParam;
	AES70_RX_PIN_MAP_T				m_cPinMap;
	unsigned __int16 GetPinCount() const
	{
		return m_nPinCount;								//注意：这是连接器的当前流的引脚数
	}
	unsigned __int16 GetPinNumInUsed(void) const
	{
		return (unsigned _int16)m_cPinMap.size();
	}
	EXPORT_AES70_CLASS  bool DisConnectPortNo(AES70_CONNECTOR_PORT_NO_T RxPortNo,AES70_CONNECTOR_PIN_NO_T *pDisConnectedPinNo = NULL);
	EXPORT_AES70_CLASS  bool IsPortConnectedToPin(AES70_CONNECTOR_PORT_NO_T PortNo,AES70_CONNECTOR_PIN_NO_T *pPinNo = NULL ) const;
	EXPORT_AES70_CLASS  bool IsPinInUsed(AES70_CONNECTOR_PIN_NO_T PinNo, ::std::vector<AES70_CONNECTOR_PORT_NO_T> &ConnectedPorts) const;
	EXPORT_AES70_CLASS	bool AddPortNo(AES70_CONNECTOR_PIN_NO_T PinNo,  AES70_CONNECTOR_PORT_NO_T PortNo);
	EXPORT_AES70_CLASS  UINT16 GetKeyValuePairNum(const AES70_CONNECTOR_PIN_NO_T &Key) const;
} ;


typedef std::vector<AES70_TX_CONNECTOR_T>	AES70_TX_CONNECTORS_T;
typedef std::vector<AES70_RX_CONNECTOR_T>	AES70_RX_CONNECTORS_T;

