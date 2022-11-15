#include "pch.h"
#include "stdafx.h"
#include "NILayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CNILayer::CNILayer(char* pName, LPADAPTER* pAdapterObject, int iNumAdapter)
	: CBaseLayer(pName)
{
	m_AdapterObject = NULL;
	m_iNumAdapter = iNumAdapter;
	m_thrdSwitch = TRUE;
	SetAdapterList(NULL);
}

CNILayer::~CNILayer()
{
}

void CNILayer::PacketStartDriver()
{
	char errbuf[PCAP_ERRBUF_SIZE];

	if (m_iNumAdapter == -1)
	{
		AfxMessageBox("Not exist NICard");
		return;
	}

	m_AdapterObject = pcap_open_live(m_pAdapterList[m_iNumAdapter]->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, errbuf);
	if (!m_AdapterObject)
	{
		AfxMessageBox(errbuf);
		return;
	}
	//AfxBeginThread(ReadingThread, this);
}

void CNILayer::PacketEndDriver()
{
	//	pcap_close(m_AdapterObject);
	m_thrdSwitch = FALSE;
}

pcap_if_t* CNILayer::GetAdapterObject(int iIndex)
{
	return m_pAdapterList[iIndex];
}

void CNILayer::SetAdapterNumber(int iNum)
{
	m_iNumAdapter = iNum;
}

void CNILayer::SetAdapterList(LPADAPTER* plist)
{
	pcap_if_t* alldevs;
	pcap_if_t* d;
	int i = 0;

	char errbuf[PCAP_ERRBUF_SIZE];

	for (int j = 0; j < NI_COUNT_NIC; j++)
	{
		m_pAdapterList[j] = NULL;
	}

	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		AfxMessageBox("Not exist NICard");
		return;
	}
	if (!alldevs)
	{
		AfxMessageBox("Not exist NICard");
		return;
	}

	for (d = alldevs; d; d = d->next)
	{
		m_pAdapterList[i++] = d;
	}
}

CString CNILayer::GetNICardAddress(char* adapter_name)
{
	PPACKET_OID_DATA OidData;
	LPADAPTER lpAdapter;

	OidData = (PPACKET_OID_DATA)malloc(6 + sizeof(PACKET_OID_DATA));
	if (OidData == NULL)
	{
		return "None";
	}

	OidData->Oid = OID_802_3_CURRENT_ADDRESS;
	OidData->Length = 6;
	ZeroMemory(OidData->Data, 6);

	lpAdapter = PacketOpenAdapter(adapter_name);

	CString NICardAddress;

	if (PacketRequest(lpAdapter, FALSE, OidData))
	{
		NICardAddress.Format("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
			(OidData->Data)[0],
			(OidData->Data)[1],
			(OidData->Data)[2],
			(OidData->Data)[3],
			(OidData->Data)[4],
			(OidData->Data)[5]);
	}

	PacketCloseAdapter(lpAdapter);
	free(OidData);
	return NICardAddress;
}

BOOL CNILayer::Send(unsigned char* ppayload, int nlength) // 패킷 전송
{
	if (pcap_sendpacket(m_AdapterObject, ppayload, nlength))
	{
		AfxMessageBox("패킷 전송 실패");
		return FALSE;
	}
	AfxMessageBox("패킷 전송 성성공공!");
	return TRUE;
}


BOOL CNILayer::Receive()
{
	int result;
	struct pcap_pkthdr* header;
	const unsigned char* pkt_data;
	unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * ETHER_MAX_SIZE);

	//make packet's memory
	memset(data, '\0', ETHER_MAX_SIZE);

	//pcap_next_ex(receive packet)
	while ((result = pcap_next_ex(m_AdapterObject, &header, &pkt_data)) >= 0) 
	{
		if (result == 0)
			continue;

		memcpy(data, pkt_data, ETHER_MAX_SIZE);    // 패킷 복사
		mp_aUpperLayer[0]->Receive(data);			// 패킷 상위 12비트 (목적지 mac주소, 출발 mac주소)가 제거된 데이터 전달
	}

	if (result = -1)   // 오류
		return FALSE;
	else
		return TRUE;
}