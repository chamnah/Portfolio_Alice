#include "CGruntMiniScript.h"
#include "CAnimator3D.h"
#include "CScriptMgr.h"
#include "CMeshRender.h"
#include "CEventMgr.h"
#include "KeyMgr.h"
#include "CPlayerMgr.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CResMgr.h"
#include "CEquipScript.h"
#include "KeyMgr.h"
#include "CCollider.h"
#include "CItemScript.h"

CGruntMiniScript::CGruntMiniScript()
	:m_iHP(4)
	, m_fSpeed(100.f)
	, m_bAttack(false)
	, m_bCombo(false)
	, m_bSpawn(true)
	, m_pAttObj(nullptr)
	, m_bDamage(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CGRUNTMINISCRIPT);
	m_vRight = Vec3(0.f, 1.f, 0.f);
	m_vPlayerDir = Vec3(1.f, 0.f, 0.f);
}

CGruntMiniScript::~CGruntMiniScript()
{
}

void CGruntMiniScript::Start()
{
	Animator3D()->PlayAnim(L"spawn2", false);
}

int CGruntMiniScript::Update()
{
	if (m_bSpawn)
	{
		if (Animator3D()->GetCurAnimKey(L"spawn2") && Animator3D()->GetFinish())
			m_bSpawn = false;
		return 0;
	}

	if (GetOwner()->GetDead())
		return 0;
	
	CGameObj* pPlayer = CPlayerMgr::GetInst()->GetPlayer();
	if (pPlayer != nullptr)
	{
		Vec3 vPlayerPos = pPlayer->GetTransForm()->GetPos();
		Vec3 vMyPos = TransForm()->GetPos();

		float fDis = Vec3::Distance(vPlayerPos, vMyPos);
		
		//CATRigLArmPalm
		//CATRigRArmPalm

		if (m_bDamage)
		{
			if(Animator3D()->GetCurAnimKey(L"damage_L3") && Animator3D()->GetFinish())
				m_bDamage = false;
			else if (!GetOwner()->GetDead() && Animator3D()->GetCurAnimKey(L"die2") && Animator3D()->GetFinish())
			{
				GetOwner()->IsDead();
				CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL });
			}
			
			return 0;
		}
		if (m_bAttack)
		{
			if (Animator3D()->GetCurAnimKey(m_strAttack + L"_End") && Animator3D()->GetFinish())
				m_bAttack = false;
			else if (Animator3D()->GetFinish())
			{
				Animator3D()->PlayAnim(m_strAttack + L"_End", false);
				CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pAttObj,NULL });
				m_pAttObj = nullptr;
			}

			return 0;
		}

		if (fDis < 150.f)
		{
			m_bAttack = true;
			m_pAttObj = CResMgr::GetInst()->Load<CPrefab>(L"Attack")->Instantiate();
			m_pAttObj->SetTeamType(TEAM_TYPE::MONSTER);
			CEquipScript* pEquip = new CEquipScript;
			m_pAttObj->AddComponent(pEquip);
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::ADD_CHILD,(INT_PTR)GetOwner(),(INT_PTR)m_pAttObj });
			if (m_bCombo)
			{
				pEquip->SetEquipBone(L"CATRigLArmPalm", GetOwner());
				m_strAttack = L"attack02_B";
				m_bCombo = false;
			}
			else
			{
				pEquip->SetEquipBone(L"CATRigRArmPalm", GetOwner());
				m_strAttack = L"attack02_A";
				m_bCombo = true;
			}
			Animator3D()->PlayAnim(m_strAttack, false);
		}
		else 
		{
			m_bCombo = false;
			if (fDis < 300.f)
			{
				Vec3 vDir = vPlayerPos - vMyPos;
				vDir.Normalize();
				TransForm()->SetPos(vMyPos + (m_fSpeed * vDir * DT));
				TransForm()->SetLookAddRot(vDir);

				Animator3D()->PlayAnim(L"strafe3_F", true);
			}
			else
				Animator3D()->PlayAnim(L"attack_Idle", true);
		}
	}

	return 0;
}

void CGruntMiniScript::CollisionEnter(CCollider * _pColl)
{
	if (_pColl->GetOwner()->GetTeamType() == TEAM_TYPE::PLAYER)
	{
		Animator3D()->PlayAnim(L"damage_L3",false);
		m_iHP -= 2;
		m_bDamage = true;
	}
	if (!GetOwner()->GetDead() && m_iHP <= 0 && !Animator3D()->GetCurAnimKey(L"die2"))
	{
		m_bDamage = true;
		GetOwner()->DelComponent(COMPONENT_TYPE::COLLIDER);
		vector<CGameObj*>& vecChild = GetOwner()->GetChild();
		for (size_t i = 0; i < vecChild.size(); i++)
			vecChild[i]->DelComponent(COMPONENT_TYPE::COLLIDER);
		Animator3D()->PlayAnim(L"die2",false);
		CGameObj* pTooth = CResMgr::GetInst()->Load<CPrefab>(L"Tooth")->CreateObjClone(L"Default");
		pTooth->GetTransForm()->SetPos(TransForm()->GetPos());
		pTooth->AddComponent(new CItemScript);
	}
}