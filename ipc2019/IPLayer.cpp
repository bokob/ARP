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
	: CBaseLayer(pName) // CBaseLayer���� ��ӹ޴´�. �Է¹��� ���ڷ� CBaseLayer�� ������ ����
{
	ResetHeader();				// ��� �ʱ�ȭ
}

CIPLayer::~CIPLayer()	// �Ҹ���
{
}

void CIPLayer::ResetHeader()	// ��� �ʱ�ȭ
{
	memset(m_sHeader.ip_dstaddr, 0, 6);
	memset(m_sHeader.ip_srcaddr, 0, 6);
	memset(m_sHeader.ip_data, IP_MAX_DATA_SIZE, 6);
	m_sHeader.ip_type = 0;
}

unsigned char* CIPLayer::GetSourceAddress() // ����� ����ִ� �����ּҸ� ��ȯ�Ѵ�.
{
	return m_sHeader.ip_srcaddr;
}

unsigned char* CIPLayer::GetDestinAddress() // ����� ����ִ� �����ּҸ� ��ȯ�Ѵ�.
{
	return m_sHeader.ip_dstaddr;
}

void CIPLayer::SetSourceAddress(unsigned char* pAddress)
{
	memcpy(m_sHeader.ip_srcaddr, pAddress, 4);
}

void CIPLayer::SetDestinAddress(unsigned char* pAddress) // �Ѱܹ��� ������ �ּҸ� Ethernet destination�ּҷ� ����
{
	memcpy(m_sHeader.ip_dstaddr, pAddress, 4);
}

BOOL CIPLayer::Send(unsigned char* ppayload, int nlength)
{
	memcpy(m_sHeader.ip_data, ppayload, nlength);

	BOOL bSuccess = FALSE;

	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, nlength + IP_HEADER_SIZE, m_sHeader.ip_dstaddr);

	return bSuccess;
}

BOOL CIPLayer::Send(unsigned char* IPaddr)
{
	memcpy(m_sHeader.ip_data, IPaddr, 4);

	BOOL bSuccess = FALSE;

	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, IP_HEADER_SIZE, m_sHeader.ip_dstaddr);

	return bSuccess;
}

unsigned char* CIPLayer::Receive()
{
	unsigned char* ppayload = mp_UnderLayer->Receive();

	PIP_HEADER pFrame = (PIP_HEADER)ppayload;

	return pFrame->ip_data;
}
