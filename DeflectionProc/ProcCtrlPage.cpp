// ProcCtrlPage.cpp : implementation file
//

#include "stdafx.h"
#include "DeflectionProc.h"
#include "DeflectionProcDoc.h"
#include "DeflectionProcView.h"
#include "CltCtrlSheet.h"
#include "CltCtrlView.h"
#include "ProcCtrlPage.h"
#include "afxdialogex.h"


// CProcCtrlPage dialog

IMPLEMENT_DYNAMIC(CProcCtrlPage, CPropertyPage)

CProcCtrlPage::CProcCtrlPage()
	: CPropertyPage(CProcCtrlPage::IDD)
	, m_val_srcPath(_T(""))
	, m_val_thres(200)
	, m_param_a(5e-07)
	, m_param_b(1e-13)
	, m_param_c(0)
	, m_param_U(0)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	pSuperSheet = nullptr;
}

CProcCtrlPage::~CProcCtrlPage()
{
}

void CProcCtrlPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_SRCPATH, m_val_srcPath);
	DDX_Text(pDX, IDC_EDT_THRES, m_val_thres);
	DDV_MinMaxUInt(pDX, m_val_thres, 0, 255);
	DDX_Text(pDX, IDC_EDT_INPUTA, m_param_a);
	DDX_Text(pDX, IDC_EDT_INPUTB, m_param_b);
	DDX_Text(pDX, IDC_EDT_INPUTC, m_param_c);
	DDX_Text(pDX, IDC_EDT_INPUTU, m_param_U);
}


BEGIN_MESSAGE_MAP(CProcCtrlPage, CPropertyPage)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CProcCtrlPage::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_SETTHRES, &CProcCtrlPage::OnBnClickedBtnSetthres)
	ON_BN_CLICKED(IDC_RADIO_SELSOURCE, &CProcCtrlPage::OnBnClickedRadioSelsource)
	ON_BN_CLICKED(IDC_RADIO_SELBINARIZED, &CProcCtrlPage::OnBnClickedRadioSelbinarized)
	ON_BN_CLICKED(IDC_RADIO_SELSHRINKED, &CProcCtrlPage::OnBnClickedRadioSelshrinked)
	ON_BN_CLICKED(IDC_BTN_SAVEBMP, &CProcCtrlPage::OnBnClickedBtnSavebmp)
	ON_BN_CLICKED(IDC_CHK_LIGHTCENTER, &CProcCtrlPage::OnBnClickedChkLightcenter)
	ON_BN_CLICKED(IDC_BTN_CORRECTDISTORTION, &CProcCtrlPage::OnBnClickedBtnCorrectdistortion)
	ON_BN_CLICKED(IDC_RADIO_SELCORRECTED, &CProcCtrlPage::OnBnClickedRadioSelcorrected)
	ON_BN_CLICKED(IDC_RADIO_SELEXPANDED, &CProcCtrlPage::OnBnClickedRadioSelexpanded)
	ON_BN_CLICKED(IDC_RADIO_SELDISTORTED, &CProcCtrlPage::OnBnClickedRadioSeldistorted)
END_MESSAGE_MAP()


// CProcCtrlPage message handlers


void CProcCtrlPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);
	CRect cltRect;
	GetClientRect(&cltRect);
	
	// TODO: Add your message handler code here
}


void CProcCtrlPage::OnBnClickedBtnBrowse()
{
	// TODO: Add your control notification handler code here
	CDeflectionProcDoc* pDoc = pSuperSheet->pSuperView->GetDocument();
	wchar_t szFilters[] =
		_T("Filter Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("Bitmap file"), _T("*.bmp"),
		OFN_FILEMUSTEXIST, LPCTSTR(szFilters));

	UpdateData(TRUE);
	if (fileDlg.DoModal() == IDOK) {
		m_val_srcPath = fileDlg.GetPathName();
	}
	else
	{
		return;
	}
	UpdateData(FALSE);

	pDoc->_LoadBitmap(&m_val_srcPath, &(pDoc->bmpDataset));
	pDoc->pDeflectionProcView->Invalidate(TRUE);
}


void CProcCtrlPage::OnBnClickedBtnSetthres()
{
	// TODO: Add your control notification handler code here
	CDeflectionProcDoc* pDoc = pSuperSheet->pSuperView->GetDocument();
	UpdateData(TRUE);
	pDoc->pDeflectionProcView->UpdateLightCenter();
	pDoc->pDeflectionProcView->Invalidate(TRUE);
}


