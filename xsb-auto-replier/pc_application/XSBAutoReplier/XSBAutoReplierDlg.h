// XSBAutoReplierDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CXSBAutoReplierDlg dialog
class CXSBAutoReplierDlg : public CDialog
{
// Construction
public:
	CXSBAutoReplierDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_XSBAUTOREPLIER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCancel();

private:
	void CXSBAutoReplierDlg::TypeString(char* str);
	void CXSBAutoReplierDlg::SendAsciiKey(char c);
	void CXSBAutoReplierDlg::ReadConfig();
	void CXSBAutoReplierDlg::WriteConfig();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit _ctrl_txtPath;
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnSw();


	CEdit *_ctrl_txt_reps[7];

	CButton _ctrl_btnSW;
	CStatic _ctrl_lbl_status_sim;
	CStatic _ctrl_lbl_status_tool;
	CEdit _ctrl_txt_rep1;
	CEdit _ctrl_txt_rep2;
	CEdit _ctrl_txt_rep3;
	CEdit _ctrl_txt_rep4;
	CEdit _ctrl_txt_rep5;
	CEdit _ctrl_txt_rep6;
	CEdit _ctrl_txt_rep7;

	BOOL isRunning;
	HWND xpWindowhWnd;
	int currentReplyIndex;
	bool justReplied;

	CButton _ctrl_btn_browse;
};
