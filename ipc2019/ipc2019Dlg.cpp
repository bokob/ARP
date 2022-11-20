
// ipc2019Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ipc2019.h"
#include "ipc2019Dlg.h"
#include "afxdialogex.h"
#include "ProxyAddDlg.h"

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
	// m_stDstAddr(_T(""))
	, m_strGratuitousAddr(_T(""))
{
	//대화상자 멤버 변수 초기화
	//m_unDstAddr = 0;
	// m_unSrcAddr = 0;
	//  m_stMessage = _T("");
	//대화 상자 멤버 초기화 완료

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//Protocol Layer Setting
	m_LayerMgr.AddLayer(new CIPLayer("IP"));
	m_LayerMgr.AddLayer(new CARPLayer("ARP"));
	m_LayerMgr.AddLayer(new CEthernetLayer("Ethernet"));
	m_LayerMgr.AddLayer(new CNILayer("NI"));
	m_LayerMgr.AddLayer(this);

	// 레이어를 연결한다. (레이어 생성)
	m_LayerMgr.ConnectLayers("NI ( *Ethernet ( *ARP ( *IP ( *ChatDlg ) ) +IP ) )");
	m_IP = (CIPLayer*)m_LayerMgr.GetLayer("IP");
	m_ARP = (CARPLayer*)m_LayerMgr.GetLayer("ARP");
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
	DDX_Control(pDX, IDC_SRC_IP_ADDR, m_IPSrcAddr);
	DDX_Control(pDX, IDC_EDIT_DST_IP_ADDR, m_IPDstAddr);
	DDX_Text(pDX, IDC_HW_ADDR, m_strGratuitousAddr);
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
	ON_BN_CLICKED(IDC_GRATUITOUS_SEND, &Cipc2019Dlg::OnBnClickedGratuitousSend)
	ON_BN_CLICKED(IDC_PARP_ADD_BUTTON, &Cipc2019Dlg::OnBnClickedParpAddButton)
	ON_BN_CLICKED(IDC_PARP_DELETE_BUTTON, &Cipc2019Dlg::OnBnClickedParpDeleteButton)
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

	for (i = 0; i < NI_COUNT_NIC; i++) 
	{
		if (!m_NI->GetAdapterObject(i))
			break;
		else 
		{
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
	//m_PARPListView.InsertColumn(3, _T("Status"), LVCFMT_CENTER, 150);

	// 테이블에서 한 행 전체 선택 가능하게 하기
	m_ARPListView.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_PARPListView.SetExtendedStyle(LVS_EX_FULLROWSELECT);

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


void Cipc2019Dlg::SetRegstryMessage()
{
	nRegSendMsg = RegisterWindowMessage(_T("Send IPC Message"));
	nRegAckMsg = RegisterWindowMessage(_T("Ack IPC Message"));

}

void Cipc2019Dlg::SendARP(unsigned char* destIP)
{
	if (destIP != NULL)
		mp_UnderLayer->Send(destIP);
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

	CButton* pSendButton = (CButton*)GetDlgItem(IDC_BUTTON_SEND);
	CButton* pSetAddrButton = (CButton*)GetDlgItem(bt_setting);
	CButton* pAllDeleteButton = (CButton*)GetDlgItem(IDC_ARP_ALL_DELETE_BUTTON);
	CButton* pItemDeleteButton = (CButton*)GetDlgItem(IDC_ARP_ITEM_DELETE_BUTTON);
	CButton* pPAddButton = (CButton*)GetDlgItem(IDC_PARP_ADD_BUTTON);
	CButton* pPDeleteButton = (CButton*)GetDlgItem(IDC_PARP_DELETE_BUTTON);
	//CEdit* pDstEdit = (CEdit*)GetDlgItem(IDC_EDIT_DST);
	/*
	CEdit* pMsgEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
	CEdit* pSrcEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	CEdit* pDstEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	*/
	switch (state)
	{
	
	case IPC_INITIALIZING:
		pSendButton->EnableWindow(FALSE);
		m_IPDstAddr.EnableWindow(FALSE);
		pAllDeleteButton->EnableWindow(FALSE);
		pItemDeleteButton->EnableWindow(FALSE);
		pPAddButton->EnableWindow(TRUE);
		pPDeleteButton->EnableWindow(TRUE);
		break;
	case IPC_ADDR_SET:	// Select 버튼 누름
		pSetAddrButton->SetWindowText(_T("재설정"));
		pSendButton->EnableWindow(TRUE);
		m_IPDstAddr.EnableWindow(TRUE);
		pAllDeleteButton->EnableWindow(TRUE);
		pItemDeleteButton->EnableWindow(TRUE);
		m_Adapter.EnableWindow(FALSE);
		m_IPSrcAddr.EnableWindow(FALSE);
		m_Adapter.EnableWindow(false);
		break;
	case IPC_ADDR_RESET:	// 재설정 버튼 누름
		pSetAddrButton->SetWindowText(_T("Select"));
		pSendButton->EnableWindow(FALSE);
		m_IPDstAddr.EnableWindow(FALSE);
		pAllDeleteButton->EnableWindow(FALSE);
		pItemDeleteButton->EnableWindow(FALSE);
		m_Adapter.EnableWindow(TRUE);
		m_IPSrcAddr.EnableWindow(TRUE);

		//m_MacSrcAddr = "";
		m_Adapter.EnableWindow(true);
		//m_Ether->SetSourceAddress((unsigned char*)"00000000");
		//m_Ether->SetDestinAddress((unsigned char*)"00000000");
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

void Cipc2019Dlg::OnBnClickedButtonAddr()	// select 버튼 눌렀을 때 일어나는 이벤트
{
	UpdateData(TRUE);
	if (!m_bSendReady) 
	{
		m_bSendReady = TRUE;
		SetDlgState(IPC_ADDR_SET);

		// Source Mac 주소 설정
		unsigned char* eth_temp = MacAddrToHexInt(m_MacSrcAddr);	
		ETHERNET_ADDR srcaddr;
		srcaddr.addr0 = eth_temp[0];
		srcaddr.addr1 = eth_temp[1];
		srcaddr.addr2 = eth_temp[2];
		srcaddr.addr3 = eth_temp[3];
		srcaddr.addr4 = eth_temp[4];
		srcaddr.addr5 = eth_temp[5];
		m_Ether->SetSourceAddress(srcaddr.addrs);

		// Destination Mac 주소 설정
		CString m_stDstAdd = "FF:FF:FF:FF:FF:FF";
		unsigned char* eth_temp2 = MacAddrToHexInt(m_stDstAdd);	
		ETHERNET_ADDR dstaddr;
		dstaddr.addr0 = eth_temp2[0];
		dstaddr.addr1 = eth_temp2[1];
		dstaddr.addr2 = eth_temp2[2];
		dstaddr.addr3 = eth_temp2[3];
		dstaddr.addr4 = eth_temp2[4];
		dstaddr.addr5 = eth_temp2[5];
		m_Ether->SetDestinAddress(dstaddr.addrs);

		// 내가 입력한 Source IP 주소 설정
		unsigned char SrcIP[4];
		m_IPSrcAddr.GetAddress(SrcIP[0], SrcIP[1], SrcIP[2], SrcIP[3]);	// SrcIP에 출발 IP 주소를 담는다.
		m_IP->SetSourceAddress(SrcIP);
		
		m_NI->SetAdapterNumber(m_Adapter.GetCurSel());
		m_NI->PacketStartDriver();

		// 설정 눌렀을 때, 모든 출발 주소가 설정된 후 쓰레드 시작
		m_RecvThread = AfxBeginThread(ReceiveThread, (LPVOID)m_NI);
	}
	else 
	{
		// 재설정을 하면 연결이 끊어진다. 그러므로 쓰레드 종료
		TerminateThread(m_RecvThread->m_hThread, 0);

		m_bSendReady = FALSE;
		m_ARP->initARPTable();
		m_ARPListView.DeleteAllItems();
		SetDlgState(IPC_ADDR_RESET);
		SetDlgState(IPC_INITIALIZING);
	}
	UpdateData(FALSE);
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


unsigned char* Cipc2019Dlg::MacAddrToHexInt(CString ether)
{
	// 콜론(:)으로 구분 되어진 Ethernet 주소를
	// 콜론(:)을 토큰으로 한 바이트씩 값을 가져와서 Ethernet배열에 넣어준다.
	CString cstr;
	unsigned char* file_ether = (u_char*)malloc(sizeof(u_char) * 6);

	for (int i = 0; i < 6; i++)
	{
		AfxExtractSubString(cstr, ether, i, ':');
		// strtoul -> 문자열을 원하는 진수로 변환 시켜준다.
		file_ether[i] = (unsigned char)strtoul(cstr.GetString(), NULL, 16);
	}
	file_ether[6] = '\0';

	return file_ether;
}

BOOL Cipc2019Dlg::ConvertHex(CString cs, unsigned char* hex)//change string to hex
{
	int i;
	char* srcStr = cs.GetBuffer(0);

	for (i = 0; i < 12; i++) {
		// error
		if (srcStr[i] < '0' || (srcStr[i] > '9' && srcStr[i] < 'a') || srcStr[i] > 'f')
			return FALSE;
	}
	for (i = 0; i < 12; i = i + 2) {
		hex[i / 2] = (((srcStr[i] > '9') ? (srcStr[i] - 87) : (srcStr[i] - '0')) << 4 |
			((srcStr[i + 1] > '9') ? (srcStr[i + 1] - 87) : (srcStr[i + 1] - '0')));
	}
	return TRUE;
}


BOOL Cipc2019Dlg::ConvertStringToIP(CString cs, unsigned char* ip) //change string to hex
{
	int j = 0;
	LPCSTR ipString = cs.GetString();

	memset(ip, 0, 4);

	for (int i = 0; i < 4; i++)
	{
		while (ipString[j] != NULL && ipString[j] != '.')
		{
			ip[i] *= 10;
			ip[i] += ipString[j] - '0';
			j++;
		}
		j++;
	}

	return true;
}




void Cipc2019Dlg::OnCbnSelchangeAdapter() // 어댑터 설정하면 Source Mac 주소 설정
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	// 어댑터 목록 가져오기
	CComboBox* pEX_EtherComboBox = (CComboBox*)GetDlgItem(IDC_ADAPTER);

	// 선택된 NIC의 인덱스 번호 가져오기
	int cIndex = pEX_EtherComboBox->GetCurSel();

	// 가져온 인덱스 번호로 Adapter 이름 가져오기
	CString nicName = m_NI->GetAdapterObject(cIndex)->name;

	// 어댑터 이름으로 mfc에서 mac 주소 나오는 곳에 Mac주소 넣기
	m_MacSrcAddr = m_NI->GetNICardAddress((char*)nicName.GetString());

	UpdateData(FALSE);
}


void Cipc2019Dlg::OnBnClickedArpAllDeleteButton()	// ARP 테이블의 모든 행 삭제
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ARPListView.DeleteAllItems();
	m_ARP->initARPTable();
}


void Cipc2019Dlg::OnBnClickedArpItemDeleteButton()	// ARP 테이블이 선택한 한 행 삭제
{
	/*
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION pos;
	pos = m_ARPListView.GetFirstSelectedItemPosition();
	int idx = m_ARPListView.GetNextSelectedItem(pos);
	m_ARPListView.DeleteItem(idx);
	*/
	
	UpdateData(TRUE);

	POSITION pos = m_ARPListView.GetFirstSelectedItemPosition();
	int idx = m_ARPListView.GetNextSelectedItem(pos);
	CString strIP = m_ARPListView.GetItemText(idx, 0);
	m_ARPListView.DeleteItem(idx);

	unsigned char* deletedIP = new unsigned char[4];

	ConvertStringToIP(strIP, deletedIP);
	m_ARP->deleteOneARPTable(deletedIP);

	Refresh();

	UpdateData(FALSE);

}


void Cipc2019Dlg::OnBnClickedButtonSend()	// 목적지 IP 주소 입력하는 곳 옆 send 버튼 눌렀을 때
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (!m_IPDstAddr.IsBlank()) // 무엇인가 적혀 있다면
	{	
		// ARP Table에 추가 시작

		int row = m_ARPListView.GetItemCount();	// ARP Table에 행이 얼마인지 구한다.
		

		// CIPAddresssCtrl을 문자열로 바꾸는 과정
		BYTE ipFirst, ipSecond, ipThird, ipForth;
		m_IPDstAddr.GetAddress(ipFirst, ipSecond, ipThird, ipForth);	// 목적지 ip 주소를 각 변수에 나눠서 저장한다.
		char szIPAddr[30];

		// CIPAddressCtrl을 문자열로 바꾼 후 ARP 테이블에 추가한다.
		::wsprintf(szIPAddr, "%d.%d.%d.%d", ipFirst, ipSecond, ipThird, ipForth);
		m_ARPListView.InsertItem(row, szIPAddr);	// 행 추가
		m_ARPListView.SetItemText(row, 1, _T("00:00:00:00:00:00"));	// 1열에 목적지의 Mac주소 모르니까 초기화
		m_ARPListView.SetItemText(row, 2, _T("incomplete"));
		
		// 사용자가 입력한 목적지 IP 주소를 세팅한다.
		unsigned char DstIP[4];
		m_IPDstAddr.GetAddress(DstIP[0], DstIP[1], DstIP[2], DstIP[3]);	// DstIP에 목적지 IP 주소를 담는다.
		m_IP->SetDestinAddress(DstIP);
		//m_ARP->SetDestinProtoAddress((unsigned char*)ip_dstAddr);

		unsigned char* destIP = (unsigned char*)szIPAddr;
		SendARP(destIP);


		// ARP 테이블 갱신
		Refresh();
	}
	else
	{
		AfxMessageBox(" 목적지 IP 주소 입력 안됨. 다시 확인하셈 ");
	}
	UpdateData(FALSE);
}

/*
BOOL Cipc2019Dlg::Receive(unsigned char* ppayload) 
{ 
	ARP_BODY* recivedARP = (ARP_BODY*)ppayload;

	if (recivedARP->op == ARP_REQUEST)
	{
		memcpy(m_arpBody.srcEthernetAddr, ((CARPLayer*)mp_UnderLayer)->GetSourceIPAddress(), 4);
		// 송신측의 IP 주소를 저장한다.

		memcpy(m_arpBody.srcIPAddr, ((CARPLayer*)mp_aUpperLayer[0])->GetSourceEtherAddress(), 6);	
		// 송신측의 Ethernet 주소를 넣는다.


		
	}
	else if(recivedARP->op == ARP_REPLY)


	return FALSE; 
}
*/


void Cipc2019Dlg::Refresh()
{
	// ARP 테이블 갱신
	char** keyIPTable = NULL;
	int tableSize;

	ARPElement* arpElements = m_ARP->getARPElements(&keyIPTable, &tableSize);

	m_ARPListView.DeleteAllItems();

	for (int i = 0; i < tableSize; i++)
	{
		CString ipAddr;
		ipAddr.Format("%.2u.%.2u.%.2u.%.2u",
			(unsigned char)keyIPTable[i][0], (unsigned char)keyIPTable[i][1], (unsigned char)keyIPTable[i][2],
			(unsigned char)keyIPTable[i][3]);

		CString macAddr;
		if (arpElements[i].MACAddr != NULL)
			macAddr.Format("%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",
				arpElements[i].MACAddr[0], arpElements[i].MACAddr[1], arpElements[i].MACAddr[2],
				arpElements[i].MACAddr[3], arpElements[i].MACAddr[4], arpElements[i].MACAddr[5]);
		else
			macAddr.Format("00:00:00:00:00:00");

		m_ARPListView.InsertItem(i, ipAddr);
		m_ARPListView.SetItem(i, 1, LVIF_TEXT, macAddr, 0, 0, 0, NULL);
		if (arpElements[i].state == ARP_COMPLETE)
			m_ARPListView.SetItem(i, 2, LVIF_TEXT, "complete", 0, 0, 0, NULL);
		else
			m_ARPListView.SetItem(i, 2, LVIF_TEXT, "incomplete", 0, 0, 0, NULL);
	}
}

void Cipc2019Dlg::RefreshP()
{
	// PARP 테이블 갱신
	char** keyIPTable = NULL;
	int tableSize;

	ARPElement* proxyElements = m_ARP->getProxyElements(&keyIPTable, &tableSize);

	m_PARPListView.DeleteAllItems();

	for (int i = 0; i < tableSize; i++)
	{
		CString ipAddr;
		ipAddr.Format("%.2u.%.2u.%.2u.%.2u",
			(unsigned char)keyIPTable[i][0], (unsigned char)keyIPTable[i][1], (unsigned char)keyIPTable[i][2],
			(unsigned char)keyIPTable[i][3]);

		CString macAddr;
		if (proxyElements[i].MACAddr != NULL)
			macAddr.Format("%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",
				proxyElements[i].MACAddr[0], proxyElements[i].MACAddr[1], proxyElements[i].MACAddr[2],
				proxyElements[i].MACAddr[3], proxyElements[i].MACAddr[4], proxyElements[i].MACAddr[5]);
		else
			macAddr.Format("00:00:00:00:00:00");

		m_PARPListView.InsertItem(i, _T("Default Device"));
		m_PARPListView.SetItem(i, 1, LVIF_TEXT, ipAddr, 0, 0, 0, NULL);
		m_PARPListView.SetItem(i, 2, LVIF_TEXT, macAddr, 0, 0, 0, NULL);

		/*
		if (proxyElements[i].state == ARP_COMPLETE)
			m_PARPListView.SetItem(i, 3, LVIF_TEXT, "COMPLETE", 0, 0, 0, NULL);
		else
			m_PARPListView.SetItem(i, 3, LVIF_TEXT, "INCOMPLETE", 0, 0, 0, NULL);
		*/
	}
}



BOOL Cipc2019Dlg::Receive(unsigned char* ppayload) // ARP Reply 받았을 때 Refresh() 함수 실행시키려고 만든 Receive()
{
	BOOL bSuccess = FALSE;

	ARP_BODY* recivedARP = (ARP_BODY*)ppayload;

	Refresh();

	/*
	if (ppayload != NULL)
	{
		bSuccess = TRUE;
		//AfxMessageBox("reply 잘 날라옴");
	} 
	*/

	
	return bSuccess;
}

UINT Cipc2019Dlg::ReceiveThread(LPVOID pParam)	// 패킷 수신을 위한 스레드 함수
{
	((CNILayer*)pParam)->Receive();	// 쓰레드를 이용해 NILayer의 Receive 함수를 호출

	return 0;
}

void Cipc2019Dlg::OnBnClickedGratuitousSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (!m_strGratuitousAddr.IsEmpty())
	{
		/*
		if (m_strGratuitousAddr.GetLength() != 12)
		{
			MessageBox("MAC 주소 12자리 입력하셈");
			return;
		}
		*/

		// 입력받은 H/W 주소를 Source Mac 주소로 설정
		unsigned char* eth_temp = MacAddrToHexInt(m_strGratuitousAddr);
		ETHERNET_ADDR srcaddr;
		srcaddr.addr0 = eth_temp[0];
		srcaddr.addr1 = eth_temp[1];
		srcaddr.addr2 = eth_temp[2];
		srcaddr.addr3 = eth_temp[3];
		srcaddr.addr4 = eth_temp[4];
		srcaddr.addr5 = eth_temp[5];
		m_Ether->SetSourceAddress(srcaddr.addrs);

		// Destination Mac 주소 설정
		CString m_stDstAdd = "FF:FF:FF:FF:FF:FF";
		unsigned char* eth_temp2 = MacAddrToHexInt(m_strGratuitousAddr);
		ETHERNET_ADDR dstaddr;
		dstaddr.addr0 = eth_temp2[0];
		dstaddr.addr1 = eth_temp2[1];
		dstaddr.addr2 = eth_temp2[2];
		dstaddr.addr3 = eth_temp2[3];
		dstaddr.addr4 = eth_temp2[4];
		dstaddr.addr5 = eth_temp2[5];
		m_Ether->SetDestinAddress(dstaddr.addrs);

		SendARP(m_IP->GetSourceAddress());
	}

	UpdateData(FALSE);
}


void Cipc2019Dlg::OnBnClickedParpAddButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	unsigned char _hexProxyMacAddr[6];
	unsigned char _proxyIPAddr[4];

	CProxyAddDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		//dlg.m_EtherAddr;
		//AfxMessageBox(dlg.m_EtherAddr); 잘 가져옴
		//dlg.m_IPAddr;

		memcpy(_hexProxyMacAddr, dlg.Ether, 6);
		memcpy(_proxyIPAddr, dlg.IP, 4);
		m_ARP->insertProxyTable(_proxyIPAddr, _hexProxyMacAddr);
		RefreshP();
	}

	UpdateData(FALSE);

	/*
	UpdateData(TRUE);

	unsigned char _hexProxyMacAddr[6];
	unsigned char _proxyIPAddr[4];

	if (m_proxyMacAddr.GetLength() != 12)
	{
		MessageBox("MAC 주소 12자리를 입력해 주세요.");
		return;
	}

	ConvertHex(m_proxyMacAddr, _hexProxyMacAddr);	// CString change to Hex
	m_proxyMacAddr.Format("");

	m_proxyIPAddr.GetAddress(_proxyIPAddr[0], _proxyIPAddr[1], _proxyIPAddr[2], _proxyIPAddr[3]);
	m_proxyIPAddr.ClearAddress();

	m_ARP->insertProxyTable(_proxyIPAddr, _hexProxyMacAddr);
	RefreshProxyTable();

	UpdateData(FALSE);
	*/
}


void Cipc2019Dlg::OnBnClickedParpDeleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	POSITION pos = m_PARPListView.GetFirstSelectedItemPosition();
	int nItem = m_PARPListView.GetNextSelectedItem(pos);
	CString strIP = m_PARPListView.GetItemText(nItem, 0);

	unsigned char* deletedIP = new unsigned char[4];

	ConvertStringToIP(strIP, deletedIP);
	m_ARP->deleteProxyTable(deletedIP);

	RefreshP();

	UpdateData(FALSE);
}
