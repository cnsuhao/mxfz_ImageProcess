#pragma once
#include "afxwin.h"


// CFilterDefineDlg 对话框

class CFilterDefineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterDefineDlg)

public:
	CFilterDefineDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFilterDefineDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_FILTER_DEFINE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_data[5][5];
private:
	double m_data_old[5][5];
	int m_data_divide,m_data_divide_old;
	int m_data_move,m_data_move_old;
	BOOL m_bPreview,m_bPreview_old;
	CString m_Default,m_Default_old;
	bool m_bData_Changed;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedCheckPreview();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnCbnSelchangeComboDefault();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
};
