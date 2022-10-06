
// DeflectionProcDoc.cpp : implementation of the CDeflectionProcDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DeflectionProc.h"
#endif

#include "DeflectionProcView.h"
#include "DeflectionProcDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDeflectionProcDoc

IMPLEMENT_DYNCREATE(CDeflectionProcDoc, CDocument)

BEGIN_MESSAGE_MAP(CDeflectionProcDoc, CDocument)
END_MESSAGE_MAP()


// CDeflectionProcDoc construction/destruction

CDeflectionProcDoc::CDeflectionProcDoc() 
{
	// TODO: add one-time construction code here
	pDeflectionProcView = nullptr;
	threshold = nullptr;
	isDisplayLightCenter = false;
	bmpDisplaySel = BmpDisplaySel::NOTLOADED;
	bmpDataset.srcBitmap.bmBits = nullptr;
	bmpDataset.binarizedBitmap.bmBits = nullptr;
	bmpDataset.shrinkedBitmap.bmBits = nullptr;
	bmpDataset.correctedBitmap.bmBits = nullptr;
	bmpDataset.expandedBitmap.bmBits = nullptr;
	bmpDataset.distortedBitmap.bmBits = nullptr;
	bmpDataset.isSrcLoaded = false;
	bmpDataset.isShrinkedLoaded = false;
}

CDeflectionProcDoc::~CDeflectionProcDoc()
{
}

BOOL CDeflectionProcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CDeflectionProcDoc serialization

void CDeflectionProcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CDeflectionProcDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CDeflectionProcDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CDeflectionProcDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDeflectionProcDoc diagnostics

#ifdef _DEBUG
void CDeflectionProcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDeflectionProcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDeflectionProcDoc commands


void CDeflectionProcDoc::_LoadBitmap(CString *filePath, BitmapDatasetEx *bmpData)
{
	// TODO: Add your implementation code here.
	bmpData->isSrcLoaded = FALSE;

	bmpData->hSrcBitmap = (HBITMAP)::LoadImage(
		NULL,
		*filePath,
		IMAGE_BITMAP,
		0,
		0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE
	);
	bmpData->srcObjBitmap.Detach();
	bmpData->srcObjBitmap.Attach(bmpData->hSrcBitmap);
	bmpData->srcObjBitmap.GetBitmap(&(bmpData->srcBitmap));

	CFile workFile;
	CFileException except;
	workFile.Open(
		*filePath,
		CFile::modeRead,
		&except
	);
	workFile.Read(&(bmpData->srcFileHeader), sizeof(BITMAPFILEHEADER));
	workFile.Read(&(bmpData->srcBitmapInfo), sizeof(BITMAPINFO));
	workFile.Close();

	if (bmpData->srcBitmapInfo.bmiHeader.biClrUsed == 0)
	{
		bmpData->srcBitmapInfo.bmiColors[0].rgbBlue = 0;
		bmpData->srcBitmapInfo.bmiColors[0].rgbGreen = 0;
		bmpData->srcBitmapInfo.bmiColors[0].rgbRed = 0;
		bmpData->srcBitmapInfo.bmiColors[0].rgbReserved = 0;
	}

	bmpData->isSrcLoaded = true;
}
