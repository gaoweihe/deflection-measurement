#pragma once
#include <afxwin.h>
#include "stdafx.h"

class CDeflectionProcDoc;
class CCltCtrlSheet;

class CCltCtrlView :
	public CView
{
	DECLARE_DYNCREATE(CCltCtrlView)

public:
	CCltCtrlView();
	virtual ~CCltCtrlView();
	virtual void OnDraw(CDC* /*pDC*/);
	CDeflectionProcDoc* GetDocument() const;
protected:
	CCltCtrlSheet* pCltCtrlSheet;



#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

