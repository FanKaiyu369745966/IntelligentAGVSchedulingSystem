// EQ2008_Dll_TestDlg.h : header file
//

#include "afxwin.h"
#if !defined(AFX_EQ2008_DLL_TESTDLG_H__A4D90C4A_CC47_4032_8385_531403771451__INCLUDED_)
#define AFX_EQ2008_DLL_TESTDLG_H__A4D90C4A_CC47_4032_8385_531403771451__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEQ2008_Dll_TestDlg dialog

class CEQ2008_Dll_TestDlg : public CDialog
{
// Construction
public:
	CEQ2008_Dll_TestDlg(CWnd* pParent = NULL);	// standard constructor

public:
	int     m_iCardNum;
	int     m_iProgramIndex;
	int     m_iProgramCount;

// Dialog Data
	//{{AFX_DATA(CEQ2008_Dll_TestDlg)
	enum { IDD = IDD_EQ2008_DLL_TEST_DIALOG };
	UINT	m_iScreenX;
	UINT	m_iScreenY;
	UINT	m_iScreenHeight;
	UINT	m_iScreenWidth;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEQ2008_Dll_TestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEQ2008_Dll_TestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAddprogram();
	afx_msg void OnAjusttime();
	afx_msg void OnAddtext();
	afx_msg void OnAddrtf();
	afx_msg void OnAddsinglelinetext();
	afx_msg void OnAddbmp();
	afx_msg void OnAddtime();
	afx_msg void OnAddtimecount();
	afx_msg void OnAddtemperature();
	afx_msg void OnClosescreen();
	afx_msg void OnOpenscreen();
	afx_msg void OnSend();
	afx_msg void OnDllprogram();
	afx_msg void OnBtnConnect();
	afx_msg void OnBtnRealtimesend();
	afx_msg void OnBtnDisconnect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ctrCardNum;
	afx_msg void OnCbnSelchangeComboCardaddr();
	afx_msg void OnBnClickedBtnRealtimesend2();
	afx_msg void OnBnClickedBtnRealtimesend3();
	afx_msg void OnBnClickedBtnDisconnect2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQ2008_DLL_TESTDLG_H__A4D90C4A_CC47_4032_8385_531403771451__INCLUDED_)
