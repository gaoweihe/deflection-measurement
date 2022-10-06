// CltCtrlSheet.cpp : implementation file
//

#include "stdafx.h"
#include "DeflectionProc.h"
#include "ProcCtrlPage.h"
#include "CltCtrlSheet.h"


// CCltCtrlSheet

IMPLEMENT_DYNAMIC(CCltCtrlSheet, CPropertySheet)

CCltCtrlSheet::CCltCtrlSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	pSuperView = nullptr;
	pProcCtrlPage = new CProcCtrlPage();
}

CCltCtrlSheet::CCltCtrlSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	pSuperView = nullptr;
	pProcCtrlPage = new CProcCtrlPage();
}

CCltCtrlSheet::~CCltCtrlSheet()
{
}

void CCltCtrlSheet::InitPages()
{
	pProcCtrlPage->pSuperSheet = this;
	AddPage(pProcCtrlPage);

}


BEGIN_MESSAGE_MAP(CCltCtrlSheet, CPropertySheet)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCltCtrlSheet message handlers


void CCltCtrlSheet::OnSize(UINT nType, int cx, int cy)
{
	CPropertySheet::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}
