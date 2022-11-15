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
	BOOL				Receive(unsigned char* ppayload);
	BOOL				Send(unsigned char* ppayload, int nlength, unsigned char* pAddress);
	void				SetDestinEtherAddress(unsigned char* pAddress);
	void				SetSourceEtherAddress(unsigned char* pAddress);
	void				SetDestinIPAddress(unsigned char* pAddress);
	void				SetSourceIPAddress(unsigned char* pAddress);
	char*				GetSourceEtherAddress();
	char*				GetSourceIPAddress();
	char*				GetDestinEtherAddress();
	char*				GetDestinIPAddress();

	unsigned char*		getMacAddressFromARPTable(unsigned char* pAddress); // ARP Table에서 Mac주소 가져오는 함수
	bool				setARPElement(char* ipAddr, char* macAddr);
	ARPElement*			getARPElements(char*** keyIPs, int* tableSize);
	bool				insertProxyTable(unsigned char* proxyIPAddr, unsigned char* proxyMacAddr);
	bool				deleteProxyTable(unsigned char* proxyIPAddr);
	ARPElement*			getProxyElements(char*** keyIPs, int* tableSize);
	void				initARPTable();

	typedef struct _ARP_BODY // 28바이트
	{
		short hardType;							 
		short protType;							 
		char hardSize;							// 하드웨어 주소 길이
		char protSize;							// 프로토콜 주소의 길이
		short op;								// ARP에서 어떤 동작을 하는지 알려주기 위한 것 ARP request일 때 1, reply일 때 2
		char srcEthernetAddr[6];		// 출발 Ethernet 주소
		char srcIPAddr[4];				// 출발 IP 주소
		char targetEthernetAddr[6];	// 목적지 Ethernet 주소
		char targetIPAddr[4];			// 목적지 IP 주소
	} ARP_BODY, * PARP_BODY;


protected:
	ARP_BODY	m_arpBody;
	typedef CMap <int, int, ARPElement, ARPElement&> CMapForARPTable;
	CMapForARPTable ARPTable;
	CMapForARPTable ProxyTable;
};