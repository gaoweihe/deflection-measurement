
// DeflectionProcView.cpp : implementation of the CDeflectionProcView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DeflectionProc.h"
#endif

#include "DeflectionProcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDeflectionProcView

IMPLEMENT_DYNCREATE(CDeflectionProcView, CView)

BEGIN_MESSAGE_MAP(CDeflectionProcView, CView)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CDeflectionProcView construction/destruction

CDeflectionProcView::CDeflectionProcView()
{
	// TODO: add construction code here

}

CDeflectionProcView::~CDeflectionProcView()
{
}

BOOL CDeflectionProcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDeflectionProcView drawing

void CDeflectionProcView::OnDraw(CDC* /*pDC*/)
{
	CDeflectionProcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	if (pDoc->bmpDataset.isSrcLoaded)
	{
		RedrawBmp(&(pDoc->bmpDataset));
	}

}


// CDeflectionProcView diagnostics

#ifdef _DEBUG
void CDeflectionProcView::AssertValid() const
{
	CView::AssertValid();
}

void CDeflectionProcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDeflectionProcDoc* CDeflectionProcView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeflectionProcDoc)));
	return (CDeflectionProcDoc*)m_pDocument;
}
#endif //_DEBUG


// CDeflectionProcView message handlers


void CDeflectionProcView::RedrawBmp(BitmapDatasetEx* bmpData)
{
	// TODO: Add your implementation code here.
	CDeflectionProcDoc* pDoc = GetDocument();

	BITMAP bitmap;
	BITMAPINFO bitmapInfo;

	switch (pDoc->bmpDisplaySel)
	{
	case BmpDisplaySel::SOURCE:
		bitmap = bmpData->srcBitmap;
		bitmapInfo = bmpData->srcBitmapInfo;
		break;
	case BmpDisplaySel::BINARIZED:
		bitmap = bmpData->binarizedBitmap;
		bitmapInfo = bmpData->binarizedBitmapInfo;
		break;
	case BmpDisplaySel::SHRINKED:
		bitmap = bmpData->shrinkedBitmap;
		bitmapInfo = bmpData->shrinkedBitmapInfo;
		break;
	case BmpDisplaySel::CORRECTED:
		bitmap = bmpData->correctedBitmap;
		bitmapInfo = bmpData->correctedBitmapInfo;
		break;
	case BmpDisplaySel::EXPANDED:
		bitmap = bmpData->expandedBitmap;
		bitmapInfo = bmpData->expandedBitmapInfo;
		break;
	case BmpDisplaySel::DISTORTED:
		bitmap = bmpData->distortedBitmap;
		bitmapInfo = bmpData->distortedBitmapInfo;
		break;
	default:
		//CDC *pDC = GetDC();
		//HDC hDC = pDC->GetSafeHdc();
		//CreateCompatibleDC(hDC);
		//ReleaseDC(pDC);
		return;
		break;
	}

	CDC* pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	StretchDIBits(
		hDC,
		DEST_X_ORIIMG,
		DEST_Y_ORIIMG,
		bitmap.bmWidth,
		bitmap.bmHeight,
		0,
		0,
		bitmap.bmWidth,
		bitmap.bmHeight,
		bitmap.bmBits,
		&(bitmapInfo),
		DIB_RGB_COLORS,
		SRCCOPY
		);
	ReleaseDC(pDC);

	if (pDoc->isDisplayLightCenter && bmpData->isSrcLoaded)
	{
		if (bmpData->isShrinkedLoaded)
		{
			CPoint center = bmpData->lightCenter;
			CRect cltRect;
			GetClientRect(&cltRect);
			CClientDC clientDC(this);
			CPen* pPen = new CPen(PS_SOLID, 1, 0x000000FF /*Red*/);
			CPen* pOldPen = clientDC.SelectObject(pPen);

			if (pOldPen != HGDI_ERROR && pOldPen != nullptr)
			{
				pOldPen->DeleteObject();
			}

			clientDC.MoveTo(center.x + DEST_X_ORIIMG, 0);
			clientDC.LineTo(center.x + DEST_X_ORIIMG, cltRect.bottom);
			clientDC.MoveTo(0, center.y + DEST_Y_ORIIMG);
			clientDC.LineTo(cltRect.right, center.y + DEST_Y_ORIIMG);
		}
	}
}


