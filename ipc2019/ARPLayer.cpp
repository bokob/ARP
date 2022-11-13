#include "pch.h"
#include "stdafx.h"
#include "ARPLayer.h"

/*
CARPLayer::CARPLayer()			// 생성자
{
	ResetHeader();
	ARPTable.InitHashTable(23);
}
*/

CARPLayer::CARPLayer(char* pName)	// 생성자
	: CBaseLayer(pName)
{
	ResetHeader();				// ARP 헤더 초기화
	ARPTable.InitHashTable(23); //afxtempl.h에 들어있는 함수, 해시테이블을 만든다.
}

CARPLayer::~CARPLayer()	// 소멸자
{
}

void CARPLayer::ResetHeader()	// ARP 헤더 초기화
{
	m_arpBody.hardType = 0;
	m_arpBody.protType = 0;
	m_arpBody.hardSize = 0;
	m_arpBody.protSize = 0;
	m_arpBody.op = 0;
	memset(m_arpBody.srcEthernetAddr, 0, 6);
	memset(m_arpBody.srcIPAddr, 0, 4);
	memset(m_arpBody.targetEthernetAddr, 0, 6);
	memset(m_arpBody.targetIPAddr, 0, 4);
}

/*
void CARPLayer::SetSourceAddress(unsigned char* pAddress) //src setting
{
	//memcpy(m_arpBody.enet_srcaddr, pAddress, 6);
}

void CARPLayer::SetDestinAddress(unsigned char* pAddress) //des setting
{
	//memcpy(m_arpBody.enet_dstaddr, pAddress, 6);
}
*/

BOOL CARPLayer::Send(unsigned char* ppayload, int nlength, unsigned char* pAddress) // Ethernet 계층으로 보내느 함수
{
	BOOL bSuccess = FALSE;
	unsigned char* desMacAddr;

	if ((desMacAddr = getMacAddressFromARPTable(pAddress)) == NULL || true)
	{
		m_arpBody.hardType = 0x0100;
		m_arpBody.protType = 0x0008;
		m_arpBody.hardSize = 6;
		m_arpBody.protSize = 4;
		m_arpBody.op = 0x0100;
		memcpy(m_arpBody.srcEthernetAddr, ((CEthernetLayer*)mp_UnderLayer)->GetSourceAddress(), 6);
		memcpy(m_arpBody.srcIPAddr, ((CIPLayer*)mp_aUpperLayer[0])->GetSourceAddress(), 4);
		memcpy(m_arpBody.targetIPAddr, pAddress, 4);
		bSuccess = mp_UnderLayer->Send((unsigned char*)&m_arpBody, 28, (short)0x0608);
		// lock
	}
	else
	{
		//unlock
	}
	/*
	//while (true)	// lock
	{}
	if (desMacAddr != NULL)		// arp table이 있어야 전송 가능
		bSuccess = mp_UnderLayer->Send((unsigned char*)&ppayload, nlength, desMacAddr, 0x0008);
	*/

	return bSuccess;
}

unsigned char* CARPLayer::Receive()
{
	unsigned char* ppayload = mp_UnderLayer->Receive();

	ARP_BODY* recivedARP = (ARP_BODY*)ppayload;

	memcpy(m_arpBody.srcEthernetAddr, ((CEthernetLayer*)mp_UnderLayer)->GetSourceAddress(), 6);
	memcpy(m_arpBody.srcIPAddr, ((CIPLayer*)mp_aUpperLayer[0])->GetSourceAddress(), 4);

	if (ppayload != NULL) {

		setARPElement(recivedARP->srcIPAddr, recivedARP->srcEthernetAddr);

		switch (recivedARP->op)
		{
		case ARP_REQUEST:
			if (!strncmp(recivedARP->targetIPAddr, m_arpBody.srcIPAddr, 4))
			{
				recivedARP->op = 0x0200;
				memcpy(recivedARP->targetEthernetAddr, recivedARP->srcEthernetAddr, 6);
				memcpy(recivedARP->srcEthernetAddr, m_arpBody.srcEthernetAddr, 6);
				memcpy(recivedARP->targetIPAddr, recivedARP->srcIPAddr, 4);
				memcpy(recivedARP->srcIPAddr, m_arpBody.srcIPAddr, 4);
				mp_UnderLayer->Send((unsigned char*)recivedARP, 28, (unsigned char*)recivedARP->targetEthernetAddr, (short)0x0608);
			}
			else
			{
				int ip;
				memcpy(&ip, recivedARP->targetIPAddr, 4);
				ARPElement* _element = new(ARPElement);

				if (ProxyTable.Lookup(ip, *_element))		// proxy arp reply
				{
					recivedARP->op = 0x0200;
					memcpy(recivedARP->targetEthernetAddr, recivedARP->srcEthernetAddr, 6);
					memcpy(recivedARP->srcEthernetAddr, m_arpBody.srcEthernetAddr, 6);
					memcpy(recivedARP->targetIPAddr, recivedARP->srcIPAddr, 4);
					memcpy(recivedARP->srcIPAddr, &ip, 4);
					mp_UnderLayer->Send((unsigned char*)recivedARP, 28, (unsigned char*)recivedARP->targetEthernetAddr, (short)0x0608);
				}
			}
			break;
		}
	}
	return 0;
}

