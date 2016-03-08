#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMorphologyDlg 对话框

class CMorphologyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMorphologyDlg)

public:
	CMorphologyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMorphologyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_MORPHOLOGY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_Type;
	int m_Threshold;
	CSliderCtrl m_ThresholdSld;
	bool** m_filter;
	CRect m_FilterRect[9][9];
	int m_MouseInRect;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnNMCustomdrawThresholdSld(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	bool** GetFilter();
	int GetThreshold();
	CString GetType();
private:
	CString m_xPos;
	CString m_yPos;
public:
	afx_msg void OnCbnSelchangeType();
	afx_msg void OnBnClickedOk();
private:
	BOOL m_bPreview;
public:
	afx_msg void OnBnClickedPreview();
	afx_msg void OnClose();
	afx_msg void OnBnClickedMorphologycancel();
private:
	BOOL m_B_and_W;
public:
	afx_msg void OnBnClickedBAndW();
private:
	CEdit m_Threshold_ctrl;
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