BOOL CProcCtrlPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CDeflectionProcDoc* pDoc = pSuperSheet->pSuperView->GetDocument();
	pDoc->threshold = &m_val_thres;
	pDoc->distortionCorrectionParam.a = &m_param_a;
	pDoc->distortionCorrectionParam.b = &m_param_b;
	pDoc->distortionCorrectionParam.c = &m_param_c;
	pDoc->distortionCorrectionParam.U = &m_param_U;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CProcCtrlPage::OnBnClickedRadioSelsource()
{
	// TODO:  在此添加控件通知处理程序代码
	CDeflectionProcDoc* pDoc = pSuperSheet->pSuperView->GetDocument();
	pDoc->bmpDisplaySel = BmpDisplaySel::SOURCE;
	pDoc->pDeflectionProcView->Invalidate(TRUE);
}


void CProcCtrlPage::OnBnClickedRadioSelbinarized()
{
	// TODO:  在此添加控件通知处理程序代码
	CDeflectionProcDoc* pDoc = pSuperSheet->pSuperView->GetDocument();
	pDoc->bmpDisplaySel = BmpDisplaySel::BINARIZED;
	pDoc->pDeflectionProcView->Invalidate(TRUE);
}


void CProcCtrlPage::OnBnClickedRadioSelshrinked()
{
	// TODO:  在此添加控件通知处理程序代码
	CDeflectionProcDoc* pDoc = pSuperSheet->pSuperView->GetDocument();
	pDoc->bmpDisplaySel = BmpDisplaySel::SHRINKED;
	pDoc->pDeflectionProcView->Invalidate(TRUE);
}


void CProcCtrlPage::OnBnClickedBtnSavebmp()
{
	// TODO:  在此添加控件通知处理程序代码
	CDeflectionProcDoc* pDoc = pSuperSheet->pSuperView->GetDocument();

	//Select folder path
	CString filePath;
	CString fileFilter = _T("Bitmap(*.bmp)|*.bmp|");
	CFileDialog hFileDlg(
		FALSE,
		0,
		0,
		OFN_HIDEREADONLY,
		fileFilter,
		NULL,
		0,
		TRUE);
	if (hFileDlg.DoModal() == IDOK)
	{
		filePath = hFileDlg.GetPathName();
	}
	else
	{
		return;
	}

	BITMAP *bitmap = nullptr;
	BITMAPINFO *bmpInfo = nullptr;
	BITMAPFILEHEADER *bmpFileHeader = nullptr;
	switch (pDoc->bmpDisplaySel)
	{
	case BmpDisplaySel::SOURCE:
		bitmap = &(pDoc->bmpDataset.srcBitmap);
		bmpInfo = &(pDoc->bmpDataset.srcBitmapInfo);
		bmpFileHeader = &(pDoc->bmpDataset.srcFileHeader);
		break;
	case BmpDisplaySel::BINARIZED:
		bitmap = &(pDoc->bmpDataset.binarizedBitmap);
		bmpInfo = &(pDoc->bmpDataset.binarizedBitmapInfo);
		bmpFileHeader = &(pDoc->bmpDataset.binarizedFileHeader);
		break;
	case BmpDisplaySel::CORRECTED:
		bitmap = &(pDoc->bmpDataset.correctedBitmap);
		bmpInfo = &(pDoc->bmpDataset.correctedBitmapInfo);
		bmpFileHeader = &(pDoc->bmpDataset.correctedFileHeader);
		break;
	case BmpDisplaySel::SHRINKED:
		bitmap = &(pDoc->bmpDataset.shrinkedBitmap);
		bmpInfo = &(pDoc->bmpDataset.shrinkedBitmapInfo);
		bmpFileHeader = &(pDoc->bmpDataset.shrinkedFileHeader);
		break;
	case BmpDisplaySel::EXPANDED:
		bitmap = &(pDoc->bmpDataset.expandedBitmap);
		bmpInfo = &(pDoc->bmpDataset.expandedBitmapInfo);
		bmpFileHeader = &(pDoc->bmpDataset.expandedFileHeader);
		break;
	case BmpDisplaySel::DISTORTED:
		bitmap = &(pDoc->bmpDataset.distortedBitmap);
		bmpInfo = &(pDoc->bmpDataset.distortedBitmapInfo);
		bmpFileHeader = &(pDoc->bmpDataset.distortedFileHeader);
		break;
	default:
		return;
		break;
	}

	//Render bitmap pixel buffer
	size_t buffLen = (bitmap->bmWidth + 1) * 3 / 4 * 4 * bitmap->bmHeight;
	BYTE *pPixelsBuff = new BYTE[buffLen];
	memset(pPixelsBuff, 0, buffLen);
	int bytesPerRow = (bitmap->bmWidth + 1) * 3 / 4 * 4;
	for (LONG i = 0; i < bitmap->bmHeight; i++)
	{
		for (LONG j = 0; j < bitmap->bmWidth; j++)
		{
			pPixelsBuff[i * bytesPerRow + j * 3] = ((BYTE *)(bitmap->bmBits))[i * bytesPerRow + j * 3 + 1];
			pPixelsBuff[i * bytesPerRow + j * 3 + 1] = ((BYTE *)(bitmap->bmBits))[i * bytesPerRow + j * 3 + 2];
			pPixelsBuff[i * bytesPerRow + j * 3 + 2] = ((BYTE *)(bitmap->bmBits))[i * bytesPerRow + j * 3];
		}
		for (LONG lp = 0; lp < bytesPerRow - bitmap->bmWidth * 3; lp++)
		{
			pPixelsBuff[i * bytesPerRow + bitmap->bmWidth * 3 + lp] = 0;
		}
	}

	CFile hFile;
	CFileException e;
	hFile.Open(filePath, CFile::modeCreate | CFile::modeReadWrite, &e);
	hFile.Write(bmpFileHeader, sizeof(BITMAPFILEHEADER));
	hFile.Write(bmpInfo, sizeof(BITMAPINFO));
	hFile.Write(bitmap->bmBits, (bitmap->bmWidth + 1) * 3 / 4 * 4 * bitmap->bmHeight);
	hFile.Close();

	delete pPixelsBuff;
	pPixelsBuff = nullptr;
}


