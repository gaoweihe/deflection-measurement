#pragma once
#include "resource.h"
#include "stdafx.h"

class CCltCtrlSheet;
// CProcCtrlPage dialog

class CProcCtrlPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CProcCtrlPage)

public:
	// standard constructor
	CProcCtrlPage();
	virtual ~CProcCtrlPage();

	enum { IDD = IDD_PROCCTRL };

	CCltCtrlSheet* pSuperSheet;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCCTRL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnBrowse();
	CString m_val_srcPath;
	afx_msg void OnBnClickedBtnSetthres();
	unsigned int m_val_thres;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioSelsource();
	afx_msg void OnBnClickedRadioSelbinarized();
	afx_msg void OnBnClickedRadioSelshrinked();
	afx_msg void OnBnClickedBtnSavebmp();
	afx_msg void OnBnClickedChkLightcenter();
	double m_param_a;
	double m_param_b;
	double m_param_c;
	double m_param_U;
	afx_msg void OnBnClickedBtnCorrectdistortion();
	afx_msg void OnBnClickedRadioSelcorrected();
	afx_msg void OnBnClickedRadioSelexpanded();
	afx_msg void OnBnClickedRadioSeldistorted();
};
