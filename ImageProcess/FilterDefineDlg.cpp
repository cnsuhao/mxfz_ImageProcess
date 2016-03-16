// FilterDefineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "FilterDefineDlg.h"
#include "afxdialogex.h"
#include "MfcFun.h"

// CFilterDefineDlg �Ի���

IMPLEMENT_DYNAMIC(CFilterDefineDlg, CDialogEx)

CFilterDefineDlg::CFilterDefineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilterDefineDlg::IDD, pParent)
	, m_data_divide(0)
	, m_data_move(0)
	, m_bPreview(TRUE)
	, m_Default(_T(""))
	, m_bData_Changed(0)
{
	memset(m_data,0,sizeof(double)*25);
	m_data[2][2]=1;
}

CFilterDefineDlg::~CFilterDefineDlg()
{
}

void CFilterDefineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_00, m_data[0][0]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_01, m_data[0][1]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_02, m_data[0][2]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_03, m_data[0][3]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_04, m_data[0][4]);

	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_10, m_data[1][0]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_11, m_data[1][1]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_12, m_data[1][2]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_13, m_data[1][3]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_14, m_data[1][4]);

	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_20, m_data[2][0]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_21, m_data[2][1]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_22, m_data[2][2]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_23, m_data[2][3]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_24, m_data[2][4]);

	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_30, m_data[3][0]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_31, m_data[3][1]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_32, m_data[3][2]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_33, m_data[3][3]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_34, m_data[3][4]);

	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_40, m_data[4][0]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_41, m_data[4][1]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_42, m_data[4][2]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_43, m_data[4][3]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_44, m_data[4][4]);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_DIVIDE, m_data_divide);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_MOVE, m_data_move);
	DDX_Check(pDX, IDC_TOOL_IMAGEPROCESS_CHECK_PREVIEW, m_bPreview);
	DDX_CBString(pDX, IDC_TOOL_IMAGEPROCESS_COMBO_DEFAULT, m_Default);
	//DDX_Control(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_00, m_dtat_aa);
}


BEGIN_MESSAGE_MAP(CFilterDefineDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TOOL_IMAGEPROCESS_CHECK_PREVIEW, &CFilterDefineDlg::OnBnClickedCheckPreview)
	ON_BN_CLICKED(IDOK, &CFilterDefineDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_TOOL_IMAGEPROCESS_COMBO_DEFAULT, &CFilterDefineDlg::OnCbnSelchangeComboDefault)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CFilterDefineDlg ��Ϣ�������


void CFilterDefineDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(1);
	UpdateData();
	if(m_bPreview)
	{
		if(nIDEvent==1)
		{
			for(int i=0;i<5;i++)
			{
				for(int j=0;j<5;j++)
				{
					(m_data[i][j])==m_data_old[i][j]?NULL:(m_data_old[i][j]=m_data[i][j],m_bData_Changed=1);
				}
			}
			m_data_divide_old==m_data_divide?NULL:(m_data_divide_old=m_data_divide,m_bData_Changed=1);
			m_data_move_old==m_data_move?NULL:(m_data_move_old=m_data_move,m_bData_Changed=1);

			if(m_bData_Changed)
			{
				GetParent()->SendMessage(WM_FILTER_DEFINE_OK,m_data_divide,m_data_move);
				m_bData_Changed=0;
			}
		}
	}
	UpdateData(0);
	SetTimer(1,200,NULL);
	CDialogEx::OnTimer(nIDEvent);
}

int CFilterDefineDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			m_data_old[i][j]=m_data[i][j];
		}
	}

	m_data_divide_old=m_data_divide;
	m_data_move_old=m_data_move;

	SetTimer(1,200,NULL);

	return 0;
}


void CFilterDefineDlg::OnBnClickedCheckPreview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if(m_bPreview)
	{
		GetParent()->SendMessage(WM_FILTER_DEFINE_OK,m_data_divide,m_data_move);
	}
	else
	{
		GetParent()->SendMessage(WM_FILTER_DEFINE_CANCEL);
	}
}


void CFilterDefineDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetParent()->SendMessage(WM_FILTER_DEFINE_CERTAIN,m_data_divide,m_data_move);
	CDialogEx::OnOK();
}


void CFilterDefineDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetParent()->SendMessage(WM_FILTER_DEFINE_CANCEL);
	CDialogEx::OnClose();
}


void CFilterDefineDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	GetParent()->SendMessage(WM_FILTER_DEFINE_CANCEL);
	CDialogEx::OnCancel();
}


void CFilterDefineDlg::OnCbnSelchangeComboDefault()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ԭͼ;���;3x3ƽ��;5x5ƽ��;1x2���Ҳ��;2x1���²��;2x2�Խǲ��;2x2���Խǲ��;
	UpdateData();
	if(m_Default==_T("ԭͼ"))
	{
		memset(m_data,0,sizeof(double)*25);
		m_data[2][2]=1;
		m_data_divide=0;
		m_data_move=0;
	}
	else if(m_Default==_T("���"))
	{
		memset(m_data,0,sizeof(double)*25);
		m_data_divide=0;
		m_data_move=0;
	}
	else if(m_Default==_T("3x3ƽ��"))
	{
		memset(m_data,0,sizeof(double)*25);
		for(int i=1;i<4;i++)
		{
			for(int j=1;j<4;j++)
			{
				m_data[i][j]=1;
			}
		}
		m_data_divide=9;
		m_data_move=0;
	}
	else if(m_Default==_T("5x5ƽ��"))
	{
		for(int i=0;i<5;i++)
		{
			for(int j=0;j<5;j++)
			{
				m_data[i][j]=1;
			}
		}
		m_data_divide=25;
		m_data_move=0;
	}
	else if(m_Default==_T("1x2���Ҳ��"))
	{
		memset(m_data,0,sizeof(double)*25);
		m_data[2][2]=-1;
		m_data[2][3]=1;
		m_data_divide=0;
		m_data_move=0;
	}
	else if(m_Default==_T("2x1���²��"))
	{
		memset(m_data,0,sizeof(double)*25);
		m_data[2][2]=-1;
		m_data[3][2]=1;
		m_data_divide=0;
		m_data_move=0;
	}
	else if(m_Default==_T("2x2�Խǲ��"))
	{
		memset(m_data,0,sizeof(double)*25);
		m_data[2][2]=-1;
		m_data[3][3]=1;
		m_data_divide=0;
		m_data_move=0;
	}
	else if(m_Default==_T("2x2���Խǲ��"))
	{
		memset(m_data,0,sizeof(double)*25);
		m_data[2][3]=-1;
		m_data[3][2]=1;
		m_data_divide=0;
		m_data_move=0;
	}
	UpdateData(0);
}


void CFilterDefineDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	GetParent()->SendMessage(WM_FILTER_DEFINE_OK,m_data_divide,m_data_move);
}

BOOL CFilterDefineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  Add extra initialization here
	CComboBox* pCComboBox = NULL;
	long lCursor = 0x00;
	pCComboBox = (CComboBox*)GetDlgItem(IDC_TOOL_IMAGEPROCESS_COMBO_DEFAULT);
	pCComboBox->InsertString(lCursor++, _T("ԭͼ"));
	pCComboBox->InsertString(lCursor++, _T("���"));
	pCComboBox->InsertString(lCursor++, _T("3x3ƽ��"));
	pCComboBox->InsertString(lCursor++, _T("5x5ƽ��"));
	pCComboBox->InsertString(lCursor++, _T("1x2���Ҳ��"));
	pCComboBox->InsertString(lCursor++, _T("2x1���²��"));
	pCComboBox->InsertString(lCursor++, _T("2x2�Խǲ��"));
	pCComboBox->InsertString(lCursor++, _T("2x2���Խǲ��"));
	set_DropDownSize(*pCComboBox,lCursor++);// �ڶ������������߶�����ʾ����
	pCComboBox->SetCurSel(0x00);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
