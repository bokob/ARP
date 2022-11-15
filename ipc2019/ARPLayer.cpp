#include "pch.h"
#include "stdafx.h"
#include "ARPLayer.h"
#include "ipc2019Dlg.h"

CARPLayer::CARPLayer(char* pName)	// ������
	: CBaseLayer(pName)
{
	ResetHeader();				// ARP ��� �ʱ�ȭ
	ARPTable.InitHashTable(23); //afxtempl.h�� ����ִ� �Լ�, �ؽ����̺��� �����.
}

CARPLayer::~CARPLayer()	// �Ҹ���
{
}

void CARPLayer::ResetHeader()	// ARP ��� �ʱ�ȭ
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

void CARPLayer::SetDestinEtherAddress(unsigned char* pAddress) //  ������ Ethernet �ּ� ����
{
	memcpy(m_arpBody.targetEthernetAddr, pAddress, 6);
}

void CARPLayer::SetSourceEtherAddress(unsigned char* pAddress) // ��� Ethernet �ּ� ����
{
	memcpy(m_arpBody.srcEthernetAddr, pAddress, 6);
}

void CARPLayer::SetDestinIPAddress(unsigned char* pAddress) // ������ IP �ּ� ����
{
	memcpy(m_arpBody.targetIPAddr, pAddress, 4);
}

void CARPLayer::SetSourceIPAddress(unsigned char* pAddress) // ��� IP �ּ� ����
{
	memcpy(m_arpBody.srcIPAddr, pAddress, 4);
}

char* CARPLayer::GetSourceEtherAddress()
{
	return m_arpBody.srcEthernetAddr;
}
char* CARPLayer::GetSourceIPAddress()
{
	return m_arpBody.srcIPAddr;
}
char* CARPLayer::GetDestinEtherAddress()
{
	return m_arpBody.targetEthernetAddr;
}
char* CARPLayer::GetDestinIPAddress()
{
	return m_arpBody.targetIPAddr;
}


BOOL CARPLayer::Send(unsigned char* ppayload, int nlength, unsigned char* pAddress) // Ethernet �������� ������ �Լ�
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
	}

	return bSuccess;
}

BOOL CARPLayer::Receive(unsigned char* ppayload)
{
	ARP_BODY* recivedARP = (ARP_BODY*)ppayload;

	memcpy(m_arpBody.srcEthernetAddr, ((CEthernetLayer*)mp_UnderLayer)->GetSourceAddress(), 6); // ��� Ethernet �ּҸ� �ִ´�.
	memcpy(m_arpBody.srcIPAddr, ((CIPLayer*)mp_aUpperLayer[0])->GetSourceAddress(), 4);	// ��� IP �ּҸ� �ִ´�.

	if (ppayload != NULL) 
	{


		setARPElement(recivedARP->srcIPAddr, recivedARP->srcEthernetAddr); // ARP�� ���� ���� src Ethernet, IP �ּҸ� �ؽ����̺� �����Ѵ�.

		switch (recivedARP->op)
		{
		case ARP_REQUEST:	// ARP Request�� ���
			if (!strncmp(recivedARP->targetIPAddr, m_arpBody.srcIPAddr, 4))	 
			{   // � IP�� Mac�ּҸ� �˰� �;� ���� ���� ARP�� ������ IP�� �������� IP �ּҿ� ������ ���
				
				// op ����
				recivedARP->op = 0x0200;

				memcpy(recivedARP->targetEthernetAddr, recivedARP->srcEthernetAddr, 6); 
				// ������ Ethernet �ּҿ� �۽����� Ethernet �ּҸ� �ִ´�.
				
				//CString strMac = recivedARP->srcEthernetAddr; // �۽����� Mac �ּҸ� ���߿� ARP ���̺� ������ ���� �ӽ� ����

				memcpy(recivedARP->srcEthernetAddr, m_arpBody.srcEthernetAddr, 6);		
				// �۽����� Ethernet �ּҰ� ����Ǿ��ִ� ���� ���Ź��� ���� Ethernet �ּҸ� �ִ´�.
				
				memcpy(recivedARP->targetIPAddr, recivedARP->srcIPAddr, 4);
				// ������ IP �ּҿ� �۽����� IP�ּҸ� �ִ´�.

				//CString strIP = recivedARP->srcIPAddr;	// �۽����� IP �ּҸ� ���߿� ARP ���̺� ������ ���� �ӽ� ����

				memcpy(recivedARP->srcIPAddr, m_arpBody.srcIPAddr, 4);
				// �۽����� IP�ּҰ� ����Ǿ��ִ� ���� ���Ź��� ���� IP �ּҸ� �ִ´�.

				/*
				���� ����
				targetEthernetAddr: �۽����� Ethernet �ּ� ��� ����
				srcEthernetAddr:	�������� Ethernet �ּ� ��� ����
				targetIPAddr:		�۽����� IP �ּ� ��� ����
				srcIPAddr:			�������� IP �ּ� ��� ����
				*/

				// ARP Reply ����
				mp_UnderLayer->Send((unsigned char*)recivedARP, 28, (unsigned char*)recivedARP->targetEthernetAddr, (short)0x0608);
			}
			else
			{
				int ip;
				memcpy(&ip, recivedARP->targetIPAddr, 4);
				ARPElement* _element = new(ARPElement);

				if (ProxyTable.Lookup(ip, *_element))		// Proxy ARP Reply ���
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

unsigned char* CARPLayer::getMacAddressFromARPTable(unsigned char* pAddress)	// ARP Table���� Mac�ּ� �������� �Լ�
{
	// �ʱ�ȭ
	ARPElement* _element = new(ARPElement);
	memset(_element, 0, sizeof(ARPElement));

	int ip;
	memcpy(&ip, pAddress, 4);

	ARPTable.Lookup(ip, *_element);	// Lookup �Լ��� Ű���� �Ҵ� �Ǿ��ִ��� ã�� �Լ���. ip�� �����ϸ� _element�� �� ���� ����Ű�� �Ѵ�.

	if (_element->MACAddr != NULL)	// _element�� �����ΰ� ����Ų�ٸ� -> ip�� �ִ� ���
	{// GetTickCount �Լ��� �ð� �����ϴ� �Լ�
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

bool CARPLayer::setARPElement(char* ipAddr, char* macAddr) // �ؽ����̺�� ������� ARP ���̺� �����Ѵ�.
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