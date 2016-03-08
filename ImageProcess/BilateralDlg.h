#pragma once


// CBilateralDlg 对话框

class CBilateralDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBilateralDlg)

public:
	CBilateralDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBilateralDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_BILATERAL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int r;
	double m_sigmaG;
	double m_sigmaS;
public:
	afx_msg void OnEnChangeR();
	afx_msg void OnBnClickedOk();
};
