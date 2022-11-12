// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__119ECB1B_6E70_4662_A2A9_A20B5201CA81__INCLUDED_)
#define AFX_STDAFX_H__119ECB1B_6E70_4662_A2A9_A20B5201CA81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}

#define MAX_LAYER_NUMBER		0xff	// �ִ� ���� ��

#define ETHER_MAX_SIZE			1514	// Ethernet �ִ� ũ��
#define ETHER_HEADER_SIZE		14		// Ethernet ��� ũ��
#define ETHER_MAX_DATA_SIZE		( ETHER_MAX_SIZE - ETHER_HEADER_SIZE )	// Ethernet ������ ������

#define TCP_HEADER_SIZE			20	// TCP ��� ũ��

#define IP_HEADER_SIZE			20	// IP ��� ũ��
#define IP_MAX_DATA_SIZE			1490

// App ��� ũ��, App ������ ũ��
#define APP_HEADER_SIZE			( sizeof(unsigned int) * 2 +				\
								  sizeof(unsigned short) +					\
								  sizeof(unsigned char)	)
#define APP_DATA_SIZE			( ETHER_MAX_DATA_SIZE - ( APP_HEADER_SIZE +		\
												          TCP_HEADER_SIZE +		\
												          IP_HEADER_SIZE ) )

#define NI_COUNT_NIC			16

// NIC count
#define	NI_COUNT_NIC		10
#define TIMEOFCOMPLETE		10
#define TIMEOFIMCOMPLETE	0
#define TIMEOFDELETE		-5

typedef struct _ETHERNET_ADDR
{
	union {
		struct { unsigned char e0, e1, e2, e3, e4, e5; } s_un_byte;
		unsigned char s_ether_addr[6];
	} S_un;

#define addr0 S_un.s_un_byte.e0
#define addr1 S_un.s_un_byte.e1
#define addr2 S_un.s_un_byte.e2
#define addr3 S_un.s_un_byte.e3
#define addr4 S_un.s_un_byte.e4
#define addr5 S_un.s_un_byte.e5

#define addrs  S_un.s_ether_addr

} ETHERNET_ADDR, * LPETHERNET_ADDR;

typedef struct _ARPElement
{
	unsigned char* MACAddr;
	char state;
	DWORD tick;
}ARPElement;

#define ARP_COMPLETE_TIMEOUT 1200000
#define ARP_INCOMPLETE_TIMEOUT 180000

enum { ARP_INCOMPLETE, ARP_COMPLETE };
enum { ARP_REQUEST = 0x0100, ARP_REPLY = 0x0200 };

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__119ECB1B_6E70_4662_A2A9_A20B5201CA81__INCLUDED_)
