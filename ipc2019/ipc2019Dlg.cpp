
// ipc2019Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ipc2019.h"
#include "ipc2019Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);	// 대화 상자 데이터 교환
}


/*
클래스의 멤버 함수를 정의하는 구현(.cpp) 파일에서 
매크로를 BEGIN_MESSAGE_MAP 사용하여 
메시지 맵을 시작한 다음 각 메시지 처리기 함수에 대한 매크로 항목을 추가하고 
매크로를 사용하여 메시지 맵을 END_MESSAGE_MAP 완료합니다.
*/

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cipc2019Dlg 대화 상자


Cipc2019Dlg::Cipc2019Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IPC2019_DIALOG, pParent)
	, CBaseLayer("ChatDlg")
	, m_bSendReady(FALSE)
	, m_nAckReady( -1 )
	//, m_unDstAddr(0)
	, m_MacSrcAddr(_T(""))
	//, IPDstAddr(_T(""))
	//, m_IPDstAddr(_T(""))
	, m_stDstAddr(_T(""))
{
	//대화상자 멤버 변수 초기화
	//  m_unDstAddr = 0;
	//  unSrcAddr = 0;
	//  m_stMessage = _T("");
	//대화 상자 멤버 초기화 완료

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//Protocol Layer Setting
	m_LayerMgr.AddLayer(new CIPLayer("IP"));
	//m_LayerMgr.AddLayer(new CARPLayer("ARP"));
	m_LayerMgr.AddLayer(new CEthernetLayer("Ethernet"));
	m_LayerMgr.AddLayer(new CNILayer("NI"));
	m_LayerMgr.AddLayer(this);

	// 레이어를 연결한다. (레이어 생성)
	//m_LayerMgr.ConnectLayers("NI ( *Ethernet ( *ARP ( *IP ( *ChatDlg ) ) +IP ) )");
	m_IP = (CIPLayer*)m_LayerMgr.GetLayer("IP");
	//m_ARP = (CARPLayer*)m_LayerMgr.GetLayer("ARP");
	m_Ether = (CEthernetLayer*)m_LayerMgr.GetLayer("Ethernet");
	m_NI = (CNILayer*)m_LayerMgr.GetLayer("NI");
	//Protocol Layer Setting
}

void Cipc2019Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT_SRC, m_unSrcAddr);		// source 주소 입력하는 곳
	//DDX_Text(pDX, IDC_EDIT_DST, m_unDstAddr);		// destination 주소 입력하는 곳
	//DDX_Text(pDX, IDC_EDIT_MSG, m_stMessage);		// 메시지 입력하는 곳
	//DDX_Control(pDX, IDC_LIST_CHAT, m_ListChat);	// 메시지들이 출력되는 곳
	DDX_Text(pDX, IDC_EDIT_SRC_MAC_ADDR, m_MacSrcAddr); // Source Mac 주소 나오는 곳
	DDX_Control(pDX, IDC_ARP_LIST, m_ARPListView);
	DDX_Control(pDX, IDC_PARP_LIST, m_PARPListView);
	DDX_Control(pDX, IDC_ADAPTER, m_Adapter);
	//  DDX_Control(pDX, IDC_EDIT_SRC_MAC_ADDR, m_unSrcAddr);
	//  DDX_Control(pDX, IDC_EDIT_DST_IP_ADDR, m_IPDstAddr);
	DDX_Control(pDX, IDC_SRC_IP_ADDR, m_IPSrcAddr);
	//  DDX_Control(pDX, IDC_EDIT_DST_IP_ADDR, IPDstAddr);
	//  DDX_Control(pDX, IDC_EDIT_DST_IP_ADDR, m_IPDstAddr);
	//  DDX_Control(pDX, IDC_EDIT_DST_IP_ADDR, m_IPDstAddr);
	DDX_Text(pDX, IDC_EDIT_DST_IP_ADDR, m_stDstAddr);
}

// 레지스트리에 등록하기 위한 변수
UINT nRegSendMsg;
UINT nRegAckMsg;
// 레지스트리에 등록하기 위한 변수


BEGIN_MESSAGE_MAP(Cipc2019Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADDR, &Cipc2019Dlg::OnBnClickedButtonAddr)	// Select 버튼
	ON_BN_CLICKED(IDC_BUTTON_SEND, &Cipc2019Dlg::OnBnClickedButtonSend)	// 목적지 IP Adress 옆에 있는 send 버튼
