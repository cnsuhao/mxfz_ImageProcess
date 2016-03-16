// CurveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "CurveDlg.h"
#include "afxdialogex.h"
#include "MfcFun.h"

// CCurveDlg �Ի���

IMPLEMENT_DYNAMIC(CCurveDlg, CDialogEx)

CCurveDlg::CCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCurveDlg::IDD, pParent)
	, m_Color(3)
	, m_ColorSelect(_T(""))
	, m_X(0)
	, m_Y(0)
	, m_d_X(0)
	, m_d_Y(0)
{
	m_DrawRect.left=20;
	m_DrawRect.top=40;
	m_DrawRect.right=m_DrawRect.left+256;
	m_DrawRect.bottom=m_DrawRect.top+256;
	
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<256;j++)
		{
			m_Curve[i][j]=j;
		}
		for(int j=0;j<10;j++)
		{
			m_PointSet[i][j].SetPoint(0,0);
		}
	}
	m_ColorSelect=_T("RGB");
}

CCurveDlg::~CCurveDlg()
{
}

void CCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_TOOL_IMAGEPROCESS_COLOR_SELECT, m_ColorSelect);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_MOUSE_X, m_X);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_MOUSE_Y, m_Y);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_X, m_d_X);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_Y, m_d_Y);
}


BEGIN_MESSAGE_MAP(CCurveDlg, CDialogEx)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_TOOL_IMAGEPROCESS_COLOR_SELECT, &CCurveDlg::OnCbnColorSelect)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDOK, &CCurveDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_CANCEL_CURVE, &CCurveDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_BTN_ADD_POINT, &CCurveDlg::OnBnClickedBtnAddPoint)
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_BTN_CLEAR, &CCurveDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CCurveDlg ��Ϣ�������


void CCurveDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC cdc;
	CBitmap Bmp;
	cdc.CreateCompatibleDC(&dc);
	Bmp.CreateCompatibleBitmap(&dc,m_DrawRect.Width(),m_DrawRect.Height());
	cdc.SelectObject(Bmp);

	CPen BlackPen(PS_SOLID,1,RGB(0,0,0)),RedPen(PS_SOLID,1,RGB(255,0,0)),GreenPen(PS_SOLID,1,RGB(0,255,0)),BluePen(PS_SOLID,1,RGB(0,0,255));
	cdc.SelectObject(BlackPen);
	cdc.Rectangle(0,0,m_DrawRect.Width(),m_DrawRect.Height());

	if(m_ColorSelect==_T("��")||m_ColorSelect==_T("RGB"))
	{
		cdc.MoveTo(0,255);
		cdc.SelectObject(RedPen);
		for(int i=0;i<256;i++)
		{
			cdc.LineTo(i,255-m_Curve[1][i]);
		}
		for(int i=0;i<10;i++)
		{
			if(m_PointSet[1][i].x)
			{
				cdc.Ellipse(m_PointSet[1][i].x-1,255-m_PointSet[1][i].y-1,m_PointSet[1][i].x+3,255-m_PointSet[1][i].y+3);
			}
		}
	}
	if(m_ColorSelect==_T("��")||m_ColorSelect==_T("RGB"))
	{
		cdc.MoveTo(0,255);
		cdc.SelectObject(GreenPen);
		for(int i=0;i<256;i++)
		{
			cdc.LineTo(i,255-m_Curve[2][i]);
		}
		for(int i=0;i<10;i++)
		{
			if(m_PointSet[2][i].x)
			{
				cdc.Ellipse(m_PointSet[2][i].x-1,255-m_PointSet[2][i].y-1,m_PointSet[2][i].x+3,255-m_PointSet[2][i].y+3);
			}
		}
	}
	if(m_ColorSelect==_T("��")||m_ColorSelect==_T("RGB"))
	{
		cdc.MoveTo(0,255);
		cdc.SelectObject(BluePen);
		for(int i=0;i<256;i++)
		{
			cdc.LineTo(i,255-m_Curve[3][i]);
		}
		for(int i=0;i<10;i++)
		{
			if(m_PointSet[3][i].x)
			{
				cdc.Ellipse(m_PointSet[3][i].x-1,255-m_PointSet[3][i].y-1,m_PointSet[3][i].x+3,255-m_PointSet[3][i].y+3);
			}
		}
	}
	if(m_ColorSelect==_T("RGB")||m_ColorSelect==_T("�Ҷ�"))
	{
		cdc.MoveTo(0,255);
		cdc.SelectObject(BlackPen);
		for(int i=0;i<256;i++)
		{
			cdc.LineTo(i,255-m_Curve[0][i]);
		}
		for(int i=0;i<10;i++)
		{
			if(m_PointSet[0][i].x)
			{
				cdc.Ellipse(m_PointSet[0][i].x-1,255-m_PointSet[0][i].y-1,m_PointSet[0][i].x+3,255-m_PointSet[0][i].y+3);
			}
		}
	}

	dc.BitBlt(m_DrawRect.left,m_DrawRect.top,m_DrawRect.Width(),m_DrawRect.Height(),&cdc,0,0,SRCCOPY);
	cdc.DeleteDC();
		// TODO: �ڴ˴������Ϣ����������
		// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}

