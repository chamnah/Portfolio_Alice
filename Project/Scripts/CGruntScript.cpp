#include "CGruntScript.h"
#include "CScriptMgr.h"
#include "CAnimator3D.h"
#include "CTransForm.h"
#include "CPlayerMgr.h"
#include "CResMgr.h"
#include "CEquipScript.h"
#include "TimeMgr.h"
#include "CEventMgr.h"
#include "KeyMgr.h"
#include "CBulletScript.h"
#include "CCollider.h"
#include "CItemScript.h"

CGruntScript::CGruntScript()
	:m_iHP(100)
	, m_fSpeed(100.f)
	, m_bAttack(false)
	, m_bCombo(false)
	, m_bSpawn(true)
	, m_eAttType(ATTACK_TYPE::NONE)
	, m_ePreType(ATTACK_TYPE::NONE)
	, m_fIdleTime(0.f)
	, m_pBullet(nullptr)
	, m_pAttack(nullptr)
	, m_bDamage(false)
{
	m_vPlayerDir = Vec3(1.f, 0.f, 0.f);
	SetScriptType((UINT)SCRIPT_TYPE::CGRUNTSCRIPT);
}

CGruntScript::~CGruntScript()
{
}

void CGruntScript::Start()
{
	Animator3D()->PlayAnim(L"spawn2", false);
	TransForm()->SetAddRot(Vec3(0.f, 180.f, 0.f));
}

int CGruntScript::Update()
{
	if (m_bSpawn)
	{
		if (Animator3D()->GetCurAnimKey(L"spawn2") && Animator3D()->GetFinish())
			m_bSpawn = false;
		return 0;
	}
	m_pPlayer = CPlayerMgr::GetInst()->GetPlayer();

	if (m_bDamage)
	{
		if (Animator3D()->GetCurAnimKey(L"damage_L") && Animator3D()->GetFinish())
			m_bDamage = false;
		else if (!GetOwner()->GetDead() && Animator3D()->GetCurAnimKey(L"attack_die3") && Animator3D()->GetFinish())
		{
			GetOwner()->IsDead();
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL });
		}

		return 0;
	}


	if (m_pPlayer != nullptr)
	{
		switch (m_eAttType)
		{
		case ATTACK_TYPE::NONE:
			Idle();
			break;
		case ATTACK_TYPE::TRACE:
			Trace();
			break;
		case ATTACK_TYPE::MELEE:
			MeleeAttack();
			break;
		case ATTACK_TYPE::RANGE:
			Range();
			break;
		case ATTACK_TYPE::QUAKE:
			break;
		case ATTACK_TYPE::FIREBALL:
			FireBall();
			break;
		default:
			break;
		}
	}
	return 0;
}

void CGruntScript::Idle()
{
	if (m_ePreType != m_eAttType)
	{
		m_fIdleTime = 2.f;
		m_ePreType = m_eAttType;
	}

	m_fIdleTime -= DT;

	if (m_fIdleTime <= 0.f)
	{
		srand(time(NULL));
		int iRand = (rand() % 2);
		if(iRand == 0)
			m_eAttType = ATTACK_TYPE::TRACE;
		else
			m_eAttType = ATTACK_TYPE::RANGE;
		m_fIdleTime = 0.f;
	}
}

void CGruntScript::Trace()
{
	Vec3 vPlayerPos = m_pPlayer->GetTransForm()->GetPos();
	Vec3 vMyPos = TransForm()->GetPos();

	float fDis = Vec3::Distance(vPlayerPos, vMyPos);

	if (fDis > 500.f)
		return;

	Vec3 vDir = vPlayerPos - vMyPos;
	vDir.Normalize();
	TransForm()->SetPos(vMyPos + (m_fSpeed * vDir * DT));
	TransForm()->SetLookAddRot(vDir);
	Animator3D()->PlayAnim(L"strafe_F", true);

	if (fDis < 200.f)
	{
		m_ePreType = m_eAttType;
		m_eAttType = ATTACK_TYPE::MELEE;
	}
}

