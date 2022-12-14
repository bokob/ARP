// EthernetLayer.h: interface for the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include "pch.h"

class CIPLayer
	: public CBaseLayer
{
private:
	inline void		ResetHeader();	// 헤더 초기화
public:
	BOOL			Receive(unsigned char* ppayload); // 수신
	int				Send(unsigned char* IPaddr);
	void			SetDestinAddress(unsigned char* pAddress);
	void			SetSourceAddress(unsigned char* pAddress);
	unsigned char*	GetDestinAddress();	// 헤더에 들어있는 도착주소를 반환한다.
	unsigned char*	GetSourceAddress();	// 헤더에 들어있는 시작주소를 반환한다.

	CIPLayer(char* pName);
	virtual ~CIPLayer();

	typedef struct _IP_HEADER {

		unsigned char	ip_dstaddr[4];			   // destination address of ethernet layer
		unsigned char	ip_srcaddr[4];			   // source address of ethernet layer
		unsigned short	ip_type;				   // type of ethernet layer
		unsigned char	ip_data[IP_MAX_DATA_SIZE]; // frame data

	} IP_HEADER, * PIP_HEADER;

protected:
	IP_HEADER	m_sHeader;

};