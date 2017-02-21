// XSBAutoReplier.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CXSBAutoReplierApp:
// See XSBAutoReplier.cpp for the implementation of this class
//

class CXSBAutoReplierApp : public CWinApp
{
public:
	CXSBAutoReplierApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CXSBAutoReplierApp theApp;