void CDeflectionProcView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CDeflectionProcDoc* pDoc = GetDocument();
	pDoc->pDeflectionProcView = this;
}


CPoint CDeflectionProcView::CalLightCenter(unsigned int threshold, BITMAP bitmap) const
{
	// TODO: Add your implementation code here.
	BYTE* pSrcBuff = (BYTE *)(bitmap.bmBits);
	UINT width = bitmap.bmWidth;
	UINT height = bitmap.bmHeight;

	UINT center_x = 0, center_y = 0;
	size_t counter = 0;
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			COLORREF currColor = GetPixelColor(i, j, bitmap);
			UINT r = GetRValue(currColor);
			UINT g = GetGValue(currColor);
			UINT b = GetBValue(currColor);
			UINT grayVal = (UINT)GetGrayValue(r, g, b);
			if (grayVal >= threshold)
			{
				center_x += j;
				center_y += i;
				counter++;
			}
		}
	}

	if (counter == 0)
	{
		return CPoint(0, 0);
	}

	center_x /= counter;
	center_y /= counter;

	return CPoint((int)center_x, (int)center_y);
}

COLORREF CDeflectionProcView::GetPixelColor(int row, int col, BITMAP bitmap) const
{
	// TODO: Add your implementation code here.
	int width = bitmap.bmWidth;
	int height = bitmap.bmHeight;

	if (col < 0 || row < 0 || col >= width || row >= height)
	{
		return 0xFFFFFFFF;
	}

	BYTE* pSrcBuff = (BYTE *)(bitmap.bmBits);
	size_t buffPtr = GetBmpBuffPos(row, col, bitmap);
	COLORREF result = 0;
	BYTE currR = pSrcBuff[buffPtr];
	BYTE currG = pSrcBuff[buffPtr - 1];
	BYTE currB = pSrcBuff[buffPtr - 2];
	result |= currR;
	result |= currG << 8;
	result |= currB << 16;

	return result;
}

double CDeflectionProcView::GetGrayValue(unsigned int r, unsigned int g, unsigned int b, ColorSpace colorSpace, double gamma) const
{
	// TODO: Add your implementation code here.
	double grayVal = 0;
	switch (colorSpace)
	{
	case ColorSpace::sRGB:
		grayVal = pow(
			(
			pow(double(r), gamma) * 0.2126 +
			pow(double(g), gamma) * 0.7152 +
			pow(double(b), gamma) * 0.0722
			),
			(double(1) / gamma)
			);
		break;
	default:
		break;
	}
	return grayVal;
}


