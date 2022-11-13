#pragma once
#include "BaseLayer.h"
#include "pch.h"

class CARPLayer 
	: public CBaseLayer
{
public:
	//CARPLayer();
	CARPLayer(char* pName);
	~CARPLayer();

private:
	inline void			ResetHeader();
public:
	unsigned char*		Receive();
	BOOL				Send(unsigned char* ppayload, int nlength, unsigned char* pAddress);
	//void				SetDestinAddress(unsigned char* pAddress);
	//void				SetSourceAddress(unsigned char* pAddress);
	unsigned char*		getMacAddressFromARPTable(unsigned char* pAddress);
	bool				setARPElement(char* ipAddr, char* macAddr);
	ARPElement*			getARPElements(char*** keyIPs, int* tableSize);
	bool				insertProxyTable(unsigned char* proxyIPAddr, unsigned char* proxyMacAddr);
	bool				deleteProxyTable(unsigned char* proxyIPAddr);
	ARPElement*			getProxyElements(char*** keyIPs, int* tableSize);
	void				initARPTable();

	typedef struct _ARP_BODY // 28官捞飘
	{
		short hardType;				// 1
		short protType;				// 0x800
		char hardSize;		
		char protSize;				
		short op;					// ARP request老 锭 1, reply老 锭 2
		char srcEthernetAddr[6];	// 免惯 Ethernet 林家
		char srcIPAddr[4];			// 免惯 IP 林家
		char targetEthernetAddr[6];	// 格利瘤 Ethernet 林家
		char targetIPAddr[4];		// 格利瘤 IP 林家
	} ARP_BODY, * PARP_BODY;


protected:
	ARP_BODY	m_arpBody;
	typedef CMap <int, int, ARPElement, ARPElement&> CMapForARPTable;
	CMapForARPTable ARPTable;
	CMapForARPTable ProxyTable;
};