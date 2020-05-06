#include "CBulletScript.h"
#include "CScriptMgr.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CEventMgr.h"
#include "CCollider.h"
#include "CEventMgr.h"
#include "CRenderMgr.h"

void CBulletScript::SetDir(const Vec3 & _vDir)
{
	m_vDir = _vDir;
	TransForm()->SetLookAddRot(m_vDir);
}

CBulletScript::CBulletScript()
	:m_fSpeed(0.f)
	, m_bDeath(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CBULLETSCRIPT);
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::Start()
{
	m_vCreatePos = TransForm()->GetPos();
}

int CBulletScript::Update()
{
	if (m_bDeath)
		return 0;
	
	Vec3 vPos = TransForm()->GetPos();
	vPos += m_vDir * m_fSpeed * DT;
	TransForm()->SetPos(vPos);

	if (Vec3::Distance(m_vCreatePos, vPos) > 3000.f)
	{
		m_bDeath = true;
		CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL });
	}
	
	return 0;
}

void CBulletScript::CollisionEnter(CCollider * _pColl)
{
	if (_pColl->GetOwner()->GetLayerName() == L"Default" || _pColl->GetOwner()->GetLayerName() == L"Land")
	{
		CEventMgr::GetInst()->AddEvent(tEvent{EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL});
		m_bDeath = true;

		//CGameObj* pParent = new CGameObj;
		//pParent->SetName(L"BulletDecal");
		//
		//CGameObj* pDecal = CRenderMgr::GetInst()->FindFilter(L"Decal")->Clone();
		//pDecal->SetName(L"Decal");
		//Vec3 vDir = CPlayerMgr::GetInst()->GetMoveDir();
		//vDir.Normalize();

		//Vec3 vPlayer = GetParentOfOwner()->GetCollider()->GetWorldPos();
		//Vec3 vCollPos;
		//float fDist = 0.f;
		//if (_pColl->IntersectsRay(vPlayer, vDir, fDist))
		//	vCollPos = vPlayer + (vDir * (fDist + 15.f));
		//else
		//	return;

		////Vec3 vCollPos = Collider()->GetWorldPos() + (vDir * Collider()->GetRadius() * 0.5f); // 실제 부딪힌 위치를 계산한다.
		//vCollPos.y += 50.f;
		//pParent->GetTransForm()->SetPos(vCollPos);
		//pDecal->GetTransForm()->SetScale(Vec3(256.f * 0.5f, 512.f * 0.5f, 15.f));
		//Vec3 vRot;
		//if (GetParentOfOwner()->GetAnimator3D()->GetCurAnimKey(L"c_attack0"))
		//	vRot.z += XMConvertToRadians(35.f);
		//else if (GetParentOfOwner()->GetAnimator3D()->GetCurAnimKey(L"c_attack1"))
		//	vRot.z -= XMConvertToRadians(35.f);
		//else
		//{
		//	vRot.z = XMConvertToRadians(90.f);
		//}
		//pDecal->GetTransForm()->SetRotation(vRot);
		//pParent->GetTransForm()->SetRotation(GetParentOfOwner()->GetTransForm()->GetRotation());

		//int iLayer = CSceneMgr::GetInst()->GetLayerIndex(L"Default");
		//CEffectScript* pEffect = new CEffectScript;
		//pParent->AddComponent(pEffect);
		//pEffect->SetEffectType(EFFECT_TYPE::TIMEOUT);
		//pEffect->SetTime(3.f);

		//pParent->AddChild(pDecal);
		//CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJ,(INT_PTR)iLayer,(INT_PTR)pParent });
	}

	if (GetOwner()->GetTeamType() == TEAM_TYPE::MONSTER && _pColl->GetOwner()->GetLayerName() == L"Player")
	{
		CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL });
		m_bDeath = true;
	}
}