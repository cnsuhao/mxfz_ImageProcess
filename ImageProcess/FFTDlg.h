#pragma once
#include "afxwin.h"


// CFFTDlg 对话框

class CFFTDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFFTDlg)

public:
	CFFTDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFFTDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_FFT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void TurnComplexToAmplitudeAndAngle();
	void TurnAmplitudeAndAngleToComplex();
	unsigned char* GetBmpBufTrans();
	void CalculateFilter();
protected:
	COMPLEX** m_Complex;
	double*** m_Amplitude;
	double*** m_Angle;
	unsigned char*** m_RGB;
	double*** m_RGBtmp;
	int m_Color;
	int m_Height;
	int m_Width;
private:
	int m_ShowMode;
public:
	afx_msg void OnPaint();
private:
	CString m_Filter;
	int m_Param1;
	int m_Param2;
public:
	afx_msg void OnBnClickedAmplitude();
	afx_msg void OnBnClickedAngle();
	afx_msg void OnEnChangeParam1();
private:
	CEdit m_Param1_Con;
	CEdit m_Param2_Con;
public:
	afx_msg void OnEnChangeParam2();
	afx_msg void OnCbnSelchangeFilter();
private:
	CString m_Filter_Type;
public:
	afx_msg void OnBnClickedFftcancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