//	ON_BN_CLICKED(IDC_ARP_ITEM_DELETE_BUTTON, &Cipc2019Dlg::OnBnClickedButtonSend)
// 
	
	
	
	ON_WM_TIMER()

	/*
	ON_REGISTERED_MESSAGE(nRegSendMsg, OnRegSendMsg)	  //Send 레지스터 등록
	//////////////////////// fill the blank ///////////////////////////////
		// Ack 레지스터 등록
	ON_REGISTERED_MESSAGE(nRegAckMsg, OnRegAckMsg)
	///////////////////////////////////////////////////////////////////////
	*/
	
	//ON_BN_CLICKED(IDC_CHECK_TOALL, &Cipc2019Dlg::OnBnClickedCheckToall) // 클릭 못하게 조절
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ARP_LIST, &Cipc2019Dlg::OnLvnItemchangedArpList)
	ON_CBN_SELCHANGE(IDC_ADAPTER, &Cipc2019Dlg::OnCbnSelchangeAdapter)
	ON_BN_CLICKED(IDC_ARP_ALL_DELETE_BUTTON, &Cipc2019Dlg::OnBnClickedArpAllDeleteButton)
	ON_BN_CLICKED(IDC_ARP_ITEM_DELETE_BUTTON, &Cipc2019Dlg::OnBnClickedArpItemDeleteButton)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &Cipc2019Dlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// Cipc2019Dlg 메시지 처리기

BOOL Cipc2019Dlg::OnInitDialog()	// 로그인 다이얼로그 생성
{
	CDialogEx::OnInitDialog();	// 로그인 다이얼롤그 생성

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetRegstryMessage();
	SetDlgState(IPC_INITIALIZING);


	// 어댑터 목록 가져오기
	CComboBox* p_EtherComboBox = (CComboBox*)GetDlgItem(IDC_ADAPTER);
	int i;
	CString device_description;

	for (i = 0; i < NI_COUNT_NIC; i++) {
		if (!m_NI->GetAdapterObject(i))
			break;
		else {
			device_description = m_NI->GetAdapterObject(i)->description;
			device_description.Trim();
			p_EtherComboBox->AddString(device_description);
		}
	}
	p_EtherComboBox->SetCurSel(0);

	CString inNicName = m_NI->GetAdapterObject(0)->description;

	// ARP Cache 테이블의 헤더 채우기
	// n번째 열에 "???" 컬럼 추가. LVCFMT_CENTER: 가운데 정렬 , 컬럼 폭
	m_ARPListView.InsertColumn(0, _T("IP Address"), LVCFMT_CENTER, 150);	
	m_ARPListView.InsertColumn(1, _T("Ethernet Address"), LVCFMT_CENTER, 150);
	m_ARPListView.InsertColumn(2, _T("Status"), LVCFMT_CENTER, 150);

	// Proxy ARP Cache 테이블의 헤더 채우기
	m_PARPListView.InsertColumn(0, _T("Device"), LVCFMT_CENTER, 150);
	m_PARPListView.InsertColumn(1, _T("IP Address"), LVCFMT_CENTER, 150);
	m_PARPListView.InsertColumn(2, _T("Ethernet Address"), LVCFMT_CENTER, 150);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void Cipc2019Dlg::OnSysCommand(UINT nID, LPARAM lParam) // 메뉴창 조절 함수
{	
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)	// 메뉴창 종료
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);		// 메뉴창 마우스로 이동
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Cipc2019Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Cipc2019Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




//void Cipc2019Dlg::OnBnClickedButtonSend()	// send 버튼 눌렀을 때 일어나는 이벤트(전송)
//{
//	UpdateData(TRUE);
//
//	if (!m_stDstAddr.IsEmpty())
//	{
//		/*
//		SetTimer(1, 2000, NULL);
//		m_nAckReady = 0;
//		*/
//
//		unsigned char IP[4];
//		m_IPDstAddr.GetAddress(IP[0], IP[1], IP[2], IP[3]);
//		m_stDstAddr = "";
		//m_stMessage = "";

//		(CEdit*)GetDlgItem(IDC_EDIT_DST_IP_ADDR)->SetFocus();
//	}
//
//	UpdateData(FALSE);
//}

