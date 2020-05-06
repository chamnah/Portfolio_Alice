// CCameraDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CCameraDlg.h"
#include "afxdialogex.h"
#include "CGameObj.h"
#include "CCamera.h"
#include "MainFrm.h"
#include "CSceneMgr.h"


// CCameraDlg 대화 상자

IMPLEMENT_DYNAMIC(CCameraDlg, CDialogEx)

CCameraDlg::CCameraDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_CAMERA, pParent)
	, m_bLayerFocus(false)
{
}

CCameraDlg::~CCameraDlg()
{
}

void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbCamType);
	DDX_Control(pDX, IDC_EDIT1, m_EditFar);
	DDX_Control(pDX, IDC_EDIT2, m_EditScale);
	DDX_Control(pDX, IDC_EDIT3, m_EditFOV);
	DDX_Control(pDX, IDC_COMBOBOXEX1, m_cbnLayer);
}

void CCameraDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;

	if (m_bFocus || m_cbFocus || m_bLayerFocus)
		return;
	m_bClick = false;

	if (m_pTargetObj->GetCamera()->IsPerspective())
	{
		m_cbCamType.SetCurSel(1);
		m_EditFOV.EnableWindow(true);
		m_EditScale.EnableWindow(false);
	}
	else
	{
		m_cbCamType.SetCurSel(0);
		m_EditFOV.EnableWindow(false);
		m_EditScale.EnableWindow(true);
	}

	CString strText;
	float fTemp = 0.f;


	fTemp = m_pTargetObj->GetCamera()->GetFar();
	strText.Format(L"%f", fTemp);
	m_EditFar.SetWindowTextW(strText);
	
	fTemp = m_pTargetObj->GetCamera()->GetFOV();
	strText.Format(L"%f", fTemp);
	m_EditFOV.SetWindowTextW(strText);

	fTemp = m_pTargetObj->GetCamera()->GetScale();
	strText.Format(L"%f", fTemp);
	m_EditScale.SetWindowTextW(strText);

	UINT iLayerMask = m_pTargetObj->GetCamera()->GetLayerMask();

	wchar_t szText[255] = {};
	COMBOBOXEXITEM  item{};
	item.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	item.pszText = szText;
	item.cchTextMax = 255;

	if (iLayerMask == 0xffff)
	{
		SetEverthing(); // Everything
	}
	else if (iLayerMask == 0) // Nothing
	{
		SetNothing();
	}
	else // Mixed
	{
		item.iItem = 0;
		m_cbnLayer.GetItem(&item);
		item.iImage = 0;
		m_cbnLayer.SetItem(&item);

		item.iItem = 1;
		m_cbnLayer.GetItem(&item);
		item.iImage = 0;
		m_cbnLayer.SetItem(&item);

		item.iItem = 2;
		m_cbnLayer.GetItem(&item);
		item.iImage = 1;
		m_cbnLayer.SetItem(&item);

		for (int i = 0; i < MAX_LAYER_COUNT; ++i)
		{
			if (iLayerMask & 1 << i)
			{
				item.iItem = i + 3;
				m_cbnLayer.GetItem(&item);
				item.iImage = 1;
				m_cbnLayer.SetItem(&item);
			}
		}

		m_cbnLayer.SetCurSel(2);

		//m_cbCullingMask.SetCurSel(2);
		//m_cbCullingMask.SetWindowTextW(L"Mixed ...");	
	}

	m_bClick = true; 
}

void CCameraDlg::Init()
{
	m_ImageList.Create(16,16, ILC_COLOR,2,2);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON4));

	Reset(nullptr);
}

void CCameraDlg::SetNothing()
{
	m_pTargetObj->GetCamera()->LayerMaskClear();

	int iMaxCount = m_cbnLayer.GetCount();
	wchar_t wcsName[255] = {};

	COMBOBOXEXITEM item{};

	item.mask = CBEIF_SELECTEDIMAGE | CBEIF_OVERLAY |  CBEIF_INDENT | CBEIF_TEXT | CBEIF_IMAGE;
	item.pszText = wcsName;
	item.cchTextMax = 255;

	item.iItem = 0;
	m_cbnLayer.GetItem(&item);
	item.iImage = 1;
	m_cbnLayer.SetItem(&item);

	for (int i = 1; i < iMaxCount; ++i)
	{
		item.iItem = i;
		m_cbnLayer.GetItem(&item);
		item.iImage = 0;
		m_cbnLayer.SetItem(&item);
	}

	m_cbnLayer.SetCurSel(0);
}

void CCameraDlg::SetEverthing()
{
	m_pTargetObj->GetCamera()->AllCheckLayerMask();

	int iMaxCount = m_cbnLayer.GetCount();
	wchar_t wcsName[255] = {};

	COMBOBOXEXITEM item{};

	item.mask = CBEIF_SELECTEDIMAGE | CBEIF_OVERLAY | CBEIF_INDENT | CBEIF_TEXT | CBEIF_IMAGE;
	item.pszText = wcsName;
	item.cchTextMax = 255;

	item.iItem = 0;
	m_cbnLayer.GetItem(&item);
	item.iImage = 0;
	m_cbnLayer.SetItem(&item);

	for (int i = 1; i < iMaxCount; ++i)
	{
		item.iItem = i;
		m_cbnLayer.GetItem(&item);
		item.iImage = 1;
		m_cbnLayer.SetItem(&item);
	}

	m_cbnLayer.SetCurSel(1);
}

