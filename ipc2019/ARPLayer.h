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

	unsigned char*		getMacAddressFromARPTable(unsigned char* pAddress); // ARP Table���� Mac�ּ� �������� �Լ�
	bool				setARPElement(char* ipAddr, char* macAddr);
	ARPElement*			getARPElements(char*** keyIPs, int* tableSize);
	bool				insertProxyTable(unsigned char* proxyIPAddr, unsigned char* proxyMacAddr);
	bool				deleteProxyTable(unsigned char* proxyIPAddr);
	ARPElement*			getProxyElements(char*** keyIPs, int* tableSize);
	void				initARPTable();

	typedef struct _ARP_BODY // 28����Ʈ
	{
		short hardType;							 
		short protType;							 
		char hardSize;							// �ϵ���� �ּ� ����
		char protSize;							// �������� �ּ��� ����
		short op;								// ARP���� � ������ �ϴ��� �˷��ֱ� ���� �� ARP request�� �� 1, reply�� �� 2
		char srcEthernetAddr[6];		// ��� Ethernet �ּ�
		char srcIPAddr[4];				// ��� IP �ּ�
		char targetEthernetAddr[6];	// ������ Ethernet �ּ�
		char targetIPAddr[4];			// ������ IP �ּ�
	} ARP_BODY, * PARP_BODY;


protected:
	ARP_BODY	m_arpBody;
	typedef CMap <int, int, ARPElement, ARPElement&> CMapForARPTable;
	CMapForARPTable ARPTable;
	CMapForARPTable ProxyTable;
};