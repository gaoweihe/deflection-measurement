
// DeflectionProcView.h : interface of the CDeflectionProcView class
//

#pragma once
#include "stdafx.h"
#include "DeflectionProcDoc.h"
#include "Math.h"

#define DEST_X_ORIIMG	10
#define DEST_Y_ORIIMG	10

class CDeflectionProcDoc;

enum class ColorSpace { sRGB = 1, AdobeRGB, AppleRGB, ColorMatchRGB, KODAK };

class CDeflectionProcView : public CView
{
protected: // create from serialization only
	CDeflectionProcView();
	DECLARE_DYNCREATE(CDeflectionProcView)

// Attributes
public:
	CDeflectionProcDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CDeflectionProcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual void OnInitialUpdate();

	void RedrawBmp(BitmapDatasetEx *bmpData);
	double GetGrayValue(unsigned int r, unsigned int g, unsigned int b, ColorSpace = ColorSpace::sRGB, double gamma = 2.20) const;
	CPoint CalLightCenter(unsigned int threshold, BITMAP bitmap) const;
	COLORREF GetPixelColor(int row, int col, BITMAP bitmap) const;
	size_t GetBmpBuffPos(int row, int col, BITMAP bitmap) const;
	void BinarizeBitmap(BITMAP *targetBmp, BITMAPINFO *targetBmpInfo, BITMAPFILEHEADER *targetBmpFileHeader, BITMAP *srcBmp, BITMAPINFO *srcBmpInfo, BITMAPFILEHEADER *srcBmpFileHeader);
	void ShrinkBitmap(BitmapDatasetEx* bmpData);
	void UpdateLightCenter();
	void CorrectDistortion(BITMAP* targetBmp, BITMAPINFO* targetBmpInfo, BITMAPFILEHEADER *targetBmpFileHeader, BITMAP* srcBmp, BITMAPINFO* srcBmpInfo, BITMAPFILEHEADER *srcBmpFileHeader);
	CPoint GetPosOnFilm(double a, double b, double c, double U, long x_center, long y_center, long x, long y);
	CPoint GetPosOnCCD(double a, double b, double c, double U, long x_center, long y_center, long x, long y);
	void ExpandBitmap(BITMAP* targetBmp, BITMAPINFO* targetBmpInfo, BITMAPFILEHEADER* targetBmpFileHeader, BITMAP* srcBmp, BITMAPINFO* srcBmpInfo, BITMAPFILEHEADER* srcBmpFileHeader);
	void DistortBitmap(BITMAP* targetBmp, BITMAPINFO* targetBmpInfo, BITMAPFILEHEADER* targetBmpFileHeader, BITMAP* srcBmp, BITMAPINFO* srcBmpInfo, BITMAPFILEHEADER* srcBmpFileHeader);
};

#ifndef _DEBUG  // debug version in DeflectionProcView.cpp
inline CDeflectionProcDoc* CDeflectionProcView::GetDocument() const
   { return reinterpret_cast<CDeflectionProcDoc*>(m_pDocument); }
#endif

