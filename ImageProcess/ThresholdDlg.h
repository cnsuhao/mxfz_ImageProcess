#pragma once


// CThresholdDlg 对话框

class CThresholdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThresholdDlg)

public:
	CThresholdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CThresholdDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_THRESHOLD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	int m_Threshold;
public:
	afx_msg void OnBnClickedOk();
	int GetThreshold();
};
