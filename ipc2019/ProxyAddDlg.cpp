// CProxyAddDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ipc2019.h"
#include "afxdialogex.h"
#include "ProxyAddDlg.h"

// CProxyAddDlg 대화 상자

IMPLEMENT_DYNAMIC(CProxyAddDlg, CDialog)

CProxyAddDlg::CProxyAddDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PROXY_ADD_DIALOG, pParent)
	, m_EtherAddr(_T(""))
{
	m_NI = (CNILayer*)m_LayerMgr.GetLayer("NI");
}

CProxyAddDlg::~CProxyAddDlg()
{
}

void CProxyAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROXY_ADAPTER, m_PAdapter);
	DDX_Control(pDX, IDC_PROXY_IP, m_IPAddr);
	DDX_Text(pDX, IDC_EDIT_PROXY_ETHER, m_EtherAddr);
}


BEGIN_MESSAGE_MAP(CProxyAddDlg, CDialog)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_PROXY_IP, &CProxyAddDlg::OnIpnFieldchangedProxyIp)
	ON_CBN_SELCHANGE(IDC_PROXY_ADAPTER, &CProxyAddDlg::OnCbnSelchangeProxyAdapter)
END_MESSAGE_MAP()


// CProxyAddDlg 메시지 처리기


void CProxyAddDlg::OnIpnFieldchangedProxyIp(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

/*
BOOL CProxyAddDlg::OnInitDialog()
{
	
	// 어댑터 목록 가져와서 콤보 박스에 넣기
	CComboBox* p_EtherComboBox = (CComboBox*)GetDlgItem(IDC_PROXY_ADAPTER);
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

	return TRUE;
	

}
*/



void CProxyAddDlg::OnCbnSelchangeProxyAdapter()
{
	// TODO: Add your control notification handler code here
	

	
}
