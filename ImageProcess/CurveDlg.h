#pragma once


// CCurveDlg 对话框

class CCurveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCurveDlg)

public:
	CCurveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCurveDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_CURVE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	void SetColorCount(int Color);
private:
	CRect m_DrawRect;
	int m_Curve[4][256];
	int m_Color;
	CPoint m_PointSet[4][10]; 
public:
	afx_msg void OnCbnColorSelect();
private:
	CString m_ColorSelect;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	int m_X;
	int m_Y;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void CalculateCurve();
	int GetCurveYfromX(int color,int x);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
private:
	int m_d_X;
	int m_d_Y;
public:
	afx_msg void OnBnClickedBtnAddPoint();
	afx_msg void OnBnClickedBtnClear();
	virtual BOOL OnInitDialog();
};
