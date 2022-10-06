
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "stdafx.h"

#define INITWIDTH_MAINWINDOW		1600
#define INITHEIGHT_MAINWINDOW		1400
#define INITWIDTH_LEFTCOLUMN		1200
#define INITHEIGHT_STATUSVIEW		300

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame() ;
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	BOOL m_bSplitterCreated;
	CSplitterWnd m_Splitter;
	CSplitterWndEx m_SplitterEx;
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
};