void CGruntScript::MeleeAttack()
{
	if (m_ePreType != m_eAttType)
	{
		m_ePreType = m_eAttType;
		m_strCurAnim = L"attack_S1_melee_A";
		Animator3D()->PlayAnim(m_strCurAnim, false);
	}
	else
	{
		if (Animator3D()->GetFinish())
		{
			if (m_strCurAnim == L"attack_S1_melee_A")
			{
				m_strCurAnim = L"attack_S1_melee_C";
				Animator3D()->PlayAnim(m_strCurAnim, false);
				m_pAttack = CResMgr::GetInst()->Load<CPrefab>(L"Attack")->Instantiate();
				m_pAttack->SetTeamType(TEAM_TYPE::MONSTER);
				CEquipScript* pEquip = new CEquipScript;
				m_pAttack->AddComponent(pEquip);
				CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::ADD_CHILD,(INT_PTR)GetOwner(),(INT_PTR)m_pAttack });
				pEquip->SetEquipBone(L"Bone03", GetOwner());
				m_pAttack->GetCollider()->SetPos(Vec3(40.f,0.f,-10.f));
				m_pAttack->GetCollider()->SetRadius(80.f);
			}
			else if (m_strCurAnim == L"attack_S1_melee_C")
			{
				CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pAttack,NULL });
				m_strCurAnim = L"attack_S1_melee_D";
				Animator3D()->PlayAnim(m_strCurAnim, false);
			}
			else if (m_strCurAnim == L"attack_S1_melee_D")
			{
				m_strCurAnim = L"attack_Idle";
				Animator3D()->PlayAnim(m_strCurAnim, false);
				m_eAttType = ATTACK_TYPE::NONE;
			}
		}
	}
}

void CGruntScript::Range()
{
	if (m_ePreType != m_eAttType)
	{
		m_ePreType = m_eAttType;
		m_strCurAnim = L"attack_range_A";
		Animator3D()->PlayAnim(m_strCurAnim, false);
	}
	else
	{
		if (Animator3D()->GetFinish())
		{
			if (m_strCurAnim == L"attack_range_A")
			{
				m_strCurAnim = L"attack_range_B";
				Animator3D()->PlayAnim(m_strCurAnim, false);
			}
			else if (m_strCurAnim == L"attack_range_B")
			{
				m_pBullet->GetTransForm()->SetPos(m_pBullet->GetTransForm()->GetWorldPos());
				m_pBullet->DelComponent(COMPONENT_TYPE::SCRIPT);
				m_pBullet->SetParent(nullptr);
				m_pBullet->GetTransForm()->SetMtxOwner(Mtx::Identity);
				CBulletScript* pBullet = new CBulletScript;
				m_pBullet->AddComponent(pBullet);
				pBullet->Start();
				pBullet->SetSpeed(500.f);
				Vec3 vPlayer = m_pPlayer->GetTransForm()->GetPos();
				vPlayer.y += 100.f;
				Vec3 vDir = vPlayer - m_pBullet->GetTransForm()->GetPos();
				vDir.Normalize();
				pBullet->SetDir(vDir);
				m_pBullet = nullptr;

				m_strCurAnim = L"attack_range_C";
				Animator3D()->PlayAnim(m_strCurAnim, false);
			}
			else if (m_strCurAnim == L"attack_range_C")
			{
				m_strCurAnim = L"attack_range_D";
				Animator3D()->PlayAnim(m_strCurAnim, false);
			}
			else if (m_strCurAnim == L"attack_range_D")
			{
				m_strCurAnim = L"attack_Idle";
				Animator3D()->PlayAnim(m_strCurAnim, false);
				m_eAttType = ATTACK_TYPE::NONE;
			}
		}
		else if (m_strCurAnim == L"attack_range_A")
		{
			if(Animator3D()->GetCurFrm() >= 31 && m_pBullet == nullptr)
			{ 
				m_pBullet = CResMgr::GetInst()->Load<CPrefab>(L"OilBall")->CreateObjClone(L"Monster");
				m_pBullet->SetTeamType(TEAM_TYPE::MONSTER);
				m_pBullet->SetParent(GetOwner());

				m_pEquip = new CEquipScript;
				m_pBullet->AddComponent(m_pEquip);
				m_pEquip->SetEquipBone(L"Bone03", GetOwner());
			}
		}
	}
}

void CGruntScript::FireBall()
{
}

void CGruntScript::CollisionEnter(CCollider * _pColl)
{
	if (GetOwner()->GetDead())
		return;
	if (_pColl->GetOwner()->GetTeamType() == TEAM_TYPE::PLAYER)
	{
		Animator3D()->PlayAnim(L"damage_L",false,true);
		m_iHP -= 10;
		m_bDamage = true;
	}

	if (m_iHP <= 0 && !Animator3D()->GetCurAnimKey(L"attack_die3"))
	{
		m_bDamage = true;
		Animator3D()->PlayAnim(L"attack_die3", false);
		CGameObj* pBottle = CResMgr::GetInst()->Load<CPrefab>(L"Bottle")->CreateObjClone(L"Default");
		pBottle->GetTransForm()->SetPos(TransForm()->GetPos());
	 	CItemScript* pScript = new CItemScript;
		pScript->SetItemType(ITEM_TYPE::BOTTLE);
		pBottle->AddComponent(pScript);
		GetOwner()->DelComponent(COMPONENT_TYPE::COLLIDER);
		vector<CGameObj*>& vecChild = GetOwner()->GetChild();
		for (size_t i = 0; i < vecChild.size(); i++)
			vecChild[i]->DelComponent(COMPONENT_TYPE::COLLIDER);
	}
}