#pragma once
#include "afxwin.h"


// CHistogramDlg 对话框

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistogramDlg();

// 对话框数据
	enum { IDD = IDD_HISTOGRAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	CRect m_DrawRect;
public:
	afx_msg void OnCbnSelchangeColorSelect();
private:
	CString m_ColorSelect;
	int m_Color;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	CEdit m_Min;
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_Max;
	int m_Data_Min,m_Data_Max;
	int m_Curve[256];
public:
	afx_msg void OnEnChangeMin();
	afx_msg void OnEnChangeMax();
	void CalculateCurve();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnBnClickedHistogram();
private:
	BOOL m_bPreview;
public:
	afx_msg void OnBnClickedPreview();
};
