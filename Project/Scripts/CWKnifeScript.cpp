#include "CWKnifeScript.h"
#include "CScriptMgr.h"
#include "CPlayerMgr.h"
#include "CGameObj.h"
#include "CMeshRender.h"
#include "CMesh.h"
#include "CAnimator3D.h"
#include "CTransForm.h"
#include "CRenderMgr.h"
#include "CFilterCom.h"
#include "CEventMgr.h"
#include "CSceneMgr.h"
#include "CEffectScript.h"
#include "CCollider.h"

int CWKnifeScript::Update()
{
	CGameObj* pPlayer = CPlayerMgr::GetInst()->GetPlayer();

	if (pPlayer == nullptr)
		 return 0;

	int iFrm = pPlayer->GetAnimator3D()->GetCurFrm();
	if (pPlayer->GetAnimator3D()->GetCurAnimKey(L"c_attack0"))
	{
		if (m_pColl != nullptr && iFrm >= 10 && iFrm < 14)
		{
			GetOwner()->SetComponent(m_pColl, COMPONENT_TYPE::COLLIDER);
			m_pColl = nullptr;
		}
		else if (m_pColl == nullptr && iFrm >= 14)
			SetColl();
	}
	else if(pPlayer->GetAnimator3D()->GetCurAnimKey(L"c_attack1"))
	{
		if (m_pColl != nullptr && iFrm >= 11 && iFrm < 15)
		{
			GetOwner()->SetComponent(m_pColl, COMPONENT_TYPE::COLLIDER);
			m_pColl = nullptr;
		}
		else if (m_pColl == nullptr && iFrm >= 15)
			SetColl();
	}
	else if (pPlayer->GetAnimator3D()->GetCurAnimKey(L"c_attack2"))
	{
		if (m_pColl != nullptr && iFrm >= 6 && iFrm < 9)
		{
			GetOwner()->SetComponent(m_pColl, COMPONENT_TYPE::COLLIDER);
			m_pColl = nullptr;
		}
		else if (m_pColl == nullptr && iFrm >= 9 && iFrm < 18)
			SetColl();
		else if (m_pColl != nullptr && iFrm >= 18 && iFrm < 28)
		{
			GetOwner()->SetComponent(m_pColl, COMPONENT_TYPE::COLLIDER);
			m_pColl = nullptr;
		}
		else if (m_pColl == nullptr && iFrm >= 28)
			SetColl();
	}
	else if (pPlayer->GetAnimator3D()->GetCurAnimKey(L"c_attack2_end"))
	{
		if (m_pColl == nullptr)
			SetColl();
	}
	else if (pPlayer->GetAnimator3D()->GetCurAnimKey(L"c_attack3"))
	{
		if (m_pColl != nullptr && iFrm >= 14 && iFrm < 18)
		{
			GetOwner()->SetComponent(m_pColl, COMPONENT_TYPE::COLLIDER);
			m_pColl = nullptr;
		}
		else if (m_pColl == nullptr && iFrm >= 18)
			SetColl();
	}
	else if (pPlayer->GetAnimator3D()->GetCurAnimKey(L"c_attack4"))
	{
		if (m_pColl != nullptr && iFrm >= 11 && iFrm < 15)
		{
			GetOwner()->SetComponent(m_pColl, COMPONENT_TYPE::COLLIDER);
			m_pColl = nullptr;
		}
		else if (m_pColl == nullptr && iFrm >= 15)
			SetColl();
	}

	 if (m_bOn == false)
	 {
		 m_bOn = true;
		 vector<tMtxBone>* vecBone = pPlayer->GetMeshRender()->GetMesh()->GetBone();
		 for (size_t i = 0; i < vecBone->size(); i++)
		 {
			 wstring strName = vecBone->at(i).strName;
			 //Bip01-R-Finger4
			 if (strName == L"Bip01-Prop1")
			 {
				 m_iBoneIndex = i;
			 }
		 }

		 Vec3 vRot;
		 vRot.x = XMConvertToRadians(180.f);
		 vRot.y = XMConvertToRadians(90.f);
		 TransForm()->SetRotation(vRot);
		 /*Vec3 vPos = TransForm()->GetPos();
		 vPos.z = 3.f;
		 TransForm()->SetPos(vPos);*/
	 }
	 else
	 {
		GetOwner()->SetParent(pPlayer);
		Mtx mtxBone = pPlayer->GetAnimator3D()->GetWorldBoneMtx()->at(m_iBoneIndex);
		TransForm()->SetMtxOwner(mtxBone);
	 }

	 return 0;
}

int CWKnifeScript::FinalUpdate()
{
	return 0;
}

void CWKnifeScript::SetColl()
{
	m_pColl = ((CCollider*)GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER));
	GetOwner()->SetComponent(nullptr, COMPONENT_TYPE::COLLIDER);
}

void CWKnifeScript::CollisionEnter(CCollider * _pColl)
{
	if (_pColl->GetOwner()->GetLayerName() != L"Default")
		return;

	CGameObj* pParent = new CGameObj;
	pParent->SetName(L"DecalParent");

	CGameObj* pDecal = CRenderMgr::GetInst()->FindFilter(L"Decal")->Clone();
	pDecal->SetName(L"Decal");
	Vec3 vDir = CPlayerMgr::GetInst()->GetMoveDir();
	vDir.Normalize();

	Vec3 vPlayer = GetParentOfOwner()->GetCollider()->GetWorldPos();
	Vec3 vCollPos;
	float fDist = 0.f;
	if (_pColl->IntersectsRay(vPlayer, vDir, fDist))
		vCollPos = vPlayer + (vDir * (fDist + 15.f));
	else
		return;

	//Vec3 vCollPos = Collider()->GetWorldPos() + (vDir * Collider()->GetRadius() * 0.5f); // 실제 부딪힌 위치를 계산한다.
	vCollPos.y += 50.f;
	pParent->GetTransForm()->SetPos(vCollPos);
	pDecal->GetTransForm()->SetScale(Vec3(256.f * 0.5f, 512.f * 0.5f,15.f));
	Vec3 vRot;
	if (GetParentOfOwner()->GetAnimator3D()->GetCurAnimKey(L"c_attack0"))
		vRot.z += XMConvertToRadians(35.f);
	else if (GetParentOfOwner()->GetAnimator3D()->GetCurAnimKey(L"c_attack1"))
		vRot.z -= XMConvertToRadians(35.f);
	else
	{
		vRot.z = XMConvertToRadians(90.f);
	}
	pDecal->GetTransForm()->SetRotation(vRot);
	pParent->GetTransForm()->SetRotation(GetParentOfOwner()->GetTransForm()->GetRotation());
	
	int iLayer = CSceneMgr::GetInst()->GetLayerIndex(L"Default");
	CEffectScript* pEffect = new CEffectScript;
	pParent->AddComponent(pEffect);
	pEffect->SetEffectType(EFFECT_TYPE::TIMEOUT);
	pEffect->SetTime(3.f);

	pParent->AddChild(pDecal);
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJ,(INT_PTR)iLayer,(INT_PTR)pParent });
}

CWKnifeScript::CWKnifeScript()
	:m_bOn(false)
	, m_iBoneIndex(0)
	, m_pColl(nullptr)
{
	SetScriptType((UINT)SCRIPT_TYPE::CWKNIFESCRIPT);
}

CWKnifeScript::~CWKnifeScript()
{
	SAFE_DELETE(m_pColl);
}