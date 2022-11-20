#pragma once
#include "afxdialogex.h"

class CProxyAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CProxyAddDlg)

public:
	CProxyAddDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CProxyAddDlg();
	//virtual BOOL OnInitDialog();
	

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROXY_ADD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	struct SystemEtherInfo {
		int InstallCardNo;
		char CardName[MAX_ADAPTER_NAME_LENGTH + 4 + 22];;
		char CardDesc[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];
		unsigned char MacAddr[6];
		char IPAddr[16];
	};		// 네트워크 장비 목록을 갖는 구조체

	CNILayer* m_NI;
	CLayerManager	m_LayerMgr;
	CComboBox m_PAdapter;
	afx_msg void OnIpnFieldchangedProxyIp(NMHDR* pNMHDR, LRESULT* pResult);
	CIPAddressCtrl m_IPAddr;
	CString m_EtherAddr;
	afx_msg void OnCbnSelchangeProxyAdapter();

protected:

};
