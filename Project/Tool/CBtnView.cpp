// CBtnView.cpp: 구현 파일
//
#include "stdafx.h"
#include "Tool.h"
#include "CBtnView.h"
#include "CSceneMgr.h"

// CBtnView

IMPLEMENT_DYNCREATE(CBtnView, CFormView)

CBtnView::CBtnView()
	:CFormView(IDD_BTNVIEW)
{

}

CBtnView::~CBtnView()
{
}

BEGIN_MESSAGE_MAP(CBtnView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CBtnView::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_BUTTON2, &CBtnView::OnBnClickedPause)
END_MESSAGE_MAP()

// CBtnView 그리기

void CBtnView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}

// CBtnView 진단

#ifdef _DEBUG
void CBtnView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBtnView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CBtnView 메시지 처리기

void CBtnView::OnInitialUpdate()
{
	SetScrollSizes(MM_TEXT, CSize(0, 0));

	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void CBtnView::OnBnClickedPlay()
{
	CSceneMgr::GetInst()->SetPlay(true);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CBtnView::OnBnClickedPause()
{
	CSceneMgr::GetInst()->SetPlay(false);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}