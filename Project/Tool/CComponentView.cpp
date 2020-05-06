// CComponentView.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CComponentView.h"
#include "CTransformDlg.h"
#include "CMeshRenderDlg.h"
#include "CSceneMgr.h"
#include "CGameObj.h"


// CComponentView

IMPLEMENT_DYNCREATE(CComponentView, CFormView)

CComponentView::CComponentView()
	:CFormView(IDD_COMPONENT)
{
}

CComponentView::~CComponentView()
{
}

BEGIN_MESSAGE_MAP(CComponentView, CFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CComponentView 그리기

void CComponentView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CComponentView 진단

#ifdef _DEBUG
void CComponentView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CComponentView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CComponentView 메시지 처리기

int CComponentView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_arrComDlg[(UINT)COMPONENT_TYPE::TRANSFORM] = new CTransformDlg(this);
	m_arrComDlg[(UINT)COMPONENT_TYPE::TRANSFORM]->Create(IDD_TRANSFORM,this);
	m_arrComDlg[(UINT)COMPONENT_TYPE::TRANSFORM]->ShowWindow(true);

	m_arrComDlg[(UINT)COMPONENT_TYPE::MESH_RENDER] = new CMeshRenderDlg(this);
	m_arrComDlg[(UINT)COMPONENT_TYPE::MESH_RENDER]->Create(IDD_MESHRENDER, this);
	m_arrComDlg[(UINT)COMPONENT_TYPE::MESH_RENDER]->ShowWindow(true);

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

void CComponentView::OnDestroy()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(nullptr != m_arrComDlg[i])
		m_arrComDlg[i]->DestroyWindow();
	}

	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CComponentView::Init()
{
	m_pTargetObj = CSceneMgr::GetInst()->FindObject(L"Player");

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComDlg[i])
			m_arrComDlg[i]->Init();
	}
}

void CComponentView::Update()
{
	if (nullptr == m_pTargetObj)
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr != m_arrComDlg[i])
			m_arrComDlg[i]->ShowWindow(false);
		}
		return;
	}

	DialogSort();

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComDlg[i])
		m_arrComDlg[i]->Update(m_pTargetObj);
	}
}

void CComponentView::DialogSort()
{
	CRect rt = {};
	CRect rtDlg = {};

	GetWindowRect(&rt);

	int   iPosY = 0;
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pCom = m_pTargetObj->GetComponent(COMPONENT_TYPE(i));

		if (m_arrComDlg[i] == nullptr)
			continue;

		if (pCom == nullptr)
		{
			m_arrComDlg[i]->ShowWindow(false);
			continue;
		}

		m_arrComDlg[i]->GetWindowRect(&rtDlg);
		m_arrComDlg[i]->SetWindowPos(nullptr,0, iPosY, rt.Width(), rtDlg.Height(),0);
		m_arrComDlg[i]->ShowWindow(true);
		
		iPosY += rtDlg.Height() - 1;
	}
}