unsigned char* CARPLayer::getMacAddressFromARPTable(unsigned char* pAddress)	// ARP Table에서 Mac주소 가져오는 함수
{
	// 초기화
	ARPElement* _element = new(ARPElement);
	memset(_element, 0, sizeof(ARPElement));

	int ip;
	memcpy(&ip, pAddress, 4);

	ARPTable.Lookup(ip, *_element);	// Lookup 함수는 키값이 할당 되엉있는지 찾는 함수다. ip가 존재하면 _element가 그 값을 가리키게 한다.

	if (_element->MACAddr != NULL)	// _element가 무엇인가 가리킨다면 -> ip가 있는 경우
	{// GetTickCount 함수는 시간 측정하는 함수
		if (GetTickCount() - _element->tick < 1200000 && _element->state == ARP_COMPLETE)		// 20min to time out for complete
			return _element->MACAddr;
		else
		{
			_element->MACAddr = NULL;
			_element->state = ARP_INCOMPLETE;
		}
	}

	_element->tick = GetTickCount();
	ARPTable.SetAt(ip, *_element);

	return NULL;
}

bool CARPLayer::setARPElement(char* ipAddr, char* macAddr)
{
	ARPElement* arpElement = new ARPElement;

	int ip;
	memcpy(&ip, ipAddr, 4);

	arpElement->MACAddr = new unsigned char[6];
	memcpy(arpElement->MACAddr, macAddr, 6);
	arpElement->state = ARP_COMPLETE;
	arpElement->tick = GetTickCount();
	ARPTable.SetAt(ip, *arpElement);

	return true;
}

ARPElement* CARPLayer::getARPElements(char*** keyIPs, int* tableSize)
{
	ARPElement* arpElements = (ARPElement*)malloc(sizeof(ARPElement) * ARPTable.GetCount());
	*keyIPs = (char**)malloc(sizeof(char*) * ARPTable.GetCount());
	*tableSize = 0;

	CMapForARPTable::CPair* pCur = ARPTable.PGetFirstAssoc();

	while (pCur != NULL)
	{
		DWORD dTick = GetTickCount() - pCur->value.tick;
		if ((dTick > ARP_INCOMPLETE_TIMEOUT && pCur->value.state == ARP_INCOMPLETE)
			|| (dTick > ARP_COMPLETE_TIMEOUT && pCur->value.state == ARP_COMPLETE))
		{
			CMapForARPTable::CPair* pDeletedCur = pCur;
			pCur = ARPTable.PGetNextAssoc(pCur);
			ARPTable.RemoveKey(pDeletedCur->key);

			continue;
		}

		(*keyIPs)[*tableSize] = (char*)malloc(sizeof(char) * 6);
		memcpy((*keyIPs)[*tableSize], &(pCur->key), 6);
		arpElements[*tableSize] = pCur->value;
		pCur = ARPTable.PGetNextAssoc(pCur);
		(*tableSize)++;
	}

	return arpElements;
}

bool CARPLayer::insertProxyTable(unsigned char* proxyIPAddr, unsigned char* proxyMacAddr)
{
	ARPElement* proxyElement = new ARPElement;

	int ip;
	memcpy(&ip, proxyIPAddr, 4);

	proxyElement->MACAddr = new unsigned char[6];
	memcpy(proxyElement->MACAddr, proxyMacAddr, 6);
	ProxyTable.SetAt(ip, *proxyElement);
	return true;
}

bool CARPLayer::deleteProxyTable(unsigned char* proxyIPAddr)
{
	ARPElement* proxyElement = new ARPElement;

	int ip;
	memcpy(&ip, proxyIPAddr, 4);

	ProxyTable.RemoveKey(ip);

	return true;
}

ARPElement* CARPLayer::getProxyElements(char*** keyIPs, int* tableSize)
{
	ARPElement* proxyElements = (ARPElement*)malloc(sizeof(ARPElement) * ProxyTable.GetCount());
	*keyIPs = (char**)malloc(sizeof(char*) * ProxyTable.GetCount());
	*tableSize = 0;

	CMapForARPTable::CPair* pCur = ProxyTable.PGetFirstAssoc();

	while (pCur != NULL)
	{
		(*keyIPs)[*tableSize] = (char*)malloc(sizeof(char) * 6);
		memcpy((*keyIPs)[*tableSize], &(pCur->key), 6);
		proxyElements[*tableSize] = pCur->value;
		pCur = ProxyTable.PGetNextAssoc(pCur);
		(*tableSize)++;
	}

	return proxyElements;
}

void CARPLayer::initARPTable()
{
	ARPTable.RemoveAll();
	ProxyTable.RemoveAll();
}