void CDeflectionProcView::ShrinkBitmap(BitmapDatasetEx* bmpData)
{
	// TODO: Add your implementation code here.
	CDeflectionProcDoc* pDoc = GetDocument();

	size_t binWidth = bmpData->binarizedBitmap.bmWidth;
	size_t binHeight = bmpData->binarizedBitmap.bmHeight;
	size_t shrkWidth = binWidth;
	size_t shrkHeight = binHeight;

	BITMAPFILEHEADER *pTargetBmpFileHeader = &(bmpData->shrinkedFileHeader);
	BITMAPFILEHEADER *pSrcBmpFileHeader = &(bmpData->binarizedFileHeader);
	memcpy_s(
		pTargetBmpFileHeader,
		sizeof(BITMAPFILEHEADER),
		pSrcBmpFileHeader,
		sizeof(BITMAPFILEHEADER)
		);
	pTargetBmpFileHeader->bfSize = pTargetBmpFileHeader->bfOffBits + (shrkWidth + 1) * 3 / 4 * 4 * shrkHeight;

	/*Render bitmap info*/
	BITMAPINFO *pShrkBmpInfo = &(bmpData->shrinkedBitmapInfo);
	BITMAPINFO *pBinBmpInfo = &(bmpData->binarizedBitmapInfo);
	memcpy_s(
		pShrkBmpInfo,
		sizeof(BITMAPINFO),
		pBinBmpInfo,
		sizeof(BITMAPINFO)
		);
	pShrkBmpInfo->bmiHeader.biHeight = shrkHeight;
	pShrkBmpInfo->bmiHeader.biWidth = shrkWidth;
	pShrkBmpInfo->bmiHeader.biSizeImage = 0;

	BITMAP *pShrkBmp = &(bmpData->shrinkedBitmap);
	BITMAP *pBinBmp = &(bmpData->binarizedBitmap);
	pShrkBmp->bmHeight = shrkHeight;
	pShrkBmp->bmWidth = shrkWidth;
	if (pShrkBmp->bmBits != nullptr)
	{
		delete pShrkBmp->bmBits;
		pShrkBmp->bmBits = nullptr;
	}
	pShrkBmp->bmBits = new BYTE[(shrkWidth + 1) * 3 / 4 * 4 * shrkHeight];
	memset(pShrkBmp->bmBits, 0, (shrkWidth + 1) * 3 / 4 * 4 * shrkHeight);

	for (size_t i = 0; i < shrkHeight; i++)
	{
		for (size_t j = 0; j < shrkWidth; j++)
		{
			UINT r = 0, g = 0, b = 0;
			if (i == 0 || j == 0 || i == shrkHeight - 1 || j == shrkWidth - 1) {
				goto store;
			}
			for (size_t k = i - 1; k < i + 2; k++)
			{
				for (size_t l = j; l < j + 2; l++)
				{
					if ((k == i) && (l == j)) {
						continue;
					}
					COLORREF currColor = GetPixelColor(k, l, *pBinBmp);
					/* Invalid instruction(SSE) at conditional breakpoint SDKv142*(VS2019) */
					UINT currR = GetRValue(currColor);
					UINT currG = GetGValue(currColor);
					UINT currB = GetBValue(currColor);
					r += currR;
					g += currG;
					b += currB;
				}
			}

			if (r >= 255 * 5 && g >= 255 * 5 && b >= 255 * 5)
			{
				r = 255;
				g = 255;
				b = 255;
			}
			else
			{
				r = 0;
				g = 0;
				b = 0;
			}
		store:
			BYTE *pShrkBuff = (BYTE *)(bmpData->shrinkedBitmap.bmBits);
			size_t buffPtr = GetBmpBuffPos(i, j, *pShrkBmp);
			pShrkBuff[buffPtr] = (BYTE)r;
			pShrkBuff[buffPtr - 1] = (BYTE)g;
			pShrkBuff[buffPtr - 2] = (BYTE)b;
		}
	}

	bmpData->isShrinkedLoaded = true;
}


size_t CDeflectionProcView::GetBmpBuffPos(int row, int col, BITMAP bitmap) const
{
	// TODO: Add your implementation code here.
	size_t buffPtr = 0;

	size_t width = bitmap.bmWidth;
	size_t height = bitmap.bmHeight;

	size_t bytesPerRow = (width + 1) * 3 / 4 * 4;
	size_t MemAlignDummyCnt = bytesPerRow - width * 3;
	size_t endOffset = bytesPerRow * height - 1;
	buffPtr = endOffset - bytesPerRow * row;
	buffPtr -= (width - col - 1) * 3;
	buffPtr -= MemAlignDummyCnt;
	return buffPtr;
}


void CDeflectionProcView::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CView::OnClose();
}


