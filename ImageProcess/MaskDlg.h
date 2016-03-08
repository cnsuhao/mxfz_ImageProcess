#pragma once


// CMaskDlg 对话框

class CMaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMaskDlg)

public:
	CMaskDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMaskDlg();

// 对话框数据
	enum { IDD = IDD_MASK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	double m_MaskPresentage;
public:
	afx_msg void OnBnClickedOk();
	double GetPresenage();
};