void Cipc2019Dlg::SetRegstryMessage()
{
	nRegSendMsg = RegisterWindowMessage(_T("Send IPC Message"));
	nRegAckMsg = RegisterWindowMessage(_T("Ack IPC Message"));

}


void Cipc2019Dlg::SendData()	// ChatAppLayer로 메시지 전송
{
	CString MsgHeader;
	/*
	if (m_unDstAddr == (unsigned int)0xff)
		MsgHeader.Format(_T("[%d:BROADCAST] "), m_unSrcAddr);
	else
		MsgHeader.Format(_T("[%d:%d] "), m_unSrcAddr, m_unDstAddr);

	m_ListChat.AddString(MsgHeader + m_stMessage);
	*/
	/*
	int nlength = m_stMessage.GetLength();
	unsigned char* ppayload = new unsigned char[nlength + 1];
	memcpy(ppayload, (unsigned char*)(LPCTSTR)m_stMessage, nlength);
	ppayload[nlength] = '\0';
	*/

	// 보낼 data와 메시지 길이를 Send함수로 넘겨준다.
	//m_ChatApp->Send(ppayload, nlength);
}


BOOL Cipc2019Dlg::Receive(unsigned char* ppayload) // ChatAppLayer로부터 메시지 수신
{
	if (m_nAckReady == -1)
	{
	}

	//m_ListChat.AddString((LPCTSTR)ppayload);
	return TRUE;
}

BOOL Cipc2019Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (::GetDlgCtrlID(::GetFocus()) == IDC_EDIT3)
				OnBnClickedButtonSend();					
			return FALSE;
		case VK_ESCAPE: return FALSE;
		}
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void Cipc2019Dlg::SetDlgState(int state)	// 영역별 들어갈 내용
{
	UpdateData(TRUE);

	CButton* pChkButton = (CButton*)GetDlgItem(IDC_CHECK1);

	CButton* pSendButton = (CButton*)GetDlgItem(bt_send);
	CButton* pSetAddrButton = (CButton*)GetDlgItem(bt_setting);
	/*
	CEdit* pMsgEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
	CEdit* pSrcEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	CEdit* pDstEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	*/
	switch (state)
	{
	/*
	case IPC_INITIALIZING:
		pSendButton->EnableWindow(FALSE);
		pMsgEdit->EnableWindow(FALSE);
		//m_ListChat.EnableWindow(FALSE);
		break;
	case IPC_READYTOSEND:
		pSendButton->EnableWindow(TRUE);
		pMsgEdit->EnableWindow(TRUE);
		//m_ListChat.EnableWindow(TRUE);
		break;
	case IPC_WAITFORACK:	break;
	case IPC_ERROR:		break;
	case IPC_UNICASTMODE:
		//m_unDstAddr = 0x0;
		pDstEdit->EnableWindow(TRUE);
		break;
	case IPC_BROADCASTMODE:
		//m_unDstAddr = 0xff;
		pDstEdit->EnableWindow(FALSE);
		break;
	*/
	case IPC_ADDR_SET:
		pSetAddrButton->SetWindowText(_T("재설정"));
		/*
		pSrcEdit->EnableWindow(FALSE);
		pDstEdit->EnableWindow(FALSE);
		pChkButton->EnableWindow(FALSE);
		*/
		m_Adapter.EnableWindow(false);
		break;
	case IPC_ADDR_RESET:
		pSetAddrButton->SetWindowText(_T("Select"));
		/*
		pSrcEdit->EnableWindow(TRUE);
		if (!pChkButton->GetCheck())
			pDstEdit->EnableWindow(TRUE);
		pChkButton->EnableWindow(TRUE);
		*/
		m_MacSrcAddr = "";
		m_Adapter.EnableWindow(true);
		m_Ether->SetSourceAddress((unsigned char*)"00000000");
		m_Ether->SetDestinAddress((unsigned char*)"00000000");
		break;
	}

	UpdateData(FALSE);
}


void Cipc2019Dlg::EndofProcess()
{
	m_LayerMgr.DeAllocLayer();
}


// Send메시지 레지스트리가 켜졌을 때
LRESULT Cipc2019Dlg::OnRegSendMsg(WPARAM wParam, LPARAM lParam)
{
	//////////////////////// fill the blank ///////////////////////////////
	if (m_nAckReady) {
		// File 레이어에서 상대방이 전송한 메시지가 담긴 파일을 가져옴
		if (m_LayerMgr.GetLayer("File")->Receive())
		{
			// 메시지를 받았다면 Ack 신호를 브로드캐스트로 날린다.
			::SendMessage(HWND_BROADCAST, nRegAckMsg, 0, 0);
		}
	}
	///////////////////////////////////////////////////////////////////////
	return 0;
}

