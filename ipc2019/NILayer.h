#if !defined(AFX_FILELAYER_H__D67222B3_1B00_4C77_84A4_CEF6D572E181__INCLUDED_)
#define AFX_FILELAYER_H__D67222B3_1B00_4C77_84A4_CEF6D572E181__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include <pcap.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <Packet32.h>
#pragma comment(lib, "packet.lib")

class CNILayer
	: public CBaseLayer
{
protected:
	pcap_t* m_AdapterObject;

public:
	BOOL			 m_thrdSwitch;
	unsigned char*	 m_ppayload;

	void			PacketStartDriver();
	void			PacketEndDriver();

	pcap_if_t*		GetAdapterObject(int iIndex);
	void			SetAdapterNumber(int iNum);
	void			SetAdapterList(LPADAPTER* plist);
	CString			GetNICardAddress(char* adapter_name);
	static UINT		ReadingThread(LPVOID pParam);
	//static UINT		FileTransferThread(LPVOID pParam);

	BOOL			Receive();		// 패킷 수신
	BOOL			Send(unsigned char* ppayload, int nlength);

	CNILayer(char* pName, LPADAPTER* pAdapterObject = NULL, int iNumAdapter = 0);
	virtual ~CNILayer();

protected:
	int			m_iNumAdapter;
	pcap_if_t*  m_pAdapterList[NI_COUNT_NIC];
};

#endif // !defined(AFX_FILELAYER_H__D67222B3_1B00_4C77_84A4_CEF6D572E181__INCLUDED_)