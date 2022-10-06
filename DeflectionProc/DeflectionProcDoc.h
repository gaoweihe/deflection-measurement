
// DeflectionProcDoc.h : interface of the CDeflectionProcDoc class
//


#pragma once

class CDeflectionProcView;

enum class BmpDisplaySel { 
	NOTLOADED = 0, 
	SOURCE, 
	BINARIZED, 
	SHRINKED, 
	CORRECTED, 
	EXPANDED, 
	DISTORTED
};

typedef struct tagDistortionCorrectionParam
{
	double *a;
	double *b;
	double *c;
	double *U;
} DistortionCorrectionParam;

typedef struct tagBitmapDatasetEx
{
	CBitmap srcObjBitmap; 
	HBITMAP hSrcBitmap;
	BITMAP srcBitmap;
	BITMAPFILEHEADER srcFileHeader;
	BITMAPINFO srcBitmapInfo;
	BOOL isSrcLoaded;

	CBitmap shrinkedObjBitmap;
	HBITMAP hShrinkedBitmap;
	BITMAP shrinkedBitmap;
	BITMAPFILEHEADER shrinkedFileHeader;
	BITMAPINFO shrinkedBitmapInfo;
	BOOL isShrinkedLoaded;

	CBitmap binarizedObjBitmap;
	HBITMAP hbinarizedBitmap;
	BITMAP binarizedBitmap;
	BITMAPFILEHEADER binarizedFileHeader;
	BITMAPINFO binarizedBitmapInfo;

	CBitmap correctedObjBitmap;
	HBITMAP hCorrectedBitmap;
	BITMAP correctedBitmap;
	BITMAPFILEHEADER correctedFileHeader;
	BITMAPINFO correctedBitmapInfo;

	CBitmap expandedObjBitmap;
	HBITMAP hExpandedBitmap;
	BITMAP expandedBitmap;
	BITMAPFILEHEADER expandedFileHeader;
	BITMAPINFO expandedBitmapInfo;

	CBitmap distortedObjBitmap;
	HBITMAP hDistortedBitmap;
	BITMAP distortedBitmap;
	BITMAPFILEHEADER distortedFileHeader;
	BITMAPINFO distortedBitmapInfo;
	
	CPoint lightCenter;
} BitmapDatasetEx;

class CDeflectionProcDoc : public CDocument
{
protected: // create from serialization only
	CDeflectionProcDoc();
	DECLARE_DYNCREATE(CDeflectionProcDoc)

// Attributes
public:
	CDeflectionProcView* pDeflectionProcView;

public:
	BitmapDatasetEx bmpDataset;
	UINT *threshold;
	DistortionCorrectionParam distortionCorrectionParam;
	BOOL isDisplayLightCenter;
	BmpDisplaySel bmpDisplaySel;

// Operations
public:
	
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CDeflectionProcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void _LoadBitmap(CString *filePath, BitmapDatasetEx *bmpData);
};
