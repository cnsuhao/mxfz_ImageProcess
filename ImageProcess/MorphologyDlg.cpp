// MorphologyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "MorphologyDlg.h"
#include "afxdialogex.h"


// CMorphologyDlg 对话框

IMPLEMENT_DYNAMIC(CMorphologyDlg, CDialogEx)

CMorphologyDlg::CMorphologyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMorphologyDlg::IDD, pParent)
	, m_Type(_T("膨胀"))
	, m_Threshold(-1)
	, m_MouseInRect(-1)
	, m_xPos(_T(""))
	, m_yPos(_T(""))
	, m_bPreview(true)
	, m_B_and_W(FALSE)
{
	int RectTop=80,RectLeft=120,RectLength=30;
	m_filter=new bool*[9];
	for(int i=0;i<9;i++)
	{
		*(m_filter+i)=new bool[9];
		for(int j=0;j<9;j++)
		{
			*(*(m_filter+i)+j)=false;
			m_FilterRect[i][j].SetRect(RectLeft+j*RectLength,RectTop+i*RectLength,RectLeft+(j+1)*RectLength-1,RectTop+(i+1)*RectLength-1);
		}
	}
	*(*(m_filter+4)+4)=true;
}

CMorphologyDlg::~CMorphologyDlg()
{
}

void CMorphologyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_TOOL_IMAGEPROCESS_TYPE, m_Type);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_THRESHOLD, m_Threshold);
	DDX_Control(pDX, IDC_TOOL_IMAGEPROCESS_THRESHOLD_SLD, m_ThresholdSld);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_XPOS, m_xPos);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_YPOS, m_yPos);
	DDX_Check(pDX, IDC_TOOL_IMAGEPROCESS_PREVIEW, m_bPreview);
	DDX_Check(pDX, IDC_TOOL_IMAGEPROCESS_B_AND_W, m_B_and_W);
	DDX_Control(pDX, IDC_TOOL_IMAGEPROCESS_THRESHOLD, m_Threshold_ctrl);
}


BEGIN_MESSAGE_MAP(CMorphologyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TOOL_IMAGEPROCESS_THRESHOLD_SLD, &CMorphologyDlg::OnNMCustomdrawThresholdSld)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_TOOL_IMAGEPROCESS_TYPE, &CMorphologyDlg::OnCbnSelchangeType)
	ON_BN_CLICKED(IDOK, &CMorphologyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_PREVIEW, &CMorphologyDlg::OnBnClickedPreview)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDMORPHOLOGYCANCEL, &CMorphologyDlg::OnBnClickedMorphologycancel)
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_B_AND_W, &CMorphologyDlg::OnBnClickedBAndW)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CMorphologyDlg 消息处理程序

BOOL CMorphologyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ThresholdSld.SetRange(0,255);
	m_ThresholdSld.SetTicFreq(1);
	m_ThresholdSld.SetPos(128);
	return TRUE;
}


void CMorphologyDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CBrush GrayBrush(RGB(160,160,160));
	CBrush BlackBrush(RGB(0,0,0));
	CBrush LGrayBrush(RGB(192,192,192));
	CBrush LBlackBrush(RGB(64,64,64));
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(*(*(m_filter+i)+j))
			{
				if(m_MouseInRect==i*9+j)
				{
					dc.FillRect(m_FilterRect[i][j],&LBlackBrush);
				}
				else
				{
					dc.FillRect(m_FilterRect[i][j],&BlackBrush);
				}
			}
			else
			{
				if(m_MouseInRect==i*9+j)
				{
					dc.FillRect(m_FilterRect[i][j],&LGrayBrush);
				}
				else
				{
					dc.FillRect(m_FilterRect[i][j],&GrayBrush);
				}
			}
		}
	}
}


void CMorphologyDlg::OnNMCustomdrawThresholdSld(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData();
	m_Threshold=m_ThresholdSld.GetPos();
	UpdateData(0);
	if(m_bPreview)
		GetParent()->SendMessage(WM_MORPHOLOGY_OK);
}


void CMorphologyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(PtInRect(m_FilterRect[i][j],point))
			{
				m_MouseInRect=i*9+j;
				InvalidateRect(CRect(120,80,120+30*9,80+30*9),0);
				m_xPos.Format(_T("x=%d"),j-4);
				m_yPos.Format(_T("y=%d"),i-4);
				UpdateData(0);
				CDialogEx::OnMouseMove(nFlags, point);
				return;
			}
		}
	}
	m_MouseInRect=-1;
	InvalidateRect(CRect(120,80,120+30*9,80+30*9),0);
	m_xPos.Empty();
	m_yPos.Empty();
	UpdateData(0);
	CDialogEx::OnMouseMove(nFlags, point);
}


void CMorphologyDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(PtInRect(m_FilterRect[i][j],point))
			{
				*(*(m_filter+i)+j)=!*(*(m_filter+i)+j);
			}
		}
	}
	InvalidateRect(CRect(120,80,120+30*9,80+30*9),0);
	if(m_bPreview)
		GetParent()->SendMessage(WM_MORPHOLOGY_OK);
	CDialogEx::OnLButtonDown(nFlags, point);
}

bool** CMorphologyDlg::GetFilter()
{
	return m_filter;
}
int CMorphologyDlg::GetThreshold()
{
	UpdateData();
	return m_Threshold;
}
CString CMorphologyDlg::GetType()
{
	UpdateData();
	return m_Type;
}

void CMorphologyDlg::OnCbnSelchangeType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_bPreview)
		GetParent()->SendMessage(WM_MORPHOLOGY_OK);
}


void CMorphologyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetParent()->SendMessage(WM_MORPHOLOGY_CERTAIN);
	CDialogEx::OnOK();
}


void CMorphologyDlg::OnBnClickedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_bPreview)
		GetParent()->SendMessage(WM_MORPHOLOGY_OK);
	else
		GetParent()->SendMessage(WM_MORPHOLOGY_CANCEL,1);
	UpdateData(0);
}


void CMorphologyDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetParent()->SendMessage(WM_MORPHOLOGY_CANCEL);
	CDialogEx::OnClose();
}


void CMorphologyDlg::OnBnClickedMorphologycancel()
{
	// TODO: 在此添加控件通知处理程序代码
	GetParent()->SendMessage(WM_MORPHOLOGY_CANCEL);
	CDialogEx::OnCancel();
}


void CMorphologyDlg::OnBnClickedBAndW()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_B_and_W)
	{
		m_ThresholdSld.ShowWindow(SW_SHOW);
		m_Threshold_ctrl.ShowWindow(SW_SHOW);
		m_Threshold=m_ThresholdSld.GetPos();
	}
	else
	{
		m_ThresholdSld.ShowWindow(SW_HIDE);
		m_Threshold_ctrl.ShowWindow(SW_HIDE);
		m_Threshold=-1;
	}
	UpdateData(0);
	if(m_bPreview)
			GetParent()->SendMessage(WM_MORPHOLOGY_OK);
}

void CMorphologyDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			*(*(m_filter+i)+j)=false;
		}
	}
	*(*(m_filter+4)+4)=true;
	InvalidateRect(CRect(120,80,120+30*9,80+30*9),0);
	if(m_bPreview)
		GetParent()->SendMessage(WM_MORPHOLOGY_OK);
	CDialogEx::OnLButtonDblClk(nFlags, point);
}