LRESULT Cipc2019Dlg::OnRegAckMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_nAckReady) { // Ack 신호를 받으면 타이머를 멈춘다.
		m_nAckReady = -1;
		KillTimer(1);
	}

	return 0;
}

void Cipc2019Dlg::OnTimer(UINT nIDEvent)	// 타이머
{
	// TODO: Add your message handler code here and/or call default
	//m_ListChat.AddString(_T(">> The last message was time-out.."));
	m_nAckReady = -1;
	KillTimer(1);

	CDialog::OnTimer(nIDEvent);
}



void Cipc2019Dlg::OnBnClickedButtonAddr()	// 설정 버튼 눌렀을 때 일어나는 이벤트
{
	UpdateData(TRUE);

	/*
	if (!m_unDstAddr ||
		!m_unSrcAddr)
	{
		AfxMessageBox(_T("주소를 설정 오류발생",
			"경고"),
			MB_OK | MB_ICONSTOP);

		return;
	}*/

	if (m_bSendReady) {
		SetDlgState(IPC_ADDR_RESET);
		SetDlgState(IPC_INITIALIZING);
	}
	else {
		/*
		m_ChatApp->SetSourceAddress(m_unSrcAddr);
		m_ChatApp->SetDestinAddress(m_unDstAddr);
		*/
		SetDlgState(IPC_ADDR_SET);
		SetDlgState(IPC_READYTOSEND);
	}

	m_bSendReady = !m_bSendReady;
}


/*
void Cipc2019Dlg::OnBnClickedCheckToall()	// 클릭 못하게 조절
{
	CButton* pChkButton = (CButton*)GetDlgItem(IDC_CHECK_TOALL);

	if (pChkButton->GetCheck()) {
		SetDlgState(IPC_BROADCASTMODE);
	}
	else {
		SetDlgState(IPC_UNICASTMODE);
	}
}
*/


void Cipc2019Dlg::OnLvnItemchangedArpList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


BOOL Cipc2019Dlg::ConvertHex(CString cs, unsigned char* hex) // 문자열을 Hex로 바꾸는 함수
{
	int i;
	char* srcStr = cs.GetBuffer(0);

	for (i = 0; i < 12; i++)
	{
		// error
		if (srcStr[i] < '0' || (srcStr[i] > '9' && srcStr[i] < 'a') || srcStr[i] > 'f')
			return FALSE;
	}
	for (i = 0; i < 12; i = i + 2)
	{
		hex[i / 2] = (((srcStr[i] > '9') ? (srcStr[i] - 87) : (srcStr[i] - '0')) << 4 |
			((srcStr[i + 1] > '9') ? (srcStr[i + 1] - 87) : (srcStr[i + 1] - '0')));
	}
	return TRUE;
}

void Cipc2019Dlg::OnCbnSelchangeAdapter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cipc2019Dlg::OnBnClickedArpAllDeleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cipc2019Dlg::OnBnClickedArpItemDeleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cipc2019Dlg::OnBnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (!m_stDstAddr.IsEmpty())
	{
		/*
		SetTimer(1, 2000, NULL);
		m_nAckReady = 0;
		

		SendData();
		m_stMessage = "";
		*/

		unsigned char IP[4];
		m_IPDstAddr.GetAddress(IP[0], IP[1], IP[2], IP[3]);
		// ARP 보내는 부분

		//

		m_stDstAddr = "";

		(CEdit*)GetDlgItem(IDC_EDIT_DST_IP_ADDR)->SetFocus();
	}

	UpdateData(FALSE);
}


BOOL Cipc2019Dlg::ConvertStringToIP(CString cs, unsigned char* IP)
{
	int j = 0;
	LPCSTR IPstring = cs.GetString();
	memset(IP, 0, 4);

	for (int i = 0; i < 4; i++)
	{
		while (IPstring[j] != NULL && IPstring[j] != '.')
		{
			IP[i] *= 10;
			IP[i] += IPstring[j] - '0';
			j++;
		}
		j++;
	}
	return true;
}