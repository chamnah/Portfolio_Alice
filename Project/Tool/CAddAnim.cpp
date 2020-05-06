// CAddAnim.cpp: 구현 파일
//
#include "stdafx.h"
#include "Tool.h"
#include "CAddAnim.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CAddAnim, CDialogEx)

CAddAnim::CAddAnim(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDANIM, pParent)
{
}

CAddAnim::~CAddAnim()
{
}

void CAddAnim::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditName);
}

BEGIN_MESSAGE_MAP(CAddAnim, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddAnim::OnBnFindFolder)
END_MESSAGE_MAP()

void CAddAnim::OnBnFindFolder()
{
	int iSize = m_EditName.GetWindowTextLengthW();

	if (0 >= iSize)
		return;

	ITEMIDLIST *pidlBrowse;
	
	wchar_t wPath[MAX_PATH] = L"";

	BROWSEINFO BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = wPath;
	BrInfo.lpszTitle = _T("폴더를 선택해 주십시오.");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	// 다이얼로그 띄우기
	pidlBrowse = (ITEMIDLIST*)(SHBrowseForFolder(&BrInfo));

	if (pidlBrowse != NULL)
	{
		BOOL bSuccess = ::SHGetPathFromIDList(pidlBrowse, wPath);

		if (!bSuccess)
		{
			MessageBox(_T("잘못된 폴더명 입니다."), _T(""), MB_OKCANCEL | MB_ICONASTERISK);
			return;
		}

		m_strPath = wPath;
		m_EditName.GetWindowTextW(m_strName);

		EndDialog(IDOK);
	}
	else
		EndDialog(IDCANCEL);
	
}