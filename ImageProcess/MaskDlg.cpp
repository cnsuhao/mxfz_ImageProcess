// MaskDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "MaskDlg.h"
#include "afxdialogex.h"


// CMaskDlg �Ի���

IMPLEMENT_DYNAMIC(CMaskDlg, CDialogEx)

CMaskDlg::CMaskDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMaskDlg::IDD, pParent)
	, m_MaskPresentage(20)
{

}

CMaskDlg::~CMaskDlg()
{
}

void CMaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT1, m_MaskPresentage);
}


BEGIN_MESSAGE_MAP(CMaskDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMaskDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMaskDlg ��Ϣ�������


void CMaskDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if(m_MaskPresentage>100)
	{
		m_MaskPresentage=100;
	}
	if(m_MaskPresentage<0)
	{
		m_MaskPresentage=0;
	}
	UpdateData(0);
	CDialogEx::OnOK();
}
double CMaskDlg::GetPresenage()
{
	return m_MaskPresentage;
}