#pragma once


// CThresholdDlg �Ի���

class CThresholdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThresholdDlg)

public:
	CThresholdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CThresholdDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_THRESHOLD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	int m_Threshold;
public:
	afx_msg void OnBnClickedOk();
	int GetThreshold();
};
