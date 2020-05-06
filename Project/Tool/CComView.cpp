#include "stdafx.h"
#include "Tool.h"
#include "CComView.h"
#include "CGameObj.h"
#include "CSceneMgr.h"
#include "CComponentDlg.h"
#include "CTransformDlg.h"
#include "CMeshRenderDlg.h"
#include "CCameraDlg.h"
#include "CColliderDlg.h"
#include "CAddComponentDlg.h"
#include "CAnimatorDlg.h"
#include "CGridDlg.h"
#include "CScriptDlg.h"
#include "CSaveLoadMgr.h"
#include "CManageDlg.h"
#include "CLayerDlg.h"
#include "CIncidentDlg.h"
#include "CLightDlg.h"
#include "CPaintDlg.h"
#include "CTerrainDlg.h"
#include "CAnimator3DDlg.h"
#include "CTextDlg.h"

IMPLEMENT_DYNAMIC(CComView, CDockablePane)

CComView::CComView()
	:m_arrComDlg{}
	, m_iOldScrollPos(0)
{
}

CComView::~CComView()
{
}

BEGIN_MESSAGE_MAP(CComView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
//	ON_WM_PAINT()
END_MESSAGE_MAP()

void CComView::Init()
{
	//m_pTargetObj = CSceneMgr::GetInst()->FindObject(L"Camera");

	for (UINT i = 0; i < (UINT)COMDLG_TYPE::END; ++i)
	{
		if (nullptr != m_arrComDlg[i])
		{
			m_arrComDlg[i]->Init();
			m_arrComDlg[i]->ShowWindow(false);
		}
	}

	m_pAddCom->Init();
	m_pAddCom->ShowWindow(false);

	m_Layer->Init();

	RECT WinRect = {};
	GetWindowRect(&WinRect);
	
	m_Scroll = new CManageDlg(this);
	m_Scroll->Create(IDD_MANAGE,this);
	m_Scroll->ShowWindow(true);
	m_Scroll->SetWindowPos(nullptr, WinRect.right - WinRect.left - 20, 0, 20, WinRect.bottom - WinRect.top - 20, 0);
	m_Scroll->GetScrollBar().SetWindowPos(nullptr, 0, 0, 20,WinRect.bottom - WinRect.top - 20, 0);

	/*m_pPaint = new CPaintDlg(this);
	m_pPaint->Create(IDD_CPaintDlg, this);
	m_pPaint->ShowWindow(true);*/
}

void CComView::Update()
{
	if (m_pTargetObj != nullptr && m_pTargetObj->GetDead())
		m_pTargetObj = nullptr;
	
	DialogSort();

	m_pAddCom->Update(m_pTargetObj);
	m_Layer->Update(m_pTargetObj);

	if (nullptr == m_pTargetObj)
		return;

	for (UINT i = 0; i < (UINT)COMDLG_TYPE::END; ++i)
	{
		if (i == (UINT)COMDLG_TYPE::SCRIPT)
		{
			m_arrComDlg[i]->Update(m_pTargetObj);
			continue;
		}

		if (nullptr != m_arrComDlg[i] && m_pTargetObj->GetComponent((COMPONENT_TYPE)i) != nullptr)
			m_arrComDlg[i]->Update(m_pTargetObj);
	}
}

void CComView::DialogSort()
{
	//Invalidate(TRUE);
	CRect rt = {};
	CRect rtDlg = {};
	GetWindowRect(&rt);

	//m_pPaint->SetWindowPos(nullptr, 0, 0, rt.right - rt.left - 20, rt.bottom - rt.top,0);

	int   iScrollY = m_Scroll->GetScrollPosY();
	m_Layer->GetWindowRect(&rtDlg);
	m_Layer->SetWindowPos(nullptr, 0, 0 - iScrollY, rtDlg.Width(), rtDlg.Height(), 0);
	
	int   iPosY = rtDlg.Height() + 1;
	//Invalidate(FALSE);
	for (UINT i = 0; i < (UINT)COMDLG_TYPE::END; ++i)
	{
		if (i != (UINT)COMDLG_TYPE::SCRIPT)
		{
			CComponent* pCom = nullptr;
			if(m_pTargetObj != nullptr)
				pCom = m_pTargetObj->GetComponent(COMPONENT_TYPE(i));

			if (m_arrComDlg[i] == nullptr)
				continue;

			if (pCom == nullptr)
			{
				m_arrComDlg[i]->ShowWindow(false);
				continue;
			}
		}

		m_arrComDlg[i]->GetWindowRect(&rtDlg);
		m_arrComDlg[i]->SetWindowPos(nullptr, 0, iPosY - iScrollY, rt.Width() - 25, rtDlg.Height(),0);
		if(m_iOldScrollPos != iScrollY)
			m_arrComDlg[i]->Invalidate(FALSE);
		
		m_arrComDlg[i]->ShowWindow(true);
		iPosY += rtDlg.Height() - 1;
	}

	if (m_pAddCom != nullptr)
	{
		m_pAddCom->GetWindowRect(&rtDlg);

		float fPosX = (rt.Width() * 0.5f) - (rtDlg.Width() * 0.5f);
		m_pAddCom->SetWindowPos(nullptr, (int)fPosX, iPosY + 5 - iScrollY, rtDlg.Width(), rtDlg.Height(), 0);
		m_pAddCom->ShowWindow(true);
		if (m_iOldScrollPos != iScrollY)
			m_pAddCom->Invalidate(FALSE);
		Invalidate(TRUE);
	}

	m_iOldScrollPos = iScrollY;
}

void CComView::Reset()
{
	if (m_pTargetObj == nullptr)
		return;

	if (m_pTargetObj->GetDead())
	{
		m_pTargetObj = nullptr;
		return;
	}
	for (UINT i = 0; i < (UINT)COMDLG_TYPE::END; ++i)
	{
		if (i == (UINT)COMDLG_TYPE::SCRIPT)
		{
			m_arrComDlg[i]->Reset(m_pTargetObj);
			continue;
		}

		if (nullptr != m_arrComDlg[i] && m_pTargetObj->GetComponent((COMPONENT_TYPE)i) != nullptr)
			m_arrComDlg[i]->Reset(m_pTargetObj);
	}
	
}

int CComView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pAddCom = new CAddComponentDlg(this);
	m_pAddCom->Create(IDD_ADDCOMPONENT, this);
	m_pAddCom->ShowWindow(true);

	m_arrComDlg[(UINT)COMDLG_TYPE::TRANSFORM] = new CTransformDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::TRANSFORM]->Create(IDD_TRANSFORM, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::TRANSFORM]->ShowWindow(true);

	m_arrComDlg[(UINT)COMDLG_TYPE::MESH_RENDER] = new CMeshRenderDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::MESH_RENDER]->Create(IDD_MESHRENDER, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::MESH_RENDER]->ShowWindow(true);

	m_arrComDlg[(UINT)COMDLG_TYPE::CAMERA] = new CCameraDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::CAMERA]->Create(IDD_CAMERA, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::CAMERA]->ShowWindow(true);

	m_arrComDlg[(UINT)COMDLG_TYPE::COLLIDER] = new CColliderDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::COLLIDER]->Create(IDD_COLLIDER, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::COLLIDER]->ShowWindow(true);

	m_arrComDlg[(UINT)COMDLG_TYPE::ANIMATOR_2D] = new CAnimatorDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::ANIMATOR_2D]->Create(IDD_ANIMATOR, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::ANIMATOR_2D]->ShowWindow(true);

	m_arrComDlg[(UINT)COMDLG_TYPE::ANIMATOR_3D] = new CAnimator3DDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::ANIMATOR_3D]->Create(IDD_ANIMATOR3DDlg, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::ANIMATOR_3D]->ShowWindow(true);

	m_arrComDlg[(UINT)COMDLG_TYPE::GRID] = new CGridDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::GRID]->Create(IDD_GRID, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::GRID]->ShowWindow(true);

	CIncidentDlg* Incident = new CIncidentDlg(this);

	m_arrComDlg[(UINT)COMDLG_TYPE::INCIDENT] = Incident;
	m_arrComDlg[(UINT)COMDLG_TYPE::INCIDENT]->Create(IDD_EVENT_DLG, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::INCIDENT]->ShowWindow(true);

	m_arrComDlg[(UINT)COMDLG_TYPE::LIGHT] = new CLightDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::LIGHT]->Create(IDD_LIGHTDLG, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::LIGHT]->ShowWindow(true);

	m_arrComDlg[(UINT)COMDLG_TYPE::SCRIPT] = new CScriptDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::SCRIPT]->Create(IDD_SCRIPT, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::SCRIPT]->ShowWindow(true);

	m_arrComDlg[(UINT)COMDLG_TYPE::TERRAIN] = new CTerrainDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::TERRAIN]->Create(IDD_TerrainDlg, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::TERRAIN]->ShowWindow(true);

	m_arrComDlg[(UINT)COMDLG_TYPE::TEXT] = new CTextDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::TEXT]->Create(IDD_TEXTDLG, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::TEXT]->ShowWindow(true);

	m_Layer = new CLayerDlg;
	m_Layer->Create(IDD_LAYER,this);
	CRect rt = {};
	m_Layer->GetWindowRect(&rt);
	m_Layer->SetWindowPos(nullptr, 20, 0, rt.Width(), rt.Height(), 0);
	m_Layer->ShowWindow(true);
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

void CComView::OnDestroy()
{
	for (UINT i = 0; i < (UINT)COMDLG_TYPE::END; ++i)
	{
		if (nullptr != m_arrComDlg[i])
			m_arrComDlg[i]->DestroyWindow();
	}

	if (nullptr != m_pAddCom)
		m_pAddCom->DestroyWindow();

	m_Scroll->DestroyWindow();

	m_Layer->DestroyWindow();

	CDockablePane::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CComView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (zDelta > 0)
	{
		m_Scroll->AddScrollPosY(-10);
	}
	else if (zDelta < 0)
	{
		m_Scroll->AddScrollPosY(10);
	}

	return CDockView::OnMouseWheel(nFlags, zDelta, pt);
}

void CComView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDockView::OnPaint()을(를) 호출하지 마십시오.

	HBRUSH brush = CreateSolidBrush(RGB(241, 241, 241));
	SelectObject(dc, brush);
	RECT rt = {};
	GetClientRect(&rt);
	Rectangle(dc, 0, 0, rt.right - rt.left, rt.bottom - rt.top);
	DeleteObject(brush);
}
