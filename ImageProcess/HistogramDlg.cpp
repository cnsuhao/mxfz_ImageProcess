// HistogramDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"


// CHistogramDlg 对话框

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistogramDlg::IDD, pParent)
	, m_ColorSelect(_T("RGB"))
	, m_bPreview(TRUE)
{
	m_DrawRect.top=m_DrawRect.left=0;
	m_DrawRect.right=258;
	m_DrawRect.bottom=102;
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_TOOL_IMAGEPROCESS_COLOR_SELECT, m_ColorSelect);
	DDX_Control(pDX, IDC_TOOL_IMAGEPROCESS_MIN, m_Min);
	DDX_Control(pDX, IDC_TOOL_IMAGEPROCESS_MAX, m_Max);
	DDX_Check(pDX, IDC_TOOL_IMAGEPROCESS_PREVIEW, m_bPreview);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_TOOL_IMAGEPROCESS_COLOR_SELECT, &CHistogramDlg::OnCbnSelchangeColorSelect)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_TOOL_IMAGEPROCESS_MIN, &CHistogramDlg::OnEnChangeMin)
	ON_EN_CHANGE(IDC_TOOL_IMAGEPROCESS_MAX, &CHistogramDlg::OnEnChangeMax)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDOK, &CHistogramDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL_HISTOGRAM, &CHistogramDlg::OnBnClickedHistogram)
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_PREVIEW, &CHistogramDlg::OnBnClickedPreview)
END_MESSAGE_MAP()


// CHistogramDlg 消息处理程序


