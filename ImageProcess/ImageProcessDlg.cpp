
// ImageProcessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_DIALOG_TOOL_IMAGEPROCESS_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedUrlAboutboxLink();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedUrlAboutboxBloglink();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_STN_CLICKED(IDC_TOOL_IMAGEPROCESS_URL_ABOUTBOX_LINK, &CAboutDlg::OnStnClickedUrlAboutboxLink)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_TOOL_IMAGEPROCESS_URL_ABOUTBOX_BLOGLINK, &CAboutDlg::OnStnClickedUrlAboutboxBloglink)
END_MESSAGE_MAP()

void CAboutDlg::OnStnClickedUrlAboutboxLink()
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,NULL,"http://www.mxfz.net",NULL,NULL,0);
}

void CAboutDlg::OnStnClickedUrlAboutboxBloglink()
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,NULL,"http://mxfz.net/cxtlife",NULL,NULL,0);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
#ifndef IDC_HAND         
	MAKEINTRESOURCE(32649)
#endif
		::SetClassLong(GetDlgItem(IDC_TOOL_IMAGEPROCESS_URL_ABOUTBOX_LINK)->m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_HAND));
	::SetClassLong(GetDlgItem(IDC_TOOL_IMAGEPROCESS_URL_ABOUTBOX_BLOGLINK)->m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_HAND));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CAboutDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	::SetClassLong(GetDlgItem(IDC_TOOL_IMAGEPROCESS_URL_ABOUTBOX_LINK)->m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_ARROW));
	::SetClassLong(GetDlgItem(IDC_TOOL_IMAGEPROCESS_URL_ABOUTBOX_BLOGLINK)->m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_ARROW));
}


HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID()==IDC_TOOL_IMAGEPROCESS_URL_ABOUTBOX_LINK
		|| pWnd->GetDlgCtrlID()==IDC_TOOL_IMAGEPROCESS_URL_ABOUTBOX_BLOGLINK)
		pDC->SetTextColor(RGB(0,0,255));

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


// CImageProcessDlg �Ի���



CImageProcessDlg::CImageProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageProcessDlg::IDD, pParent)
	, m_FileName(_T(""))
	, m_bBusy(false)
	, m_bOpenDlg(true)
	, m_bSmoothDlgOpened(false)
	, m_bFilterDlgOpened(false)
	, m_bCurveDlgOpened(false)
	, m_bHistogramDlgOpened(false)
	, m_bFT(false)
	, m_bFFTDlgOpened(false)
	, m_bMorphologyOpened(false)
	, m_bIsJpg(false)
	, m_bIsCutting(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pt_lu.SetPoint(0,0);
	m_pt_rd.SetPoint(0,0);
}

void CImageProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageProcessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_FILE_OPEN, &CImageProcessDlg::OnBnClickedFileOpen)
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_SAVE_FILE, &CImageProcessDlg::OnBnClickedSaveFile)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_SAVE_TEXT, &CImageProcessDlg::OnBnClickedSaveText)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_FILE_OPEN, &CImageProcessDlg::OnPopupFileOpen)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_FILE_SAVE, &CImageProcessDlg::OnPopupFileSave)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_FILE_SAVE_TEXT, &CImageProcessDlg::OnPopupFileSaveText)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_FILE_INFO, &CImageProcessDlg::OnPopupFileInfo)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_REVERSE_COLOR, &CImageProcessDlg::OnPopupReverseColor)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_REVERSE_LR, &CImageProcessDlg::OnPopupReverseLr)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_REVERSE_UD, &CImageProcessDlg::OnPopupReverseUd)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_REVERSE_180, &CImageProcessDlg::OnPopupReverse180)
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_FILE_INFO, &CImageProcessDlg::OnBnClickedFileInfo)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_33_SMOOTH, &CImageProcessDlg::OnPopup33Smooth)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_55_SMOOTH, &CImageProcessDlg::OnPopup55Smooth)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_AVG_SMOOTH, &CImageProcessDlg::OnPopupAvgSmooth)
	ON_MESSAGE(WM_SMOOTH_OK, &CImageProcessDlg::OnSmoothOk)
	ON_MESSAGE(WM_CANCEL_SMOOTH, &CImageProcessDlg::OnCancelSmooth)
	ON_MESSAGE(WM_SMOOTH_CERTAIN, &CImageProcessDlg::OnSmoothCertain)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_33_MEDIAN, &CImageProcessDlg::OnPopup33Median)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_55_MEDIAN, &CImageProcessDlg::OnPopup55Median)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_MEDIAN, &CImageProcessDlg::OnPopupMedian)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_FILTER_DEFINE, &CImageProcessDlg::OnFilterDefine)
	ON_MESSAGE(WM_FILTER_DEFINE_OK, &CImageProcessDlg::OnFilterDefineOk)
	ON_MESSAGE(WM_FILTER_DEFINE_CANCEL, &CImageProcessDlg::OnFilterDefineCancel)
	ON_MESSAGE(WM_FILTER_DEFINE_CERTAIN, &CImageProcessDlg::OnFilterDefineCertain)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_CURVE, &CImageProcessDlg::OnPopupCurve)
	ON_MESSAGE(WM_CURVE_OK, &CImageProcessDlg::OnCurveOk)
	ON_MESSAGE(WM_CURVE_CERTAIN, &CImageProcessDlg::OnCurveCertain)
	ON_MESSAGE(WM_CURVE_CANCEL, &CImageProcessDlg::OnCurveCancel)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_HISTOGRAM_EQUALIZATION, &CImageProcessDlg::OnHistogramEqualization)
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_BTN_RELOAD, &CImageProcessDlg::OnBnClickedBtnReload)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_RELOAD, &CImageProcessDlg::OnPopupReload)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_THRESHOLD, &CImageProcessDlg::OnPopupThreshold)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_LPLS_4, &CImageProcessDlg::OnPopupLpls4)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_LPLS_8, &CImageProcessDlg::OnPopupLpls8)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_HISTOGRAM, &CImageProcessDlg::OnHistogram)
	ON_MESSAGE(WM_HISTOGRAM_OK, &CImageProcessDlg::OnHistogramOk)
	ON_MESSAGE(WM_HISTOGRAM_CERTIAN, &CImageProcessDlg::OnHistogramCertian)
	ON_MESSAGE(WM_HISTOGRAM_CANCEL, &CImageProcessDlg::OnHistogramCancel)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_RGB_2_GRAY, &CImageProcessDlg::OnPopupRgb2Gray)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_ZOOM, &CImageProcessDlg::OnPopupZoom)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_DFT, &CImageProcessDlg::OnPopupDft)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_FFT, &CImageProcessDlg::OnPopupFft)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_IFFT, &CImageProcessDlg::OnPopupIfft)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_IDFT, &CImageProcessDlg::OnPopupIdft)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_FFT_FILTER, &CImageProcessDlg::OnFftFilter)
	ON_MESSAGE(WM_FFT_CANCEL, &CImageProcessDlg::OnFftCancel)
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_BTN_CONNECT_ME, &CImageProcessDlg::OnBnClickedBtnConnectMe)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_MORPHOLOGY, &CImageProcessDlg::OnPopupMorphology)
	ON_MESSAGE(WM_MORPHOLOGY_OK, &CImageProcessDlg::OnMorphologyOk)
	ON_MESSAGE(WM_MORPHOLOGY_CERTAIN, &CImageProcessDlg::OnMorphologyCertain)
	ON_MESSAGE(WM_MORPHOLOGY_CANCEL, &CImageProcessDlg::OnMorphologyCancel)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_GETBOARD, &CImageProcessDlg::OnPopupGetboard)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_MASK, &CImageProcessDlg::OnPopupMask)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_THRESHOLD_EX, &CImageProcessDlg::OnPopupThresholdEx)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_CUT, &CImageProcessDlg::OnPopupCut)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_CUT_INFO, &CImageProcessDlg::OnPopupCutInfo)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_BILATERAL, &CImageProcessDlg::OnPopupBilateral)
	ON_COMMAND(ID_TOOL_IMAGEPROCESS_POPUP_GAUSS, &CImageProcessDlg::OnPopupGauss)
