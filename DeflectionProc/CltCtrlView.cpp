#include "stdafx.h"
#include "DeflectionProcDoc.h"
#include "CltCtrlSheet.h"
#include "CltCtrlView.h"

IMPLEMENT_DYNCREATE(CCltCtrlView, CView)

CCltCtrlView::CCltCtrlView()
{
	pCltCtrlSheet = new CCltCtrlSheet(_T(""), this, 1);
}

CCltCtrlView::~CCltCtrlView()
{
}

#ifdef _DEBUG
void CCltCtrlView::AssertValid() const
{
	CView::AssertValid();
}
#ifndef _WIN32_WCE
void CCltCtrlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif

void CCltCtrlView::OnDraw(CDC*)
{

}

CDeflectionProcDoc* CCltCtrlView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeflectionProcDoc)));
	return (CDeflectionProcDoc*)m_pDocument;
}
BEGIN_MESSAGE_MAP(CCltCtrlView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


int CCltCtrlView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	pCltCtrlSheet->pSuperView = this;
	pCltCtrlSheet->InitPages();
	pCltCtrlSheet->Create(this, WS_CHILD | WS_VISIBLE | WS_THICKFRAME);
	pCltCtrlSheet->SetActivePage(0);
	return 0;
}


void CCltCtrlView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	pCltCtrlSheet->MoveWindow(CRect(0, 0, cx, cy));
	pCltCtrlSheet->GetTabControl()->MoveWindow(0, 0, cx, cy);
}
