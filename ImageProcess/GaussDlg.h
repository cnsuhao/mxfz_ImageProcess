#pragma once


// CGaussDlg 对话框

class CGaussDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGaussDlg)

public:
	CGaussDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGaussDlg();

// 对话框数据
	enum { IDD = IDD_GAUSS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int r;
	afx_msg void OnBnClickedOk();
	double m_sigma;
};
