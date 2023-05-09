#pragma once

#include "DeclareExportAES70ExtDLL.h"
#include	"..\SDK_INCLUDE\AES70ManagerDataType.h"
#include <vector>
#include <string>

class EXPORT_AES70_CLASS CDeviceSystemCfg{
public:
	CDeviceSystemCfg();
private:
	UINT32		m_nMediaTransportNetworkONo;
	UINT32		m_nSilenceDetectorONo;
	UINT32		m_nDeviceNetworkONo;
	UINT16		m_nMaxSourceConnectors;	//���ķ���������
	UINT16		m_nMaxSinkConnectors;		//���Ľ������ĸ���
	UINT16		m_nMaxPinsPerConnector;	//ÿ�����������������Ƶͨ����
	UINT16		m_nMaxPortsPerPin;		//ÿ������ͨ���������������˿���
	
	std::vector<UINT32>			m_MediaClock3List;
	std::vector<AES70_PORT_CFG_T>	m_cRxPortCfg;
	std::vector<AES70_PORT_CFG_T>	m_cTxPortCfg;
	bool		m_bIsStreamCfgSetup;
public:
	UINT32		GetMediaTransportNetworkONo(void) const;
	UINT32		GetSilenceDetectorONo(void) const;
	UINT32		GetDeviceNetworkONo(void) const;

	UINT16		GetMaxSourceConnectors(void) const;
	UINT16		GetMaxSinkConnectors(void) const;
	UINT16		GetMaxPinsPerConnector(void) const;
	UINT16		GetMaxPortsPerPin(void) const;
	UINT16		GetTxChNum(void) const;
	UINT16		GetRxChNum(void) const;
	UINT16		GetFirstTxChNo(void) const ;
	UINT16		GetFirstRxChNo(void) const;

	UINT32		GetMediaClock3ONo(void) const;
	void		GetMediaClock3ONoList(std::vector<UINT32>	&MediaClock3List) const;

	std::vector<AES70_PORT_CFG_T>	    &GetRxPort(void);
	const std::vector<AES70_PORT_CFG_T>	&GetRxPort(void) const;

	std::vector<AES70_PORT_CFG_T>	    &GetTxPort(void);
	const std::vector<AES70_PORT_CFG_T>	&GetTxPort(void) const;

	void SetMediaTransportNetworkONo(UINT32 nMediaTransportNetworkONo);
	void SetSilenceDetectorONo(UINT32	nSilenceDetectorONo) ;
	void SetDeviceNetworkONo(UINT32		nDeviceNetworkONo);
	void SetMaxSourceConnectors(UINT16	nMaxSourceConnectors);	//���ķ���������
	void SetMaxSinkConnectors(UINT16 nMaxSinkConnectors);	//���Ľ������ĸ���
	void SetMaxPinsPerConnector(UINT16 nMaxPinsPerConnector);	//ÿ�����������������Ƶͨ����m_nMaxSinkConnectors

	void SetMaxPortsPerPin(UINT16 nMaxPortsPerPin);		//ÿ������ͨ���������������˿���
	void SetMediaClock3List(const std::vector<UINT32>	&MediaClock3List);
	void SetTxPort(const std::vector<AES70_PORT_CFG_T>	&TxPortCfg)	;
	void SetRxPort(const std::vector<AES70_PORT_CFG_T>	&RxPortCfg);

	void		AddRxPort(const AES70_PORT_CFG_T &Port);
	void		AddTxPort(const AES70_PORT_CFG_T &Port);

	void		ClearRxPort(void) { GetRxPort().clear(); }
	void		ClearTxPort(void) { GetTxPort().clear(); }

	void		PatchInfo(void);
	bool		IsStreamCfgSetup(void)	const;
	void		SetStreamCfgFlag(bool bFlag = true);
	void		SortPort(void);
};
