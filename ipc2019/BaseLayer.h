#pragma once
// BaseLayer.h: interface for the CBaseLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include"stdafx.h"

#if !defined(AFX_BASELAYER_H__041C5A07_23A9_4CBC_970B_8743460A7DA9__INCLUDED_)
#define AFX_BASELAYER_H__041C5A07_23A9_4CBC_970B_8743460A7DA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseLayer
{
public:
	char* GetLayerName();					// 계층의 이름 반환

	CBaseLayer*		GetUnderLayer();			// 하위 계층을 반환한다.
	CBaseLayer*		GetUpperLayer(int nindex);	// 인덱스에 해당하는 상위 계층을 반환한다.
	void			SetUnderUpperLayer(CBaseLayer* pUULayer = NULL); // 인자로 받은 계층을 현재 계층의 Under로, 현재 계층을 인자로 받은 계층의 Upper로 설정
	void			SetUpperUnderLayer(CBaseLayer* pUULayer = NULL); // 인자로 받은 계층을 현재 계층의 Upper로, 현재 계층을 인자로 받은 계층의 Under로 설정
	void			SetUnderLayer(CBaseLayer* pUnderLayer = NULL); // 인자로 받은 계층을 현계층의 Under로 설정
	void			SetUpperLayer(CBaseLayer* pUpperLayer = NULL);// 인자로 받은 계층을 현계층의 Upper로 설정

	CBaseLayer(char* pName = NULL);		// 생성자, 기본값은 NULL이다.
	virtual ~CBaseLayer();				// 소멸자

	// param : unsigned char*	- the data of the upperlayer
	//         int				- the length of data
	virtual	BOOL			Send(unsigned char*, int) { return FALSE; }	// 다른 계층에서 전송
	virtual	BOOL			Send(unsigned char*, int, unsigned char*) { return FALSE; }	// ARP 계층에서 전송
	virtual	BOOL			Send(unsigned char*, int, short) { return FALSE; }	// Ethernet 계층에서 전송
	virtual	BOOL			Send(unsigned char*) { return FALSE; }	// IP 계층에서 전송
	// param : unsigned char*	- the data of the underlayer	
	virtual	BOOL			Receive(unsigned char* ppayload) { return FALSE; }	// Dlg 계층에서 수신
	virtual	unsigned char*	Receive() { return FALSE; }	// 다른 계층에서 수신

protected:
	char*			m_pLayerName;							// 계층 이름
	CBaseLayer*		mp_UnderLayer;							// UnderLayer pointer
	CBaseLayer*		mp_aUpperLayer[MAX_LAYER_NUMBER];		// UpperLayer pointer
	int				m_nUpperLayerCount;						// UpperLayer Count
};

#endif // !defined(AFX_BASELAYER_H__041C5A07_23A9_4CBC_970B_8743460A7DA9__INCLUDED_)