void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC cdc;
	CBitmap Bmp;
	cdc.CreateCompatibleDC(&dc);
	Bmp.CreateCompatibleBitmap(&dc,m_DrawRect.Width(),m_DrawRect.Height());
	cdc.SelectObject(Bmp);

	CPen pen(PS_SOLID,1,RGB(170,170,170));
	CPen *oldpen=cdc.SelectObject(&pen);
	cdc.Rectangle(m_DrawRect);
	cdc.SelectObject(oldpen);

	int **Histogram=((CImageProcessDlg*)GetParent())->m_Bmp.GetHistogram();
	m_Color=((CImageProcessDlg*)GetParent())->m_Bmp.GetColor();

	UpdateData();
	if(m_ColorSelect!=_T("灰度")&&m_Color==1)
	{
		m_ColorSelect=_T("灰度");
	}
	else if(m_ColorSelect==_T("灰度")&&m_Color!=1)
	{
		m_ColorSelect=_T("RGB");
	}
	UpdateData(0);

	int HistogramMax=0;
	if(m_ColorSelect==_T("灰度")||(m_ColorSelect==_T("红")&&m_Color==3))
	{
		for(int i=0;i<256;i++)
		{
			HistogramMax<*(*(Histogram+0)+i)?HistogramMax=*(*(Histogram+0)+i):NULL;
		}
		for(int i=0;i<256;i++)
		{
			cdc.MoveTo(i+1,m_DrawRect.bottom-2);
			cdc.LineTo(i+1,static_cast<int>(m_DrawRect.bottom-2-double(*(*(Histogram+0)+i))/HistogramMax*100));
		}
	}
	else if((m_ColorSelect==_T("红")&&m_Color==4)||(m_ColorSelect==_T("绿")&&m_Color==3))
	{
		for(int i=0;i<256;i++)
		{
			HistogramMax<*(*(Histogram+1)+i)?HistogramMax=*(*(Histogram+1)+i):NULL;
		}
		for(int i=0;i<256;i++)
		{
			cdc.MoveTo(i+1,m_DrawRect.bottom-2);
			cdc.LineTo(i+1,static_cast<int>(m_DrawRect.bottom-2-double(*(*(Histogram+1)+i))/HistogramMax*100));
		}
	}
	else if((m_ColorSelect==_T("绿")&&m_Color==4)||(m_ColorSelect==_T("蓝")&&m_Color==3))
	{
		for(int i=0;i<256;i++)
		{
			HistogramMax<*(*(Histogram+2)+i)?HistogramMax=*(*(Histogram+2)+i):NULL;
		}
		for(int i=0;i<256;i++)
		{
			cdc.MoveTo(i+1,m_DrawRect.bottom-2);
			cdc.LineTo(i+1,static_cast<int>(m_DrawRect.bottom-2-double(*(*(Histogram+2)+i))/HistogramMax*100));
		}
	}
	else if(m_ColorSelect==_T("蓝")&&m_Color==4)
	{
		for(int i=0;i<256;i++)
		{
			HistogramMax<*(*(Histogram+3)+i)?HistogramMax=*(*(Histogram+3)+i):NULL;
		}
		for(int i=0;i<256;i++)
		{
			cdc.MoveTo(i+1,m_DrawRect.bottom-2);
			cdc.LineTo(i+1,static_cast<int>(m_DrawRect.bottom-2-double(*(*(Histogram+3)+i))/HistogramMax*100));
		}
	}

	else if(m_ColorSelect==_T("RGB"))
	{
		for(int i=0;i<256;i++)
		{
			*(*(Histogram+m_Color-3)+i)=*(*(Histogram+m_Color-3)+i)+*(*(Histogram+m_Color-2)+i)+*(*(Histogram+m_Color-1)+i);
		}
		for(int i=0;i<256;i++)
		{
			HistogramMax<*(*(Histogram+m_Color-3)+i)?HistogramMax=*(*(Histogram+m_Color-3)+i):NULL;
		}
		for(int i=0;i<256;i++)
		{
			cdc.MoveTo(i+1,m_DrawRect.bottom-2);
			cdc.LineTo(i+1,static_cast<int>(m_DrawRect.bottom-2-double(*(*(Histogram+m_Color-3)+i))/HistogramMax*100));
		}
	}

	CPen GreenPen(PS_SOLID,1,RGB(0,255,0)),RedPen(PS_SOLID,1,RGB(255,0,0));
	m_Data_Min<m_Data_Max?cdc.SelectObject(GreenPen):cdc.SelectObject(RedPen);
	if(m_Data_Min!=0)
	{
		cdc.MoveTo(m_Data_Min+1,m_DrawRect.bottom-2);
		cdc.LineTo(m_Data_Min+1,m_DrawRect.top);
	}
	if(m_Data_Max!=255)
	{
		cdc.MoveTo(m_Data_Max+1,m_DrawRect.bottom-2);
		cdc.LineTo(m_Data_Max+1,m_DrawRect.top);
	}

	cdc.SelectObject(oldpen);
	dc.BitBlt(m_DrawRect.left+10,m_DrawRect.top+10,m_DrawRect.Width(),m_DrawRect.Height(),&cdc,0,0,SRCCOPY);
	cdc.DeleteDC();
}


void CHistogramDlg::OnCbnSelchangeColorSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	InvalidateRect(CRect(m_DrawRect.left+10,m_DrawRect.top+10,m_DrawRect.left+10+m_DrawRect.Width(),m_DrawRect.top+10+m_DrawRect.Height()),0);
	CalculateCurve();
}


void CHistogramDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(((CImageProcessDlg*)GetParent())->m_bHistogramDlgOpened)
	{
		if(nIDEvent==0)
		{
			InvalidateRect(CRect(m_DrawRect.left+10,m_DrawRect.top+10,m_DrawRect.left+10+m_DrawRect.Width(),m_DrawRect.top+10+m_DrawRect.Height()),0);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


int CHistogramDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTimer(0,500,NULL);
		return 0;
}


BOOL CHistogramDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Min.SetWindowText(_T("0"));
	m_Max.SetWindowText(_T("255"));
	return 0;
}


void CHistogramDlg::OnEnChangeMin()
{
	LPTSTR str=new TCHAR[10];
	m_Min.GetWindowText(str,5);
	m_Data_Min=StrToIntA(str);
	if(m_Data_Min>255||m_Data_Min<0)
	{
		m_Data_Min=0;
		wsprintf(str,_T("%d"),m_Data_Min);
		m_Min.SetWindowText(str);
	}
	CalculateCurve();
}


