// ProxyAddDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ipc2019.h"
#include "afxdialogex.h"
#include "ProxyAddDlg.h"


// CProxyAddDlg 대화 상자

IMPLEMENT_DYNAMIC(CProxyAddDlg, CDialogEx)

CProxyAddDlg::CProxyAddDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROXYADD_DIALOG, pParent)
	, m_EtherAddr(_T(""))
{
	
}

CProxyAddDlg::~CProxyAddDlg()
{
}

void CProxyAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDR, m_IPAddr);
	DDX_Text(pDX, IDC_EDIT_ETHERADDR, m_EtherAddr);
	DDX_Control(pDX, IDC_PADAPTER, m_PAdapter);
}


BEGIN_MESSAGE_MAP(CProxyAddDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CProxyAddDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CProxyAddDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_PADAPTER, &CProxyAddDlg::OnCbnSelchangePadapter)
END_MESSAGE_MAP()


// CProxyAddDlg 메시지 처리기


BOOL CProxyAddDlg::OnInitDialog()
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

	//추가 초기화 작업
	SetDlgState();

	m_PAdapter.AddString(_T("Default Device"));
	//p_EtherComboBox->AddString(_T("Default Device"));
	m_PAdapter.SetCurSel(0);

	return TRUE;
}

void CProxyAddDlg::SetDlgState()	// 영역별 들어갈 내용
{
	UpdateData(TRUE);

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_PADAPTER);

	pComboBox->EnableWindow(TRUE);

	UpdateData(FALSE);
}






void CProxyAddDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(true);

	bool isRight = false;

	if (m_EtherAddr.GetLength() == 17)
	{
		int i = 0;
		for (i; i < 17; i++)
		{
			char c = m_EtherAddr.GetAt(i);
			if ((i + 1) % 3 == 0)
			{
				if (c != ':')
					break;
			}
			else if (!((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9')))
				break;
		}
		if (i == 17)
			isRight = true;
	}
	if (m_EtherAddr.IsEmpty() || m_IPAddr.IsBlank())
	{
		AfxMessageBox("입력되지 않은 항목이 있음.");
		return;
	}
	else if (isRight == false)
	{
		AfxMessageBox("주소형식 이렇게 입력하셈( xx:xx:xx:xx:xx:xx )");
		return;
	}

	//cb_device.GetLBText(cb_device.GetCurSel(), m_buffer.device);

	m_IPAddr.GetAddress(IP[0], IP[1], IP[2], IP[3]);

	for (int i = 0; i < 10; i++)
	{
		CString temp;
		unsigned char char_temp[10];

		AfxExtractSubString(temp, m_EtherAddr, i, ':');
		sscanf(temp, "%x", &char_temp[i]);
		Ether[i] = char_temp[i];
	}

	CDialogEx::OnOK();
}


void CProxyAddDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CProxyAddDlg::OnCbnSelchangePadapter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_PAdapter.AddString(_T("Default Device"));

	//CComboBox* p_EtherComboBox = (CComboBox*)GetDlgItem(IDC_PADAPTER);
	//device_description.Trim();
	//m_PAdapter.AddString(_T("Default Device"));
	//p_EtherComboBox->AddString(_T("Default Device"));
	//m_PAdapter.SetCurSel(0);
}