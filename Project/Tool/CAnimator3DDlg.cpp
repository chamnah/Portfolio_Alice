// CAnimator3DDlg.cpp: 구현 파일
#include "stdafx.h"
#include "Tool.h"
#include "CAnimator3DDlg.h"
#include "afxdialogex.h"
#include "CGameObj.h"
#include "CAnimator3D.h"
#include "CPathMgr.h"

// CAnimator3DDlg 대화 상자
IMPLEMENT_DYNAMIC(CAnimator3DDlg, CDialogEx)

CAnimator3DDlg::CAnimator3DDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_ANIMATOR3DDlg, pParent)
{

}

CAnimator3DDlg::~CAnimator3DDlg()
{
}

void CAnimator3DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listAnim);
	DDX_Control(pDX, IDC_EDIT1, m_editKey);
	DDX_Control(pDX, IDC_EDIT2, m_editFrame);
}

void CAnimator3DDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;
	if (m_pTargetObj == nullptr)
		return;
}

void CAnimator3DDlg::Reset(CGameObj * _pObj)
{
	m_listAnim.ResetContent();
	map<wstring, tAnim3D*>& mapAnim = _pObj->GetAnimator3D()->GetMapAnim();
	map<wstring, tAnim3D*>::iterator iter = mapAnim.begin();

	for (; iter != mapAnim.end(); ++iter)
		m_listAnim.AddString(iter->first.c_str());
}

BEGIN_MESSAGE_MAP(CAnimator3DDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimator3DDlg::OnBnAddAnim)
	ON_BN_CLICKED(IDC_BUTTON5, &CAnimator3DDlg::OnBnDeleteAnim)
	ON_LBN_SELCHANGE(IDC_LIST1, &CAnimator3DDlg::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimator3DDlg::OnBnPlayAnim)
	ON_BN_CLICKED(IDC_BUTTON6, &CAnimator3DDlg::OnBnChange)
	ON_BN_CLICKED(IDC_BUTTON7, &CAnimator3DDlg::OnBnAnimDeleteAll)
	ON_BN_CLICKED(IDC_BUTTON8, &CAnimator3DDlg::OnBnSaveAnimFile)
	ON_BN_CLICKED(IDC_BUTTON13, &CAnimator3DDlg::OnBnLoadAnimFile)
END_MESSAGE_MAP()

// CAnimator3DDlg 메시지 처리기

void CAnimator3DDlg::OnBnAddAnim()
{
	if (m_pTargetObj == nullptr)
		return;
	
	CString strText;
	m_editKey.GetWindowTextW(strText);

	CString strFrame;
	m_editFrame.GetWindowTextW(strFrame);

	int iFrm = _wtoi(strFrame);
	if (iFrm == 0)
		return;

	if(m_pTargetObj->GetAnimator3D()->AddAnimation(strText.GetBuffer(),iFrm))
		m_listAnim.AddString(strText);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimator3DDlg::OnBnDeleteAnim()
{
	if (m_pTargetObj == nullptr)
		return;

	CString strText;
	m_editKey.GetWindowTextW(strText);

	m_pTargetObj->GetAnimator3D()->DeleteAnimation(strText.GetBuffer());

	int iSel = m_listAnim.GetCurSel();
	m_listAnim.DeleteString(iSel);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimator3DDlg::OnLbnSelchangeList()
{
	int iSel = m_listAnim.GetCurSel();
	CString strText;
	m_listAnim.GetText(iSel, strText);
	m_editKey.SetWindowTextW(strText);

	map<wstring, tAnim3D*>& mapAnim = m_pTargetObj->GetAnimator3D()->GetMapAnim();
	map<wstring, tAnim3D*>::iterator iter = mapAnim.begin();

	for (; iter != mapAnim.end(); ++iter)
	{
		if (iter->first == strText.GetBuffer())
		{
			CString strLength;
			strLength.Format(L"%d",iter->second->iFrmLength);
			m_editFrame.SetWindowTextW(strLength);
			break;
		}
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAnimator3DDlg::OnBnPlayAnim()
{
	int iSel = m_listAnim.GetCurSel();
	CString strText;
	m_listAnim.GetText(iSel, strText);

	if(strText != L"")
		m_pTargetObj->GetAnimator3D()->PlayAnim(strText.GetBuffer(), false,true);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAnimator3DDlg::OnBnChange()
{
	int iSel = m_listAnim.GetCurSel();
	CString strText;
	m_listAnim.GetText(iSel, strText);

	CString strUpdate;
	m_editKey.GetWindowTextW(strUpdate);
	if (strText != L"" && strUpdate != L"")
		m_pTargetObj->GetAnimator3D()->ChangeText(strText.GetBuffer(), strUpdate.GetBuffer());

	Reset(m_pTargetObj);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAnimator3DDlg::OnBnAnimDeleteAll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_pTargetObj == nullptr)
		return;

	m_editKey.SetWindowTextW(L"");
	m_editFrame.SetWindowTextW(L"");

	m_pTargetObj->GetAnimator3D()->DeleteAll();

	m_listAnim.ResetContent();
}

void CAnimator3DDlg::OnBnSaveAnimFile()
{
	if (m_pTargetObj == nullptr)
		return;

	wstring Path = CPathMgr::GetInst()->GetPath();
	Path += L"Anim\\";

	wchar_t szFilter[50] = L"";
	CFileDialog dlg(FALSE, L"Anim", L"*.anim", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = Path.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	Path = dlg.GetPathName();

	m_pTargetObj->GetAnimator3D()->FileSave(Path);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimator3DDlg::OnBnLoadAnimFile()
{
	wstring Path = CPathMgr::GetInst()->GetPath();
	Path += L"Anim\\";
	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"Anim", L"*.anim", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);
	dlg.m_ofn.lpstrInitialDir = Path.c_str();
	if (dlg.DoModal() != IDOK)
		return;

	Path = dlg.GetPathName();
	m_pTargetObj->GetAnimator3D()->FileLoad(Path);

	Reset(m_pTargetObj);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}