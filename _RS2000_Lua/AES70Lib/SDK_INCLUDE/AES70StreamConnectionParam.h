#pragma once
//2020-06-12新增
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
	EXPORT_AES70_CLASS	void	SetTxDeviceName(const char *strTxDeviceName);						//输入的是多字节字符串，函数自动转成UTF-8字符串和Unicode字符串保存
	EXPORT_AES70_CLASS	void	SetTxDeviceName_Unicode(const wchar_t *strTxDeviceName_Unicode);	//输入的是Unicode节字符串，函数自动转成UTF-8字符串和Unicode字符串保存
	EXPORT_AES70_CLASS	void	SetTxDeviceName_UTF_8(const char *strTxDeviceName_UTF_8);			//输入的是UFT_8字符串，函数自动转成UTF-8字符串和Unicode字符串保存

	EXPORT_AES70_CLASS	void	SetRxDeviceName(const char    *strRxDeviceName);
	EXPORT_AES70_CLASS	void	SetRxDeviceName_Unicode(const wchar_t *strRxDeviceName_Unicode);
	EXPORT_AES70_CLASS	void	SetRxDeviceName_UTF_8(const char    *strRxDeviceName_UTF_8);



public:
	
	bool				m_bIsMasterInterfaceActive;			//true = 主音频流连接地址和端口号有效。
	bool				m_bIsSlaveInterfaceActive;			//true = 备份音频流连接地址和端口号有效。对应于双网口的情形。
	unsigned char		m_timeToLive;
	unsigned char		m_payloadType;
	unsigned __int64	m_nSessionVersion;
	unsigned __int64	m_nSessionID;						//每次创建新的会话音频流时,该会话ID就发生变化,在该设备中是唯一的标识ID
															//创建接收流时，若m_nSessionID，则有设备自动生成在该设备中唯一的会话ID
	unsigned _int32		m_nSrcIpAddress[eMaxInterfaceNum];	//主/从网口音频流的源始IP地址（发送地址）
	unsigned _int32		m_nDstIpAddress[eMaxInterfaceNum];	//主/从网口音频流的目标IP地址（接收地址）
	unsigned _int16		m_nSrcIpPortNo[eMaxInterfaceNum];	//主/从网口音频流的源始端口号（发送端口号），这里约定使用AES70服务端口号。
															//同一台计算机上若运行多个虚拟AES70设备，每个设备的AES70服务端口号各不相同
	unsigned _int16		m_nDstIpPortNo[eMaxInterfaceNum];	//主/从网口音频流的目标端口号（接收端口号）
															//创建接收流时，若接收端口号为0，则由设备自动分配
	std::string			m_strTxDeviceName_UTF_8;			//UTF_8名称
	std::string			m_strRxDeviceName_UTF_8;			//UTF_8名称
	std::wstring		m_strTxDeviceName;					//Unicode名称
	std::wstring		m_strRxDeviceName;					//Unicode名称
} ;