void CDeflectionProcView::BinarizeBitmap(BITMAP *binBmp, BITMAPINFO *binBmpInfo, BITMAPFILEHEADER *binBmpFileHeader, BITMAP *srcBmp, BITMAPINFO *srcBmpInfo, BITMAPFILEHEADER *srcBmpFileHeader)
{
	CDeflectionProcDoc* pDoc = GetDocument();
	/**/
	memcpy_s(
		binBmpInfo,
		sizeof(BITMAPINFO),
		srcBmpInfo,
		sizeof(BITMAPINFO)
		);

	binBmp->bmBitsPixel = srcBmp->bmBitsPixel;
	binBmp->bmHeight = srcBmp->bmHeight;
	binBmp->bmPlanes = srcBmp->bmPlanes;
	binBmp->bmType = srcBmp->bmType;
	binBmp->bmWidth = srcBmp->bmWidth;
	binBmp->bmWidthBytes = srcBmp->bmWidthBytes;

	size_t width = binBmp->bmWidth;
	size_t height = binBmp->bmHeight;

	memcpy_s(
		binBmpFileHeader,
		sizeof(BITMAPFILEHEADER),
		srcBmpFileHeader,
		sizeof(BITMAPFILEHEADER)
		);
	binBmpFileHeader->bfSize = binBmpFileHeader->bfOffBits + (width + 1) * 3 / 4 * 4 * height;

	BYTE *tempBuffer = new BYTE[(width + 1) * 3 / 4 * 4 * height];
	memset(tempBuffer, 0, (width + 1) * 3 / 4 * 4 * height);
	memcpy_s(
		tempBuffer,
		(width + 1) * 3 / 4 * 4 * height,
		srcBmp->bmBits,
		(width + 1) * 3 / 4 * 4 * height
		);

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			BYTE* pBuff = (BYTE*)(tempBuffer);
			size_t buffPtr = pDoc->pDeflectionProcView->GetBmpBuffPos(i, j, *srcBmp);
			COLORREF currColor = 0;
			BYTE r = pBuff[buffPtr]; //r
			BYTE g = pBuff[buffPtr - 1]; //g
			BYTE b = pBuff[buffPtr - 2]; //b
			currColor += (b << 16) + (g << 8) + r;
			BYTE grayVal = (BYTE)(pDoc->pDeflectionProcView->GetGrayValue(r, g, b));
			if (grayVal < *(pDoc->threshold))
			{
				pBuff[buffPtr] = 0;
				pBuff[buffPtr - 1] = 0;
				pBuff[buffPtr - 2] = 0;
			}
			else
			{
				pBuff[buffPtr] = 255;
				pBuff[buffPtr - 1] = 255;
				pBuff[buffPtr - 2] = 255;
			}
		}
	}
	/**/

	if (binBmp->bmBits != nullptr)
	{
		delete binBmp->bmBits;
		binBmp->bmBits = nullptr;
	}
	binBmp->bmBits = new BYTE[(width + 1) * 3 / 4 * 4 * height];
	memset(binBmp->bmBits, 0, (width + 1) * 3 / 4 * 4 * height);
	memcpy_s(
		binBmp->bmBits,
		(width + 1) * 3 / 4 * 4 * height,
		tempBuffer,
		(width + 1) * 3 / 4 * 4 * height
		);
	delete tempBuffer;
	tempBuffer = nullptr;
}


