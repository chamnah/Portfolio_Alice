// CAnimationDlg.cpp: 구현 파일
#include "stdafx.h"
#include "Tool.h"
#include "CAnimationDlg.h"
#include "afxdialogex.h"
#include "CGameObj.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"
#include "CTexture.h"
#include "CPathMgr.h"
#include "CAddAnim.h"

// CAnimationDlg 대화 상자

IMPLEMENT_DYNAMIC(CAnimationDlg, CDialogEx)

CAnimationDlg::CAnimationDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_ANIMATION, pParent)
	, m_pCurSelAnim(nullptr)
	, m_bSelectFrm(false)
{

}

CAnimationDlg::~CAnimationDlg()
{
}

void CAnimationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListClip);
	DDX_Control(pDX, IDC_LIST2, m_ListFrm);
	for (int i = 0; i < 3; ++i)
	{
		DDX_Control(pDX, IDC_EDIT1 + i, m_EditAnim[i]);
	}
	DDX_Control(pDX, IDC_CHECK1, m_Check);
	DDX_Control(pDX, IDC_EDIT4, m_editLeft);
	DDX_Control(pDX, IDC_EDIT5, m_editTop);
	DDX_Control(pDX, IDC_EDIT6, m_editRight);
	DDX_Control(pDX, IDC_EDIT7, m_editBottom);
}

void CAnimationDlg::Init()
{
}

void CAnimationDlg::Update(CGameObj * _pObj)
{
	if (m_bFocus)
		return;
	m_bClick = false;
	m_pTargetObj = _pObj;

	Vec2 vPos = m_pTargetObj->GetAnimator2D()->GetOffset();
	CString strTemp;
	strTemp.Format(L"%f", vPos.x);
	m_EditAnim[0].SetWindowTextW(strTemp);

	strTemp.Format(L"%f", vPos.y);
	m_EditAnim[1].SetWindowTextW(strTemp);

	if (m_pTargetObj != _pObj || m_pTargetObj->GetAnimator2D()->IsReset())
	{
		m_ListClip.ResetContent();

		map<wstring, CAnimation2D*>& mapAnim = m_pTargetObj->GetAnimator2D()->GetAnimation();

		for (auto Anim : mapAnim)
		{
			m_ListClip.AddString(Anim.second->GetName().c_str());
		}

		m_pTargetObj->GetAnimator2D()->SetReset(false);
	}

	m_bClick = true;
}

BEGIN_MESSAGE_MAP(CAnimationDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CAnimationDlg::OnLbnChangeClip)
	ON_LBN_SELCHANGE(IDC_LIST2, &CAnimationDlg::OnLbnChangeFrame)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1,IDC_EDIT7, &CAnimationDlg::OnEnChangeEdit)
	ON_BN_CLICKED(IDC_BUTTON4, &CAnimationDlg::OnBnPlay)
	ON_BN_CLICKED(IDC_BUTTON6, &CAnimationDlg::OnBnPause)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimationDlg::OnBnAddAnimation)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimationDlg::OnBnRemoveAnimation)
	ON_BN_CLICKED(IDC_BUTTON7, &CAnimationDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON8, &CAnimationDlg::OnBnAddAnim)

	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT3, &CComponentDlg::OnEditKillfocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT3, &CComponentDlg::OnEditSetfocus)
	ON_EN_CHANGE(IDC_EDIT5, &CAnimationDlg::OnEnChangeEdit5)
	ON_BN_CLICKED(IDC_BUTTON11, &CAnimationDlg::OnBnClickedAddTex)
END_MESSAGE_MAP()

void CAnimationDlg::OnLbnChangeClip()
{
	int iSel = m_ListClip.GetCurSel();

	if (iSel != LB_ERR)
	{
		m_ListFrm.ResetContent();

		CString ItemSelected;
		m_ListClip.GetText(iSel, ItemSelected);
		wstring strSel = ItemSelected;

		map<wstring, CAnimation2D*>& mapAnim = m_pTargetObj->GetAnimator2D()->GetAnimation();
		map<wstring, CAnimation2D*>::iterator Animater;

		
		Animater = mapAnim.find(strSel);
		assert(!(Animater == mapAnim.end()));

		m_pCurSelAnim = Animater->second;
		m_pTargetObj->GetAnimator2D()->SetCurAnim(m_pCurSelAnim);

		vector<tAnim>& vecFrm = m_pCurSelAnim->GetAllFrame();

		for (UINT i = 0; i < vecFrm.size(); ++i)
		{
			//wstring& wcsName = CPathMgr::GetInst()->GetPathSlice(vecFrm[i].pTex->GetName());
			 
			m_ListFrm.InsertString(i, vecFrm[i].pTex->GetName().c_str());
		}
	}
}

