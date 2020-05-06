// CPaintDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CPaintDlg.h"
#include "afxdialogex.h"


// CPaintDlg 대화 상자

IMPLEMENT_DYNAMIC(CPaintDlg, CDialogEx)

CPaintDlg::CPaintDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CPaintDlg, pParent)
{

}

CPaintDlg::~CPaintDlg()
{
}

void CPaintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPaintDlg, CDialogEx)
END_MESSAGE_MAP()


// CPaintDlg 메시지 처리기
