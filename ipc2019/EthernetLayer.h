// EthernetLayer.h: interface for the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
#define AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include "pch.h"

class CEthernetLayer
	: public CBaseLayer
{
private:
	inline void		ResetHeader();	// 헤더 초기화

public:
	BOOL					Receive(unsigned char* ppayload);			// NI 계층에서 수신
	BOOL					Send(unsigned char* ppayload, int nlength, short frameType);
	BOOL					Send(unsigned char* ppayload, int nlength, unsigned char* desMacAddr, short frameType); // ARP Reply 날릴 때 쓰이는 것
	void					SetDestinAddress(unsigned char* pAddress);	// 넘겨받은 source 주소를 Ethernet source주소로 지정
	void					SetSourceAddress(unsigned char* pAddress);	// 넘겨받은 목적지 주소를 Ethernet destination 주소로 지정
	unsigned char*			GetDestinAddress();	// 헤더에 들어있는 도착주소를 반환한다.
	unsigned char*			GetSourceAddress();	// 헤더에 들어있는 시작주소를 반환한다.
	void					SetEnetType(unsigned short enet_type);

	CEthernetLayer(char* pName);	// 생성자
	virtual ~CEthernetLayer();		// 소멸자

	typedef struct _ETHERNET_HEADER {

		unsigned char	enet_dstaddr[6];		// destination address of ethernet layer
		unsigned char	enet_srcaddr[6];		// source address of ethernet layer
		unsigned short	enet_type;		// type of ethernet layer
		unsigned char	enet_data[ETHER_MAX_DATA_SIZE]; // frame data

	} ETHERNET_HEADER, * PETHERNET_HEADER;

protected:
	ETHERNET_HEADER	m_sHeader;
};

#endif // !defined(AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
