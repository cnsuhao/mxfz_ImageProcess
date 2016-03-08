// MaskDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "MaskDlg.h"
#include "afxdialogex.h"


// CMaskDlg 对话框

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


// CMaskDlg 消息处理程序


void CMaskDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
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