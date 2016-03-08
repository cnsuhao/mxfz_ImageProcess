// ThresholdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ThresholdDlg.h"
#include "afxdialogex.h"


// CThresholdDlg �Ի���

IMPLEMENT_DYNAMIC(CThresholdDlg, CDialogEx)

CThresholdDlg::CThresholdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CThresholdDlg::IDD, pParent)
	, m_Threshold(128)
{

}

CThresholdDlg::~CThresholdDlg()
{
}

void CThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_THRESHOLD, m_Threshold);
	DDV_MinMaxInt(pDX, m_Threshold, 0, 255);
}


BEGIN_MESSAGE_MAP(CThresholdDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CThresholdDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CThresholdDlg ��Ϣ�������


void CThresholdDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	UpdateData(0);
	CDialogEx::OnOK();
}

int CThresholdDlg::GetThreshold()
{
	return m_Threshold;
}