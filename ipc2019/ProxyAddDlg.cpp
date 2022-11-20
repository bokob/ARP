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
		AfxMessageBox("입력되지 않은 항목이 있습니다.");
		return;
	}
	else if (isRight == false)
	{
		AfxMessageBox("주소형식이 올바르지 않습니다.( FORMAT : xx:xx:xx:xx:xx:xx )");
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

	//CComboBox* p_EtherComboBox = (CComboBox*)GetDlgItem(IDC_PADAPTER);
	//device_description.Trim();
	//m_PAdapter.AddString(_T("Default Device"));
	//p_EtherComboBox->AddString(_T("Default Device"));
	//m_PAdapter.SetCurSel(0);
}

/*
BOOL CProxyAddDlg::OnInitDialog()
{
	m_PAdapter.AddString(_T("Default Device"));
	//p_EtherComboBox->AddString(_T("Default Device"));
	m_PAdapter.SetCurSel(0);

	return TRUE;
}
*/
