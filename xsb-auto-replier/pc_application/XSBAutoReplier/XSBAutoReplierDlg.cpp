// XSBAutoReplierDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XSBAutoReplier.h"
#include "XSBAutoReplierDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXSBAutoReplierDlg dialog

CXSBAutoReplierDlg::CXSBAutoReplierDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXSBAutoReplierDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXSBAutoReplierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, _ctrl_txtPath);
	DDX_Control(pDX, IDC_BTN_SW, _ctrl_btnSW);
	DDX_Control(pDX, IDC_LBL_STATUS_SIM, _ctrl_lbl_status_sim);
	DDX_Control(pDX, IDC_LBL_STATUS_TOOL, _ctrl_lbl_status_tool);
	DDX_Control(pDX, IDC_TXT_REP1, _ctrl_txt_rep1);
	DDX_Control(pDX, IDC_TXT_REP2, _ctrl_txt_rep2);
	DDX_Control(pDX, IDC_TXT_REP3, _ctrl_txt_rep3);
	DDX_Control(pDX, IDC_TXT_REP4, _ctrl_txt_rep4);
	DDX_Control(pDX, IDC_TXT_REP5, _ctrl_txt_rep5);
	DDX_Control(pDX, IDC_TXT_REP6, _ctrl_txt_rep6);
	DDX_Control(pDX, IDC_TXT_REP7, _ctrl_txt_rep7);
	DDX_Control(pDX, IDC_BTN_BROWSE, _ctrl_btn_browse);
}

BEGIN_MESSAGE_MAP(CXSBAutoReplierDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CXSBAutoReplierDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CXSBAutoReplierDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_SW, &CXSBAutoReplierDlg::OnBnClickedBtnSw)
END_MESSAGE_MAP()


// CXSBAutoReplierDlg message handlers

