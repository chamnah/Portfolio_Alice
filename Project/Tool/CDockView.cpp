#include "stdafx.h"
#include "CDockView.h"

CDockView::CDockView()
: m_pTargetObj(nullptr)

{
}

CDockView::~CDockView()
{
}

void CDockView::PostNcDestroy()
{
	CDockablePane::PostNcDestroy();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}BEGIN_MESSAGE_MAP(CDockView, CDockablePane)
ON_WM_PAINT()
ON_WM_SIZE()
END_MESSAGE_MAP()


void CDockView::OnPaint()
{
	CPaintDC dc(this);

	CRect rt;
	GetWindowRect(&rt);
	rt.left = 0;
	rt.top = 0;
	dc.FillSolidRect(&rt, RGB(240, 240, 240));
}


void CDockView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	Invalidate();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