void CHistogramDlg::OnEnChangeMax()
{
	LPTSTR str=new TCHAR[10];
	m_Max.GetWindowText(str,5);
	m_Data_Max=StrToIntA(str);

	if(m_Data_Max>255||m_Data_Max<0)
	{
		m_Data_Max=255;
		wsprintf(str,_T("%d"),m_Data_Max);
		m_Max.SetWindowText(str);
	}
	CalculateCurve();
}

void CHistogramDlg::CalculateCurve()
{
	InvalidateRect(CRect(m_DrawRect.left+10,m_DrawRect.top+10,m_DrawRect.left+10+m_DrawRect.Width(),m_DrawRect.top+10+m_DrawRect.Height()),0);
	if(m_Data_Max<=m_Data_Min)
	{
		return;
	}	
	int m_WhatColor;//0:R 1:G 2:B 3:RGBor灰度
	if(m_ColorSelect==_T("RGB")||m_ColorSelect==_T("灰度"))
	{
		m_WhatColor=3;
	}
	else if(m_ColorSelect==_T("红"))
	{
		m_WhatColor=0;
	}
	else if(m_ColorSelect==_T("绿"))
	{
		m_WhatColor=1;
	}
	else if(m_ColorSelect==_T("蓝"))
	{
		m_WhatColor=2;
	}
	if(m_bPreview)
	{
		GetParent()->SendMessage(WM_HISTOGRAM_OK,m_WhatColor,m_Data_Min*256+m_Data_Max);
	}
	//发信息
}

void CHistogramDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(PtInRect(CRect(m_DrawRect.left+10,m_DrawRect.top+10,m_DrawRect.left+10+m_DrawRect.Width(),m_DrawRect.top+10+m_DrawRect.Height()),point))
	{
		LPTSTR str=new TCHAR[10];
		m_Data_Min=point.x-11;
		wsprintf(str,_T("%d"),m_Data_Min);
		m_Min.SetWindowText(str);
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CHistogramDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(PtInRect(CRect(m_DrawRect.left+10,m_DrawRect.top+10,m_DrawRect.left+10+m_DrawRect.Width(),m_DrawRect.top+10+m_DrawRect.Height()),point))
	{
		LPTSTR str=new TCHAR[10];
		m_Data_Max=point.x-11;
		wsprintf(str,_T("%d"),m_Data_Max);
		m_Max.SetWindowText(str);
	}
	CDialogEx::OnRButtonUp(nFlags, point);
}


void CHistogramDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nFlags&VK_LBUTTON)
	{
		OnLButtonDown(nFlags,point);
	}
	if(nFlags&VK_RBUTTON)
	{
		OnRButtonDown(nFlags,point);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CHistogramDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Data_Max<=m_Data_Min)
	{
		return;
	}	
	int m_WhatColor;//0:R 1:G 2:B 3:RGBor灰度
	if(m_ColorSelect==_T("RGB")||m_ColorSelect==_T("灰度"))
	{
		m_WhatColor=3;
	}
	else if(m_ColorSelect==_T("红"))
	{
		m_WhatColor=0;
	}
	else if(m_ColorSelect==_T("绿"))
	{
		m_WhatColor=1;
	}
	else if(m_ColorSelect==_T("蓝"))
	{
		m_WhatColor=2;
	}
	GetParent()->SendMessage(WM_HISTOGRAM_CERTIAN,m_WhatColor,m_Data_Min*256+m_Data_Max);
	CDialogEx::OnOK();
}


void CHistogramDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetParent()->SendMessage(WM_HISTOGRAM_CANCEL);
	CDialogEx::OnClose();
}


void CHistogramDlg::OnBnClickedHistogram()
{
	OnClose();
	CDialogEx::OnCancel();
	// TODO: 在此添加控件通知处理程序代码
}


void CHistogramDlg::OnBnClickedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(!m_bPreview)
	{
		GetParent()->SendMessage(WM_HISTOGRAM_OK,0,255);
	}
	else
	{
		CalculateCurve();
	}
}