void CDeflectionProcView::UpdateLightCenter()
{
	CDeflectionProcDoc* pDoc = GetDocument();
	BinarizeBitmap(
		&(pDoc->bmpDataset.binarizedBitmap),
		&(pDoc->bmpDataset.binarizedBitmapInfo),
		&(pDoc->bmpDataset.binarizedFileHeader),
		&(pDoc->bmpDataset.srcBitmap),
		&(pDoc->bmpDataset.srcBitmapInfo),
		&(pDoc->bmpDataset.srcFileHeader)
		);
	ShrinkBitmap(&(pDoc->bmpDataset));
	CorrectDistortion(
		&(pDoc->bmpDataset.correctedBitmap),
		&(pDoc->bmpDataset.correctedBitmapInfo),
		&(pDoc->bmpDataset.correctedFileHeader),
		&(pDoc->bmpDataset.shrinkedBitmap),
		&(pDoc->bmpDataset.shrinkedBitmapInfo),
		&(pDoc->bmpDataset.shrinkedFileHeader)
		);
	ExpandBitmap(
		&(pDoc->bmpDataset.expandedBitmap),
		&(pDoc->bmpDataset.expandedBitmapInfo),
		&(pDoc->bmpDataset.expandedFileHeader),
		&(pDoc->bmpDataset.correctedBitmap),
		&(pDoc->bmpDataset.correctedBitmapInfo),
		&(pDoc->bmpDataset.correctedFileHeader)
		);
	DistortBitmap(
		&(pDoc->bmpDataset.distortedBitmap),
		&(pDoc->bmpDataset.distortedBitmapInfo),
		&(pDoc->bmpDataset.distortedFileHeader),
		&(pDoc->bmpDataset.srcBitmap),
		&(pDoc->bmpDataset.srcBitmapInfo),
		&(pDoc->bmpDataset.srcFileHeader)
		);
	CPoint tempCenter = CalLightCenter(*(pDoc->threshold), pDoc->bmpDataset.expandedBitmap);
	pDoc->bmpDataset.lightCenter.SetPoint(tempCenter.x, tempCenter.y);
}


void CDeflectionProcView::CorrectDistortion(BITMAP* targetBmp, BITMAPINFO* targetBmpInfo, BITMAPFILEHEADER *targetBmpFileHeader, BITMAP* srcBmp, BITMAPINFO* srcBmpInfo, BITMAPFILEHEADER *srcBmpFileHeader)
{
	CDeflectionProcDoc* pDoc = GetDocument();
	double a = *(pDoc->distortionCorrectionParam.a);
	double b = *(pDoc->distortionCorrectionParam.b);
	double c = *(pDoc->distortionCorrectionParam.c);
	double U = *(pDoc->distortionCorrectionParam.U);


	CSize srcSize = CSize(srcBmp->bmWidth, srcBmp->bmHeight);
	double diagonal = sqrt(pow((double)srcSize.cx / 2, 2) + pow((double)srcSize.cy / 2, 2));
	double ratio = 1 + a * pow(diagonal, 2) + b * pow(diagonal, 4) + c * pow(diagonal, 6);
	CSize targetSize = CSize(
		(size_t)(srcSize.cx * ratio + 1),
		(size_t)(srcSize.cy * ratio + 1)
		);

	if (targetBmp->bmBits != nullptr)
	{
		delete targetBmp->bmBits;
		targetBmp->bmBits = nullptr;
	}
	targetBmp->bmBits = new BYTE[(targetSize.cx + 1) * 3 / 4 * 4 * targetSize.cy];
	memset(targetBmp->bmBits, 0, (targetSize.cx + 1) * 3 / 4 * 4 * targetSize.cy);
	BYTE *pTargetBuff = (BYTE *)(targetBmp->bmBits);
	BYTE *pSrcBuff = (BYTE *)(srcBmp->bmBits);

	memcpy_s(
		targetBmpInfo,
		sizeof(BITMAPINFO),
		srcBmpInfo,
		sizeof(BITMAPINFO)
		);
	targetBmpInfo->bmiHeader.biHeight = targetSize.cy;
	targetBmpInfo->bmiHeader.biWidth = targetSize.cx;
	targetBmpInfo->bmiHeader.biSizeImage = 0;

	size_t src = _msize(pSrcBuff);
	size_t target = _msize(pTargetBuff);

	targetBmp->bmBitsPixel = srcBmp->bmBitsPixel;
	targetBmp->bmHeight = targetSize.cy;
	targetBmp->bmPlanes = srcBmp->bmPlanes;
	targetBmp->bmType = srcBmp->bmType;
	targetBmp->bmWidth = targetSize.cx;
	targetBmp->bmWidthBytes = srcBmp->bmWidthBytes;

	memcpy_s(
		targetBmpFileHeader,
		sizeof(BITMAPFILEHEADER),
		srcBmpFileHeader,
		sizeof(BITMAPFILEHEADER)
		);
	targetBmpFileHeader->bfSize = targetBmpFileHeader->bfOffBits + (targetSize.cx + 1) * 3 / 4 * 4 * targetSize.cy;

	for (size_t i = 0; i < (size_t)(srcSize.cy); i++)
	{
		for (size_t j = 0; j < (size_t)(srcSize.cx); j++)
		{
			CPoint currPos = GetPosOnFilm(a, b, c, U, (srcSize.cx - 1) / 2, (srcSize.cy - 1) / 2, j, i);
			size_t targetBuffPtr = GetBmpBuffPos(currPos.y, currPos.x, *targetBmp);
			size_t srcBuffPtr = GetBmpBuffPos(i, j, *srcBmp);
			pTargetBuff[targetBuffPtr] = pSrcBuff[srcBuffPtr];
			pTargetBuff[targetBuffPtr - 1] = pSrcBuff[srcBuffPtr - 1];
			pTargetBuff[targetBuffPtr - 2] = pSrcBuff[srcBuffPtr - 2];
		}
	}
}

