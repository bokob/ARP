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
	inline void		ResetHeader();	// ��� �ʱ�ȭ
public:
	unsigned char*	Receive(); // ����
	int				Send(unsigned char* ppayload, int nlength);
	int				Send(unsigned char* IPaddr);
	void			SetDestinAddress(unsigned char* pAddress);
	void			SetSourceAddress(unsigned char* pAddress);
	unsigned char*	GetDestinAddress();	// ����� ����ִ� �����ּҸ� ��ȯ�Ѵ�.
	unsigned char*	GetSourceAddress();	// ����� ����ִ� �����ּҸ� ��ȯ�Ѵ�.

	CIPLayer(char* pName);
	virtual ~CIPLayer();

	typedef struct _IP_HEADER {

		unsigned char	ip_dstaddr[6];			   // destination address of ethernet layer
		unsigned char	ip_srcaddr[6];			   // source address of ethernet layer
		unsigned short	ip_type;				   // type of ethernet layer
		unsigned char	ip_data[IP_MAX_DATA_SIZE]; // frame data

	} IP_HEADER, * PIP_HEADER;

protected:
	IP_HEADER	m_sHeader;

};