void CCurveDlg::SetColorCount(int Color)
{
	m_Color=Color;
}

void CCurveDlg::OnCbnColorSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if(m_Color==1&&m_ColorSelect!=_T("�Ҷ�"))
	{
		m_ColorSelect=_T("�Ҷ�");
	}
	else if(m_Color!=1&&m_ColorSelect==_T("�Ҷ�"))
	{
		m_ColorSelect=_T("RGB");
	}
	InvalidateRect(m_DrawRect,0);
	UpdateData(0);

	int WhatColor=0;//0:�Ҷ� 1:�� 2:�� 3:�� 4:ȫ��
	if(m_ColorSelect==_T("��"))
	{
		WhatColor=1;
	}
	else if(m_ColorSelect==_T("��"))
	{
		WhatColor=2;
	}
	else if(m_ColorSelect==_T("��"))
	{
		WhatColor=3;
	}
	else if(m_ColorSelect==_T("RGB"))
	{
		WhatColor=4;
	}
	GetParent()->PostMessage(WM_CURVE_OK,m_Color,WhatColor);
}


int CCurveDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	if(m_Color==1)
	{
		m_ColorSelect=_T("�Ҷ�");
	}
		return 0;
}


void CCurveDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UpdateData();
	if(PtInRect(m_DrawRect,point))
	{
		m_X=point.x-m_DrawRect.left;
		m_Y=m_DrawRect.bottom-point.y-1;
	}
	UpdateData(0);
	CDialogEx::OnMouseMove(nFlags, point);
}


void CCurveDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!PtInRect(m_DrawRect,point))
	{
		return;
	}

	UpdateData();
	if(m_Color==1&&m_ColorSelect!=_T("�Ҷ�"))
	{
		m_ColorSelect=_T("�Ҷ�");
		InvalidateRect(m_DrawRect,0);
	}
	else if(m_Color!=1&&m_ColorSelect==_T("�Ҷ�"))
	{
		m_ColorSelect=_T("RGB");
		InvalidateRect(m_DrawRect,0);
	}
	UpdateData(0);

	m_X=point.x-m_DrawRect.left;
	m_Y=m_DrawRect.bottom-point.y-1;
	
	if(m_ColorSelect==_T("RGB")||m_ColorSelect==_T("�Ҷ�"))
	{
		for(int i=0;i<10;i++)
		{
			if(PtInRect(CRect(m_PointSet[0][i].x-3,m_PointSet[0][i].y-3,m_PointSet[0][i].x+3,m_PointSet[0][i].y+3),CPoint(m_X,m_Y)))
			{
				m_PointSet[0][i]=CPoint(0,0);
				break;
			}
			if(m_PointSet[0][i]==CPoint(0,0)||m_PointSet[0][i].x==m_X)
			{
				m_PointSet[0][i]=CPoint(m_X,m_Y);
				break;
			}
			if(i==9)
			{
				MessageBox(_T("���������֧��10���㣡"),_T("�Բ���"));
			}
		}
	}

	if(m_ColorSelect==_T("��"))
	{
		for(int i=0;i<10;i++)
		{
			if(PtInRect(CRect(m_PointSet[1][i].x-3,m_PointSet[1][i].y-3,m_PointSet[1][i].x+3,m_PointSet[1][i].y+3),CPoint(m_X,m_Y)))
			{
				m_PointSet[1][i]=CPoint(0,0);
				break;
			}
			if(m_PointSet[1][i]==CPoint(0,0)||m_PointSet[1][i].x==m_X)
			{
				m_PointSet[1][i]=CPoint(m_X,m_Y);
				break;
			}
			if(i==9)
			{
				MessageBox(_T("���������֧��10���㣡"),_T("�Բ���"));
			}
		}
	}

	if(m_ColorSelect==_T("��"))
	{
		for(int i=0;i<10;i++)
		{
			if(PtInRect(CRect(m_PointSet[2][i].x-3,m_PointSet[2][i].y-3,m_PointSet[2][i].x+3,m_PointSet[2][i].y+3),CPoint(m_X,m_Y)))
			{
				m_PointSet[2][i]=CPoint(0,0);
				break;
			}
			if(m_PointSet[2][i]==CPoint(0,0)||m_PointSet[2][i].x==m_X)
			{
				m_PointSet[2][i]=CPoint(m_X,m_Y);
				break;
			}
			if(i==9)
			{
				MessageBox(_T("���������֧��10���㣡"),_T("�Բ���"));
			}
		}
	}

	if(m_ColorSelect==_T("��"))
	{
		for(int i=0;i<10;i++)
		{
			if(PtInRect(CRect(m_PointSet[3][i].x-3,m_PointSet[3][i].y-3,m_PointSet[3][i].x+3,m_PointSet[3][i].y+3),CPoint(m_X,m_Y)))
			{
				m_PointSet[3][i]=CPoint(0,0);
				break;
			}
			if(m_PointSet[3][i]==CPoint(0,0)||m_PointSet[3][i].x==m_X)
			{
				m_PointSet[3][i]=CPoint(m_X,m_Y);
				break;
			}
			if(i==9)
			{
				MessageBox(_T("���������֧��10���㣡"),_T("�Բ���"));
			}
		}
	}

	for(int i=0;i<4;i++)
	{
		for(int j=0;j<10;j++)
		{
			for(int k=j+1;k<10;k++)
			{
				if(m_PointSet[i][j].x<m_PointSet[i][k].x)
				{
					CPoint TmpPt=m_PointSet[i][j];
					m_PointSet[i][j]=m_PointSet[i][k];
					m_PointSet[i][k]=TmpPt;
				}
			}
		}
	}
	CalculateCurve();
	InvalidateRect(m_DrawRect,0);

	int WhatColor=0;//0:�Ҷ� 1:�� 2:�� 3:�� 4:ȫ��
	if(m_ColorSelect==_T("��"))
	{
		WhatColor=1;
	}
	else if(m_ColorSelect==_T("��"))
	{
		WhatColor=2;
	}
	else if(m_ColorSelect==_T("��"))
	{
		WhatColor=3;
	}
	else if(m_ColorSelect==_T("RGB"))
	{
		WhatColor=4;
	}
	GetParent()->PostMessage(WM_CURVE_OK,m_Color,WhatColor);
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCurveDlg::CalculateCurve()
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(!m_PointSet[i][j].x)
			{
				if(m_PointSet[i][j].y)
				{
					if(j>0)
					{
						if(!m_PointSet[i][j-1].x)
						{
							CPoint TmpPt=m_PointSet[i][j];
							m_PointSet[i][j]=m_PointSet[i][j-1];
							m_PointSet[i][j-1]=TmpPt;
						}
					}
				}
			}
		}
	}

	

	for(int i=0;i<4;i++)
	{
		for(int j=9;j>=0;j--)
		{
			if(m_PointSet[i][j]!=CPoint(0,0))
			{
				if(j==9)
				{
					for(int k=0;k!=m_PointSet[i][j].x;k++)
					{
						m_Curve[i][k]=static_cast<int>(k*double(m_PointSet[i][j].y)/m_PointSet[i][j].x);
					}
				}
				else
				{
					for(int k=m_PointSet[i][j+1].x;k!=m_PointSet[i][j].x;k++)
					{
						m_Curve[i][k]=static_cast<int>((k-m_PointSet[i][j+1].x)*double(m_PointSet[i][j].y-m_PointSet[i][j+1].y)/(m_PointSet[i][j].x-m_PointSet[i][j+1].x)+m_PointSet[i][j+1].y);
					}
				}
			}
		}

		if(m_PointSet[i][0]!=CPoint(0,0))
		{
			for(int k=m_PointSet[i][0].x;k<256;k++)
			{
				m_Curve[i][k]=static_cast<int>((k-m_PointSet[i][0].x)*double(255-m_PointSet[i][0].y)/(255-m_PointSet[i][0].x)+m_PointSet[i][0].y);
			}
			if(m_PointSet[i][0].x==255)
			{
				m_Curve[i][255]=m_PointSet[i][0].y;
			}
		}
		else
		{
			for(int k=0;k<256;k++)
			{
				m_Curve[i][k]=k;
			}
		}
	}
}

