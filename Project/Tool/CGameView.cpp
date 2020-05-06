// CGameView.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CGameView.h"
#include "CTestMgr.h"
#include "MainFrm.h"
#include "CComponentView.h"
#include "CGameObj.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "CTransForm.h"
#include "CRenderMgr.h"
#include "CCamera.h"
//#include "CToolScript.h"

// CGameView
IMPLEMENT_DYNCREATE(CGameView, CFormView)

CGameView::CGameView()
	:CFormView(IDD_GAME)
{

}

CGameView::~CGameView()
{
}

BEGIN_MESSAGE_MAP(CGameView, CFormView)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CGameView 그리기
void CGameView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}
// CGameView 진단

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CGameView 메시지 처리기

void CGameView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	
	//CGameObj* pTarget = nullptr;//((CToolScript*)CTestMgr::GetInst()->GetMouseObj()->GetScript()[0])->GetTargetObj();

	CFormView::OnLButtonDown(nFlags, point);
}

void CGameView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	UINT iCount = CSceneMgr::GetInst()->GetCurLayerCount();
	tRay Ray = CRenderMgr::GetInst()->GetMainCam()->GetRay();
	float fDist = 0.f;
	float fCompare = 0.f;
	bool bFirst = true;
	BoundingBox Box;
	CGameObj* pTargetObj = nullptr;

	UINT iContinue = CSceneMgr::GetInst()->GetLayerIndex(L"Camera");

	for (UINT i = 0; i < iCount; i++)
	{
		if (i == iContinue)
			continue;

		CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(i);
		vector<CGameObj*>& vecObj = pLayer->GetAllObj();
		for (size_t i = 0; i < vecObj.size(); i++)
		{
			if (vecObj[i]->GetFrustum())
			{
				Box.Center = vecObj[i]->GetTransForm()->GetWorldMtx().Translation();
				Box.Extents = vecObj[i]->GetTransForm()->GetScale();

				if (Box.Intersects(Ray.vStart, Ray.vDir, fDist))
				{
					if (bFirst) // 처음 피킹된 경우
					{
						pTargetObj = vecObj[i];
						bFirst = false;
						fCompare = fDist;
					}
					else if (fCompare < fDist) // 처음이 아닌경우고 거리가 더 짧은 경우
					{
						fCompare = fDist;
						pTargetObj = vecObj[i];
					}
				}
			}
		}
	}

	if (pTargetObj == nullptr)
		return;

	CTestMgr::GetInst()->SetTargetObj(pTargetObj);
	((CMainFrame*)AfxGetMainWnd())->GetView(VIEW_TYPE::COMPONENT)->SetTargetObj(pTargetObj);

	CFormView::OnLButtonDblClk(nFlags, point);
}


void CGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnLButtonDown(nFlags, point);
}


void CGameView::OnKillFocus(CWnd* pNewWnd)
{
	CFormView::OnKillFocus(pNewWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CGameView::OnSetFocus(CWnd* pOldWnd)
{
	CFormView::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
