#pragma once


// CMedianDlg 对话框

class CMedianDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMedianDlg)

public:
	CMedianDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMedianDlg();

// 对话框数据
	enum { IDD = IDD_MEDIAN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_data_x;
	int m_data_y;
};