void CDeflectionProcView::DistortBitmap(
	BITMAP* targetBmp, 
	BITMAPINFO* targetBmpInfo, 
	BITMAPFILEHEADER *targetBmpFileHeader, 
	BITMAP* srcBmp, 
	BITMAPINFO* srcBmpInfo, 
	BITMAPFILEHEADER *srcBmpFileHeader
	)
{
	CDeflectionProcDoc* pDoc = GetDocument();
	double a = *(pDoc->distortionCorrectionParam.a);
	double b = *(pDoc->distortionCorrectionParam.b);
	double c = *(pDoc->distortionCorrectionParam.c);
	double U = *(pDoc->distortionCorrectionParam.U);


	CSize srcSize = CSize(srcBmp->bmWidth, srcBmp->bmHeight);
	size_t xMaxRadius = (srcSize.cx - 1) / 2;
	size_t yMaxRadius = (srcSize.cy - 1) / 2;
	double xMaxRatio = 1 + a * pow((double)xMaxRadius, 2) + b * pow((double)xMaxRadius, 4) + c * pow((double)xMaxRadius, 6);
	double yMaxRatio = 1 + a * pow((double)yMaxRadius, 2) + b * pow((double)yMaxRadius, 4) + c * pow((double)yMaxRadius, 6);
	CSize targetSize = CSize(
		(size_t)((double)srcSize.cx / xMaxRatio),
		(size_t)((double)srcSize.cy / yMaxRatio)
		);

	if (targetBmp->bmBits != nullptr)
	{
		delete targetBmp->bmBits;
		targetBmp->bmBits = nullptr;
	}
	targetBmp->bmBits = new BYTE[(targetSize.cx + 1) * 3 / 4 * 4 * targetSize.cy];
	memset(targetBmp->bmBits, 0, (targetSize.cx + 1) * 3 / 4 * 4 * targetSize.cy);
	BYTE *pTargetBuff = (BYTE *)(targetBmp->bmBits);
	BYTE *pSrcBuff = (BYTE *)(srcBmp->bmBits);

	memcpy_s(
		targetBmpInfo,
		sizeof(BITMAPINFO),
		srcBmpInfo,
		sizeof(BITMAPINFO)
		);
	targetBmpInfo->bmiHeader.biHeight = targetSize.cy;
	targetBmpInfo->bmiHeader.biWidth = targetSize.cx;
	targetBmpInfo->bmiHeader.biSizeImage = 0;

	targetBmp->bmBitsPixel = srcBmp->bmBitsPixel;
	targetBmp->bmHeight = targetSize.cy;
	targetBmp->bmPlanes = srcBmp->bmPlanes;
	targetBmp->bmType = srcBmp->bmType;
	targetBmp->bmWidth = targetSize.cx;
	targetBmp->bmWidthBytes = srcBmp->bmWidthBytes;

	memcpy_s(
		targetBmpFileHeader,
		sizeof(BITMAPFILEHEADER),
		srcBmpFileHeader,
		sizeof(BITMAPFILEHEADER)
		);
	targetBmpFileHeader->bfSize = targetBmpFileHeader->bfOffBits + (targetSize.cx + 1) * 3 / 4 * 4 * targetSize.cy;

	for (size_t i = 0; i < (size_t)(srcSize.cy); i++)
	{
		for (size_t j = 0; j < (size_t)(srcSize.cx); j++)
		{
			CPoint currPos = GetPosOnCCD(a, b, c, U, (srcSize.cx - 1) / 2, (srcSize.cy - 1) / 2, j, i);
			size_t targetBuffPtr = GetBmpBuffPos(currPos.y, currPos.x, *targetBmp);
			size_t srcBuffPtr = GetBmpBuffPos(i, j, *srcBmp);
			pTargetBuff[targetBuffPtr] = pSrcBuff[srcBuffPtr];
			pTargetBuff[targetBuffPtr - 1] = pSrcBuff[srcBuffPtr - 1];
			pTargetBuff[targetBuffPtr - 2] = pSrcBuff[srcBuffPtr - 2];
		}
	}
}


