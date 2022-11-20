#pragma once
#include "afxdialogex.h"


// CProxyAddDlg 대화 상자

class CProxyAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProxyAddDlg)

public:
	CProxyAddDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CProxyAddDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROXYADD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	//virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CIPAddressCtrl m_IPAddr;
	CString m_EtherAddr;

	unsigned char IP[4];
	unsigned char Ether[10];
	afx_msg void OnCbnSelchangePadapter();
	CComboBox m_PAdapter;
};
