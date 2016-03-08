#pragma once
#include "afxcmn.h"


// CSmoothDlg �Ի���

class CSmoothDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSmoothDlg)

public:
	CSmoothDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSmoothDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_SMOOTH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CSliderCtrl m_Slider_x;
	CSliderCtrl m_Slider_y;
	int m_data_x,m_xOrg;
	int m_data_y,m_yOrg;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSliderX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditX();
	afx_msg void OnEnChangeEditY();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedIdcancel();
	afx_msg void OnClose();
private:
	BOOL m_bPreview;
public:
	afx_msg void OnBnClickedCheckPreview();
};