void CDeflectionProcView::ExpandBitmap(BITMAP* targetBmp, BITMAPINFO* targetBmpInfo, BITMAPFILEHEADER* targetBmpFileHeader, BITMAP* srcBmp, BITMAPINFO* srcBmpInfo, BITMAPFILEHEADER* srcBmpFileHeader)
{
	CSize srcSize = CSize(srcBmp->bmWidth, srcBmp->bmHeight);
	CSize targetSize = CSize(srcSize.cx, srcSize.cy);

	if (targetBmp->bmBits != nullptr)
	{
		delete targetBmp->bmBits;
		targetBmp->bmBits = nullptr;
	}
	targetBmp->bmBits = new BYTE[(targetSize.cx + 1) * 3 / 4 * 4 * targetSize.cy];
	memset(targetBmp->bmBits, 0, (targetSize.cx + 1) * 3 / 4 * 4 * targetSize.cy);
	BYTE *pTargetBuff = (BYTE *)(targetBmp->bmBits);
	BYTE *pSrcBuff = (BYTE *)(srcBmp->bmBits);

	memcpy_s(
		targetBmpInfo,
		sizeof(BITMAPINFO),
		srcBmpInfo,
		sizeof(BITMAPINFO)
		);
	targetBmpInfo->bmiHeader.biHeight = targetSize.cy;
	targetBmpInfo->bmiHeader.biWidth = targetSize.cx;
	targetBmpInfo->bmiHeader.biSizeImage = 0;

	targetBmp->bmBitsPixel = srcBmp->bmBitsPixel;
	targetBmp->bmHeight = targetSize.cy;
	targetBmp->bmPlanes = srcBmp->bmPlanes;
	targetBmp->bmType = srcBmp->bmType;
	targetBmp->bmWidth = targetSize.cx;
	targetBmp->bmWidthBytes = srcBmp->bmWidthBytes;

	memcpy_s(
		targetBmpFileHeader,
		sizeof(BITMAPFILEHEADER),
		srcBmpFileHeader,
		sizeof(BITMAPFILEHEADER)
		);
	targetBmpFileHeader->bfSize = targetBmpFileHeader->bfOffBits + (targetSize.cx + 1) * 3 / 4 * 4 * targetSize.cy;

	for (size_t i = 0; i < (size_t)(srcSize.cy); i++)
	{
		for (size_t j = 0; j < (size_t)(srcSize.cx); j++)
		{
			UINT r = 0, g = 0, b = 0;
			if (i == 0 || j == 0 || i == srcSize.cy - 1 || j == srcSize.cx - 1) {
				goto store;
			}
			for (size_t k = i - 1; k < i + 2; k++)
			{
				for (size_t l = j - 1; l < j + 2; l++)
				{
					if ((k == i) && (i == j)) {
						continue;
					}
					COLORREF currColor = GetPixelColor(k, l, *srcBmp);
					/* Invalid instruction(SSE) at conditional breakpoint SDKv142*(VS2019) */
					UINT currR = GetRValue(currColor);
					UINT currG = GetGValue(currColor);
					UINT currB = GetBValue(currColor);
					r += currR;
					g += currG;
					b += currB;
				}
			}

			if (r >= 255 * 4 && g >= 255 * 4 && b >= 255 * 4)
			{
				r = 255;
				g = 255;
				b = 255;
			}
			else
			{
				r = 0;
				g = 0;
				b = 0;
			}

		store:
			size_t buffPtr = GetBmpBuffPos(i, j, *targetBmp);
			pTargetBuff[buffPtr] = (BYTE)r;
			pTargetBuff[buffPtr - 1] = (BYTE)g;
			pTargetBuff[buffPtr - 2] = (BYTE)b;
		}
	}
}