void CAnimationDlg::OnLbnChangeFrame()
{
	int iSel = m_ListFrm.GetCurSel();

	if (iSel != LB_ERR)
	{
		m_bSelectFrm = true;

		m_pTargetObj->GetAnimator2D()->ChangeAnimation(m_pCurSelAnim);
		m_pCurSelAnim->SetMoveFrm(iSel);

		Vec4 vCutSize;
		vCutSize.x = m_pCurSelAnim->GetAllFrame()[iSel].vLT.x * (float)m_pCurSelAnim->GetAllFrame()[iSel].pTex->GetWidth();
		vCutSize.y = m_pCurSelAnim->GetAllFrame()[iSel].vLT.y * (float)m_pCurSelAnim->GetAllFrame()[iSel].pTex->GetHeight();
		vCutSize.z = m_pCurSelAnim->GetAllFrame()[iSel].vSize.x * (float)m_pCurSelAnim->GetAllFrame()[iSel].pTex->GetWidth();
		vCutSize.w = m_pCurSelAnim->GetAllFrame()[iSel].vSize.y * (float)m_pCurSelAnim->GetAllFrame()[iSel].pTex->GetHeight();

		CString strTemp;
		strTemp.Format(L"%f",m_pCurSelAnim->GetAllFrame()[iSel].vOffset.x);
		m_EditAnim[0].SetWindowTextW(strTemp);

		strTemp.Format(L"%f", m_pCurSelAnim->GetAllFrame()[iSel].vOffset.y);
		m_EditAnim[1].SetWindowTextW(strTemp);

		strTemp.Format(L"%f", m_pCurSelAnim->GetAllFrame()[iSel].fDuration);
		m_EditAnim[2].SetWindowTextW(strTemp);

		strTemp.Format(L"%f", vCutSize.x);
		m_editLeft.SetWindowTextW(strTemp);

		strTemp.Format(L"%f", vCutSize.y);
		m_editTop.SetWindowTextW(strTemp);

		strTemp.Format(L"%f", vCutSize.z);
		m_editRight.SetWindowTextW(strTemp);

		strTemp.Format(L"%f", vCutSize.w);
		m_editBottom.SetWindowTextW(strTemp);

		m_bSelectFrm = false;
	}
}

void CAnimationDlg::OnEnChangeEdit(UINT _iId)
{
	if (!m_bClick)
		return;

	if (m_bSelectFrm)
		return;

	CString strTemp;
	Vec2 vOffset;
	float fDuration;
	Vec4 vCutSize;

	m_EditAnim[0].GetWindowTextW(strTemp);
	vOffset.x = (float)_wtof(strTemp);

	m_EditAnim[1].GetWindowTextW(strTemp);
	vOffset.y = (float)_wtof(strTemp);

	m_EditAnim[2].GetWindowTextW(strTemp);
	fDuration = (float)_wtof(strTemp);

	m_editLeft.GetWindowTextW(strTemp);
	vCutSize.x = (float)_wtof(strTemp);

	m_editTop.GetWindowTextW(strTemp);
	vCutSize.y = (float)_wtof(strTemp);

	m_editRight.GetWindowTextW(strTemp);
	vCutSize.z = (float)_wtof(strTemp);

	m_editBottom.GetWindowTextW(strTemp);
	vCutSize.w = (float)_wtof(strTemp);

	m_pTargetObj->GetAnimator2D()->SetOffset(vOffset);
	m_pTargetObj->GetAnimator2D()->SetDurationTime(fDuration);
	m_pTargetObj->GetAnimator2D()->SetCurFrmCutSize(vCutSize);
}

