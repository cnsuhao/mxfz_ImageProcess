#pragma once


// CGaussDlg �Ի���

class CGaussDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGaussDlg)

public:
	CGaussDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGaussDlg();

// �Ի�������
	enum { IDD = IDD_GAUSS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int r;
	afx_msg void OnBnClickedOk();
	double m_sigma;
};