END_MESSAGE_MAP()


// CImageProcessDlg ��Ϣ�������

BOOL CImageProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	SetWindowPos(0,0,0,315,100,SWP_NOMOVE|SWP_NOZORDER);
	ShowWindow(SW_SHOWNORMAL);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	UpdateData(0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CImageProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CImageProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if(m_Bmp.Exist())
		{
			CPaintDC dc(this);
			CDC MemDc,BmpDc;
			MemDc.CreateCompatibleDC(&dc);
			BmpDc.CreateCompatibleDC(&dc);
			CBitmap CBmp;
			CRect WndRect;
			GetWindowRect(WndRect);
			CBmp.CreateCompatibleBitmap(&dc,WndRect.Width(),WndRect.Height());
			MemDc.SelectObject(CBmp);

			CPen Pen(PS_SOLID,1,RGB(170,170,170));
			CPen *OldPen=MemDc.SelectObject(&Pen);
			MemDc.Rectangle(0,0,m_Bmp.GetWidth()+2,m_Bmp.GetHeight()+2);
			MemDc.SelectObject(OldPen);

			CBitmap Bmp;
			unsigned char *Buf=m_Bmp.GetBmpBufTrans();
			Bmp.CreateBitmap(m_Bmp.GetWidth(),m_Bmp.GetHeight(),1,32,Buf);
			delete(Buf);
			BmpDc.SelectObject(Bmp);
			MemDc.BitBlt(1,1,m_Bmp.GetWidth(),m_Bmp.GetHeight(),&BmpDc,0,0,SRCCOPY);

			CFont cf;
			cf.CreatePointFont(120,_T("����"));

			if(m_bBusy)
			{
				MemDc.SelectObject(cf);
				MemDc.SetBkMode(TRANSPARENT);
				MemDc.DrawText(_T("���ڴ����У����Ժ�......"),CRect(0,0,m_Bmp.GetWidth()+2,m_Bmp.GetHeight()+2),DT_CENTER);
			}

			dc.BitBlt(m_ShowRect.left-1,m_ShowRect.top-1,m_ShowRect.Width()+2,m_ShowRect.Height()+2,&MemDc,0,0,SRCCOPY);

			if(m_bIsCutting)
			{
				dc.SetBkMode(TRANSPARENT);
				CPen CutPen(PS_DOT,1,RGB(255,192,203));
				CBrush NullBrush;
				NullBrush.CreateStockObject(NULL_BRUSH);
				dc.SelectObject(NullBrush);
				dc.SelectObject(CutPen);
				dc.Rectangle(CRect(m_pt_lu,m_pt_rd));
			}

			Bmp.DeleteObject();
			OldPen->DeleteObject();
			Pen.DeleteObject();
			CBmp.DeleteObject();
			ReleaseDC(&MemDc);
			ReleaseDC(&BmpDc);
		}

		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CImageProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CImageProcessDlg::OnBnClickedFileOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_bFFTDlgOpened)
	{
		return;
	}
	if(m_Bmp.Exist())
	{
		if(IDYES!=MessageBox(_T("�㵱ǰ��δ�����ͼƬ������ʧ���Ƿ�����򿪣�"),_T("���ļ�"),4))
		{
			return;
		}
	}

	if(m_bOpenDlg)
	{
		CFileDialog Fdlg(true,_T("*.bmp"),_T("��ѡ��.bmp"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("���������о�ϲ��(*.bmp)|*.bmp|����JpgҲ֧����Ŷ��(*.jpg)|*.jpg|"),NULL,0,TRUE);
		if(IDOK!=Fdlg.DoModal())
		{
			return;
		}
		m_FileName=Fdlg.GetPathName();
	}

	m_bIsJpg=0;
	CString TmpName=m_FileName;
	if(-1==m_FileName.Find(_T(".bmp"),max(0,m_FileName.GetLength()-4))&&-1==m_FileName.Find(_T(".BMP"),max(0,m_FileName.GetLength()-4)))
	{
		if(-1==m_FileName.Find(_T(".jpg"),max(0,m_FileName.GetLength()-4))&&-1==m_FileName.Find(_T(".JPG"),max(0,m_FileName.GetLength()-4)))
		{
		MessageBox(_T("���ң���ֻ��bmp��jpg��ʽлл��"),_T("���ļ�"));
		return;
		}
		else
		{
			CImage img;
			img.Load(m_FileName);
			m_FileName=_T("TMP.BMP");
			if(0!=img.Save(m_FileName))
			{
				MessageBox(_T("��ϵͳ�̸�Ŀ¼��ʹ�ñ����������Ҫ����ԱȨ�ޣ����ƶ�������Ŀ¼����D�̣�ʹ�ã�лл��"));
				return;
			}
			m_bIsJpg=1;
		}
	}
	
	if(m_Bmp.Open(m_FileName.GetBuffer()))
	{
		m_ShowRect.top=130-32-8;
		m_ShowRect.left=15;
		m_ShowRect.bottom=m_ShowRect.top+m_Bmp.GetHeight();
		m_ShowRect.right=m_ShowRect.left+m_Bmp.GetWidth();
		SetWindowPos(0,0,0,max(315,m_Bmp.GetWidth()+2*m_ShowRect.left+16),max(130+40,m_Bmp.GetHeight()+m_ShowRect.top+32+8+8+40),SWP_NOZORDER);
		CenterWindow();
		Invalidate();
		m_bFT=0;
		MessageBox(_T("��ȡ�ɹ�����������Ҽ���ѡ����Ҫ�õ��Ĺ��ܣ�"),_T("���ļ�"));
	}
	else
	{
		MessageBox(_T("��ȡʧ�ܣ�"),_T("���ļ�"));
	}
	if(m_bCurveDlgOpened&&m_Bmp.Exist())
	{
		m_CDlg->SetColorCount(m_Bmp.GetColor());
	}
	if(m_bIsJpg)
	{
		DeleteFile(m_FileName);
	}
	m_FileName=TmpName;
}


