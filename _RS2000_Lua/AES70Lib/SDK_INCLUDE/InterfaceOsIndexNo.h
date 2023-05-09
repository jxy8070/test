#pragma once

class CInterfaceOsIndexNo
{
public:
	enum {
		DisableIf = 0			//接口禁用时所对应的接口编号	
	};

	CInterfaceOsIndexNo()
	{
		long nByteSize = sizeof(CInterfaceOsIndexNo);
		char* pByte = (char*)this;
		for (long i = 0; i < nByteSize; i++)
		{
			pByte[i] = 0;
		}
		m_bIfEnable[0] = false;
		m_bIfEnable[1] = false;
		m_OsIndex[0] = DisableIf;
		m_OsIndex[1] = DisableIf;
	};

	bool			m_bIfEnable[2];		//true ; 使用接口，false : 接口禁用
	long			m_OsIndex[2];			//OsIndex[0] = 网络主接口的操作系统编号，
									//OsIndex[1] = 网络备接口的操作系统编号


	bool	SetOsIndex(long IfNo, long OsIndex)
	{
		if (IfNo < 0) return false;
		if (IfNo > 1) return false;
		m_OsIndex[IfNo] = OsIndex;
		if (OsIndex == DisableIf)
		{
			m_bIfEnable[IfNo] = false; 
		}
		else 
		{
			m_bIfEnable[IfNo] = true;
		}
		return true;
	}

	long	GetOsIndex(long IfNo)
	{
		if (IfNo < 0) return DisableIf;
		if (IfNo > 1) return DisableIf;

		return m_OsIndex[IfNo];
	}


	long	GetInterfaceNo(long OsIfIndex)
	{
		if (OsIfIndex == DisableIf) return -1;
		if (m_OsIndex[0] == OsIfIndex) return 0;
		if (m_OsIndex[1] == OsIfIndex) return 1;

		return -1;
	}
};