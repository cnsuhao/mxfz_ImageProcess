
// ImageProcessDlg.h : 头文件
//

#pragma once
#include "bmp.h"
#include "atltypes.h"
#include "smoothdlg.h"
#include "mediandlg.h"
#include "filterdefinedlg.h"
#include "curvedlg.h"
#include "ThresholdDlg.h"
#include "histogramdlg.h"
#include "ZoomDlg.h"
#include "FFTDlg.h"
#include "MorphologyDlg.h"
#include "MaskDlg.h"
#include "ThresholdExDlg.h"
#include "GaussDlg.h"
#include "BilateralDlg.h"
#define WM_SMOOTH_OK WM_USER+1
#define WM_CANCEL_SMOOTH WM_USER+2
#define WM_SMOOTH_CERTAIN WM_USER+3
#define WM_FILTER_DEFINE_OK WM_USER+4
#define WM_FILTER_DEFINE_CANCEL WM_USER+5
#define WM_FILTER_DEFINE_CERTAIN WM_USER+6
#define WM_CURVE_OK WM_USER+7
#define WM_CURVE_CERTAIN WM_USER+8
#define WM_CURVE_CANCEL WM_USER+9
#define WM_HISTOGRAM_OK WM_USER+10
#define WM_HISTOGRAM_CERTIAN WM_USER+11
#define WM_HISTOGRAM_CANCEL WM_USER+12
#define WM_FFT_CANCEL WM_USER+13
#define WM_MORPHOLOGY_OK WM_USER+14
#define WM_MORPHOLOGY_CERTAIN WM_USER+15
#define WM_MORPHOLOGY_CANCEL WM_USER+16
// CImageProcessDlg 对话框
class CImageProcessDlg : public CDialogEx
{
// 构造
public:
	CImageProcessDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_MFCBMP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString m_FileName;
public:
	afx_msg void OnBnClickedFileOpen();
public:
	Bmp m_Bmp;
	CRect m_ShowRect;
public:
	afx_msg void OnBnClickedSaveFile();
	afx_msg void OnDropFiles(HDROP hDropInfo);
public:
	afx_msg void OnBnClickedSaveText();
private:
	bool m_bBusy;
	bool m_bOpenDlg;
	double m_Filter[5][5];
	CPoint m_pt_lu,m_pt_rd;
	CFilterDefineDlg *m_FDDlg;
	CCurveDlg *m_CDlg;
	CHistogramDlg *m_HDlg;
	CFFTDlg *m_FFTDlg;
	CMorphologyDlg *m_MDlg;
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPopupFileOpen();
	afx_msg void OnPopupFileSave();
	afx_msg void OnPopupFileSaveText();
	afx_msg void OnPopupFileInfo();
	afx_msg void OnPopupReverseColor();
	afx_msg void OnPopupReverseLr();
	afx_msg void OnPopupReverseUd();
	afx_msg void OnPopupReverse180();
	afx_msg void OnBnClickedFileInfo();
	afx_msg void OnPopup33Smooth();
	afx_msg void OnPopup55Smooth();
	afx_msg void OnPopupAvgSmooth();
protected:
	afx_msg LRESULT OnSmoothOk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCancelSmooth(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSmoothCertain(WPARAM wParam, LPARAM lParam);
public:
	bool m_bSmoothDlgOpened;
	bool m_bFilterDlgOpened;
	bool m_bCurveDlgOpened;
	bool m_bHistogramDlgOpened;
	bool m_bFT;
	bool m_bFFTDlgOpened;
	bool m_bMorphologyOpened;
	bool m_bIsJpg;
	bool m_bIsCutting;
public:
	afx_msg void OnPopup33Median();
	afx_msg void OnPopup55Median();
	afx_msg void OnPopupMedian();
	afx_msg void OnFilterDefine();
protected:
	afx_msg LRESULT OnFilterDefineOk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFilterDefineCancel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFilterDefineCertain(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPopupCurve();
protected:
	afx_msg LRESULT OnCurveOk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCurveCertain(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCurveCancel(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnHistogramEqualization();
	afx_msg void OnBnClickedBtnReload();
	afx_msg void OnPopupReload();
	afx_msg void OnPopupThreshold();
	afx_msg void OnPopupLpls4();
	afx_msg void OnPopupLpls8();
	afx_msg void OnHistogram();
protected:
	afx_msg LRESULT OnHistogramOk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHistogramCertian(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHistogramCancel(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPopupRgb2Gray();
	afx_msg void OnPopupZoom();
	afx_msg void OnPopupDft();
	afx_msg void OnPopupFft();
	afx_msg void OnPopupIfft();
	afx_msg void OnPopupIdft();
	afx_msg void OnFftFilter();
protected:
	afx_msg LRESULT OnFftCancel(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnConnectMe();
	afx_msg void OnPopupMorphology();
protected:
	afx_msg LRESULT OnMorphologyOk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMorphologyCertain(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMorphologyCancel(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPopupGetboard();
	afx_msg void OnPopupMask();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPopupThresholdEx();
	afx_msg void OnPopupCut();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPopupCutInfo();
	afx_msg void OnPopupBilateral();
	afx_msg void OnPopupGauss();
};
