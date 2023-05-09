#pragma once
#include <windows.h>
#include "DeclareExportAES70ExtDLL.h"


class EXPORT_AES70_CLASS CAppNetIfCfg
{
public:
	CAppNetIfCfg();
	~CAppNetIfCfg();

	enum{ 
		eMajorVersion	  = 1,	//��Ҫ�汾��
		eMinorVersion	  = 0,	//��Ҫ�汾��

		DisableIf = 0			//�ӿڽ���ʱ����Ӧ�Ľӿڱ��	
	};

private:
	unsigned long	m_CRC_CheckSum;		//������ڵ�һ��λ��
	long			m_MajorVersion;
	long			m_MinorVersion;

	bool			m_bIfEnable[2];		//true ; ʹ�ýӿڣ�false : �ӿڽ���
	long			m_OsIndex[2];			//OsIndex[0] = �������ӿڵĲ���ϵͳ��ţ�
									//OsIndex[1] = ���籸�ӿڵĲ���ϵͳ���
public:
	bool	SetOsIndex(long IfNo,long OsIndex);
	long	GetOsIndex(long IfNo);
	long	GetIfNo(long OsIfIndex);
	bool	ReadOsIndexCfgFromFile(LPCTSTR strOsIndexCfgFileName);	//�������ļ�
	bool	WriteOsIndexCfgToFile(LPCTSTR strOsIndexCfgFileName);	//д�����ļ�
private:
	unsigned long	Calculate_CRC_CheckSum(void);
};


