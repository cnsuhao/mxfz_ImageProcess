// SmoothDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "SmoothDlg.h"
#include "afxdialogex.h"


// CSmoothDlg 对话框

IMPLEMENT_DYNAMIC(CSmoothDlg, CDialogEx)

CSmoothDlg::CSmoothDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSmoothDlg::IDD, pParent)
	, m_data_x(0)
	, m_data_y(0)
	, m_bPreview(FALSE)
{
	
}

CSmoothDlg::~CSmoothDlg()
{
}

void CSmoothDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_X, m_Slider_x);
	DDX_Control(pDX, IDC_SLIDER_Y, m_Slider_y);
	DDX_Text(pDX, IDC_EDIT_X, m_data_x);
	DDX_Text(pDX, IDC_EDIT_Y, m_data_y);
	DDX_Check(pDX, IDC_CHECK_PREVIEW, m_bPreview);
}


BEGIN_MESSAGE_MAP(CSmoothDlg, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_X, &CSmoothDlg::OnNMCustomdrawSliderX)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Y, &CSmoothDlg::OnNMCustomdrawSliderY)
	ON_EN_CHANGE(IDC_EDIT_X, &CSmoothDlg::OnEnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, &CSmoothDlg::OnEnChangeEditY)
	ON_BN_CLICKED(IDOK, &CSmoothDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(BTN_IDCANCEL, &CSmoothDlg::OnBnClickedIdcancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_PREVIEW, &CSmoothDlg::OnBnClickedCheckPreview)
END_MESSAGE_MAP()


// CSmoothDlg 消息处理程序


BOOL CSmoothDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CenterWindow();

	m_Slider_x.SetRange(1,30);
	m_Slider_y.SetRange(1,30);

	m_Slider_x.SetTicFreq(1);
	m_Slider_y.SetTicFreq(1);

	m_xOrg=1;
	m_yOrg=1;

	m_bPreview=1;
	UpdateData(0);

	SetTimer(0,200,NULL);
	return TRUE;
}


void CSmoothDlg::OnNMCustomdrawSliderX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//@TN
		*pResult = 0;
	// TODO: 在此添加控件通知处理程序代码
		KillTimer(0);
		UpdateData();
		m_data_x=m_Slider_x.GetPos();
		UpdateData(0);
		SetTimer(0,200,NULL);
	*pResult = 0;
}


void CSmoothDlg::OnNMCustomdrawSliderY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//@TN
		*pResult = 0;
	// TODO: 在此添加控件通知处理程序代码
		KillTimer(0);
		UpdateData();
		m_data_y=m_Slider_y.GetPos();
		UpdateData(0);
		SetTimer(0,200,NULL);
	*pResult = 0;
}


void CSmoothDlg::OnEnChangeEditX()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	KillTimer(0);
	UpdateData();
	m_Slider_x.SetPos(m_data_x);
	UpdateData(0);
	SetTimer(0,200,NULL);
	// TODO:  在此添加控件通知处理程序代码
}


void CSmoothDlg::OnEnChangeEditY()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	KillTimer(0);
	UpdateData();
	m_Slider_y.SetPos(m_data_y);
	UpdateData(0);
	SetTimer(0,200,NULL);
	// TODO:  在此添加控件通知处理程序代码
}


void CSmoothDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetParent()->SendMessage(WM_SMOOTH_CERTAIN,m_data_x,m_data_y);
	CDialogEx::OnOK();
}


void CSmoothDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!m_bPreview)
	{
		return;
	}
	if(m_xOrg!=m_data_x||m_yOrg!=m_data_y)
	{
		m_xOrg=m_data_x;
		m_yOrg=m_data_y;
		GetParent()->SendMessage(WM_SMOOTH_OK,m_data_x,m_data_y);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CSmoothDlg::OnBnClickedIdcancel()
{
	// TODO: 在此添加控件通知处理程序代码
	GetParent()->SendMessage(WM_CANCEL_SMOOTH);
	CDialogEx::OnCancel();
}


void CSmoothDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetParent()->SendMessage(WM_CANCEL_SMOOTH);
	CDialogEx::OnClose();
}



void CSmoothDlg::OnBnClickedCheckPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_bPreview==0)
	{
		GetParent()->SendMessage(WM_CANCEL_SMOOTH);
	}
	else
	{
		m_xOrg=m_data_x;
		m_yOrg=m_data_y;
		GetParent()->SendMessage(WM_SMOOTH_OK,m_data_x,m_data_y);
	}
}