void CImageProcessDlg::OnBnClickedSaveFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_Bmp.Exist())
	{
		MessageBox(_T("����δ���κ��ļ���"));
		return;
	}

	CFileDialog Fdlg(false,_T("*.bmp"),_T("��ѡ��.bmp"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("ֻ��ѡ��Bmp��ʽŶ��(*.bmp)|*.bmp|�����������Ҳû��(*.bmp)|*.bmp|"),NULL,0,TRUE);
	if(IDOK!=Fdlg.DoModal())
	{
		return;
	}
	CString TmpName=m_FileName;
	m_FileName=Fdlg.GetPathName();

	if(m_Bmp.Save(m_FileName.GetBuffer()))
	{
		MessageBox(_T("����ɹ���"),_T("�����ļ�"));
	}
	else
	{
		MessageBox(_T("����ʧ�ܣ�"),_T("�����ļ�"));
	}
	m_FileName=TmpName;
}


void CImageProcessDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UpdateData();
	if(DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,0))
	{
		TCHAR Path[MAX_PATH];
		DragQueryFile(hDropInfo,0,Path,MAX_PATH);
		m_FileName=Path;
	}
	UpdateData(0);
	m_bOpenDlg=0;
	OnBnClickedFileOpen();
	m_bOpenDlg=1;
	CDialogEx::OnDropFiles(hDropInfo);
}


void CImageProcessDlg::OnBnClickedSaveText()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(!m_Bmp.Exist())
	{
		MessageBox(_T("����δ���κ��ļ���"));
		return;
	}

	CFileDialog Fdlg(false,NULL,_T("ע�⣺���ɱ����ĵ����ܻ����ֹͣ���е����������ȱ���һ�¡�"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("��β���Bmp��������д�����ְɲ��üӺ�׺���һ�����(*.txt)|*.txt|"),NULL,0,TRUE);
	if(IDOK!=Fdlg.DoModal())
	{
		return;
	}
	CString TmpName=m_FileName;
	m_FileName=Fdlg.GetPathName();

	m_bBusy=1;
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();

	if(m_Bmp.SaveAsTxt(m_FileName.GetBuffer()))
	{
		m_bBusy=0;
		InvalidateRect(m_ShowRect,0);
		UpdateWindow();
		MessageBox(_T("�����ı��ɹ���"),_T("�����ı�"));
	}
	else
	{
		m_bBusy=0;
		InvalidateRect(m_ShowRect,0);
		UpdateWindow();
		MessageBox(_T("�����ı�ʧ�ܣ�"),_T("�����ı�"));
	}
	m_FileName=TmpName;
}


void CImageProcessDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_bIsCutting)
	{
		m_bIsCutting=false;
		m_pt_lu=m_pt_rd=point;
		InvalidateRect(m_ShowRect,0);
		if(nFlags&VK_LBUTTON)
		{
			m_bIsCutting=true;
		}
		SendMessage(WM_SETCURSOR);
		return;
	}

	if(PtInRect(m_ShowRect,point))
	{
		if(m_bFilterDlgOpened)
		{
			return;
		}
		if(m_bSmoothDlgOpened)
		{
			return;
		}
		if(m_bCurveDlgOpened)
		{
			return;
		}
		if(m_bFFTDlgOpened)
		{
			return;
		}
		if(m_bMorphologyOpened)
		{
			return;
		}

		CMenu Menu,*m;
		Menu.LoadMenu(IDR_POPUP_MENU);
		CPoint pot;
		GetCursorPos(&pot);
		m=Menu.GetSubMenu(0);

		CString str;
		int cx=point.x-m_ShowRect.left,cy=point.y-m_ShowRect.top;
		switch (m_Bmp.GetColor())
		{
			case 1: str.Format(_T("[%d,%d]GRAY(%d)"),cx,cy,m_Bmp.GetRGB(0,cy,cx));break;
			case 3: str.Format(_T("[%d,%d]RGB(%d,%d,%d)"),cx,cy,m_Bmp.GetRGB(0,cy,cx),m_Bmp.GetRGB(1,cy,cx),m_Bmp.GetRGB(2,cy,cx));break;
			case 4: str.Format(_T("[%d,%d]RGB(%d,%d,%d)T(%d)"),cx,cy,m_Bmp.GetRGB(1,cy,cx),m_Bmp.GetRGB(2,cy,cx),m_Bmp.GetRGB(3,cy,cx),m_Bmp.GetRGB(0,cy,cx));break;

			default: str=_T("24λͼ");break;
		}
		m->ModifyMenu(ID_TOOL_IMAGEPROCESS_POPUP_POINT_INFO,MF_BYCOMMAND,ID_TOOL_IMAGEPROCESS_POPUP_POINT_INFO,str);
		m->TrackPopupMenu(TPM_LEFTALIGN,pot.x,pot.y,this);
	}
	CDialogEx::OnRButtonUp(nFlags, point);
	
}


void CImageProcessDlg::OnPopupFileOpen()
{
	// TODO: �ڴ���������������
	OnBnClickedFileOpen();

}


void CImageProcessDlg::OnPopupFileSave()
{
	// TODO: �ڴ���������������
	OnBnClickedSaveFile();
}