int CCurveDlg::GetCurveYfromX(int color,int x)
{
	return m_Curve[color][x];
}

void CCurveDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int WhatColor=0;//0:�Ҷ� 1:�� 2:�� 3:�� 4:ȫ��
	if(m_ColorSelect==_T("��"))
	{
		WhatColor=1;
	}
	else if(m_ColorSelect==_T("��"))
	{
		WhatColor=2;
	}
	else if(m_ColorSelect==_T("��"))
	{
		WhatColor=3;
	}
	else if(m_ColorSelect==_T("RGB"))
	{
		WhatColor=4;
	}
	GetParent()->PostMessage(WM_CURVE_CERTAIN,m_Color,WhatColor);
	CDialogEx::OnOK();
}


void CCurveDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetParent()->PostMessage(WM_CURVE_CANCEL,NULL,NULL);
	CDialogEx::OnCancel();
}

void CCurveDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetParent()->PostMessage(WM_CURVE_CANCEL,NULL,NULL);
	CDialogEx::OnClose();
}


void CCurveDlg::OnBnClickedBtnAddPoint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	m_d_X=max(min(255,m_d_X),0);
	m_d_Y=max(min(255,m_d_Y),0);
	UpdateData(0);
	OnLButtonUp(0,CPoint(m_DrawRect.left+m_d_X,m_DrawRect.bottom-m_d_Y-1));
}


void CCurveDlg::OnBnClickedBtnClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<256;j++)
		{
			m_Curve[i][j]=j;
		}
		for(int j=0;j<10;j++)
		{
			m_PointSet[i][j].SetPoint(0,0);
		}
	}
	InvalidateRect(m_DrawRect,0);
	GetParent()->PostMessage(WM_CURVE_CANCEL,NULL,NULL);
}

BOOL CCurveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CComboBox* pCComboBox = NULL;
	long lCursor = 0x00;
	pCComboBox = (CComboBox*)GetDlgItem(IDC_TOOL_IMAGEPROCESS_COLOR_SELECT);
	pCComboBox->InsertString(lCursor++, _T("RGB"));
	pCComboBox->InsertString(lCursor++, _T("��"));
	pCComboBox->InsertString(lCursor++, _T("��"));
	pCComboBox->InsertString(lCursor++, _T("��"));
	pCComboBox->InsertString(lCursor++, _T("�Ҷ�"));
	set_DropDownSize(*pCComboBox,lCursor++);// �ڶ������������߶�����ʾ����
	pCComboBox->SetCurSel(0x00);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
