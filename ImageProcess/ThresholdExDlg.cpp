// ThresholdExDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ThresholdExDlg.h"
#include "afxdialogex.h"


// CThresholdExDlg 对话框

IMPLEMENT_DYNAMIC(CThresholdExDlg, CDialogEx)

CThresholdExDlg::CThresholdExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CThresholdExDlg::IDD, pParent)
	, m_Method(_T("直接阈值"))
	, Threshold(128)
	, m_Threshold_int(128)
{
	m_HistogramDrawRect.SetRect(0,0,257,102);
}

CThresholdExDlg::~CThresholdExDlg()
{
}

void CThresholdExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_METHOD_CMB, m_Method);
	DDX_Text(pDX, IDC_THRESHOLD_EDIT, m_Threshold_int);
}


BEGIN_MESSAGE_MAP(CThresholdExDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_METHOD_CMB, &CThresholdExDlg::OnCbnSelchangeMethodCmb)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CThresholdExDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL_THRESHOLD, &CThresholdExDlg::OnBnClickedThreshold)
END_MESSAGE_MAP()


// CThresholdExDlg 消息处理程序


INT_PTR CThresholdExDlg::DoModal(Bmp *bmp)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_Bmp=bmp;
	m_Color=bmp->GetColor();
	m_Height=bmp->GetHeight();
	m_Width=bmp->GetWidth();
	
	if(m_Color!=1)
	{
		m_Bmp->Save("tmp");
		m_Bmp->RGB2Gray();
	}

	long n=4*m_Height*m_Width;
	m_Buf=new unsigned char[n];
	SetBuf();
	m_Histogram=bmp->GetHistogram();

	return CDialogEx::DoModal();
}

void CThresholdExDlg::SetBuf()
{
	long n=4*m_Height*m_Width;
	for(int i=0;i<n;i+=4)
	{
		m_Buf[i+2]=m_Buf[i+1]=m_Buf[i]=m_Bmp->GetRGB(0,i/4/m_Width,i/4%m_Width);
	}
}

void CThresholdExDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	CBitmap Bmp;
	Bmp.CreateCompatibleBitmap(&dc,m_HistogramDrawRect.Width(),m_HistogramDrawRect.Height());
	cdc.SelectObject(Bmp);

	CPen pen(PS_SOLID,1,RGB(170,170,170));
	CPen *oldpen=cdc.SelectObject(&pen);
	cdc.Rectangle(m_HistogramDrawRect);
	cdc.SelectObject(oldpen);

	CPen GrayPen(PS_SOLID,1,RGB(64,64,64));
	if(m_Method!=_T("直接阈值"))
	{
		cdc.SelectObject(GrayPen);
	}
	int HistogramMax=0;
	for(int i=0;i<256;i++)
	{
		HistogramMax<*(*(m_Histogram+0)+i)?HistogramMax=*(*(m_Histogram+0)+i):NULL;
	}
	for(int i=0;i<256;i++)
	{
		cdc.MoveTo(i+1,m_HistogramDrawRect.bottom-2);
		cdc.LineTo(i+1,static_cast<int>(m_HistogramDrawRect.bottom-2-double(*(*(m_Histogram+0)+i))/HistogramMax*100));
	}
	
	CPen GreenPen(PS_SOLID,1,RGB(0,255,0));
	if(m_Method!=_T("直接阈值"))
	{
		cdc.SelectObject(pen);
	}
	else
	{
		cdc.SelectObject(GreenPen);
	}
	cdc.MoveTo(Threshold+1,m_HistogramDrawRect.bottom-2);
	cdc.LineTo(Threshold+1,m_HistogramDrawRect.top);

	CDC BmpDc;
	BmpDc.CreateCompatibleDC(&dc);
	CBitmap Bmp2;
	Bmp2.CreateBitmap(m_Width,m_Height,1,32,m_Buf);
	BmpDc.SelectObject(Bmp2);

	cdc.SelectObject(oldpen);
	dc.BitBlt(130,10,m_HistogramDrawRect.Width(),m_HistogramDrawRect.Height(),&cdc,0,0,SRCCOPY);
	dc.BitBlt(130+m_HistogramDrawRect.Width()+10,10,m_Width,m_Height,&BmpDc,0,0,SRCCOPY);
	cdc.DeleteDC();
	BmpDc.DeleteDC();
}


int CThresholdExDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowPos(NULL,0,0,130+m_HistogramDrawRect.Width()+10+m_Width+30,max(180,10+m_Height+50),SWP_NOMOVE);
	CenterWindow();
	ThresholdSimple();
		return 0;
}

void CThresholdExDlg::OnCbnSelchangeMethodCmb()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	DoThreshold();
	Invalidate(0);
	UpdateData(0);
}

void CThresholdExDlg::DoThreshold()
{
	if(m_Method==_T("直接阈值"))
	{
		ThresholdSimple();
	}
	else if(m_Method==_T("大津法"))
	{
		m_Threshold_int=Threshold=m_Bmp->OtsuThreshold();
		ThresholdSimple();
	}
	else if(m_Method==_T("迭代法"))
	{
		m_Threshold_int=Threshold=m_Bmp->IterationThreshold();
		ThresholdSimple();
	}
	else if(m_Method=_T("对半分"))
	{
		m_Threshold_int=Threshold=m_Bmp->HalfCutThreshold();
		ThresholdSimple();
	}
}

void CThresholdExDlg::ThresholdSimple()
{
	m_Bmp->BackUpRGB();
	m_Bmp->Threshold(Threshold);
	SetBuf();
	m_Bmp->RestoreRGB();
}

void CThresholdExDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nFlags&VK_LBUTTON)
	{
		CRect HistogramDrawRect;
		HistogramDrawRect.SetRect(130,10,130+m_HistogramDrawRect.Width(),10+m_HistogramDrawRect.Height());
		if(m_Method==_T("直接阈值")&&PtInRect(HistogramDrawRect,point))
		{
			Threshold=min(255,static_cast<unsigned char>(max(0,point.x-1-130)));
			DoThreshold();
			m_Threshold_int=Threshold;
			UpdateData(0);
			Invalidate(0);
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CThresholdExDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnMouseMove(nFlags,point);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CThresholdExDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Bmp->Threshold(Threshold);
	CDialogEx::OnOK();
}


void CThresholdExDlg::OnBnClickedThreshold()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void CThresholdExDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_Color!=1&&PathFileExists(_T("tmp")))
		m_Bmp->Open("tmp");
	CDialogEx::OnCancel();
}
