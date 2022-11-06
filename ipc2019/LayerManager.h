// LayerManager.h: interface for the CLayerManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYERMANAGER_H__D9F8CF34_8A6D_425A_BDB9_47A4874FF902__INCLUDED_)
#define AFX_LAYERMANAGER_H__D9F8CF34_8A6D_425A_BDB9_47A4874FF902__INCLUDED_

#include "pch.h"
#include "BaseLayer.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLayerManager
{
private:
	typedef struct _NODE {

		char			token[50];		// 표현식 담을 배열
		struct _NODE* next;	

	} NODE, * PNODE;

public:
	void			DeAllocLayer();					// 계층들의 연결리스트 노드 해제

	void			ConnectLayers(char* pcList);	// 계층 연결
	CBaseLayer* GetLayer(char* pName);				// 인자로 받은 이름을 지닌 계층 반환
	CBaseLayer* GetLayer(int nindex);				// 인덱스에 해당하는 계층 반환		
	void			AddLayer(CBaseLayer* pLayer);	// 계층 추가

	CLayerManager();			// 생성자
	virtual ~CLayerManager();	// 소멸자

private:
	// about stack...
	int				m_nTop;					// 스택의 top을 나타낼 변수
	CBaseLayer* mp_Stack[MAX_LAYER_NUMBER];	// 스택

	CBaseLayer* Top();	// 스택의 top 확인
	CBaseLayer* Pop();	// 스택의 pop
	void			Push(CBaseLayer* pLayer);

	PNODE			mp_sListHead;	// Layer 앞부분
	PNODE			mp_sListTail;	// Layer 뒷부분

	// about Link Layer...
	void			LinkLayer(PNODE pNode);		
	// 연결리스트로 들어온 계층들을 연결한다. -> 표현식을 후위연산으로 바꾼 후에 계층들의 위아래를 판별해서 설정한다.

	inline void		AddNode(PNODE pNode);		// 계층들의 연결리스트 뒤에 노드 추가
	inline PNODE	AllocNode(char* pcName);	// 계층들의 연결리스트의 노드 생성
	void			MakeList(char* pcList);		// 표현식을 연결리스트로 만들어준다.

	int				m_nLayerCount;				// 계층 개수
	CBaseLayer* mp_aLayers[MAX_LAYER_NUMBER];	// 계층들의 배열

};

#endif // !defined(AFX_LAYERMANAGER_H__D9F8CF34_8A6D_425A_BDB9_47A4874FF902__INCLUDED_)
