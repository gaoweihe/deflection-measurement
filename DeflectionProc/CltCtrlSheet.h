#pragma once
#include "stdafx.h"

class CCltCtrlView;
class CProcCtrlPage;

// CCltCtrlSheet

class CCltCtrlSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCltCtrlSheet)

public:
	CCltCtrlSheet(UINT nIDCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	CCltCtrlSheet(LPCTSTR pszCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	virtual ~CCltCtrlSheet();
	CCltCtrlView* pSuperView;
	CProcCtrlPage* pProcCtrlPage;

public:
	void InitPages();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


