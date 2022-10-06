
// DeflectionProc.h : main header file for the DeflectionProc application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "stdafx.h"

// CDeflectionProcApp:
// See DeflectionProc.cpp for the implementation of this class
//

class CDeflectionProcApp : public CWinApp
{
public:
	CDeflectionProcApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppExit();
};

extern CDeflectionProcApp theApp;