void CProcCtrlPage::OnBnClickedChkLightcenter()
{
	// TODO:  在此添加控件通知处理程序代码
	CDeflectionProcDoc* pDoc = pSuperSheet->pSuperView->GetDocument();
	switch (((CButton *)GetDlgItem(IDC_CHK_LIGHTCENTER))->GetCheck())
	{
	case 0: //Not checked
	{
		pDoc->isDisplayLightCenter = false;
		break;
	}
	case 1: //Checked
	{
		pDoc->isDisplayLightCenter = true;
		break;
	}
	default:
	{
		return;
		//break;
	}
	}
	pDoc->pDeflectionProcView->Invalidate(true);
}


void CProcCtrlPage::OnBnClickedBtnCorrectdistortion()
{
	// TODO:  在此添加控件通知处理程序代码
	CDeflectionProcDoc* pDoc = pSuperSheet->pSuperView->GetDocument();
	UpdateData(true);
	pDoc->pDeflectionProcView->UpdateLightCenter();
	pDoc->pDeflectionProcView->Invalidate(TRUE);
}


void CProcCtrlPage::OnBnClickedRadioSelcorrected()
{
	// TODO:  在此添加控件通知处理程序代码
	CDeflectionProcDoc* pDoc = pSuperSheet->pSuperView->GetDocument();
	pDoc->bmpDisplaySel = BmpDisplaySel::CORRECTED;
	pDoc->pDeflectionProcView->Invalidate(TRUE);
}


void CProcCtrlPage::OnBnClickedRadioSelexpanded()
{
	// TODO:  在此添加控件通知处理程序代码
	CDeflectionProcDoc* pDoc = pSuperSheet->pSuperView->GetDocument();
	pDoc->bmpDisplaySel = BmpDisplaySel::EXPANDED;
	pDoc->pDeflectionProcView->Invalidate(TRUE);
}

void CProcCtrlPage::OnBnClickedRadioSeldistorted()
{
	// TODO:  在此添加控件通知处理程序代码
	CDeflectionProcDoc* pDoc = pSuperSheet->pSuperView->GetDocument();
	pDoc->bmpDisplaySel = BmpDisplaySel::DISTORTED;
	pDoc->pDeflectionProcView->Invalidate(TRUE);
}
