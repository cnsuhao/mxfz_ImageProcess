// ZoomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "ZoomDlg.h"
#include "afxdialogex.h"


// CZoomDlg 对话框

IMPLEMENT_DYNAMIC(CZoomDlg, CDialogEx)

CZoomDlg::CZoomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CZoomDlg::IDD, pParent)
	, m_Org_H(0)
	, m_Org_W(0)
	, m_Des_H(0)
	, m_Des_W(0)
	, m_Method(_T("邻近"))
	, m_bWHinScale(TRUE)
{

}

CZoomDlg::~CZoomDlg()
{
}

void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_ORG_H, m_Org_H);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_ORG_W, m_Org_W);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_DES_H, m_Des_H);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_DES_W, m_Des_W);
	DDX_CBString(pDX, IDC_TOOL_IMAGEPROCESS_METHOD, m_Method);
	DDX_Check(pDX, IDC_TOOL_IMAGEPROCESS_SCALE, m_bWHinScale);
	DDX_Control(pDX, IDC_TOOL_IMAGEPROCESS_DES_H, m_Des_H_Control);
	DDX_Control(pDX, IDC_TOOL_IMAGEPROCESS_DES_W, m_Des_W_Control);
}


BEGIN_MESSAGE_MAP(CZoomDlg, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_SCALE, &CZoomDlg::OnBnClickedScale)
	ON_EN_CHANGE(IDC_TOOL_IMAGEPROCESS_DES_H, &CZoomDlg::OnEnChangeDesH)
	ON_EN_CHANGE(IDC_TOOL_IMAGEPROCESS_DES_W, &CZoomDlg::OnEnChangeDesW)
	ON_CBN_SELCHANGE(IDC_TOOL_IMAGEPROCESS_METHOD, &CZoomDlg::OnCbnSelchangeMethod)
END_MESSAGE_MAP()


// CZoomDlg 消息处理程序


int CZoomDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Des_H=m_Org_H=((CImageProcessDlg*)GetParent())->m_Bmp.GetHeight();
	m_Des_W=m_Org_W=((CImageProcessDlg*)GetParent())->m_Bmp.GetWidth();
	return 0;
}

int CZoomDlg::GetH()
{
	return m_Des_H;
}

int CZoomDlg::GetW()
{
	return m_Des_W;
}

int CZoomDlg::GetMethod()
{
	int Method;
	if(m_Method==_T("邻近"))
	{
		Method=0;
	}
	else if(m_Method==_T("两次线性"))
	{
		Method=1;
	}
	else if(m_Method==_T("两次立方"))
	{
		Method=2;
	}
	return Method;
}

void CZoomDlg::OnBnClickedScale()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_bWHinScale)
	{
		OnEnChangeDesH();
	}
	UpdateData(0);
}


void CZoomDlg::OnEnChangeDesH()
{
	if(m_bWHinScale)
	{
		LPTSTR str=new TCHAR[10];
		m_Des_H_Control.GetWindowText(str,10);
		if(str[0]==0)
		{
			return;
		}
		m_Des_H=StrToIntA(str);
		m_Des_W=m_Des_H*m_Org_W/m_Org_H;
		UpdateData(0);
	}
}


void CZoomDlg::OnEnChangeDesW()
{
	if(m_bWHinScale)
	{
		LPTSTR str=new TCHAR[10];
		m_Des_W_Control.GetWindowText(str,10);
		if(str[0]==0)
		{
			return;
		}
		m_Des_W=StrToIntA(str);
		m_Des_H=m_Des_W*m_Org_H/m_Org_W;
		UpdateData(0);
	}
}


void CZoomDlg::OnCbnSelchangeMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	UpdateData(0);
}
