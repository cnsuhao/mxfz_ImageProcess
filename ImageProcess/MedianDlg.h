#pragma once


// CMedianDlg �Ի���

class CMedianDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMedianDlg)

public:
	CMedianDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMedianDlg();

// �Ի�������
	enum { IDD = IDD_MEDIAN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_data_x;
	int m_data_y;
};
