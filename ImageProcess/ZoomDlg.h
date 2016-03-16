#pragma once
#include "afxwin.h"


// CZoomDlg �Ի���

class CZoomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZoomDlg)

public:
	CZoomDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZoomDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_ZOOM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	int m_Org_H;
	int m_Org_W;
	int m_Des_H;
	int m_Des_W;
	CString m_Method;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	int GetH(),GetW(),GetMethod();
private:
	BOOL m_bWHinScale;
public:
	afx_msg void OnBnClickedScale();
	afx_msg void OnEnChangeDesH();
private:
	CEdit m_Des_H_Control;
	CEdit m_Des_W_Control;
public:
	afx_msg void OnEnChangeDesW();
	afx_msg void OnCbnSelchangeMethod();
	virtual BOOL OnInitDialog();
};
