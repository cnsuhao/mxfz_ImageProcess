// GaussDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "GaussDlg.h"
#include "afxdialogex.h"


// CGaussDlg �Ի���

IMPLEMENT_DYNAMIC(CGaussDlg, CDialogEx)

CGaussDlg::CGaussDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGaussDlg::IDD, pParent)
	, r(2)
	, m_sigma(1)
{

}

CGaussDlg::~CGaussDlg()
{
}

void CGaussDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_R, r);
	DDV_MinMaxInt(pDX, r, 0, 255);
	DDX_Text(pDX, IDC_EDIT_SIGMA, m_sigma);
	DDV_MinMaxDouble(pDX, m_sigma, 0, 255);
}


BEGIN_MESSAGE_MAP(CGaussDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGaussDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGaussDlg ��Ϣ�������


void CGaussDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CDialogEx::OnOK();
}
