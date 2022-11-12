#pragma once
#include "BaseLayer.h"
#include "pch.h"

class CARPLayer 
	: public CBaseLayer
{
public:
	CARPLayer();
	CARPLayer(char* pName);
	~CARPLayer();

private:
	inline void		ResetHeader();
public:
	unsigned char* Receive();
	BOOL			Send(unsigned char* ppayload, int nlength, unsigned char* pAddress);
	void			SetDestinAddress(unsigned char* pAddress);
	void			SetSourceAddress(unsigned char* pAddress);
	unsigned char* getMacAddressFromARPTable(unsigned char* pAddress);
	bool			setARPElement(char* ipAddr, char* macAddr);
	ARPElement* getARPElements(char*** keyIPs, int* tableSize);
	bool			insertProxyTable(unsigned char* proxyIPAddr, unsigned char* proxyMacAddr);
	bool			deleteProxyTable(unsigned char* proxyIPAddr);
	ARPElement* getProxyElements(char*** keyIPs, int* tableSize);
	void			initARPTable();

	typedef struct _ARP_BODY {

		short hardType;
		short protoType;
		char hardSize;
		char protoSize;
		short opCode;
		char srcEthernetAddr[6];
		char srcIPAddr[4];
		char targetEthernetAddr[6];
		char targetIPAddr[4];

	} ARP_BODY, * PARP_BODY;


protected:
	ARP_BODY	m_arpBody;
	typedef CMap <int, int, ARPElement, ARPElement&> CMapForARPTable;
	CMapForARPTable ARPTable;
	CMapForARPTable ProxyTable;
};