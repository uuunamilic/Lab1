
// IND_19748.h : main header file for the IND_19748 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND19748App:
// See IND_19748.cpp for the implementation of this class
//

class CIND19748App : public CWinApp
{
public:
	CIND19748App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND19748App theApp;