void CImageProcessDlg::OnPopupFileSaveText()
{
	// TODO: �ڴ���������������
	OnBnClickedSaveText();
}


void CImageProcessDlg::OnPopupFileInfo()
{
	// TODO: �ڴ���������������
	CString str;
	str.Format(_T(
"�ļ����ͣ�%c%c\n\
�ļ���С��%dBytes(%dKB)\n\
\n\
ͼƬλ����%dλ\n\
ͼƬ��ߣ�%d*%d(��*��)\n\
ͼƬ�ߴ磺%d����/��(��)*%d����/��(��)\n\
ͼƬ��С��%dBytes(%dKB)\
"),
		(char)m_Bmp.GetBmpFileHeader().bfType,
		(char)(m_Bmp.GetBmpFileHeader().bfType>>8),
		m_Bmp.GetBmpFileHeader().bfSize,
		m_Bmp.GetBmpFileHeader().bfSize/1024,
		m_Bmp.GetBmpInfoHeader().biBitCount,
		m_Bmp.GetBmpInfoHeader().biWidth,
		m_Bmp.GetBmpInfoHeader().biHeight,
		m_Bmp.GetBmpInfoHeader().biXPelsPerMeter,
		m_Bmp.GetBmpInfoHeader().biYPelsPerMeter,
		m_Bmp.GetBmpInfoHeader().biSizeImage,
		m_Bmp.GetBmpInfoHeader().biSizeImage/1024
		);

	MessageBox(str);
}


