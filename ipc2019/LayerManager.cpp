#pragma once
// LayerManager.cpp: implementation of the CLayerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "LayerManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayerManager::CLayerManager()
	: m_nLayerCount(0),	// Layer 개수
	mp_sListHead(NULL),	// Layer 앞부분
	mp_sListTail(NULL),	// Layer 뒷부분
	m_nTop(-1)
{

}

CLayerManager::~CLayerManager()	// 소멸자
{

}

void CLayerManager::AddLayer(CBaseLayer* pLayer)	// 계층 추가
{
	mp_aLayers[m_nLayerCount++] = pLayer;	
}

CBaseLayer* CLayerManager::GetLayer(int nindex)	// 인덱스에 해당하는 계층 반환
{
	return mp_aLayers[nindex];
}

CBaseLayer* CLayerManager::GetLayer(char* pName)	// 인자로 받은 이름을 지닌 계층 반환
{
	for (int i = 0; i < m_nLayerCount; i++)
	{
		if (!strcmp(pName, mp_aLayers[i]->GetLayerName()))
			return mp_aLayers[i];
	}

	return NULL;
}

void CLayerManager::ConnectLayers(char* pcList)	// 계층 연결
{
	MakeList(pcList);			// 입력받은 인자를 연결리스트로 만든다.
	LinkLayer(mp_sListHead);
	int arr;
	arr = 3;
}

void CLayerManager::MakeList(char* pcList)	// 표현식을 연결리스트로 만들어준다.
{
	while (pcList != (char*)0x01)
	{
		char sBuff[100];
		sscanf_s(pcList, "%s", sBuff, sizeof(sBuff));
		pcList = strchr(pcList, ' ') + 1;

		PNODE pNode = AllocNode(sBuff);
		AddNode(pNode);
	}
}

CLayerManager::PNODE CLayerManager::AllocNode(char* pcName)	// 계층들의 연결리스트의 노드 생성
{
	PNODE node = new NODE;
	ASSERT(node);

	strcpy_s(node->token, pcName);
	node->next = NULL;

	return node;
}

void CLayerManager::AddNode(PNODE pNode)	// 계층들의 연결리스트 뒤에 노드 추가
{
	if (!mp_sListHead)
	{
		mp_sListHead = mp_sListTail = pNode;
	}
	else
	{
		mp_sListTail->next = pNode;
		mp_sListTail = pNode;
	}
}


void CLayerManager::Push(CBaseLayer* pLayer) // 스택에 계층 push
{
	if (m_nTop >= MAX_LAYER_NUMBER)
	{
#ifdef _DEBUG
		TRACE("The Stack is full.. so cannot run the push operation.. \n");
#endif
		return;
	}

	mp_Stack[++m_nTop] = pLayer;
}

CBaseLayer* CLayerManager::Pop()	// 스택의 pop
{
	if (m_nTop < 0)
	{
#ifdef _DEBUG
		TRACE("The Stack is empty.. so cannot run the pop operation.. \n");
#endif
		return NULL;
	}

	CBaseLayer* pLayer = mp_Stack[m_nTop];
	mp_Stack[m_nTop] = NULL;
	m_nTop--;

	return pLayer;
}

CBaseLayer* CLayerManager::Top()	// 스택의 top 확인
{
	if (m_nTop < 0)
	{
#ifdef _DEBUG
		TRACE("The Stack is empty.. so cannot run the top operation.. \n");
#endif
		return NULL;
	}

	return mp_Stack[m_nTop];
}

void CLayerManager::LinkLayer(PNODE pNode)
// 연결리스트로 들어온 계층들을 연결한다. -> 표현식을 후위연산으로 바꾼 후에 계층들의 위아래를 판별해서 설정한다.
{
	CBaseLayer* pLayer = NULL;

	while (pNode)
	{
		if (!pLayer)
			pLayer = GetLayer(pNode->token);
		else
		{
			if (*pNode->token == '(')
				Push(pLayer);
			else if (*pNode->token == ')')
				Pop();
			else
			{
				char cMode = *pNode->token;
				char* pcName = pNode->token + 1;

				pLayer = GetLayer(pcName);

				switch (cMode)
				{
				case '*': Top()->SetUpperUnderLayer(pLayer); break;
				case '+': Top()->SetUpperLayer(pLayer); break;
				case '-': Top()->SetUnderLayer(pLayer); break;
				}
			}
		}

		pNode = pNode->next;
	}
}

void CLayerManager::DeAllocLayer()	// 계층들의 연결리스트 노드 해제
{
	for (int i = 0; i < this->m_nLayerCount; i++)
		delete this->mp_aLayers[i];
}
