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
	: CBaseLayer(pName) // CBaseLayer에서 상속받는다. 입력받은 인자로 CBaseLayer의 생성자 실행
{
	ResetHeader();				// 헤더 초기화
}

CEthernetLayer::~CEthernetLayer()	// 소멸자
{
}

void CEthernetLayer::ResetHeader()	// 헤더 초기화
{
	memset(m_sHeader.enet_dstaddr, 0, 6);
	memset(m_sHeader.enet_srcaddr, 0, 6);
	memset(m_sHeader.enet_data, ETHER_MAX_DATA_SIZE, 6);
	m_sHeader.enet_type = 0;
}

unsigned char* CEthernetLayer::GetSourceAddress()
// 헤더에 들어있는 시작주소를 반환한다.
{
	return m_sHeader.enet_srcaddr;
}

unsigned char* CEthernetLayer::GetDestinAddress()
// 헤더에 들어있는 도착주소를 반환한다.
{
	//////////////////////// fill the blank ///////////////////////////////
	// Ethernet 목적지 주소 return
	return m_sHeader.enet_dstaddr;
	///////////////////////////////////////////////////////////////////////
}

void CEthernetLayer::SetSourceAddress(unsigned char* pAddress)
{
	//////////////////////// fill the blank ///////////////////////////////
		// 넘겨받은 source 주소를 Ethernet source주소로 지정
	memcpy(m_sHeader.enet_srcaddr, pAddress, 6);
	///////////////////////////////////////////////////////////////////////
}

void CEthernetLayer::SetDestinAddress(unsigned char* pAddress)
// 넘겨받은 목적지 주소를 Ethernet destination주소로 지정
{
	memcpy(m_sHeader.enet_dstaddr, pAddress, 6);
}

void CEthernetLayer::SetEnetType(unsigned short enet_type) 
{
	this->m_sHeader.enet_type = htons(enet_type);
}


BOOL CEthernetLayer::Send(unsigned char* ppayload, int nlength, short frameType) // Ethernet에서 보통의 경우에 쓰이는 전송 함수
{
	m_sHeader.enet_type = frameType;
	memcpy(m_sHeader.enet_data, ppayload, nlength);

	BOOL bSuccess = FALSE;
	//////////////////////// fill the blank ///////////////////////////////
	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, nlength + ETHER_HEADER_SIZE);
	///////////////////////////////////////////////////////////////////////
	return bSuccess;
}

BOOL CEthernetLayer::Send(unsigned char* ppayload, int nlength, unsigned char* desAddr, short frameType) // ARP Reply때 쓰임
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

		bSuccess = mp_aUpperLayer[1]->Receive((unsigned char*)pFrame->enet_data);	// IP 계층으로 올림
		bSuccess = mp_aUpperLayer[0]->Receive((unsigned char*)pFrame->enet_data);	// ARP 계층으로 올림
		return bSuccess;
	}
	return bSuccess;
}