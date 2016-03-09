// FFTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "FFTDlg.h"
#include "afxdialogex.h"


// CFFTDlg 对话框

IMPLEMENT_DYNAMIC(CFFTDlg, CDialogEx)

CFFTDlg::CFFTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFFTDlg::IDD, pParent)
	, m_ShowMode(0)
	, m_Filter(_T("无"))
	, m_Param1(0)
	, m_Param2(0)
	, m_Filter_Type(_T("理想滤波器"))
{

}

CFFTDlg::~CFFTDlg()
{
}

void CFFTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_TOOL_IMAGEPROCESS_AMPLITUDE, m_ShowMode);
	DDX_CBString(pDX, IDC_TOOL_IMAGEPROCESS_FILTER, m_Filter);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_PARAM1, m_Param1);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_PARAM2, m_Param2);
	DDX_Control(pDX, IDC_TOOL_IMAGEPROCESS_PARAM1, m_Param1_Con);
	DDX_Control(pDX, IDC_TOOL_IMAGEPROCESS_PARAM2, m_Param2_Con);
	DDX_CBString(pDX, IDC_TOOL_IMAGEPROCESS_FILTER_TYPE, m_Filter_Type);
}


BEGIN_MESSAGE_MAP(CFFTDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_AMPLITUDE, &CFFTDlg::OnBnClickedAmplitude)
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_ANGLE, &CFFTDlg::OnBnClickedAngle)
	ON_EN_CHANGE(IDC_TOOL_IMAGEPROCESS_PARAM1, &CFFTDlg::OnEnChangeParam1)
	ON_EN_CHANGE(IDC_TOOL_IMAGEPROCESS_PARAM2, &CFFTDlg::OnEnChangeParam2)
	ON_CBN_SELCHANGE(IDC_TOOL_IMAGEPROCESS_FILTER, &CFFTDlg::OnCbnSelchangeFilter)
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_FFTCANCEL, &CFFTDlg::OnBnClickedFftcancel)
	ON_BN_CLICKED(IDOK, &CFFTDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFFTDlg 消息处理程序


int CFFTDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Complex=((CImageProcessDlg*)GetParent())->m_Bmp.GetFFTComplex();
	m_Color=((CImageProcessDlg*)GetParent())->m_Bmp.GetColor();
	m_Height=((CImageProcessDlg*)GetParent())->m_Bmp.GetHeight();
	m_Width=((CImageProcessDlg*)GetParent())->m_Bmp.GetWidth();

	m_Amplitude=new double**[m_Color];
	m_Angle=new double**[m_Color];
	m_RGB=new unsigned char**[m_Color];
	m_RGBtmp=new double**[m_Color];
	for(int i=0;i<m_Color;i++)
	{
		m_Amplitude[i]=new double*[m_Height];
		m_Angle[i]=new double*[m_Height];
		m_RGB[i]=new unsigned char*[m_Height];
		m_RGBtmp[i]=new double*[m_Height];
	}
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			m_Amplitude[i][j]=new double[m_Width];
			m_Angle[i][j]=new double[m_Width];
			m_RGB[i][j]=new unsigned char[m_Width];
			m_RGBtmp[i][j]=new double[m_Width];
		}
	}

	TurnComplexToAmplitudeAndAngle();

	SetWindowPos(0,0,0,m_Width+160+16+20,max(392,m_Height+32+16+20),SWP_NOZORDER);
	CenterWindow();
	return 0;
}

void CFFTDlg::TurnComplexToAmplitudeAndAngle()
{
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				m_Amplitude[i][j][k]=sqrt(m_Complex[i][j*m_Width+k].re*m_Complex[i][j*m_Width+k].re+m_Complex[i][j*m_Width+k].im*m_Complex[i][j*m_Width+k].im);
				m_Angle[i][j][k]=atan(m_Complex[i][j*m_Width+k].im/m_Complex[i][j*m_Width+k].re);
			}
		}
	}
}

void CFFTDlg::TurnAmplitudeAndAngleToComplex()
{
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				m_Complex[i][j*m_Width+k].re=m_Amplitude[i][j][k]*cos(m_Angle[i][j][k]);
				m_Complex[i][j*m_Width+k].im=m_Amplitude[i][j][k]*sin(m_Angle[i][j][k]);
			}
		}
	}
}

void CFFTDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC BmpDc;
	CPen GreenPen(PS_SOLID,1,RGB(0,255,0));
	CPen BluePen(PS_SOLID,1,RGB(0,0,255));
	BmpDc.CreateCompatibleDC(&dc);
	UpdateData();

	CBitmap Bmp;
	unsigned char *Buf=GetBmpBufTrans();
	Bmp.CreateBitmap(m_Width,m_Height,1,32,Buf);
	delete(Buf);
	BmpDc.SelectObject(Bmp);
	dc.BitBlt(160,20,m_Width,m_Height,&BmpDc,0,0,SRCCOPY);

	dc.SelectStockObject(NULL_BRUSH);
	if(m_Param1>0&&m_Param1&&m_Filter!=_T("无"))
	{
		dc.SelectObject(GreenPen);
		int mid_x=160+m_Width/2;
		int mid_y=20+m_Height/2;
		dc.Ellipse(mid_x-m_Param1,mid_y-m_Param1,mid_x+m_Param1,mid_y+m_Param1);
	}
	if(m_Param2>0&&(m_Filter==_T("带通")||m_Filter==_T("带阻")))
	{
		dc.SelectObject(BluePen);
		int mid_x=160+m_Width/2;
		int mid_y=20+m_Height/2;
		dc.Ellipse(mid_x-m_Param2,mid_y-m_Param2,mid_x+m_Param2,mid_y+m_Param2);
	}

	GreenPen.DeleteObject();
	BluePen.DeleteObject();
	ReleaseDC(&BmpDc);
	UpdateData(0);
}

unsigned char* CFFTDlg::GetBmpBufTrans()//返回转换为显示图片需要的排列方式的指针
{
	int i,j,k;
	double big=0;
	double ***Target;

	if(!m_ShowMode)
	{
		Target=m_Amplitude;
	}
	else
	{
		Target=m_Angle;
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				if(!m_ShowMode)
				{
					m_RGBtmp[i][j][k]=log10(Target[i][j][k]);
				}
				else
				{
					m_RGBtmp[i][j][k]=Target[i][j][k]+3.14159265359;
				}
				big<m_RGBtmp[i][j][k]?big=m_RGBtmp[i][j][k]:NULL;
			}
		}
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				m_RGB[i][j][k]=static_cast<unsigned char>(m_RGBtmp[i][j][k]*255/big);
			}
		}
	}

	int tmpRGB;
	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height/2;j++)
		{
			for(k=0;k<m_Width/2;k++)
			{
				tmpRGB=m_RGB[i][j][k];
				m_RGB[i][j][k]=m_RGB[i][j+m_Height/2][k+m_Width/2];
				m_RGB[i][j+m_Height/2][k+m_Width/2]=tmpRGB;

				tmpRGB=m_RGB[i][j+m_Height/2][k];
				m_RGB[i][j+m_Height/2][k]=m_RGB[i][j][k+m_Width/2];
				m_RGB[i][j][k+m_Width/2]=tmpRGB;
			}
		}
	}

	long n=m_Width*m_Height*4;
	unsigned char* Buf=new unsigned char[n];

	if(m_Color==1)
	{
		for(i=0;i<n;i+=4)
		{
			*(Buf+i)=m_RGB[0][i/4/m_Width][i/4%m_Width];
			*(Buf+i+1)=*(Buf+i);
			*(Buf+i+2)=*(Buf+i+1);
		}
	}
	else if(m_Color==3)
	{
		for(i=0;i<n;i+=4)
		{
			*(Buf+i)=m_RGB[2][i/4/m_Width][i/4%m_Width];
			*(Buf+i+1)=m_RGB[1][i/4/m_Width][i/4%m_Width];
			*(Buf+i+2)=m_RGB[0][i/4/m_Width][i/4%m_Width];
		}
	}
	else if(m_Color==4)
	{
		for(i=0;i<n;i+=4)
		{
			*(Buf+i)=m_RGB[3][i/4/m_Width][i/4%m_Width];
			*(Buf+i+1)=m_RGB[2][i/4/m_Width][i/4%m_Width];
			*(Buf+i+2)=m_RGB[1][i/4/m_Width][i/4%m_Width];
			*(Buf+i+3)=m_RGB[0][i/4/m_Width][i/4%m_Width];
		}
	}

	return Buf;
}

void CFFTDlg::OnBnClickedAmplitude()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CalculateFilter();
	Invalidate(0);
	
}


void CFFTDlg::OnBnClickedAngle()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CalculateFilter();
	Invalidate(0);
}


void CFFTDlg::OnEnChangeParam1()
{
	LPTSTR str=new TCHAR[10];
	m_Param1_Con.GetWindowText(str,10);
	if(str[0]==0)
	{
		return;
	}
	m_Param1=StrToIntA(str);
	if(m_Param1>min(m_Height/2,m_Width/2))
	{
		m_Param1=min(m_Height/2,m_Width/2);
	}
	UpdateData(0);
	CalculateFilter();
	Invalidate(0);
}


void CFFTDlg::OnEnChangeParam2()
{
	LPTSTR str=new TCHAR[10];
	m_Param2_Con.GetWindowText(str,10);
	if(str[0]==0)
	{
		return;
	}
	m_Param2=StrToIntA(str);
	if(m_Param2>min(m_Height/2,m_Width/2))
	{
		m_Param2=min(m_Height/2,m_Width/2);
	}
	UpdateData(0);
	CalculateFilter();
	Invalidate(0);
}

