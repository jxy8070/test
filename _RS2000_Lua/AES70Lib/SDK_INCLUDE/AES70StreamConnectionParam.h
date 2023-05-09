#pragma once
//2020-06-12����
#include "DeclareExportAES70ExtDLL.h"

#include <windows.h>
#include <string>

class  AES70_STREAM_CONNECTION_PARAM_T{
public:
	enum {
		eMaxInterfaceNum = 2,
	};

	EXPORT_AES70_CLASS AES70_STREAM_CONNECTION_PARAM_T();
	EXPORT_AES70_CLASS bool operator==(const ::AES70_STREAM_CONNECTION_PARAM_T& src) const;
	EXPORT_AES70_CLASS	void	SetTxDeviceName(const char *strTxDeviceName);						//������Ƕ��ֽ��ַ����������Զ�ת��UTF-8�ַ�����Unicode�ַ�������
	EXPORT_AES70_CLASS	void	SetTxDeviceName_Unicode(const wchar_t *strTxDeviceName_Unicode);	//�������Unicode���ַ����������Զ�ת��UTF-8�ַ�����Unicode�ַ�������
	EXPORT_AES70_CLASS	void	SetTxDeviceName_UTF_8(const char *strTxDeviceName_UTF_8);			//�������UFT_8�ַ����������Զ�ת��UTF-8�ַ�����Unicode�ַ�������

	EXPORT_AES70_CLASS	void	SetRxDeviceName(const char    *strRxDeviceName);
	EXPORT_AES70_CLASS	void	SetRxDeviceName_Unicode(const wchar_t *strRxDeviceName_Unicode);
	EXPORT_AES70_CLASS	void	SetRxDeviceName_UTF_8(const char    *strRxDeviceName_UTF_8);



public:
	
	bool				m_bIsMasterInterfaceActive;			//true = ����Ƶ�����ӵ�ַ�Ͷ˿ں���Ч��
	bool				m_bIsSlaveInterfaceActive;			//true = ������Ƶ�����ӵ�ַ�Ͷ˿ں���Ч����Ӧ��˫���ڵ����Ρ�
	unsigned char		m_timeToLive;
	unsigned char		m_payloadType;
	unsigned __int64	m_nSessionVersion;
	unsigned __int64	m_nSessionID;						//ÿ�δ����µĻỰ��Ƶ��ʱ,�ûỰID�ͷ����仯,�ڸ��豸����Ψһ�ı�ʶID
															//����������ʱ����m_nSessionID�������豸�Զ������ڸ��豸��Ψһ�ĻỰID
	unsigned _int32		m_nSrcIpAddress[eMaxInterfaceNum];	//��/��������Ƶ����ԴʼIP��ַ�����͵�ַ��
	unsigned _int32		m_nDstIpAddress[eMaxInterfaceNum];	//��/��������Ƶ����Ŀ��IP��ַ�����յ�ַ��
	unsigned _int16		m_nSrcIpPortNo[eMaxInterfaceNum];	//��/��������Ƶ����Դʼ�˿ںţ����Ͷ˿ںţ�������Լ��ʹ��AES70����˿ںš�
															//ͬһ̨������������ж������AES70�豸��ÿ���豸��AES70����˿ںŸ�����ͬ
	unsigned _int16		m_nDstIpPortNo[eMaxInterfaceNum];	//��/��������Ƶ����Ŀ��˿ںţ����ն˿ںţ�
															//����������ʱ�������ն˿ں�Ϊ0�������豸�Զ�����
	std::string			m_strTxDeviceName_UTF_8;			//UTF_8����
	std::string			m_strRxDeviceName_UTF_8;			//UTF_8����
	std::wstring		m_strTxDeviceName;					//Unicode����
	std::wstring		m_strRxDeviceName;					//Unicode����
} ;