void CImageProcessDlg::OnPopupReverseColor()
{
	// TODO: �ڴ���������������
	m_Bmp.ReverseColor();
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnPopupReverseLr()
{
	// TODO: �ڴ���������������
	m_Bmp.ReverseLR();
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnPopupReverseUd()
{
	// TODO: �ڴ���������������
	m_Bmp.ReverseUD();
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnPopupReverse180()
{
	// TODO: �ڴ���������������
	m_Bmp.Reverse180();
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnBnClickedFileInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_Bmp.Exist())
	{
		OnPopupFileInfo();
	}
}


void CImageProcessDlg::OnPopup33Smooth()
{
	// TODO: �ڴ���������������
	m_Bmp.SmoothAverage(3,3);
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnPopup55Smooth()
{
	// TODO: �ڴ���������������
	m_Bmp.SmoothAverage(5,5);
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnPopupAvgSmooth()
{
	// TODO: �ڴ���������������
	if(m_bSmoothDlgOpened)
	{
		MessageBox(_T("ιι�����Ѿ�����һ���Զ���Ի����˰�- -��"),_T("��ע����"));
		return;
	}
	CSmoothDlg *Sdlg=new CSmoothDlg;
	Sdlg->Create(IDD_DIALOG_TOOL_IMAGEPROCESS_SMOOTH_DLG);
	Sdlg->ShowWindow(SW_SHOW);
	m_bSmoothDlgOpened=1;
}


afx_msg LRESULT CImageProcessDlg::OnSmoothOk(WPARAM wParam, LPARAM lParam)
{
	m_bBusy=1;
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	m_Bmp.BackUpRGB();
	m_Bmp.SmoothAverage(wParam,lParam);
	m_bBusy=0;
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	m_Bmp.RestoreRGB();
	return 0;
}


afx_msg LRESULT CImageProcessDlg::OnCancelSmooth(WPARAM wParam, LPARAM lParam)
{
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	return m_bSmoothDlgOpened=0;
}


afx_msg LRESULT CImageProcessDlg::OnSmoothCertain(WPARAM wParam, LPARAM lParam)
{
	m_bBusy=1;
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	m_Bmp.BackUpRGB();
	m_Bmp.SmoothAverage(wParam,lParam);
	m_bBusy=0;
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	return m_bSmoothDlgOpened=0;
}


void CImageProcessDlg::OnPopup33Median()
{
	// TODO: �ڴ���������������
	m_Bmp.SmoothMedian(3,3);
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnPopup55Median()
{
	// TODO: �ڴ���������������
	m_bBusy=1;
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	m_Bmp.SmoothMedian(5,5);
	m_bBusy=0;
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnPopupMedian()
{
	// TODO: �ڴ���������������
	CMedianDlg MDlg;
	if(MDlg.DoModal()==IDOK)
	{
		m_bBusy=1;
		InvalidateRect(m_ShowRect,0);
		UpdateWindow();
		m_Bmp.SmoothMedian(MDlg.m_data_x,MDlg.m_data_y);
		m_bBusy=0;
		InvalidateRect(m_ShowRect,0);
	}
}


void CImageProcessDlg::OnFilterDefine()
{
	// TODO: �ڴ���������������
	if(m_bFilterDlgOpened)
	{
		MessageBox(_T("ιι�����Ѿ�����һ���Զ���Ի����˰�- -��"),_T("��ע����"));
		return;
	}
	m_FDDlg=new CFilterDefineDlg;
	m_FDDlg->Create(IDD_DIALOG_TOOL_IMAGEPROCESS_FILTER_DEFINE_DLG);
	m_FDDlg->ShowWindow(SW_SHOW);
	m_bFilterDlgOpened=1;
}


afx_msg LRESULT CImageProcessDlg::OnFilterDefineOk(WPARAM wParam, LPARAM lParam)
{
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			m_Filter[i][j]=m_FDDlg->m_data[i][j];
		}
	}

	m_Bmp.BackUpRGB();
	m_Bmp.FilterDefine(m_Filter,wParam,lParam);
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	m_Bmp.RestoreRGB();
	return 0;
}


afx_msg LRESULT CImageProcessDlg::OnFilterDefineCancel(WPARAM wParam, LPARAM lParam)
{
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	return m_bFilterDlgOpened=0;
}


afx_msg LRESULT CImageProcessDlg::OnFilterDefineCertain(WPARAM wParam, LPARAM lParam)
{
	m_Bmp.FilterDefine(m_Filter,wParam,lParam);
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	return m_bFilterDlgOpened=0;
}



void CImageProcessDlg::OnPopupCurve()
{
	// TODO: �ڴ���������������
	if(m_bCurveDlgOpened)
	{
		MessageBox(_T("ιι�����Ѿ�����һ���Զ���Ի����˰�- -��"),_T("��ע����"));
		return;
	}
	m_CDlg=new CCurveDlg;
	m_CDlg->SetColorCount(m_Bmp.GetColor());
	m_CDlg->Create(IDD_DIALOG_TOOL_IMAGEPROCESS_CURVE_DLG);
	m_CDlg->ShowWindow(SW_SHOW);
	m_bCurveDlgOpened=1;
}


afx_msg LRESULT CImageProcessDlg::OnCurveOk(WPARAM wParam, LPARAM lParam)
{
	m_Bmp.BackUpRGB();
	OnCurveCertain(wParam,lParam);
	m_Bmp.RestoreRGB();
	return m_bCurveDlgOpened=1;
}


afx_msg LRESULT CImageProcessDlg::OnCurveCertain(WPARAM wParam, LPARAM lParam)
{
	if(!lParam&&wParam==1)
	{
		int curve[256];
		for(int i=0;i<256;i++)
		{
			curve[i]=m_CDlg->GetCurveYfromX(0,i);
		}
		m_Bmp.CurveFunction(0,curve);
	}
	else if(lParam!=4)
	{
		int curve[256],color;
		m_Bmp.GetColor()==3?color=lParam-1:color=lParam;
		for(int i=0;i<256;i++)
		{
			curve[i]=m_CDlg->GetCurveYfromX(lParam,i);
		}
		m_Bmp.CurveFunction(color,curve);
	}
	else
	{
		int curveRGB[256];
		for(int i=0;i<256;i++)
		{
			curveRGB[i]=m_CDlg->GetCurveYfromX(0,i);
		}
		for(int n=1;n<4;n++)
		{
			int curve[256],color;
			m_Bmp.GetColor()==3?color=n-1:color=n;
			for(int i=0;i<256;i++)
			{
				curve[i]=m_CDlg->GetCurveYfromX(n,i);
			}
			m_Bmp.CurveFunction(color,curve);
			m_Bmp.CurveFunction(color,curveRGB);
		}
	}
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	return m_bCurveDlgOpened=0;
}


afx_msg LRESULT CImageProcessDlg::OnCurveCancel(WPARAM wParam, LPARAM lParam)
{
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	return m_bCurveDlgOpened=0;
}


void CImageProcessDlg::OnHistogramEqualization()
{
	// TODO: �ڴ���������������
	m_Bmp.HistogramEqualization();
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnBnClickedBtnReload()
{
	if(m_bFFTDlgOpened)
	{
		return;
	}

	m_bIsJpg=0;
	CString TmpName=m_FileName;
	if(-1==m_FileName.Find(_T(".bmp"),max(0,m_FileName.GetLength()-4))&&-1==m_FileName.Find(_T(".BMP"),max(0,m_FileName.GetLength()-4)))
	{
		if(-1==m_FileName.Find(_T(".jpg"),max(0,m_FileName.GetLength()-4))&&-1==m_FileName.Find(_T(".JPG"),max(0,m_FileName.GetLength()-4)))
		{
		MessageBox(_T("���ң���ֻ��bmp��ʽлл��"),_T("���ļ�"));
		return;
		}
		else
		{
			CImage img;
			img.Load(m_FileName);
			m_FileName=_T("TMP.BMP");
			if(0!=img.Save(m_FileName))
				return;
			m_bIsJpg=1;
		}
	}
	
	if(m_Bmp.Open(m_FileName.GetBuffer()))
	{
		m_ShowRect.top=130-32-8;
		m_ShowRect.left=15;
		m_ShowRect.bottom=m_ShowRect.top+m_Bmp.GetHeight();
		m_ShowRect.right=m_ShowRect.left+m_Bmp.GetWidth();
		SetWindowPos(0,0,0,max(315,m_Bmp.GetWidth()+2*m_ShowRect.left+16),max(130+40,m_Bmp.GetHeight()+m_ShowRect.top+32+8+8+40),SWP_NOZORDER);
		CenterWindow();
		Invalidate();
		m_bFT=0;
	}
	else
	{
		MessageBox(_T("��ȡʧ�ܣ�"),_T("���ļ�"));
	}
	if(m_bCurveDlgOpened&&m_Bmp.Exist())
	{
		m_CDlg->SetColorCount(m_Bmp.GetColor());
	}
	if(m_bIsJpg)
	{
		DeleteFile(m_FileName);
	}
	m_FileName=TmpName;
}

void CImageProcessDlg::OnPopupReload()
{
	// TODO: �ڴ���������������
	OnBnClickedBtnReload();
}


void CImageProcessDlg::OnPopupThreshold()
{
	// TODO: �ڴ���������������
	CThresholdDlg CTDlg;
	if(CTDlg.DoModal()==IDOK)
	{
		m_Bmp.Threshold(CTDlg.GetThreshold());
	}
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnPopupLpls4()
{
	// TODO: �ڴ���������������
	double filter[5][5]={
		0,0,0,0,0,
		0,0,1,0,0,
		0,1,-5,1,0,
		0,0,1,0,0,
		0,0,0,0,0,
	};
	m_Bmp.FilterDefine(filter,1,0);
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnPopupLpls8()
{
	// TODO: �ڴ���������������
	double filter[5][5]={
		0,0,0,0,0,
		0,1,1,1,0,
		0,1,-9,1,0,
		0,1,1,1,0,
		0,0,0,0,0,
	};
	m_Bmp.FilterDefine(filter,1,0);
	InvalidateRect(m_ShowRect,0);
}

void CImageProcessDlg::OnHistogram()
{
	// TODO: �ڴ���������������
	if(m_bHistogramDlgOpened)
	{
		MessageBox(_T("ιι�����Ѿ�����һ���Զ���Ի����˰�- -��"),_T("��ע����"));
		return;
	}
	m_HDlg=new CHistogramDlg;
	m_HDlg->Create(IDD_DIALOG_TOOL_IMAGEPROCESS_HISTOGRAM_DLG);
	m_HDlg->ShowWindow(SW_SHOW);
	m_bHistogramDlgOpened=1;
}

afx_msg LRESULT CImageProcessDlg::OnHistogramOk(WPARAM wParam, LPARAM lParam)
{
	m_Bmp.BackUpRGB();
	OnHistogramCertian(wParam,lParam);
	m_Bmp.RestoreRGB();
	return m_bHistogramDlgOpened=1;
}


afx_msg LRESULT CImageProcessDlg::OnHistogramCertian(WPARAM wParam, LPARAM lParam)
{
	int Color=wParam,Min=lParam/256,Max=lParam%256;
	if((Color==3&&m_Bmp.GetColor()==1)||(Color==0&&m_Bmp.GetColor()==3))
	{
		m_Bmp.Histogram(0,Min,Max);
	}
	else if((Color==0&&m_Bmp.GetColor()==4)||(Color==1&&m_Bmp.GetColor()==3))
	{
		m_Bmp.Histogram(1,Min,Max);
	}
	else if((Color==1&&m_Bmp.GetColor()==4)||(Color==2&&m_Bmp.GetColor()==3))
	{
		m_Bmp.Histogram(2,Min,Max);
	}
	else if(Color==2&&m_Bmp.GetColor()==4)
	{
		m_Bmp.Histogram(3,Min,Max);
	}
	else if(Color==3&&m_Bmp.GetColor()!=1)
	{
		for(int i=m_Bmp.GetColor()-3;i<m_Bmp.GetColor();i++)
		{
			m_Bmp.Histogram(i,Min,Max);
		}
	}
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	return m_bHistogramDlgOpened=0;
}


afx_msg LRESULT CImageProcessDlg::OnHistogramCancel(WPARAM wParam, LPARAM lParam)
{
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	return m_bHistogramDlgOpened=0;
}


void CImageProcessDlg::OnPopupRgb2Gray()
{
	// TODO: �ڴ���������������
	m_Bmp.RGB2Gray();
	if(m_bCurveDlgOpened&&m_Bmp.Exist())
	{
		m_CDlg->SetColorCount(m_Bmp.GetColor());
	}
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnPopupZoom()
{
	// TODO: �ڴ���������������
	if(m_bFT)
	{
		MessageBox(_T("Ƶ��ͼ�������ţ�"));
		return;
	}
	CZoomDlg CZDlg;
	if(CZDlg.DoModal()!=IDOK)
	{
		return;
	}
	m_Bmp.Zoom(CZDlg.GetH(),CZDlg.GetW(),CZDlg.GetMethod());
	m_ShowRect.top=130-32-8;
	m_ShowRect.left=15;
	m_ShowRect.bottom=m_ShowRect.top+m_Bmp.GetHeight();
	m_ShowRect.right=m_ShowRect.left+m_Bmp.GetWidth();
	SetWindowPos(0,0,0,max(315,m_Bmp.GetWidth()+2*m_ShowRect.left+16),max(130+40,m_Bmp.GetHeight()+m_ShowRect.top+32+8+8+40),SWP_NOZORDER);
	CenterWindow();
	Invalidate();
}


void CImageProcessDlg::OnPopupDft()
{
	// TODO: �ڴ���������������
	if(m_Bmp.GetWidth()*m_Bmp.GetHeight()>65536)
	{
		if(IDYES!=MessageBox(_T("ѡ���С256*256���ϵ�ͼƬ��ʹ����ʱ�����������Ƿ���Ҫ�������и���Ҷ�任��"),_T("ע��"),MB_YESNO))
		{
			return;
		}
	}
	m_bBusy=1;
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	m_Bmp.DFT();
	InvalidateRect(m_ShowRect,0);
	m_bFT=1;
	m_bBusy=0;
	InvalidateRect(m_ShowRect,0);
}

void CImageProcessDlg::OnPopupFft()
{
	// TODO: �ڴ���������������
	int H_Is2=m_Bmp.GetHeight()&(m_Bmp.GetHeight()-1);
	int W_Is2=m_Bmp.GetWidth()&(m_Bmp.GetWidth()-1);
	if(H_Is2!=0||W_Is2!=0)
	{
		MessageBox(_T("���п��ٸ���Ҷ�任���뱣֤�����Ϊ2��������"));
		OnPopupZoom();
		return;
	}
	m_Bmp.FFT();
	InvalidateRect(m_ShowRect,0);
	m_bFT=1;
}

void CImageProcessDlg::OnPopupIdft()
{
	// TODO: �ڴ���������������
	OnPopupIfft();
}

void CImageProcessDlg::OnPopupIfft()
{
	// TODO: �ڴ���������������
	if(!m_bFT)
	{
		return;
	}
	int H_Is2=m_Bmp.GetHeight()&(m_Bmp.GetHeight()-1);
	int W_Is2=m_Bmp.GetWidth()&(m_Bmp.GetWidth()-1);
	if(H_Is2!=0||W_Is2!=0)
	{
		MessageBox(_T("���п��ٸ���Ҷ���任���뱣֤�����Ϊ2��������"));
		return;
	}

	m_Bmp.iFFT();
	InvalidateRect(m_ShowRect,0);
	m_bFT=0;
}

void CImageProcessDlg::OnFftFilter()
{
	// TODO: �ڴ���������������
	int H_Is2=m_Bmp.GetHeight()&(m_Bmp.GetHeight()-1);
	int W_Is2=m_Bmp.GetWidth()&(m_Bmp.GetWidth()-1);
	if(H_Is2!=0||W_Is2!=0)
	{
		MessageBox(_T("ʹ���˲���������뱣֤�����Ϊ2��������"));
		OnPopupZoom();
		return;
	}
	if(m_bFT)
	{
		m_Bmp.iFFT();
	}
	m_bBusy=1;
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	m_Bmp.BackUpRGB();
	m_Bmp.FFT();
	m_Bmp.RestoreRGB();
	m_FFTDlg=new CFFTDlg;
	m_FFTDlg->Create(IDD_DIALOG_TOOL_IMAGEPROCESS_FFT_DLG);
	m_FFTDlg->ShowWindow(SW_SHOW);
	m_bBusy=0;
	InvalidateRect(m_ShowRect,0);
	m_bFFTDlgOpened=1;
}


afx_msg LRESULT CImageProcessDlg::OnFftCancel(WPARAM wParam, LPARAM lParam)
{
	m_bFFTDlgOpened=0;
	return 0;
}


void CImageProcessDlg::OnBnClickedBtnConnectMe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAboutDlg cCAboutDlg;
	cCAboutDlg.DoModal();
}





void CImageProcessDlg::OnPopupMorphology()
{
	// TODO: �ڴ���������������
	if(m_bMorphologyOpened)
	{
		MessageBox(_T("ιι�����Ѿ�����һ���Զ���Ի����˰�- -��"),_T("��ע����"));
		return;
	}
	bool filter[9][9]={0};
	m_MDlg=new CMorphologyDlg;
	m_MDlg->Create(IDD_DIALOG_TOOL_IMAGEPROCESS_MORPHOLOGY_DLG);
	m_MDlg->ShowWindow(SW_SHOW);
	m_bMorphologyOpened=1;
}


afx_msg LRESULT CImageProcessDlg::OnMorphologyOk(WPARAM wParam, LPARAM lParam)
{
	m_Bmp.BackUpRGB();
	OnMorphologyCertain(wParam,lParam);
	m_Bmp.RestoreRGB();
	m_bMorphologyOpened=1;
	return 0;
}


afx_msg LRESULT CImageProcessDlg::OnMorphologyCertain(WPARAM wParam, LPARAM lParam)
{
	CString str=m_MDlg->GetType();
	if(str==_T("����"))
	{
		m_Bmp.Morphology(0,m_MDlg->GetFilter(),m_MDlg->GetThreshold());
	}
	else if(str==_T("��ʴ"))
	{
		m_Bmp.Morphology(1,m_MDlg->GetFilter(),m_MDlg->GetThreshold());
	}
	else if(str==_T("��"))
	{
		m_Bmp.Morphology(1,m_MDlg->GetFilter(),m_MDlg->GetThreshold());
		m_Bmp.Morphology(0,m_MDlg->GetFilter(),m_MDlg->GetThreshold());
	}
	else if(str==_T("��"))
	{
		m_Bmp.Morphology(0,m_MDlg->GetFilter(),m_MDlg->GetThreshold());
		m_Bmp.Morphology(1,m_MDlg->GetFilter(),m_MDlg->GetThreshold());
	}
	InvalidateRect(m_ShowRect,0);
	UpdateWindow();
	m_bMorphologyOpened=0;
	return 0;
}


afx_msg LRESULT CImageProcessDlg::OnMorphologyCancel(WPARAM wParam, LPARAM lParam)
{
	if(!wParam)
		m_bMorphologyOpened=0;
	InvalidateRect(m_ShowRect,0);
	return 0;
}


void CImageProcessDlg::OnPopupGetboard()
{
	// TODO: �ڴ���������������
	bool **filter=new bool*[9];
	for(int i=0;i<9;i++)
	{
		*(filter+i)=new bool[9];
		for(int j=0;j<9;j++)
		{
			*(*(filter+i)+j)=0;
		}
	}
	for(int i=3;i<6;i++)
	{
		for(int j=3;j<6;j++)
		{
			*(*(filter+i)+j)=1;
		}
	}
	m_Bmp.BackUpRGB();
	m_Bmp.Morphology(1,filter,-1);
	m_Bmp.BkRGBMinusRGB();
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnPopupMask()
{
	// TODO: �ڴ���������������
	CMaskDlg CMDlg;
	if(IDOK==CMDlg.DoModal())
	{
		int Height=m_Bmp.GetHeight(),Width=m_Bmp.GetWidth();
		double Presentage=CMDlg.GetPresenage();
		m_Bmp.Zoom(static_cast<int>(double(Height)/(max(1,Presentage))),static_cast<int>(double(Width)/(max(1,Presentage))),0);
		m_Bmp.Zoom(Height,Width,0);
		InvalidateRect(m_ShowRect,0);
	}
}


void CImageProcessDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!m_Bmp.Exist())
		return;

	if(m_bIsCutting&&nFlags&MK_LBUTTON)
	{
		m_pt_rd=point;
		if(nFlags&MK_SHIFT)
		{
			int width=min(m_pt_rd.y-m_pt_lu.y,m_pt_rd.x-m_pt_lu.x);
			m_pt_rd.y=m_pt_lu.y+width;
			m_pt_rd.x=m_pt_lu.x+width;
		}
		InvalidateRect(m_ShowRect,0);
	}

	if(!PtInRect(m_ShowRect,point))
		return;

	CDC *cdc=GetDC();
	CDC dc;
	dc.CreateCompatibleDC(cdc);
	CBitmap CBmp;
	CRect WndRect;
	GetWindowRect(WndRect);
	CBmp.CreateCompatibleBitmap(cdc,WndRect.Width(),WndRect.Height());
	dc.SelectObject(CBmp);
	dc.FillRect(CRect(m_ShowRect.left,m_ShowRect.bottom+10,m_ShowRect.left+1500,m_ShowRect.bottom+100),&CBrush(cdc->GetPixel(0,0)));

	CBrush RedBrush(RGB(255,0,0));
	CBrush GreenBrush(RGB(0,255,0));
	CBrush BlueBrush(RGB(0,0,255));
	CBrush ColorBrush(RGB(m_Bmp.GetRGB(0,point.y-m_ShowRect.top,point.x-m_ShowRect.left),m_Bmp.GetRGB(1,point.y-m_ShowRect.top,point.x-m_ShowRect.left),m_Bmp.GetRGB(2,point.y-m_ShowRect.top,point.x-m_ShowRect.left)));
	int RectLen=50,RectGap=5;
	CRect RedRect(m_ShowRect.left,m_ShowRect.bottom+10,m_ShowRect.left+RectLen,m_ShowRect.bottom+35);
	CRect GreenRect(m_ShowRect.left+RectLen+RectGap,m_ShowRect.bottom+10,m_ShowRect.left+RectLen+RectGap+RectLen,m_ShowRect.bottom+35);
	CRect BlueRect(m_ShowRect.left+2*(RectLen+RectGap),m_ShowRect.bottom+10,m_ShowRect.left+2*(RectLen+RectGap)+RectLen,m_ShowRect.bottom+35);
	CRect XRect(m_ShowRect.left+3*(RectLen+RectGap),m_ShowRect.bottom+10,m_ShowRect.left+3*(RectLen+RectGap)+RectLen,m_ShowRect.bottom+35);
	CRect YRect(m_ShowRect.left+4*(RectLen+RectGap),m_ShowRect.bottom+10,m_ShowRect.left+4*(RectLen+RectGap)+RectLen,m_ShowRect.bottom+35);
	CRect CLRect(m_ShowRect.left,m_ShowRect.bottom+37,m_ShowRect.left+4*(RectLen+RectGap)+RectLen,m_ShowRect.bottom+40);
	dc.FillRect(RedRect,&RedBrush);
	dc.FillRect(GreenRect,&GreenBrush);
	dc.FillRect(BlueRect,&BlueBrush);
	if(m_bIsCutting&&nFlags&VK_LBUTTON)
	{
		CBrush PinkBrush(RGB(255,192,203));
		dc.FillRect(XRect,&PinkBrush);
		dc.FillRect(YRect,&PinkBrush);
	}
	else
	{
		CBrush PurpleBrush(RGB(192,64,255));
		dc.FillRect(XRect,&PurpleBrush);
		dc.FillRect(YRect,&PurpleBrush);
	}
	dc.FillRect(CLRect,&ColorBrush);

	dc.SetBkMode(TRANSPARENT);
	CFont cf;
	cf.CreatePointFont(180,_T("����"));
	dc.SelectObject(cf);
	CString str;

	str.Format(_T("%d"),m_Bmp.GetRGB(max(m_Bmp.GetColor()-3,0),point.y-m_ShowRect.top,point.x-m_ShowRect.left));
	dc.DrawText(str,RedRect,DT_CENTER);
	str.Format(_T("%d"),m_Bmp.GetRGB(max(m_Bmp.GetColor()-2,0),point.y-m_ShowRect.top,point.x-m_ShowRect.left));
	dc.DrawText(str,GreenRect,DT_CENTER);
	str.Format(_T("%d"),m_Bmp.GetRGB(max(m_Bmp.GetColor()-1,0),point.y-m_ShowRect.top,point.x-m_ShowRect.left));
	dc.DrawText(str,BlueRect,DT_CENTER);
	if(m_bIsCutting&&nFlags&MK_LBUTTON)
	{
		str.Format(_T("%d"),abs(m_pt_rd.x-m_pt_lu.x));
		dc.DrawText(str,XRect,DT_CENTER);
		str.Format(_T("%d"),abs(m_pt_rd.y-m_pt_lu.y));
		dc.DrawText(str,YRect,DT_CENTER);
	}
	else
	{
		str.Format(_T("%d"),point.x-m_ShowRect.left);
		dc.DrawText(str,XRect,DT_CENTER);
		str.Format(_T("%d"),point.y-m_ShowRect.top);
		dc.DrawText(str,YRect,DT_CENTER);
	}

	cdc->BitBlt(m_ShowRect.left,m_ShowRect.bottom+10,1500,100,&dc,m_ShowRect.left,m_ShowRect.bottom+10,SRCCOPY);
	ReleaseDC(&dc);
	ReleaseDC(cdc);
	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CImageProcessDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if(PtInRect(m_ShowRect,pt))
	{
		if(m_bIsCutting)
		{
			SetCursor(LoadCursor(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDC_TOOL_IMAGEPROCESS_AIR_CURSOR_CUT)));
		}
		else
		{
			SetCursor(LoadCursor(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDC_TOOL_IMAGEPROCESS_AIR_CURSOR)));
		}
	}
	else
	{
		CDialogEx::OnSetCursor(pWnd, nHitTest, message);
	}
	return 0;
}


void CImageProcessDlg::OnPopupThresholdEx()
{
	CThresholdExDlg CTEXDlg;
	CTEXDlg.DoModal(&m_Bmp);
	DeleteFile(_T("tmp"));
	InvalidateRect(m_ShowRect,0);
}


void CImageProcessDlg::OnPopupCut()
{
	// TODO: �ڴ���������������
	m_bIsCutting=true;
}


void CImageProcessDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_bIsCutting)
	{
		m_pt_rd=m_pt_lu=point;
		InvalidateRect(m_ShowRect,0);
	}
	else
	{
		m_pt_rd=m_pt_lu=0;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CImageProcessDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!m_bIsCutting)
	{
		m_pt_lu=m_pt_rd=0;
		return;
	}
	if(m_pt_lu==m_pt_rd)
	{
		return;
	}

	m_pt_lu.x=min(max(m_ShowRect.left,m_pt_lu.x),m_ShowRect.right-1);
	m_pt_rd.x=min(max(m_ShowRect.left,m_pt_rd.x),m_ShowRect.right-1);
	m_pt_lu.y=min(max(m_ShowRect.top,m_pt_lu.y),m_ShowRect.bottom-1);
	m_pt_rd.y=min(max(m_ShowRect.top,m_pt_rd.y),m_ShowRect.bottom-1);

	m_pt_lu-=m_ShowRect.TopLeft();
	m_pt_rd-=m_ShowRect.TopLeft();

	if(m_Bmp.Cut(m_pt_lu,m_pt_rd))
	{
		m_ShowRect.top=130-32-8;
		m_ShowRect.left=15;
		m_ShowRect.bottom=m_ShowRect.top+m_Bmp.GetHeight();
		m_ShowRect.right=m_ShowRect.left+m_Bmp.GetWidth();
		SetWindowPos(0,0,0,max(315,m_Bmp.GetWidth()+2*m_ShowRect.left+16),max(130+40,m_Bmp.GetHeight()+m_ShowRect.top+32+8+8+40),SWP_NOZORDER);
		CenterWindow();
	}
	
	m_pt_lu=m_pt_rd=0;
	m_bIsCutting=false;
	Invalidate();
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CImageProcessDlg::OnPopupCutInfo()
{
	MessageBox(_T("ʹ�������ͼ���ϰ���������ƶ������ͷ�������вü�,��ʱ�·���ɫλ�õ���ֵΪ�ü���Ĵ�С��"),_T("�ü�˵��1"));
	MessageBox(_T("�ü����̣��ѳ��ֲü����а�������Ҽ�����ȡ�����βü��������������ʼ�µĲü���"),_T("�ü�˵��2"));
	MessageBox(_T("�ü����̣��޲ü���ʱ���а�������Ҽ�����ȡ���ü����ܣ��ص�����״̬���ü�ʱ����shift�����Խ��������βü���"),_T("�ü�˵��3"));
}


void CImageProcessDlg::OnPopupBilateral()
{
	// TODO:  �ڴ���������������
	CBilateralDlg CBDlg;
	if (!(CBDlg.DoModal() == IDOK))
	{
		return;
	}

	m_bBusy = 1;
	InvalidateRect(m_ShowRect, 0);
	UpdateWindow();
	m_Bmp.BilateralFilter(CBDlg.r, CBDlg.m_sigmaG, CBDlg.m_sigmaS);
	m_bBusy = 0;
	InvalidateRect(m_ShowRect, 0);
}



void CImageProcessDlg::OnPopupGauss()
{
	// TODO:  �ڴ���������������
	CGaussDlg CGDlg;
	if (!(CGDlg.DoModal() == IDOK))
	{
		return;
	}

	m_bBusy = 1;
	InvalidateRect(m_ShowRect, 0);
	UpdateWindow();
	m_Bmp.GaussSmooth(CGDlg.r, CGDlg.m_sigma);
	m_bBusy = 0;
	InvalidateRect(m_ShowRect, 0);
}

