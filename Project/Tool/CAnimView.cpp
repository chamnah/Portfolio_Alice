// CAnimView.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CAnimView.h"
#include "CAnimationDlg.h"
#include "CSceneMgr.h"
#include "CGameObj.h"
#include "CAnimator2D.h"
#include "CSaveLoadMgr.h"
#include "CTransForm.h"
#include "KeyMgr.h"
// CAnimView

IMPLEMENT_DYNAMIC(CAnimView, CDockablePane)

CAnimView::CAnimView()
{
}

CAnimView::~CAnimView()
{
}

BEGIN_MESSAGE_MAP(CAnimView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()
// CAnimView 메시지 처리기

int CAnimView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Anim = new CAnimationDlg(this);
	m_Anim->Create(IDD_ANIMATION, this);
	m_Anim->ShowWindow(true);

	CRect rt,rt2;

	m_Anim->GetClientRect(&rt);
	GetClientRect(&rt2);
	SetWindowPos(nullptr, rt2.left,rt2.top, rt2.Width(),rt.Height(),0);
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

void CAnimView::Init()
{
	//m_pTargetObj = CSceneMgr::GetInst()->FindObject(L"Player");
}

void CAnimView::Update()
{
	if (m_pTargetObj != nullptr && m_pTargetObj->GetDead())
		m_pTargetObj = nullptr;
	
	return;
	//CSaveLoadMgr::LoadingReset(m_pTargetObj);

	if (nullptr == m_pTargetObj)
		return;

	if (m_pTargetObj->GetCamera() != nullptr)
		return;

	if (nullptr != m_pTargetObj->GetAnimator2D() && nullptr != m_pTargetObj->GetMeshRender())
	{
		if (m_Anim->GetCheck())
		{
			Vec2 vPos = m_pTargetObj->GetAnimator2D()->GetOffset();

			if (KEYTAB(KEY_TYPE::KEY_LEFT))
			{
				vPos.x -= 0.005f;
				m_pTargetObj->GetAnimator2D()->SetOffset(vPos);
			}
			else if (KEYTAB(KEY_TYPE::KEY_RIGHT))
			{
				vPos.x += 0.005f;
				m_pTargetObj->GetAnimator2D()->SetOffset(vPos);
			}
			else if (KEYTAB(KEY_TYPE::KEY_UP))
			{
				vPos.y += 0.005f;
				m_pTargetObj->GetAnimator2D()->SetOffset(vPos);
			}
			else if (KEYTAB(KEY_TYPE::KEY_DOWN))
			{
				vPos.y -= 0.005f;
				m_pTargetObj->GetAnimator2D()->SetOffset(vPos);
			}
		}
	}
	else
	{
		if (!m_Anim->GetCheck())
		{
			Vec3 vPos = m_pTargetObj->GetTransForm()->GetPos();

			if (KEYHOLD(KEY_TYPE::KEY_CTRL))
			{
				if (KEYTAB(KEY_TYPE::KEY_LEFT))
				{
					vPos.x -= 1.f;
				}
				else if (KEYTAB(KEY_TYPE::KEY_RIGHT))
				{
					vPos.x += 1.f;
				}
				else if (KEYTAB(KEY_TYPE::KEY_UP))
				{
					vPos.y += 1.f;
				}
				else if (KEYTAB(KEY_TYPE::KEY_DOWN))
				{
					vPos.y -= 1.f;
				}
			}
			m_pTargetObj->GetTransForm()->SetPos(vPos);
		}
		return;
	}
	
	m_Anim->Update(m_pTargetObj);
}