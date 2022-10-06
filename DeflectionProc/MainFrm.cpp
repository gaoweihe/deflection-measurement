
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DeflectionProc.h"
#include "DeflectionProcView.h"
#include "CltCtrlView.h"
#include "StatusView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame() 
{
	// TODO: add member initialization code here
	m_bSplitterCreated = false;
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	if (m_Splitter.CreateStatic(this, 1, 2))
	{
		CSize size;
		RECT cltRect;
		if (!m_SplitterEx.CreateStatic(
			&m_Splitter,     // our parent window is the first splitter
			2, 1, // the new splitter is 2 rows, 1 column
			WS_CHILD | WS_VISIBLE | WS_BORDER,  // style, WS_BORDER is needed
			m_Splitter.IdFromRowCol(0, 1)))
			// new splitter is in the first row, first column of first splitter
		{
			TRACE0("Failed to create nested splitter\n");
			return FALSE;
		}
		GetClientRect(&cltRect);

		size_t cltWidth = cltRect.right - cltRect.left;
		size_t cltHeight = cltRect.bottom - cltRect.top;

		size.cx = INITWIDTH_LEFTCOLUMN;
		size.cy = cltHeight;
		if (!m_Splitter.CreateView(0, 0, RUNTIME_CLASS(CDeflectionProcView),
			size, pContext)) return FALSE;

		size.cx = cltWidth - INITWIDTH_LEFTCOLUMN;
		size.cy = INITHEIGHT_STATUSVIEW;
		if (!m_SplitterEx.CreateView(0, 0, RUNTIME_CLASS(CStatusView),
			size, pContext)) return FALSE;

		size.cx = cltWidth - INITWIDTH_LEFTCOLUMN;
		size.cy = cltHeight - INITHEIGHT_STATUSVIEW;
		if (!m_SplitterEx.CreateView(1, 0, RUNTIME_CLASS(CCltCtrlView),
			size, pContext)) return FALSE;
		SetActiveView((CView*)m_Splitter.GetPane(0, 0));
	}
	m_bSplitterCreated = TRUE;

	AfxGetMainWnd()->MoveWindow(CRect(0, 0, INITWIDTH_MAINWINDOW, INITHEIGHT_MAINWINDOW), FALSE);

	return TRUE;


	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_SYSMENU;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers



void CMainFrame::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CFrameWnd::OnClose();
}