BOOL CXSBAutoReplierDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Init
	this->SetTimer(0, 10000, (TIMERPROC) NULL);
	this->isRunning = 0;
	this->currentReplyIndex = 0;
	this->justReplied = 0;

	this->_ctrl_txt_reps[0] = &this->_ctrl_txt_rep1;
	this->_ctrl_txt_reps[1] = &this->_ctrl_txt_rep2;
	this->_ctrl_txt_reps[2] = &this->_ctrl_txt_rep3;
	this->_ctrl_txt_reps[3] = &this->_ctrl_txt_rep4;
	this->_ctrl_txt_reps[4] = &this->_ctrl_txt_rep5;
	this->_ctrl_txt_reps[5] = &this->_ctrl_txt_rep6;
	this->_ctrl_txt_reps[6] = &this->_ctrl_txt_rep7;

	this->ReadConfig();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXSBAutoReplierDlg::OnPaint()
{
	CDialog::OnPaint();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXSBAutoReplierDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CXSBAutoReplierDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CXSBAutoReplierDlg::ReadConfig()
{
	TCHAR iniPath[500];
	memset(iniPath, 0, sizeof(TCHAR) * 500);
	GetCurrentDirectory(499, iniPath);
	_tcscat_s(iniPath, 500, _T("\\XSBAutoReplier.ini"));

	TCHAR buf[500];

	GetPrivateProfileString(_T("XSBAutoReplier"), _T("xpPath"), _T(""), buf, 500, iniPath);
	this->_ctrl_txtPath.SetWindowText(buf);

	GetPrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText1"), _T(""), buf, 500, iniPath);
	this->_ctrl_txt_reps[0]->SetWindowText(buf);
	GetPrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText2"), _T(""), buf, 500, iniPath);
	this->_ctrl_txt_reps[1]->SetWindowText(buf);
	GetPrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText3"), _T(""), buf, 500, iniPath);
	this->_ctrl_txt_reps[2]->SetWindowText(buf);
	GetPrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText4"), _T(""), buf, 500, iniPath);
	this->_ctrl_txt_reps[3]->SetWindowText(buf);
	GetPrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText5"), _T(""), buf, 500, iniPath);
	this->_ctrl_txt_reps[4]->SetWindowText(buf);
	GetPrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText6"), _T(""), buf, 500, iniPath);
	this->_ctrl_txt_reps[5]->SetWindowText(buf);
	GetPrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText7"), _T(""), buf, 500, iniPath);
	this->_ctrl_txt_reps[6]->SetWindowText(buf);

}

void CXSBAutoReplierDlg::WriteConfig()
{
	TCHAR iniPath[500];
	memset(iniPath, 0, sizeof(TCHAR) * 500);
	GetCurrentDirectory(499, iniPath);
	_tcscat_s(iniPath, 500, _T("\\XSBAutoReplier.ini"));

	TCHAR buf[500];
	this->_ctrl_txtPath.GetWindowText(buf, 499);
	WritePrivateProfileString(_T("XSBAutoReplier"), _T("xpPath"), buf, iniPath);

	this->_ctrl_txt_reps[0]->GetWindowText(buf, 499);
	WritePrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText1"), buf, iniPath);
	this->_ctrl_txt_reps[1]->GetWindowText(buf, 499);
	WritePrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText2"), buf, iniPath);
	this->_ctrl_txt_reps[2]->GetWindowText(buf, 499);
	WritePrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText3"), buf, iniPath);
	this->_ctrl_txt_reps[3]->GetWindowText(buf, 499);
	WritePrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText4"), buf, iniPath);
	this->_ctrl_txt_reps[4]->GetWindowText(buf, 499);
	WritePrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText5"), buf, iniPath);
	this->_ctrl_txt_reps[5]->GetWindowText(buf, 499);
	WritePrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText6"), buf, iniPath);
	this->_ctrl_txt_reps[6]->GetWindowText(buf, 499);
	WritePrivateProfileString(_T("XSBAutoReplier"), _T("autoReplyText7"), buf, iniPath);

}

void CXSBAutoReplierDlg::SendAsciiKey(char c)
{ 
	char ch = c;
	int isShift = 0;
	INPUT input[2]; 
	memset(input, 0, 2 * sizeof(INPUT));

	if (GetKeyState(VK_CAPITAL) & 0x1)
	{
		if(ch >= 'A' && ch <= 'Z')
		{
			isShift = 0;
		}
		else
			isShift = 1;
	}
	else
	{
		if( ch >= 'A' && ch <= 'Z')
		{
			isShift = 1;
		}

		if (ch == '_') {
			isShift = 1;
		}
	}

	if (isShift)
	{
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = VK_SHIFT;
		SendInput(1, input, sizeof(INPUT));

	}

	SHORT tmp = VkKeyScan(ch);
	WPARAM wParam = tmp & 0xFF;
	LPARAM lParam = 1;
	lParam += MapVirtualKey((UINT) wParam, 0) << 16;
	::PostMessage(xpWindowhWnd, WM_KEYDOWN, wParam, lParam);
	lParam += 1 << 30;
	lParam += 1 << 31;
	::PostMessage(xpWindowhWnd, WM_KEYUP, wParam, lParam);
	Sleep(25);
	if (isShift)
	{
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = VK_SHIFT;
		input[0].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, input, sizeof(INPUT));
		Sleep(25);
	}
}


void CXSBAutoReplierDlg::TypeString(char* str) {

	xpWindowhWnd = 0;
	xpWindowhWnd = ::FindWindow(NULL, _T("X-System"));
	
	if (!xpWindowhWnd)
		return;

	int len = (int) strlen(str);

	::SendNotifyMessage(xpWindowhWnd, WM_KEYDOWN, VK_ESCAPE, 0); ::SendNotifyMessage(xpWindowhWnd, WM_KEYUP, VK_ESCAPE, 0);
	::SendNotifyMessage(xpWindowhWnd, WM_KEYDOWN, VK_RETURN, 0); ::SendNotifyMessage(xpWindowhWnd, WM_KEYUP, VK_RETURN, 0);

	for (int i = 0; i < len; i++) {
		SendAsciiKey(str[i]);
	}
	
	::SendNotifyMessage(xpWindowhWnd, WM_KEYDOWN, VK_RETURN, 0); ::SendNotifyMessage(xpWindowhWnd, WM_KEYUP, VK_RETURN, 0);
}

void CXSBAutoReplierDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
	
	xpWindowhWnd = 0;
	xpWindowhWnd = ::FindWindow(NULL, _T("X-System"));
	if (xpWindowhWnd) {
		this->_ctrl_lbl_status_sim.SetWindowText(_T("Found"));
	} else {
		this->_ctrl_lbl_status_sim.SetWindowText(_T("Not Found"));
	}

	if (!isRunning)
		return;

	TCHAR txtPath[500];
	memset(txtPath, 0, 500 * sizeof(TCHAR));

	this->_ctrl_txtPath.GetWindowText(txtPath, 499);
	if (!_tcslen(txtPath)) {
		return;
	}
	_tcscat_s(txtPath, 500, _T("\\flxsbar_pm.txt"));

	// Open file
	FILE *xsbReaderFile = NULL;
	_tfopen_s(&xsbReaderFile, txtPath, _T("r"));

	if (!xsbReaderFile)
		return;

	// Init buffers
	char buf[100];
	char buf_msg[500];
	char buf_msg_user[500];
	TCHAR buf_msg_user_w[500];

	memset(buf, 0, 100);
	memset(buf_msg, 0, 500);
	memset(buf_msg_user, 0, 500);
	memset(buf_msg_user_w, 0, 500);

	// Read
	fscanf_s(xsbReaderFile, "%s\n", buf, 99);

	// Close file
	fclose(xsbReaderFile);

	// Load user message
	this->_ctrl_txt_reps[currentReplyIndex]->GetWindowText(buf_msg_user_w, 499);
	if (!_tcslen(buf_msg_user_w)) {
		currentReplyIndex = 0;
		this->_ctrl_txt_reps[currentReplyIndex]->GetWindowText(buf_msg_user_w, 499);
	}

	currentReplyIndex++;
	if (currentReplyIndex >= 7) 
		currentReplyIndex = 0;

	// Convert
	for (int i = 0; i < 500; i++) {
		TCHAR wchr = buf_msg_user_w[i];
		if (!wchr) break;
		buf_msg_user[i] = buf_msg_user_w[i] & 0xFF;
	}

	// Compose message
	sprintf_s(buf_msg, 499, ".msg %s %s", buf, buf_msg_user);

	// Type on X-Plane
	this->TypeString(buf_msg);
	
	// Delete file
	_wremove(txtPath);
	
}

void CXSBAutoReplierDlg::OnBnClickedBtnBrowse()
{

	BROWSEINFO bi;
	TCHAR Buffer[512];

	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;
	bi.lpszTitle = _T("Select X-Plane path");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;

	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		this->_ctrl_txtPath.SetWindowText(Buffer);
	} else {
		return;
	}
}

void CXSBAutoReplierDlg::OnBnClickedBtnSw()
{	
	TCHAR xpPath[500];
	memset(xpPath, 0, 500 * sizeof(TCHAR));

	this->_ctrl_txtPath.GetWindowText(xpPath, 500);
	
	if (!_tcslen(xpPath)) {
		MessageBox(_T("No X-Plane path specified."), 0, 0); 
		return;
	}

	_tcscat_s(xpPath, 499, _T("\\X-Plane.exe"));

	if (!isRunning) {
		CFileFind finder;
		if (!finder.FindFile(xpPath)) {
			MessageBox(_T("Invalid X-Plane path."), 0, 0);
			return;
		}

		// Disable controls
		this->_ctrl_txtPath.EnableWindow(FALSE);
		this->_ctrl_btn_browse.EnableWindow(FALSE);
		for (int i = 0; i < 7; i++) {
			this->_ctrl_txt_reps[i]->EnableWindow(FALSE);
		}

		this->_ctrl_btnSW.SetWindowText(_T("Stop"));
		this->_ctrl_lbl_status_tool.SetWindowText(_T("Running"));

		// Save config
		this->WriteConfig();

		isRunning = 1;

	} else {

		this->_ctrl_btnSW.SetWindowText(_T("Start"));
		this->_ctrl_lbl_status_tool.SetWindowText(_T("Stopped"));

		// Enable controls
		this->_ctrl_txtPath.EnableWindow(TRUE);
		this->_ctrl_btn_browse.EnableWindow(TRUE);
		for (int i = 0; i < 7; i++) {
			this->_ctrl_txt_reps[i]->EnableWindow(TRUE);
		}

		isRunning = 0;

	}

}
