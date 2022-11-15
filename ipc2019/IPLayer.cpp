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

CIPLayer::CIPLayer(char* pName)
	: CBaseLayer(pName) // CBaseLayer에서 상속받는다. 입력받은 인자로 CBaseLayer의 생성자 실행
{
	ResetHeader();				// 헤더 초기화
}

CIPLayer::~CIPLayer()	// 소멸자
{
}

void CIPLayer::ResetHeader()	// 헤더 초기화
{
	memset(m_sHeader.ip_dstaddr, 0, 6);
	memset(m_sHeader.ip_srcaddr, 0, 6);
	memset(m_sHeader.ip_data, IP_MAX_DATA_SIZE, 6);
	m_sHeader.ip_type = 0;
}

unsigned char* CIPLayer::GetSourceAddress() // 헤더에 들어있는 Source 주소를 반환한다.
{
	return m_sHeader.ip_srcaddr;
}

unsigned char* CIPLayer::GetDestinAddress() // 헤더에 들어있는 목적지 주소를 반환한다.
{
	return m_sHeader.ip_dstaddr;
}

void CIPLayer::SetSourceAddress(unsigned char* pAddress) // 넘겨받은 주소를 Source IP 주소로 설정
{
	memcpy(m_sHeader.ip_srcaddr, pAddress, 4);
}

void CIPLayer::SetDestinAddress(unsigned char* pAddress) // 넘겨받은 주소를 목적지 IP 주소로 설정
{
	memcpy(m_sHeader.ip_dstaddr, pAddress, 4);
}

BOOL CIPLayer::Send(unsigned char* IPaddr)
{
	memcpy(m_sHeader.ip_data, IPaddr, 4);

	BOOL bSuccess = FALSE;

	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, IP_HEADER_SIZE, m_sHeader.ip_dstaddr);

	return bSuccess;
}

BOOL CIPLayer::Receive(unsigned char* ppayload)
{
	PIP_HEADER pFrame = (PIP_HEADER)ppayload;
	BOOL bSuccess = FALSE;

	if (ppayload != NULL) 
	{
		PIP_HEADER pFrame = (PIP_HEADER)ppayload;

		return bSuccess = mp_aUpperLayer[0]->Receive(pFrame->ip_data);
	}
	return bSuccess;



	/*
	bool isOK = false;		// 수신허가 여부를 위한 변수
	int i = 0;

	for (i = 0; i < 4; i++)
		if (pFrame->ip_dstaddr[i] != m_sHeader.ip_srcaddr[i])
			break;
	if (i == 4)
		isOK = true;


	for (i = 0; i < 4; i++)
		if (pFrame->ip_srcaddr[i] != m_sHeader.ip_srcaddr[i])
			break;
	if (i != 4) 
	{
		for (i = 0; i < 4; i++)
			if (pFrame->ip_dstaddr[i] != 255)
				break;
		if (i == 4)
			isOK = true;
	}
	// 수신 허가시 상위계층으로 데이터 전달
	if (isOK)
	{
		bSuccess = mp_aUpperLayer[0]->Receive((unsigned char*)pFrame->ip_data);
		if (bSuccess == FALSE)
			::AfxMessageBox("Error while Receiving something.");
		return bSuccess;
	}
	else
		return FALSE;
	*/
}