CPoint CDeflectionProcView::GetPosOnFilm(double a, double b, double c, double U, long x_center, long y_center, long x, long y)
{
	double r = sqrt(pow((double)(x - x_center), 2) + pow((double)(y - y_center), 2));
	double ratio = 1 + a * pow(r, 2) + b * pow(r, 4) + c * pow(r, 6);
	double maxRadius = sqrt(pow((double)x_center, 2) + pow((double)y_center, 2));
	double maxRatio = 1 + a * pow(maxRadius, 2) + b * pow(maxRadius, 4) + c * pow(maxRadius, 6);
	CPoint srcCenter = CPoint(x_center, y_center);
	CPoint targetCenter = CPoint(
		(int)(srcCenter.x * maxRatio),
		(int)(srcCenter.y * maxRatio)
		);
	CPoint result = CPoint(
		(int)(targetCenter.x + (x - srcCenter.x) * ratio),
		(int)(targetCenter.y + (y - srcCenter.y) * ratio)
		);
	return result;
}

CPoint CDeflectionProcView::GetPosOnCCD(double a, double b, double c, double U, long x_center, long y_center, long x, long y)
{
	CSize srcSize = CSize(2 * x + 1, 2 * y + 1);
	size_t xMaxRadius = srcSize.cx / 2;
	size_t yMaxRadius = srcSize.cy / 2;
	double xMaxRatio = 1 + a * pow((double)xMaxRadius, 2) + b * pow((double)xMaxRadius, 4) + c * pow((double)xMaxRadius, 6);
	double yMaxRatio = 1 + a * pow((double)yMaxRadius, 2) + b * pow((double)yMaxRadius, 4) + c * pow((double)yMaxRadius, 6);
	CSize targetSize = CSize(
		(size_t)((double)srcSize.cx / xMaxRatio + 1),
		(size_t)((double)srcSize.cy / yMaxRatio + 1)
		);

	double r = sqrt(pow((double)(x - x_center), 2) + pow((double)(y - y_center), 2));
	double ratio = 1 + a * pow(r, 2) + b * pow(r, 4) + c * pow(r, 6);
	CPoint srcCenter = CPoint(x_center, y_center);
	CPoint targetCenter = CPoint(
		(int)((double)srcCenter.x / xMaxRatio),
		(int)((double)srcCenter.y / yMaxRatio)
		);
	CPoint result = CPoint(
		(int)(targetCenter.x + (double)(x - srcCenter.x) / ratio),
		(int)(targetCenter.y + (double)(y - srcCenter.y) / ratio)
		);
	return result;
}