void CAnimationDlg::OnBnPlay()
{
	if (m_pTargetObj == nullptr || m_pTargetObj->GetAnimator2D() == nullptr)
		return;

	m_pTargetObj->GetAnimator2D()->IsPlay(true);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAnimationDlg::OnBnPause()
{
	m_pTargetObj->GetAnimator2D()->IsPlay(false);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAnimationDlg::OnBnAddAnimation()
{
	if (m_pTargetObj == nullptr)
		return;

	CAddAnim Add;

	INT_PTR id = Add.DoModal();

	if (id == IDOK)
	{
		wstring Name = Add.GetName();
		wstring Path = Add.GetPath();

		Path += L"\\";

		m_pTargetObj->GetAnimator2D()->AddAnimation(Path, Name);
	}
}

void CAnimationDlg::OnBnRemoveAnimation()
{	
	int iSel = m_ListClip.GetCurSel();

	if (iSel != LB_ERR)
	{
		CString ItemSelected;
		m_ListClip.GetText(iSel, ItemSelected);
		wstring strSel = ItemSelected;
		
		m_ListFrm.ResetContent();

		m_pTargetObj->GetAnimator2D()->RemoveAnimation(strSel);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}	
	
void CAnimationDlg::OnBnClickedSave()
{	
	wstring strPath = CPathMgr::GetInst()->GetPath();

	// FileDialog
	wchar_t szFilter[50] = L"";

	CFileDialog dlg(FALSE, L"anim", L"*.anim",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = strPath.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	strPath = dlg.GetPathName();

	FILE* pFile = NULL;
	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	 map<wstring, CAnimation2D*>& mapAnim = m_pTargetObj->GetAnimator2D()->GetAnimation();
	 map<wstring, CAnimation2D*>::iterator Animater = mapAnim.begin();

	int iClipSize = (int)mapAnim.size();
	fwrite(&iClipSize, sizeof(int), 1, pFile);

	int iNameLength = 0;
	int iPathLength = 0;

	for (; Animater != mapAnim.end(); ++Animater)
	{
		iNameLength = (int)Animater->second->GetName().length();
		fwrite(&iNameLength, sizeof(int), 1, pFile);
		fwrite(Animater->second->GetName().c_str(), sizeof(wchar_t), iNameLength, pFile);
		
		iPathLength = (int)Animater->second->GetPath().length();
		fwrite(&iPathLength, sizeof(int), 1, pFile);
		fwrite(Animater->second->GetPath().c_str(), sizeof(wchar_t), iPathLength, pFile);

		vector<tAnim>& vecAnim = Animater->second->GetAllFrame();

		int iFrmSize = (int)vecAnim.size();
		fwrite(&iFrmSize, sizeof(int), 1, pFile);

		if (iPathLength == 0) // 낱장 형식으로 읽는 애니메이션이다.
		{
			for (int i = 0; i < iFrmSize; ++i)
			{
				fwrite(&vecAnim[i].vOffset.x, sizeof(float), 1, pFile);
				fwrite(&vecAnim[i].vOffset.y, sizeof(float), 1, pFile);
				fwrite(&vecAnim[i].fDuration, sizeof(float), 1, pFile);
				fwrite(&vecAnim[i].vLT, sizeof(Vec2), 1, pFile);
				fwrite(&vecAnim[i].vSize, sizeof(Vec2), 1, pFile);
				SaveWString(vecAnim[i].pTex->GetPath(), pFile);
			}
		}
		else // 폴더 단위로 읽는 애니메이션이다.
		{
			for (int i = 0; i < iFrmSize; ++i)
			{
				fwrite(&vecAnim[i].vOffset.x, sizeof(float), 1, pFile);
				fwrite(&vecAnim[i].vOffset.y, sizeof(float), 1, pFile);
				fwrite(&vecAnim[i].fDuration, sizeof(float), 1, pFile);
			}
		}
	}

	fclose(pFile);
}

void CAnimationDlg::OnBnAddAnim()
{
	if (m_pTargetObj == nullptr)
		return;

	ITEMIDLIST *pidlBrowse;
	CString    strPath;

	wchar_t wPath[MAX_PATH] = L"";

	BROWSEINFO BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = wPath;
	BrInfo.lpszTitle = _T("폴더를 선택해 주십시오.");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	// 다이얼로그 띄우기
	pidlBrowse = (ITEMIDLIST*)SHBrowseForFolder(&BrInfo);

	if (pidlBrowse != NULL)
	{
		BOOL bSuccess = ::SHGetPathFromIDList(pidlBrowse, wPath);

		if (!bSuccess)
		{
			MessageBox(_T("잘못된 폴더명 입니다."), _T(""), MB_OKCANCEL | MB_ICONASTERISK);
			return;
		}

		strPath = wPath;
		strPath += L"\\*.*";


		CFileFind file;

		bool bFile = file.FindFile(strPath.GetBuffer());

		while (bFile)
		{
			bFile = file.FindNextFileW();
			if (file.IsDots())
				continue;

			CString strPath = file.GetFilePath();
			CString strName = file.GetFileName();
			strPath += L"\\";
			m_pTargetObj->GetAnimator2D()->AddAnimation(strPath.GetBuffer(), file.GetFileName().GetBuffer());
		}
	}
}

void CAnimationDlg::OnEnChangeEdit5()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CComponentDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAnimationDlg::OnBnClickedAddTex()
{
	if (m_pTargetObj == nullptr)
		return;

	wstring Path = CPathMgr::GetInst()->GetPath();
	Path += L"Texture\\";

	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"Texture", L"*.tga", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = Path.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	Path = dlg.GetPathName();
	wstring wcsKey = dlg.GetFileName();
	CTexture* pTex = CTexture::CreateTexture(Path);
	
	m_pTargetObj->GetAnimator2D()->AddAnimation(pTex, Vec2(0.f, 0.f), Vec2(pTex->GetWidth(), pTex->GetHeight()), 1, wcsKey);
}