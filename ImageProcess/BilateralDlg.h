#pragma once


// CBilateralDlg �Ի���

class CBilateralDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBilateralDlg)

public:
	CBilateralDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBilateralDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_BILATERAL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int r;
	double m_sigmaG;
	double m_sigmaS;
public:
	afx_msg void OnEnChangeR();
	afx_msg void OnBnClickedOk();
};