void CCameraDlg::SetMixing(UINT _iId)
{
	m_pTargetObj->GetCamera()->CheckLayerMask(_iId - 3);

	wchar_t wcsName[255] = {};

	COMBOBOXEXITEM item{};

	item.mask = CBEIF_SELECTEDIMAGE | CBEIF_OVERLAY | CBEIF_INDENT | CBEIF_TEXT | CBEIF_IMAGE;
	item.pszText = wcsName;
	item.cchTextMax = 255;

	item.iItem = 0;
	m_cbnLayer.GetItem(&item);
	item.iImage = 0;
	m_cbnLayer.SetItem(&item);

	item.iItem = 1;
	m_cbnLayer.GetItem(&item);
	item.iImage = 0;
	m_cbnLayer.SetItem(&item);

	item.iItem = _iId;
	m_cbnLayer.GetItem(&item);
	
	if(item.iImage == 0)
		item.iImage = 1;
	else if(item.iImage == 1)
		item.iImage = 0;
	m_cbnLayer.SetItem(&item);

	m_cbnLayer.SetCurSel(_iId);
}

void CCameraDlg::Reset(CGameObj* _pObj)
{
	m_cbCamType.ResetContent();

	m_cbCamType.AddString(L"Orthographic");
	m_cbCamType.AddString(L"Perspective");

	m_cbnLayer.ResetContent();
	m_cbnLayer.SetImageList(&m_ImageList);

	COMBOBOXEXITEM item{};
	item.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY |
		CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	item.iItem = 0;
	item.pszText = L"Nothing";
	item.cchTextMax = (int)wcslen(item.pszText);
	item.iImage = 0;
	item.iSelectedImage = 1;
	item.iOverlay = 0;

	m_cbnLayer.InsertItem(&item);

	item.iItem = 1;
	item.pszText = L"Everything";
	item.cchTextMax = (int)wcslen(item.pszText);

	m_cbnLayer.InsertItem(&item);

	item.iItem = 2;
	item.pszText = L"Mixing";
	item.cchTextMax = (int)wcslen(item.pszText);

	m_cbnLayer.InsertItem(&item);

	UINT iCount = CSceneMgr::GetInst()->GetCurLayerCount();

	for (UINT i = 0; i < iCount; ++i)
	{
		item.pszText = L"";
		item.iItem = i + 3;
		item.iImage = 0;
		item.pszText = const_cast<wchar_t*>(CSceneMgr::GetInst()->GetLayerName(i).c_str());
		item.cchTextMax = (int)wcslen(item.pszText);
		m_cbnLayer.InsertItem(&item);
	}

	m_cbnLayer.SetCurSel(1);
}

BEGIN_MESSAGE_MAP(CCameraDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT3, &CComponentDlg::OnEditKillfocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT3, &CComponentDlg::OnEditSetfocus)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT3, &CCameraDlg::OnChangeEdit)

	ON_CBN_KILLFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnKillfocus)
	ON_CBN_SETFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnSetfocus)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCameraDlg::OnCbnSelchange)

	ON_CBN_SELCHANGE(IDC_COMBOBOXEX1, &CCameraDlg::OnCbnSelchangeLayer)
	ON_CBN_KILLFOCUS(IDC_COMBOBOXEX1, &CCameraDlg::OnCbnKillfocusLayer)
	ON_CBN_SETFOCUS(IDC_COMBOBOXEX1, &CCameraDlg::OnCbnSetfocusLayer)
END_MESSAGE_MAP()

void CCameraDlg::OnChangeEdit(UINT _iId)
{
	if (!m_bClick)
		return;

	CString strText;
	float fTemp = 0.f;

	m_EditFar.GetWindowTextW(strText);
	fTemp = (float)_wtof(strText);
	m_pTargetObj->GetCamera()->SetFar(fTemp);

	m_EditFOV.GetWindowTextW(strText);
	fTemp = (float)_wtof(strText);
	m_pTargetObj->GetCamera()->SetFOV(fTemp);

	m_EditScale.GetWindowTextW(strText);
	fTemp = (float)_wtof(strText);
	m_pTargetObj->GetCamera()->SetScale(fTemp);
}

void CCameraDlg::OnCbnSelchange()
{
	if(m_cbCamType.GetCurSel() == 1)
		m_pTargetObj->GetCamera()->SetPerspective(true);
	else
		m_pTargetObj->GetCamera()->SetPerspective(false);

	::SetFocus(((CMainFrame*)AfxGetMainWnd())->GetGameHwnd());
}

void CCameraDlg::OnCbnSelchangeLayer()
{
	int id = m_cbnLayer.GetCurSel();

	if (id == 0)
	{
		SetNothing();
	}
	else if (id == 1)
	{
		SetEverthing();
	}
	else if(id >= 3)
	{
		SetMixing(id);
	}

	::SetFocus(((CMainFrame*)AfxGetMainWnd())->GetGameHwnd());	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CCameraDlg::OnCbnKillfocusLayer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bLayerFocus = false;
}

void CCameraDlg::OnCbnSetfocusLayer()
{
	m_bLayerFocus = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
