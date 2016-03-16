#pragma once


// CThresholdExDlg 对话框

class CThresholdExDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThresholdExDlg)

public:
	CThresholdExDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CThresholdExDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_THRESHOLD_EX_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal(Bmp *bmp);
protected:
	Bmp *m_Bmp;
	unsigned char *m_Buf;
	unsigned char***m_RGB_Bk;
	int **m_Histogram;
	long m_Height;
	long m_Width;
	long m_Color;
	CRect m_HistogramDrawRect;
	unsigned char Threshold;
private:
	CString m_Method;
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void SetBuf();
	afx_msg void OnCbnSelchangeMethodCmb();
	void ThresholdSimple();
	void DoThreshold();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	int m_Threshold_int;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedThreshold();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
};
