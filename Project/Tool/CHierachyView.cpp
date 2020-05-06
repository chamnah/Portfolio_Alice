// CHierachyView.cpp: 구현 파일
//
#include "stdafx.h"
#include "Tool.h"
#include "CHierachyView.h"
#include "CHierarchyDlg.h"
#include "CResDlg.h"
#include "CEventMgr.h"
#include "CGameObj.h"
#include "KeyMgr.h"
// CHierachyView
IMPLEMENT_DYNAMIC(CHierachyView, CDockablePane)

CHierachyView::CHierachyView()
{
}

CHierachyView::~CHierachyView()
{
}

BEGIN_MESSAGE_MAP(CHierachyView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()
// CHierachyView 메시지 처리기

int CHierachyView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pHierarchy = new CHierarchyDlg;
	m_pHierarchy->Create(IDD_HIERARCHY,this);
	m_pHierarchy->ShowWindow(true);

	m_pRes = new CResDlg;
	m_pRes->Create(IDD_RESDLG, this);
	m_pRes->ShowWindow(true);
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	return 0;
}

void CHierachyView::Init()
{
	m_pHierarchy->Init();
	m_pRes->Init();
}

void CHierachyView::Update()
{
	if (CEventMgr::GetInst()->IsReset())
	{
		CEventMgr::GetInst()->SetReset(false);
		m_pHierarchy->Reset();
		m_pRes->Reset();
	}

	if(KEYTAB(KEY_TYPE::KEY_ENTER))
		m_pHierarchy->Reset();
}

void CHierachyView::OnDestroy()
{
	CDockView::OnDestroy();

	m_pHierarchy->DestroyWindow();
	m_pRes->DestroyWindow();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CHierachyView::OnSize(UINT nType, int cx, int cy)
{
	CDockView::OnSize(nType, cx, cy);

	m_pHierarchy->SetWindowPos(nullptr, 1, 0, cx - 1, cy / 2, 0);
	m_pRes->SetWindowPos(nullptr, 1, cy / 2 - 1, cx - 1, cy / 2 - 1, 0);
}