#pragma once
#include <windows.h>
#include "DeclareExportAES70ExtDLL.h"


class EXPORT_AES70_CLASS CAppNetIfCfg
{
public:
	CAppNetIfCfg();
	~CAppNetIfCfg();

	enum{ 
		eMajorVersion	  = 1,	//主要版本号
		eMinorVersion	  = 0,	//次要版本号

		DisableIf = 0			//接口禁用时所对应的接口编号	
	};

private:
	unsigned long	m_CRC_CheckSum;		//必须放在第一个位置
	long			m_MajorVersion;
	long			m_MinorVersion;

	bool			m_bIfEnable[2];		//true ; 使用接口，false : 接口禁用
	long			m_OsIndex[2];			//OsIndex[0] = 网络主接口的操作系统编号，
									//OsIndex[1] = 网络备接口的操作系统编号
public:
	bool	SetOsIndex(long IfNo,long OsIndex);
	long	GetOsIndex(long IfNo);
	long	GetIfNo(long OsIfIndex);
	bool	ReadOsIndexCfgFromFile(LPCTSTR strOsIndexCfgFileName);	//读配置文件
	bool	WriteOsIndexCfgToFile(LPCTSTR strOsIndexCfgFileName);	//写配置文件
private:
	unsigned long	Calculate_CRC_CheckSum(void);
};


