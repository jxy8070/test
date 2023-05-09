#pragma once
#include <windows.h>
#include <vector>
#include <map>
#include <string>
#include "DeclareExportAES70ExtDLL.h"
#include "AES70StreamConnectionParam.h"

//================================================
//  �������������ڲ�ʹ�õĲ���
//================================================
typedef UINT16 AES70_CONNECTOR_ID_T;			//��������ID����1��ʼ���
typedef UINT16 AES70_CONNECTOR_PIN_NO_T;		//�����������ŵı�ţ���1��ʼ��š�0����Ч�����ű��
typedef UINT16 AES70_CONNECTOR_PORT_NO_T;		//���������������ӵ�Ƶ����ţ���1��ʼ��š�0����Ч��Ƶ����š�
typedef UINT64 AES70_SESSION_ID_T;				//���ỰID�ţ���1��ʼ���
typedef UINT32 AES70_ONO_T;						//OcaONo����1��ʼ���


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
	AES70_CONNECTOR_PORT_NO_T	m_nPortNo;			//�˿ڱ��,0Ϊ�ڲ�����ʹ�ã���Ŵ�1��ʼ
} ;



class AES70_PORT_CFG_T {
public:
	AES70_PORT_ID_T			m_tPortID;
	std::string				m_strName_UTF_8;	//�˿����ƣ�ʹ��UTF8�����ʽ
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


//���巢�������������ڲ�����
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
	AES70_CONNECTOR_MODE_T	m_eConnnetorMode;			//�ڽ���·����������༭ʱ��������¼�������ı༭״̬
	bool					m_bIsMulticast;				//true = ���鲥����������false = �ǵ�����������
	AES70_CONNECTOR_ID_T	m_nConnectorID;				//������ID��š�0 ��ϵͳ�ڲ�����ID��ţ������á�
	std::string				m_strIDExternal;			//������ʾ������������
	unsigned __int16		m_nPinCount;				//����������ʹ�õ���ʱ϶��
//	unsigned __int16		m_nPinNumInUsed;			//�����������ǿ��е�ʱ϶��
	AES70_STREAM_CONNECTION_PARAM_T	m_cConnectionParam;	//��������صĲ���
	AES70_CONNECTOR_CODEC_PARAM_T	m_cCodecParam;		//��������صĲ���
	AES70_TX_PIN_MAP_T				m_cPinMap;			//�����������ź�Ƶ�������ӹ�ϵ
	unsigned __int16 GetPinCount() const
	{
		return m_nPinCount;				//ע�⣺�����������ĵ�ǰ����������
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


//������������������ڲ�����

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
	AES70_CONNECTOR_MODE_T	m_eConnnetorMode;			//�ڽ���·����������༭ʱ��������¼�������ı༭״̬
	bool					m_bIsMulticast;				//true = ���鲥����������false = �ǵ�����������
	unsigned __int16		m_nConnectorID;				//������ID��š�0 ��ϵͳ�ڲ�����ID��ţ������á�
	std::string				m_strIDExternal;			//������ʾ������������	
	unsigned __int16		m_nPinCount;				//�������������е���ʱ϶��
//	unsigned __int16		m_nPinNumInUsed;			//�����������ǿ��е�ʱ϶��

	AES70_STREAM_CONNECTION_PARAM_T	m_cConnectionParam;	//�����ӵĲ���
	AES70_CONNECTOR_CODEC_PARAM_T	m_cCodecParam;
	AES70_RX_PIN_MAP_T				m_cPinMap;
	unsigned __int16 GetPinCount() const
	{
		return m_nPinCount;								//ע�⣺�����������ĵ�ǰ����������
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

