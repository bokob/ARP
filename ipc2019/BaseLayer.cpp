// BaseLayer.cpp: implementation of the CBaseLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ipc2019.h"
#include "BaseLayer.h"
#include "pch.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseLayer::CBaseLayer(char* pName)	 // 생성자
	: m_nUpperLayerCount(0),	// 상위 계층의 개수를 나타내는 변수다. 0으로 초기화
	mp_UnderLayer(NULL)			// 현 계층의 바로 밑 계층을 나타내는 변수// NULL로 초기화
{
	m_pLayerName = pName;		// 계층의 이름을 매개변수로 한다.
}

CBaseLayer::~CBaseLayer()		// 소멸자
{

}

void CBaseLayer::SetUnderUpperLayer(CBaseLayer* pUULayer)							
// 인자로 받은 계층을 현재 계층의 Under로, 현재 계층을 인자로 받은 계층의 Upper로 설정
{
	if (!pUULayer) // if the pointer is null, 경고문
	{
#ifdef _DEBUG
		TRACE("[CBaseLayer::SetUnderUpperLayer] The variable , 'pUULayer' is NULL");
#endif
		return;
	}

	//////////////////////// fill the blank ///////////////////////////////
		// 인자로 받은 계층은 현재 계층의 Under로 놓고
		// 현재 계층을 인자로 받은 계층의 Upper로 놓는다.
	SetUnderLayer(pUULayer);
	pUULayer->SetUpperLayer(this);
	///////////////////////////////////////////////////////////////////////
}

void CBaseLayer::SetUpperUnderLayer(CBaseLayer* pUULayer)
// 인자로 받은 계층을 현재 계층의 Upper로, 현재 계층을 인자로 받은 계층의 Under로 설정
{
	if (!pUULayer) // if the pointer is null, 경고문
	{
#ifdef _DEBUG
		TRACE("[CBaseLayer::SetUpperUnderLayer] The variable , 'pUULayer' is NULL");
#endif
		return;
	}

		// 인자로 받은 계층을 Upper에 놓고
		// 현재 계층은 Upper로 놓은 계층의 Under로 놓는다.
	SetUpperLayer(pUULayer);
	pUULayer->SetUnderLayer(this);
}

void CBaseLayer::SetUpperLayer(CBaseLayer* pUpperLayer)	
// 인자로 받은 계층을 현계층의 Upper로 설정
{
	if (!pUpperLayer) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE("[CBaseLayer::SetUpperLayer] The variable , 'pUpperLayer' is NULL");
#endif
		return;
	}

	// UpperLayer is added..
	this->mp_aUpperLayer[m_nUpperLayerCount++] = pUpperLayer; // 현 계층의 상위 계층 배열에 추가
}

void CBaseLayer::SetUnderLayer(CBaseLayer* pUnderLayer)
// 인자로 받은 계층을 현계층의 Under로 설정
{
	if (!pUnderLayer) // if the pointer is null, 경고문
	{
#ifdef _DEBUG
		TRACE("[CBaseLayer::SetUnderLayer] The variable , 'pUnderLayer' is NULL\n");
#endif
		return;
	}

	// UnderLayer assignment..
	this->mp_UnderLayer = pUnderLayer;	// 현계층의 하위 계층으로 한다.
}

CBaseLayer* CBaseLayer::GetUpperLayer(int nindex)
// 인덱스에 해당하는 상위 계층을 반환한다.
{
	if (nindex < 0 ||
		nindex > m_nUpperLayerCount ||
		m_nUpperLayerCount < 0)
	{
#ifdef _DEBUG
		TRACE("[CBaseLayer::GetUpperLayer] There is no UpperLayer in Array..\n");
#endif 
		return NULL;
	}

	return mp_aUpperLayer[nindex];
}

CBaseLayer* CBaseLayer::GetUnderLayer()
// 하위 계층을 반환한다.
{
	if (!mp_UnderLayer)
	{
#ifdef _DEBUG
		TRACE("[CBaseLayer::GetUnderLayer] There is not a UnerLayer..\n");
#endif 
		return NULL;
	}

	return mp_UnderLayer;
}

char* CBaseLayer::GetLayerName()	// 계층의 이름을 반환한다.
{
	return m_pLayerName;
}