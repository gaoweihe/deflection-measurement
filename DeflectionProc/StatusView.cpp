#include "stdafx.h"
#include "DeflectionProcDoc.h"
#include "StatusView.h"

IMPLEMENT_DYNCREATE(CStatusView, CView)

CStatusView::CStatusView()
{
}

CStatusView::~CStatusView()
{

}

#ifdef _DEBUG
void CStatusView::AssertValid() const
{
	CView::AssertValid();
}
#ifndef _WIN32_WCE
void CStatusView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif

void CStatusView::OnDraw(CDC*)
{

}

CDeflectionProcDoc* CStatusView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeflectionProcDoc)));
	return (CDeflectionProcDoc*)m_pDocument;
}
