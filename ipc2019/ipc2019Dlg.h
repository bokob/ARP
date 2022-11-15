
// ipc2019Dlg.h: 헤더 파일
//

#pragma once

#include "LayerManager.h"	// Added by ClassView
#include "IPLayer.h"
//#include "ARPLayer.h"
#include "EthernetLayer.h"	// Added by ClassView
#include "NILayer.h"
#include "ARPLayer.h"

// Cipc2019Dlg 대화 상자
class Cipc2019Dlg : public CDialogEx, public CBaseLayer
{
// 생성입니다.
public:
	Cipc2019Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.



// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IPC2019_DIALOG };
#endif

	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	UINT m_unDstAddr;
//	UINT unSrcAddr;
//	CString m_stMessage;
//	CListBox m_ListChat;
	
	afx_msg void OnTimer(UINT nIDEvent);


public:
	//BOOL			Receive(unsigned char* ppayload);
	//inline void		SendData();		// ChatAppLayer로 메시지 전송
	inline void		SendARP(unsigned char* destIP);
	unsigned char*  MacAddrToHexInt(CString ether);
	BOOL			ConvertHex(CString cs, unsigned char* hex);
	void			Refresh(unsigned char *ppayload);

	typedef struct _ARP_BODY // 28바이트
	{
		short hardType;							// 1
		short protType;							// 0x800
		char hardSize;							// 하드웨어 주소 길이
		char protSize;							// 프로토콜 주소의 길이
		short op;								// ARP에서 어떤 동작을 하는지 알려주기 위한 것 ARP request일 때 1, reply일 때 2
		char srcEthernetAddr[6];		// 출발 Ethernet 주소
		char srcIPAddr[4];				// 출발 IP 주소
		char targetEthernetAddr[6];	// 목적지 Ethernet 주소
		char targetIPAddr[4];			// 목적지 IP 주소
	} ARP_BODY, * PARP_BODY;


protected:
	ARP_BODY	m_arpBody;
	typedef CMap <int, int, ARPElement, ARPElement&> CMapForARPTable;
	CMapForARPTable ARPTable;
	CMapForARPTable ProxyTable;


private:
	CLayerManager	m_LayerMgr;
	int				m_nAckReady;

	enum {
		IPC_INITIALIZING,
		IPC_READYTOSEND,
		IPC_WAITFORACK,
		IPC_ERROR,
		IPC_BROADCASTMODE,
		IPC_UNICASTMODE,
		IPC_ADDR_SET,
		IPC_ADDR_RESET
	};

	void			SetDlgState(int state);
	inline void		EndofProcess();
	inline void		SetRegstryMessage();
	LRESULT			OnRegSendMsg(WPARAM wParam, LPARAM lParam);
	LRESULT			OnRegAckMsg(WPARAM wParam, LPARAM lParam);

	BOOL			m_bSendReady;

	unsigned char* m_srcMacAddress;		// 출발 Mac 주소
	unsigned char  m_dstMacAddress[6];	// 목적지 Mac 주소

	// Object App
	//CChatAppLayer* m_ChatApp;
	CIPLayer* m_IP;
	CARPLayer* m_ARP;
	CEthernetLayer* m_Ether;
	CNILayer* m_NI;

	// Implementation
	UINT			m_wParam;
	DWORD			m_lParam;

	CWinThread* m_RecvThread;
	static UINT	ReceiveThread(LPVOID pParam);		// 패킷 수신을 위한 스레드 함수
	static UINT SendIPThread(LPVOID pParam);		// IP주소지로 연결 호스트 검색시 프로그램이 잠드는것을 막기 위한 스레드

public:
	afx_msg void OnBnClickedButtonAddr();	// 설정 버튼 눌렀을 때 일어나는 이벤트
//	afx_msg void OnBnClickedButtonSend();	// send 버튼 눌렀을 때 일어나는 이벤트
//	UINT m_unSrcAddr;
//	UINT m_unDstAddr;
//	CString m_stMessage;
//	CListBox m_ListChat;
//	afx_msg void OnBnClickedCheckToall();	// 클릭 못하게 조절
	afx_msg void OnLbnSelchangeList4();
	CListCtrl m_ARPListView;
	CListCtrl m_PARPListView;
	afx_msg void OnLvnItemchangedArpList(NMHDR* pNMHDR, LRESULT* pResult);
	CComboBox m_Adapter;
	CString m_unSrcAddr;
	CString m_MacSrcAddr;
	CIPAddressCtrl m_IPSrcAddr;
//	CString m_IPDstAddr;
	afx_msg void OnCbnSelchangeAdapter();
	afx_msg void OnBnClickedArpAllDeleteButton();
	afx_msg void OnBnClickedArpItemDeleteButton();
	afx_msg void OnBnClickedButtonSend();
	CIPAddressCtrl m_IPDstAddr;
};
