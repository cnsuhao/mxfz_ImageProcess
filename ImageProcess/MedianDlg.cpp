// MedianDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "MedianDlg.h"
#include "afxdialogex.h"


// CMedianDlg 对话框

IMPLEMENT_DYNAMIC(CMedianDlg, CDialogEx)

CMedianDlg::CMedianDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMedianDlg::IDD, pParent)
	, m_data_x(7)
	, m_data_y(7)
{

}

CMedianDlg::~CMedianDlg()
{
}

void CMedianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_MEDIAN_X, m_data_x);
	DDV_MinMaxInt(pDX, m_data_x, 1, 10);
	DDX_Text(pDX, IDC_TOOL_IMAGEPROCESS_EDIT_MEDIAN_Y, m_data_y);
	DDV_MinMaxInt(pDX, m_data_y, 1, 10);
}


BEGIN_MESSAGE_MAP(CMedianDlg, CDialogEx)
END_MESSAGE_MAP()


// CMedianDlg 消息处理程序
