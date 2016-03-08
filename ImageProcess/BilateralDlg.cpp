// BilateralDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "BilateralDlg.h"
#include "afxdialogex.h"


// CBilateralDlg 对话框

IMPLEMENT_DYNAMIC(CBilateralDlg, CDialogEx)

CBilateralDlg::CBilateralDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBilateralDlg::IDD, pParent)
	, r(2)
	, m_sigmaG(50)
	, m_sigmaS(12.5)
{

}

CBilateralDlg::~CBilateralDlg()
{
}

void CBilateralDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_R, r);
	DDV_MinMaxInt(pDX, r, 0, 255);
	DDX_Text(pDX, IDC_SIGMA_G, m_sigmaG);
	DDV_MinMaxDouble(pDX, m_sigmaG, 0, 65535);
	DDX_Text(pDX, IDC_SIGMA_S, m_sigmaS);
	DDV_MinMaxDouble(pDX, m_sigmaS, 0, 65535);
}


BEGIN_MESSAGE_MAP(CBilateralDlg, CDialogEx)
	ON_EN_CHANGE(IDC_R, &CBilateralDlg::OnEnChangeR)
	ON_BN_CLICKED(IDOK, &CBilateralDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBilateralDlg 消息处理程序


void CBilateralDlg::OnEnChangeR()
{
	UpdateData();
	double d = 2 * r + 1;
	m_sigmaG = d*d * 2;
	m_sigmaS = d*d / 2;
	UpdateData(0);
}


void CBilateralDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	CDialogEx::OnOK();
}
