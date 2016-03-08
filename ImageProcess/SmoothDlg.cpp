// SmoothDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "SmoothDlg.h"
#include "afxdialogex.h"


// CSmoothDlg �Ի���

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


// CSmoothDlg ��Ϣ�������


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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		KillTimer(0);
		UpdateData();
		m_data_y=m_Slider_y.GetPos();
		UpdateData(0);
		SetTimer(0,200,NULL);
	*pResult = 0;
}


void CSmoothDlg::OnEnChangeEditX()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	KillTimer(0);
	UpdateData();
	m_Slider_x.SetPos(m_data_x);
	UpdateData(0);
	SetTimer(0,200,NULL);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CSmoothDlg::OnEnChangeEditY()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	KillTimer(0);
	UpdateData();
	m_Slider_y.SetPos(m_data_y);
	UpdateData(0);
	SetTimer(0,200,NULL);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CSmoothDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetParent()->SendMessage(WM_SMOOTH_CERTAIN,m_data_x,m_data_y);
	CDialogEx::OnOK();
}


void CSmoothDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetParent()->SendMessage(WM_CANCEL_SMOOTH);
	CDialogEx::OnCancel();
}


void CSmoothDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetParent()->SendMessage(WM_CANCEL_SMOOTH);
	CDialogEx::OnClose();
}



void CSmoothDlg::OnBnClickedCheckPreview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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