void CFFTDlg::CalculateFilter()
{
	double ***Target;
	int d1,d2,d3,d4;
	TurnComplexToAmplitudeAndAngle();

	if(!m_ShowMode)
	{
		Target=m_Amplitude;
	}
	else
	{
		Target=m_Angle;
	}

	if(m_Filter==_T("低通"))
	{
		for(int i=0;i<m_Color;i++)
		{
			for(int j=0;j<m_Height;j++)
			{
				for(int k=0;k<m_Width;k++)
				{
					d1=static_cast<int>(sqrt(double(j*j+k*k)));
					d2=static_cast<int>(sqrt(double((j-m_Height)*(j-m_Height)+k*k)));
					d3=static_cast<int>(sqrt(double(j*j+(k-m_Width)*(k-m_Width))));
					d4=static_cast<int>(sqrt(double((j-m_Height)*(j-m_Height)+(k-m_Width)*(k-m_Width))));
					if(d1>m_Param1&&d2>m_Param1&&d3>m_Param1&&d4>m_Param1)
					{
						Target[i][j][k]/=5;
					}
				}
			}
		}
	}
	else if(m_Filter==_T("高通"))
	{
		for(int i=0;i<m_Color;i++)
		{
			for(int j=0;j<m_Height;j++)
			{
				for(int k=0;k<m_Width;k++)
				{
					d1=static_cast<int>(sqrt(double(j*j+k*k)));
					d2=static_cast<int>(sqrt(double((j-m_Height)*(j-m_Height)+k*k)));
					d3=static_cast<int>(sqrt(double(j*j+(k-m_Width)*(k-m_Width))));
					d4=static_cast<int>(sqrt(double((j-m_Height)*(j-m_Height)+(k-m_Width)*(k-m_Width))));
					if(d1<m_Param1||d2<m_Param1||d3<m_Param1||d4<m_Param1)
					{
						Target[i][j][k]/=5;
					}
				}
			}
		}
	}
	else if(m_Filter==_T("带阻"))
	{
		if(m_Param1>m_Param2)
		{
			int tmp=m_Param1;
			m_Param1=m_Param2;
			m_Param2=tmp;
		}
		for(int i=0;i<m_Color;i++)
		{
			for(int j=0;j<m_Height;j++)
			{
				for(int k=0;k<m_Width;k++)
				{
					d1=static_cast<int>(sqrt(double(j*j+k*k)));
					d2=static_cast<int>(sqrt(double((j-m_Height)*(j-m_Height)+k*k)));
					d3=static_cast<int>(sqrt(double(j*j+(k-m_Width)*(k-m_Width))));
					d4=static_cast<int>(sqrt(double((j-m_Height)*(j-m_Height)+(k-m_Width)*(k-m_Width))));
					if(d1>m_Param1&&d2>m_Param1&&d3>m_Param1&&d4>m_Param1)
					{
						if(d1<m_Param2||d2<m_Param2||d3<m_Param2||d4<m_Param2)
						{
							Target[i][j][k]/=5;
						}
					}
				}
			}
		}
	}
	else if(m_Filter==_T("带通"))
	{
		if(m_Param1>m_Param2)
		{
			int tmp=m_Param1;
			m_Param1=m_Param2;
			m_Param2=tmp;
		}
		for(int i=0;i<m_Color;i++)
		{
			for(int j=0;j<m_Height;j++)
			{
				for(int k=0;k<m_Width;k++)
				{
					d1=static_cast<int>(sqrt(double(j*j+k*k)));
					d2=static_cast<int>(sqrt(double((j-m_Height)*(j-m_Height)+k*k)));
					d3=static_cast<int>(sqrt(double(j*j+(k-m_Width)*(k-m_Width))));
					d4=static_cast<int>(sqrt(double((j-m_Height)*(j-m_Height)+(k-m_Width)*(k-m_Width))));
					if(d1>m_Param2&&d2>m_Param2&&d3>m_Param2&&d4>m_Param2)
					{
						Target[i][j][k]/=5;
					}
					else if(d1<m_Param1||d2<m_Param1||d3<m_Param1||d4<m_Param1)
					{
						Target[i][j][k]/=5;
					}
				}
			}
		}
	}
}

void CFFTDlg::OnCbnSelchangeFilter()
{
	// TODO: 在此添加控件通知处理程序代码
	LPTSTR str=new TCHAR[10];
	m_Param1_Con.GetWindowText(str,10);
	if(str[0]==0)
	{
		return;
	}
	m_Param2_Con.GetWindowText(str,10);
	if(str[0]==0)
	{
		return;
	}
	UpdateData();
	CalculateFilter();
	Invalidate(0);
}


void CFFTDlg::OnBnClickedFftcancel()
{
	// TODO: 在此添加控件通知处理程序代码
	GetParent()->SendMessage(WM_FFT_CANCEL);
	CDialogEx::OnCancel();
}


void CFFTDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetParent()->SendMessage(WM_FFT_CANCEL);
	CDialogEx::OnOK();
}
