#pragma once
#include <afxwin.h>
#include "stdafx.h"

class CDeflectionProcDoc;

class CStatusView :
	public CView
{
	DECLARE_DYNCREATE(CStatusView)
public:
	CStatusView();
	~CStatusView();
	virtual void OnDraw(CDC* /*pDC*/);
	CDeflectionProcDoc* GetDocument() const;
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
};

