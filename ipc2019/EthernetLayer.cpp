// EthernetLayer.cpp: implementation of the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "EthernetLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEthernetLayer::CEthernetLayer(char* pName)	 
	: CBaseLayer(pName) // CBaseLayer���� ��ӹ޴´�. �Է¹��� ���ڷ� CBaseLayer�� ������ ����
{
	ResetHeader();				// ��� �ʱ�ȭ
}

CEthernetLayer::~CEthernetLayer()	// �Ҹ���
{
}

void CEthernetLayer::ResetHeader()	// ��� �ʱ�ȭ
{
	memset(m_sHeader.enet_dstaddr, 0, 6);
	memset(m_sHeader.enet_srcaddr, 0, 6);
	memset(m_sHeader.enet_data, ETHER_MAX_DATA_SIZE, 6);
	m_sHeader.enet_type = 0;
}

unsigned char* CEthernetLayer::GetSourceAddress()
// ����� ����ִ� �����ּҸ� ��ȯ�Ѵ�.
{
	return m_sHeader.enet_srcaddr;
}

unsigned char* CEthernetLayer::GetDestinAddress()
// ����� ����ִ� �����ּҸ� ��ȯ�Ѵ�.
{
	//////////////////////// fill the blank ///////////////////////////////
	// Ethernet ������ �ּ� return
	return m_sHeader.enet_dstaddr;
	///////////////////////////////////////////////////////////////////////
}

void CEthernetLayer::SetSourceAddress(unsigned char* pAddress)
{
	//////////////////////// fill the blank ///////////////////////////////
		// �Ѱܹ��� source �ּҸ� Ethernet source�ּҷ� ����
	memcpy(m_sHeader.enet_srcaddr, pAddress, 6);
	///////////////////////////////////////////////////////////////////////
}

void CEthernetLayer::SetDestinAddress(unsigned char* pAddress)
// �Ѱܹ��� ������ �ּҸ� Ethernet destination�ּҷ� ����
{
	memcpy(m_sHeader.enet_dstaddr, pAddress, 6);
}

void CEthernetLayer::SetEnetType(unsigned short enet_type) 
{
	this->m_sHeader.enet_type = htons(enet_type);
}


BOOL CEthernetLayer::Send(unsigned char* ppayload, int nlength, short frameType) // Ethernet���� ������ ��쿡 ���̴� ���� �Լ�
{
	m_sHeader.enet_type = frameType;
	memcpy(m_sHeader.enet_data, ppayload, nlength);

	BOOL bSuccess = FALSE;
	//////////////////////// fill the blank ///////////////////////////////
	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, nlength + ETHER_HEADER_SIZE);
	///////////////////////////////////////////////////////////////////////
	return bSuccess;
}

BOOL CEthernetLayer::Send(unsigned char* ppayload, int nlength, unsigned char* desAddr, short frameType) // ARP Reply�� ����
{
	m_sHeader.enet_type = frameType;
	memcpy(m_sHeader.enet_dstaddr, desAddr, 6);
	memcpy(m_sHeader.enet_data, ppayload, nlength);
	BOOL bSuccess = FALSE;

	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, nlength + ETHER_HEADER_SIZE);

	return bSuccess;
}


BOOL CEthernetLayer::Receive(unsigned char* ppayload)
{
	BOOL bSuccess = FALSE;
	unsigned char	ed[6];
	unsigned char	es[6];
	memset(ed, 0, 6);
	memset(es, 0, 6);

	if (ppayload != NULL) 
	{
		PETHERNET_HEADER pFrame = (PETHERNET_HEADER)ppayload;
		unsigned char F = 255;// = 0xff(breadcast)
		memcpy(ed, pFrame->enet_dstaddr, 6);
		memcpy(es, pFrame->enet_srcaddr, 6);

		bSuccess = mp_aUpperLayer[1]->Receive((unsigned char*)pFrame->enet_data);	// IP �������� �ø�
		bSuccess = mp_aUpperLayer[0]->Receive((unsigned char*)pFrame->enet_data);	// ARP �������� �ø�
		return bSuccess;
	}
	return bSuccess;
}