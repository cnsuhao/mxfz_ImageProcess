#pragma once


// CMaskDlg �Ի���

class CMaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMaskDlg)

public:
	CMaskDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMaskDlg();

// �Ի�������
	enum { IDD = IDD_MASK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	double m_MaskPresentage;
public:
	afx_msg void OnBnClickedOk();
	double GetPresenage();
};
